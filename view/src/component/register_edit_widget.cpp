#include "register_edit_widget.hpp"
#include "network_client_helper.hpp"
#include <QLabel>
#include <QCheckBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QJsonObject>
#include <QMessageBox>

const int MAX_BIT_NUM = 32;

RegisterEditWidget::RegisterEditWidget(QWidget *parent) : QWidget(parent)
{
    initUI();

    shared_ptr<NetworkClientHelper> network_helper = NetworkClientHelper::getInstance();
    connect(network_helper.get(), &NetworkClientHelper::sigReceiveRegMsg, this,
            &RegisterEditWidget::slotUpdateReg);
}

void RegisterEditWidget::initUI()
{
    main_layout = new QHBoxLayout(this);
    this->setMaximumWidth(1200);

    QGridLayout *edit_layout = new QGridLayout;
    QLabel *addr_label = new QLabel(tr("Address:"));
    addr_line = new CvLineEdit();
    addr_line->setText("0x00000000");
    addr_line->setMaximumWidth(200);
    QLabel *value_label = new QLabel(tr("Value:"));
    value_line = new CvLineEdit();
    value_line->setText("0x00000000");
    value_line->setMaximumWidth(200);
    QPushButton *read_btn = new QPushButton(tr("Read"));
    QPushButton *write_btn = new QPushButton(tr("Write"));

    edit_layout->addWidget(addr_label, 0, 0);
    edit_layout->addWidget(addr_line, 0, 1);
    edit_layout->addWidget(read_btn, 0, 2);
    edit_layout->addWidget(value_label, 1, 0);
    edit_layout->addWidget(value_line, 1, 1);
    edit_layout->addWidget(write_btn, 1, 2);

    connect(addr_line, &CvLineEdit::sigLostFocus, this, &RegisterEditWidget::addrChanged);

    connect(value_line, &CvLineEdit::textChanged, this, &RegisterEditWidget::slotValueChanged);
    connect(value_line, &CvLineEdit::sigLostFocus, [=] (QString text) {
        bool ok;
        unsigned int value;
        if (text.startsWith("0x", Qt::CaseInsensitive)) {
            value = text.toUInt(&ok, 16);
        } else {
            value = text.toUInt(&ok, 10);
        }
        if (!ok) value = 0;
        value_line->setText(QString("0x%1").arg(value, 8, 16, QLatin1Char('0')).toUpper());
    });

    connect(read_btn, &QPushButton::clicked, this, &RegisterEditWidget::readReg);
    connect(write_btn, &QPushButton::clicked, this, &RegisterEditWidget::writeReg);

    /*checkbox*/
    QGridLayout *checkbox_layout = new QGridLayout;
    for (int i = 0; i < MAX_BIT_NUM; i++) {
        QLabel *title = new QLabel();
        title->setText(QString::number(MAX_BIT_NUM - i - 1));
        QCheckBox *checkbox = new QCheckBox();

        checkbox_layout->addWidget(title, 0, i);
        checkbox_layout->addWidget(checkbox, 1, i);
        checkbox_list.append(checkbox);
        connect(checkbox, &QCheckBox::clicked,
                this, &RegisterEditWidget::checkboxChecked);
    }

    main_layout->addLayout(edit_layout, 1);
    main_layout->addLayout(checkbox_layout, 2);
}

void RegisterEditWidget::checkboxChecked()
{
    unsigned int value = 0;
    for (int i = 0; i < MAX_BIT_NUM; i++) {
        value = value | (checkbox_list[MAX_BIT_NUM - 1 - i]->isChecked() << i);
    }

    value_line->setText(QString("0x%1").arg(value, 8, 16, QLatin1Char('0')).toUpper());
}

void RegisterEditWidget::slotValueChanged(QString text)
{
    QString input = text.trimmed();
    unsigned int value = 0;
    bool ok;
    if (input.startsWith("0x", Qt::CaseInsensitive)) {
        value = text.toUInt(&ok, 16);
    } else {
        value = text.toUInt(&ok, 10);
    }

    if (!ok)
    {
        value = 0;
    }

    qDebug("value:%d", value);
    for (int i = 0; i < MAX_BIT_NUM; i++)
    {
        if ((value & 0x1) == 0x1) {
            checkbox_list[MAX_BIT_NUM - 1 - i]->setCheckState(Qt::CheckState::Checked);
        } else {
            checkbox_list[MAX_BIT_NUM - 1 - i]->setCheckState(Qt::CheckState::Unchecked);
        }
        value = value >> 1;
    }
}

void RegisterEditWidget::addrChanged(QString text)
{
    bool ok;
    unsigned int value;
    if (text.startsWith("0x", Qt::CaseInsensitive)) {
        value = text.toUInt(&ok, 16);
    } else {
        value = text.toUInt(&ok, 10);
    }
    if (!ok) value =0;
    addr_line->setText(QString("0x%1").arg(value, 8, 16, QLatin1Char('0')).toUpper());
}


void RegisterEditWidget::readReg()
{
    if (NetworkClientHelper::getInstance()->isNormalStatus()) {
        QJsonObject send_obj;
        QJsonObject data_obj;
        send_obj[MSG_KEY_CMD] = CMD_READ_REG;
        data_obj["reg"] = addr_line->text();
        send_obj[MSG_KEY_DATA] = data_obj;
        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
    } else {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
    }
}

void RegisterEditWidget::writeReg()
{
    if (NetworkClientHelper::getInstance()->isNormalStatus()) {
        QJsonObject send_obj;
        QJsonObject data_obj;
        send_obj[MSG_KEY_CMD] = CMD_WRITE_REG;
        data_obj["reg"] = addr_line->text();
        data_obj["value"] = value_line->text();
        send_obj[MSG_KEY_DATA] = data_obj;
        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
    } else {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
    }
}

void RegisterEditWidget::slotUpdateReg(QString cmd)
{
    QByteArray data = NetworkClientHelper::getInstance()->popCmdData(cmd);
}
