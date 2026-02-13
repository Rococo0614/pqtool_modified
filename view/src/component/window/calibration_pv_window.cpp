#include "calibration_pv_window.hpp"

#include <QFileDialog>
#include <QSlider>
#include <QMessageBox>

CalibrationPVWindow::CalibrationPVWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle(tr("View Image"));
    setWindowFlags(Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
    setMinimumSize(1080, 720);

    initUI();
    initEvent();
}

void CalibrationPVWindow::initUI()
{
    main_widget = new QWidget();
    main_layout = new QVBoxLayout();

    input_img = new ImageViewerWidget();
    img_label = new ImageLabel(this, ImageLabel::SINGLE_RECT);
    input_img->setImageLabel(img_label);
    input_img->setFilePathHidden();

    QHBoxLayout *h_layout = new QHBoxLayout();
    QLabel *width_lab = new QLabel("Width:");
    width_lab->setFixedSize(50,25);
    width_edit = new QLineEdit;
    width_edit->setFixedSize(50,25);
    width_edit->setText("0");
    width_edit->setEnabled(false);
    QLabel *height_lab = new QLabel("Height:");
    height_lab->setFixedSize(50,25);
    height_edit = new QLineEdit;
    height_edit->setFixedSize(50,25);
    height_edit->setText("0");
    height_edit->setEnabled(false);

    QLabel *cropX_lab = new QLabel("CropX:");
    cropX_lab->setFixedSize(50,25);
    cropX_edit = new QLineEdit;
    cropX_edit->setFixedSize(50,25);
    cropX_edit->setText("0");
    connect(cropX_edit,&QLineEdit::editingFinished,[=]{
        int x = cropX_edit->text().toInt();
        int width = width_edit->text().toInt();
        x = (x+1)/2*2;
        x = qBound(0, x, width);
        cropX_edit->setText(QString::number(x));
        int w = cropW_edit->text().toInt();
        if (w > (width-x)) {
            cropW_edit->setText(QString::number(width-x));
        }
        handleCropInfoToRect();
    });
    QLabel *cropY_lab = new QLabel("CropY:");
    cropY_lab->setFixedSize(50,25);
    cropY_edit = new QLineEdit;
    cropY_edit->setFixedSize(50,25);
    cropY_edit->setText("0");
    connect(cropY_edit,&QLineEdit::editingFinished,[=]{
        int y = cropY_edit->text().toInt();
        int height = height_edit->text().toInt();
        y = (y+1)/2*2;
        y = qBound(0, y, height);
        cropY_edit->setText(QString::number(y));
        int h = cropH_edit->text().toInt();
        if (h > (height-y)) {
            cropH_edit->setText(QString::number(height-y));
        }
        handleCropInfoToRect();
    });
    QLabel *cropW_lab = new QLabel("CropW:");
    cropW_lab->setFixedSize(50,25);
    cropW_edit = new QLineEdit;
    cropW_edit->setFixedSize(50,25);
    cropW_edit->setText("0");
    connect(cropW_edit,&QLineEdit::editingFinished,[=]{
        int w = cropW_edit->text().toInt();
        int width = width_edit->text().toInt();
        int x = cropX_edit->text().toInt();
        w = (w+1)/2*2;
        w = qBound(0, w, width-x);
        cropW_edit->setText(QString::number(w));
        handleCropInfoToRect();
    });
    QLabel *cropH_lab = new QLabel("CropH:");
    cropH_lab->setFixedSize(50,25);
    cropH_edit = new QLineEdit;
    cropH_edit->setFixedSize(50,25);
    cropH_edit->setText("0");
    connect(cropH_edit,&QLineEdit::editingFinished,[=]{
        int h = cropH_edit->text().toInt();
        int height = height_edit->text().toInt();
        int y = cropY_edit->text().toInt();
        h = (h+1)/2*2;
        h = qBound(0, h, height-y);
        cropH_edit->setText(QString::number(h));
        handleCropInfoToRect();
    });

    ok_btn = new QPushButton("OK");
    ok_btn->setFixedSize(60,25);
    connect(ok_btn, &QPushButton::clicked, this, &CalibrationPVWindow::handleOk);
    cancel_btn = new QPushButton("Cancel");
    cancel_btn->setFixedSize(60,25);
    connect(cancel_btn, &QPushButton::clicked, this, &CalibrationPVWindow::close);

    h_layout->addWidget(width_lab);
    h_layout->addWidget(width_edit);
    h_layout->addWidget(height_lab);
    h_layout->addWidget(height_edit);
    h_layout->addWidget(cropX_lab);
    h_layout->addWidget(cropX_edit);
    h_layout->addWidget(cropY_lab);
    h_layout->addWidget(cropY_edit);
    h_layout->addWidget(cropW_lab);
    h_layout->addWidget(cropW_edit);
    h_layout->addWidget(cropH_lab);
    h_layout->addWidget(cropH_edit);
    h_layout->addStretch(1);
    h_layout->addWidget(ok_btn);
    h_layout->addWidget(cancel_btn);

    main_layout->addWidget(input_img);
    main_layout->addLayout(h_layout);
    setCentralWidget(main_widget);
    centralWidget()->setLayout(main_layout);
}

