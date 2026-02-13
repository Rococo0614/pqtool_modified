#include "bracketing_window.hpp"
#include "raw_image_format_dialog.hpp"
#include "utils.hpp"
#include "value_validator.hpp"
#include "global_data.hpp"
#include "main_window.hpp"

#include <QLabel>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>

const int MIN_WINDOW_WIDTH = 960;
const int MIN_WINDOW_HEIGHT = 480;
const int LINEAR_RAW = 0;
const int WDR_RAW = 1;
const int WDR_LE_RAW = 2;
const int WDR_SE_RAW = 3;

#define RAW_MODE_LINEAR (0)
#define RAW_MODE_WDR (1)
#define RAW_MODE_AUTO (2)

#define STD_IMAGE_WIDTH (1920)
#define STD_IMAGE_HEIGHT (1080)

#define RAW_LINE_BYTE (3840)
#define CV183X_LINE_BUFFER_WIDTH 2688
#define CV183X_TILE_START_INDEX 1920
#define CV182X_LINE_BUFFER_WIDTH 2304
#define CV182X_TILE_START_INDEX 1536

BracketingWindow::BracketingWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle(tr("Bracketing"));
    setWindowFlags(Qt::WindowMaximizeButtonHint|Qt::WindowCloseButtonHint);

    initMenu();
    initUI();
    initEvent();
}

void BracketingWindow::initMenu()
{
}

void BracketingWindow::initUI()
{
    main_widget = new QWidget();
    //main_widget->setStyleSheet("background-color: #ffffff;");
    main_widget->setMinimumSize(QSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT));
    main_layout = new QHBoxLayout();
    QWidget *widget = new QWidget();
    widget->setMaximumHeight(400);
    QVBoxLayout *left_layout = new QVBoxLayout();
    widget->setLayout(left_layout);

    bracketing_group = createBracketingGroupbox();
    img_viewer = new MultiImageViewerWidget();
    img_viewer->setFilePathHidden();
    ImageLabel *image_label = new ImageLabel(this, ImageLabel::NORMAL);
    connect(image_label, &ImageLabel::sigPositionChange, img_viewer, &MultiImageViewerWidget::slotPositionChange);
    connect(image_label, &ImageLabel::sigUpdateSingleRect, img_viewer, &MultiImageViewerWidget::handleRectToCropInfo);
    img_viewer->setImageLabel(image_label);
    img_viewer->setMinimumHeight(600);
    pixmap = new QPixmap;

    ae_btn = new QPushButton("AE 10 RAW");
    ae_btn->setFixedWidth(100);
    connect(ae_btn, &QPushButton::clicked, this, &BracketingWindow::get10AeRaw);

    shared_ptr<GlobalData> global_data = GlobalData::getInstance();
    QString save_dir = global_data->getSettings(SETTINGS_SECTION, KEY_BRACKET_SAVE_PATH).toString();
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
            global_data->setSettings(SETTINGS_SECTION, KEY_BRACKET_SAVE_PATH, "");
            global_data->saveSettings();
            QMessageBox::warning(this, tr("Warning"), tr("Can't find \"" + save_dir.toUtf8() + "\" directory! Use default save path."), QMessageBox::Ok);
        }
    } else {
        file_path->setText(exe_dir);
    }

    QHBoxLayout *filepath_layout = new QHBoxLayout;
    filepath_layout->addWidget(open_btn);
    filepath_layout->addWidget(file_path);
    connect(open_btn, &QPushButton::clicked, this, &BracketingWindow::OpenSaveFilePath);

    QVBoxLayout *checkbox_layout = new QVBoxLayout;
    save_bmp_file_checkbox = new QCheckBox("Save BMP file");
    checkbox_layout->addWidget(save_bmp_file_checkbox);

    left_layout->addWidget(bracketing_group, 1, Qt::AlignTop);
    left_layout->addWidget(ae_btn);
    left_layout->addLayout(filepath_layout, 0);
    left_layout->addLayout(checkbox_layout, 0);
    left_layout->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);

    main_layout->addWidget(widget, 1, Qt::AlignTop);
    main_layout->addWidget(img_viewer, 2);

    setCentralWidget(main_widget);
    centralWidget()->setLayout(main_layout);
}


QGroupBox *BracketingWindow::createBracketingGroupbox()
{
    QGroupBox * groupbox = new QGroupBox();
    groupbox->setTitle("Bracketing");

    bracketing_cancel_btn = new QPushButton(tr("Cancel"));
    bracketing_cancel_btn->setMaximumSize(200, 38);
    bracketing_cancel_btn->setEnabled(false);
    connect(bracketing_cancel_btn, &QPushButton::clicked,
            this, &BracketingWindow::handleCancelEvent);

    bracketing_capture_btn = new QPushButton;
    bracketing_capture_btn->setText(tr("Capture"));
    bracketing_capture_btn->setMaximumSize(200, 38);
    connect(bracketing_capture_btn, &QPushButton::clicked,
            this, &BracketingWindow::handledump);

    le_range_max = new DoubleSliderCaliItemWidget("LE Range Max");
    le_range_max->setRange(-10, 10);
    le_range_max->setData(2);
    le_range_min = new DoubleSliderCaliItemWidget("LE Range Min");
    le_range_min->setRange(-10, 10);
    le_range_min->setData(0);
    le_step = new DoubleSliderCaliItemWidget("LE Step");
    le_step->setRange(0.1, 2);
    le_step->setData(1);
    se_range_max = new DoubleSliderCaliItemWidget("SE Range Max");
    se_range_max->setRange(-10, 10);
    se_range_max->setData(-1);
    se_range_min = new DoubleSliderCaliItemWidget("SE Range Min");
    se_range_min->setRange(-10, 10);
    se_range_min->setData(-3);
    se_step = new DoubleSliderCaliItemWidget("SE Step");
    se_step->setRange(0.1, 2);
    se_step->setData(1);
    scene_item = new LineEditCaliItemWidget("Scene");
    scene_item->setData("office");
    frames_per_step_item = new LineEditCaliItemWidget("Frames Per Step");
    frames_per_step_item->setData(1);
    frames_per_step_item->setEnable(false);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(bracketing_capture_btn);
    layout->addWidget(bracketing_cancel_btn);

    QVBoxLayout *vertical_layout = new QVBoxLayout;
    vertical_layout->addWidget(le_range_max);
    vertical_layout->addWidget(le_range_min);
    vertical_layout->addWidget(le_step);
    vertical_layout->addWidget(se_range_max);
    vertical_layout->addWidget(se_range_min);
    vertical_layout->addWidget(se_step);
    vertical_layout->addWidget(scene_item);
    vertical_layout->addWidget(frames_per_step_item);
    vertical_layout->addLayout(layout);

    groupbox->setLayout(vertical_layout);
    //groupbox->setVisible(false);
    return groupbox;
}

