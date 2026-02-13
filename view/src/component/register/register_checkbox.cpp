#include "register_checkbox.hpp"
#include "global_data.hpp"
#include "utils.hpp"

#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QtCore>

const int MAX_BIT_NUM = 32;

RegisterCheckBox::RegisterCheckBox(QWidget *parent) : QWidget(parent)
{
    initUI();
    retranslateUI();
}

void RegisterCheckBox::initUI()
{
    QVBoxLayout *bit_layout = new QVBoxLayout(this);
    bit_layout->setAlignment(Qt::AlignTop);
    bit_layout->setMargin(0);

    QGridLayout *display_layout = new QGridLayout;
    addr = new QLabel();
    bitmask = new QLabel();
    value = new QLabel();
    selected_address_label = new QLabel();
    selected_bitmask_label = new QLabel();
    selected_value_label = new QLabel();

    display_layout->addWidget(addr, 0, 0);
    display_layout->addWidget(bitmask, 0, 1);
    display_layout->addWidget(value, 0, 2);
    display_layout->addWidget(selected_address_label, 1, 0);
    display_layout->addWidget(selected_bitmask_label, 1, 1);
    display_layout->addWidget(selected_value_label, 1, 2);
    display_layout->setColumnStretch(0, 1);
    display_layout->setColumnStretch(1, 1);
    display_layout->setColumnStretch(2, 1);

    QGridLayout *checkbox_layout = new QGridLayout;

    for (int i = 0; i < MAX_BIT_NUM; i++) {
        QLabel *title = new QLabel();
        title->setText(QString::number(MAX_BIT_NUM - i));
        QCheckBox *checkbox = new QCheckBox();

        checkbox_layout->addWidget(title, 0, i);
        checkbox_layout->addWidget(checkbox, 1, i);
        checkbox_list.append(checkbox);
        connect(checkbox, &QCheckBox::clicked,
                this, &RegisterCheckBox::checkboxChecked);
    }

    bit_layout->addLayout(display_layout);
    bit_layout->addLayout(checkbox_layout);
}

void RegisterCheckBox::setAddressText(QString address)
{
    selected_address_label->setText(address);
}

void RegisterCheckBox::setBitmask(QString bitmask)
{
    selected_bitmask_label->setText(bitmask);
}

void RegisterCheckBox::refreshData(const SecKeyType&)
{
    if (reg_section_key_list.isEmpty()) return;

    SecKeyType reg_section_key = reg_section_key_list.first();
    QVariant value;
    value = GlobalData::getInstance()->getRegValue(reg_section_key.first, reg_section_key.second);

    unsigned int u32_value = utils::toU32MaskValue(value.toUInt(), this->selected_bitmask_label->text().replace("0x", ""));
    selected_value_label->setText(utils::toHex(u32_value, 8));
    updateCheckbox(value.toUInt());
}

void RegisterCheckBox::updateCheckbox(unsigned int value)
{
    for (int i = 0; i < MAX_BIT_NUM; i++) {
        bool bit_checked = (value >> i) & 1;

        checkbox_list[MAX_BIT_NUM - 1 - i]->setChecked(bit_checked);
    }
}

void RegisterCheckBox::changeEvent(QEvent *event)
{
    switch(event->type()) {
    case QEvent::LanguageChange:
        retranslateUI();
        break;
    default:
        break;
    }
    QWidget::changeEvent(event);
}

void RegisterCheckBox::retranslateUI()
{
    addr->setText(tr("Address"));
    bitmask->setText(tr("Bitmask"));
    value->setText(tr("Value"));
}

void RegisterCheckBox::checkboxChecked()
{
    if (reg_section_key_list.isEmpty()) return;

    SecKeyType reg_section_key = reg_section_key_list.first();
    unsigned int value = 0;
    for (int i = 0; i < MAX_BIT_NUM; i++) {
        value = value | (checkbox_list[MAX_BIT_NUM - 1 - i]->isChecked() << i);
    }

    GlobalData::getInstance()->setRegValue(reg_section_key.first, reg_section_key.second, value);
}
