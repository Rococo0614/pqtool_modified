#include "ccm_lab_img_window.hpp"

#include <QFileDialog>
#include <QSlider>
#include <QMessageBox>

CcmLabImageWindow::CcmLabImageWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle(tr("Specify patches"));
    setWindowFlags(Qt::WindowCloseButtonHint);
    this->setFixedSize(960, 640);

    initUI();
    initEvent();
    retranslateUi();
}

void CcmLabImageWindow::initUI()
{
    main_widget = new QWidget();
    main_layout = new QVBoxLayout();

    input_img = new ImageViewerWidget();
    input_img_label = new ImageLabel(this, ImageLabel::COLOR_CHECKER);
    input_img->setImageLabel(input_img_label);
    input_img->setFilePathHidden();
    roi_control_widget = createRoiControlUI(input_img_label);

    edit_roi_btn = new QPushButton("Edit ROIs");
    connect(edit_roi_btn, &QPushButton::clicked, [=]() {
        if (roi_control_widget != nullptr) {
            roi_control_widget->show();
        }
    });

    ok_btn = new QPushButton("OK");
    connect(ok_btn, &QPushButton::clicked, this, &CcmLabImageWindow::handleOk);
    cancel_btn = new QPushButton("Cancel");
    connect(cancel_btn, &QPushButton::clicked, this, &CcmLabImageWindow::close);
    QHBoxLayout *h_layout = new QHBoxLayout();
    h_layout->addStretch();
    h_layout->addWidget(edit_roi_btn);
    h_layout->addStretch();
    h_layout->addWidget(ok_btn);
    h_layout->addWidget(cancel_btn);

    main_layout->addWidget(input_img);
    main_layout->addLayout(h_layout);
    setCentralWidget(main_widget);
    centralWidget()->setLayout(main_layout);
}

void CcmLabImageWindow::initEvent()
{

}

void CcmLabImageWindow::retranslateUi()
{

}

void  CcmLabImageWindow::OpenImage(QString path)
{
    if (path.isEmpty()) {
        return;
    }

    QFileInfo file(path);
    if (!file.exists()) {
        QMessageBox::warning(this, tr("Error!"), tr("Can't find image file!"), QMessageBox::Ok);
        return;
    }

    this->path = path;
    input_img->openImgPath(path);

}

void CcmLabImageWindow::handleOk()
{
    int roi_width = 10, roi_height = 10;

    QVector<QRectF> color_rects = input_img_label->getColorCheckerRect();
    double scale_factor = input_img->getScaleFactor();
    if (color_rects.size() != 24) {
        return;
    }
    roi_width = qFloor(color_rects.first().width()/scale_factor);
    roi_height = qFloor(color_rects.first().height()/scale_factor);

    qDebug("ccm, roi_width:%d roi_height:%d\n", roi_width, roi_height);

    if (path.isEmpty()) {
        return;
    }

    QImage src(path);
    QImage src_img;
    if (src.format() != QImage::Format_RGB888) {
        src_img = src.convertToFormat(QImage::Format_RGB888);
    } else {
        src_img = src;
    }

    coordinate coordinate[24] = {};
    ROI_INFO roi_info[24] = {};
    unsigned char *img_uchar =  new unsigned char[src_img.width() * src_img.height() * 3];

    qDebug("ccm, width:%d, height:%d, src format:%d, det format:%d\n",
    src_img.width(), src_img.height(), src.format(), src_img.format());

    for (int y = 0; y < src_img.height(); y++) {
        int idx = y * src_img.bytesPerLine();
        memcpy(&img_uchar[idx], src_img.scanLine(y), src_img.bytesPerLine());
    }

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 6; x++) {
            QRectF rect = color_rects[x + y * 6];

            if (scale_factor > 0) {
                rect.setX(qFloor(rect.x()/scale_factor));
                rect.setY(qFloor(rect.y()/scale_factor));
            }
            // color rect position need even
            if (qFloor(rect.x()) % 2 == 1) {
                rect.setX(qFloor(rect.x()) + 1);
            }
            if (qFloor(rect.y()) % 2 == 1) {
                rect.setY(qFloor(rect.y()) + 1);
            }
            if (rect.x() < 0) rect.setX(0);
            if (rect.y() < 0) rect.setY(0);

            roi_info[x + y * 6].win_width = roi_width;
            roi_info[x + y * 6].win_height = roi_height;
            roi_info[x + y * 6].str_x = rect.x();
            roi_info[x + y * 6].str_y = rect.y();
        }
    }

    cali_roi_rgb_cal(img_uchar, src_img.width(), src_img.height(), coordinate, roi_info, 24);

    for (int i = 0; i < 24; i++) {
        qDebug("coordinate:%f, %f, %f, %d, %d\n",
        coordinate[i].x, coordinate[i].y, coordinate[i].z, coordinate[i].valid, coordinate[i].create_idx);
    }

    for (int i = 0; i < 24; i++) {
        red[i] = coordinate[i].x;
        green[i] = coordinate[i].y;
        blue[i] = coordinate[i].z;
    }
    emit sigLabImgOk();

    delete [] img_uchar;
    this->close();

}

void CcmLabImageWindow::GetImgRgb(double *r, double *g, double *b)
{
    if ((r == nullptr) || (r == nullptr) || (r == nullptr)) {
        return;
    }

    for (int i = 0; i < 24; i++) {
        r[i] = red[i];
        g[i] = green[i];
        b[i] = blue[i];
    }
}

