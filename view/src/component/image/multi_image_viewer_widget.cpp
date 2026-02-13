#include "utils.hpp"
#include "multi_image_viewer_widget.hpp"
#include "raw_image_format_dialog.hpp"
#include "yuv_image_format_dialog.hpp"
#include "simple_pipe_dialog.hpp"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QImageReader>
#include <QScrollArea>
#include <QtCore>
#include <QPainter>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QtGlobal>
#include <QMessageBox>

static QMap<utils::IMAGE_TYPE, QString> img_type_str_map = {
    {utils::IMAGE_TYPE::IMAGE_RAW, "RAW"},
    {utils::IMAGE_TYPE::IMAGE_YUV, "YUV"},
    {utils::IMAGE_TYPE::IMAGE_YUV420, "YUV420"},
    {utils::IMAGE_TYPE::IMAGE_NV21, "NV21"},
    {utils::IMAGE_TYPE::IMAGE_NV12, "NV12"},
    {utils::IMAGE_TYPE::IMAGE_JPG, "JPG"},
    {utils::IMAGE_TYPE::IMAGE_BMP, "BMP"},
    {utils::IMAGE_TYPE::IMAGE_PPM, "PPM"},
    {utils::IMAGE_TYPE::IMAGE_UNKNOWN, "Unknown"}
};

MultiImageViewerWidget::MultiImageViewerWidget(QWidget *parent) :
    QWidget(parent),
    scale_factor(1.0)
{
    initUI();
    retranslateUi();
    setAcceptDrops(true);
    // simple pipe
    blc_attr.offset_r = blc_attr.offset_g = blc_attr.offset_b = 240;
    wbg_attr.wb_gain_r = 1500;
    wbg_attr.wb_gain_g = 1024;
    wbg_attr.wb_gain_b = 2000;
    simple_pipe_enable.pipe_enable = 0;
    simple_pipe_enable.blc_enable =
    simple_pipe_enable.wbg_enable =
    simple_pipe_enable.rgbgamma_enable = 1;

    raw_statistic_window = nullptr;
    level_hist_window = nullptr;
    color_hist_window = nullptr;
    simple_pipe_dialog = nullptr;

    selected_rect.setRect(100, 200, 1920, 1080);
}

void MultiImageViewerWidget::setFilePathHidden()
{
    file_path_line_edit->setHidden(true);
}

void MultiImageViewerWidget::setFileButtonHidden()
{
    choose_file_btn->setHidden(true);
}

void MultiImageViewerWidget::setColorHistogramButtonHidden()
{
    color_histogram_btn->setHidden(true);
}

void MultiImageViewerWidget::setImg(QPixmap img)
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
    scaleImg();

    emit sigSetImage(image_label->pixmap());
}

void MultiImageViewerWidget::setImageLabel(ImageLabel *label)
{
    image_label = label;
    image_label->setScaledContents(true);
    scrollArea->setWidget(image_label);
}

int MultiImageViewerWidget::setRawImgData(QByteArray &data, unsigned int width, unsigned int height,
        utils::RAW_BAYER_ID bayer_id, utils::RAW_BITS bits, utils::RAW_PACK_TYPE raw_pack_type)
{
    // before set raw data, should clear image data first
    clearImageData();
    unsigned int raw_size = utils::getRawSize(width, height, bits, raw_pack_type);

    if (raw_size == 0) {
        qDebug() << "raw size is 0!" << endl;
        return -1;
    }

    int count = data.size() / raw_size;
    int res = data.size() % raw_size;

    qDebug() << "file data size: " << data.size() <<", sigle frame size: " << raw_size << endl;
    qDebug() << "calucate frame: " << count << ", res: " << res << endl;

    if (res != 0) {
        qDebug() << "res is not 0, so file size and sigle size not match!" << endl;
        return -1;
    }

    total_frame_cnt = count;

    for (int i = 0; i < count; i++)
    {
        QByteArray frame(data.data()+i*raw_size, raw_size);

        raw_list.append(frame);
    }
    // just convert one frame
    img_list.append(utils::convertRaw2RGBImage(raw_list[0], width, height,
            (utils::RAW_BAYER_ID)(bayer_id%4), bits, raw_pack_type,
            &blc_attr, &wbg_attr, &simple_pipe_enable));

    img_type = utils::IMAGE_TYPE::IMAGE_RAW;

    mul_width = width;
    mul_height = height;
    mul_bits = bits;
    mul_bayer = static_cast<utils::RAW_BAYER_ID>(bayer_id % 4);
    mul_raw_pack_type = raw_pack_type;

    setDefaultImgDisplay();

    return 0;
}

