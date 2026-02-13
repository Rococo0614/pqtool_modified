#include <QLabel>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QSemaphore>

#include "auto_analyser_window.hpp"
#include "network_client_helper.hpp"
#include "raw_image_format_dialog.hpp"
#include "utils.hpp"
#include "global_data.hpp"

const int MIN_WINDOW_WIDTH = 1150;
const int MIN_WINDOW_HEIGHT = 750;

const int POLLING_PERIOD_MS = 6000;

static const QList<QString> capture_src_list{CAPTURE_SRC_VI, CAPTURE_SRC_VPSS, CAPTURE_SRC_VO};
/* Hide H2 Statistics */
// static const QList<QString> fv_key_list {KEY_ATTR_AF_ZONE_V1, KEY_ATTR_AF_ZONE_H1, KEY_ATTR_AF_ZONE_H2, KEY_ATTR_AF_ZONE_HLCNT};
static const QList<QString> fv_key_list {KEY_ATTR_AF_ZONE_V1, KEY_ATTR_AF_ZONE_H1, KEY_ATTR_AF_ZONE_HLCNT};

AutoAnalyserWindow::AutoAnalyserWindow(QWidget *parent):
    QMainWindow(parent)
{
    setWindowTitle(tr("3A Analyser"));

    timer = new QTimer(this);

    initMenu();
    initUI();
    initEvent();
}

void AutoAnalyserWindow::initMenu()
{
}

void AutoAnalyserWindow::initUI()
{
    main_widget = new QWidget();
    main_widget->setMinimumSize(QSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT));
    main_layout = new QVBoxLayout();

    QHBoxLayout *header_layout = new QHBoxLayout;
    cap_checkbox = new QCheckBox;
    QLabel *cap_label = new QLabel(tr("Capture Image From:"));
    QComboBox *src_combobox = new QComboBox;
    src_combobox->addItems(capture_src_list);
    src_combobox->setCurrentIndex(0);
    src_combobox->setEnabled(false);

    connect(cap_checkbox, &QCheckBox::clicked, [=]{
        if (cap_checkbox->isChecked()) {
            if (!NetworkClientHelper::getInstance()->isNormalStatus())
            {
                QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
                status_label->setText(tr("communication is not ready."));
                return;
            }
            status_label->setText(tr("communication is ready."));
            src_idx = src_combobox->currentIndex();
            timer->start(POLLING_PERIOD_MS);
            use_yuv_show = true;
            qDebug("Captue image from src(%d)", src_idx);
        } else {
            timer->stop();
        }
    });

    check_state_label = new QLabel(tr("Current Checking:None"));

    // QPushButton *decr_btn = new QPushButton("-");
    // QPushButton *incr_btn = new QPushButton("+");
    // QLineEdit *factor_lineedit = new QLineEdit();
    // QPushButton *reset_btn = new QPushButton(tr("reset"));

    QPushButton *ae_btn = new QPushButton(tr("AE"));
    QPushButton *af_btn = new QPushButton(tr("AF"));
    QPushButton *awb_btn = new QPushButton(tr("AWB"));

    connect(ae_btn, &QPushButton::clicked, this, &AutoAnalyserWindow::slotCheckAEInfo);
    connect(af_btn, &QPushButton::clicked, this, &AutoAnalyserWindow::slotCheckAFInfo);
    connect(awb_btn, &QPushButton::clicked, this, &AutoAnalyserWindow::slotCheckAWBInfo);

    header_layout->addWidget(cap_checkbox, 1);
    header_layout->addWidget(cap_label, 1);
    header_layout->addWidget(src_combobox, 1);
    header_layout->addStretch(3);
    header_layout->addWidget(check_state_label, 1);
    header_layout->addStretch(3);
    // header_layout->addWidget(decr_btn, 1);
    // header_layout->addWidget(factor_lineedit, 1);
    // header_layout->addWidget(incr_btn, 1);
    // header_layout->addWidget(reset_btn, 1);
    header_layout->addStretch(3);
    header_layout->addWidget(ae_btn, 1);
    //header_layout->addWidget(af_btn, 1);
    header_layout->addWidget(awb_btn, 1);

    scroll_area = new QScrollArea;
    scroll_area->setBackgroundRole(QPalette::Dark);
    img_label = new ImageLabel(this, ImageLabel::GRID_3A);
    img_label->setScaleFactor(1.0);
    scroll_area->setWidget(img_label);

    QHBoxLayout *bottom_layout = new QHBoxLayout;
    status_label = new QLabel(tr("ready"));
    img_source_label = new QLabel(tr("image source: none"));
    bottom_layout->addWidget(status_label);
    bottom_layout->addStretch();
    bottom_layout->addWidget(img_source_label);

    main_layout->addLayout(header_layout);
    main_layout->addWidget(scroll_area);
    main_layout->addLayout(bottom_layout);

    setCentralWidget(main_widget);
    centralWidget()->setLayout(main_layout);

    // AEStatWindow
    ae_window = new AEStatWindow;
    connect(ae_window, &AEStatWindow::sigWinClose, this, &AutoAnalyserWindow::slotCloseInfoWindow);

    // AFStatWindow
    af_window = new AFStatWindow;
    connect(af_window, &AFStatWindow::sigWinClose, this, &AutoAnalyserWindow::slotCloseInfoWindow);
    connect(af_window, &AFStatWindow::sigChangeFocusItem, this, &AutoAnalyserWindow::slotChangeFocusItem);

    // AWBStatWindow
    awb_window = new AWBStatWindow;
    connect(awb_window, &AWBStatWindow::sigWinClose, this, &AutoAnalyserWindow::slotCloseInfoWindow);
}

