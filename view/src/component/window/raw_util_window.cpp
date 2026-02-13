#include "raw_util_window.hpp"
#include "main_window.hpp"

#include "utils.hpp"
#include "raw_image_option_dialog.hpp"
#include "yuv_image_option_dialog.hpp"
#include "capture_window.hpp"

#include <QLabel>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QAction>

#include "windows.h"


const int MIN_WINDOW_WIDTH = 1300;
const int MIN_WINDOW_HEIGHT = 768;

#define RAW_MODE_LINEAR (0)
#define RAW_MODE_WDR (1)

#define CV183X_LINE_BUFFER_WIDTH 2688
#define CV183X_TILE_START_INDEX 1920
#define CV182X_LINE_BUFFER_WIDTH 2304
#define CV182X_TILE_START_INDEX 1536


RawUtilWindow::RawUtilWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle(tr("Raw Utility"));
    this->setWindowFlags(Qt::WindowCloseButtonHint|Qt::MSWindowsFixedSizeDialogHint);
    initMenu();
    initUI();
    retranslateUi();
    initEvent();
}

void RawUtilWindow::initMenu()
{
}

void RawUtilWindow::initUI()
{
    main_widget = new QWidget();
    main_widget->setFixedSize(QSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT));
    main_layout = new QGridLayout();
    raw_list_view = new RawImageListWidget;

    add_img_btn = new QPushButton();
    dis_select_btn = new QPushButton;
    move_up_btn = new QPushButton;
    move_down_btn = new QPushButton;
    del_btn = new QPushButton;
    del_all_btn = new QPushButton;
    stitch_flag = new QCheckBox;
    isp_info = new QCheckBox;
    snap_mode = new QCheckBox;
    QHBoxLayout *h = new QHBoxLayout;
    QLabel *label = new QLabel(tr("Flag:"));
    flag_box = new QComboBox;
    flag_box->addItems({tr("FE"), tr("BE")});
    h->addWidget(label);
    h->addWidget(flag_box);
    h->setAlignment(Qt::AlignLeft);
    loop = new QCheckBox;
    send_btn = new QPushButton;
    flush_btn = new QPushButton;
    cancel_btn = new QPushButton;
    batch_btn = new QPushButton;
    yuv_btn = new QPushButton;
    yuv_btn->setEnabled(false);
    ae_sim = new QCheckBox("AE_sim");
    awb_sim = new QCheckBox("AWB_sim");
    save_btn = new QCheckBox("save unpacked raw");
    QWidget *widget = new QWidget;
    QVBoxLayout *v_layout = new QVBoxLayout(widget);
    batch_timer = new QTimer();


    v_layout->addWidget(add_img_btn);
    // v_layout->addWidget(dis_select_btn);
    // v_layout->addWidget(move_up_btn);
    // v_layout->addWidget(move_down_btn);
    v_layout->addWidget(del_btn);
    v_layout->addWidget(del_all_btn);
    // v_layout->addWidget(stitch_flag);
    // v_layout->addWidget(isp_info);
    // v_layout->addWidget(snap_mode);
    // v_layout->addLayout(h);
    // v_layout->addWidget(loop);
    v_layout->addWidget(send_btn);
    // v_layout->addWidget(flush_btn);
    v_layout->addWidget(cancel_btn);
    v_layout->addWidget(batch_btn);
    v_layout->addWidget(yuv_btn);
    v_layout->addWidget(ae_sim);
    v_layout->addWidget(awb_sim);
    v_layout->addWidget(save_btn);

    widget->setMaximumWidth(300);
    main_layout->addWidget(widget, 0, 0);
    main_layout->setColumnMinimumWidth(0, 300);
    main_layout->addWidget(raw_list_view, 0, 1);
    main_layout->setColumnMinimumWidth(1, 1000);
    setCentralWidget(main_widget);
    centralWidget()->setLayout(main_layout);

    connect(add_img_btn, &QPushButton::clicked, this, &RawUtilWindow::handleOpenImgFile);
    connect(dis_select_btn, &QPushButton::clicked, [=]{
        disel_toggle = disel_toggle ? false : true;
        if (disel_toggle) {
            raw_list_view->setCheckState(Qt::CheckState::Unchecked);
            dis_select_btn->setText("Select All");
        } else {
            raw_list_view->setCheckState(Qt::CheckState::Checked);
            dis_select_btn->setText("Deselect All");
        }
    });

    connect(move_up_btn, &QPushButton::clicked, [=]{
        raw_list_view->moveup_sel();
    });

    connect(move_down_btn, &QPushButton::clicked, [=]{
        raw_list_view->movedown_sel();
    });

    connect(del_btn, &QPushButton::clicked, [=]{
        delItemSelected();
    });

    connect(del_all_btn, &QPushButton::clicked, [=]{
        delItemAll();
    });

    connect(send_btn, &QPushButton::clicked, this, &RawUtilWindow::slotSend);
    connect(flush_btn, &QPushButton::clicked, this, &RawUtilWindow::slotFlush);
    connect(cancel_btn, &QPushButton::clicked, this, &RawUtilWindow::slotCancel);
    connect(batch_btn, &QPushButton::clicked, this, &RawUtilWindow::slotBatch);
    connect(batch_timer,&QTimer::timeout,this,&RawUtilWindow::batch_updateProgress);
    connect(yuv_btn, &QPushButton::clicked, this, &RawUtilWindow::slotGetYuv);
}

void RawUtilWindow::initEvent()
{
    shared_ptr<NetworkClientHelper> network_helper = NetworkClientHelper::getInstance();
    connect(network_helper.get(), &NetworkClientHelper::sigRawUtilityMessage, this, &RawUtilWindow::handleReceiveMessage);
    connect(network_helper.get(), &NetworkClientHelper::sigRawUtilYuvStatus, this, &RawUtilWindow::handleMultiYuvStatus);
    connect(network_helper.get(), &NetworkClientHelper::sigRawUtilYuvData, this, &RawUtilWindow::slotGotYuvImg);
}

void RawUtilWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void RawUtilWindow::retranslateUi()
{
    add_img_btn->setText(tr("Add Image"));
    dis_select_btn->setText(tr("UnSelect All"));
    move_up_btn->setText(tr("Move Up"));
    move_down_btn->setText(tr("Move Down"));
    del_btn->setText(tr("Delete"));
    del_all_btn->setText(tr("Delete All"));
    stitch_flag->setText(tr("StitchFlag"));
    isp_info->setText(tr("ISP Info"));
    snap_mode->setText(tr("Snap Mode"));
    loop->setText(tr("Loop"));
    send_btn->setText(tr("Send"));
    flush_btn->setText(tr("Flush"));
    cancel_btn->setText(tr("Cancel"));
    batch_btn->setText(tr("Batch"));
    yuv_btn->setText(tr("Get yuv"));
}

void RawUtilWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        this->close();
    }
}

void RawUtilWindow::delItemSelected()
{
    if (dataFormat) {
        yuv_list_view->del_selected();
    } else {
        raw_list_view->del_selected();
    }
}

void RawUtilWindow::delItemAll()
{
    if (dataFormat) {
        yuv_list_view->del_all();
    } else {
        raw_list_view->del_all();
    }
}

void RawUtilWindow::slotGetYuv()
{
    qDebug("RawUtilWindow::slotGetYuv\n");

    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        return;
    }

    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);
    yuv_btn->setEnabled(false);
    startCapture = true;

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_RAWREPLAY_GET_MULTI_YUV;
    int id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

    QJsonObject param_obj;
    param_obj["from"] = 0;
    param_obj["frames"] = 1;
    param_obj["tightly"] = false;
    param_obj["getRawReplayYuv"] = true;
    param_obj["getRawReplayYuvId"] = yuvIndex;
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_MULTI_YUV;
    send_obj[MSG_KEY_ID] = id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_PARAMS] = param_obj;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void RawUtilWindow::handleMultiYuvStatus(int id)
{
    qDebug("RawUtilWindow::handleMultiYuvStatus\n");

    QJsonObject obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);

    if(!startCapture) {
        return;
    }

    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();
    if (result_obj.isEmpty()) {
        yuv_btn->setEnabled(true);
        QMessageBox::warning(this, tr("Error"), tr("Capture yuv failed!"), QMessageBox::Ok);
        return;
    }

    QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();
    int recvSize = params_obj["Data size"].toInt();
    NetworkClientHelper::getInstance()->setCallerInfo(recvSize, CMD_RAWREPLAY_GET_MULTI_YUV);
    qDebug("Set receive size:%d\n", recvSize);

    int temp_id = GlobalData::getInstance()->getJsonrpcId();
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_IMAGE_DATA;
    send_obj[MSG_KEY_ID] = temp_id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void RawUtilWindow::slotGotYuvImg(QString cmd)
{
    qDebug("RawUtilWindow::handleMultiYuvStatus\n");

    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);
    QByteArray data = NetworkClientHelper::getInstance()->popCmdData(cmd);

    if(!startCapture) {
        return;
    }

    QByteArray header = data.left(sizeof(YUV_HEADER));
    YUV_HEADER * pstHeader = (YUV_HEADER *) header.data();
    data.remove(0, sizeof(YUV_HEADER));

    qDebug("numframe:%d, curframe:%d, width:%d, height:%d, stride:%d,%d,%d, format::%d\n", pstHeader->numFrame, pstHeader->curFrame,
        pstHeader->width, pstHeader->height, pstHeader->stride[0], pstHeader->stride[1], pstHeader->stride[2], pstHeader->pixelFormat);
    qDebug("data.size:%d, pstHeader->size:%d\n", data.size(), pstHeader->size);

    if (data.size() != (int)pstHeader->size) {
        QMessageBox::warning(this, tr("Error"), tr("Capture Tool, The received yuv data size is wrong!"), QMessageBox::Ok);
        return;
    }

    yuv_btn->setEnabled(true);
    saveYuvFile(data, *pstHeader, 1);
    QMessageBox::information(this, tr("Info"), tr("Get yuv successfully!"), QMessageBox::Ok);
}

