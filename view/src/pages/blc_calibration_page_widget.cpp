#include "blc_calibration_page_widget.hpp"
#include "utils.hpp"
#include "otp_pdaf_cali.h"
#include "network_client_helper.hpp"
#include "raw_image_format_dialog.hpp"
#include <QGroupBox>
#include <QMessageBox>

BLCCalibrationPageWidget::BLCCalibrationPageWidget(QWidget *parent) : BaseCalibrationPageWidget(static_cast<int>(CALIBRATION_MODULE_ID::BLC), parent)
{
    setAcceptScene(SCENE::DARK_FRAME);
    initLeftArea();
    initEvent();
    retranslateUi();
}

void BLCCalibrationPageWidget::ClearData()
{
    for (int i = 0; i < rawItem.size(); i++) {
        if (rawItem[i]) {
            delete rawItem[i];
        }
    }
    rawItem.clear();
    calibrated = false;
    caliResultMean.clear();
    caliResultMedian.clear();
    raw_img_item_vec.clear();
    quickMode = false;
    cali_status = CALIBRATION_STATUS::READY;
}

void BLCCalibrationPageWidget::initLeftArea()
{
    initImportRawUI();
    initCaliUI();
    initDisplayDataUI();
    initWriteDataUI();
    initImportDataUI();
    initExportDataUI();
    initQuickCalibrationUI();
    param_layout->setAlignment(Qt::AlignTop);
}

void BLCCalibrationPageWidget::initEvent()
{
    connect(this, &BaseCalibrationPageWidget::sigImportData, this, &BLCCalibrationPageWidget::handleImportData);
    shared_ptr<NetworkClientHelper> network_helper = NetworkClientHelper::getInstance();
    connect(network_helper.get(), &NetworkClientHelper::sigReceiveBlcCalibration,
        this, &BLCCalibrationPageWidget::handleReceiveBlcCalibration);
}