void AutoAnalyserWindow::initEvent()
{
    shared_ptr<NetworkClientHelper> network_helper = NetworkClientHelper::getInstance();
    connect(network_helper.get(), &NetworkClientHelper::sig3AYuvStatus, this, &AutoAnalyserWindow::handleImageStatus);
    connect(network_helper.get(), &NetworkClientHelper::sig3AYuvData, this, &AutoAnalyserWindow::slotUpdateImage);
    connect(network_helper.get(), &NetworkClientHelper::sig3AStatisticsInfo, this, &AutoAnalyserWindow::slotUpdateData);
    connect(network_helper.get(), &NetworkClientHelper::sig3ABinInfo, this, &AutoAnalyserWindow::HandleAWBBinStatus);
    connect(network_helper.get(), &NetworkClientHelper::sig3ABinData, this, &AutoAnalyserWindow::HandleAWBBinData);
    connect(network_helper.get(), &NetworkClientHelper::sigCaptue3ARawStatus, this, &AutoAnalyserWindow::handleSingleRawStatus);
    connect(network_helper.get(), &NetworkClientHelper::sig3ARawData, this, &AutoAnalyserWindow::slotUpdateRawData);

    connect(timer, &QTimer::timeout, this, &AutoAnalyserWindow::slotSendCapCmd);
    connect(img_label, &ImageLabel::sigGridCellSel, this, &AutoAnalyserWindow::slotUpdateInfoOnSel);
}

void AutoAnalyserWindow::closeEvent(QCloseEvent *event)
{
    if (timer->isActive()) {
        timer->stop();
    }

    cap_checkbox->setChecked(false);
    if (cur_active_win) {
        cur_active_win->close();
    }

    changeState(STATE_NONE);
    cur_active_win = nullptr;
    event->accept();
}

void AutoAnalyserWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        this->close();
    }
}

void AutoAnalyserWindow::changeState(AutoAnaState state)
{
    if (mode != state)
    {
        mode = state;
        row_sel = -1;
        col_sel = -1;
        if (mode == STATE_AE) {
            check_state_label->setText(tr("Current Checking:AE"));
            img_label->setEnGridMode(STATE_AE);
            img_label->setHasFvTable(false);
        } else if (mode == STATE_AF) {
            check_state_label->setText(tr("Current Checking:AF"));
            img_label->setEnGridMode(STATE_AF);
            img_label->setHasFvTable(true);
        } else if (mode == STATE_AWB) {
            check_state_label->setText(tr("Current Checking:AWB"));
            img_label->setEnGridMode(STATE_AWB);
            img_label->setHasFvTable(false);
        } else {
            check_state_label->setText(tr("Current Checking:None"));
            img_label->setEnGridMode(STATE_NONE);
            img_label->setHasFvTable(false);
        }

        img_label->repaint();
    }
}

void AutoAnalyserWindow::slotUpdateImage(QString cmd)
{
    qDebug("Receive 3A image");
    QByteArray data = NetworkClientHelper::getInstance()->popCmdData(cmd);

    QByteArray header = data.left(sizeof(YUV_HEADER));
    YUV_HEADER *pstHeader = (YUV_HEADER *) header.data();
    data.remove(0, sizeof(YUV_HEADER));

    qDebug("numframe:%d, curframe:%d, width:%d, height:%d, stride:%d,%d,%d, format::%d\n", pstHeader->numFrame, pstHeader->curFrame,
        pstHeader->width, pstHeader->height, pstHeader->stride[0], pstHeader->stride[1], pstHeader->stride[2], pstHeader->pixelFormat);
    qDebug("data.size:%d, pstHeader->size:%d\n", data.size(), pstHeader->size);

    if (data.size() != (int)(pstHeader->size)) {
        QMessageBox::warning(this, tr("Error"), tr("3A Analyser, The received yuv data size is wrong!"), QMessageBox::Ok);
        return;
    }

    if (!data.isEmpty()) {
        if (img_label)
        {
            image = utils::convertYUV2RGBImage(data, pstHeader);
            cropImage(image, crop_image);
            img_label->setPixmap(crop_image.scaled(1280, 720, Qt::KeepAspectRatio));
            img_label->adjustSize();
            img_label->setScaledContents(true);
            setImgSourceType(IMG_TYPE::IMG_YUV);
        }
    }
    Get3AStatisticsInfo();
}

inline void AutoAnalyserWindow::cropImage(QImage &source, QPixmap &destination)
{
    int x = 0, y = 0, w = 0, h = 0;
    if (mode == STATE_AWB) {
        // yuv or raw size??
        // DIS on, yuv != raw
        if (wb_crop_state) {
            x = wb_win_crop[2];
            y = wb_win_crop[3];
            w = wb_win_crop[4];
            h = wb_win_crop[5];
            if (w == 0 || h == 0) {
                w = source.width();
                h = source.height()/64*64;
                x = 0;
                y = 0;
            }
        } else {
            w = source.width();
            h = source.height()/64*64;
            x = 0;
            y = 0;
        }
        destination = QPixmap::fromImage(source.copy(x, y, w, h));
    } else if (mode == STATE_AE) {
        if (ae_win_crop[0]) { //crop enable
            x = ae_win_crop[1];//u16X
            y = ae_win_crop[2];//u16Y
            w = ae_win_crop[3];//u16W
            h = ae_win_crop[4];//u16H
            w = (((w / GRID_AE_COL) >> 1) << 1) * GRID_AE_COL;
            h = (((h / GRID_AE_ROW) >> 1) << 1) * GRID_AE_ROW;
        } else {
            w = source.width();
            h = source.height();
            w = (((w / GRID_AE_COL) >> 1) << 1) * GRID_AE_COL;
            h = (((h / GRID_AE_ROW) >> 1) << 1) * GRID_AE_ROW;
            x = ((source.width() - w) >> 2 ) << 1;
            y = ((source.height() - h) >> 2 ) << 1;
        }

        destination = QPixmap::fromImage(source.copy(x, y, w, h));
    } else {
        destination = QPixmap::fromImage(source);
    }
}