void RawUtilWindow::saveYuvFile(QByteArray &data, YUV_HEADER &header, int frame)
{
    int n = 0, i = 0, idx = 0;
    unsigned int frame_start = 0, u_start = 0, v_start = 0, uv_start = 0;
    const char *pData = data.data();

    if (!dirPath.isEmpty()) {
        QString yuv_path, jpg_path;

        QString path = dirPath + "/" + QString("%1X%2_").arg(header.width).arg(header.height);
        if (header.pixelFormat == PIXEL_FORMAT_NV21) {
            path = path + "NV21_";
        } else if (header.pixelFormat == PIXEL_FORMAT_NV12) {
            path = path + "NV12_";
        } else  if (header.pixelFormat == PIXEL_FORMAT_YUV_PLANAR_420) {
            path = path + "YUV420_";
        } else {
            path = path + "NULL_";
        }
        QString dateTime_str = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
        yuv_path = path + QString("-bits=8_-index=%1_").arg(yuvIndex) + dateTime_str + ".yuv";
        jpg_path = path + QString("-bits=8_-index=%1_").arg(yuvIndex) + dateTime_str + ".jpg";

        QFile fileMultiYuv(yuv_path);
        fileMultiYuv.open(QIODevice::WriteOnly);
        QImage image = utils::convertYUV2RGBImage(data, &header);
        image.save(jpg_path, "JPG");

        if (header.pixelFormat == PIXEL_FORMAT_NV21 || header.pixelFormat == PIXEL_FORMAT_NV12) {
            for(n = 0; n < frame; n++) {
                uv_start = header.stride[0] * header.height;
                for(i = 0, idx = frame_start; i < header.height; i++) {
                    fileMultiYuv.write(pData + idx, header.width);
                    idx += header.stride[0];
                }

                for(i = 0, idx = frame_start + uv_start; i < header.height / 2; i++) {
                    fileMultiYuv.write(pData + idx, header.width);
                    idx += header.stride[1];
                }

                frame_start += header.size;
            }
        } else if (header.pixelFormat == PIXEL_FORMAT_YUV_PLANAR_420) {
            for(n = 0; n < frame; n++) {
                u_start = header.stride[0] * header.height;
                v_start = u_start + header.stride[1] * header.height / 2;
                for(i = 0, idx = frame_start; i < header.height; i++) {
                    fileMultiYuv.write(pData + idx, header.width);
                    idx += header.stride[0];
                }

                for(i = 0, idx = frame_start + u_start; i < header.height / 2; i++) {
                    fileMultiYuv.write(pData + idx, header.width / 2);
                    idx += header.stride[1];
                }

                for(i = 0, idx = frame_start + v_start; i < header.height / 2; i++) {
                    fileMultiYuv.write(pData + idx, header.width / 2);
                    idx += header.stride[2];
                }

                frame_start += header.size;
            }
        }  else {
            QMessageBox::warning(this, tr("Error"), tr("YUV pixel format is not supported!"), QMessageBox::Ok);
        }

        fileMultiYuv.close();
    } else {
        QMessageBox::warning(this, tr("Error"), tr("Save directory path is empty!"), QMessageBox::Ok);
    }
}

void RawUtilWindow::saveUnpackedRaw(QString path, QByteArray full, QByteArray roi, RAW_INFO raw_info, RoiInfo roiInfo)
{
    if (path.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("The path is wrong!"), QMessageBox::Ok);
        return;
    }

    QByteArray dstData, fullData;
    QByteArray roiData = roi;
    QString soc = GlobalData::getInstance()->getCvipqToolSoc();

    int roiWidth = roiInfo.width;
    if (raw_info.enWDR) {
        roiWidth *= 2;
    }

    if ((soc == CV186X_SOC || soc == CV184X_SOC || soc == CV181X_SOC || soc == CV180X_SOC) && raw_info.enWDR) {
        QByteArray le = full.mid(0, full.size() / 2);
        QByteArray se = full.mid(full.size() / 2, full.size() / 2);
        fullData = RawLinearToWdr(le, se, raw_info.height);
    } else {
        fullData = full;
    }

    int roi_frame_size = roiWidth * roiInfo.height * 2;
    int roi_buffer_size = roi_frame_size * roiInfo.frames;
    unsigned char *unpacked_buffer = (unsigned char *) malloc(roi_buffer_size);
    for (int i = 0, idx_unpacked = 0, idx_packed = 0; i < roiInfo.frames; i++) {
        utils::raw_packed2unpacked((unsigned char *)(roiData.data() + idx_packed), (unsigned short *)(unpacked_buffer + idx_unpacked), roiWidth, roiInfo.height);
        idx_unpacked += roi_frame_size;
        idx_packed += 1.5 * roiWidth * roiInfo.height;
    }
    QByteArray roi_data_unpacked = QByteArray::fromRawData((const char *)unpacked_buffer, roi_buffer_size);
    for (int n = 0; n < roiInfo.frames; n++) {
        QByteArray roiTempData = roi_data_unpacked.mid(n * roi_frame_size, roi_frame_size);
        if (raw_info.enWDR) {
            QByteArray roiWdr;
            if (soc == CV186X_SOC || soc == CV184X_SOC || soc == CV181X_SOC || soc == CV180X_SOC) {
                QByteArray roiLe = roiTempData.mid(0, roi_frame_size / 2);
                QByteArray roiSe = roiTempData.mid(roi_frame_size / 2, roi_frame_size / 2);
                roiWdr = RawLinearToWdr(roiLe, roiSe, roiInfo.height);
            } else {
                roiWdr = roiTempData;
            }
            for (int h = raw_info.roiRect.y, i = 0; h < raw_info.roiRect.y + raw_info.roiRect.h; h++, i++) {
               int idx = h * raw_info.width * 2 + raw_info.roiRect.x * 2;
               QByteArray roi_lineData = roiWdr.mid(i * roiWidth * 2, roiWidth * 2);
               QByteArray roi_lineData_le = roi_lineData.mid(0, roiInfo.width * 2);
               QByteArray roi_lineData_se = roi_lineData.mid(roiInfo.width * 2, roiInfo.width * 2);
               fullData.replace(idx, roiInfo.width * 2, roi_lineData_le);
               idx += raw_info.width;
               fullData.replace(idx, roiInfo.width * 2, roi_lineData_se);
            }
        } else {
            for (int h = raw_info.roiRect.y, i = 0; h < raw_info.roiRect.y + raw_info.roiRect.h; h++, i++) {
               int idx = h * raw_info.width * 2 + raw_info.roiRect.x * 2;
               QByteArray roi_lineData = roiTempData.mid(i * roiWidth * 2, roiWidth * 2);
               fullData.replace(idx, roiWidth * 2, roi_lineData);
            }
        }
        dstData.append(fullData);
    }

    QFile raw_file(path);
    raw_file.open(QIODevice::WriteOnly|QIODevice::Truncate);
    raw_file.write(dstData);
    raw_file.close();
    free(unpacked_buffer);
}