void MultiImageViewerWidget::setYUVImgData(QByteArray &data, YUV_HEADER *header)
{
    // before set yuv data, should clear image data first
    clearImageData();

    int i = 0, num_frame = 0;

    num_frame = data.size() / header->size;
    for (i = 0; i < num_frame; i++) {
        QByteArray frame(data.data() + i * header->size, header->size);
        yuv_list.append(frame);
        img_list.append(utils::convertYUV2RGBImage(frame, header));
    }

    total_frame_cnt = num_frame;

    if (header->pixelFormat == PIXEL_FORMAT_YUV_PLANAR_420) {
        img_type = utils::IMAGE_TYPE::IMAGE_YUV420;
    } else if (header->pixelFormat == PIXEL_FORMAT_NV21) {
        img_type = utils::IMAGE_TYPE::IMAGE_NV21;
    } else if (header->pixelFormat == PIXEL_FORMAT_NV12) {
        img_type = utils::IMAGE_TYPE::IMAGE_NV12;
    } else {
        img_type = utils::IMAGE_TYPE::IMAGE_YUV;
    }

    mul_width = header->width;
    mul_height = header->height;
    mul_bits = utils::RAW_BITS::BITS_8;

    setDefaultImgDisplay();
}

void MultiImageViewerWidget::setJPGImgData(QList<QImage> list)
{
    // before set yuv data, should clear image data first
    clearImageData();

    img_list = list;
    total_frame_cnt = img_list.size();
    img_type = utils::IMAGE_TYPE::IMAGE_JPG;
    setDefaultImgDisplay();
}

void MultiImageViewerWidget::setDefaultImgDisplay()
{
    if (img_list.size() > 0) {
        cur_idx = 0;
        setImg(QPixmap::fromImage(img_list.value(cur_idx)));
        updatePrompt(img_type, cur_idx+1, total_frame_cnt);
    }
}

void MultiImageViewerWidget::updatePrompt(utils::IMAGE_TYPE img_type, int cur, int total)
{
    if (prompt_label) {
        prompt_label->setText(QString("%1 Frame: %2/%3").arg(img_type_str_map[img_type]).arg(cur).arg(total));
    }
}

double MultiImageViewerWidget::getScaleFactor()
{
    return this->scale_factor;
}

void MultiImageViewerWidget::dragEnterEvent(QDragEnterEvent *event)
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

void MultiImageViewerWidget::dropEvent(QDropEvent *event)
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

void MultiImageViewerWidget::changeEvent(QEvent *event)
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

void MultiImageViewerWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
}

void MultiImageViewerWidget::wheelEvent(QWheelEvent  *event)
{
    QWidget::wheelEvent(event);

    if (image_label->pixmap() == nullptr) {
        return;
    }

    scale_factor += event->delta() / 1000.0;
    scale_factor = qBound(0.1, scale_factor, 10.0);

    scaleImg();
}