bool BLCCalibrationPageWidget::calibration()
{
    using utils::RAW_BAYER_ID;
    using utils::RAW_BITS;

    if (raw_img_item_vec.isEmpty()) {
        return false;
    }

    RawImageItem * raw_img_item = this->raw_img_item_vec.first();
    int width = raw_img_item->getWidth();
    int height = raw_img_item->getHeight();
    QString raw_path = raw_img_item->getFilePath();
    bool wdr_mode = raw_img_item->getFrameMode() == FRAME_MODE::WDR ? true : false;
    unique_ptr<int []> image = utils::readRawIntData(raw_path, width, height, wdr_mode);
    if (image == nullptr) {
        return false;
    }

    cali_status = CALIBRATION_STATUS::CALIBRATION;

    int blc_rr_hist[BLC_BAYER_MAX_VALUE] = {};
    int blc_gr_hist[BLC_BAYER_MAX_VALUE] = {};
    int blc_gb_hist[BLC_BAYER_MAX_VALUE] = {};
    int blc_bb_hist[BLC_BAYER_MAX_VALUE] = {};
    BLC_CALI_RESULT mean_result;
    BLC_CALI_RESULT median_result;
    QString soc = GlobalData::getInstance()->getCvipqToolSoc();
    int order = raw_img_item->getBayer().toInt();

    int ir_position = raw_img_item->getIrPosition().toInt();
    order = ir_position * 4 + order;

    if (!quickMode) {
        calibrated = false;
        caliResultMean.clear();
        caliResultMedian.clear();
        int iso = GetIsoFromFileName(raw_img_item->getFileName());
        raw_img_item->setISO(iso);
    }

    // calibration
    int *raw_image = image.get();
    if (wdr_mode) width /= 2;

    if (soc == CV183X_SOC) {
        cali_blc_calc_by_mean(&raw_image, width, height, order, &mean_result.offset_r, &mean_result.offset_gr, &mean_result.offset_gb,
            &mean_result.offset_b, &mean_result.gain_r, &mean_result.gain_gr, &mean_result.gain_gb, &mean_result.gain_b, CV183X);
        cali_blc_calc_by_median(&raw_image, width, height, order, &median_result.offset_r, &median_result.offset_gr, &median_result.offset_gb,
            &median_result.offset_b, &median_result.gain_r, &median_result.gain_gr, &median_result.gain_gb, &median_result.gain_b, CV183X);
    } else {
        cali_blc_calc_by_mean(&raw_image, width, height, order, &mean_result.offset_r, &mean_result.offset_gr, &mean_result.offset_gb,
            &mean_result.offset_b, &mean_result.gain_r, &mean_result.gain_gr, &mean_result.gain_gb, &mean_result.gain_b, CV182X);
        cali_blc_calc_by_median(&raw_image, width, height, order, &median_result.offset_r, &median_result.offset_gr, &median_result.offset_gb,
            &median_result.offset_b, &median_result.gain_r, &median_result.gain_gr, &median_result.gain_gb, &median_result.gain_b, CV182X);
    }

    caliResultMean.insert(raw_img_item->getISO(), mean_result);
    caliResultMedian.insert(raw_img_item->getISO(), median_result);

    if (!quickMode) {
        if (soc == CV183X_SOC) {
            cali_blc_get_histogram(&raw_image, width, height, order, blc_rr_hist, blc_gr_hist, blc_gb_hist, blc_bb_hist, CV183X);
        } else {
            cali_blc_get_histogram(&raw_image, width, height, order, blc_rr_hist, blc_gr_hist, blc_gb_hist, blc_bb_hist, CV182X);
        }
        blc_histogram->handleUpdatePoints(blc_rr_hist, blc_gr_hist, blc_gb_hist, blc_bb_hist, mean_result.offset_r, mean_result.offset_gr, mean_result.offset_gb,
                mean_result.offset_b, median_result.offset_r, median_result.offset_gr, median_result.offset_gb, median_result.offset_b);


        int *raw_blc_image_mean = new int [width * height];
        int *raw_blc_image_median = new int [width * height];
        cali_blc_verify(&raw_image, width, height, order, mean_result.offset_r, mean_result.offset_gr, mean_result.offset_gb,
                        mean_result.offset_b, mean_result.gain_r, mean_result.gain_gr, mean_result.gain_gb, mean_result.gain_b, &raw_blc_image_mean);
        cali_blc_verify(&raw_image, width, height, order, median_result.offset_r, median_result.offset_gr, median_result.offset_gb,
                        median_result.offset_b, median_result.gain_r, median_result.gain_gr, median_result.gain_gb, median_result.gain_b, &raw_blc_image_median);
        // display result img
        RAW_BAYER_ID bayer_id = static_cast<RAW_BAYER_ID>(raw_img_item->getBayer().toInt());
        RAW_BITS bits = static_cast<RAW_BITS>(raw_img_item->getBits().toInt());
        unsigned char *result_img_rbg_median = utils::rawIntData2RGBData(raw_blc_image_median, width, height, bayer_id, bits);
        QImage result_img_median(result_img_rbg_median, width, height, QImage::Format_RGB32);
        emit sigShowOutputRawImage(result_img_median);

        unsigned char *result_img_rbg_mean = utils::rawIntData2RGBData(raw_blc_image_mean, width, height, bayer_id, bits);
        QImage result_img_mean(result_img_rbg_mean, width, height, QImage::Format_RGB32);
        emit sigShowOutputBlcImage(result_img_mean);

        display_tab_widget->setCurrentWidget(blc_output_img_median);

        delete [] result_img_rbg_mean;
        delete [] result_img_rbg_median;
        delete[] raw_blc_image_mean;
        delete[] raw_blc_image_median;
    }

    calibrated = true;
    cali_status = CALIBRATION_STATUS::READY;

    return true;
}

