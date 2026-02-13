#include "continuous_raw_window.hpp"
#include "utils.hpp"
#include "value_validator.hpp"
#include "global_data.hpp"

#include <QLabel>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>

const int MIN_WINDOW_WIDTH = 960;
const int MIN_WINDOW_HEIGHT = 580;

#define IMAGE_MIN_WIDTH (10)
#define IMAGE_MIN_HEIGHT (10)

#define CV183X_LINE_BUFFER_WIDTH 2688
#define CV183X_TILE_START_INDEX 1920
#define CV182X_LINE_BUFFER_WIDTH 2304
#define CV182X_TILE_START_INDEX 1536

ContinuousRawWindow::ContinuousRawWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle(tr("Continuous Raw"));
    setWindowFlags(Qt::WindowMaximizeButtonHint|Qt::WindowCloseButtonHint);
    delay = new QTimer(this);
    delay->setSingleShot(true);
    initUI();
    initEvent();
}

void ContinuousRawWindow::initUI()
{
    main_widget = new QWidget();
    main_widget->setMinimumSize(QSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT));
    main_layout = new QHBoxLayout();

    QWidget *widget = new QWidget();
    QVBoxLayout *left_layout = new QVBoxLayout();
    widget->setLayout(left_layout);

    preview_group = new QGroupBox("Preview");
    QHBoxLayout *preview_layout = new QHBoxLayout();
    preview_group->setLayout(preview_layout);
    preview_btn = new QPushButton("Capture");
    preview_btn->setFixedWidth(120);
    connect(preview_btn,&QPushButton::clicked,this,&ContinuousRawWindow::GetSingleYuv);
    preview_cancel_btn = new QPushButton("Cancel");
    preview_cancel_btn->setFixedWidth(120);
    preview_cancel_btn->setEnabled(false);
    connect(preview_cancel_btn,&QPushButton::clicked,this,&ContinuousRawWindow::CancelGetSingleYuv);
    preview_layout->addWidget(preview_btn,0,Qt::AlignLeft);
    preview_layout->addWidget(preview_cancel_btn,0,Qt::AlignLeft);

    roi_group = new QGroupBox("ROI");
    QGridLayout * roi_layout = new QGridLayout;
    roi_group->setLayout(roi_layout);
    roi_group->setFixedHeight(200);
    QLabel *frames_lab = new QLabel("Frames:");
    frames_edit = new QLineEdit("2");
    frames_edit->setMaxLength(8);
    frames_edit->setFixedWidth(80);
    connect(frames_edit, &QLineEdit::editingFinished, [=]{
        int frames = frames_edit->text().toInt();
        frames = qMax(2, frames);
        frames_edit->setText(QString::number(frames));
    });
    QLabel *frames_rang_lab = new QLabel("(>=2)");
    QLabel *w_lab = new QLabel("Width:");
    w_edit = new QLineEdit("1920");
    w_edit->setReadOnly(true);
    w_edit->setFixedWidth(80);
    QLabel *h_lab = new QLabel("Height:");
    h_edit = new QLineEdit("1080");
    h_edit->setReadOnly(true);
    h_edit->setFixedWidth(80);
    QLabel *cropX_lab = new QLabel("CropX:");
    cropX_edit = new QLineEdit("0");
    cropX_edit->setReadOnly(true);
    cropX_edit->setEnabled(false);
    cropX_edit->setFixedWidth(80);
    QLabel *cropY_lab = new QLabel("CropY:");
    cropY_edit = new QLineEdit("0");
    cropY_edit->setMaxLength(8);
    cropY_edit->setFixedWidth(80);
    connect(cropY_edit, &QLineEdit::editingFinished, [=]{
        int y = cropY_edit->text().toInt()/2*2;
        int h = h_edit->text().toInt();
        y = qBound(0, y, h-IMAGE_MIN_HEIGHT);
        cropY_edit->setText(QString::number(y));
    });
    QLabel *cropW_lab = new QLabel("CropW:");
    cropW_edit = new QLineEdit("1920");
    cropW_edit->setMaxLength(8);
    cropW_edit->setFixedWidth(80);
    connect(cropW_edit, &QLineEdit::editingFinished, [=]{
        int cropW = cropW_edit->text().toInt()/4*4;
        int w = w_edit->text().toInt();
        cropW = qBound(IMAGE_MIN_WIDTH, cropW, w);
        cropW_edit->setText(QString::number(cropW));
    });
    QLabel *cropH_lab = new QLabel("CropH:");
    cropH_edit = new QLineEdit("1080");
    cropH_edit->setMaxLength(8);
    cropH_edit->setFixedWidth(80);
    connect(cropH_edit, &QLineEdit::editingFinished, [=]{
        int cropH = cropH_edit->text().toInt()/2*2;
        int h = h_edit->text().toInt();
        int y = cropY_edit->text().toInt();
        cropH = qBound(IMAGE_MIN_HEIGHT, cropH, h-y);
        cropH_edit->setText(QString::number(cropH));
    });

    QPushButton *maxframes_btn = new QPushButton("Suggest Frame");
    maxframes_btn->setFixedWidth(120);
    connect(maxframes_btn, &QPushButton::clicked, this, &ContinuousRawWindow::GetRawDumpMaxFrames);
    maxframes_edit = new QLineEdit("0");
    maxframes_edit->setReadOnly(true);
    maxframes_edit->setFixedWidth(80);
    maxframes_edit->setToolTip("frames suggest capture at max!");

    roi_layout->addWidget(frames_lab,0,0,Qt::AlignLeft);
    roi_layout->addWidget(frames_edit,0,1,Qt::AlignLeft);
    roi_layout->addWidget(frames_rang_lab,0,2,Qt::AlignLeft);
    roi_layout->addWidget(w_lab,1,0,Qt::AlignLeft);
    roi_layout->addWidget(w_edit,1,1,Qt::AlignLeft);
    roi_layout->addWidget(h_lab,1,2,Qt::AlignLeft);
    roi_layout->addWidget(h_edit,1,3,Qt::AlignLeft);
    roi_layout->addWidget(cropX_lab,2,0,Qt::AlignLeft);
    roi_layout->addWidget(cropX_edit,2,1,Qt::AlignLeft);
    roi_layout->addWidget(cropY_lab,2,2,Qt::AlignLeft);
    roi_layout->addWidget(cropY_edit,2,3,Qt::AlignLeft);
    roi_layout->addWidget(cropW_lab,3,0,Qt::AlignLeft);
    roi_layout->addWidget(cropW_edit,3,1,Qt::AlignLeft);
    roi_layout->addWidget(cropH_lab,3,2,Qt::AlignLeft);
    roi_layout->addWidget(cropH_edit,3,3,Qt::AlignLeft);
    roi_layout->addWidget(maxframes_btn, 4, 0, Qt::AlignLeft);
    roi_layout->addWidget(maxframes_edit, 4, 1, Qt::AlignLeft);

    QHBoxLayout *delay_layout = new QHBoxLayout(this);
    QLabel *delay_lab = new QLabel("Time Delay(s):", this);
    delay_lab->setFixedWidth(120);
    time_edit = new QLineEdit("0", this);
    time_edit->setMaxLength(8);
    time_edit->setFixedWidth(80);
    connect(time_edit, &QLineEdit::editingFinished, [=]{
        int time = time_edit->text().toInt();
        time = qBound(0, time, 999999);
        time_edit->setText(QString::number(time));
    });
    delay_layout->addWidget(delay_lab,0,Qt::AlignLeft);
    delay_layout->addWidget(time_edit,0,Qt::AlignLeft);

    QHBoxLayout *capture_layout = new QHBoxLayout();
    capture_btn = new QPushButton("Capture");
    capture_btn->setFixedWidth(120);
    connect(capture_btn, &QPushButton::clicked, this, &ContinuousRawWindow::HandelCapture);
    capture_cancel_btn = new QPushButton("Cancel");
    capture_cancel_btn->setFixedWidth(120);
    capture_cancel_btn->setEnabled(false);
    connect(capture_cancel_btn, &QPushButton::clicked, this, &ContinuousRawWindow::CancelDump);
    capture_layout->addWidget(capture_btn,0,Qt::AlignLeft);
    capture_layout->addWidget(capture_cancel_btn,0,Qt::AlignLeft);

    save_group = new QGroupBox("Save");
    QVBoxLayout *save_layout = new QVBoxLayout;
    save_group->setLayout(save_layout);
    QHBoxLayout *location_layout = new QHBoxLayout;
    QLabel *save_lab = new QLabel("Save Location:");
    save_local = new QCheckBox("local");
    save_local->setChecked(true);
    connect(save_local,&QCheckBox::clicked,[=]{
            save_local->setChecked(true);
            save_sdcard->setChecked(false);
            saveLocal = true;
    });
    save_sdcard = new QCheckBox("sd card");
    save_sdcard->setChecked(false);
    connect(save_sdcard,&QCheckBox::clicked,[=]{
            save_local->setChecked(false);
            save_sdcard->setChecked(true);
            saveLocal = false;
    });
    location_layout->addWidget(save_lab);
    location_layout->addWidget(save_local);
    location_layout->addWidget(save_sdcard);

    shared_ptr<GlobalData> global_data = GlobalData::getInstance();
    QString save_dir = global_data->getSettings(SETTINGS_SECTION, KEY_CONTINUOUS_SAVE_PATH).toString();
    QString exe_dir = QCoreApplication::applicationDirPath();
    open_btn = new QPushButton(tr("Open Save Path"));
    file_path = new QLineEdit();
    file_path->setReadOnly(true);
    if (!save_dir.isEmpty()) {
        QDir dir(save_dir);
        if(dir.exists()) {
            file_path->setText(save_dir);
        } else {
            file_path->setText(exe_dir);
            global_data->setSettings(SETTINGS_SECTION, KEY_CONTINUOUS_SAVE_PATH, "");
            global_data->saveSettings();
            QMessageBox::warning(this, tr("Warning"), tr("Can't find \"" + save_dir.toUtf8() + "\" directory! Use default save path."), QMessageBox::Ok);
        }
    } else {
        file_path->setText(exe_dir);
    }
    QHBoxLayout *filepath_layout = new QHBoxLayout;
    filepath_layout->addWidget(open_btn);
    filepath_layout->addWidget(file_path);
    connect(open_btn, &QPushButton::clicked, this, &ContinuousRawWindow::OpenSaveFilePath);

    save_layout->addLayout(filepath_layout);
    save_layout->addLayout(location_layout);

    img_viewer = new MultiImageViewerWidget();
    img_viewer->setFilePathHidden();
    img_viewer->showRawStatisticBtn();
    img_viewer->showLevelMeterBtn();
    ImageLabel *image_label = new ImageLabel(this, ImageLabel::NORMAL);
    connect(image_label, &ImageLabel::sigPositionChange, img_viewer, &MultiImageViewerWidget::slotPositionChange);
    connect(image_label, &ImageLabel::sigUpdateSingleRect, img_viewer, &MultiImageViewerWidget::handleRectToCropInfo);

    img_viewer->setImageLabel(image_label);
    img_viewer->setMinimumHeight(600);

    left_layout->addWidget(preview_group);
    left_layout->addWidget(roi_group);
    left_layout->addLayout(delay_layout);
    left_layout->addLayout(capture_layout);
    left_layout->addWidget(save_group);
    main_layout->addWidget(widget, 1, Qt::AlignTop);
    main_layout->addWidget(img_viewer, 2);
    setCentralWidget(main_widget);
    centralWidget()->setLayout(main_layout);
}

