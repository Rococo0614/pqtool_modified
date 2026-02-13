#include "file_dialog.hpp"
#include <QFileDialog>
#include <QTextStream>

FileDialog::FileDialog(QWidget *parent, MODE mode) :
    QDialog(parent)
{
    setWindowTitle(tr("Dialog"));
    setWindowFlags(Qt::CustomizeWindowHint|Qt::WindowCloseButtonHint);
    setFixedSize(200, 160);

    this->mode = mode;
    initUI();
    retranslateUi();
}

void FileDialog::initUI()
{
    main_layout = new QVBoxLayout(this);
    file_format_combobox = new QComboBox();
    ok_btn = new QPushButton;
    cancel_btn = new QPushButton;

    QList<QString> format_list = {CVITEK_FORMAT_STR, HI_FORMAT_STR, CVS_FORMAT_STR};
    for (QString format : format_list) {
        file_format_combobox->addItem(format);
    }

    main_layout->addWidget(file_format_combobox);
    main_layout->addWidget(ok_btn, Qt::AlignCenter);
    main_layout->addWidget(cancel_btn, Qt::AlignCenter);

    connect(ok_btn, &QPushButton::clicked, this, &FileDialog::handleOpenFile);
    connect(cancel_btn, &QPushButton::clicked, this, &FileDialog::close);
}

void FileDialog::setMode(MODE mode)
{
    this->mode = mode;
}

void FileDialog::retranslateUi()
{
    ok_btn->setText(tr("OK"));
    cancel_btn->setText(tr("CANCEL"));

}

void FileDialog::handleOpenFile()
{
    QString file_format = file_format_combobox->currentText();
    QString dialog_format;
    if (file_format == CVITEK_FORMAT_STR) {
        dialog_format = "Json files (*.json)";
    } else if (file_format == HI_FORMAT_STR){
        dialog_format = "Text Files (*.txt)";
    } else {
        dialog_format = "CSV Files (*.csv)";
    }

    QString file_path = "";
    if (mode == MODE_LOAD) {
        file_path = QFileDialog::getOpenFileName(this,
              tr("Open Data File"), "./", dialog_format);
    } else {
        file_path = QFileDialog::getSaveFileName(this,
                tr("Save Gamma Settings file"), "",
                dialog_format);
    }

    if (file_path.isEmpty()) {
        close();
        return;
    }

    emit sigSelectFile(file_format, file_path, mode);
}