QWidget *CcmLabImageWindow::createRoiControlUI(ImageLabel *img_label)
{
    if (img_label == nullptr) return nullptr;

    QWidget *roi_control_widget = new QWidget();
    roi_control_widget->setWindowFlags(Qt::WindowCloseButtonHint|Qt::WindowStaysOnTopHint);
    roi_control_widget->setFixedSize(250, 400);

    QPushButton *up = new QPushButton();
    up->setIcon(QIcon(":/icons/control_arrow_up.png"));
    QPushButton *down = new QPushButton();
    down->setIcon(QIcon(":/icons/control_arrow_down.png"));
    QPushButton *left = new QPushButton();
    left->setIcon(QIcon(":/icons/control_arrow_left.png"));
    QPushButton *right = new QPushButton();
    right->setIcon(QIcon(":/icons/control_arrow_right.png"));
    QPushButton *scale_up = new QPushButton();
    scale_up->setIcon(QIcon(":/icons/zoom_in.png"));
    QPushButton *scale_down = new QPushButton();
    scale_down->setIcon(QIcon(":/icons/zoom_out.png"));

    QPushButton *v_space_up = new QPushButton("+");
    QPushButton *v_space_down = new QPushButton("-");
    QPushButton *h_space_up = new QPushButton("+");
    QPushButton *h_space_down = new QPushButton("-");
    QPushButton *reset_btn = new QPushButton("reset");
    QPushButton *h_interval_up = new QPushButton("+");
    QPushButton *v_interval_up = new QPushButton("+");
    QPushButton *h_interval_down = new QPushButton("-");
    QPushButton *v_interval_down = new QPushButton("-");

    connect(up, &QPushButton::clicked, this, [=]() {
        img_label->up();
    });
    connect(down, &QPushButton::clicked, this, [=]() {
        img_label->down();
    });
    connect(left, &QPushButton::clicked, this, [=]() {
        img_label->left();
    });
    connect(right, &QPushButton::clicked, this, [=]() {
        img_label->right();
    });
    connect(scale_up, &QPushButton::clicked, this, [=]() {
        img_label->large();
    });
    connect(scale_down, &QPushButton::clicked, this, [=]() {
        img_label->small();
    });

    connect(v_space_up, &QPushButton::clicked, this, [=]() {
        img_label->vspaceLarge();
    });
    connect(v_space_down, &QPushButton::clicked, this, [=]() {
        img_label->vspaceSmall();
    });

    connect(h_space_up, &QPushButton::clicked, this, [=]() {
        img_label->hspaceLarge();
    });
    connect(h_space_down, &QPushButton::clicked, this, [=]() {
        img_label->hspaceSmall();
    });

    connect(reset_btn, &QPushButton::clicked, this, [=]() {
        img_label->reset();
    });

    connect(h_interval_down, &QPushButton::clicked, this, [=]() {
        img_label->hIntervalSmall();
    });

    connect(h_interval_up, &QPushButton::clicked, this, [=]() {
        img_label->hIntervalLarge();
    });

    connect(v_interval_down, &QPushButton::clicked, this, [=]() {
        img_label->vIntervalSmall();
    });

    connect(v_interval_up, &QPushButton::clicked, this, [=]() {
        img_label->vIntervalLarge();
    });

    QHBoxLayout *h_layout = new QHBoxLayout();
    QVBoxLayout *v_layout = new QVBoxLayout();
    h_layout->setAlignment(Qt::AlignCenter);
    v_layout->setAlignment(Qt::AlignCenter);

    v_layout->addWidget(up);
    v_layout->addWidget(scale_up);
    v_layout->addWidget(scale_down);
    v_layout->addWidget(down);

    h_layout->addWidget(left);
    h_layout->addLayout(v_layout);
    h_layout->addWidget(right);

    v_layout = new QVBoxLayout();
    v_layout->setAlignment(Qt::AlignCenter);
    v_layout->addLayout(h_layout);

    QHBoxLayout *vspace_layout = new QHBoxLayout;
    QLabel *label_vspace = new QLabel("v space   ");
    vspace_layout->addWidget(label_vspace);
    vspace_layout->addWidget(v_space_down);
    vspace_layout->addWidget(v_space_up);
    v_layout->addLayout(vspace_layout);

    QHBoxLayout *hspace_layout = new QHBoxLayout;
    QLabel *label_hspace = new QLabel("h space   ");
    hspace_layout->addWidget(label_hspace);
    hspace_layout->addWidget(h_space_down);
    hspace_layout->addWidget(h_space_up);
    v_layout->addLayout(hspace_layout);

    QHBoxLayout *vinterval_layout = new QHBoxLayout;
    QLabel *label_vinterval = new QLabel("v interval");
    vinterval_layout->addWidget(label_vinterval);
    vinterval_layout->addWidget(v_interval_down);
    vinterval_layout->addWidget(v_interval_up);
    v_layout->addLayout(vinterval_layout);

    QHBoxLayout *hinterval_layout = new QHBoxLayout;
    QLabel *label_hinterval = new QLabel("h interval");
    hinterval_layout->addWidget(label_hinterval);
    hinterval_layout->addWidget(h_interval_down);
    hinterval_layout->addWidget(h_interval_up);
    v_layout->addLayout(hinterval_layout);

    v_layout->addWidget(reset_btn);

    v_layout->addSpacing(10);

    roi_control_widget->setLayout(v_layout);

    return roi_control_widget;
}