void ContinuousRawWindow::GetRawDumpMaxFrames()
{
    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        return;
    }

    if ((cropW_edit->text().toInt() == 0) || (cropH_edit->text().toInt() == 0)) {
        QMessageBox::warning(this, tr("Warning"), tr("Please set CropW and CropH first!"), QMessageBox::Ok);
        return;
    }

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_CONTINUOUS_GET_MAX_FRAMES;
    int id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

    QJsonObject param_obj;
    param_obj["cropW"] = cropW_edit->text().toInt();
    param_obj["cropH"] = cropH_edit->text().toInt();
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_CONTINUOUS_MAX_FRAMES;
    send_obj[MSG_KEY_ID] = id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_PARAMS] = param_obj;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void ContinuousRawWindow::ReceiveRawDumpMaxFrames(int id)
{
    QJsonObject obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();

    if (result_obj.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), "Get max frames failed!", QMessageBox::Ok);
        return;
    }

    QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();
    int frames = params_obj["max frames"].toInt();
    qDebug("receive frames:%d\n", frames);

    frames = qMax(0, frames);
    maxframes_edit->setText(QString::number(frames));
}

void ContinuousRawWindow::GetSingleYuv()
{
    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        return;
    }

    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);
    disableComponents();
    preview_cancel_btn->setEnabled(true);

    img_yuv_data.clear();
    yuvHeader.clear();

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_CONTINUOUS_GET_SINGLE_YUV;
    int id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_SINGLE_YUV;
    send_obj[MSG_KEY_ID] = id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void ContinuousRawWindow::handleSingleImageStatus(int id)
{
    QJsonObject obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();

    if (result_obj.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), "Get Single Image failed!", QMessageBox::Ok);
        enableComponents();
        return;
    }
    QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();
    int size = params_obj["Data size"].toInt();
    NetworkClientHelper::getInstance()->setCallerInfo(size, CMD_CONTINUOUS_GET_SINGLE_YUV);
    qDebug("Set receive size:%d\n", size);

    int temp_id = GlobalData::getInstance()->getJsonrpcId();
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_IMAGE_DATA;
    send_obj[MSG_KEY_ID] = temp_id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void ContinuousRawWindow::ReceiveSingleYuv(QString cmd)
{
    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);
    QByteArray data = NetworkClientHelper::getInstance()->popCmdData(cmd);

    QByteArray header = data.left(sizeof(YUV_HEADER));
    YUV_HEADER * pstHeader = (YUV_HEADER *) header.data();
    data.remove(0, sizeof(YUV_HEADER));
    yuvHeader = header;
    img_yuv_data = data;

    qDebug("numframe:%d, curframe:%d, width:%d, height:%d, stride:%d,%d,%d, format::%d\n", pstHeader->numFrame, pstHeader->curFrame,
        pstHeader->width, pstHeader->height, pstHeader->stride[0], pstHeader->stride[1], pstHeader->stride[2], pstHeader->pixelFormat);
    qDebug("data.size:%d, pstHeader->size:%d\n", data.size(), pstHeader->size);

    if (data.size() != (int)(pstHeader->size)) {
        QMessageBox::warning(this, tr("Error"), tr("Continuous Raw, The received yuv data size is wrong!"), QMessageBox::Ok);
        enableComponents();
        return;
    }

    if (!startDump) {
        w_edit->setText(QString::number(pstHeader->width));
        h_edit->setText(QString::number(pstHeader->height));
        cropW_edit->setText(QString::number(pstHeader->width));
        cropH_edit->setText(QString::number(pstHeader->height));
    }

    img_viewer->setYUVImgData(data, pstHeader);
    enableComponents();
    if (startDump) {
        emit sigGetYuvEnd();
    }
}