void CalibrationPVWindow::initEvent()
{
    connect(img_label, &ImageLabel::sigUpdateSingleRect, this, &CalibrationPVWindow::handleRectToCropInfo);
}

void  CalibrationPVWindow::OpenImage(RawImageItem *item)
{
    QString path = item->getFilePath();
    if (path.isEmpty()) {
        return;
    }

    QFileInfo file(path);
    if (!file.exists()) {
        QMessageBox::warning(this, tr("Error!"), tr("Can't find image file!"), QMessageBox::Ok);
        return;
    }

    currItem = item;
    unsigned int width = item->getWidth();
    unsigned int height = item->getHeight();
    utils::RAW_BAYER_ID bayer = static_cast<utils::RAW_BAYER_ID>(item->getBayer().toInt());
    utils::RAW_BITS bits = static_cast<utils::RAW_BITS>(item->getBits().toInt());
    utils::RAW_PACK_TYPE raw_pack_type = item->getPacked();
    unsigned char *r = utils::readRawImage(path, width, height, bayer, bits, raw_pack_type);
    QImage img(r, static_cast<int>(width), static_cast<int>(height), QImage::Format_RGB32);
    input_img->setImg(QPixmap::fromImage(img));

    RECT_S rect = item->getRect();
    if ((rect.w == 0) || (rect.h == 0)) {
        rect.w = (int)((width * (float)0.125) / 2) * 2;
        rect.h = (int)((height * (float)0.1667) / 2) * 2;
        rect.x = (width - rect.w) / 2;
        rect.y = (height - rect.h) / 2;
        item->setRect(rect);
    }
    width_edit->setText(QString::number(width));
    height_edit->setText(QString::number(height));
    cropX_edit->setText(QString::number(rect.x));
    cropY_edit->setText(QString::number(rect.y));
    cropW_edit->setText(QString::number(rect.w));
    cropH_edit->setText(QString::number(rect.h));
    img_label->updateSingleRect(rect.x, rect.y, rect.w, rect.h);
}

void CalibrationPVWindow::handleOk()
{
    RECT_S rect;
    rect.x = cropX_edit->text().toInt();
    rect.y = cropY_edit->text().toInt();
    rect.w = cropW_edit->text().toInt();
    rect.h = cropH_edit->text().toInt();
    currItem->setRect(rect);
    this->close();
}

void CalibrationPVWindow::handleCropInfoToRect()
{
    int x = cropX_edit->text().toInt();
    int y = cropY_edit->text().toInt();
    int w = cropW_edit->text().toInt();
    int h = cropH_edit->text().toInt();
    img_label->updateSingleRect(x, y, w, h);
}

void CalibrationPVWindow::handleRectToCropInfo(QRectF rect, double factor)
{
    double rect_w, rect_h, center_x, center_y;
    int w, h, x, y;

    rect_w = rect.width();
    rect_h = rect.height();
    center_x = rect.x();
    center_y = rect.y();

    w = ((int)(rect_w /factor + 0.5) + 1) /2 * 2;
    h = ((int)(rect_h /factor + 0.5) + 1) /2 * 2;
    x = ((int)(center_x /factor + 0.5) + 1) /2 * 2;
    y = ((int)(center_y /factor + 0.5) + 1) /2 * 2;

    cropX_edit->setText(QString::number(x));
    cropY_edit->setText(QString::number(y));
    cropW_edit->setText(QString::number(w));
    cropH_edit->setText(QString::number(h));
}