void MultiImageViewerWidget::initUI()
{
    file_path_line_edit = new QLineEdit();
    file_path_line_edit->setReadOnly(true);

    choose_file_btn = new QPushButton();
    connect(choose_file_btn, &QPushButton::clicked, this, &MultiImageViewerWidget::openImg);

    simple_pipe_btn = new QPushButton(tr("Simple Pipe"));
    connect(simple_pipe_btn, &QPushButton::clicked, this, &MultiImageViewerWidget::handleSimplePipeBnt);

    en_grid = new QCheckBox("En Grid");
    en_grid->setHidden(true);
    connect(en_grid, &QCheckBox::stateChanged, this, &MultiImageViewerWidget::slotEnableGrid);

    zoom_in_btn = new QPushButton();
    zoom_out_btn = new QPushButton();
    zoom_in_btn->setDisabled(true);
    zoom_out_btn->setDisabled(true);
    connect(zoom_in_btn, &QPushButton::clicked, this, &MultiImageViewerWidget::zoomin);
    connect(zoom_out_btn, &QPushButton::clicked, this, &MultiImageViewerWidget::zoomout);

    zoom_box = new QComboBox;
    zoom_box->addItems({"25%", "50%", "100%", "200%", "400%"});
    zoom_box->setCurrentIndex(2);
    zoom_box->setDisabled(true);
    connect(zoom_box, &QComboBox::currentTextChanged, this, &MultiImageViewerWidget::slotSelectFactor);

    factor_label = new QLabel();
    scrollArea = new QScrollArea();
    scrollArea->setBackgroundRole(QPalette::Dark);

    raw_statistics_btn = new QPushButton();
    raw_statistics_btn->setHidden(true);
    connect(raw_statistics_btn, &QPushButton::clicked, this, &MultiImageViewerWidget::handleOpenRawStatistics);
    connect(raw_statistics_btn, &QPushButton::clicked, this, &MultiImageViewerWidget::enableRect);

    color_histogram_btn = new QPushButton();
    connect(color_histogram_btn, &QPushButton::clicked, this, &MultiImageViewerWidget::handleOpenColorHistogram);

    level_meter_btn = new QPushButton();
    level_meter_btn->setHidden(true);
    connect(level_meter_btn, &QPushButton::clicked, this, &MultiImageViewerWidget::handleOpenLevelMeter);
    connect(level_meter_btn, &QPushButton::clicked, this, &MultiImageViewerWidget::enableRect);

    save_btn = new QPushButton();
    connect(save_btn, &QPushButton::clicked, this, &MultiImageViewerWidget::handleSaveImage);

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(file_path_line_edit);
    layout->addWidget(choose_file_btn);
    layout->addWidget(simple_pipe_btn);
    layout->addWidget(en_grid);
    layout->addWidget(zoom_in_btn);
    layout->addWidget(zoom_out_btn);
    layout->addWidget(zoom_box);
    layout->addWidget(factor_label);
    layout->addWidget(save_btn);

    QHBoxLayout *analyer_layout = new QHBoxLayout();
    analyer_layout->addWidget(color_histogram_btn);
    analyer_layout->addWidget(level_meter_btn);
    analyer_layout->addWidget(raw_statistics_btn);

    prompt_label = new QLabel(tr("Unknown Frame: 0/0"));
    prev_btn = new QPushButton(tr("Previous"));
    connect(prev_btn, &QPushButton::clicked, this, std::bind(&MultiImageViewerWidget::handleImgNavigat, this, -1));
    next_btn = new QPushButton(tr("Next"));
    connect(next_btn, &QPushButton::clicked, this, std::bind(&MultiImageViewerWidget::handleImgNavigat, this, 1));
    QHBoxLayout *control_layout = new QHBoxLayout();
    control_layout->addWidget(prompt_label);
    control_layout->addWidget(prev_btn);
    control_layout->addWidget(next_btn);

    QHBoxLayout *bottom_layout = new QHBoxLayout();
    status_label = new QLabel(tr("X=0,Y=0,R=0,G=0,B=0<br>Y=0,U=0,V=0,H=0,S=0,V=0"));
    bottom_layout->addWidget(status_label);
    bottom_layout->addStretch();

    // crop rect
    rect_h_layout = new QHBoxLayout();
    width_lab = new QLabel("Width:");
    width_lab->setFixedSize(50,25);
    width_edit = new QLineEdit;
    width_edit->setFixedSize(50,25);
    width_edit->setText("0");
    width_edit->setEnabled(false);
    height_lab = new QLabel("Height:");
    height_lab->setFixedSize(50,25);
    height_edit = new QLineEdit;
    height_edit->setFixedSize(50,25);
    height_edit->setText("0");
    height_edit->setEnabled(false);

    cropX_lab = new QLabel("CropX:");
    cropX_lab->setFixedSize(50,25);
    cropX_edit = new QLineEdit;
    cropX_edit->setFixedSize(50,25);
    cropX_edit->setText("0");
    connect(cropX_edit,&QLineEdit::editingFinished,[=]{
        int x = cropX_edit->text().toInt();
        int width = width_edit->text().toInt();
        x = (x + 1) / 2 * 2;
        x = qBound(1, x, width - 2);
        cropX_edit->setText(QString::number(x));
        int w = cropW_edit->text().toInt();
        if (w > (width-x)) {
            cropW_edit->setText(QString::number(width-x));
        }
        handleCropInfoToRect();
    });
    cropY_lab = new QLabel("CropY:");
    cropY_lab->setFixedSize(50,25);
    cropY_edit = new QLineEdit;
    cropY_edit->setFixedSize(50,25);
    cropY_edit->setText("0");
    connect(cropY_edit,&QLineEdit::editingFinished,[=]{
        int y = cropY_edit->text().toInt();
        int height = height_edit->text().toInt();
        y = (y + 1) / 2 * 2;
        y = qBound(1, y, height - 2);
        cropY_edit->setText(QString::number(y));
        int h = cropH_edit->text().toInt();
        if (h > (height-y)) {
            cropH_edit->setText(QString::number(height-y));
        }
        handleCropInfoToRect();
    });
    cropW_lab = new QLabel("CropW:");
    cropW_lab->setFixedSize(50,25);
    cropW_edit = new QLineEdit;
    cropW_edit->setFixedSize(50,25);
    cropW_edit->setText("0");
    connect(cropW_edit,&QLineEdit::editingFinished,[=]{
        int w = cropW_edit->text().toInt();
        int width = width_edit->text().toInt();
        int x = cropX_edit->text().toInt();
        w = (w + 1) / 2 * 2;
        w = qBound(1, w, width - x - 1);
        cropW_edit->setText(QString::number(w));
        handleCropInfoToRect();
    });
    cropH_lab = new QLabel("CropH:");
    cropH_lab->setFixedSize(50,25);
    cropH_edit = new QLineEdit;
    cropH_edit->setFixedSize(50,25);
    cropH_edit->setText("0");
    connect(cropH_edit,&QLineEdit::editingFinished,[=]{
        int h = cropH_edit->text().toInt();
        int height = height_edit->text().toInt();
        int y = cropY_edit->text().toInt();
        h = (h + 1) / 2 * 2;
        h = qBound(1, h, height - y - 1);
        cropH_edit->setText(QString::number(h));
        handleCropInfoToRect();
    });

    rect_h_layout->addWidget(width_lab);
    rect_h_layout->addWidget(width_edit);
    rect_h_layout->addWidget(height_lab);
    rect_h_layout->addWidget(height_edit);
    rect_h_layout->addWidget(cropX_lab);
    rect_h_layout->addWidget(cropX_edit);
    rect_h_layout->addWidget(cropY_lab);
    rect_h_layout->addWidget(cropY_edit);
    rect_h_layout->addWidget(cropW_lab);
    rect_h_layout->addWidget(cropW_edit);
    rect_h_layout->addWidget(cropH_lab);
    rect_h_layout->addWidget(cropH_edit);
    rect_h_layout->addStretch(1);

    for (int i = 0; i < rect_h_layout->count(); i++) {
        QLayoutItem *item = rect_h_layout->itemAt(i);
        QWidget *widget = item->widget();
        if (widget) {
            widget->hide();
        }
    }

    QVBoxLayout *vlayout = new QVBoxLayout();
    vlayout->addLayout(layout);
    vlayout->addLayout(analyer_layout);
    vlayout->addLayout(control_layout);
    vlayout->addWidget(scrollArea);
    vlayout->addLayout(bottom_layout);
    vlayout->addLayout(rect_h_layout);

    setLayout(vlayout);

    connect(this, &MultiImageViewerWidget::sigSetImage, this, &MultiImageViewerWidget::handleSetImage);
}