QByteArray RawUtilWindow::handleImportRoiRawData(QString path, RAW_INFO *raw_info, RoiInfo roi_info)
{
    QByteArray data, readData, tempData;
    QByteArray unpackData;
    RAW_HEADER pstHeader;

    QFile file(path);
    file.open(QIODevice::ReadOnly);
    readData = file.readAll();
    file.close();

    RAW_INFO tmp;
    tmp.width = raw_info->enWDR ? raw_info->width / 2 : raw_info->width;
    tmp.roiRect.w = roi_info.width;
    int tileIdx = 0;
    bool tileMode = utils::isTileMode(tmp, RAW_ROI, &tileIdx);
    QString soc = GlobalData::getInstance()->getCvipqToolSoc();

    if ( raw_info->enWDR == 1) {
        raw_info->roiFrameNum *= 2;
    }
    int unpackSize = raw_info->roiRect.w * 1.5 * raw_info->roiRect.h * raw_info->roiFrameNum;

    pstHeader.compress = (unpackSize == readData.size()) ? 0 : 1;
    pstHeader.width = roi_info.width;
    pstHeader.height = roi_info.height;
    pstHeader.cropX = roi_info.cropX;
    pstHeader.cropY = roi_info.cropY;
    pstHeader.cropWidth = roi_info.cropWidth;
    pstHeader.cropHeight = roi_info.cropHeight;
    raw_info->roiRect.x += roi_info.cropX;
    raw_info->roiRect.y += roi_info.cropY;
    raw_info->roiRect.w = roi_info.cropWidth;
    raw_info->roiRect.h = roi_info.cropHeight;
    qDebug("roi, enWDR:%d, compress:%d, unpackSize:%d, readSize:%d\n",raw_info->enWDR, pstHeader.compress, unpackSize, readData.size());
    qDebug("roi, roiRect.x:%d, roiRect.y:%d, roiRect.w:%d, roiRect.h:%d\n", raw_info->roiRect.x, raw_info->roiRect.y, raw_info->roiRect.w, raw_info->roiRect.h);

    int size = readData.size() / raw_info->roiFrameNum;
    for (int i = 0, p = 0; i < raw_info->roiFrameNum; i++, p += size) {
        QByteArray d = readData.mid(p, size);
        QByteArray d1 = utils::cropUnpackRaw(d, pstHeader, tileMode, tileIdx);;
        unpackData.append(d1);
    }

    if (soc != CV186X_SOC && soc != CV181X_SOC && soc != CV180X_SOC && raw_info->enWDR) {
        int frmSize = unpackData.size() / raw_info->roiFrameNum;
        for (int i = 0; i < raw_info->roiFrameNum; i+=2) {
            QByteArray le = unpackData.mid(i * frmSize, frmSize);
            QByteArray se = unpackData.mid((i + 1) * frmSize, frmSize);
            QByteArray wdr = RawLinearToWdr(le, se, roi_info.height);
            tempData.append(wdr);
        }
    } else {
        tempData.append(unpackData);
    }

    int width = pstHeader.cropWidth;
    raw_info->roiFrameSize = 1.5 * pstHeader.cropWidth * pstHeader.cropHeight;
    if ( raw_info->enWDR == 1) {
        raw_info->roiFrameNum /= 2;
        raw_info->roiFrameSize *= 2;
        width *= 2;
    }
    qDebug("roi, roiFrameNum:%d, roiFrameSize:%d\n", raw_info->roiFrameNum, raw_info->roiFrameSize);

    int buffer_size = raw_info->roiFrameSize * raw_info->roiFrameNum;
    unsigned char *packed_buffer = (unsigned char *) malloc(buffer_size);
    for (int i = 0, idx_unpacked = 0, idx_packed = 0; i < raw_info->roiFrameNum; i++) {
        utils::raw_unpacked2packed((unsigned short *)(tempData.data() + idx_unpacked), packed_buffer + idx_packed, width, pstHeader.cropHeight);
        idx_unpacked += (2 * width * pstHeader.cropHeight);
        idx_packed += raw_info->roiFrameSize;
    }
    data = QByteArray::fromRawData((const char *)packed_buffer, buffer_size);

    return data;
}

QString RawUtilWindow::findFileTimeStamp(QString &file_name)
{
    int idx = file_name.indexOf("ISO");
    idx = file_name.indexOf("_", idx);

    return file_name.mid(idx + 1, 14);
}

void RawUtilWindow::setFileNameFrameNum(QString &file_name, int frame_num)
{
    int frame_pos = 0;
    int start_idx = file_name.indexOf("frame");
    int end_idx = file_name.indexOf("_", start_idx);

    frame_pos = start_idx + 6;
    file_name.replace(frame_pos, end_idx - frame_pos, QString::number(frame_num));
}

QByteArray RawUtilWindow::RawLinearToWdr(QByteArray srcLe, QByteArray srcSe, int height)
{
    QByteArray dst;

    if (srcLe.isEmpty() || srcSe.isEmpty() || srcLe.size() != srcSe.size() || height <= 0) {
        qDebug("RawUtilWindow::RawLinearToWdr, input error!");
        return dst;
    }

    int lineSize = srcLe.size() / height;

    for (int i = 0; i < height; i++) {
        dst.append(srcLe.mid(i * lineSize, lineSize));
        dst.append(srcSe.mid(i * lineSize, lineSize));
    }

    return dst;
}

QByteArray RawUtilWindow::RawWdrToLInear(QByteArray src, int height)
{
    QByteArray dst, le, se;

    if (src.isEmpty() || height <= 0) {
        qDebug("RawUtilWindow::RawWdrToLInear, input error!");
        return dst;
    }

    int lineSize = src.size() / height;

    for (int i = 0; i < height; i++) {
        QByteArray lineWdr = src.mid(i * lineSize, lineSize);
        le.append(lineWdr.mid(0, lineSize / 2));
        se.append(lineWdr.mid(lineSize / 2, lineSize / 2));
    }

    dst.append(le);
    dst.append(se);

    return dst;
}

QByteArray RawUtilWindow::yuvDataConvert(YUV_HEADER *head, QByteArray data)
{
    QByteArray dst, le, se;
    uint8_t pix_y0 = 0, pix_y1 = 0, pix_u = 0, pix_v = 0,
            temp_y0 = 0, temp_y1 = 0, temp_hy0 = 0, temp_hy1 = 0,
            temp_u = 0, temp_v = 0, temp_hu = 0, temp_hv = 0,
            pix_tmp = 0;
    uint32_t y_index = 0, uv_index = 0;

    if (head->pixelFormat == PIXEL_FORMAT_YUYV) {
        int pixel_num = head->width * head->height * 2;
        for (int i = 0; i < pixel_num; i += 4) {
            pix_y0 = data[i];
            pix_u = data[i + 1];
            pix_y1 = data[i + 2];
            pix_v = data[i + 3];

            temp_y0 = pix_y0 >> 2;
            temp_hy0 = (pix_y0 & 0x03) << 2;

            temp_u = pix_u >> 2;
            temp_hu = (pix_u & 0x03) << 2;

            temp_y1 = pix_y1 >> 2;
            temp_hy1 = (pix_y1 & 0x03) << 6;

            temp_v = pix_v >> 2;
            temp_hv = (pix_v & 0x03) << 6;

            le[y_index++] = temp_y0;
            le[y_index++] = temp_y1;
            le[y_index++] = temp_hy0 | temp_hy1;
            se[uv_index++] = temp_u;
            se[uv_index++] = temp_v;
            se[uv_index++] = temp_hu | temp_hv;
        }
    } else if (head->pixelFormat == PIXEL_FORMAT_NV21 || head->pixelFormat == PIXEL_FORMAT_NV12) {
        int start = head->stride[0] * head->height;
        for (int i = 0; i < head->height; i++) {
            for (int j = 0; j < head->width; j += 2) {
                pix_y0 = static_cast<uint8_t>(data[j + i * head->stride[0]]);
                pix_y1 = static_cast<uint8_t>(data[j + 1 + i * head->stride[0]]);
                pix_v = static_cast<uint8_t>(data[start + j + (i/2) * head->stride[1]]);
                pix_u = static_cast<uint8_t>(data[start + j + 1 + (i/2) * head->stride[1]]);

                if (head->pixelFormat == PIXEL_FORMAT_NV12) {
                    pix_tmp = pix_u;
                    pix_u = pix_v;
                    pix_v = pix_tmp;
                }

                temp_y0 = pix_y0 >> 2;
                temp_hy0 = (pix_y0 & 0x03) << 2;

                temp_u = pix_u >> 2;
                temp_hu = (pix_u & 0x03) << 2;

                temp_y1 = pix_y1 >> 2;
                temp_hy1 = (pix_y1 & 0x03) << 6;

                temp_v = pix_v >> 2;
                temp_hv = (pix_v & 0x03) << 6;

                le[y_index++] = temp_y0;
                le[y_index++] = temp_y1;
                le[y_index++] = temp_hy0 | temp_hy1;
                se[uv_index++] = temp_u;
                se[uv_index++] = temp_v;
                se[uv_index++] = temp_hu | temp_hv;
            }
        }
    } else if (head->pixelFormat == PIXEL_FORMAT_YUV_PLANAR_420) {
        int u_start = head->stride[0] * head->height;
        int v_start = u_start + head->stride[1] * head->height / 2;
        for (int i = 0; i < head->height; i++) {
            for (int j = 0; j < head->width; j += 2) {
                pix_y0 = static_cast<uint8_t>(data[j + i * head->stride[0]]);
                pix_y1 = static_cast<uint8_t>(data[j + 1 + i * head->stride[0]]);
                pix_u = static_cast<uint8_t>(data[u_start + ((j/2) + (i/2) * head->stride[1])]);
                pix_v = static_cast<uint8_t>(data[v_start + ((j/2) + (i/2) * head->stride[2])]);

                temp_y0 = pix_y0 >> 2;
                temp_hy0 = (pix_y0 & 0x03) << 2;

                temp_u = pix_u >> 2;
                temp_hu = (pix_u & 0x03) << 2;

                temp_y1 = pix_y1 >> 2;
                temp_hy1 = (pix_y1 & 0x03) << 6;

                temp_v = pix_v >> 2;
                temp_hv = (pix_v & 0x03) << 6;

                le[y_index++] = temp_y0;
                le[y_index++] = temp_y1;
                le[y_index++] = temp_hy0 | temp_hy1;
                se[uv_index++] = temp_u;
                se[uv_index++] = temp_v;
                se[uv_index++] = temp_hu | temp_hv;
            }
        }
    }

    dst.append(le);
    dst.append(se);
    head->size = dst.size();

    return dst;
}