QJsonArray BLCCalibrationPageWidget::getCalibrationData()
{
    QJsonArray data_array;
    QJsonObject data;
    BLC_CALI_RESULT temp = {};
    int idx = optype_combox->currentIndex();

    data[KEY_REG_KEY] = "ISO";
    data[KEY_VALUE] = iso_list[idx];
    data[KEY_VALUE_TYPE] = TYPE_INT;
    data[KEY_REG_OP_TYPE] = OP_TYPE_AUTO;
    data_array.append(data);

    if (method_combox->currentText() == "Mean") {
        if (quickMode) {
            if (caliResultMean.contains(iso_list[idx])) {
                temp = caliResultMean.value(iso_list[idx]);
            }
        } else {
            temp = caliResultMean.first();
        }
    } else {
        if (quickMode) {
            if (caliResultMedian.contains(iso_list[idx])) {
                temp = caliResultMedian.value(iso_list[idx]);
            }
        } else {
            temp = caliResultMedian.first();
        }
    }

    QString soc = GlobalData::getInstance()->getCvipqToolSoc();
    if (CV183X_SOC == soc) {
        /* Workaround, CY need to update dll */
        if (temp.offset_r > 1023)
            temp.offset_r = 1023;
        if (temp.offset_gr > 1023)
            temp.offset_gr = 1023;
        if (temp.offset_gb > 1023)
            temp.offset_gb = 1023;
        if (temp.offset_b > 1023)
            temp.offset_b = 1023;
        temp.gain_r = 4095 / (float)(4095 - temp.offset_r);
        temp.gain_gr = 4095 / (float)(4095 - temp.offset_gr);
        temp.gain_gb = 4095 / (float)(4095 - temp.offset_gb);
        temp.gain_b = 4095 / (float)(4095 - temp.offset_b);
        /* Workaround, CY need to update dll */
    }

    data[KEY_REG_KEY] = "blc_r_offset";
    data[KEY_VALUE] = temp.offset_r;
    data[KEY_VALUE_TYPE] = TYPE_INT;
    data[KEY_REG_OP_TYPE] = OP_TYPE_AUTO;
    data_array.append(data);

    data[KEY_REG_KEY] = "blc_gr_offset";
    data[KEY_VALUE] = temp.offset_gr;
    data[KEY_VALUE_TYPE] = TYPE_INT;
    data[KEY_REG_OP_TYPE] = OP_TYPE_AUTO;
    data_array.append(data);

    data[KEY_REG_KEY] = "blc_gb_offset";
    data[KEY_VALUE] = temp.offset_gb;
    data[KEY_VALUE_TYPE] = TYPE_INT;
    data[KEY_REG_OP_TYPE] = OP_TYPE_AUTO;
    data_array.append(data);

    data[KEY_REG_KEY] = "blc_b_offset";
    data[KEY_VALUE] = temp.offset_b;
    data[KEY_VALUE_TYPE] = TYPE_INT;
    data[KEY_REG_OP_TYPE] = OP_TYPE_AUTO;
    data_array.append(data);

    data[KEY_REG_KEY] = "blc_r_gain";
    data[KEY_VALUE] = static_cast<int>(temp.gain_r * 1024 + 0.5);
    data[KEY_VALUE_TYPE] = TYPE_INT;
    data[KEY_REG_OP_TYPE] = OP_TYPE_AUTO;
    data_array.append(data);

    data[KEY_REG_KEY] = "blc_gr_gain";
    data[KEY_VALUE] = static_cast<int>(temp.gain_gr * 1024 + 0.5);
    data[KEY_VALUE_TYPE] = TYPE_INT;
    data[KEY_REG_OP_TYPE] = OP_TYPE_AUTO;
    data_array.append(data);

    data[KEY_REG_KEY] = "blc_gb_gain";
    data[KEY_VALUE] = static_cast<int>(temp.gain_gb * 1024 + 0.5);
    data[KEY_VALUE_TYPE] = TYPE_INT;
    data[KEY_REG_OP_TYPE] = OP_TYPE_AUTO;
    data_array.append(data);

    data[KEY_REG_KEY] = "blc_b_gain";
    data[KEY_VALUE] = static_cast<int>(temp.gain_b * 1024 + 0.5);
    data[KEY_VALUE_TYPE] = TYPE_INT;
    data[KEY_REG_OP_TYPE] = OP_TYPE_AUTO;
    data_array.append(data);

    return data_array;
}

void BLCCalibrationPageWidget::initWriteDataUI()
{
    QLabel *label = new QLabel("ISO: ");
    optype_combox = new QComboBox;
    QStringList item_list;
    for (int i=0; i<iso_list.size(); i++) {
        item_list.append(QString::number(iso_list[i]));
    }
    optype_combox->addItems(item_list);
    optype_combox->setCurrentIndex(0);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(label);
    layout->addWidget(optype_combox);

    QLabel *method_label = new QLabel("Calibration Method: ");
    method_combox = new QComboBox;
    method_combox->addItems({"Median", "Mean"});
    QHBoxLayout *method_layout = new QHBoxLayout;
    method_layout->addWidget(method_label);
    method_layout->addWidget(method_combox);

    QPushButton *write_data_btn = new QPushButton(tr("Write calibration data"));
    connect(write_data_btn, &QPushButton::clicked, this, std::bind(&BaseCalibrationPageWidget::writeCalibrationData, this, true));

    QVBoxLayout *v_layout = new QVBoxLayout;
    v_layout->addLayout(layout, Qt::AlignTop);
    v_layout->addLayout(method_layout, Qt::AlignTop);
    v_layout->addWidget(write_data_btn, Qt::AlignTop);
    QGroupBox *write_group = new QGroupBox(tr("Write Calibration Data"));
    write_group->setLayout(v_layout);
    write_group->setMaximumHeight(150);

    param_layout->addWidget(write_group);
}

