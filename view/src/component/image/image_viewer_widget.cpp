#include "image_viewer_widget.hpp"
#include "color_histogram_window.hpp"
#include "raw_image_format_dialog.hpp"
#include "yuv_image_format_dialog.hpp"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QImageReader>
#include <QScrollArea>
#include <QtCore>
#include <QPainter>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QtGlobal>

ImageViewerWidget::ImageViewerWidget(QWidget *parent) :
    QWidget(parent),
    scale_factor(1.0)
{
    initUI();
    retranslateUi();
    setAcceptDrops(true);
}

void ImageViewerWidget::setFilePathHidden()
{
    file_path->setHidden(true);
    choose_file_btn->setHidden(true);
}

void ImageViewerWidget::setPathHidden()
{
    file_path->setHidden(true);
}

void ImageViewerWidget::setSaveBtnHidden()
{
    save_btn->setHidden(true);
}

void ImageViewerWidget::setImg(QPixmap img)
{
    double scale_h = 1.0, scale_w = 1.0;
    if (image_label == nullptr) return;

    // check image is same
    if ((image_label->pixmap()) && (*image_label->pixmap() == img)) {
        return;
    }

    zoom_in_btn->setEnabled(true);
    zoom_out_btn->setEnabled(true);
    zoom_box->setEnabled(true);

    if ((!img.isNull()) && (img.width() != 0)) {
        scale_w = static_cast<double>(scrollArea->width())/static_cast<double>(img.width());
        scale_h = static_cast<double>(scrollArea->height())/static_cast<double>(img.height());

        if(scale_w < scale_h)
            scale_factor = scale_w;
        else
            scale_factor = scale_h;
    }

    image_label->setPixmap(img.copy());
    image_label->adjustSize();
    image_label->setRepaintEvent(true);
    scaleImg();

    emit sigSetImage(image_label->pixmap());
}

void ImageViewerWidget::setImageLabel(ImageLabel *label)
{
    image_label = label;
    image_label->setScaledContents(true);
    scrollArea->setWidget(image_label);
}

double ImageViewerWidget::getScaleFactor()
{
    return this->scale_factor;
}

void ImageViewerWidget::dragEnterEvent(QDragEnterEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()) {
        return;
    }

    QString fileName = urls.first().toLocalFile();
    if (fileName.isEmpty()) {
        return;
    }

    QFileInfo fileinfo(fileName);
    QString ext = fileinfo.suffix();

    if (isSupportedFileExtension(ext)) {
        event->acceptProposedAction();
    }
}

void ImageViewerWidget::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()) {
        return;
    }

    QString fileName = urls.first().toLocalFile();
    if (fileName.isEmpty()) {
        return;
    }

    openImgPath(fileName);
}

void ImageViewerWidget::changeEvent(QEvent *event)
{
    switch(event->type()) {
    case QEvent::LanguageChange:
        retranslateUi();
        break;
    default:
        break;
    }
    QWidget::changeEvent(event);
}

void ImageViewerWidget::setFvTable(QVector<QVector<int>> &fv)
{
    image_label->setFvTable(fv);
}

void ImageViewerWidget::triggerGrid3AEnable(bool en)
{
    if (en) {
        image_label->setEnGridMode(STATE_AF);
    } else {
        image_label->setEnGridMode(STATE_NONE);
    }

    image_label->repaint();
}

void ImageViewerWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
}

void ImageViewerWidget::wheelEvent(QWheelEvent  *event)
{
    QWidget::wheelEvent(event);

    if (image_label->pixmap() == nullptr) {
        return;
    }

    scale_factor += event->delta() / 1000.0;
    scale_factor = qBound(0.1, scale_factor, 10.0);

    scaleImg();
}