void RawUtilWindow::openYuvFile(QString path)
{
    YuvImageOptionDialog yuvOptDialog;
    yuvOptDialog.setOptionData(yuvOption);
    yuvOptDialog.setFilePath(path);
    if (QDialog::Accepted == yuvOptDialog.exec()) {
        QByteArray data;
        yuvOption = yuvOptDialog.getOptionData();
        YuvImageListItem *item = new YuvImageListItem();
        item->fromYuvImgOption(yuvOption);
        item->setFStart(1);
        item->setFGop(1);

        QFile file(path);
        file.open(QIODevice::ReadOnly);
        QByteArray readData = file.readAll();
        file.close();

        int frameSize;
        int format = yuvOption.pixelType.toInt();
        int w = yuvOption.width;
        int h = yuvOption.height;
        YUV_HEADER head;
        head.curFrame = 0;
        head.width = w;
        head.height = h;
        if (format == 0) {
            head.pixelFormat = PIXEL_FORMAT_YUYV;
            frameSize = w * h * 2;
        } else if (format == 1) {
            head.pixelFormat = PIXEL_FORMAT_NV21;
            head.stride[0] = w;
            head.stride[1] = w;
            frameSize = w * h / 2 * 3;
        } else if (format == 2) {
            head.pixelFormat = PIXEL_FORMAT_NV12;
            head.stride[0] = w;
            head.stride[1] = w;
            frameSize = w * h / 2 * 3;
        } else if (format == 3) {
            head.pixelFormat = PIXEL_FORMAT_YUV_PLANAR_420;
            head.stride[0] = w;
            head.stride[1] = w / 4;
            head.stride[2] = w / 4;
            frameSize = w * h / 2 * 3;
        } else {
            QMessageBox::warning(this, tr("Error!"), tr("Not support yuv format!"), QMessageBox::Ok);
            return;
        }

        head.pixelFormat = PIXEL_FORMAT_YUYV;
        head.numFrame = readData.size() / frameSize;
        for (int i = 0; i < head.numFrame; i++) {
            QByteArray frameData = readData.mid(frameSize * i, frameSize);
            QByteArray tmpData = yuvDataConvert(&head, frameData);
            data.append(tmpData);
        }

        item->setFEnd(head.numFrame);
        item->setYuvInfo(head);
        item->setYuvdata(data);

        yuv_list_view->addTableItem(item);
    }
}

void RawUtilWindow::handleOpenImgFile()
{
    QString filename;
    QFileDialog file_dialog(this);

    QString filename_roi = "";
    QString filename_full = "";
    QString unpack_dir = "";
    QString raw_file_name = "";
    QString time_stamp = "";
    bool is_roi = false;
    roiMode = false;

    if (!isBatchMode) {
        filename = file_dialog.getOpenFileName(this, tr("Open raw file"), "", tr("*.raw *.yuv"));
    } else {
        filename = curBatchRawName;
    }

    time_stamp = findFileTimeStamp(filename);
    if (!filename.isEmpty()) {
        if (filename.endsWith(".raw")) {
            if (dataFormat) {
                yuv_list_view->del_all();
                main_layout->removeWidget(yuv_list_view);
                yuv_list_view->hide();
                main_layout->addWidget(raw_list_view, 0, 1);
                raw_list_view->show();
                dataFormat = 0;
            }
            if (filename.contains("_roi")) {
                is_roi = true;
                filename_roi = filename;
                int idx = filename.indexOf("_roi");
                filename_full = filename;
                filename_full = filename_full.replace(idx, filename.length() - idx, ".raw");
                qDebug("filename:%s\n", qUtf8Printable(filename));
                qDebug("filename_roi:%s\n", qUtf8Printable(filename_roi));
                qDebug("filename_full:%s\n", qUtf8Printable(filename_full));
                QFileInfo file(filename_full);
                if (!file.exists()) {
                    QMessageBox::warning(this, tr("Error!"), tr("Can't find full size raw file!"), QMessageBox::Ok);
                    return;
                }
                roiMode = true;
            } else {
                RAW_INFO temp;
                getRawInfoFromFile(filename, &temp);
                QString roi_str = QString("_roi=%1,%2,%3,%4,%5.raw").arg(temp.roiFrameNum).arg(temp.roiRect.x)
                        .arg(temp.roiRect.y).arg(temp.roiRect.w).arg(temp.roiRect.h);
                filename_roi = filename;
                filename_roi = filename_roi.replace(filename.length() - 4, 4, roi_str);
                filename_full = filename;
                qDebug("filename:%s\n", qUtf8Printable(filename));
                qDebug("filename_roi:%s\n", qUtf8Printable(filename_roi));
                qDebug("filename_full:%s\n", qUtf8Printable(filename_full));
                QFileInfo file(filename_roi);
                if (!file.exists()) {
                    roiMode = false;
                } else {
                    roiMode = true;
                }
            }
        } else if (filename.endsWith(".yuv")) {
            if (!dataFormat) {
                raw_list_view->del_all();
                main_layout->removeWidget(raw_list_view);
                raw_list_view->hide();
                if (yuv_list_view == nullptr) {
                    yuv_list_view = new YuvImageListWidget;
                }
                main_layout->addWidget(yuv_list_view, 0, 1);
                yuv_list_view->show();
                dataFormat = 1;
            }
            openYuvFile(filename);
            return;
        } else {
            QMessageBox::warning(this, tr("Error!"), tr("Not support file format!"), QMessageBox::Ok);
            return;
        }

        RawImageOptionDialog option_dialog;
        option_dialog.setData(rawOption);
        option_dialog.setFilePath(filename);

        if (isBatchMode || QDialog::Accepted == option_dialog.exec()) {
            QByteArray data, roiData;
            rawOption = option_dialog.getOptionData();
            RawImageListItem *item = new RawImageListItem();
            item->fromRawImgOption(rawOption);
            item->setFStart(1);
            item->setFGop(1);

            int width = item->getWidth();
            int height = item->getHeight();

            QFile file(filename_full);
            file.open(QIODevice::ReadOnly);
            QByteArray readData = file.readAll();
            file.close();

            RAW_INFO raw_info = {};
            getRawInfoFromFile(filename_full, &raw_info);
            raw_info.width = width;
            raw_info.height = height;
            raw_info.bayerID = item->getBayer().toUInt();
            raw_info.enWDR = item->getMode().toUInt();
            raw_info.curFrame = 0;
            QString soc = GlobalData::getInstance()->getCvipqToolSoc();

            if (findCropInfo) {
                int unpackSize;
                QByteArray tempData, cropUnpackData;
                RAW_HEADER pstHeader;
                pstHeader.fusionFrame = raw_info.enWDR;
                pstHeader.cropHeight = height;
                pstHeader.width = leWidth;
                pstHeader.height = leHeight;

                unpackSize = (pstHeader.width * 1.5 + 31) / 32 * 32 * pstHeader.height;
                if ( pstHeader.fusionFrame == 1) {
                    unpackSize *= 2;
                }
                pstHeader.compress = ( unpackSize == readData.size()) ? 0 : 1;

                qDebug("Raw replay, enWDR:%d, compress:%d, unpackSize:%d, readSize:%d\n",pstHeader.fusionFrame, pstHeader.compress, unpackSize, readData.size());

                RAW_INFO rawInfo;
                rawInfo.width = pstHeader.width;
                int tileIdx = 0;
                bool tileMode = utils::isTileMode(rawInfo, RAW_NORMAL, &tileIdx);

                if ( pstHeader.fusionFrame == 1) {
                    QByteArray le_data, se_data, wdr_data;

                    pstHeader.cropWidth = width / 2;
                    int size = readData.size() / 2 ;
                    //le data
                    pstHeader.cropX = leCropX;
                    pstHeader.cropY = leCropY;
                    tempData = readData.mid(0, size);
                    le_data = utils::cropUnpackRaw(tempData, pstHeader, tileMode, tileIdx);

                    //se data
                    pstHeader.cropX = seCropX;
                    pstHeader.cropY = seCropY;
                    tempData = readData.mid(size, size);
                    se_data = utils::cropUnpackRaw(tempData, pstHeader, tileMode, tileIdx);

                    wdr_data = RawLinearToWdr(le_data, se_data, pstHeader.height);
                    cropUnpackData.append(wdr_data);
                    if (soc == CV186X_SOC || soc == CV184X_SOC || soc == CV181X_SOC || soc == CV180X_SOC) {
                        data.append(le_data);
                        data.append(se_data);
                    } else {
                        data.append(wdr_data);
                    }
                } else {
                    pstHeader.cropWidth = width;
                    pstHeader.cropX = leCropX;
                    pstHeader.cropY = leCropY;
                    tempData = utils::cropUnpackRaw(readData, pstHeader, tileMode, tileIdx);
                    cropUnpackData.append(tempData);
                    data.append(tempData);
                }
                qDebug("Raw replay, width:%d, height:%d, cropWidth:%d, cropHeight:%d\n",
                       pstHeader.width, pstHeader.height, pstHeader.cropWidth, pstHeader.height);

                if (save_btn->isChecked()) {
                    QFileInfo file(filename_full);
                    unpack_dir += file.absolutePath() + "/";
                    unpack_dir += time_stamp;
                    if (is_roi) {
                        unpack_dir += "_UnpackRoiRaw";
                    } else {
                        unpack_dir += "_UnpackRaw";
                    }
                    QDir().mkdir(unpack_dir);
                    raw_file_name = unpack_dir + "/" + file.fileName();
                    setFileNameFrameNum(raw_file_name, raw_info.roiFrameNum);
                    QFile raw_file(raw_file_name);
                    raw_file.open(QIODevice::WriteOnly);
                    raw_file.write(cropUnpackData);
                    raw_file.close();
                }
            } else {
                if ((soc == CV186X_SOC || soc == CV184X_SOC || soc == CV181X_SOC || soc == CV180X_SOC) && raw_info.enWDR) {
                    int frmsize = 2 * width * height;
                    int frmnum = readData.size() / frmsize;
                    for (int i = 0; i < frmnum; i++) {
                        QByteArray wdrdata = readData.mid(i * frmsize, frmsize);
                        data.append(RawWdrToLInear(wdrdata, height));
                    }
                } else {
                    data.append(readData);
                }
            }

            int frame_num = data.size() / (height * width * sizeof(short));
            int frame_size = 1.5 * width * height;
            int buffer_size = frame_size * frame_num;
            unsigned char *packed_buffer = (unsigned char *) malloc(buffer_size);
            for (int i = 0, idx_unpacked = 0, idx_packed = 0; i < frame_num; i++) {
                utils::raw_unpacked2packed((unsigned short *)(data.data() + idx_unpacked), packed_buffer + idx_packed, width, height);
                idx_unpacked += (2 * width * height);
                idx_packed += frame_size;
            }

            QString expFilePath = filename_full;
            expFilePath.replace(".raw", "-expInfo.txt");
            qDebug()<<expFilePath;
            QFile expfile(expFilePath);
            if (expfile.exists()) {
                QList<EXP_INFO> list = getExpInfoFromFile(expFilePath);
                item->setExpInfo(list);
            }

            if (roiMode) {
                RoiInfo roiInfo;
                roiInfo.cropChange = false;
                roiInfo.frames = raw_info.roiFrameNum;
                roiInfo.width = roiInfo.cropWidth = raw_info.roiRect.w;
                roiInfo.height = roiInfo.cropHeight = raw_info.roiRect.h;
                roiInfo.cropX = roiInfo.cropY = 0;
                item->setRoiInfo(roiInfo);
                item->setRoiMode(true);
                item->setFrameNum(raw_info.roiFrameNum + 1);
                item->setRoiPath(filename_roi);
                roiData = handleImportRoiRawData(filename_roi, &raw_info, roiInfo);

                if (save_btn->isChecked()) {
                    saveUnpackedRaw(raw_file_name, data, roiData, raw_info, roiInfo);
                }
            } else {
                item->setFrameNum(frame_num);
                item->setRoiMode(false);
            }
            raw_info.numFrame = item->getFrameNum();
            raw_info.size = frame_size;
            item->setFEnd(item->getFrameNum());
            item->setRawInfo(raw_info);

            QList<RawIspInfo> RawIspInfo_list;
            RawIspInfo info;
            info.ISO = raw_info.ISO;
            info.LightValue = raw_info.lightValue;
            info.ColorTemp = raw_info.colorTemp;
            info.AGain = raw_info.exposureAGain;
            info.DGain = raw_info.exposureDGain;
            info.ISPDGain = raw_info.ispDGain;
            info.ExpTime[0] = raw_info.longExposure;
            info.ExpTime[1] = raw_info.shortExposure;
            info.Ratio = raw_info.exposureRatio;
            for (int i = 0; i < frame_num; i++) {
                RawIspInfo_list.append(info);
            }
            item->setRawIspInfo(std::move(RawIspInfo_list));
            QList<int> black_lvl;
            black_lvl.append(raw_info.BLC_Offset[0]);
            black_lvl.append(raw_info.BLC_Offset[1]);
            black_lvl.append(raw_info.BLC_Offset[2]);
            black_lvl.append(raw_info.BLC_Offset[3]);
            item->setBlackLvl(std::move(black_lvl));

            QByteArray raw_data_packed = QByteArray::fromRawData((const char *)packed_buffer, buffer_size);
            if (roiMode) {
                raw_data_packed.append(roiData);
            }
            item->setRawdata(raw_data_packed);
            raw_list_view->addTableItem(item);
            if (isBatchMode)
                raw_list_view->setCheckState(Qt::Checked);

            free(packed_buffer);
        }
    }
}

