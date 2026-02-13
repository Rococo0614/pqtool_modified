#include "yuv_image_format_dialog.hpp"
#include "utils.hpp"
#include <QFileDialog>

const QString DEFAULT_WIDTH = "1920";
const QString DEFAULT_HEIGHT = "1080";
const int ITEM_WIDTH = 120;

YuvImageFormatDialog::YuvImageFormatDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Yuv Format Dialog"));
    setWindowFlags(Qt::WindowCloseButtonHint);
    this->setFixedSize(580, 200);

    initUI();
    retranslateUi();
}

void YuvImageFormatDialog::initUI()
{
    main_layout = new QVBoxLayout(this);

    QHBoxLayout *path_layout = new QHBoxLayout();
    filepath_label = new QLabel;
    filepath_edit = new QLineEdit;
    filepath_edit->setFixedWidth(450);
    filepath_edit->setReadOnly(true);
    path_layout->addWidget(filepath_label);
    path_layout->addWidget(filepath_edit);

    width_label = new QLabel;
    width_label->setFixedWidth(ITEM_WIDTH);
    width_line_edit = new QLineEdit(DEFAULT_WIDTH);
    width_line_edit->setFixedWidth(ITEM_WIDTH);
    width_line_edit->setMaxLength(6);
    connect(width_line_edit, &QLineEdit::editingFinished, [=]{
        unsigned int w = width_line_edit->text().toUInt();
        w = qBound((unsigned int)64, w, (unsigned int)999999);
        width_line_edit->setText(QString::number(w));
    });

    height_label = new QLabel;
    height_label->setFixedWidth(ITEM_WIDTH);
    height_line_edit = new QLineEdit(DEFAULT_HEIGHT);
    height_line_edit->setFixedWidth(ITEM_WIDTH);
    height_line_edit->setMaxLength(6);
    connect(height_line_edit, &QLineEdit::editingFinished, [=]{
        unsigned int h = height_line_edit->text().toInt();
        h = qBound((unsigned int)32, h, (unsigned int)999999);
        height_line_edit->setText(QString::number(h));
    });

    format_label = new QLabel;
    format_label->setFixedWidth(ITEM_WIDTH);
    format_combobox = new QComboBox;
    format_combobox->setFixedWidth(ITEM_WIDTH);
    QStringList formatstr = {"YUV420", "NV21", "NV12"};
    format_combobox->addItems(formatstr);
    format_combobox->setCurrentIndex(0);

    align_label = new QLabel;
    align_label->setFixedWidth(ITEM_WIDTH);
    align_line_edit = new QLineEdit("1");
    align_line_edit->setFixedWidth(ITEM_WIDTH);
    align_line_edit->setMaxLength(6);

    connect(align_line_edit, &QLineEdit::editingFinished, [=]{
        unsigned int h = align_line_edit->text().toInt();
        h = qBound((unsigned int)1, h, (unsigned int)64);
        align_line_edit->setText(QString::number(h));
    });

    QGroupBox *option_group = new QGroupBox("Option");
    QGridLayout *option_layout = new QGridLayout;
    option_layout->addWidget(width_label, 0, 0, Qt::AlignLeft);
    option_layout->addWidget(width_line_edit, 0, 1, Qt::AlignLeft);
    option_layout->addWidget(height_label, 0, 2, Qt::AlignLeft);
    option_layout->addWidget(height_line_edit, 0, 3, Qt::AlignLeft);
    option_layout->addWidget(format_label, 1, 0, Qt::AlignLeft);
    option_layout->addWidget(format_combobox, 1, 1, Qt::AlignLeft);
    option_layout->addWidget(align_label, 1, 2, Qt::AlignLeft);
    option_layout->addWidget(align_line_edit, 1, 3, Qt::AlignLeft);
    option_group->setLayout(option_layout);

    QHBoxLayout *select_layout = new QHBoxLayout();
    ok_btn = new QPushButton;
    ok_btn->setFixedWidth(ITEM_WIDTH);
    cancel_btn = new QPushButton;
    cancel_btn->setFixedWidth(ITEM_WIDTH);
    select_layout->addStretch();
    select_layout->addWidget(ok_btn);
    select_layout->addWidget(cancel_btn);

    main_layout->addLayout(path_layout);
    main_layout->addWidget(option_group);
    main_layout->addLayout(select_layout);

    connect(ok_btn, &QPushButton::clicked, this, &YuvImageFormatDialog::handleOk);
    connect(cancel_btn, &QPushButton::clicked, this, &YuvImageFormatDialog::handleCancel);
}

void YuvImageFormatDialog::retranslateUi()
{
    ok_btn->setText(tr("OK"));
    cancel_btn->setText(tr("CANCEL"));
    width_label->setText(tr("Width:"));
    height_label->setText(tr("Height:"));
    filepath_label->setText("File Path:");
    format_label->setText("Format:");
    align_label->setText(tr("Width Align:"));
}

void YuvImageFormatDialog::handleOk()
{
    this->accept();
}

void YuvImageFormatDialog::handleCancel()
{
    this->reject();
}

std::tuple<QString, QString> YuvImageFormatDialog::getSize()
{
    std::tuple<QString, QString> result = std::make_tuple(width_line_edit->text(), height_line_edit->text());
    return result;
}

QString YuvImageFormatDialog::getFormat()
{
    return format_combobox->currentText();
}

QString YuvImageFormatDialog::getAlign()
{
    return align_line_edit->text();
}

void YuvImageFormatDialog::setFilepath(QString path)
{
    filepath_edit->setText(path);
    QStringList strNameList = path.split("/");
    if (strNameList.size() > 0) {
        QString strName = strNameList[strNameList.size() - 1];
        QStringList strParaList = strName.split("_");
        if (strParaList.size() >= 2) {
            QStringList strResList = strParaList[0].split("X");
            if (strResList.size() == 2) {
                int resX = strResList[0].toInt();
                int resY = strResList[1].toInt();
                if ((resX != 0) && (resY != 0) ) {
                    width_line_edit->setText(strResList[0]);
                    height_line_edit->setText(strResList[1]);
                } else {
                    return;
                }
            }

            QString strFormat = strParaList[1];
            if (strFormat == "YUV420") {
                format_combobox->setCurrentIndex(0);
            } else if (strFormat == "NV21") {
                format_combobox->setCurrentIndex(1);
            } else if (strFormat == "NV12") {
                format_combobox->setCurrentIndex(2);
            } else {
                return;
            }
        }
    }
}

