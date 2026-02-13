#include "raw_scene_dialog.hpp"
#include <QFileDialog>

RawSceneDialog::RawSceneDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Raw Scene Dialog"));
    this->setMinimumWidth(400);

    initUI();
    retranslateUi();
}

void RawSceneDialog::initUI()
{
    main_layout = new QVBoxLayout(this);
    text_edit = new QPlainTextEdit;
    text_edit->setReadOnly(true);
    ok_btn = new QPushButton;
    cancel_btn = new QPushButton;
    scene_label = new QLabel;
    scene_combobox = new QComboBox;

    for (int i = static_cast<int>(SCENE::BEGIN); i < static_cast<int>(SCENE::COLOR124); i++) {
        scene_combobox->addItem(getSceneName(static_cast<SCENE>(i)), i);
    }

    QHBoxLayout *h_layout = new QHBoxLayout();
    h_layout->addWidget(scene_label);
    h_layout->addWidget(scene_combobox);
    h_layout->addWidget(ok_btn);
    h_layout->addWidget(cancel_btn);

    main_layout->addWidget(text_edit);
    main_layout->addLayout(h_layout);

    connect(ok_btn, &QPushButton::clicked, this, &RawSceneDialog::handleOk);
    connect(cancel_btn, &QPushButton::clicked, this, &RawSceneDialog::handleCancel);
}

void RawSceneDialog::retranslateUi()
{
    ok_btn->setText(tr("OK"));
    cancel_btn->setText(tr("CANCEL"));
    scene_label->setText(tr("RAW Scene:"));
}

void RawSceneDialog::handleOk()
{
    this->accept();
}

void RawSceneDialog::handleCancel()
{
    this->reject();
}

SCENE RawSceneDialog::getScene()
{
    return static_cast<SCENE>(scene_combobox->currentData().toInt());
}

void RawSceneDialog::setFileList(const QStringList &file_list)
{
    for (auto file_path : file_list) {
        text_edit->appendPlainText(file_path);
    }
}