void BracketingWindow::initEvent()
{
    shared_ptr<NetworkClientHelper> network_helper = NetworkClientHelper::getInstance();
    connect(network_helper.get(), &NetworkClientHelper::sigBracketSingleRawStatus, this,
            &BracketingWindow::handleSingleRawStatus);
    connect(network_helper.get(), &NetworkClientHelper::sigBracketBinInfo, this,
        &BracketingWindow::handleRecvBinInfo);
    connect(network_helper.get(), &NetworkClientHelper::sigBracketBinData, this,
        &BracketingWindow::handleRecvBinData);
    connect(network_helper.get(), &NetworkClientHelper::sigBracketSingleRawData, this,
            &BracketingWindow::handleSingleRawData);
    connect(this, &BracketingWindow::sigCaptureRawEnd, this, &BracketingWindow::handleGetSingleYuv);
    connect(network_helper.get(), &NetworkClientHelper::sigBracketSingleYuvStatus, this,
            &BracketingWindow::handleSingleYuvStatus);
    connect(network_helper.get(), &NetworkClientHelper::sigBracketSingleYuvData, this,
            &BracketingWindow::handleSingleYuvData);
    connect(this, &BracketingWindow::sigDumpEnd, this, &BracketingWindow::handleGetBracket);
    connect(network_helper.get(), &NetworkClientHelper::sigBracketStatus, this,
        &BracketingWindow::handleBracketStatus);
    connect(network_helper.get(), &NetworkClientHelper::sigBracketData, this,
            &BracketingWindow::handleBracketData);
}

void BracketingWindow::closeEvent(QCloseEvent *event)
{
    shared_ptr<NetworkClientHelper> network_helper = NetworkClientHelper::getInstance();
    event->accept();
    QMainWindow::closeEvent(event);
    //TODO handle close event
}

void BracketingWindow::refresh()
{
}

void BracketingWindow::get10AeRaw()
{
    dateTime_str = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    directory_str = file_path->text() + "/" + "AEB_" + dateTime_str;
    QDir().mkdir(directory_str);

    MainWindow::getInstance()->saveFile(directory_str + "/IQ_Para.json");

    ae10raw = true;
    disableComponents();
    bracketing_cancel_btn->setEnabled(true);
    //handleCaptureBracketing();
    start_dump = true;
    handleGetSingleRaw();
}

void BracketingWindow::saveBracketingFile(QByteArray &data, RAW_HEADER &header, int frameStep, int mode, int index, double leEV, double seEV)
{
    if (!directory_str.isEmpty()) {
        int width = 0;
        QString mode_str;
        QString bayer_str = "RGGB";

        if (header.bayerID < 4) {
            bayer_str = MW_BAYER_FORMAT_STRING[header.bayerID];
        }

        if (mode == LINEAR_RAW) {
            width = header.cropWidth;
            mode_str = bayer_str + "_Linear_";
        } else if (mode == WDR_RAW) {
            width = header.cropWidth * 2;
            mode_str = bayer_str + "_WDR_";
        }
        QString strNumber;
        strNumber.sprintf("_%02d_", index);
        QString path = directory_str + "/" + QString("%1X%2_").arg(width).arg(header.cropHeight);
        path = path + mode_str + dateTime_str + strNumber;
        path = path + QString("-color=3_-bits=12_-frame=%1_").arg(frameStep);
        path = path + QString("-hdr=%1_ISO=%2_").arg(header.fusionFrame).arg(header.iso);
        path = path + QString("-scene=%1_").arg(scene_item->getData().toString());
        QString strLeEV;
        strLeEV.sprintf("-leev=%+4.1f_", leEV);
        path = path + strLeEV;
        QString strSeEV;
        strSeEV.sprintf("-seev=%+4.1f_", seEV);
        path = path + strSeEV;
        yuv_bmp_path.append(path);
        QString pathRawInfo = path + ".txt";
        QString pathRaw = path + ".raw";
        QString pathRegDump = path + ".json";
        QString pathAeLog = path + "-aelog.txt";
        QString pathAwbLog = path + "-awblog.txt";
        QString pathAwbBin = path + "-awb.wbin";

        QFile raw_info_file(pathRawInfo);
        raw_info_file.open(QIODevice::WriteOnly);
        raw_info_file.write(raw_info_log[index].toUtf8());
        raw_info_file.close();

        QFile raw_file(pathRaw);
        raw_file.open(QIODevice::WriteOnly);
        raw_file.write(data);
        raw_file.close();

        QFile reg_dump_file(pathRegDump);
        reg_dump_file.open(QIODevice::WriteOnly);
        reg_dump_file.write(reg_dump_log[index].toUtf8());
        reg_dump_file.close();

        QFile ae_log_file(pathAeLog);
        ae_log_file.open(QIODevice::WriteOnly);
        ae_log_file.write(ae_log[index].toUtf8());
        ae_log_file.close();

        QFile awb_log_file(pathAwbLog);
        awb_log_file.open(QIODevice::WriteOnly);
        awb_log_file.write(awb_log[index].toUtf8());
        awb_log_file.close();

        QFile awb_bin_file(pathAwbBin);
        awb_bin_file.open(QIODevice::WriteOnly);
        awb_bin_file.write(awb_bin[index]);
        awb_bin_file.close();
    }else {
        QMessageBox::warning(this, tr("Error"), tr("Save directory path is empty!"), QMessageBox::Ok);
    }
}

void *new2d(int h, int w, int size)
{
    int i;
    void **p;

    p = NULL;

    // p = (void**)new char[h*sizeof(void*) + h*w*size];
    p = (void **)malloc(h * sizeof(void *) + h * w * size);
    if (p == NULL) {
        printf("Dll_Fatal Err , alloc NG @new2d w:%d h:%d size:%d\n", w, h, size);
        while (1) { };
    }
    for (i = 0; i < h; i++) {
        p[i] = ((char *)(p + h)) + i * w * size;
    }
    return p;
}