void AutoAnalyserWindow::Get3AStatisticsInfo()
{
    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        status_label->setText(tr("communication is not ready."));
        return;
    }
    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_3A_GET_STATISTICS_INFO;
    int id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_3A_STATISTICS_INFO;
    send_obj[MSG_KEY_ID] = id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void AutoAnalyserWindow::slotUpdateData(int id)
{
    qDebug("Receive 3A msg");
    QJsonObject obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();

    if (result_obj.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), "Get 3a statistics info failed!", QMessageBox::Ok);
        return;
    }

    QString soc = GlobalData::getInstance()->getCvipqToolSoc();
    if (CV183X_SOC == soc){
        awbGridRow = GRID_AWB_ROW;
        awbGridCol = GRID_AWB_COL;
    } else {
        awbGridRow = awbGridCol = GRID_AWB_ROW;
    }

    QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();
    saveStatisticInfo(params_obj);

    QVector<QVector<int>> fv_table;
    QJsonArray fv_array, wb_table_r[2], wb_table_g[2], wb_table_b[2], wb_table_count[2],
        curve_r, curve_b, curve_b_top, curve_b_bot, a_wb_win_crop, a_wb_curve_region, a_wb_calib_pt;

    fv_array = all_data[KEY_ATTR_AF_ZONE_V1].toArray();
    wb_global_r[0] = all_data[KEY_ATTR_LE_WB_GLOBAL_R].toInt();
    wb_global_g[0] = all_data[KEY_ATTR_LE_WB_GLOBAL_G].toInt();
    wb_global_b[0] = all_data[KEY_ATTR_LE_WB_GLOBAL_B].toInt();
    wb_global_count[0] = all_data[KEY_ATTR_LE_WB_GLOBAL_ALL].toInt();
    wb_table_r[0] = all_data[KEY_ATTR_LE_WB_ZONE_R].toArray();
    wb_table_g[0] = all_data[KEY_ATTR_LE_WB_ZONE_G].toArray();
    wb_table_b[0] = all_data[KEY_ATTR_LE_WB_ZONE_B].toArray();
    wb_table_count[0] = all_data[KEY_ATTR_LE_WB_ZONE_ALL].toArray();
    wb_global_r[1] = all_data[KEY_ATTR_SE_WB_GLOBAL_R].toInt();
    wb_global_g[1] = all_data[KEY_ATTR_SE_WB_GLOBAL_G].toInt();
    wb_global_b[1] = all_data[KEY_ATTR_SE_WB_GLOBAL_B].toInt();
    wb_global_count[1] = all_data[KEY_ATTR_SE_WB_GLOBAL_ALL].toInt();
    wb_table_r[1] = all_data[KEY_ATTR_SE_WB_ZONE_R].toArray();
    wb_table_g[1] = all_data[KEY_ATTR_SE_WB_ZONE_G].toArray();
    wb_table_b[1] = all_data[KEY_ATTR_SE_WB_ZONE_B].toArray();
    wb_table_count[1] = all_data[KEY_ATTR_SE_WB_ZONE_ALL].toArray();
    curve_r = all_data[KEY_AWB_CURVE_R].toArray();
    curve_b = all_data[KEY_AWB_CURVE_B].toArray();
    curve_b_top = all_data[KEY_AWB_CURVE_B_TOP].toArray();
    curve_b_bot = all_data[KEY_AWB_CURVE_B_BOT].toArray();
    blance_r = all_data[KEY_AWB_BALANCE_R].toInt();
    blance_b = all_data[KEY_AWB_BALANCE_B].toInt();
    curve_left_limit = all_data[KEY_AWB_CURVE_LEFT_LIMIT].toInt();
    curve_right_limit = all_data[KEY_AWB_CURVE_RIGHT_LIMIT].toInt();
    extra_light_enabled = all_data[KEY_AWB_EXTRA_LIGHT_ENABLED].toInt();
    lightInfo_white_rgain[0] = all_data[KEY_AWB_LIGHTINFO0_WHITE_RGAIN].toInt();
    lightInfo_white_bgain[0] = all_data[KEY_AWB_LIGHTINFO0_WHITE_BGAIN].toInt();
    lightInfo_light_status[0] = all_data[KEY_AWB_LIGHTINFO0_LIGHT_STATUS].toInt();
    lightInfo_radius[0] = all_data[KEY_AWB_LIGHTINFO0_RADIUS].toInt();
    lightInfo_white_rgain[1] = all_data[KEY_AWB_LIGHTINFO1_WHITE_RGAIN].toInt();
    lightInfo_white_bgain[1] = all_data[KEY_AWB_LIGHTINFO1_WHITE_BGAIN].toInt();
    lightInfo_light_status[1] = all_data[KEY_AWB_LIGHTINFO1_LIGHT_STATUS].toInt();
    lightInfo_radius[1] = all_data[KEY_AWB_LIGHTINFO1_RADIUS].toInt();
    lightInfo_white_rgain[2] = all_data[KEY_AWB_LIGHTINFO2_WHITE_RGAIN].toInt();
    lightInfo_white_bgain[2] = all_data[KEY_AWB_LIGHTINFO2_WHITE_BGAIN].toInt();
    lightInfo_light_status[2] = all_data[KEY_AWB_LIGHTINFO2_LIGHT_STATUS].toInt();
    lightInfo_radius[2] = all_data[KEY_AWB_LIGHTINFO2_RADIUS].toInt();
    lightInfo_white_rgain[3] = all_data[KEY_AWB_LIGHTINFO3_WHITE_RGAIN].toInt();
    lightInfo_white_bgain[3] = all_data[KEY_AWB_LIGHTINFO3_WHITE_BGAIN].toInt();
    lightInfo_light_status[3] = all_data[KEY_AWB_LIGHTINFO3_LIGHT_STATUS].toInt();
    lightInfo_radius[3] = all_data[KEY_AWB_LIGHTINFO3_RADIUS].toInt();
    calibration_status = all_data[KEY_AWB_CALIBRATION_STATUS].toInt();
    a_wb_win_crop = all_data[KEY_AWB_CROP_INFO].toArray();
    a_wb_curve_region = all_data[KEY_AWB_CURVE_REGION].toArray();
    a_wb_calib_pt = all_data[KEY_AWB_CALIB_INFO].toArray();
    wb_crop_state = all_data[KEY_AWB_CROP_STATUS].toInt();
    wdr_mode = all_data[KEY_ATTR_IS_WDR_MODE].toInt();

    if (!wdr_mode) {
        wb_global_count[1] = 0;
    }

    for (int i=0; i < a_wb_win_crop.size(); i++) {
        wb_win_crop[i] = a_wb_win_crop[i].toInt();
    }
    for (int i=0; i < a_wb_curve_region.size(); i++) {
        wb_curve_region[i] = a_wb_curve_region[i].toInt();
    }
    for (int i=0; i < a_wb_calib_pt.size()/3; i++) {
        wb_calib_pt[i][0] = a_wb_calib_pt[i*3 + 0].toInt();
        wb_calib_pt[i][1] = a_wb_calib_pt[i*3 + 1].toInt();
        wb_calib_pt[i][2] = a_wb_calib_pt[i*3 + 2].toInt();
    }
    if (wb_win_crop[0] && wb_win_crop[1]) {
        awbGridRow = wb_win_crop[1];
        awbGridCol = wb_win_crop[0];
        img_label->awb_win_num_x = wb_win_crop[0];
        img_label->awb_win_num_y = wb_win_crop[1];
        awb_window->h_wnd = img_label->awb_win_num_x;
        awb_window->v_wnd = img_label->awb_win_num_y;
    }

    if (!fv_array.isEmpty()) {
        for (int i=0; i < fv_array.size(); i++) {
            QJsonArray row = fv_array[i].toArray();
            QVector<int> row_vec;
            for (int j = 0; j < row.size(); j++) {
                row_vec.append(row[j].toInt());
            }
            fv_table.append(row_vec);
        }
    }

    img_label->setFvTable(fv_table);

    // set awb tables from json array
    if (!wb_table_r[0].isEmpty() && !wb_table_g[0].isEmpty() && !wb_table_b[0].isEmpty() && !wb_table_count[0].isEmpty() &&
        !curve_r.isEmpty() && !curve_b.isEmpty() && !curve_b_top.isEmpty() && !curve_b_bot.isEmpty()) {
        for (int i = 0; i < awbGridRow; i++) {
            for (int j = 0; j < awbGridCol; j++) {
                wb_zone_r_table[0][i][j] = wb_table_r[0][i*awbGridCol+j].toInt();
                wb_zone_g_table[0][i][j] = wb_table_g[0][i*awbGridCol+j].toInt();
                wb_zone_b_table[0][i][j] = wb_table_b[0][i*awbGridCol+j].toInt();
                wb_zone_count_table[0][i][j] = wb_table_count[0][i*awbGridCol+j].toInt();
                table_rg[0][i*awbGridCol+j] = (float) wb_zone_r_table[0][i][j] / (float) wb_zone_g_table[0][i][j];
                table_bg[0][i*awbGridCol+j] = (float) wb_zone_b_table[0][i][j] / (float) wb_zone_g_table[0][i][j];

                wb_zone_r_table[1][i][j] = wb_table_r[1][i*awbGridCol+j].toInt();
                wb_zone_g_table[1][i][j] = wb_table_g[1][i*awbGridCol+j].toInt();
                wb_zone_b_table[1][i][j] = wb_table_b[1][i*awbGridCol+j].toInt();
                wb_zone_count_table[1][i][j] = wb_table_count[1][i*awbGridCol+j].toInt();
                if (wdr_mode) {
                    table_rg[1][i*awbGridCol+j] = (float) wb_zone_r_table[1][i][j] / (float) wb_zone_g_table[1][i][j];
                    table_bg[1][i*awbGridCol+j] = (float) wb_zone_b_table[1][i][j] / (float) wb_zone_g_table[1][i][j];
                } else {
                    table_rg[1][i*awbGridCol+j] = 0;
                    table_bg[1][i*awbGridCol+j] = 0;
                }
            }
        }

        num_white_zone_mid = 0;
        num_white_zone_top = 0;
        num_white_zone_bot = 0;
        for (int i = 0; i < 256; i++) {
            if ((curve_r[i].toInt() > 0) && curve_b[i].toInt() > 0) {
                white_zone_mid_x[num_white_zone_mid] = AWB_GAIN_BASE / curve_r[i].toDouble();
                white_zone_mid_y[num_white_zone_mid] = AWB_GAIN_BASE / curve_b[i].toDouble();
                num_white_zone_mid++;
            }

            if (curve_r[i].toInt() > 0 && curve_b_top[i].toInt() > 0) {
                white_zone_top_x[num_white_zone_top] = AWB_GAIN_BASE / curve_r[i].toDouble();
                white_zone_top_y[num_white_zone_top] = AWB_GAIN_BASE / curve_b_top[i].toDouble();
                num_white_zone_top++;
            }

            if (curve_r[i].toInt() > 0 && curve_b_bot[i].toInt() > 0) {
                white_zone_bot_x[num_white_zone_bot] = AWB_GAIN_BASE / curve_r[i].toDouble();
                white_zone_bot_y[num_white_zone_bot] = AWB_GAIN_BASE / curve_b_bot[i].toDouble();
                num_white_zone_bot++;
            }
        }

        float degrees = 0, radians = 0;
        for (int i = 0; i < 4; i++) {
            if ((lightInfo_white_rgain[i] >= 512) && (lightInfo_white_bgain[i] >= 512)) {
                degrees = 0;
                 for(int j = 0; j < 25; j++) {
                    radians = qDegreesToRadians(degrees);
                    light_souce_x[i][j] = (float) AWB_GAIN_BASE / lightInfo_white_rgain[i] + (float) lightInfo_radius[i] / AWB_GAIN_SMALL_BASE * qSin(radians);
                    light_souce_y[i][j] = (float) AWB_GAIN_BASE / lightInfo_white_bgain[i] + (float) lightInfo_radius[i] / AWB_GAIN_SMALL_BASE * qCos(radians);
                    degrees += 15;
                }
            } else {
                for(int j = 0; j < 25; j++) {
                    light_souce_x[i][j] = 2.0;
                    light_souce_y[i][j] = 2.0;
                }
            }
        }
    } else {
        qDebug("Received awb statistics data is wrong");
        return;
    }
    GetAWBBin();
}