void RawUtilWindow::getRawInfoFromFile(QString path, RAW_INFO *raw_info)
{
    bool findAGain = false, findDGain = false;
    path.remove(path.length() - 4, 4);
    path.append(".txt");
    QFile raw_info_file(path);

    findCropInfo = false;
    if (raw_info_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream text_stream(&raw_info_file);
        while (!text_stream.atEnd()) {
            QString line = text_stream.readLine();
            QList<QString> raw_info_list = line.split('=');
            if (raw_info_list.size() == 2) {
                if (raw_info_list[0] == "ISO ") {
                    raw_info->ISO = raw_info_list[1].toInt();
                } else if (raw_info_list[0] == "Light Value ") {
                    raw_info->lightValue = raw_info_list[1].toFloat();
                } else if (raw_info_list[0] == "ISP DGain ") {
                    raw_info->ispDGain = raw_info_list[1].toInt();
                } else if (raw_info_list[0] == "Exposure Ratio ") {
                    raw_info->exposureRatio = raw_info_list[1].toInt();
                } else if (raw_info_list[0] == "Exposure AGain ") {
                    raw_info->exposureAGain = raw_info_list[1].toInt();
                    findAGain = true;
                } else if (raw_info_list[0] == "Exposure DGain ") {
                    raw_info->exposureDGain = raw_info_list[1].toInt();
                    findDGain = true;
                } else if (raw_info_list[0] == "Exposure AGainSF ") {
                    raw_info->AGainSF = raw_info_list[1].toInt();
                } else if (raw_info_list[0] == "Exposure DGainSF ") {
                    raw_info->DGainSF = raw_info_list[1].toInt();
                } else if (raw_info_list[0] == "Exposure ISPDGainSF ") {
                    raw_info->ispDGainSF = raw_info_list[1].toInt();
                } else if (raw_info_list[0] == "Exposure Time ") {
                    raw_info->longExposure = raw_info_list[1].toInt();
                } else if (raw_info_list[0] == "Short Exposure ") {
                    raw_info->shortExposure = raw_info_list[1].toInt();
                } else if (raw_info_list[0] == "reg_wbg_rgain ") {
                    raw_info->WB_RGain = raw_info_list[1].toInt();
                } else if (raw_info_list[0] == "reg_wbg_grgain ") {
                    raw_info->WB_GGain = raw_info_list[1].toInt();
                } else if (raw_info_list[0] == "reg_wbg_bgain ") {
                    raw_info->WB_BGain = raw_info_list[1].toInt();
                } else if (raw_info_list[0] == "reg_ccm_00 ") {
                    raw_info->CCM[0] = raw_info_list[1].toInt();
                } else if (raw_info_list[0] == "reg_ccm_01 ") {
                    raw_info->CCM[1] = raw_info_list[1].toInt();
                } else if (raw_info_list[0] == "reg_ccm_02 ") {
                    raw_info->CCM[2] = raw_info_list[1].toInt();
                } else if (raw_info_list[0] == "reg_ccm_10 ") {
                    raw_info->CCM[3] = raw_info_list[1].toInt();
                } else if (raw_info_list[0] == "reg_ccm_11 ") {
                    raw_info->CCM[4] = raw_info_list[1].toInt();
                } else if (raw_info_list[0] == "reg_ccm_12 ") {
                    raw_info->CCM[5] = raw_info_list[1].toInt();
                } else if (raw_info_list[0] == "reg_ccm_20 ") {
                    raw_info->CCM[6] = raw_info_list[1].toInt();
                } else if (raw_info_list[0] == "reg_ccm_21 ") {
                    raw_info->CCM[7] = raw_info_list[1].toInt();
                } else if (raw_info_list[0] == "reg_ccm_22 ") {
                    raw_info->CCM[8] = raw_info_list[1].toInt();
                } else if (raw_info_list[0] == "reg_blc_offset_r ") {
                    raw_info->BLC_Offset[0] = raw_info_list[1].toInt();
                } else if (raw_info_list[0] == "reg_blc_offset_gr ") {
                    raw_info->BLC_Offset[1] = raw_info_list[1].toInt();
                } else if (raw_info_list[0] == "reg_blc_offset_gb ") {
                    raw_info->BLC_Offset[2] = raw_info_list[1].toInt();
                } else if (raw_info_list[0] == "reg_blc_offset_b ") {
                    raw_info->BLC_Offset[3] = raw_info_list[1].toInt();
                } else if (raw_info_list[0] == "reg_blc_gain_r ") {
                    raw_info->BLC_Gain[0] = raw_info_list[1].toInt();
                } else if (raw_info_list[0] == "reg_blc_gain_gr ") {
                    raw_info->BLC_Gain[1] = raw_info_list[1].toInt();
                } else if (raw_info_list[0] == "reg_blc_gain_gb ") {
                    raw_info->BLC_Gain[2] = raw_info_list[1].toInt();
                } else if (raw_info_list[0] == "reg_blc_gain_b ") {
                    raw_info->BLC_Gain[3] = raw_info_list[1].toInt();
                } else if (raw_info_list[0] == "LE crop ") {
                    QString leCrop = QString::fromStdString(raw_info_list[1].toStdString());
                    QList<QString> le_info_list = leCrop.split(',');
                    leCropX = le_info_list[0].toInt();
                    leCropY = le_info_list[1].toInt();
                    leWidth = le_info_list[2].toInt();
                    leHeight = le_info_list[3].toInt();
                    findCropInfo = true;
                } else if (raw_info_list[0] == "SE crop ") {
                    QString seCrop = QString::fromStdString(raw_info_list[1].toStdString());
                    QList<QString> se_info_list = seCrop.split(',');
                    seCropX = se_info_list[0].toInt();
                    seCropY = se_info_list[1].toInt();
                    seWidth = se_info_list[2].toInt();
                    seHeight = se_info_list[3].toInt();
                    findCropInfo = true;
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
        if (!findAGain) {
            raw_info->exposureAGain = 1024;
            QMessageBox::information(this, tr("Info"), tr("Don't find Exposure AGain in raw info txt file,set it's value to 1024!"), QMessageBox::Ok);
        }
        if (!findDGain) {
            raw_info->exposureDGain = 1024;
            QMessageBox::information(this, tr("Info"), tr("Don't find Exposure DGain in raw info txt file,set it's value to 1024!"), QMessageBox::Ok);
        }
    }else {
        QMessageBox::warning(this, tr("Warning"), tr("Please make sure the raw info txt file exists!"), QMessageBox::Ok);
    }
}

QList<EXP_INFO> RawUtilWindow::getExpInfoFromFile(QString path)
{
    QList<EXP_INFO> list;
    QFile file(path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream text_stream(&file);
        while (!text_stream.atEnd()) {
            QString line = text_stream.readLine();
            QList<QString> strList = line.split('\t');
            if (strList.size() == 10) {
                if (strList[0] == "Time0") {
                    continue;
                } else {
                    EXP_INFO temp;
                    temp.time_le = strList[0].toInt();
                    temp.aGain_le = strList[1].toInt();
                    temp.dGain_le = strList[2].toInt();
                    temp.ispDGain_le = strList[3].toInt();
                    temp.time_se = strList[4].toInt();
                    temp.aGain_se = strList[5].toInt();
                    temp.dGain_se = strList[6].toInt();
                    temp.ispDGain_se = strList[7].toInt();
                    temp.ISO = strList[8].toInt();
                    temp.lightValue = (float)(strList[9].toInt()/100.0);
                    list.append(temp);
                    qDebug("get exp info,time le/se:%d,%d, ispDGain le/se:%d,%d, AGain le/se:%d,%d, DGain le/se:%d,%d, ISO:%d, lightValue:%f\n",
                        temp.time_le,temp.time_se,temp.ispDGain_le,temp.ispDGain_se,temp.aGain_le,temp.aGain_se,temp.dGain_le,temp.dGain_se,temp.ISO,temp.lightValue);
                }
            }
        }
        file.close();
    }else {
        QMessageBox::warning(this, tr("Warning"), tr("The expInfo.txt file was not found!"), QMessageBox::Ok);
    }

    return list;
}

QJsonObject RawUtilWindow::SetRawHeadJson(RAW_INFO header)
{
    QJsonObject obj;
    obj["pixformat"] = 0;
    obj["raw_numFrame"] = header.numFrame;
    obj["raw_curFrame"] = header.curFrame;
    obj["raw_width"] = header.width;
    obj["raw_height"] = header.height;
    obj["raw_bayerID"] = header.bayerID;
    obj["raw_enWDR"] = header.enWDR;
    obj["raw_ISO"] = header.ISO;
    obj["raw_lightValueX100"] = (int)(header.lightValue * 100);
    obj["raw_colorTemp"] = header.colorTemp;
    obj["raw_ispDGain"] = header.ispDGain;
    obj["raw_exposureRatio"] = header.exposureRatio;
    obj["raw_exposureAGain"] = header.exposureAGain;
    obj["raw_exposureDGain"] = header.exposureDGain;
    obj["raw_longExposure"] = header.longExposure;
    obj["raw_shortExposure"] = header.shortExposure;
    obj["raw_WB_RGain"] = header.WB_RGain;
    obj["raw_WB_BGain"] = header.WB_BGain;
    obj["raw_WB_GGain"] = header.WB_GGain;
    obj["raw_size"] = header.size;
    obj["raw_roiFrameNum"] = header.roiFrameNum;
    obj["raw_stRoiRect.s32X"] = header.roiRect.x;
    obj["raw_stRoiRect.s32Y"] = header.roiRect.y;
    obj["raw_stRoiRect.u32Width"] = header.roiRect.w;
    obj["raw_stRoiRect.u32Height"] = header.roiRect.h;
    obj["raw_roiFrameSize"] = header.roiFrameSize;
    obj["raw_op_mode"] = header.op_mode;
    obj["raw_AGainSF"] = header.AGainSF;
    obj["raw_DGainSF"] = header.DGainSF;
    obj["raw_ispDGainSF"] = header.ispDGainSF;
    obj["raw_BLCOffset"] =  utils::arrayPtrToJsonArray(header.BLC_Offset, 4);
    obj["raw_BLCGain"] = utils::arrayPtrToJsonArray(header.BLC_Gain, 4);

    return obj;
}

QJsonObject RawUtilWindow::SetYuvHeadJson(YUV_HEADER head)
{
    QJsonObject obj;
    obj["pixformat"] = head.pixelFormat;
    obj["yuv_numFrame"] = head.numFrame;
    obj["yuv_curFrame"] = head.curFrame;
    obj["yuv_width"] = head.width;
    obj["yuv_height"] = head.height;
    obj["yuv_size"] = (int)head.size;

    return obj;
}

void RawUtilWindow::slotSendYuv()
{
    QList<YuvImageListItem *> item_list = yuv_list_view->getSelYuvImageList();
    if(item_list.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("No yuv file is selected!"), QMessageBox::Ok);
        return;
    }

    YuvImageListItem *item = item_list.first();
    yuvHeader = item->getYuvInfo();
    replay_data.append(QByteArray(*item->getYuvdata()));
    fStart = item->getFStart();
    fEnd = item->getFEnd();
    yuvHeader.numFrame = fEnd - fStart + 1;
    yuvIndex = fEnd - fStart;
    QFileInfo fileInfo(item->getFilePath());
    dirPath = fileInfo.absolutePath();

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_RAWUTIL_SEND_RAW_HEAD;
    int id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

    QJsonObject param_obj = SetYuvHeadJson(yuvHeader);
    param_obj["content"] = RAW_BIN_DATA;
    param_obj["size"] = (int)yuvHeader.size;
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_SEND_BIN_INFO;
    send_obj[MSG_KEY_ID] = id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_PARAMS] = param_obj;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void RawUtilWindow::slotSend()
{
    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        return;
    }

    replay_data.clear();

    if (dataFormat) {
        slotSendYuv();
        return;
    }

    QList<RawImageListItem *> item_list = raw_list_view->getSelRawImageList();
    if(item_list.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("No raw file is selected!"), QMessageBox::Ok);
        return;
    }
    RawImageListItem *item = item_list.first();
    rawHeader = item->getRawInfo();
    replay_data.append(QByteArray(*item->getRawdata()));
    rawHeader.op_mode = RAW_OP_MODE_NORMAL;
    if (ae_sim->isChecked()) {
        rawHeader.op_mode = RAW_OP_MODE_AE_SIM;
    }
    if (awb_sim->isChecked()) {
        rawHeader.op_mode = RAW_OP_MODE_AWB_SIM;
    }

    QByteArray data;
    fStart = item->getFStart();
    fEnd = item->getFEnd();
    rawHeader.numFrame = fEnd - fStart + 1;
    roiMode = item->getRoiMode();
    yuvIndex = fEnd - fStart;
    QFileInfo fileInfo(item->getFilePath());
    dirPath = fileInfo.absolutePath();
    expInfoList = item->getExpInfo();
    if (!expInfoList.isEmpty()) {
        int idx = roiMode ? 0 : 1;
        rawHeader.ISO = expInfoList[idx].ISO;
        rawHeader.lightValue = expInfoList[idx].lightValue;
        rawHeader.longExposure = expInfoList[idx].time_le;
        rawHeader.shortExposure = expInfoList[idx].time_se;
        rawHeader.ispDGain = expInfoList[idx].ispDGain_le;
        rawHeader.ispDGainSF = expInfoList[idx].ispDGain_se;
        rawHeader.exposureDGain = expInfoList[idx].dGain_le;
        rawHeader.DGainSF = expInfoList[idx].dGain_se;
        rawHeader.exposureAGain = expInfoList[idx].aGain_le;
        rawHeader.AGainSF = expInfoList[idx].aGain_se;
        qDebug("rawHeader info,idx:0,ispDGain ls/se:%d,%d, expTime ls/se:%d,%d, DGain ls/se:%d,%d, AGain ls/se:%d,%d, ISO:%d,lightValue:%f\n",
               rawHeader.ispDGain,rawHeader.ispDGainSF,rawHeader.longExposure,rawHeader.shortExposure,rawHeader.exposureDGain,rawHeader.DGainSF,
               rawHeader.exposureAGain,rawHeader.AGainSF,rawHeader.ISO,rawHeader.lightValue);
    }
    if (roiMode) {
        RoiInfo roiInfo = item->getRoiInfo();
        if (roiInfo.cropChange) {
            replay_data.remove(rawHeader.size, replay_data.size() - rawHeader.size);
            QByteArray roiData = handleImportRoiRawData(item->getRoiPath(), &rawHeader, roiInfo);
            replay_data.append(roiData);
            item->setRawdata(replay_data);
        }
        if (fStart > 1) {
            rawHeader.numFrame = fEnd - fStart + 1 + 1;
            yuvIndex += 1;
        }
        rawHeader.roiFrameNum = rawHeader.numFrame - 1;
    }

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_RAWUTIL_SEND_RAW_HEAD;
    int id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

    QJsonObject param_obj = SetRawHeadJson(rawHeader);
    param_obj["content"] = RAW_BIN_DATA;
    param_obj["size"] = rawHeader.size;
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_SEND_BIN_INFO;
    send_obj[MSG_KEY_ID] = id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_PARAMS] = param_obj;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void RawUtilWindow::slotFlush()
{

}