void BracketingWindow::saveAe10BracketingFile(RAW_HEADER &header)
{
#define NEW2D(H, W, TYPE) (TYPE **)new2d(H, W, sizeof(TYPE))

    if (!directory_str.isEmpty()) {
        int width = 0;
        QString mode_str;
        QString bayer_str = "RGGB";

        if (header.bayerID < 4) {
            bayer_str = MW_BAYER_FORMAT_STRING[header.bayerID];
        }
        if (header.fusionFrame == 1) {//linear mode
            width = header.cropWidth;
            mode_str = bayer_str + "_Linear_";

        } else {
            width = header.cropWidth * 2;
            mode_str = bayer_str + "_WDR_";
        }

        QString strNumber;
        strNumber.sprintf("_%02d_", 0);
        QString path = directory_str + "/" + QString("%1X%2_").arg(width).arg(header.cropHeight);
        path = path + mode_str + dateTime_str + strNumber;
        if (header.fusionFrame == 1) {//linear mode
            path = path + QString("-color=3_-bits=12_-frame=%1_").arg(6);//11??
        } else {
            path = path + QString("-color=3_-bits=12_-frame=%1_").arg(6);
        }
        path = path + QString("-hdr=%1_ISO=%2_").arg(header.fusionFrame).arg(100);
        path = path + QString("-scene=%1_").arg(scene_item->getData().toString());
        yuv_bmp_path.append(path);
        QString pathRawInfo = path + ".txt";
        QString pathRaw = path + ".raw";

        QFile raw_info_file(pathRawInfo);
        raw_info_file.open(QIODevice::WriteOnly);
        raw_info_file.write(raw_info_log[4].toUtf8());//ISO 100 info
        raw_info_file.close();

        string fnamestr = pathRaw.toStdString();
        char fname[1024];
        FILE *outraw;

        uint32_t i, j;
        uint8_t *tmpbuf;

        uint32_t frame_num = totalStep;
        char *le;
        int frame_size = wdr_raw_data_list[0].size();

        if (header.fusionFrame == 1) {//linear mode
            frame_size = header.cropWidth * header.cropHeight * 2;
        }
        printf("frame_num:%d %d\n",frame_num, frame_size);
        printf("WH :%d %d\n", width,header.cropHeight);
        tmpbuf = (uint8_t*)malloc(header.cropHeight * width * 2);
        strcpy( fname, fnamestr.c_str() );

        outraw = fopen(fname,"wb");
        if (header.fusionFrame == 1) {
            fwrite(tmpbuf, 1, header.cropHeight * width * 2, outraw);//dummy
            for (i = 0; i < frame_num ; i++) {
                QByteArray raw_data = receive_raw_data.mid(i * frame_size * framesPerStep, frame_size * framesPerStep);
                le = raw_data.data();
                fwrite(le, 1, header.cropHeight * width * 2 , outraw);
            }
        } else {
            fwrite(tmpbuf, 1,header.cropHeight * width * 2, outraw);//dummy
            for (i = 0; i < frame_num ; i++) {
                le = wdr_raw_data_list[i].data();
                fwrite(le, 1, header.cropHeight * width ,outraw);
            }
        }
        free(tmpbuf);
        fclose(outraw);


        FILE *inRaw;
        uint32_t k, w, h;
        uint16_t **rawbuff[10];
        uint8_t **isOK;
        uint32_t val, blc, blcgain;
        float **finalRaw;
        uint32_t color_id;
        float fval;
        uint32_t finalHist[HIST_SIZE];//-10 ~ 20 EV ,per EV 256
        QString histtxt = path + "_hist.txt";

        //Linear & WDR: 1920 1080
        w = header.cropWidth;
        h = header.cropHeight;
        color_id = header.bayerID;
        blc = (header.blcOffset[1] + header.blcOffset[2])/2;
        printf("RAW %d %d %d %d\n",w,h,color_id,blc);
        blcgain = 4096 * 1024 / (4096 - blc);

        inRaw = fopen(fname, "rb");
        if (inRaw == NULL) {
            printf("can't open %s\n", fname);
            return;
        }

        isOK = NEW2D(h, w, uint8_t);
        memset(&isOK[0][0], 0, w * h);

        finalRaw = NEW2D(h, w, float);
        if (header.fusionFrame == 1) {//linear mode
            fseek(inRaw, w * h * 2 * 1, SEEK_SET);// LE Dummy RAW
        } else {
            fseek(inRaw, w * h * 2 * 2, SEEK_SET);// LE + SE Dummy RAW
        }
        for (i = 0; i < 10; i++) {
            rawbuff[i] = NEW2D(h, w, uint16_t);
            fread(&rawbuff[i][0][0], h * w * 2, 1, inRaw);
        }
        fclose(inRaw);

        // luma  16   1024   4096
        // 2^    4     10    12
        // gap      6      2
        for (i = 0; i < h; i++) {
            for (j = 0; j < w; j++) {
                for (k = 0; k < 10; k++) {
                    val = rawbuff[k][i][j];
                    if (val < 4000) {
                        isOK[i][j] = 1;
                        if (val > blc) {
                            val -= blc;
                        } else {
                            val = 0;
                        }

                        val = (val * blcgain) >> 10;
                        if (val == 0) {
                            val = 1;
                        }
                        if (val > 4095) {
                            val = 4095;
                        }
                        fval = val / (float)1024.0;
                        fval = (EV_OFFSET + k * 2 + log(fval) / log(2));
                        finalRaw[i][j] = fval;
                        break;
                    }
                }//k
            }//j
        }//i
        // calc Hist
        memset(finalHist, 0, sizeof(uint32_t) * HIST_SIZE);
        for (i = 0; i < h; i += 2) {
            for (j = 0; j < w; j += 2) {
                if (color_id == 1 || color_id == 2) {
                    fval = (finalRaw[i][j] + finalRaw[i + 1][j + 1]) / 2.0;
                } else {
                    fval = (finalRaw[i][j + 1] + finalRaw[i + 1][j]) / 2.0;
                }
                fval = HIST_PER_EV * (fval) + 0.5;
                val = fval;
                if (val > (HIST_SIZE - 1)) {
                    val = HIST_SIZE - 1;
                    printf("@\n");
                }
                finalHist[val]++;
            }
        }

        FILE *outHist;

        strcpy( fname, histtxt.toStdString().c_str() );
        outHist = fopen(fname, "wb");
        for (i = 0; i < HIST_SIZE; i++) {
            fprintf(outHist, "%d\n", finalHist[i]);
        }
        fclose(outHist);
        free(isOK);
        free(finalRaw);


    }else {
        QMessageBox::warning(this, tr("Error"), tr("Save directory path is empty!"), QMessageBox::Ok);
    }
}