void ImageViewerWidget::initUI()
{
    formt_dialog = new RawImageFormatDialog;

    file_path = new QLineEdit();
    file_path->setReadOnly(true);

    choose_file_btn = new QPushButton();
    connect(choose_file_btn, &QPushButton::clicked, this, &ImageViewerWidget::openImg);

    en_grid = new QCheckBox("En Grid");
    en_grid->setHidden(true);
    connect(en_grid, &QCheckBox::stateChanged, this, &ImageViewerWidget::slotEnableGrid);

    zoom_in_btn = new QPushButton();
    zoom_out_btn = new QPushButton();
    zoom_in_btn->setDisabled(true);
    zoom_out_btn->setDisabled(true);
    connect(zoom_in_btn, &QPushButton::clicked, this, &ImageViewerWidget::zoomin);
    connect(zoom_out_btn, &QPushButton::clicked, this, &ImageViewerWidget::zoomout);

    zoom_box = new QComboBox;
    zoom_box->addItems({"25%", "50%", "100%", "200%", "400%"});
    zoom_box->setCurrentIndex(2);
    zoom_box->setDisabled(true);
    connect(zoom_box, &QComboBox::currentTextChanged, this, &ImageViewerWidget::slotSelectFactor);

    factor_label = new QLabel();
    scrollArea = new QScrollArea();
    scrollArea->setBackgroundRole(QPalette::Dark);

    color_histogram_btn = new QPushButton();
    connect(color_histogram_btn, &QPushButton::clicked, this, &ImageViewerWidget::handleOpenColorHistogram);

    save_btn = new QPushButton();
    connect(save_btn, &QPushButton::clicked, this, &ImageViewerWidget::handleSaveImage);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(file_path);
    layout->addWidget(choose_file_btn);
    layout->addWidget(en_grid);
    layout->addWidget(zoom_in_btn);
    layout->addWidget(zoom_out_btn);
    layout->addWidget(zoom_box);
    layout->addWidget(factor_label);
    layout->addWidget(save_btn);
    layout->addWidget(color_histogram_btn);

    QVBoxLayout *vlayout = new QVBoxLayout();
    vlayout->addLayout(layout);
    vlayout->addWidget(scrollArea);

    setLayout(vlayout);
}

void ImageViewerWidget::retranslateUi()
{
    factor_label->setText(QString::number(scale_factor, 'f', 2) + "x");
    choose_file_btn->setText(tr("Open File"));
    zoom_in_btn->setText(tr("Zoom In"));
    zoom_out_btn->setText(tr("Zoom out"));
    color_histogram_btn->setText(tr("Color Histogram"));
    save_btn->setText(tr("Save"));
}

void ImageViewerWidget::modifyChooseFileBtnName(QString name)
{
    choose_file_btn->setText(name);
}

void ImageViewerWidget::setHistogramName(QString btn_name)
{
    color_histogram_btn_name = btn_name;
    color_histogram_btn->setText(btn_name);
}

void ImageViewerWidget::showEnGridCheckBox()
{
    en_grid->setHidden(false);
}

