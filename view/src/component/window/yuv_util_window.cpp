#include "yuv_util_window.hpp"
#include "network_client_helper.hpp"
#include "raw_image_format_dialog.hpp"
#include "utils.hpp"
#include "yuv_image_option_dialog.hpp"

#include <QLabel>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QAction>

const int MIN_WINDOW_WIDTH = 1300;
const int MIN_WINDOW_HEIGHT = 768;
const QStringList send_target_list = {"VI", "VPSS", "AVSP"};

YuvUtilWindow::YuvUtilWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle(tr("Yuv Utility"));
    this->setWindowFlags(Qt::WindowCloseButtonHint|Qt::MSWindowsFixedSizeDialogHint);
    initMenu();
    initUI();
    retranslateUi();
    initEvent();
}

void YuvUtilWindow::initMenu()
{
}

void YuvUtilWindow::initUI()
{
    main_widget = new QWidget();
    main_widget->setFixedSize(QSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT));
    main_layout = new QGridLayout();
    image_list_view = new YuvImageListWidget;

    add_yuv_btn = new QPushButton();
    dis_select_btn = new QPushButton;
    move_up_btn = new QPushButton;
    move_down_btn = new QPushButton;
    del_btn = new QPushButton;
    del_all_btn = new QPushButton;

    QLabel *target_label = new QLabel("Yuv Sending to:");
    send_target = new QComboBox;
    send_target->addItems(send_target_list);
    send_target->setCurrentIndex(0);
    QHBoxLayout *target_layout = new QHBoxLayout;
    target_layout->addWidget(target_label);
    target_layout->addWidget(send_target);
    stitch_flag = new QCheckBox;
    snap_mode = new QCheckBox;

    loop = new QCheckBox;
    send_btn = new QPushButton;
    flush_btn = new QPushButton;
    cancel_btn = new QPushButton;
    QWidget *widget = new QWidget;
    QVBoxLayout *v_layout = new QVBoxLayout(widget);
    v_layout->addWidget(add_yuv_btn);
    v_layout->addWidget(dis_select_btn);
    v_layout->addWidget(move_up_btn);
    v_layout->addWidget(move_down_btn);
    v_layout->addWidget(del_btn);
    v_layout->addWidget(del_all_btn);
    v_layout->addLayout(target_layout);
    v_layout->addWidget(stitch_flag);
    v_layout->addWidget(snap_mode);

    v_layout->addWidget(loop);
    v_layout->addWidget(send_btn);
    v_layout->addWidget(flush_btn);
    v_layout->addWidget(cancel_btn);

    widget->setMaximumWidth(300);
    main_layout->addWidget(widget, 0, 0);
    main_layout->setColumnMinimumWidth(0, 300);
    main_layout->addWidget(image_list_view, 0, 1);
    main_layout->setColumnMinimumWidth(1, 1000);
    setCentralWidget(main_widget);
    centralWidget()->setLayout(main_layout);

    connect(add_yuv_btn, &QPushButton::clicked, this, &YuvUtilWindow::handleOpenYuvFile);
    connect(dis_select_btn, &QPushButton::clicked, [=]{
        disel_toggle = disel_toggle ? false : true;
        if (disel_toggle) {
            image_list_view->setCheckState(Qt::CheckState::Unchecked);
            dis_select_btn->setText("Select All");
        } else {
            image_list_view->setCheckState(Qt::CheckState::Checked);
            dis_select_btn->setText("Deselect All");
        }
    });

    connect(move_up_btn, &QPushButton::clicked, [=]{
        image_list_view->moveup_sel();
    });

    connect(move_down_btn, &QPushButton::clicked, [=]{
        image_list_view->movedown_sel();
    });

    connect(del_btn, &QPushButton::clicked, [=]{
        image_list_view->del_selected();
    });

    connect(del_all_btn, &QPushButton::clicked, [=]{
        image_list_view->del_all();
    });

    connect(send_btn, &QPushButton::clicked, this, &YuvUtilWindow::slotSend);
    connect(flush_btn, &QPushButton::clicked, this, &YuvUtilWindow::slotFlush);
    connect(cancel_btn, &QPushButton::clicked, this, &YuvUtilWindow::slotCancel);
}

void YuvUtilWindow::initEvent()
{
}

void YuvUtilWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void YuvUtilWindow::retranslateUi()
{
    add_yuv_btn->setText(tr("Add Yuv Image"));
    dis_select_btn->setText(tr("UnSelect All"));
    move_up_btn->setText(tr("Move Up"));
    move_down_btn->setText(tr("Move Down"));
    del_btn->setText(tr("Delete"));
    del_all_btn->setText(tr("Delete All"));
    stitch_flag->setText(tr("StitchFlag"));
    snap_mode->setText(tr("Snap Mode"));
    loop->setText(tr("Loop"));
    send_btn->setText(tr("Send"));
    flush_btn->setText(tr("Flush"));
    cancel_btn->setText(tr("Cancel"));
}

void YuvUtilWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        this->close();
    }
}


void YuvUtilWindow::handleOpenYuvFile()
{
    QFileDialog file_dialog(this);
    QString filename = file_dialog.getOpenFileName(this, tr("Open yuv file"), "",
                                                   tr("*.yuv"));
    if (!filename.isEmpty()) {
        YuvImageOptionDialog option_dialog;
        option_dialog.setFilePath(filename);
        if (QDialog::Accepted == option_dialog.exec())
        {
            YuvImgOption options = option_dialog.getOptionData();
            YuvImageListItem *item = new YuvImageListItem();
            item->fromYuvImgOption(options);
            item->setFStart(1);
            item->setFGop(1);

            int width = item->getWidth();
            int height = item->getHeight();
            double pixel_size = sizeof(short);
            if (item->getPixelType() == 0 || item->getPixelType() == 2)
            {
                pixel_size = 2;
            }
            else if (item->getPixelType() == 1 || item->getPixelType() == 3)
            {
                pixel_size = 3 / 2.0;
            }
            else
            {
                pixel_size = 1;
            }

            QFile file(filename);
            file.open(QIODevice::ReadOnly);
            QByteArray data = file.readAll();
            file.close();
            item->setFrameNum(data.size() / (height * width * pixel_size));
            item->setFEnd(item->getFrameNum());
            item->setYuvdata(data);
            image_list_view->addTableItem(item);
        }
    }
}

void YuvUtilWindow::slotSend()
{

}

void YuvUtilWindow::slotFlush()
{

}

void YuvUtilWindow::slotCancel()
{

}