void BLCCalibrationPageWidget::initQuickCalibrationUI()
{
    QGroupBox *quick_group = new QGroupBox(tr("Quick Calibration"));
    QVBoxLayout *v_layout = new QVBoxLayout;
    quick_group->setLayout(v_layout);
    quick_group->setMaximumHeight(120);

    QHBoxLayout *path_layout = new QHBoxLayout;
    open_btn = new QPushButton("Open path");
    connect(open_btn, &QPushButton::clicked, this, &BLCCalibrationPageWidget::OpenFilePath);
    path_edit = new QLineEdit;
    path_edit->setPlaceholderText("For example, D:/../BLC");
    path_edit->setReadOnly(true);
    path_layout->addWidget(open_btn);
    path_layout->addWidget(path_edit);

    quick_btn = new QPushButton("Calibration");
    connect(quick_btn, &QPushButton::clicked, this, std::bind(&BLCCalibrationPageWidget::handleQuickCalibration, this, true));

    v_layout->addLayout(path_layout, Qt::AlignTop);
    v_layout->addWidget(quick_btn, Qt::AlignTop);

    param_layout->addWidget(quick_group);
}

void BLCCalibrationPageWidget::OpenFilePath()
{
    QString save_dir = GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_CALI_SAVE_PATH).toString();
    QString dir_path;

    if (!save_dir.isEmpty()) {
        QDir dir(save_dir);
        if(dir.exists()) {
            dir_path = QFileDialog::getExistingDirectory(this, "Please select file directory", save_dir);
        } else {
            dir_path = QFileDialog::getExistingDirectory(this, "Please Select file directory", "./");
        }
    } else {
        dir_path = QFileDialog::getExistingDirectory(this, "Please Select file directory", "./");
    }

    if (!dir_path.isEmpty()) {
        if (dir_path != save_dir) {
            GlobalData::getInstance()->setSettings(SETTINGS_SECTION, KEY_CALI_SAVE_PATH, dir_path);
            GlobalData::getInstance()->saveSettings();
        }
    } else {
        QMessageBox::warning(this, tr("Warning"), tr("Open dir fail!"), QMessageBox::Ok);
        return;
    }

    path_edit->setText(dir_path);
    qDebug("Opened dir: %s", qUtf8Printable(dir_path));

    int ret = QuickCalibrationLoadData(dir_path);
    if (ret < 0) {
        QMessageBox::warning(this, tr("Warning"), tr("Load file fail!"), QMessageBox::Ok);
        return;
    }
}

int BLCCalibrationPageWidget::handleQuickCalibration(bool showMessage)
{
    if (rawItem.isEmpty()) {
        if (showMessage) {
            QMessageBox::warning(this, tr("Warning"), tr("Please open the file directory first!"), QMessageBox::Ok);
        }
        return -1;
    }

    if (showMessage) {
        emit sigChangeCaliStatus(CALIBRATION_STATUS::CALIBRATION);
    }

    quickMode = true;
    for (int i=0; i<rawItem.size(); i++) {
        raw_img_item_vec.clear();
        raw_img_item_vec.append(rawItem[i]);
        int iso = rawItem[i]->getISO();
        optype_combox->setCurrentIndex(iso_list.indexOf(iso));
        bool status = calibration();
        if (!status) {
            ClearData();
            path_edit->clear();
            optype_combox->setCurrentIndex(0);
            if (showMessage) {
                emit sigChangeCaliStatus(CALIBRATION_STATUS::CAL_ERROR);
            }
            return -1;
        }
        QString path = rawItem[i]->getFilePath();
        QString name = rawItem[i]->getFileName();
        path.remove(path.length()-name.length(), name.length());
        name = QString("blc_iso=%1.json").arg(iso);
        path.append(name);
        qDebug()<<path;
        QJsonObject data;
        QJsonArray items = getCalibrationData();
        data["Cali BLC"] = items;
        utils::saveJson(path, data);
    }

    int ret = writeCalibrationData(showMessage);
    ClearData();
    path_edit->clear();
    optype_combox->setCurrentIndex(0);
    if (showMessage) {
        emit sigChangeCaliStatus(CALIBRATION_STATUS::READY);
        QMessageBox::information(this, tr("Info"), tr("Calibration finished!"), QMessageBox::Ok);
    }

    if (ret < 0) {
        return -2;
    }

    return 0;
}