void ImageViewerWidget::openImgPath(QString img_path)
{
    if (img_path.isEmpty()) {
        return;
    }

    if (img_path.endsWith(".raw")) {
        // Choose raw image format
        formt_dialog->setFilepath(img_path);
        if(formt_dialog->exec()) {
            std::tuple<QString, QString> size = formt_dialog->getSize();
            unsigned int width = std::get<0>(size).toUInt();
            unsigned int height = std::get<1>(size).toUInt();
            utils::RAW_BAYER_ID bayer = static_cast<utils::RAW_BAYER_ID>(formt_dialog->getBayer().toInt());
            utils::RAW_BITS bits = static_cast<utils::RAW_BITS>(formt_dialog->getBits().toInt());
            utils::RAW_PACK_TYPE raw_pack_type = formt_dialog->getPacked();
            unsigned char *r = utils::readRawImage(img_path, width, height, bayer, bits, raw_pack_type);

            if (r == nullptr) {
                setLoadRawFailImg();
                setRawImageInfo("", 0, 0, bayer, bits);
                return;
            }

            QImage img(r, static_cast<int>(width), static_cast<int>(height), QImage::Format_RGB32);
            setImg(QPixmap::fromImage(img));
            setRawImageInfo(img_path, width, height, bayer, bits);
            free(r);
        }
    } else if (img_path.endsWith(".yuv")) {
        YuvImageFormatDialog *formt_dialog = new YuvImageFormatDialog;
        formt_dialog->setFilepath(img_path);
        if(formt_dialog->exec()) {
            QFile img_file(img_path);
            if (!img_file.open(QIODevice::ReadOnly)) return;
            QByteArray file_data = img_file.readAll();
            YUV_HEADER header = {};
            std::tuple<QString, QString> size = formt_dialog->getSize();
            quint16 align = formt_dialog->getAlign().toUInt();
            header.width = std::get<0>(size).toUInt();
            header.height = std::get<1>(size).toUInt();
            header.stride[0] = ALIGN(header.width, align);
            QString formatstr = formt_dialog->getFormat();
            if (formatstr == "YUV420") {
                header.stride[1] =
                header.stride[2] = ALIGN(header.width / 2, align);
                header.pixelFormat = PIXEL_FORMAT_YUV_PLANAR_420;
            } else if (formatstr == "NV21" || formatstr == "NV12") {
                header.stride[1] = header.stride[0];
                header.stride[2] = 0;
                header.pixelFormat = (formatstr == "NV21") ? PIXEL_FORMAT_NV21 : PIXEL_FORMAT_NV12;
            }
            header.size = header.stride[0] * header.height * 3 / 2;
            setImg(QPixmap::fromImage(utils::convertYUV2RGBImage(file_data, &header)));
        }
    } else {
        QImageReader reader(img_path);
        QImage img = reader.read();
        file_path->setText(img_path);
        setImg(QPixmap::fromImage(img));
    }
}

void ImageViewerWidget::setSceneLabelState(bool state)
{
    if (formt_dialog != nullptr) {
        formt_dialog->setSceneLabelState(state);
    }
}

QString ImageViewerWidget::getPath()
{
    return file_path->text();
}

bool ImageViewerWidget::isSupportedFileExtension(QString extension)
{
    if (extension == "png" || extension == "tiff" || extension == "raw" || extension == "jpg") {
        return true;
    }
    return false;
}

void ImageViewerWidget::openImg()
{
    QString img_path = QFileDialog::getOpenFileName(this, tr("Open File"), "", "*.raw");
    openImgPath(img_path);
}

void ImageViewerWidget::openJpg()
{
    QString img_path = QFileDialog::getOpenFileName(this, tr("Open File"), "", "*.jpg *.bmp");
    openImgPath(img_path);
}

void ImageViewerWidget::zoomin()
{
    scale_factor = scale_factor / 0.8;
    scaleImg();
}

void ImageViewerWidget::zoomout()
{
    scale_factor = scale_factor * 0.8;
    scaleImg();
}

void ImageViewerWidget::zoomImage(double factor)
{
    scale_factor = factor;
    scaleImg();
}

void ImageViewerWidget::scaleImg()
{
    factor_label->setText(QString::number(scale_factor, 'f', 2) + "x");
    image_label->setScaleFactor(scale_factor);
    image_label->resize(scale_factor * image_label->pixmap()->size());
}

void ImageViewerWidget::handleOpenColorHistogram()
{
    if (color_hist_window == nullptr) {
        color_hist_window = new ColorHistogramWindow();
        connect(this, &ImageViewerWidget::sigSetImage, color_hist_window, &ColorHistogramWindow::handleUpdatePoints);
    }
    color_hist_window->show();

    if ((image_label->pixmap()) && (!image_label->pixmap()->isNull())) {
        color_hist_window->handleUpdatePoints(image_label->pixmap());
    }
}