void MultiImageViewerWidget::retranslateUi()
{
    factor_label->setText(QString::number(scale_factor, 'f', 2) + "x");
    choose_file_btn->setText(tr("Open File"));
    zoom_in_btn->setText(tr("Zoom In"));
    zoom_out_btn->setText(tr("Zoom out"));
    raw_statistics_btn->setText(tr("Roi Histogram"));
    level_meter_btn->setText(tr("Level Meter"));
    color_histogram_btn->setText(tr("Color Histogram"));
    save_btn->setText(tr("Save"));
}

void MultiImageViewerWidget::setChooseFileBtnName()
{
    choose_file_btn->setText(tr("Import Image"));
}

void MultiImageViewerWidget::openImgPath(QString img_path)
{

    if (img_path.isEmpty()) {
        return;
    }

    // set the image path
    file_path = img_path;
    file_path_line_edit->setText(img_path);

    if (img_path.endsWith(".raw")) {
        // Choose raw image format
        RawImageFormatDialog *formt_dialog = new RawImageFormatDialog;
        formt_dialog->setFilepath(img_path);
         if(formt_dialog->exec()) {

            std::tuple<QString, QString> size = formt_dialog->getSize();
            unsigned int width = std::get<0>(size).toUInt();
            unsigned int height = std::get<1>(size).toUInt();
            utils::RAW_BAYER_ID bayer = static_cast<utils::RAW_BAYER_ID>(formt_dialog->getBayer().toInt());
            utils::RAW_BITS bits = static_cast<utils::RAW_BITS>(formt_dialog->getBits().toInt());
            utils::RAW_PACK_TYPE raw_pack_type = formt_dialog->getPacked();
            QFile img_file(img_path);
            if (!img_file.open(QIODevice::ReadOnly)) return;
            QByteArray file_data = img_file.readAll();
            if (setRawImgData(file_data, width, height, bayer, bits, raw_pack_type) != 0) {
                setLoadRawFailImg();
            }
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
            setYUVImgData(file_data, &header);
        }
    } else {
        clearImageData();
        QImageReader reader(img_path);
        QImage img = reader.read();
        QFileInfo fileInfo(img_path);

        img_type = utils::IMAGE_TYPE::IMAGE_UNKNOWN;
        for (const auto& pair : img_type_str_map.toStdMap()) {
            if (pair.second.toLower() == fileInfo.suffix().toLower()) {
                img_type = pair.first;
            }
        }

        mul_width = img.width();
        mul_height = img.height();
        img_list.append(img);
        total_frame_cnt = 1;
        setDefaultImgDisplay();
    }
}