void BLCCalibrationPageWidget::handleImportData(QString name, QJsonObject data)
{
    qDebug()<<"BLCCalibrationPageWidget import file: "<<name;

    QString blc_module_name = PREFIX_CALI_STR + getCaliModuleName(CALIBRATION_MODULE_ID::BLC);
    if (data.contains(blc_module_name)) {
        QJsonArray blc_data_array = data[blc_module_name].toArray();
        int iso = 100;
        BLC_CALI_RESULT temp = {};

        for (auto blc_data : blc_data_array) {
            QJsonObject blc_data_obj = blc_data.toObject();
            if (blc_data_obj[KEY_REG_KEY] == "ISO") {
                iso = blc_data_obj[KEY_VALUE].toInt();
            } else if (blc_data_obj[KEY_REG_KEY] == "blc_r_offset") {
                temp.offset_r = static_cast<unsigned int>(blc_data_obj[KEY_VALUE].toInt());
            } else if (blc_data_obj[KEY_REG_KEY] == "blc_gr_offset") {
                temp.offset_gr = static_cast<unsigned int>(blc_data_obj[KEY_VALUE].toInt());
            } else if (blc_data_obj[KEY_REG_KEY] == "blc_gb_offset") {
                temp.offset_gb = static_cast<unsigned int>(blc_data_obj[KEY_VALUE].toInt());
            } else if (blc_data_obj[KEY_REG_KEY] == "blc_b_offset") {
                temp.offset_b = static_cast<unsigned int>(blc_data_obj[KEY_VALUE].toInt());
            } else if (blc_data_obj[KEY_REG_KEY] == "blc_r_gain") {
                temp.gain_r = static_cast<float>(blc_data_obj[KEY_VALUE].toInt()/1024.0);
            } else if (blc_data_obj[KEY_REG_KEY] == "blc_gr_gain") {
                temp.gain_gr = static_cast<float>(blc_data_obj[KEY_VALUE].toInt()/1024.0);
            } else if (blc_data_obj[KEY_REG_KEY] == "blc_gb_gain") {
                temp.gain_gb = static_cast<float>(blc_data_obj[KEY_VALUE].toInt()/1024.0);
            } else if (blc_data_obj[KEY_REG_KEY] == "blc_b_gain") {
                temp.gain_b = static_cast<float>(blc_data_obj[KEY_VALUE].toInt()/1024.0);
            }
        }

        caliResultMean.insert(iso, temp);
        caliResultMedian.insert(iso, temp);
        calibrated = true;
    }
}

QJsonObject BLCCalibrationPageWidget::GetModuleObjectToNetwork()
{
    QJsonObject send_obj;
    QJsonObject data_obj;
    QJsonArray items = getCalibrationData();

    if (!items.isEmpty()) {
        send_obj[MSG_KEY_CMD] = CMD_WRITE_MODULE;
        CALIBRATION_MODULE_ID id = static_cast<CALIBRATION_MODULE_ID>(property("cali_id").toInt());
        data_obj[MSG_KEY_MODULE] = PREFIX_CALI_STR + getCaliModuleName(id);
        if (optype_combox->currentIndex() == iso_list.size()) {
            data_obj[MSG_KEY_OP_TYPE] = OP_TYPE_MANUAL;
        }else {
            data_obj[MSG_KEY_OP_TYPE] = OP_TYPE_AUTO;
        }
        data_obj[MSG_KEY_ITEMS] = items;
        send_obj[MSG_KEY_DATA] = data_obj;
    }

    return send_obj;
}

bool BLCCalibrationPageWidget::checkShallExport()
{
    if (!calibrated) {
        return false;
    }

    if (method_combox->currentText() == "Mean") {
        return caliResultMean.isEmpty() ? false : true;
    } else {
        return caliResultMedian.isEmpty() ? false : true;
    }
}