void AutoAnalyserWindow::GetAWBBin()
{
    qDebug("AutoAnalyserWindow::GetAWBBin\n");

    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        status_label->setText(tr("communication is not ready."));
        return;
    }

    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_3A_GET_AWB_BIN;
    int id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

    QJsonObject param_obj;
    param_obj["content"] = AWB_BIN_DATA;
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_PREPARE_BIN;
    send_obj[MSG_KEY_ID] = id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_PARAMS] = param_obj;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));

}

void AutoAnalyserWindow::HandleAWBBinStatus(int id)
{
    qDebug("AutoAnalyserWindow::HandleAWBBinStatus\n");

    QJsonObject obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();
    if (result_obj.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), "3A Analyser, Get awb bin failed!", QMessageBox::Ok);
        return;
    }
    QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();
    int size = params_obj["Data size"].toInt();
    NetworkClientHelper::getInstance()->setCallerInfo(size, CMD_3A_GET_AWB_BIN);
    qDebug("Set receive size:%d\n", size);

    int temp_id = GlobalData::getInstance()->getJsonrpcId();
    QJsonObject param_obj;
    param_obj["content"] = AWB_BIN_DATA;
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_BIN_DATA;
    send_obj[MSG_KEY_ID] = temp_id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_PARAMS] = param_obj;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void AutoAnalyserWindow::HandleAWBBinData(QString cmd)
{
    qDebug("AutoAnalyserWindow::HandleAWBBinData\n");

    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);
    QByteArray data = NetworkClientHelper::getInstance()->popCmdData(cmd);

    QFile bin_file(QCoreApplication::applicationDirPath() + PATH_SEPARATOR + "awb.bin");
    bin_file.open(QIODevice::WriteOnly);
    bin_file.write(data);
    bin_file.close();

    s_AWB_DBG_S *ptmpAWB_Dbg   =(s_AWB_DBG_S *)data.data();
    memcpy(&sAwb_dbg, ptmpAWB_Dbg, sizeof(s_AWB_DBG_S));

    updated = true;
    updateStatisticInfo(row_sel, col_sel);
    get_raw_finish = true;
}