QByteArray BracketingWindow::cropUnpackRaw(QByteArray src, RAW_HEADER &header)
{
    QByteArray src_6b, src_12b, crop_image, det_6b;
    ushort temp[4] = {0};
    int h, w, m, n, i, width;
    uchar v0, v1, v2;
    int src_stride = src.size() / header.height;
    char c = 0, zero = 0;

    if (header.compress == 0) {
        // Unpack and Crop RAW
        for (h = header.cropY; h < header.cropHeight + header.cropY; h++) {
             for (w = header.cropX, m = header.cropX / 2 * 3, i = h * src_stride; w < header.cropWidth + header.cropX; w+=2, m+=3) {
                 v0 = (uchar) src.at(i + m);
                 v1 = (uchar) src.at(i + m + 1);
                 v2 = (uchar) src.at(i + m + 2);
                 temp[0] = static_cast<ushort>((v0 << 4) | ((v2 >> 0) & 0x0f));
                 temp[1] = static_cast<ushort>((v1 << 4) | ((v2 >> 4) & 0x0f));

                 c = temp[0] & 0xFF;
                 crop_image.append(c);
                 c = (temp[0] >> 8) & 0xFF;
                 crop_image.append(c);

                 c = temp[1] & 0xFF;
                 crop_image.append(c);
                 c = (temp[1] >> 8) & 0xFF;
                 crop_image.append(c);
             }
         }
    } else if (header.compress == 1) {
        int line_buffer_width = 0xfffffff;
        int title_start_idx = 0xfffffff;
        QString soc = GlobalData::getInstance()->getCvipqToolSoc();
        if (CV182X_SOC == soc) {
            line_buffer_width = CV182X_LINE_BUFFER_WIDTH;
            title_start_idx = CV182X_TILE_START_INDEX;
        } else if (CV183X_SOC == soc) {
            line_buffer_width = CV183X_LINE_BUFFER_WIDTH;
            title_start_idx = CV183X_TILE_START_INDEX;
        }

        // Unpack RAW
        if (header.width > line_buffer_width) {
            width = header.width + 8;
        } else {
            width = header.width;
        }

        for (h = 0; h < header.height; h++) {
            for (w = 0, m = 0, i = h * src_stride; w < width; w+=4, m+=3) {
                v0 = (uchar) src.at(i + m);
                v1 = (uchar) src.at(i + m + 1);
                v2 = (uchar) src.at(i + m + 2);
                temp[0] = static_cast<ushort>(((v0 & 0x03) << 4) | (v2 & 0x0F ));
                temp[1] = static_cast<ushort>((v0 & 0xFC) >> 2);
                temp[2] = static_cast<ushort>(((v1 & 0x03) << 4) | ((v2 & 0xF0) >> 4));
                temp[3] = static_cast<ushort>((v1 & 0xFC) >> 2);

                c = temp[0] & 0xFF;
                src_6b.append(c);
                src_6b.append(zero);
                c = temp[1] & 0xFF;
                src_6b.append(c);
                src_6b.append(zero);
                c = temp[2] & 0xFF;
                src_6b.append(c);
                src_6b.append(zero);
                c = temp[3] & 0xFF;
                src_6b.append(c);
                src_6b.append(zero);
            }
        }

        if (width > line_buffer_width) {
            for (int x = 0; x < header.height; x++) {
                for (int y = 0, k = 0; y < header.width * 2; y++, k++) {
                    if (y == title_start_idx * 2) {
                        k += (8 * 2);
                    }
                    det_6b.append(src_6b.at(x * width * 2 + k));
                }
            }
        } else {
            det_6b.append(src_6b);
        }
        src_12b.resize(header.width * header.height * 2);
        utils::dpcm_rx((ushort *)det_6b.data(), (ushort *)src_12b.data(), header.width, header.height, 7);

        // Crop Raw
        ushort *pSrc_12b, *pCrop_image;
        crop_image.resize(header.cropWidth * header.cropHeight * 2);
        pSrc_12b = (ushort *)src_12b.data();
        pCrop_image = (ushort *)crop_image.data();
        for (h = 0, n = header.cropY; h < header.cropHeight; h++, n++) {
            for (w = 0, m = header.cropX; w < header.cropWidth; w++, m++) {
                pCrop_image[w + h * header.cropWidth] = pSrc_12b[m + n * header.width];
            }
        }
    } else {
        QMessageBox::warning(this, tr("Error"), tr("The received compress mode is wrong!"), QMessageBox::Ok);
    }

    return crop_image;
}

void BracketingWindow::handleGetBracket()
{
    QString strScene = scene_item->getData().toString();
    if (strScene.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("Please input scene text!"), QMessageBox::Ok);
        return;
    }

    int n = frames_per_step_item->getData().toInt();
    if ((!ae10raw) && (n < 1 || n > 8)) {
        QMessageBox::warning(this, tr("Error"), tr("Please input number(1~8) to Fram per step!"), QMessageBox::Ok);
        return;
    }

    if ((!ae10raw) && (le_range_max->getData().toDouble() <= le_range_min->getData().toDouble())) {
        QMessageBox::warning(this, tr("Error"), tr("Please check LE Range. Max value must be greater than Min value."), QMessageBox::Ok);
        return;
    }

    if ((!ae10raw) && (se_range_max->getData().toDouble() <= se_range_min->getData().toDouble())) {
        QMessageBox::warning(this, tr("Error"), tr("Please check SE Range. Max value must be greater than Min value."), QMessageBox::Ok);
        return;
    }

    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        QMessageBox::warning(this, tr("Warning"), tr("Network not connected!"), QMessageBox::Ok);
        return;
    }

    quint8 fusionFrame = GlobalData::getInstance()->GetWDRFusionFrame();

    if (start_capture) {
        stopCapture();
    }

    start_capture = true;
    currentStep = 0;
    leEV.clear();
    seEV.clear();
    raw_info_log.clear();
    reg_dump_log.clear();
    ae_log.clear();
    awb_log.clear();
    awb_bin.clear();
    yuv_data_list.clear();
    yuv_head_list.clear();
    yuv_bmp_path.clear();

    if (ae10raw) {
        framesPerStep = 1;
        totalStep = 10;
        for (int i = 0; i < totalStep; i++) {
            leEV.append(0);
            seEV.append(0);
        }
    } else {
        double leRangeMax = le_range_max->getData().toDouble();
        double leRangeMin = le_range_min->getData().toDouble();
        double leStep = le_step->getData().toDouble();
        double seRangeMax = se_range_max->getData().toDouble();
        double seRangeMin = se_range_min->getData().toDouble();
        double seStep = se_step->getData().toDouble();
        framesPerStep = frames_per_step_item->getData().toInt();
        int leTotalStep = (leRangeMax - leRangeMin) / leStep + 1;
        int seTotalStep = (seRangeMax - seRangeMin) / seStep + 1;
        if (fusionFrame > 1) {
            totalStep = leTotalStep * seTotalStep;
            for (int i = 0; i < leTotalStep; i++) {
                for(int j = 0; j < seTotalStep; j++) {
                    leEV.append((leRangeMin + i * leStep) * 10);
                    seEV.append((seRangeMin + j * seStep) * 10);
                }
            }
        } else {
            totalStep = leTotalStep;
            for (int i = 0; i < leTotalStep; i++) {
                leEV.append((leRangeMin + i * leStep) * 10);
                seEV.append(0);
            }
        }
    }

    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_BRACKET_START;
    int id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

    QJsonObject param_obj;
    param_obj["ae10raw"] = ae10raw;
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_BRACKET_START;
    send_obj[MSG_KEY_ID] = id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_PARAMS] = param_obj;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void BracketingWindow::handleBracketStatus(int id)
{
    qDebug("BracketingWindow::handleBracketStatus\n");

    QJsonObject obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();
    if (result_obj.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), "Get bracketing data failed!", QMessageBox::Ok);
        return;
    }
    QJsonObject info_obj = obj[MSG_KEY_ID_INFO].toObject();
    QString cmd = info_obj[MSG_KEY_CMD].toString();

    if (cmd == CMD_BRACKET_START) {
        QJsonObject id_info;
        id_info[MSG_KEY_CMD] = CMD_BRACKET_GET_DATA;
        int id = GlobalData::getInstance()->getJsonrpcId();
        NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

        QJsonObject param_obj;
        param_obj["leEV"] = leEV[0];
        param_obj["seEV"] = seEV[0];
        QJsonObject send_obj;
        send_obj[MSG_KEY_METHOD] = METHOD_BRACKET_GET_DATA;
        send_obj[MSG_KEY_ID] = id;
        send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
        send_obj[MSG_KEY_PARAMS] = param_obj;
        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
    } else if (cmd == CMD_BRACKET_GET_DATA) {
        QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();
        rawSize = params_obj["Data size"].toInt();
        raw_info_log.append(params_obj["Raw info log"].toString());
        if (ae10raw) {
            NetworkClientHelper::getInstance()->setCallerInfo(rawSize, CMD_BRACKET_GET_DATA);
            qDebug("Set receive size:%d\n", rawSize);

            int temp_id = GlobalData::getInstance()->getJsonrpcId();
            QJsonObject send_obj;
            send_obj[MSG_KEY_METHOD] = METHOD_GET_IMAGE_DATA;
            send_obj[MSG_KEY_ID] = temp_id;
            send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
            NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
        } else {
            reg_dump_log.append(params_obj["Reg dump log"].toString());
            ae_log.append(params_obj["AE log"].toString());
            awb_log.append(params_obj["AWB log"].toString());

            QJsonObject id_info;
            id_info[MSG_KEY_CMD] = CMD_BRACKET_GET_MULTI_AWB_BIN;
            int temp_id = GlobalData::getInstance()->getJsonrpcId();
            NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(temp_id, id_info);

            QJsonObject param_obj;
            param_obj["content"] = AWB_BIN_DATA;
            QJsonObject send_obj;
            send_obj[MSG_KEY_METHOD] = METHOD_GET_BIN_INFO;
            send_obj[MSG_KEY_ID] = temp_id;
            send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
            send_obj[MSG_KEY_PARAMS] = param_obj;
            NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
        }
    } else if (cmd == CMD_BRACKET_FINISH) {
        start_capture = false;
        if (ae10raw) {
            ae10raw = false;
            QMessageBox message(QMessageBox::NoIcon, "Notice", QString("Get 10 ae raw successfully!"));
            message.exec();
        } else {
            for (int i = 0; i < totalStep; i++) {
                YUV_HEADER *pstHeader = (YUV_HEADER *) yuv_head_list[i].data();
                saveYuvFile(yuv_data_list[i], *pstHeader, 1, i);
            }
            yuv_head_list.clear();
            yuv_data_list.clear();
            QMessageBox message(QMessageBox::NoIcon, "Notice", QString("Bracketing successfully!"));
            message.exec();
        }
    }
}