bool MultiImageViewerWidget::isSupportedFileExtension(QString extension)
{
    if (extension == "png" || extension == "tiff" || extension == "raw" || extension == "jpg" || extension == "yuv") {
        return true;
    }
    return false;
}

void MultiImageViewerWidget::openImg()
{
    QString img_path = QFileDialog::getOpenFileName(this, tr("Open File"), "", "*.jpg *.png *.raw *.tiff *.yuv");
    openImgPath(img_path);
}

void MultiImageViewerWidget::scaleByFactor(double factor)
{
    scale_factor = factor;
    scaleImg();
}

void MultiImageViewerWidget::showEnGridCheckBox()
{
    en_grid->setHidden(false);
}

void MultiImageViewerWidget::slotEnableGrid(int state)
{
    bool en = state > 0 ? true : false;

    if (image_label == nullptr) {
        return;
    }

    image_label->enableGrid(en);
    image_label->repaint();
}

void MultiImageViewerWidget::zoomin()
{
    scale_factor = scale_factor / 0.8;
    scaleImg();
}

void MultiImageViewerWidget::zoomout()
{
    scale_factor = scale_factor * 0.8;
    scaleImg();
}

void MultiImageViewerWidget::slotSelectFactor(QString text)
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

void MultiImageViewerWidget::scaleImg()
{
    factor_label->setText(QString::number(scale_factor, 'f', 2) + "x");
    image_label->setScaleFactor(scale_factor);
    image_label->resize(scale_factor * image_label->pixmap()->size());
}

void MultiImageViewerWidget::handleOpenColorHistogram()
{
    if (color_hist_window == nullptr) {
        color_hist_window = new ColorHistogramWindow();
    }

    color_hist_window->show();

    if ((image_label->pixmap()) && (!image_label->pixmap()->isNull())) {
        color_hist_window->handleUpdatePoints(image_label->pixmap());
    }
}

void MultiImageViewerWidget::handleOpenLevelMeter()
{
    if (level_hist_window == nullptr) {
        level_hist_window = new LevelMeterHistogramWindow;
        connect(level_hist_window, &LevelMeterHistogramWindow::sigChildWindowEvent, this, &MultiImageViewerWidget:: handleChildWindowEvent);
    }

    if (level_hist_window->isHidden()) {
        roi_rect_ref_cnt++;
        level_hist_window->show();
        if ((image_label->pixmap()) && (!image_label->pixmap()->isNull())) {
        updateLevelMeterHistogram(image_label->pixmap());
        }
    }
}

void MultiImageViewerWidget::slotPositionChange(QString text)
{
    if (status_label) {
        status_label->setText(QString("%1").arg(text));
    }
}

void MultiImageViewerWidget::handleShowRawItem(RawImageItem *raw_item)
{
    if (raw_item->getFilePath().isEmpty()) {
        return;
    }

    int width = raw_item->getWidth();
    int height = raw_item->getHeight();
    utils::RAW_BITS bits = static_cast<utils::RAW_BITS>(raw_item->getBits().toInt());
    utils::RAW_BAYER_ID bayer = static_cast<utils::RAW_BAYER_ID>(raw_item->getBayer().toInt());
    utils::RAW_PACK_TYPE raw_pack_type = raw_item->getPacked();
    unsigned char *r = utils::readRawImage(raw_item->getFilePath(), static_cast<unsigned int>(width), static_cast<unsigned int>(height),
            bayer, bits, raw_pack_type);
    QImage img(r, width, height, QImage::Format_RGB32);
    setImg(QPixmap::fromImage(img));
}