void ContinuousRawWindow::CancelGetSingleYuv()
{
    time_edit->setText("0");
    enableComponents();
    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);
}

void ContinuousRawWindow::initEvent()
{
    shared_ptr<NetworkClientHelper> network_helper = NetworkClientHelper::getInstance();
    connect(network_helper.get(), &NetworkClientHelper::sigContinuousYuvStatus, this, &ContinuousRawWindow::handleSingleImageStatus);
    connect(network_helper.get(), &NetworkClientHelper::sigContinuousYuvData, this, &ContinuousRawWindow::ReceiveSingleYuv);
    connect(&rawDump, &RawDump::DumpStatus, this, &ContinuousRawWindow::DumpStatus);
    connect(this, &ContinuousRawWindow::sigGetYuvEnd, this, &ContinuousRawWindow::RawDump);
    connect(delay, &QTimer::timeout, this, &ContinuousRawWindow::StartDump);
    connect(network_helper.get(), &NetworkClientHelper::sigContinuousGetMaxFrames, this, &ContinuousRawWindow::ReceiveRawDumpMaxFrames);
}

void ContinuousRawWindow::closeEvent(QCloseEvent *event)
{
    rawDump.Disconnect();
    startDump = false;
    connected = false;
    dump_error = false;
    event->accept();
    QMainWindow::closeEvent(event);
}