void BracketingWindow::handleBracketData(QString cmd)
{
    qDebug("BracketingWindow::handleBracketData\n");

    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);
    QByteArray data = NetworkClientHelper::getInstance()->popCmdData(cmd);
    if(!start_capture) {
        return;
    }

    bool remain = false;
    int received_frames = 0;
    int step = 0;

    QByteArray header = data.left(sizeof(RAW_HEADER));
    RAW_HEADER *pstHeader = (RAW_HEADER *) header.data();
    data.remove(0, sizeof(RAW_HEADER));

    qDebug("numframe:%d, curframe:%d, width:%d, height:%d,  compress:%d",
           pstHeader->numFrame, pstHeader->curFrame, pstHeader->width, pstHeader->height, pstHeader->compress);
    qDebug("fusionFrame:%d, bayerid:%d, cropX:%d, cropY:%d, cropWidth:%d, cropHeight:%d", pstHeader->fusionFrame,
           pstHeader->bayerID, pstHeader->cropX, pstHeader->cropY, pstHeader->cropWidth, pstHeader->cropHeight);
    qDebug("data.size:%d, pstHeader->size:%d", data.size(), pstHeader->size);

    if (data.size() != (int)pstHeader->size) {
        QMessageBox::warning(this, tr("Error"), tr("Bracketing, The received bracketing data size is wrong!"), QMessageBox::Ok);
        return;
    }

    //crop & unpack
    QByteArray d = cropUnpackRaw(data, *pstHeader);
    step = pstHeader->fusionFrame;
    if (step == 1) {
        receive_raw_data.append(d);
    } else {
        wdr_raw_data_list.append(d);
    }

    if(pstHeader->curFrame < pstHeader->numFrame - 1) {
        remain = true;
    } else {
        remain = false;
        if (!ae10raw) {
            if (currentStep < totalStep - 1) {
                handleGetSingleYuv();
                return;
            } else {
                handleGetSingleYuv();
            }
        }
    }

    if (remain) {
        //call for remain frames
        NetworkClientHelper::getInstance()->setCallerInfo(rawSize, CMD_BRACKET_GET_DATA);
        qDebug("Set receive size:%d\n", rawSize);

        int id = GlobalData::getInstance()->getJsonrpcId();
        QJsonObject send_obj;
        send_obj[MSG_KEY_METHOD] = METHOD_GET_IMAGE_DATA;
        send_obj[MSG_KEY_ID] = id;
        send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
    } else if (currentStep < totalStep - 1) {
        currentStep++;
        QJsonObject id_info;
        id_info[MSG_KEY_CMD] = CMD_BRACKET_GET_DATA;
        int id = GlobalData::getInstance()->getJsonrpcId();
        NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

        QJsonObject param_obj;
        param_obj["leEV"] = leEV[currentStep];
        param_obj["seEV"] = seEV[currentStep];
        QJsonObject send_obj;
        send_obj[MSG_KEY_METHOD] = METHOD_BRACKET_GET_DATA;
        send_obj[MSG_KEY_ID] = id;
        send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
        send_obj[MSG_KEY_PARAMS] = param_obj;
        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
    } else {
        if (step == 1) {
            int index = 0, oneFrameSize = pstHeader->cropWidth * pstHeader->cropHeight * 2;
            if (!ae10raw) {
                for (int i = 0; i < totalStep; i++) {
                    QByteArray raw_data = receive_raw_data.mid(index, oneFrameSize * framesPerStep);
                    saveBracketingFile(raw_data, *pstHeader, framesPerStep, LINEAR_RAW, i, leEV[i]/10.0, seEV[i]/10.0);
                    index += oneFrameSize * framesPerStep;
                }
                img_viewer->setRawImgData(receive_raw_data, pstHeader->cropWidth, pstHeader->cropHeight,
                                          (utils::RAW_BAYER_ID) pstHeader->bayerID, utils::RAW_BITS::BITS_12, utils::RAW_PACK_TYPE::RAW_UNPACKED);
            } else {
                saveAe10BracketingFile(*pstHeader);
            }
            receive_raw_data.clear();
        } else {
            QByteArray wdr_data, wdr_le_data, wdr_se_data;
            int frame_num = wdr_raw_data_list.count();
            int raw_line_byte = pstHeader->cropWidth * 2;
            char *line = new char[raw_line_byte];

            memset(line, 0, raw_line_byte);
            for (int i = 0; i < frame_num - 1; i += 2) {
                char *le = wdr_raw_data_list[i].data();
                char *se = wdr_raw_data_list[i + 1].data();
                int frame_size = wdr_raw_data_list[i].size();
                for (int j = 0; j < frame_size; j += raw_line_byte) {
                    memcpy(line, le+j, raw_line_byte);
                    wdr_data.append(line, raw_line_byte);
                    memcpy(line, se+j, raw_line_byte);
                    wdr_data.append(line, raw_line_byte);
                }
            }

            int index = 0, oneFrameSize = wdr_raw_data_list[0].size() * 2;
            if (!ae10raw) {
                for (int i = 0; i < totalStep; i++) {
                    QByteArray raw_data = wdr_data.mid(index, oneFrameSize * framesPerStep);
                    saveBracketingFile(raw_data, *pstHeader, framesPerStep, WDR_RAW, i, leEV[i]/10.0, seEV[i]/10.0);
                    index += oneFrameSize * framesPerStep;
                }
                img_viewer->setRawImgData(wdr_data, pstHeader->cropWidth*2, pstHeader->cropHeight,
                                          (utils::RAW_BAYER_ID) pstHeader->bayerID, utils::RAW_BITS::BITS_12, utils::RAW_PACK_TYPE::RAW_UNPACKED);
            } else {
                saveAe10BracketingFile(*pstHeader);
            }

            wdr_raw_data_list.clear();
            delete [] line;
        }
        rawSize = 0;
        enableComponents();
        bracketing_cancel_btn->setEnabled(false);
        if (ae10raw) {
            QJsonObject id_info;
            id_info[MSG_KEY_CMD] = CMD_BRACKET_FINISH;
            int id = GlobalData::getInstance()->getJsonrpcId();
            NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);
            QJsonObject send_obj;
            send_obj[MSG_KEY_METHOD] = METHOD_BRACKET_FINISH;
            send_obj[MSG_KEY_ID] = id;
            send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
            NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
        }
    }
}