void RawUtilWindow::slotCancel()
{
    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        return;
    }

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_RAWUTIL_CANCEL_RAW_REPLAY;
    int temp_id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(temp_id, id_info);

    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_CANCEL_RAW_REPLAY;
    send_obj[MSG_KEY_ID] = temp_id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void RawUtilWindow::handleReceiveMessageYuv(QString cmd)
{
    QString msg;

    if (cmd == CMD_RAWUTIL_SEND_RAW_HEAD) {
        QByteArray data;
        data = replay_data.mid((yuvHeader.curFrame + fStart - 1) * yuvHeader.size, yuvHeader.size);
        yuvHeader.curFrame++;

        NetworkClientHelper::getInstance()->setCallerInfo(0, CMD_RAWUTIL_SEND_RAW_DATA);
        NetworkClientHelper::getInstance()->sendData(data);
        return;
    } else if (cmd == CMD_RAWUTIL_SEND_RAW_DATA) {
        if (yuvHeader.curFrame < yuvHeader.numFrame) {
            QJsonObject id_info;
            id_info[MSG_KEY_CMD] = CMD_RAWUTIL_SEND_RAW_HEAD;
            int temp_id = GlobalData::getInstance()->getJsonrpcId();
            NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(temp_id, id_info);

            QJsonObject param_obj = SetYuvHeadJson(yuvHeader);
            param_obj["content"] = RAW_BIN_DATA;
            param_obj["size"] = (int)yuvHeader.size;
            QJsonObject send_obj;
            send_obj[MSG_KEY_METHOD] = METHOD_SEND_BIN_INFO;
            send_obj[MSG_KEY_ID] = temp_id;
            send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
            send_obj[MSG_KEY_PARAMS] = param_obj;
            NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
            return;
        } else if (yuvHeader.curFrame == yuvHeader.numFrame) {
            replay_data.clear();
            QJsonObject id_info;
            id_info[MSG_KEY_CMD] = CMD_RAWUTIL_START_RAW_REPLAY;
            int temp_id = GlobalData::getInstance()->getJsonrpcId();
            NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(temp_id, id_info);

            QJsonObject send_obj;
            send_obj[MSG_KEY_METHOD] = METHOD_START_RAW_REPLAY;
            send_obj[MSG_KEY_ID] = temp_id;
            send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
            NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
            return;
        } else {
            msg = "Send yuv data failed!";
        }
    } else if (cmd == CMD_RAWUTIL_START_RAW_REPLAY) {
        msg = "Start yuv replay successfully!";
        yuv_btn->setEnabled(true);
    } else if (cmd == CMD_RAWUTIL_CANCEL_RAW_REPLAY) {
        msg = "Cancel yuv replay successfully!";
        yuv_btn->setEnabled(false);
        startCapture = false;
    }

    QMessageBox message(QMessageBox::NoIcon, "Notice", msg);
    message.exec();
}