void ContinuousRawWindow::showEvent(QShowEvent *event)
{
    QString ip = GlobalData::getInstance()->getSettings(DEVICE_SECTION, KEY_IP_ADDRESS).toString();
    rawDump.Connect(ip);
    startDump = false;
    connected = false;
    dump_error = false;
    event->accept();
    QMainWindow::showEvent(event);
}

void ContinuousRawWindow::OpenSaveFilePath()
{
    shared_ptr<GlobalData> global_data = GlobalData::getInstance();
    QString open_dir = global_data->getSettings(SETTINGS_SECTION, KEY_CONTINUOUS_SAVE_PATH).toString();
    QString dir_path;
    if (!open_dir.isEmpty()) {
        QDir dir(open_dir);
        if(dir.exists()) {
            dir_path = QFileDialog::getExistingDirectory(this, "Please Select Image Save Path", open_dir);
        } else {
            dir_path = QFileDialog::getExistingDirectory(this, "Please Select Image Save Path", "./");
        }
    } else {
        dir_path = QFileDialog::getExistingDirectory(this, "Please Select Image Save Path", "./");
    }

    qDebug("Opened Save Path: %s", qUtf8Printable(dir_path));
    if (!dir_path.isEmpty()) {
        file_path->setText(dir_path);
        global_data->setSettings(SETTINGS_SECTION, KEY_CONTINUOUS_SAVE_PATH, dir_path);
        global_data->saveSettings();
    }
}