int BLCCalibrationPageWidget::readCalibrationData(bool showMessage)
{
    qDebug("BLCCalibrationPageWidget::readCalibrationData");
    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        if (showMessage) {
            QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        }
        return -1;
    }

    QStringList struct_list = getStructList();
    QMap<QString, QString> struct_get_api_map = getStructGetApiMap();
    CALIBRATION_MODULE_ID module_id = static_cast<CALIBRATION_MODULE_ID>(property("cali_id").toInt());
    NetworkClientHelper::getInstance()->clearIdInfoJsonrpc();
    for (auto struct_id : struct_list) {
        QJsonObject send_obj;
        QJsonObject id_info;
        int jsonrpc_id = GlobalData::getInstance()->getJsonrpcId();
        send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
        send_obj[MSG_KEY_METHOD] = struct_get_api_map[struct_id];
        send_obj[MSG_KEY_ID] = jsonrpc_id;
        id_info[MSG_KEY_CMD] = CMD_READ_BLC_CALIBRATION;
        id_info[MSG_KEY_MODULE] = getCaliModuleName(module_id);
        id_info[MSG_KEY_STRUCT] = struct_id;

        NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(jsonrpc_id, id_info);
        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));

        bool gotResponse = false;
        QTime stopTime = QTime::currentTime().addSecs(READ_WRITE_TIMEOUT);
        while (QTime::currentTime() < stopTime) {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
            if (!GlobalData::getInstance()->getWaitForResponse()) {
                gotResponse = true;
                break;
            }
        }

        if (!gotResponse) {
            NetworkClientHelper::getInstance()->removeIdInfoJsonrpc(jsonrpc_id);
            GlobalData::getInstance()->setWaitForResponse(false);
            if (showMessage) {
                QString message = getTitle() + QString(" Page: Failed to read %1. Timeout: Can't get reponse from board within %2 seconds.").arg(struct_id).arg(READ_WRITE_TIMEOUT);
                emit sigStatusLog(message, LOG_LEVEL::LOG_ERROR);
            }
            return -1;
        }
    }

    return 0;
}

void BLCCalibrationPageWidget::handleReceiveBlcCalibration(int id)
{
    qDebug("BLCCalibrationPageWidget::handleReceiveBlcCalibration.");
    QJsonObject data_obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    if (!data_obj.isEmpty()) {
        QJsonObject result_obj = data_obj[MSG_KEY_RESULT].toObject();
        if (!result_obj.isEmpty()) {
            QJsonObject info_obj = data_obj[MSG_KEY_ID_INFO].toObject();
            QString struct_id = info_obj[MSG_KEY_STRUCT].toString();
            QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();

            QJsonObject mapping_obj;
            QJsonObject param_data;
            utils::parseParams(struct_id, params_obj, mapping_obj, param_data);
            params.insert(struct_id, param_data);
        } else {
            qDebug("BLCCalibrationPageWidget::handleReceiveBlcCalibration. Error: result_obj is empty.");
        }
    } else {
        qDebug("BLCCalibrationPageWidget::handleReceiveBlcCalibration. Error: data_obj is empty.");
    }
}