void MultiImageViewerWidget::handleShowImage(QImage &image)
{
    setImg(QPixmap::fromImage(image));
}

void MultiImageViewerWidget::clearImageData()
{
    raw_list.clear();
    yuv_list.clear();
    img_list.clear();
    total_frame_cnt = 0;
    image_label->clear();
    updatePrompt(utils::IMAGE_TYPE::IMAGE_UNKNOWN, 0, 0);
}

void MultiImageViewerWidget::handleSaveImage()
{
    QImage image = image_label->getImage();
    if (image.isNull()) {
        qDebug("Image is null");
        return;
    }

    QString file_path = QFileDialog::getSaveFileName(this, tr("Save Image"), "", "*.bmp");
    image.save(file_path);
}

void MultiImageViewerWidget::handleImgNavigat(int dir)
{
    if (dir != -1 && dir != 1) return;
    if (cur_idx + dir >= total_frame_cnt || cur_idx + dir < 0) return;

    cur_idx += dir;
    if (img_list.size() <= cur_idx && img_type == utils::IMAGE_TYPE::IMAGE_RAW) {
        // raw -> rgb
        int cur_img_list_size = img_list.size();
        for (int idx = cur_img_list_size; idx <= cur_idx; idx++) {
            img_list.append(utils::convertRaw2RGBImage(raw_list[idx], mul_width, mul_height,
            mul_bayer, mul_bits, mul_raw_pack_type,
            &blc_attr, &wbg_attr, &simple_pipe_enable));
        }
    }

    setImg(QPixmap::fromImage(img_list.value(cur_idx)));
    updatePrompt(img_type, cur_idx + 1, total_frame_cnt);
}

void MultiImageViewerWidget::reset()
{
    //TODO clear img
}

void MultiImageViewerWidget::triggerGrid3AEnable(bool en)
{
    if (en) {
        image_label->setEnGridMode(STATE_AF);
    } else {
        image_label->setEnGridMode(STATE_NONE);
    }

    image_label->repaint();
}

void MultiImageViewerWidget::setFvTable(QVector<QVector<int>> &fv)
{
    image_label->setFvTable(fv);
}

void MultiImageViewerWidget::setLoadRawFailImg()
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

void MultiImageViewerWidget::updateSimplePipeAttr()
{
    simple_pipe_dialog->setSimplePipeAttr(&blc_attr, &wbg_attr, &simple_pipe_enable);

    if (img_type == utils::IMAGE_TYPE::IMAGE_RAW) {
        // clear the img list
        img_list.clear();

        for (int idx = 0; idx <= cur_idx; idx++) {
            img_list.append(utils::convertRaw2RGBImage(raw_list[idx], mul_width, mul_height,
            mul_bayer, mul_bits, mul_raw_pack_type,
            &blc_attr, &wbg_attr, &simple_pipe_enable));
        }

        // display the cur idx
        setImg(QPixmap::fromImage(img_list.value(cur_idx)));
        updatePrompt(img_type, cur_idx + 1, total_frame_cnt);
    }
}

void MultiImageViewerWidget::handleSimplePipeBnt()
{
    qDebug() << "click the  simple pipe btn" << endl;

    if (simple_pipe_dialog == nullptr) {
        simple_pipe_dialog = new SimplePipeDialog;
        simple_pipe_dialog->getSimplePipeAttr(&blc_attr, &wbg_attr, &simple_pipe_enable);
        connect(simple_pipe_dialog, &SimplePipeDialog::updateSimplePipeAttr,
                this, &MultiImageViewerWidget::updateSimplePipeAttr);
    }

    simple_pipe_dialog->show();
}

void MultiImageViewerWidget::hideSimplePipeBtn()
{
    simple_pipe_btn->setHidden(true);
}

void MultiImageViewerWidget::handleOpenRawStatistics()
{
    if (raw_statistic_window == nullptr) {
        raw_statistic_window = new RawStatisticsWindow();
        connect(raw_statistic_window, &RawStatisticsWindow::sigChildWindowEvent, this, &MultiImageViewerWidget:: handleChildWindowEvent);
    }

    if (raw_statistic_window->isHidden()) {
        roi_rect_ref_cnt++;
        raw_statistic_window->show();

        if ((image_label->pixmap()) && (!image_label->pixmap()->isNull())) {
            updateRawStatistic(image_label->pixmap(), true);
        }
    }
}

void MultiImageViewerWidget::handleChildWindowEvent(int window_type)
{
    if (window_type == 0 || window_type == 1) {
        roi_rect_ref_cnt--;
        disableRect();
    }
}