void ContinuousRawWindow::disableComponents()
{
    img_viewer->setEnabled(false);
    roi_group->setEnabled(false);
    save_group->setEnabled(false);
    preview_btn->setEnabled(false);
    preview_cancel_btn->setEnabled(false);
    capture_btn->setEnabled(false);
    capture_cancel_btn->setEnabled(false);
    time_edit->setEnabled(false);
}

void ContinuousRawWindow::enableComponents()
{
    img_viewer->setEnabled(true);
    roi_group->setEnabled(true);
    save_group->setEnabled(true);
    preview_btn->setEnabled(true);
    preview_cancel_btn->setEnabled(false);
    capture_btn->setEnabled(true);
    capture_cancel_btn->setEnabled(false);
    time_edit->setEnabled(true);
}


void ContinuousRawWindow::getRawInfoFromFile(QString path, RAW_INFO *raw_info)
{
    path.remove(path.length() - 4, 4);
    path.append(".txt");
    QFile raw_info_file(path);

    if (raw_info_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream text_stream(&raw_info_file);
        while (!text_stream.atEnd()) {
            QString line = text_stream.readLine();
            QList<QString> raw_info_list = line.split('=');
            if (raw_info_list.size() == 2) {
                if (raw_info_list[0] == "LE crop ") {
                    QString leCrop = QString::fromStdString(raw_info_list[1].toStdString());
                    QList<QString> le_info_list = leCrop.split(',');
                    leCropX = le_info_list[0].toInt();
                    leCropY = le_info_list[1].toInt();
                    raw_info->width = le_info_list[2].toInt();
                    raw_info->height = le_info_list[3].toInt();
                } else if (raw_info_list[0] == "SE crop ") {
                    QString seCrop = QString::fromStdString(raw_info_list[1].toStdString());
                    QList<QString> se_info_list = seCrop.split(',');
                    seCropX = se_info_list[0].toInt();
                    seCropY = se_info_list[1].toInt();
                } else if (raw_info_list[0] == "roi ") {
                    QString roiStr = QString::fromStdString(raw_info_list[1].toStdString());
                    QList<QString> roi_info_list = roiStr.split(',');
                    raw_info->roiFrameNum = roi_info_list[0].toInt();
                    raw_info->roiRect.x = roi_info_list[1].toInt();
                    raw_info->roiRect.y = roi_info_list[2].toInt();
                    raw_info->roiRect.w = roi_info_list[3].toInt();
                    raw_info->roiRect.h = roi_info_list[4].toInt();
                }
            }
        }
        raw_info_file.close();
    }
}