QJsonObject BLCCalibrationPageWidget::GetModuleObjectToNetworkJsonrpc()
{
    qDebug("BLCCalibrationPageWidget::GetModuleObjectToNetworkJsonrpc.");
    QJsonObject module_obj;
    QStringList struct_list = getStructList();
    QMap<QString, QString> struct_set_api_map = getStructSetApiMap();
    CALIBRATION_MODULE_ID id = static_cast<CALIBRATION_MODULE_ID>(property("cali_id").toInt());
    QString module_name = getCaliModuleName(id);

    NetworkClientHelper::getInstance()->clearIdInfoJsonrpc();
    int jsonrpc_id = GlobalData::getInstance()->getJsonrpcId();
    QString struct_id = struct_list[0];
    QJsonObject param_data = params.value(struct_id);
    QJsonArray arrR = param_data["ISP_BLACK_LEVEL_ATTR_S.stAuto.OffsetR"].toArray();
    QJsonArray arrGr = param_data["ISP_BLACK_LEVEL_ATTR_S.stAuto.OffsetGr"].toArray();
    QJsonArray arrGb = param_data["ISP_BLACK_LEVEL_ATTR_S.stAuto.OffsetGb"].toArray();
    QJsonArray arrB = param_data["ISP_BLACK_LEVEL_ATTR_S.stAuto.OffsetB"].toArray();

    if (method_combox->currentText() == "Mean") {
        if (quickMode) {
            for (int i = 0; i < iso_list.size(); i++) {
                if (caliResultMean.contains(iso_list[i])) {
                    BLC_CALI_RESULT temp = caliResultMean.value(iso_list[i]);
                    arrR.replace(i, temp.offset_r);
                    arrGr.replace(i, temp.offset_gr);
                    arrGb.replace(i, temp.offset_gb);
                    arrB.replace(i, temp.offset_b);
                }
            }
        } else {
            BLC_CALI_RESULT temp = caliResultMean.first();
            int idx = optype_combox->currentIndex();
            arrR.replace(idx, temp.offset_r);
            arrGr.replace(idx, temp.offset_gr);
            arrGb.replace(idx, temp.offset_gb);
            arrB.replace(idx, temp.offset_b);
        }
    } else {
        if (quickMode) {
            for (int i = 0; i < iso_list.size(); i++) {
                if (caliResultMedian.contains(iso_list[i])) {
                    BLC_CALI_RESULT temp = caliResultMedian.value(iso_list[i]);
                    arrR.replace(i, temp.offset_r);
                    arrGr.replace(i, temp.offset_gr);
                    arrGb.replace(i, temp.offset_gb);
                    arrB.replace(i, temp.offset_b);
                }
            }
        } else {
            BLC_CALI_RESULT temp = caliResultMedian.first();
            int idx = optype_combox->currentIndex();
            arrR.replace(idx, temp.offset_r);
            arrGr.replace(idx, temp.offset_gr);
            arrGb.replace(idx, temp.offset_gb);
            arrB.replace(idx, temp.offset_b);
        }
    }

    param_data.insert("ISP_BLACK_LEVEL_ATTR_S.stAuto.OffsetR", arrR);
    param_data.insert("ISP_BLACK_LEVEL_ATTR_S.stAuto.OffsetGr", arrGr);
    param_data.insert("ISP_BLACK_LEVEL_ATTR_S.stAuto.OffsetGb", arrGb);
    param_data.insert("ISP_BLACK_LEVEL_ATTR_S.stAuto.OffsetB", arrB);

    QJsonObject params_obj;
    QStringList keys = param_data.keys();
    for (int i = 0; i < keys.size(); i++) {
        QString path = keys[i];
        QJsonValue value = param_data[path];
        QStringList path_split = path.split(QLatin1Char('.'));
        if (path_split.count() < 2) {
            qDebug("[Error] Invalid path: %s.", qUtf8Printable(path));
            continue;
        } else {
            QJsonObject mapping_obj;
            path_split.removeFirst();
            utils::createParamJsonrpc(path_split, params_obj, value, mapping_obj);
        }
    }

    QJsonObject send_obj;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_METHOD] = struct_set_api_map[struct_id];
    send_obj[MSG_KEY_ID] = jsonrpc_id;
    send_obj[MSG_KEY_PARAMS] = params_obj;
    module_obj[struct_id] = send_obj;

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_WRITE_BLC_CALIBRATION;
    id_info[MSG_KEY_MODULE] = module_name;
    id_info[MSG_KEY_STRUCT] = struct_id;
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(jsonrpc_id, id_info);

    return module_obj;
}

int BLCCalibrationPageWidget::GetIsoFromFileName(QString fileName)
{
    int iso = 100;
    if (!fileName.isEmpty()) {
        QStringList paraList = fileName.split("_");
        for (auto str : paraList) {
            if (str.contains("ISO")) {
                QStringList strList = str.split("=");
                if (strList.count() == 2) {
                    iso = strList[1].toInt();
                }
                break;
            }
        }
    }

    return iso;
}

