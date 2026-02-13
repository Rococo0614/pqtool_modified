#include "base_image_option.hpp"
#include <QFileDialog>

BaseImageOptionDialog::BaseImageOptionDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle(tr("Image Options"));
    setWindowFlags(Qt::WindowCloseButtonHint);
    this->setFixedSize(720, 240);

    initUI();
    retranslateUi();
}

void BaseImageOptionDialog::initUI()
{
    main_layout = new QVBoxLayout(this);

    QLabel *file_label = new QLabel("File Path");
    file_path = new QLineEdit;
    file_path->setReadOnly(true);
    QHBoxLayout *file_layout = new QHBoxLayout;
    file_layout->addWidget(file_label);
    file_layout->addWidget(file_path);

    option_group = new QGroupBox("Options");
    group_layout = new QVBoxLayout;
    QLabel *res_label = new QLabel("Resolution (W x H):");
    QLabel *x_label = new QLabel("x");
    w_res_line = new QLineEdit("1920");
    h_res_line = new QLineEdit("1080");
    res_list_box = new QComboBox;
    QStringList res_strlist = {
        "1280 x 720",
        "1920 x 1080",
        "2048 x 1536",
        "2592 x 1520",
        "2592 x 1944",
        "3840 x 2160"
    };
    res_list_box->addItems(res_strlist);
    res_list_box->setCurrentIndex(1);
    QHBoxLayout *res_layout = new QHBoxLayout;
    res_layout->addWidget(res_label);
    res_layout->addWidget(w_res_line);
    res_layout->addWidget(x_label);
    res_layout->addWidget(h_res_line);
    res_layout->addWidget(res_list_box);
    res_layout->addStretch();

    group_layout->addLayout(res_layout);
    group_layout->setAlignment(Qt::AlignCenter);
    option_group->setLayout(group_layout);
    option_group->setAlignment(Qt::AlignLeft|Qt::AlignTop);

    ok_btn = new QPushButton;
    cancel_btn = new QPushButton;
    QHBoxLayout *h_layout = new QHBoxLayout();
    h_layout->addStretch();
    h_layout->addWidget(ok_btn);
    h_layout->addWidget(cancel_btn);

    main_layout->addLayout(file_layout);
    main_layout->addWidget(option_group);
    main_layout->addLayout(h_layout);

    connect(res_list_box,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, &BaseImageOptionDialog::slotQuickSelect);
    connect(ok_btn, &QPushButton::clicked, this, &BaseImageOptionDialog::handleOk);
    connect(cancel_btn, &QPushButton::clicked, this, &BaseImageOptionDialog::handleCancel);
}

void BaseImageOptionDialog::retranslateUi()
{
    ok_btn->setText(tr("OK"));
    cancel_btn->setText(tr("CANCEL"));
}

void BaseImageOptionDialog::handleOk()
{
    this->accept();
}

void BaseImageOptionDialog::handleCancel()
{
    this->reject();
}

void BaseImageOptionDialog::addOptionItem(QLayout *item_layout)
{
    group_layout->addLayout(item_layout);
}

void BaseImageOptionDialog::addOptionItem(QWidget *item)
{
    group_layout->addWidget(item);
}

void BaseImageOptionDialog::setResolution(QString res_x, QString res_y)
{
    w_res_line->setText(res_x);
    h_res_line->setText(res_y);
}

void BaseImageOptionDialog::slotQuickSelect(int index)
{
    switch(index) {
        case 0:
        {
            w_res_line->setText("1280");
            h_res_line->setText("720");
            break;
        }
        case 1:
        {
            w_res_line->setText("1920");
            h_res_line->setText("1080");
            break;
        }
        case 2:
        {
            w_res_line->setText("2048");
            h_res_line->setText("1536");
            break;
        }
        case 3:
        {
            w_res_line->setText("2592");
            h_res_line->setText("1520");
            break;
        }
        case 4:
        {
            w_res_line->setText("2592");
            h_res_line->setText("1944");
            break;
        }
        case 5:
        {
            w_res_line->setText("3840");
            h_res_line->setText("2160");
            break;
        }
        default:
        {
            w_res_line->setText("1920");
            h_res_line->setText("1080");
            break;
        }
    }
}