void ImageViewerWidget::handleShowRawItem(RawImageItem *raw_item)
{
    if (raw_item->getFilePath().isEmpty()) {
        return;
    }

    int width = raw_item->getWidth();
    int height = raw_item->getHeight();
    utils::RAW_BITS bits = static_cast<utils::RAW_BITS>(raw_item->getBits().toInt());
    utils::RAW_BAYER_ID bayer = static_cast<utils::RAW_BAYER_ID>(raw_item->getBayer().toInt());
    utils::RAW_PACK_TYPE raw_pack_type = raw_item->getPacked();
    bool wdr_mode = raw_item->getFrameMode() == FRAME_MODE::WDR ? true : false;

    QString raw_path = raw_item->getFilePath();
    if (raw_path.indexOf(".yuv", 0, Qt::CaseInsensitive) != -1) {
        unique_ptr<int []> image = NULL;
        int size = width * height;
        int *raw_image;
        if (size % 2) {
            size = static_cast<int>(size * 1.5 + 1);
        } else {
            size = static_cast<int>(size * 1.5);
        }

        raw_image = new int[size];
        utils::readYuvIntData(raw_path, raw_image, size);
        QImage img = utils::YUV21toRGBImage((unsigned char *)raw_image, width, height);

        setImg(QPixmap::fromImage(img));
    } else {
        unsigned char *r = utils::readRawImage(raw_item->getFilePath(), static_cast<unsigned int>(width), static_cast<unsigned int>(height),
                bayer, bits, raw_pack_type, wdr_mode);

        if (r == nullptr) {
            qDebug() << "read raw image fail!" << endl;
            return;
        }
        QImage img(r, wdr_mode ? width / 2 : width, height, QImage::Format_RGB32);
        setImg(QPixmap::fromImage(img));
    }
}

void ImageViewerWidget::handleShowImage(QImage &image)
{
    setImg(QPixmap::fromImage(image));
}

void ImageViewerWidget::handleSaveImage()
{
    QImage image = image_label->getImage();
    if (image.isNull()) {
        qDebug("Image is null");
    }

    QString file_path = QFileDialog::getSaveFileName(this,
            tr("Save Image"), "", "*.bmp");

    image.save(file_path);
}

void ImageViewerWidget::scaleByFactor(double factor)
{
    scale_factor = factor;
    scaleImg();
}

void ImageViewerWidget::slotEnableGrid(int state)
{
    bool en = state > 0 ? true : false;

    if (image_label == nullptr) {
        return;
    }

    image_label->enableGrid(en);
    image_label->repaint();
}

void ImageViewerWidget::slotSelectFactor(QString text)
{
    if (text == "25%") {
        scaleByFactor(0.25);
    } else if (text == "50%") {
        scaleByFactor(0.5);
    } else if (text == "100%") {
        scaleByFactor(1.0);
    } else if (text == "200%") {
        scaleByFactor(2.0);
    } else if (text == "400%") {
        scaleByFactor(4.0);
    }
}

void ImageViewerWidget::setSeletedRect(QRectF rect)
{
    this->selected_rect = rect;
}

void ImageViewerWidget::setRawImageInfo(QString rawfilename, unsigned int width, unsigned int height, utils::RAW_BAYER_ID bayer, utils::RAW_BITS bits)
{
    this->raw_file_name = rawfilename;
    this->image_width = width;
    this->image_height = height;
    this->bayer = bayer;
    this->bits = bits;
}

void ImageViewerWidget::setLoadRawFailImg()
{
    qDebug() << "read raw image fail!" << endl;
    int textWidth = 320;
    int textHeight = 180;
    QImage errorImg(textWidth, textHeight, QImage::Format_RGB32);
    errorImg.fill(Qt::black);

    QPainter painter(&errorImg);
    painter.setPen(Qt::red);
    QFont font = painter.font();
    font.setPointSize(12);
    painter.setFont(font);
    painter.drawText(errorImg.rect(), Qt::AlignCenter, "Open Raw Fail!");

    setImg(QPixmap::fromImage(errorImg));
}