int BLCCalibrationPageWidget::QuickCalibrationLoadData(QString path, bool externRequest)
{
    ClearData();
    QDir dir(path);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for(int i = 0; i < folder_list.size(); i++) {
        QString dir_name = folder_list.at(i).fileName();
        for (int j=0; j<iso_list.size(); j++) {
            QString name = "ISO" + QString::number(iso_list[j]);
            if (dir_name == name) {
                QString dirPath = path + "/" + dir_name;
                QStringList fileList;
                utils::findFiles(dirPath, ".raw", fileList, false);
                if (!fileList.isEmpty()) {
                    qDebug() << fileList[0];
                    RawImageItem *item = new RawImageItem(fileList[0], SCENE::DARK_FRAME);
                    item->setISO(iso_list[j]);
                    rawItem.append(item);
                }
                break;
            }
        }
        if (rawItem.size() >= iso_list.size()) {
            break;
        }
    }

    if (!rawItem.isEmpty()) {
        if (externRequest) {
            int width = config.getWidth().toInt();
            int height = config.getHeight().toInt();
            QVariant bits = config.getBits();
            QVariant bayer = config.getBayer();
            utils::RAW_PACK_TYPE packed = config.getPacked();
            FRAME_MODE  frame_mode = config.getFrameMode();
            for (int i=0; i<rawItem.size(); i++) {
                rawItem[i]->setImgSize(width, height);
                rawItem[i]->setBits(bits);
                rawItem[i]->setBayer(bayer);
                rawItem[i]->setPacked(packed);
                rawItem[i]->setFrameMode(frame_mode);
            }
        } else {
            RawImageFormatDialog *formt_dialog = new RawImageFormatDialog;
            formt_dialog->setFilepath(rawItem[0]->getFilePath());
            formt_dialog->setScene(SCENE::DARK_FRAME);
            if (QDialog::Accepted == formt_dialog->exec()) {
                std::tuple<QString, QString> size = formt_dialog->getSize();
                int width = std::get<0>(size).toInt();
                int height = std::get<1>(size).toInt();
                QVariant bits = formt_dialog->getBits();
                QVariant bayer = formt_dialog->getBayer();
                utils::RAW_PACK_TYPE packed = formt_dialog->getPacked();
                FRAME_MODE  frame_mode = formt_dialog->getFrameMode();
                for (int i=0; i<rawItem.size(); i++) {
                    rawItem[i]->setImgSize(width, height);
                    rawItem[i]->setBits(bits);
                    rawItem[i]->setBayer(bayer);
                    rawItem[i]->setPacked(packed);
                    rawItem[i]->setFrameMode(frame_mode);
                }
            } else {
                ClearData();
                path_edit->clear();
                return -1;
            }
        }
    } else {
        path_edit->clear();
        return -2;
    }

    return 0;
}

void BLCCalibrationPageWidget::HandleCalibrationRequst(QJsonObject params, RawConfigItem config)
{
    QString path;
    int ret = 0;
    if (params.contains("path") && !(params["path"].toString().isEmpty())) {
        path = params["path"].toString();
    }

    if (path.isEmpty()) {
        emit sigCaliStatus(CALIBRATION_MODULE_ID::BLC, CALIBRATION_STATUS::CAL_ERROR, "Image path is empty!");
        return;
    }

    QDir dir(path);
    if (!dir.exists()) {
        emit sigCaliStatus(CALIBRATION_MODULE_ID::BLC, CALIBRATION_STATUS::CAL_ERROR, "Image path not exists!");
        return;
    }

    if (cali_status == CALIBRATION_STATUS::CALIBRATION) {
        emit sigCaliStatus(CALIBRATION_MODULE_ID::BLC, CALIBRATION_STATUS::CAL_ERROR, "Calibration is busy!");
        return;
    }

    if (params.contains("method")) {
        int method = params["method"].toInt();
        method_combox->setCurrentIndex(method);
    }

    this->config = config;

    ret = QuickCalibrationLoadData(path, true);
    if (ret < 0) {
        emit sigCaliStatus(CALIBRATION_MODULE_ID::BLC, CALIBRATION_STATUS::CAL_ERROR, "Load file fail!");
        return;
    }

    ret = handleQuickCalibration(false);
    if (ret == -1) {
        emit sigCaliStatus(CALIBRATION_MODULE_ID::BLC, CALIBRATION_STATUS::CAL_ERROR, "Calibration fail!");
        return;
    } else if (ret == -2) {
        emit sigCaliStatus(CALIBRATION_MODULE_ID::BLC, CALIBRATION_STATUS::WARNING, "Write data fail!");
        return;
    }

    emit sigCaliStatus(CALIBRATION_MODULE_ID::BLC, CALIBRATION_STATUS::READY, "Calibration finish!");
}