void BracketingWindow::handleGetSingleYuv()
{
    qDebug("BracketingWindow::handleGetSingleYuv\n");

    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        return;
    }

    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_BRACKET_GET_SINGLE_YUV;
    int id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_SINGLE_YUV;
    send_obj[MSG_KEY_ID] = id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void BracketingWindow::handleSingleYuvStatus(int id)
{
    qDebug("BracketingWindow::handleSingleYuvStatus\n");

    QJsonObject obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    if((!start_capture) && (!start_dump)) {
        return;
    }

    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();
    if (result_obj.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), "Get single yuv failed!", QMessageBox::Ok);
        return;
    }
    QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();
    int size = params_obj["Data size"].toInt();
    NetworkClientHelper::getInstance()->setCallerInfo(size, CMD_BRACKET_GET_SINGLE_YUV);
    qDebug("Set receive size:%d\n", size);

    int temp_id = GlobalData::getInstance()->getJsonrpcId();
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_IMAGE_DATA;
    send_obj[MSG_KEY_ID] = temp_id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void BracketingWindow::handleSingleYuvData(QString cmd)
{
    qDebug("BracketingWindow::handleSingleYuvData\n");

    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);
    QByteArray data = NetworkClientHelper::getInstance()->popCmdData(cmd);
    if((!start_capture) && (!start_dump)) {
        return;
    }

    QByteArray header = data.left(sizeof(YUV_HEADER));
    YUV_HEADER *pstHeader = (YUV_HEADER *) header.data();
    data.remove(0, sizeof(YUV_HEADER));

    qDebug("numframe:%d, curframe:%d, width:%d, height:%d, stride:%d,%d,%d, format::%d\n", pstHeader->numFrame, pstHeader->curFrame,
        pstHeader->width, pstHeader->height, pstHeader->stride[0], pstHeader->stride[1], pstHeader->stride[2], pstHeader->pixelFormat);
    qDebug("data.size:%d, pstHeader->size:%d\n", data.size(), pstHeader->size);

    if (data.size() != (int)(pstHeader->size)) {
        QMessageBox::warning(this, tr("Error"), tr("Bracketing, The received yuv data size is wrong!"), QMessageBox::Ok);
        return;
    }

    yuv_head_list.append(header);
    yuv_data_list.append(data);
    if (start_dump) {
        start_dump = false;
        saveYuvFile(yuv_data_list[0], *pstHeader, 1, 0);
        yuv_head_list.clear();
        yuv_data_list.clear();
        emit sigDumpEnd();
    } else {
        if (currentStep < totalStep - 1) {
            currentStep++;
            QJsonObject id_info;
            id_info[MSG_KEY_CMD] = CMD_BRACKET_GET_DATA;
            int id = GlobalData::getInstance()->getJsonrpcId();
            NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

            QJsonObject param_obj;
            param_obj["leEV"] = leEV[currentStep];
            param_obj["seEV"] = seEV[currentStep];
            QJsonObject send_obj;
            send_obj[MSG_KEY_METHOD] = METHOD_BRACKET_GET_DATA;
            send_obj[MSG_KEY_ID] = id;
            send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
            send_obj[MSG_KEY_PARAMS] = param_obj;
            NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
        } else {
            QJsonObject id_info;
            id_info[MSG_KEY_CMD] = CMD_BRACKET_FINISH;
            int id = GlobalData::getInstance()->getJsonrpcId();
            NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);
            QJsonObject send_obj;
            send_obj[MSG_KEY_METHOD] = METHOD_BRACKET_FINISH;
            send_obj[MSG_KEY_ID] = id;
            send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
            NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
        }
    }
}