QByteArray ContinuousRawWindow::unpackRawDataByFrame(QByteArray dataSrc, RAW_INFO rawInfo, RAW_TYPE rawType)
{
    QByteArray data;
    RAW_HEADER stHeader;

    int tile_idx = 0xfffffff;
    bool tileMode = utils::isTileMode(rawInfo, rawType, &tile_idx);

    stHeader.compress = 1;
    stHeader.cropX = 0;
    stHeader.cropY = 0;
    if (rawType == 0) {
        stHeader.width = rawInfo.width;
        stHeader.height = rawInfo.height;
        stHeader.cropWidth = w_edit->text().toInt();
        stHeader.cropHeight = h_edit->text().toInt();
    } else {
        stHeader.width = rawInfo.roiRect.w;
        stHeader.height = rawInfo.roiRect.h;
        stHeader.cropX = 0;
        stHeader.cropY = 0;
        stHeader.cropWidth = rawInfo.roiRect.w;
        stHeader.cropHeight = rawInfo.roiRect.h;
    }

    int frameSize = dataSrc.size();
    if (rawInfo.enWDR) {
        QByteArray d, d1, d2;
        frameSize /= 2;
        d = dataSrc.mid(0, frameSize);
        if (rawType == 0) {
            stHeader.cropX = leCropX;
            stHeader.cropY = leCropY;
        }
        d1 = utils::cropUnpackRaw(d, stHeader, tileMode, tile_idx);
        d2.append(d1);
        d = dataSrc.mid(frameSize, frameSize);
        if (rawType == 0) {
            stHeader.cropX = seCropX;
            stHeader.cropY = seCropY;
        }
        d1 = utils::cropUnpackRaw(d, stHeader, tileMode, tile_idx);
        d2.append(d1);

        QByteArray le = d2.mid(0, d2.size()/2);
        QByteArray se = d2.mid(d2.size()/2, d2.size()/2);
        int lineSize = le.size() / stHeader.cropHeight;
        for (int i = 0; i < stHeader.cropHeight; i++){
            data.append(le.mid(i * lineSize, lineSize));
            data.append(se.mid(i * lineSize, lineSize));
        }
    } else {
        if (rawType == 0) {
            stHeader.cropX = leCropX;
            stHeader.cropY = leCropY;
        }
        data = utils::cropUnpackRaw(dataSrc, stHeader, tileMode, tile_idx);
    }

    return data;
}

QByteArray ContinuousRawWindow::fillRoiRaw(QByteArray fullData, QByteArray roiData, RAW_INFO rawInfo)
{
    QByteArray dstData = fullData;
    QByteArray srcData = roiData;

    int fullW = w_edit->text().toInt();
    int fullH = h_edit->text().toInt();
    int roiW = rawInfo.roiRect.w;
    int roiH = rawInfo.roiRect.h;
    int roiX = rawInfo.roiRect.x;
    int roiY = rawInfo.roiRect.y;

    if ((roiW == fullW) && (roiH == fullH)) {
        return srcData;
    }

    int roiLineSize = srcData.size() / roiH;
    int fullLineSize = dstData.size() / fullH;
    if (rawInfo.enWDR) {
        for (int h = 0, i = roiY; h < roiH; h++, i++) {
            int idx = i * fullLineSize + roiX * 2;
            QByteArray roiLineData = srcData.mid(h * roiLineSize, roiLineSize);
            QByteArray roiLineDataLe = roiLineData.mid(0, roiLineSize/2);
            QByteArray roiLineDataSe = roiLineData.mid(roiLineSize/2, roiLineSize/2);
            dstData.replace(idx, roiLineSize/2, roiLineDataLe);
            idx += fullLineSize / 2;
            dstData.replace(idx, roiLineSize/2, roiLineDataSe);
        }
    } else {
        for (int h = 0, i = roiY; h < roiH; h++, i++) {
            int idx = i * fullLineSize + roiX * 2;
            QByteArray roiLineData = srcData.mid(h * roiLineSize, roiLineSize);
            dstData.replace(idx, roiLineSize, roiLineData);
        }
    }

    return dstData;
}