void MultiImageViewerWidget::showRawStatisticBtn()
{
    raw_statistics_btn->setHidden(false);
}

void MultiImageViewerWidget::showLevelMeterBtn()
{
    level_meter_btn->setHidden(false);
}

void MultiImageViewerWidget::setSeletedRect(QRectF rect)
{
    this->selected_rect = rect;
}

void MultiImageViewerWidget::updateLevelMeterHistogram(const QPixmap *image)
{
    if (level_hist_window != nullptr) {
        level_hist_window->setStatusBarState("data is calculating...");
        QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
        level_hist_window->handleUpdatePointsInfo(image, selected_rect);
        level_hist_window->setStatusBarState("calculated data is finished!");
    }

    if (img_type != utils::IMAGE_TYPE::IMAGE_RAW) {
        return;
    }

}

void MultiImageViewerWidget::updateRawStatistic(const QPixmap *image, bool is_update_colortype)
{
    if (raw_statistic_window != nullptr) {
        if (img_type == utils::IMAGE_TYPE::IMAGE_RAW) {
            unsigned char *unpack_raw = utils::convertAnyRaw2Raw16(reinterpret_cast<const unsigned char *>(raw_list[cur_idx].constData()),
                           mul_width, mul_height, mul_bits, mul_raw_pack_type);
            raw_statistic_window->setStatusBarState("data is calculating...");
            QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
            raw_statistic_window->handleUpdatePointsInfo(unpack_raw, mul_width, mul_height,
                                     mul_bayer, mul_bits, selected_rect, is_update_colortype);
            raw_statistic_window->setStatusBarState("calculated data is finished!");
        } else {
            raw_statistic_window->setStatusBarState("data is calculating...");
            QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
            raw_statistic_window->handleUpdatePointsInfo(image, selected_rect, is_update_colortype);
            raw_statistic_window->setStatusBarState("calculated data is finished!");
        }
    }
}

void MultiImageViewerWidget::handleSetImage(const QPixmap *image)
{
    // color histgram
    if (color_hist_window != nullptr && !color_hist_window->isHidden()) {
        color_hist_window->handleUpdatePoints(image);
    }

    level_meter_btn->setEnabled(true);
    raw_statistics_btn->setEnabled(true);
    // image level meter
    if (level_hist_window != nullptr && !level_hist_window->isHidden()) {
        updateLevelMeterHistogram(image);
    }
    // image statistic
    if (raw_statistic_window != nullptr && !raw_statistic_window->isHidden()) {
        updateRawStatistic(image, true);
    }
}

void MultiImageViewerWidget::handleCropInfoToRect()
{
    int x = cropX_edit->text().toInt();
    int y = cropY_edit->text().toInt();
    int w = cropW_edit->text().toInt();
    int h = cropH_edit->text().toInt();

    image_label->setRepaintEvent(true);
    image_label->updateSingleRect(x, y, w, h);
}

void MultiImageViewerWidget::handleRectToCropInfo(QRectF rect, double factor)
{
    this->scale_factor = factor;
    double rect_w, rect_h, center_x, center_y;
    int w, h, x, y;
    QImage image = image_label->getImage();
    QRectF init_rect;

    rect_w = rect.width();
    rect_h = rect.height();
    center_x = rect.x();
    center_y = rect.y();

    w = ((int)(rect_w / factor + 0.5) + 1) / 2 * 2;
    h = ((int)(rect_h / factor + 0.5) + 1) / 2 * 2;
    x = ((int)(center_x / factor + 0.5) + 1) / 2 * 2;
    y = ((int)(center_y / factor + 0.5) + 1) / 2 * 2;

    x = qBound(1, x, image.width() - 2);
    y = qBound(1, y, image.height() - 2);
    w = qBound(1, w, image.width() - x - 2);
    if (w < 2) {
        w = 2;
        x = image.width() - 2 * 2;
    }
    h = qBound(1, h, image.height() - y - 2);
    if (h < 2) {
        h = 2;
        y = image.height() - 2 * 2;
    }

    width_edit->setText(QString::number(image.width()));
    height_edit->setText(QString::number(image.height()));
    cropX_edit->setText(QString::number(x));
    cropY_edit->setText(QString::number(y));
    cropW_edit->setText(QString::number(w));
    cropH_edit->setText(QString::number(h));
    init_rect.setRect(x, y, w, h);
    setSeletedRect(init_rect);
    QPixmap tempPixmap = QPixmap::fromImage(image);
    // update level meter and raw statistic window
    updateLevelMeterHistogram(&tempPixmap);
    updateRawStatistic(&tempPixmap, false);
}