void AutoAnalyserWindow::handleCaptureRawImg()
{
    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);

    receive_raw_data.clear();
    wdr_raw_data_list.clear();

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_CAPTURE_GET_SINGLE_RAW;
    int id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

    QJsonObject param_obj;
    param_obj["frames"] = 1;
    param_obj["tightly"] = false;
    param_obj["dump_reg"] = false;

    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_MULTI_RAW;
    send_obj[MSG_KEY_ID] = id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_PARAMS] = param_obj;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void AutoAnalyserWindow::handleSingleRawStatus(int id)
{
    qDebug("AutoAnalyserWindow::handleSingleRawStatus\n");

    QJsonObject obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();
    if (result_obj.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("3A get single raw failed!"), QMessageBox::Ok);
        return;
    }

    QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();
    int raw_size = params_obj["Data size"].toInt();
    if (raw_size == 0) {
        QMessageBox::warning(this, tr("Error"), tr("3A get single raw failed!"), QMessageBox::Ok);
        return;
    }
    NetworkClientHelper::getInstance()->setCallerInfo(raw_size, CMD_CAPTURE_GET_SINGLE_RAW);

    int temp_id = GlobalData::getInstance()->getJsonrpcId();
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_IMAGE_DATA;
    send_obj[MSG_KEY_ID] = temp_id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void AutoAnalyserWindow::slotUpdateRawData(QString cmd)
{
    qDebug("AutoAnalyserWindow::slotUpdateRawData\n");
    QByteArray data = NetworkClientHelper::getInstance()->popCmdData(cmd);
    int step = 0;
    int received_frames = 0;
    bool remain = false;

    QByteArray header = data.left(sizeof(RAW_HEADER));
    RAW_HEADER *pstHeader = (RAW_HEADER *) header.data();
    data.remove(0, sizeof(RAW_HEADER));

    qDebug("numframe:%d, curframe:%d, width:%d, height:%d,  compress:%d",
           pstHeader->numFrame, pstHeader->curFrame, pstHeader->width, pstHeader->height, pstHeader->compress);
    qDebug("fusionFrame:%d, bayerid:%d, cropX:%d, cropY:%d, cropWidth:%d, cropHeight:%d", pstHeader->fusionFrame,
           pstHeader->bayerID, pstHeader->cropX, pstHeader->cropY, pstHeader->cropWidth, pstHeader->cropHeight);
    qDebug("data.size:%d, pstHeader->size:%d", data.size(), pstHeader->size);

    if (data.size() != (int)pstHeader->size) {
        QMessageBox::warning(this, tr("Error"), tr("Analyser Tool, The received raw data size is wrong!"), QMessageBox::Ok);
        return;
    }

    //crop & unpack
    RAW_INFO rawInfo;
    rawInfo.width = pstHeader->width;
    int tileIdx = 0;
    bool tileMode = utils::isTileMode(rawInfo, RAW_NORMAL, &tileIdx);
    QByteArray d = utils::cropUnpackRaw(data, *pstHeader, tileMode, tileIdx);
    if (d.isEmpty()) {
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

    int raw_size;
    if (remain) {
        //call for remain frames
        NetworkClientHelper::getInstance()->setCallerInfo(raw_size, CMD_CAPTURE_GET_SINGLE_RAW);

        int temp_id = GlobalData::getInstance()->getJsonrpcId();
        QJsonObject send_obj;
        send_obj[MSG_KEY_METHOD] = METHOD_GET_IMAGE_DATA;
        send_obj[MSG_KEY_ID] = temp_id;
        send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
    } else {
        if (step == 1) {
            raw_size = pstHeader->cropWidth * pstHeader->cropHeight * sizeof(short);
            QByteArray frame(receive_raw_data.data(), raw_size);

            image = utils::convertRaw2RGBImage(frame, pstHeader->cropWidth, pstHeader->cropHeight, (utils::RAW_BAYER_ID) pstHeader->bayerID, utils::RAW_BITS::BITS_12);
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
            raw_size = pstHeader->cropWidth * 2 * pstHeader->cropHeight * sizeof(short);
            QByteArray frame(wdr_data.data(), raw_size);
            image = utils::convertRaw2RGBImage(frame, pstHeader->cropWidth * 2, pstHeader->cropHeight, (utils::RAW_BAYER_ID)pstHeader->bayerID, utils::RAW_BITS::BITS_12);
            wdr_raw_data_list.clear();
            delete [] line;
        }
        cropImage(image, crop_image);
        img_label->setPixmap(crop_image.scaled(1280, 720, Qt::KeepAspectRatio));
        img_label->adjustSize();
        img_label->setScaledContents(true);
        setImgSourceType(IMG_TYPE::IMG_RAW);
        Get3AStatisticsInfo();
    }
}

void AutoAnalyserWindow::setImgSourceType(IMG_TYPE img_type)
{
    switch(img_type) {
        case IMG_TYPE::IMG_YUV:
            img_source_label->setText(tr("image source: yuv"));
            break;

        case IMG_TYPE::IMG_RAW:
            img_source_label->setText(tr("image source: raw"));
            break;
    }
}

void AutoAnalyserWindow::sendCaptureRawCmd()
{
    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        status_label->setText(tr("communication is not ready."));
        get_raw_finish = true;
        timeout_times = 0;
        return;
    }

    if (get_raw_finish) {
        get_raw_finish = false;
        handleCaptureRawImg();
    } else {
        timeout_times++;
        if (timeout_times > 3) {
            timeout_times = 0;
            get_raw_finish = true;
        }
    }
}