void ContinuousRawWindow::UnpackRaw()
{
    if (savePath.isEmpty()) {
        return;
    }

    QDir dir(savePath);
    if (!dir.exists()) {
        return;
    }

    QStringList filters;
    filters<<QString("*.raw");
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    dir.setNameFilters(filters);
    QFileInfoList list = dir.entryInfoList();
    if (list.size() != 2) {
        return;
    }

    QString filename_roi, filename_full;
    for (int i=0; i<list.size(); i++) {
        QFileInfo fileInfo = list.at(i);
        QString name = fileInfo.fileName();
        if (name.contains("roi")) {
            filename_roi = name;
        } else {
            filename_full = name;
        }
    }

    RAW_INFO raw_info = {};
    getRawInfoFromFile(savePath + "/" + filename_full, &raw_info);
    if (filename_full.contains("_WDR_")) {
        raw_info.enWDR = 1;
    } else {
        raw_info.enWDR = 0;
    }

    int fusion_Frame = GlobalData::getInstance()->GetWDRFusionFrame();
    QString hdr_str_new = QString("-hdr=%1_").arg(fusion_Frame);
    QString hdr_str_old = QString("-hdr=%1_").arg(raw_info.enWDR);

    //copy files to roi_raw dir
    QString dir_roi = savePath + "/roi_raw";
    QDir().mkdir(dir_roi);
    QDir dir_src(savePath);
    dir_src.setFilter(QDir::Files | QDir::NoSymLinks);
    QFileInfoList fileList = dir_src.entryInfoList();
    for (int i=0; i<fileList.size(); i++) {
        QFileInfo fileInfo = fileList.at(i);
        QString srcFilePath = fileInfo.absoluteFilePath();
        QString newFileName = fileInfo.fileName().replace(hdr_str_old, hdr_str_new);
        QString dstFilePath = dir_roi + "/" + newFileName;
        QFile::copy(srcFilePath, dstFilePath);
    }

    //umpack and save raw
    QByteArray data, roiData;
    QFile fullfile(savePath + "/" + filename_full);
    fullfile.open(QIODevice::ReadOnly);
    QByteArray fullPackData = fullfile.readAll();
    fullfile.close();

    QByteArray fullFrameData = unpackRawDataByFrame(fullPackData, raw_info, RAW_NORMAL);
    QString frames_str = QString("-frame=%1_").arg(raw_info.roiFrameNum);
    QString rawFilePath = savePath + "/" + filename_full;
    rawFilePath.replace("-frame=1_", frames_str);
    QFile rawFile(rawFilePath);
    rawFile.open(QIODevice::WriteOnly);

    QFile roiFile(savePath + "/" + filename_roi);
    roiFile.open(QIODevice::ReadOnly);
    int roifileSize = roiFile.size();
    int roiframeSize = roifileSize / raw_info.roiFrameNum;

    for (int i = 0; i < raw_info.roiFrameNum; i++) {
        QByteArray roiPackData = roiFile.read(roiframeSize);
        QByteArray roiUnpackData = unpackRawDataByFrame(roiPackData, raw_info, RAW_ROI);
        QByteArray roiFrameData = fillRoiRaw(fullFrameData, roiUnpackData, raw_info);
        rawFile.write(roiFrameData);
    }
    rawFile.close();
    roiFile.close();
    QFile::remove(savePath + "/" + filename_roi);
    QFile::remove(savePath + "/" + filename_full);

    //delete some info of txt file
    QString textFilePath = savePath + "/" + filename_full;
    textFilePath.replace(".raw", ".txt");
    QFile raw_info_file(textFilePath);
    raw_info_file.open(QIODevice::ReadWrite | QIODevice::Text);
    QTextStream text_stream(&raw_info_file);
    QFile temp(savePath + "/temp.txt");
    temp.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    QTextStream temp_stream(&temp);
    while (!text_stream.atEnd()) {
        QString line = text_stream.readLine();
        if (!line.contains("LE crop =") && !line.contains("SE crop =") && !line.contains("roi =")) {
           temp_stream<<line;
           temp_stream<<"\n";
        }
    }
    raw_info_file.close();
    temp.close();
    QFile::remove(textFilePath);
    QFile::rename(savePath + "/temp.txt", textFilePath);

    //rename files
    QDir dir_dst(savePath);
    dir_dst.setFilter(QDir::Files | QDir::NoSymLinks);
    QFileInfoList fileDst = dir_dst.entryInfoList();
    for (int i=0; i<fileDst.size(); i++) {
        QFileInfo fileInfo = fileDst.at(i);
        QString srcFilePath = fileInfo.absoluteFilePath();
        QString dstFilePath = srcFilePath;
        dstFilePath.replace("-frame=1_", frames_str);
        dstFilePath.replace(hdr_str_old, hdr_str_new);
        QFile::rename(srcFilePath, dstFilePath);
    }
}

void ContinuousRawWindow::DumpStatus(int status)
{
    switch (status) {
    case RAW_DUMP_STATUS_E::RAW_DUMP_CONNECT_SUCCEED: {
            connected = true;
            dump_error = false;
            break;
    }
    case RAW_DUMP_STATUS_E::RAW_DUMP_CONNECT_FAIL: {
            connected = false;
            dump_error = false;
            break;
    }
    case RAW_DUMP_STATUS_E::RAW_DUMP_ERROR: {
            dump_error = true;
            break;
    }
    case RAW_DUMP_STATUS_E::RAW_DUMP_FINISH: {
            if (dump_error) {
                QMessageBox::warning(this, tr("Error"), tr("Capture failed!"), QMessageBox::Ok);
            } else {
                UnpackRaw();
                QMessageBox::information(this, tr("Info"), tr("Capture success!"), QMessageBox::Ok);
            }
            dump_error = false;
            startDump = false;
            enableComponents();
            time_edit->setText("0");
            break;
    }
    default:
        break;
    }
}