void MultiImageViewerWidget::enableRect()
{
    // set mode
    image_label->setMode(ImageLabel::SINGLE_RECT);
    // show the rect widget
    for (int i = 0; i < rect_h_layout->count(); i++) {
        QLayoutItem *item = rect_h_layout->itemAt(i);
        QWidget *widget = item->widget();
        if (widget) {
            widget->show();
        }
    }
}

void MultiImageViewerWidget::disableRect()
{
    if (roi_rect_ref_cnt > 0) {
        return;
    }
    // set mode
    if (image_label->getMode() == ImageLabel::SINGLE_RECT) {
        image_label->setMode(ImageLabel::NORMAL);
    }
    // hide the rect widget
    for (int i = 0; i < rect_h_layout->count(); i++) {
        QLayoutItem *item = rect_h_layout->itemAt(i);
        QWidget *widget = item->widget();
        if (widget) {
            widget->hide();
        }
    }
}

void MultiImageViewerWidget::roiRectRefSub()
{
    roi_rect_ref_cnt--;
}


void MultiImageViewerWidget::roiRectRefAdd()
{
    roi_rect_ref_cnt++;
}

int MultiImageViewerWidget::getCenterRoiAvgLuma(int &r, int &g, int &b)
{
    using namespace utils;
    if (raw_list.empty()) {
        return -1;
    }

    const int roi_w = 32;
    const int roi_h = 18;
    int x_s = mul_width / 2 - roi_w / 2;
    int y_s = mul_height / 2 - roi_h / 2;
    int x_e = mul_width / 2 + roi_w / 2;
    int y_e = mul_height / 2 + roi_h / 2;
    COLOR_RGB e_color[2][2];
    unsigned char *raw_src = utils::convertAnyRaw2Raw16(reinterpret_cast<const unsigned char *>(raw_list[0].constData()),
            mul_width, mul_height, mul_bits, mul_raw_pack_type);

    unsigned short *src = reinterpret_cast<unsigned short *>(raw_src);

    if (mul_bayer == BAYER_ID_RG) {
        e_color[0][0] = COLOR_R;
        e_color[0][1] = COLOR_G;
        e_color[1][0] = COLOR_G;
        e_color[1][1] = COLOR_B;
    } else if (mul_bayer == BAYER_ID_GR) {
        e_color[0][0] = COLOR_G;
        e_color[0][1] = COLOR_R;
        e_color[1][0] = COLOR_B;
        e_color[1][1] = COLOR_G;
    } else if (mul_bayer == BAYER_ID_BG) {
        e_color[0][0] = COLOR_B;
        e_color[0][1] = COLOR_G;
        e_color[1][0] = COLOR_G;
        e_color[1][1] = COLOR_R;
    } else {
        e_color[0][0] = COLOR_G;
        e_color[0][1] = COLOR_B;
        e_color[1][0] = COLOR_R;
        e_color[1][1] = COLOR_G;
    }

    unsigned int sum_r = 0;
    unsigned int sum_g = 0;
    unsigned int sum_b = 0;
    int cnt_r = 0;
    int cnt_g = 0;
    int cnt_b = 0;

    unsigned int tmp_val = 0;

    for (int y = y_s; y < y_e; ++y) {
        for (int x = x_s; x < x_e; x++) {
            tmp_val = src[y * mul_width + x] >> 4; // left shift 4 bits
            qDebug() << "(x, y)=(" << x << ", " << y << "), pixel value: " << tmp_val;
            COLOR_RGB color_id = e_color[y%2][x%2];
            switch (color_id) {
            case COLOR_G: {
                sum_g += tmp_val;
                cnt_g++;
                break;
            }
            case COLOR_R: {
                sum_r += tmp_val;
                cnt_r++;
                break;
            }
            case COLOR_B: {
                sum_b += tmp_val;
                cnt_b++;
                break;
            }
            default:
                break;
            }
        }
    }

    r = sum_r / cnt_r;
    g = sum_g / cnt_g;
    b = sum_b / cnt_b;

    qDebug() << "getCenterRoiAvgLuma, size: " << roi_w << "x" << roi_h <<
                ", avg_r: " << r << ", avg_g: " << g << ", avg_b: " << b << endl;
    qDebug() << "pixel count, r: " << cnt_r << ", g: " << cnt_g << ", b: " << cnt_b << endl;

    free(raw_src);
    return 0;
}