void BracketingWindow::saveYuvFile(QByteArray &data, YUV_HEADER &header, int frames, int index)
{
    int n = 0, i = 0, idx = 0;
    unsigned int frame_start = 0, u_start = 0, v_start = 0, uv_start = 0;
    const char *pData = data.data();

    if (!directory_str.isEmpty()) {
        QString yuv_path = yuv_bmp_path[index] + ".yuv";
        QString bmp_path = yuv_bmp_path[index] + ".bmp";
        QString jpg_path = yuv_bmp_path[index] + ".jpg";
        QFile fileMultiYuv(yuv_path);
        fileMultiYuv.open(QIODevice::WriteOnly);
        QImage image = utils::convertYUV2RGBImage(data, &header);
        image.save(jpg_path, "JPG");
        if (save_bmp_file_checkbox->isChecked()) {
            image.save(bmp_path, "BMP");
        }

        if (header.pixelFormat == PIXEL_FORMAT_NV21 || header.pixelFormat == PIXEL_FORMAT_NV12) {
            for(n = 0; n < frames; n++) {
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
            for(n = 0; n < frames; n++) {
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

void BracketingWindow::handleGetSingleRaw()
{
    qDebug("BracketingWindow::handleCaptureSingleRaw\n");

    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        return;
    }

    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);

    raw_info_log.clear();
    reg_dump_log.clear();
    ae_log.clear();
    awb_log.clear();
    awb_bin.clear();
    ae_bin.clear();
    yuv_bmp_path.clear();
    receive_raw_data.clear();
    wdr_raw_data_list.clear();

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_BRACKET_GET_SINGLE_RAW;
    int id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);
    QJsonObject param_obj;
    param_obj["frames"] = 1;
    param_obj["tightly"] = false;
    param_obj["dump_reg"] = true;
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_MULTI_RAW;
    send_obj[MSG_KEY_ID] = id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_PARAMS] = param_obj;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void BracketingWindow::handleSingleRawStatus(int id)
{
    qDebug("BracketingWindow::handleSingleRawStatus\n");

    QJsonObject obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    if(!start_dump) {
        return;
    }

    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();
    if (result_obj.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("Get single raw failed!"), QMessageBox::Ok);
        return;
    }

    // get logs
    QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();
    reg_dump_log.append(params_obj["Reg dump log"].toString());
    raw_info_log.append(params_obj["Raw info log"].toString());
    ae_log.append(params_obj["AE log"].toString());
    awb_log.append(params_obj["AWB log"].toString());
    rawSize = params_obj["Data size"].toInt();

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_BRACKET_GET_AE_BIN;
    int temp_id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(temp_id, id_info);

    QJsonObject param_obj;
    param_obj["content"] = AE_BIN_DATA;
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_BIN_INFO;
    send_obj[MSG_KEY_ID] = temp_id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_PARAMS] = param_obj;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void BracketingWindow::handleRecvBinInfo(int id)
{
    qDebug("BracketingWindow::handleRecvBinInfo\n");

    QJsonObject obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    if((!start_capture) && (!start_dump)) {
        return;
    }

    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();
    if (result_obj.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("Get bin info failed!"), QMessageBox::Ok);
        return;
    }

    QJsonObject info_obj = obj[MSG_KEY_ID_INFO].toObject();
    QString cmd = info_obj[MSG_KEY_CMD].toString();
    QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();
    recvSize = params_obj["Data size"].toInt();

    BinDataType type = NONE_BIN_DATA;
    if (cmd == CMD_BRACKET_GET_AE_BIN) {
        type = AE_BIN_DATA;
    } else if ((cmd == CMD_BRACKET_GET_AWB_BIN) || (cmd == CMD_BRACKET_GET_MULTI_AWB_BIN)) {
        type = AWB_BIN_DATA;
    }

    NetworkClientHelper::getInstance()->setCallerInfo(recvSize, cmd);
    qDebug("Set receive size:%d\n", recvSize);

    int temp_id = GlobalData::getInstance()->getJsonrpcId();
    QJsonObject param_obj;
    param_obj["content"] = type;
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_BIN_DATA;
    send_obj[MSG_KEY_ID] = temp_id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_PARAMS] = param_obj;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void BracketingWindow::handleRecvBinData(QString cmd)
{
    qDebug("BracketingWindow::handleRecvBinData\n");

    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);
    QByteArray data = NetworkClientHelper::getInstance()->popCmdData(cmd);
    if((!start_capture) && (!start_dump)) {
        return;
    }

    recvSize = 0;
    if (cmd == CMD_BRACKET_GET_AE_BIN) {
        ae_bin.append(data);
        QJsonObject id_info;
        id_info[MSG_KEY_CMD] = CMD_BRACKET_GET_AWB_BIN;
        int id = GlobalData::getInstance()->getJsonrpcId();
        NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

        QJsonObject param_obj;
        param_obj["content"] = AWB_BIN_DATA;
        QJsonObject send_obj;
        send_obj[MSG_KEY_METHOD] = METHOD_GET_BIN_INFO;
        send_obj[MSG_KEY_ID] = id;
        send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
        send_obj[MSG_KEY_PARAMS] = param_obj;
        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
    } else if (cmd == CMD_BRACKET_GET_AWB_BIN) {
        awb_bin.append(data);
        NetworkClientHelper::getInstance()->setCallerInfo(rawSize, CMD_BRACKET_GET_SINGLE_RAW);
        qDebug("Set receive size:%d\n", rawSize);

        int id = GlobalData::getInstance()->getJsonrpcId();
        QJsonObject send_obj;
        send_obj[MSG_KEY_METHOD] = METHOD_GET_IMAGE_DATA;
        send_obj[MSG_KEY_ID] = id;
        send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
    } else if (cmd == CMD_BRACKET_GET_MULTI_AWB_BIN) {
        awb_bin.append(data);
        NetworkClientHelper::getInstance()->setCallerInfo(rawSize, CMD_BRACKET_GET_DATA);
        qDebug("Set receive size:%d\n", rawSize);

        int temp_id = GlobalData::getInstance()->getJsonrpcId();
        QJsonObject send_obj;
        send_obj[MSG_KEY_METHOD] = METHOD_GET_IMAGE_DATA;
        send_obj[MSG_KEY_ID] = temp_id;
        send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
    }
}

void BracketingWindow::handleSingleRawData(QString cmd)
{
    qDebug("BracketingWindow::handleSingleRawData\n");

    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);
    QByteArray data = NetworkClientHelper::getInstance()->popCmdData(cmd);
    if(!start_dump) {
        return;
    }

    bool remain = false;
    int received_frames = 0;
    int step = 0;

    QByteArray header = data.left(sizeof(RAW_HEADER));
    RAW_HEADER *pstHeader = (RAW_HEADER *) header.data();
    data.remove(0, sizeof(RAW_HEADER));

    qDebug("numframe:%d, curframe:%d, width:%d, height:%d,  compress:%d",
           pstHeader->numFrame, pstHeader->curFrame, pstHeader->width, pstHeader->height, pstHeader->compress);
    qDebug("fusionFrame:%d, bayerid:%d, cropX:%d, cropY:%d, cropWidth:%d, cropHeight:%d", pstHeader->fusionFrame,
           pstHeader->bayerID, pstHeader->cropX, pstHeader->cropY, pstHeader->cropWidth, pstHeader->cropHeight);
    qDebug("data.size:%d, pstHeader->size:%d", data.size(), pstHeader->size);

    if (data.size() != (int)pstHeader->size) {
        QMessageBox::warning(this, tr("Error"), tr("The received data size is wrong!"), QMessageBox::Ok);
        return;
    }

    //crop & unpack
    QByteArray d = cropUnpackRaw(data, *pstHeader);
    if (d.isEmpty()) {
        // Compress Mode != 0 or 1
        return;
    }

    step = pstHeader->fusionFrame;
    if (step == 1) {
        receive_raw_data.append(d);
    } else {
        wdr_raw_data_list.append(d);
    }

    if(pstHeader->curFrame < pstHeader->numFrame - 1) {
        remain = true;
    } else if (pstHeader->curFrame == pstHeader->numFrame - 1) {
        remain = false;
    }

    if (remain) {
        //call for remain frames
        NetworkClientHelper::getInstance()->setCallerInfo(rawSize, CMD_BRACKET_GET_SINGLE_RAW);
        qDebug("Set receive size:%d\n", rawSize);

        int id = GlobalData::getInstance()->getJsonrpcId();
        QJsonObject send_obj;
        send_obj[MSG_KEY_METHOD] = METHOD_GET_IMAGE_DATA;
        send_obj[MSG_KEY_ID] = id;
        send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
    } else {
        if (step == 1) {
            received_frames = receive_raw_data.count() / (pstHeader->cropWidth * pstHeader->cropHeight * 2);
            saveRawFile(receive_raw_data, *pstHeader, LINEAR_RAW, received_frames, 0);
            receive_raw_data.clear();
        } else {
            QByteArray wdr_data, wdr_le_data, wdr_se_data;
            int frame_num = wdr_raw_data_list.count();
            int raw_line_byte = pstHeader->cropWidth * 2;
            char *line = new char[raw_line_byte];

            received_frames = frame_num / step;
            memset(line, 0, raw_line_byte);
            for (int i = 0; i < frame_num - 1; i += 2) {
                char *le = wdr_raw_data_list[i].data();
                char *se = wdr_raw_data_list[i + 1].data();
                int frame_size = wdr_raw_data_list[i].size();
                for (int j = 0; j < frame_size; j += raw_line_byte) {
                    memcpy(line, le+j, raw_line_byte);
                    wdr_data.append(line, raw_line_byte);
                    memcpy(line, se+j, raw_line_byte);
                    wdr_data.append(line, raw_line_byte);
                }
            }

            saveRawFile(wdr_data, *pstHeader, WDR_RAW, received_frames, 0);
            wdr_raw_data_list.clear();
            delete [] line;
        }
        rawSize = 0;
        emit sigCaptureRawEnd();
    }
}