void AutoAnalyserWindow::slotSendCapCmd()
{
    if (!use_yuv_show) {
        sendCaptureRawCmd();
        return;
    }
    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_3A_GET_SINGLE_YUV;
    int id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_SINGLE_YUV;
    send_obj[MSG_KEY_ID] = id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void AutoAnalyserWindow::handleImageStatus(int id)
{
    QJsonObject obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();

    if (result_obj.isEmpty()) {
        use_yuv_show = false;
        return;
    }
    QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();
    int size = params_obj["Data size"].toInt();
    NetworkClientHelper::getInstance()->setCallerInfo(size, CMD_3A_GET_SINGLE_YUV);
    qDebug("Set receive size:%d\n", size);

    int temp_id = GlobalData::getInstance()->getJsonrpcId();
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_IMAGE_DATA;
    send_obj[MSG_KEY_ID] = temp_id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void AutoAnalyserWindow::slotCheckAEInfo()
{
    if (cur_active_win) {
        cur_active_win->close();
    }

    changeState(STATE_AE);
    ae_window->show();
    ae_window->activateWindow();
    cur_active_win = ae_window;
    cropImage(image, crop_image);
    img_label->setPixmap(crop_image.scaled(1280, 720, Qt::KeepAspectRatio));
}

void AutoAnalyserWindow::slotCheckAFInfo()
{

    if (cur_active_win) {
        cur_active_win->close();
    }

    changeState(STATE_AF);
    af_window->show();
    af_window->activateWindow();
    cur_active_win = af_window;
    cropImage(image, crop_image);
    img_label->setPixmap(crop_image.scaled(1280, 720, Qt::KeepAspectRatio));
}

void AutoAnalyserWindow::slotCheckAWBInfo()
{

    if (cur_active_win) {
        cur_active_win->close();
    }

    changeState(STATE_AWB);
    awb_window->show();
    awb_window->activateWindow();
    cur_active_win = awb_window;
    cropImage(image, crop_image);
    img_label->setPixmap(crop_image.scaled(1280, 720, Qt::KeepAspectRatio));
}

void AutoAnalyserWindow::slotCloseInfoWindow()
{
    changeState(STATE_NONE);
    cur_active_win = nullptr;
}

void AutoAnalyserWindow::slotUpdateInfoOnSel(int row, int col)
{
    row_sel = row;
    col_sel = col;
    updateStatisticInfo(row, col); //maybe race, to be checked
}

void AutoAnalyserWindow::updateStatisticInfo(int row, int col)
{
    if (row != -1 && col != -1)
    {
        switch (mode) {
        case AutoAnaState::STATE_AE:
            setAEStatInfo();
            break;
        case AutoAnaState::STATE_AF:
            setAFStatInfo();
            break;
        case AutoAnaState::STATE_AWB:
            setAWBStatInfo();
            break;
        case AutoAnaState::STATE_NONE:
        default:
            break;
        }
    }
}

void AutoAnalyserWindow::saveStatisticInfo(QJsonObject &items)
{
    all_data = items;
}

void AutoAnalyserWindow::setAEStatInfo()
{
    QMap <QString, int> ae_stats;
    QMap <QString, QVariantList> ae_hist;
    QJsonArray arrays,row;

    ae_stats[KEY_ATTR_FE0_GLOBAL_R] = all_data[KEY_ATTR_FE0_GLOBAL_R].toInt();
    ae_stats[KEY_ATTR_FE0_GLOBAL_GR] = all_data[KEY_ATTR_FE0_GLOBAL_GR].toInt();
    ae_stats[KEY_ATTR_FE0_GLOBAL_GB] = all_data[KEY_ATTR_FE0_GLOBAL_GB].toInt();
    ae_stats[KEY_ATTR_FE0_GLOBAL_B] = all_data[KEY_ATTR_FE0_GLOBAL_B].toInt();
    ae_stats[KEY_ATTR_FE1_GLOBAL_R] = all_data[KEY_ATTR_FE1_GLOBAL_R].toInt();
    ae_stats[KEY_ATTR_FE1_GLOBAL_GR] = all_data[KEY_ATTR_FE1_GLOBAL_GR].toInt();
    ae_stats[KEY_ATTR_FE1_GLOBAL_GB] = all_data[KEY_ATTR_FE1_GLOBAL_GB].toInt();
    ae_stats[KEY_ATTR_FE1_GLOBAL_B] = all_data[KEY_ATTR_FE1_GLOBAL_B].toInt();

    arrays = all_data[KEY_ATTR_FE0_ZONE_R].toArray();
    row = arrays[row_sel].toArray();
    ae_stats[KEY_ATTR_FE0_ZONE_R] = row[col_sel].toInt();
    arrays = all_data[KEY_ATTR_FE0_ZONE_GR].toArray();
    row = arrays[row_sel].toArray();
    ae_stats[KEY_ATTR_FE0_ZONE_GR] = row[col_sel].toInt();
    arrays = all_data[KEY_ATTR_FE0_ZONE_GB].toArray();
    row = arrays[row_sel].toArray();
    ae_stats[KEY_ATTR_FE0_ZONE_GB] = row[col_sel].toInt();
    arrays = all_data[KEY_ATTR_FE0_ZONE_B].toArray();
    row = arrays[row_sel].toArray();
    ae_stats[KEY_ATTR_FE0_ZONE_B] = row[col_sel].toInt();
    arrays = all_data[KEY_ATTR_FE1_ZONE_R].toArray();
    row = arrays[row_sel].toArray();
    ae_stats[KEY_ATTR_FE1_ZONE_R] = row[col_sel].toInt();
    arrays = all_data[KEY_ATTR_FE1_ZONE_GR].toArray();
    row = arrays[row_sel].toArray();
    ae_stats[KEY_ATTR_FE1_ZONE_GR] = row[col_sel].toInt();
    arrays = all_data[KEY_ATTR_FE1_ZONE_GB].toArray();
    row = arrays[row_sel].toArray();
    ae_stats[KEY_ATTR_FE1_ZONE_GB] = row[col_sel].toInt();
    arrays = all_data[KEY_ATTR_FE1_ZONE_B].toArray();
    row = arrays[row_sel].toArray();
    ae_stats[KEY_ATTR_FE1_ZONE_B] = row[col_sel].toInt();

    arrays = all_data[KEY_ATTR_FE0_HIST].toArray();
    ae_hist[KEY_ATTR_FE0_HIST] = arrays.toVariantList();
    arrays = all_data[KEY_ATTR_FE1_HIST].toArray();
    ae_hist[KEY_ATTR_FE1_HIST] = arrays.toVariantList();

    arrays = all_data[KEY_ATTR_AE_WIN_CROP].toArray();
    for (int i=0; i < arrays.size(); i++) {
        ae_win_crop[i] = arrays[i].toInt();
    }

    ae_window->updateStatistic(ae_stats);
    ae_window->updateHist(ae_hist);
}

void AutoAnalyserWindow::setAFStatInfo()
{
    QMap <QString, int> af_stats;
    QJsonArray arrays;

    arrays = all_data[KEY_ATTR_AF_ZONE_V1].toArray();
    af_stats[KEY_ATTR_AF_ZONE_V1] = arrays[row_sel].toArray()[col_sel].toInt();
    arrays = all_data[KEY_ATTR_AF_ZONE_H1].toArray();
    af_stats[KEY_ATTR_AF_ZONE_H1] = arrays[row_sel].toArray()[col_sel].toInt();
    arrays = all_data[KEY_ATTR_AF_ZONE_H2].toArray();
    af_stats[KEY_ATTR_AF_ZONE_H2] = arrays[row_sel].toArray()[col_sel].toInt();
    arrays = all_data[KEY_ATTR_AF_ZONE_HLCNT].toArray();
    af_stats[KEY_ATTR_AF_ZONE_HLCNT] = arrays[row_sel].toArray()[col_sel].toInt();

    af_window->updateStatistic(af_stats);
}

void AutoAnalyserWindow::setAWBStatInfo()
{
    QMap <QString, int> awb_stats;

    awb_stats[KEY_ATTR_LE_WB_GLOBAL_R] = wb_global_r[0];
    awb_stats[KEY_ATTR_LE_WB_GLOBAL_G] = wb_global_g[0];
    awb_stats[KEY_ATTR_LE_WB_GLOBAL_B] = wb_global_b[0];
    awb_stats[KEY_ATTR_LE_WB_GLOBAL_ALL] = wb_global_count[0];
    awb_stats[KEY_ATTR_LE_WB_ZONE_R] = wb_zone_r_table[0][row_sel][col_sel];
    awb_stats[KEY_ATTR_LE_WB_ZONE_G] = wb_zone_g_table[0][row_sel][col_sel];
    awb_stats[KEY_ATTR_LE_WB_ZONE_B] = wb_zone_b_table[0][row_sel][col_sel];
    awb_stats[KEY_ATTR_LE_WB_ZONE_ALL] = wb_zone_count_table[0][row_sel][col_sel];
    awb_stats[KEY_ATTR_SE_WB_GLOBAL_R] = wb_global_r[1];
    awb_stats[KEY_ATTR_SE_WB_GLOBAL_G] = wb_global_g[1];
    awb_stats[KEY_ATTR_SE_WB_GLOBAL_B] = wb_global_b[1];
    awb_stats[KEY_ATTR_SE_WB_GLOBAL_ALL] = wb_global_count[1];
    awb_stats[KEY_ATTR_SE_WB_ZONE_R] = wb_zone_r_table[1][row_sel][col_sel];
    awb_stats[KEY_ATTR_SE_WB_ZONE_G] = wb_zone_g_table[1][row_sel][col_sel];
    awb_stats[KEY_ATTR_SE_WB_ZONE_B] = wb_zone_b_table[1][row_sel][col_sel];
    awb_stats[KEY_ATTR_SE_WB_ZONE_ALL] = wb_zone_count_table[1][row_sel][col_sel];
    awb_stats[KEY_ATTR_IS_WDR_MODE] = wdr_mode;
    awb_window->updateStatistic(awb_stats);
    awb_window->updateSelectedPoint(table_rg[0][row_sel*awbGridCol+col_sel], table_bg[0][row_sel*awbGridCol+col_sel],
                                                            table_rg[1][row_sel*awbGridCol+col_sel], table_bg[1][row_sel*awbGridCol+col_sel]);
    if (updated) {
        updated = false;
        awb_window->updateCalibrationStatus(calibration_status);
        awb_window->updateZoneSatPoints(table_rg[0], table_bg[0], table_rg[1], table_bg[1], &sAwb_dbg);
        awb_window->updateGlobalSatPoint(wb_global_r[0] / (float) wb_global_g[0], wb_global_b[0] / (float) wb_global_g[0],
                                                                    wb_global_r[1] / (float) wb_global_g[1], wb_global_b[1] / (float) wb_global_g[1]);
        awb_window->updateResultPoint(AWB_GAIN_BASE / (float) blance_r, AWB_GAIN_BASE / (float) blance_b);
        awb_window->updateWhiteZoneCurvesMid(num_white_zone_mid, white_zone_mid_x, white_zone_mid_y);
        awb_window->updateWhiteZoneCurvesTop(num_white_zone_top, white_zone_top_x, white_zone_top_y);
        awb_window->updateWhiteZoneCurvesBot(num_white_zone_bot, white_zone_bot_x, white_zone_bot_y);
        awb_window->updateCurveLeftLimit(curve_left_limit / 1024.0, curve_left_limit / 1024.0);
        awb_window->updateCurveRightLimit(curve_right_limit / 1024.0, curve_right_limit / 1024.0);
        awb_window->updateExtraLightSource(extra_light_enabled, lightInfo_light_status, light_souce_x, light_souce_y);
        awb_window->updateCurveRegion(wb_curve_region);
        awb_window->updateCalibPoint(wb_calib_pt);
    }
}

void AutoAnalyserWindow::slotChangeFocusItem(int idx)
{
    QString key_sel = fv_key_list[idx];

    QVector<QVector<int>> fv_table;

    QJsonArray fv_array = all_data[key_sel].toArray();
    if (!fv_array.isEmpty()) {
        for (int i=0; i < fv_array.size(); i++) {
            QJsonArray row = fv_array[i].toArray();
            QVector<int> row_vec;
            for (int j = 0; j < row.size(); j++) {
                row_vec.append(row[j].toInt());
            }
            fv_table.append(row_vec);
        }
    }

    img_label->setFvTable(fv_table);
    img_label->repaint();
}

void AutoAnalyserWindow::handleUpdateTheme(bool dark_theme_enabled)
{
    ae_window->handleUpdateTheme(dark_theme_enabled);
    //af_window->handleUpdateTheme(dark_theme_enabled);
    awb_window->handleUpdateTheme(dark_theme_enabled);
}