void RawUtilWindow::handleReceiveMessage(int id)
{
    qDebug(" RawUtilWindow::handleReceiveMessage\n");
    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);

    QJsonObject obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();
    QJsonObject info_obj = obj[MSG_KEY_ID_INFO].toObject();
    QString cmd = info_obj[MSG_KEY_CMD].toString();
    QString msg;

    if (result_obj.isEmpty()) {
        QJsonObject error_obj = obj[MSG_KEY_ERROR].toObject();
        QVariant code = error_obj[MSG_KEY_CODE].toVariant();
        QString message = error_obj[MSG_KEY_MESSAGE].toString();
        QString log;

        if (cmd == CMD_RAWUTIL_SEND_RAW_HEAD) {
            log = "Send raw head failed: ";
        } else if (cmd == CMD_RAWUTIL_SEND_RAW_DATA) {
            log = "Send raw data failed: ";
        } else if (cmd == CMD_RAWUTIL_START_RAW_REPLAY) {
            log = "Start raw replay failed: ";
        } else if (cmd == CMD_RAWUTIL_CANCEL_RAW_REPLAY) {
            log = "Cancel raw replay failed: ";
        }

        log = log + message + " ,errorcode = " + code.toString();
        QMessageBox::warning(this, tr("Error"), log, QMessageBox::Ok);
        return;
    }

    if (dataFormat) {
        handleReceiveMessageYuv(cmd);
        return;
    }

    if (cmd == CMD_RAWUTIL_SEND_RAW_HEAD) {
        QByteArray data;
        if (roiMode) {
            if (rawHeader.curFrame == 0) {
                data = replay_data.mid( 0, rawHeader.size);
            } else {
                if (fStart > 1) {
                    data = replay_data.mid(rawHeader.size + (fStart - 2 + rawHeader.curFrame - 1) * rawHeader.roiFrameSize, rawHeader.roiFrameSize);
                } else {
                    data = replay_data.mid(rawHeader.size + (rawHeader.curFrame - 1) * rawHeader.roiFrameSize, rawHeader.roiFrameSize);
                }
            }
        } else {
            data = replay_data.mid((rawHeader.curFrame + fStart - 1) * rawHeader.size, rawHeader.size);
        }
        rawHeader.curFrame++;

        NetworkClientHelper::getInstance()->setCallerInfo(0, CMD_RAWUTIL_SEND_RAW_DATA);
        NetworkClientHelper::getInstance()->sendData(data);
        return;
    } else if (cmd == CMD_RAWUTIL_SEND_RAW_DATA) {
        if (rawHeader.curFrame < rawHeader.numFrame) {
            if (!expInfoList.isEmpty()) {
                int idx = rawHeader.curFrame + 1;
                if (roiMode) {
                    if (fStart > 1) {
                        idx = fStart - 2 + rawHeader.curFrame;
                    } else {
                        idx = rawHeader.curFrame;
                    }
                }
                rawHeader.ISO = expInfoList[idx].ISO;
                rawHeader.lightValue = expInfoList[idx].lightValue;
                rawHeader.longExposure = expInfoList[idx].time_le;
                rawHeader.shortExposure = expInfoList[idx].time_se;
                rawHeader.ispDGain = expInfoList[idx].ispDGain_le;
                rawHeader.ispDGainSF = expInfoList[idx].ispDGain_se;
                rawHeader.exposureDGain = expInfoList[idx].dGain_le;
                rawHeader.DGainSF = expInfoList[idx].dGain_se;
                rawHeader.exposureAGain = expInfoList[idx].aGain_le;
                rawHeader.AGainSF = expInfoList[idx].aGain_se;
                qDebug("rawHeader info,idx:%d,ispDGain ls/se:%d,%d, expTime ls/se:%d,%d, DGain ls/se:%d,%d, AGain ls/se:%d,%d, ISO:%d,lightValue:%f\n",
                    idx,rawHeader.ispDGain,rawHeader.ispDGainSF,rawHeader.longExposure,rawHeader.shortExposure,rawHeader.exposureDGain,rawHeader.DGainSF,
                    rawHeader.exposureAGain,rawHeader.AGainSF,rawHeader.ISO,rawHeader.lightValue);
            }
            QJsonObject id_info;
            id_info[MSG_KEY_CMD] = CMD_RAWUTIL_SEND_RAW_HEAD;
            int temp_id = GlobalData::getInstance()->getJsonrpcId();
            NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(temp_id, id_info);

            QJsonObject param_obj = SetRawHeadJson(rawHeader);
            param_obj["content"] = RAW_BIN_DATA;
            if (roiMode) {
                param_obj["size"] = rawHeader.roiFrameSize;
            } else {
                param_obj["size"] = rawHeader.size;
            }
            QJsonObject send_obj;
            send_obj[MSG_KEY_METHOD] = METHOD_SEND_BIN_INFO;
            send_obj[MSG_KEY_ID] = temp_id;
            send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
            send_obj[MSG_KEY_PARAMS] = param_obj;
            NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
            return;
        } else if (rawHeader.curFrame == rawHeader.numFrame) {
            replay_data.clear();
            QJsonObject id_info;
            id_info[MSG_KEY_CMD] = CMD_RAWUTIL_START_RAW_REPLAY;
            int temp_id = GlobalData::getInstance()->getJsonrpcId();
            NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(temp_id, id_info);

            QJsonObject send_obj;
            send_obj[MSG_KEY_METHOD] = METHOD_START_RAW_REPLAY;
            send_obj[MSG_KEY_ID] = temp_id;
            send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
            NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
            return;
        } else {
            msg = "Send raw data failed!";
        }
    } else if (cmd == CMD_RAWUTIL_START_RAW_REPLAY) {
        msg = "Start raw replay  successfully!";
        yuv_btn->setEnabled(true);
    } else if (cmd == CMD_RAWUTIL_CANCEL_RAW_REPLAY) {
        msg = "Cancel raw replay successfully!";
        yuv_btn->setEnabled(false);
        startCapture = false;
    }

    if (!isBatchMode) {
        QMessageBox message(QMessageBox::NoIcon, "Notice", msg);
        message.exec();
    } else {
        batch_status = STS_SEND_RAW_OK;
    }
}