int ContinuousRawWindow::CheckRoiInfo()
{
    int frames = frames_edit->text().toInt();
    int width = w_edit->text().toInt();
    int height = h_edit->text().toInt();
    int cropY = cropY_edit->text().toInt();
    int cropW = cropW_edit->text().toInt();
    int cropH = cropH_edit->text().toInt();

    if (width < IMAGE_MIN_WIDTH || height < IMAGE_MIN_HEIGHT) {
        return -1;
    }

    frames = qMax(2, frames);
    cropY = qBound(0, cropY, height-IMAGE_MIN_HEIGHT);
    cropW = qBound(IMAGE_MIN_WIDTH, cropW, width);
    cropH = qBound(IMAGE_MIN_HEIGHT, cropH, height-cropY);
    frames_edit->setText(QString::number(frames));
    cropX_edit->setText(QString::number(0));
    cropY_edit->setText(QString::number(cropY));
    cropW_edit->setText(QString::number(cropW));
    cropH_edit->setText(QString::number(cropH));

    return 0;
}

void ContinuousRawWindow::HandelCapture()
{
    if ((!NetworkClientHelper::getInstance()->isNormalStatus()) || (!connected)) {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        return;
    }

    disableComponents();
    capture_cancel_btn->setEnabled(true);

    int time = time_edit->text().toUInt();
    if (time > 0) {
        delay->start(time * 1000);
    } else {
        StartDump();
    }
}

void ContinuousRawWindow::StartDump()
{
    startDump = true;
    GetSingleYuv();
}

void ContinuousRawWindow::RawDump()
{
    if (!connected) {
        startDump = false;
        QString ip = GlobalData::getInstance()->getSettings(DEVICE_SECTION, KEY_IP_ADDRESS).toString();
        rawDump.Connect(ip);
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected, reconnecting...\nYou can try capture again!"), QMessageBox::Ok);
        return;
    }

    disableComponents();

    int ret = CheckRoiInfo();
    if (ret < 0) {
        startDump = false;
        QMessageBox::warning(this, tr("Warning"), tr("Roi info is wrong!"), QMessageBox::Ok);
        return;
    }

    struct RawDumpInfo info;
    info.totalFrame = frames_edit->text().toInt();
    info.roiRectX = cropX_edit->text().toInt();
    info.roiRectY = cropY_edit->text().toInt();
    info.roiRectW = cropW_edit->text().toInt();
    info.roiRectH = cropH_edit->text().toInt();
    if (saveLocal) {
        info.bDump2Remote = true;
        savePath = file_path->text() + "/continuous_" + QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
        QDir().mkdir(savePath);
        info.pathPrefix = savePath.toStdString();
        saveJpg();
    } else {
        info.bDump2Remote = false;
        info.pathPrefix = "/mnt/sd";
    }

    rawDump.StartRawDump(info);
    capture_cancel_btn->setEnabled(true);
}

void ContinuousRawWindow::saveJpg()
{
    if (yuvHeader.isEmpty() || img_yuv_data.isEmpty()) {
        return;
    }

    if (savePath.isEmpty()) {
        return;
    }

    YUV_HEADER *header = (YUV_HEADER *) yuvHeader.data();
    QString jpg_path = savePath + "/" + QString("%1X%2_").arg(header->width).arg(header->height);
    if (header->pixelFormat == PIXEL_FORMAT_NV21) {
        jpg_path = jpg_path + "NV21_";
    } else  if (header->pixelFormat == PIXEL_FORMAT_NV12) {
        jpg_path = jpg_path + "YUV12_";
    } else  if (header->pixelFormat == PIXEL_FORMAT_YUV_PLANAR_420) {
        jpg_path = jpg_path + "YUV420_";
    } else {
        jpg_path = jpg_path + "NULL_";
    }
    jpg_path = jpg_path + "-bits=8_" + ".jpg";

    QImage image = utils::convertYUV2RGBImage(img_yuv_data, header);
    image.save(jpg_path, "JPG");
    yuvHeader.clear();
    img_yuv_data.clear();
}

void ContinuousRawWindow::CancelDump()
{
    time_edit->setText("0");
    dump_error = false;
    startDump = false;
    enableComponents();
}