void BracketingWindow::saveRawFile(QByteArray &data, RAW_HEADER &header, int raw_type, int frameNum, int index)
{
    if (!directory_str.isEmpty()) {
        int width = 0;
        QString mode_str;
        QString bayer_str = "RGGB";

        if (header.bayerID < 4) {
            bayer_str = MW_BAYER_FORMAT_STRING[header.bayerID];
        }

        if (raw_type == LINEAR_RAW) {
            width = header.cropWidth;
            mode_str = bayer_str + "_Linear_";
        } else if (raw_type == WDR_RAW) {
            width = header.cropWidth * 2;
            mode_str = bayer_str + "_WDR_";
        }
        QString strNumber;
        strNumber.sprintf("_%02d_", index);
        QString path = directory_str + "/" + QString("%1X%2_").arg(width).arg(header.cropHeight);
        path = path + mode_str + dateTime_str + strNumber;
        path = path + QString("-color=%1_-bits=12_-frame=%2_").arg(header.bayerID).arg(frameNum);
        if (!ae10raw)
            path = path + QString("-hdr=%1_ISO=%2_").arg(header.fusionFrame).arg(header.iso);
        else {
            path = path + QString("-hdr=%1_ISO=%2_").arg(header.fusionFrame).arg(100);
            path = path + QString("-scene=%1_").arg(scene_item->getData().toString());
        }

        yuv_bmp_path.append(path);
        QString pathRawInfo = path + ".txt";
        QString pathRaw = path + ".raw";
        QString pathRegDump = path + ".json";
        QString pathAeLog = path + "-aelog.txt";
        QString pathAwbLog = path + "-awblog.txt";
        QString pathAwbBin = path + "-awb.wbin";
        QString pathAeBin = path + "-ae.bin";

        QFile raw_info_file(pathRawInfo);
        raw_info_file.open(QIODevice::WriteOnly);
        raw_info_file.write(raw_info_log[index].toUtf8());
        raw_info_file.close();

        QFile raw_file(pathRaw);
        raw_file.open(QIODevice::WriteOnly);
        raw_file.write(data);
        raw_file.close();

        QFile reg_dump_file(pathRegDump);
        reg_dump_file.open(QIODevice::WriteOnly);
        reg_dump_file.write(reg_dump_log[index].toUtf8());
        reg_dump_file.close();

        QFile ae_log_file(pathAeLog);
        ae_log_file.open(QIODevice::WriteOnly);
        ae_log_file.write(ae_log[index].toUtf8());
        ae_log_file.close();

        QFile awb_log_file(pathAwbLog);
        awb_log_file.open(QIODevice::WriteOnly);
        awb_log_file.write(awb_log[index].toUtf8());
        awb_log_file.close();

        QFile awb_bin_file(pathAwbBin);
        awb_bin_file.open(QIODevice::WriteOnly);
        awb_bin_file.write(awb_bin[index]);
        awb_bin_file.close();

        QFile ae_bin_file(pathAeBin);
        ae_bin_file.open(QIODevice::WriteOnly);
        ae_bin_file.write(ae_bin[index]);
        ae_bin_file.close();
    } else {
        QMessageBox::warning(this, tr("Error"), tr("Save directory path is empty!"), QMessageBox::Ok);
    }
}

void BracketingWindow::handledump()
{
    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        return;
    }

    dateTime_str = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    directory_str = file_path->text() + "/Bracketing_" + dateTime_str;
    QDir().mkdir(directory_str);

    ae10raw = false;
    disableComponents();
    bracketing_cancel_btn->setEnabled(true);
    start_dump = true;
    handleGetSingleRaw();
}

void BracketingWindow::handleCancelEvent()
{
    stopCapture();
    start_capture = false;
    start_dump = false;
    ae10raw = false;
    enableComponents();
    bracketing_cancel_btn->setEnabled(false);
    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);
}

void BracketingWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        this->close();
    }
}

void BracketingWindow::OpenSaveFilePath()
{
    shared_ptr<GlobalData> global_data = GlobalData::getInstance();
    QString open_dir = global_data->getSettings(SETTINGS_SECTION, KEY_BRACKET_SAVE_PATH).toString();
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
        global_data->setSettings(SETTINGS_SECTION, KEY_BRACKET_SAVE_PATH, dir_path);
        global_data->saveSettings();
    }
}

void BracketingWindow::stopCapture()
{
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_STOP_GET_IMAGE_DATA;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void BracketingWindow::disableComponents()
{
    bracketing_capture_btn->setEnabled(false);
    open_btn->setEnabled(false);
    file_path->setEnabled(false);
    img_viewer->setEnabled(false);
    ae_btn->setEnabled(false);
    save_bmp_file_checkbox->setEnabled(false);
    le_range_max->setEnabled(false);
    le_range_min->setEnabled(false);
    le_step->setEnabled(false);
    se_range_max->setEnabled(false);
    se_range_min->setEnabled(false);
    se_step->setEnabled(false);
    scene_item->setEnabled(false);
}

void BracketingWindow::enableComponents()
{
    bracketing_capture_btn->setEnabled(true);
    open_btn->setEnabled(true);
    file_path->setEnabled(true);
    img_viewer->setEnabled(true);
    ae_btn->setEnabled(true);
    save_bmp_file_checkbox->setEnabled(true);
    le_range_max->setEnabled(true);
    le_range_min->setEnabled(true);
    le_step->setEnabled(true);
    se_range_max->setEnabled(true);
    se_range_min->setEnabled(true);
    se_step->setEnabled(true);
    scene_item->setEnabled(true);
}