char batRawFileNoExt[1024][2048];
int batRawNum = 0;
int batRawCur = 0;
CaptureWindow *capture_window{nullptr};

void RawUtilWindow::batch_dumpAll()
{
    qDebug("batch_dumpAll");

    curBatchOutFileNameNoExt = batchMainTarDir + "/" + batRawFileNoExt[batRawCur];
    QFileInfo info1(curBatchOutFileNameNoExt);
    curBatchOutDir = info1.absolutePath();
    QDir().mkpath(curBatchOutDir);

    curBatchOutFileNameNoExt.replace("frame=6","frame=1");
    qDebug("curBatchOutDir %s",qUtf8Printable(curBatchOutDir));
    qDebug("cFileNameNoExt %s",qUtf8Printable(curBatchOutFileNameNoExt));


    QString histSrc = batchMainSourceDir + "/" + batRawFileNoExt[batRawCur] + "_hist.txt";
    QString histDes = batchMainTarDir + "/" + batRawFileNoExt[batRawCur] + "_hist.txt";
    QFile::copy(histSrc, histDes);

    capture_window->handleDumpAll(MODE_AE10RAW_BATCH, curBatchOutDir, curBatchOutFileNameNoExt);
#if 0 //test . save txt
    QByteArray testdata;
    QString outName = curBatchOutFileNameNoExt + ".txt";
    QFile raw_file(outName);
    raw_file.open(QIODevice::WriteOnly);
    raw_file.write(testdata);
    raw_file.close();
#endif
    batch_status = STS_WAIT_DUMP;
}

void RawUtilWindow::batch_conti()
{
    qDebug("batch_conti");

    batRawCur++;
    raw_list_view->del_all();

    if (batRawCur < batRawNum) {
        curBatchRawName = batchMainSourceDir + "/" + batRawFileNoExt[batRawCur] + ".raw";
        qDebug("batch_conti %s", qUtf8Printable(curBatchRawName));
        handleOpenImgFile();
        slotSend();
    } else {
        isBatchMode = 0;
        batch_status = STS_NONE;
        batch_timer->stop();
        qDebug("batch raw finish");
        QMessageBox message(QMessageBox::NoIcon, "Notice", "batch raw finish");
        message.exec();
    }
}

void RawUtilWindow::slotBatch()
{
    QString dateTime_str;
    QString msgStr;
    string dirStr;
    char dirname[1024];
    char cmdstr[1024], txtstr[1024],rawstr[1024];
    char frameStr[16]="frame=1";
    uint32_t i;

    //ae_sim->setChecked(true);
    if (ae_sim->isChecked()) {
        sprintf(frameStr,"frame=6");
    }


    if (capture_window == nullptr) {
        qDebug("Open capture");
        capture_window = new CaptureWindow();
        //capture_window->show();
        capture_window->activateWindow();
    }

    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        return;
    }

    raw_list_view->del_all();
    isBatchMode = true;
    batRawNum = 0;
    batRawCur = 0;

    //get raw path
    QString dir_path = QFileDialog::getExistingDirectory(this, "Please Select Batch Raw Path", "./");
    if (dir_path.isEmpty()) {
        return;
    }
    batchMainSourceDir = dir_path;

    dateTime_str = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    batchMainTarDir = batchMainSourceDir + "_Res_" + dateTime_str;

    dirStr = dir_path.toStdString();
    strcpy( dirname, dirStr.c_str() );
    sprintf(cmdstr,"dir \"%s\" /ON /b /s >FileList.txt",dirname);
    system(cmdstr);
    //qDebug("Opened Batch Raw Path: %s", qUtf8Printable(dir_path));
    qDebug("dirname :%s\n",dirname);
    //qDebug("cmdstr :%s\n",cmdstr);
    //qDebug("batchMainSourceDir :%s\n",qUtf8Printable(batchMainSourceDir));
    //qDebug("batchMainTarDir    :%s\n",qUtf8Printable(batchMainTarDir));

    //find *.raw & check txt
    FILE *listTxt;

    listTxt = fopen("FileList.txt","rb");
    if(listTxt == NULL) {
        qDebug("open FileList.txt error\n");
        return;
    }

    while (!feof(listTxt)) {
        fscanf(listTxt, "%s\n", cmdstr);
        if (strstr(cmdstr,".raw")&&(strstr(cmdstr,frameStr))) {
            strcpy(rawstr,cmdstr);
            strcpy(txtstr,cmdstr);
            txtstr[strlen(txtstr)-4] = 0;
            strcat(txtstr,".txt");
            fscanf(listTxt, "%s\n", cmdstr);
            //qDebug("raw:%s",rawstr);
            //qDebug("txt:%s",txtstr);
            //qDebug("tmp:%s",cmdstr);
            if (strcmp(cmdstr, txtstr) != 0) {
                msgStr =  txtstr;
                QMessageBox message(QMessageBox::NoIcon, "File is not exist", msgStr );
                message.exec();
                fclose(listTxt);
                return;
            }
            for (i = 0; i < strlen(rawstr); i++){
                if (rawstr[i]=='\\') {//change '\' to '/'
                    rawstr[i] = '/';
                }
            }
            rawstr[strlen(rawstr)-4] = 0;
            strcpy(batRawFileNoExt[batRawNum],&rawstr[strlen(dirname)+1]);
            qDebug("Raw :%s.raw", batRawFileNoExt[batRawNum]);
            batRawNum++;
        }
    }
    fclose(listTxt);
    qDebug("Raw file num:%d",batRawNum);

    if (batRawNum == 0) {
        QMessageBox message(QMessageBox::NoIcon, "Notice", "No Raw Data");
        message.exec();
        return;
    }

//batRawNum = 1;
    qDebug("Batch Raw Start");

    QDir().mkpath(batchMainTarDir);

    //save current json at batchMainTarDir ,IQ_Para.json
    MainWindow::getInstance()->saveFile(batchMainTarDir + "/IQ_Para.json");

    batch_status = STS_START;
    batch_timer->setInterval(1000);
    batch_timer->start();
}

void RawUtilWindow::batch_updateProgress()
{
    static int cnt = 0;

    if (batch_status != STS_NONE)
        qDebug("batch_status %d", batch_status);

    if(batch_status == STS_NONE) {
        return;
    } else if(batch_status == STS_START) {
        batch_status = STS_NONE;
        curBatchRawName = batchMainSourceDir + "/" + batRawFileNoExt[batRawCur] + ".raw";
        qDebug("__batch_start %s", qUtf8Printable(curBatchRawName));
        handleOpenImgFile();
        slotSend();
        cnt = 0;
    } else if(batch_status == STS_SEND_RAW_OK) {
        qDebug("__batch_send ok %d", cnt);
        cnt++;
        if (cnt > 3) {//for restart sensor
            MainWindow::getInstance()->writeAllData();
            batch_status = STS_WAIT_IMAGE_STABLE;
            cnt = 0;
        }
    } else if(batch_status == STS_WAIT_IMAGE_STABLE) {
        qDebug("__batch_wait stable %d", cnt);
        cnt++;
        if (cnt > 7) {//7 sec
            batch_status = STS_DUMP_ALL;
            batch_dumpAll();
            cnt = 0;
        }
    } else if(batch_status == STS_WAIT_DUMP) {
        qDebug("__batch_wait busy %d", capture_window->isDumpBusy());
        if (capture_window->isDumpBusy() == false) {
            qDebug("__batch_conti ok");
            batch_status = STS_NONE;
            batch_conti();
        }
    }

}

