#include "lblc_calibration_page_widget.hpp"
#include "calibration_iso_list_lblc_widget.hpp"
#include "otp_pdaf_cali.h"
#include "utils.hpp"
#include "raw_image_format_dialog.hpp"
#include "network_client_helper.hpp"
#include <QMessageBox>
#include <QFileDialog>

LBLCCalibrationPageWidget::LBLCCalibrationPageWidget(QWidget *parent) : BaseCalibrationPageWidget(static_cast<int>(CALIBRATION_MODULE_ID::LBLC), parent)
{
    setAcceptScene(SCENE::DARK_FRAME);

    initLeftArea();
    initEvent();
    retranslateUi();
}

void LBLCCalibrationPageWidget::ClearData()
{
    for (int i = 0; i < rawItem.size(); i++) {
        if (rawItem[i] != nullptr) {
            delete rawItem[i];
        }
    }
    rawItem.clear();
    for (int i = 0; i < isoItem_list.size(); i++) {
        if (isoItem_list[i] != nullptr) {
            delete isoItem_list[i];
        }
    }
    isoItem_list.clear();
    isoItem1_list.clear();
    calibrated = false;
    quickMode = false;
    cali_status = CALIBRATION_STATUS::READY;
}

void LBLCCalibrationPageWidget::initLeftArea()
{
    initImportRawUI();

    initCaliUI();
    initWriteDataUI();
    initImportDataUI();
    initExportDataUI();
    initQuickCalibrationUI();
    initISOListUI();
}

void LBLCCalibrationPageWidget::initQuickCalibrationUI()
{
    QGroupBox *quick_group = new QGroupBox(tr("Quick Calibration"));
    QVBoxLayout *v_layout = new QVBoxLayout;
    quick_group->setLayout(v_layout);
    quick_group->setMaximumHeight(120);

    QHBoxLayout *path_layout = new QHBoxLayout;
    open_btn = new QPushButton("Open path");
    connect(open_btn, &QPushButton::clicked, this, &LBLCCalibrationPageWidget::OpenFilePath);
    path_edit = new QLineEdit;
    path_edit->setPlaceholderText("For example, D:/../LBLC");
    path_edit->setReadOnly(true);
    path_layout->addWidget(open_btn);
    path_layout->addWidget(path_edit);

    quick_btn = new QPushButton("Calibration");
    connect(quick_btn, &QPushButton::clicked, this, std::bind(&LBLCCalibrationPageWidget::handleQuickCalibration, this, true));

    v_layout->addLayout(path_layout, Qt::AlignTop);
    v_layout->addWidget(quick_btn, Qt::AlignTop);

    param_layout->addWidget(quick_group);
}

void LBLCCalibrationPageWidget::OpenFilePath()
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

int LBLCCalibrationPageWidget::handleQuickCalibration(bool showMessage)
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
        bool status = calibration();
        if (!status) {
            ClearData();
            path_edit->clear();
            if (showMessage) {
                emit sigChangeCaliStatus(CALIBRATION_STATUS::CAL_ERROR);
            }
            return -1;
        }
        isoItem_list.clear();
        ISOItem *item = new ISOItem(rawItem[i]->getFileName());
        item->setLBLC(lblc_rr_gain, lblc_gr_gain, lblc_gb_gain, lblc_bb_gain, lblc_rr_offset.get(),
                      lblc_gr_offset.get(), lblc_gb_offset.get(), lblc_bb_offset.get(), block_nums);
        item->setISO(rawItem[i]->getISO());
        isoItem_list.append(item);
        isoItem1_list.append(item);

        QString path = rawItem[i]->getFilePath();
        QString file_name = rawItem[i]->getFileName();
        path.remove(path.length() - file_name.length(), file_name.length());
        path += "lblc.json";
        QJsonObject data;
        QJsonArray items = getCalibrationData();
        data["Cali LBLC"] = items;
        utils::saveLblcJson(path, data);
    }

    isoItem_list.clear();
    isoItem_list = isoItem1_list;
    int ret = writeCalibrationData(showMessage);
    ClearData();
    path_edit->clear();
    if (showMessage) {
        emit sigChangeCaliStatus(CALIBRATION_STATUS::READY);
        QMessageBox::information(this, tr("Info"), tr("Calibration finished!"), QMessageBox::Ok);
    }
    if (ret < 0) {
        return -2;
    }

    return 0;
}

void LBLCCalibrationPageWidget::retranslateUi()
{
    BaseCalibrationPageWidget::retranslateUi();
}

void LBLCCalibrationPageWidget::initEvent()
{
    connect(this, &BaseCalibrationPageWidget::sigImportData, this, &LBLCCalibrationPageWidget::handleImportData);
}

void LBLCCalibrationPageWidget::handleImportData(QString name, QJsonObject data)
{
    qDebug()<<"LBLCCalibrationPageWidget import file: "<<name;

    // import blc data
    QString lblc_module_name = PREFIX_CALI_STR + getCaliModuleName(CALIBRATION_MODULE_ID::LBLC);
    if (data.contains(lblc_module_name)) {
        QJsonArray lblc_data_array = data[lblc_module_name].toArray();
        int iso = 100;

        for (auto lblc_data : lblc_data_array) {
            QJsonObject lblc_data_obj = lblc_data.toObject();
            if (lblc_data_obj[KEY_REG_KEY] == "ISO") {
                iso = lblc_data_obj[KEY_VALUE].toInt();
            } else if (lblc_data_obj[KEY_REG_KEY] == "lblc_rr_offset") {
                QJsonArray value_array = lblc_data_obj[KEY_VALUE].toArray();
                int *lblc_rr_offset_array = lblc_rr_offset.get();
                for (int i = 0; i < value_array.size(); i++) {
                    if (i > block_nums) break;
                    lblc_rr_offset_array[i] = value_array[i].toInt();
                }
            } else if (lblc_data_obj[KEY_REG_KEY] == "lblc_gr_offset") {
                QJsonArray value_array = lblc_data_obj[KEY_VALUE].toArray();
                int *lblc_gr_offset_array = lblc_gr_offset.get();
                for (int i = 0; i < value_array.size(); i++) {
                    if (i > block_nums) break;
                    lblc_gr_offset_array[i] = value_array[i].toInt();
                }
            } else if (lblc_data_obj[KEY_REG_KEY] == "lblc_gb_offset") {
                QJsonArray value_array = lblc_data_obj[KEY_VALUE].toArray();
                int *lblc_gb_offset_array = lblc_gb_offset.get();
                for (int i = 0; i < value_array.size(); i++) {
                    if (i > block_nums) break;
                    lblc_gb_offset_array[i] = value_array[i].toInt();
                }
            } else if (lblc_data_obj[KEY_REG_KEY] == "lblc_bb_offset") {
                QJsonArray value_array = lblc_data_obj[KEY_VALUE].toArray();
                int *lblc_bb_offset_array = lblc_bb_offset.get();
                for (int i = 0; i < value_array.size(); i++) {
                    if (i > block_nums) break;
                    lblc_bb_offset_array[i] = value_array[i].toInt();
                }
            } else if (lblc_data_obj[KEY_REG_KEY] == "lblc_rr_gain") {
                lblc_rr_gain = static_cast<float>(lblc_data_obj[KEY_VALUE].toInt()/1024.0);
            } else if (lblc_data_obj[KEY_REG_KEY] == "lblc_gr_gain") {
                lblc_gr_gain = static_cast<float>(lblc_data_obj[KEY_VALUE].toInt()/1024.0);
            } else if (lblc_data_obj[KEY_REG_KEY] == "lblc_gb_gain") {
                lblc_gb_gain = static_cast<float>(lblc_data_obj[KEY_VALUE].toInt()/1024.0);
            } else if (lblc_data_obj[KEY_REG_KEY] == "lblc_bb_gain") {
                lblc_bb_gain = static_cast<float>(lblc_data_obj[KEY_VALUE].toInt()/1024.0);
            }
        }

        ISOItem *iso_item = new ISOItem(name);
        iso_item->setLBLC(lblc_rr_gain, lblc_gr_gain, lblc_gb_gain, lblc_bb_gain, lblc_rr_offset.get(),
                          lblc_gr_offset.get(), lblc_gb_offset.get(), lblc_bb_offset.get(), block_nums);
        iso_item->setISO(iso);
        iso_item->setDisplay(Qt::CheckState::Checked);
        bool success = iso_list_widget->addTableItem(iso_item);
        if (!success) {
            QMessageBox message(QMessageBox::NoIcon, "Error", "Import lblc calibration data, Add iso item failed");
            message.exec();
            return;
        }

        calibrated = true;
    }
}


bool LBLCCalibrationPageWidget::calibration()
{
    using utils::RAW_BAYER_ID;
    using utils::RAW_BITS;

    if (raw_img_item_vec.isEmpty()) {
        return false;
    }

    cali_status = CALIBRATION_STATUS::CALIBRATION;

    RawImageItem * raw_img_item = this->raw_img_item_vec.first();
    int width = raw_img_item->getWidth();
    int height = raw_img_item->getHeight();
    int order = raw_img_item->getBayer().toInt();
    QString raw_path = raw_img_item->getFilePath();
    bool wdr_mode = raw_img_item->getFrameMode() == FRAME_MODE::WDR ? true : false;
    unique_ptr<int []> image = utils::readRawIntData(raw_path, width, height, wdr_mode);
    if (image == nullptr) {
        return false;
    }

    if (!quickMode) {
        calibrated = false;
        int iso = GetIsoFromFileName(raw_img_item->getFileName());
        raw_img_item->setISO(iso);
    }

    // calibration
    int *raw_image = image.get();
    if (wdr_mode) width /= 2;

    cali_blc_calc_by_localblc(&raw_image, width, height, order,
        lblc_rr_offset.get(), lblc_gr_offset.get(), lblc_gb_offset.get(), lblc_bb_offset.get(),
        &lblc_rr_gain, &lblc_gr_gain, &lblc_gb_gain, &lblc_bb_gain,
        LBLC_BLOCK_SIZE, CV182X);

    calibrated = true;
    cali_status = CALIBRATION_STATUS::READY;

    return true;
}

static bool compareData(ISOItem *item1, ISOItem *item2)
{
    if (item1->getISO() < item2->getISO()) {
        return true;
    }

    return false;
}

QJsonArray LBLCCalibrationPageWidget::getCalibrationData()
{
    QJsonArray data_array;
    QJsonObject iso_data;

    QList<ISOItem *> iso_list;
    if (quickMode) {
        iso_list = isoItem_list;
    } else {
        iso_list = iso_list_widget->getDisplayISOList();
    }
    std::sort(iso_list.begin(), iso_list.end(), compareData);

    int iso_array[LBLC_MAX_SIZE] = {0};
    for (int i = 0; i < iso_list.size(); i++) {
        iso_array[i] = iso_list[i]->getISO();
    }

    iso_data[KEY_REG_KEY] = "ISO";
    iso_data[KEY_VALUE] = utils::arrayPtrToJsonArray(iso_array, iso_list.size());
    iso_data[KEY_VALUE_TYPE] = TYPE_INT;
    iso_data[KEY_REG_OP_TYPE] = OP_TYPE_AUTO;
    data_array.append(iso_data);

    QJsonObject data;
    for (int i = 0; i < iso_list.size(); i++) {
        iso_list[i]->getLBLC(lblc_rr_gain, lblc_gr_gain, lblc_gb_gain, lblc_bb_gain,
                           lblc_rr_offset.get(), lblc_gr_offset.get(), lblc_gb_offset.get(), lblc_bb_offset.get());

        // LBLC
        data[KEY_REG_KEY] = QString("lblc_rr_gain[%1]").arg(i);
        data[KEY_VALUE] = static_cast<int>(lblc_rr_gain * 1024 + 0.5);
        data[KEY_VALUE_TYPE] = TYPE_INT;
        data[KEY_REG_OP_TYPE] = OP_TYPE_AUTO;
        data_array.append(data);

        data[KEY_REG_KEY] = QString("lblc_gr_gain[%1]").arg(i);
        data[KEY_VALUE] = static_cast<int>(lblc_gr_gain * 1024 + 0.5);
        data[KEY_VALUE_TYPE] = TYPE_INT;
        data[KEY_REG_OP_TYPE] = OP_TYPE_AUTO;
        data_array.append(data);

        data[KEY_REG_KEY] = QString("lblc_gb_gain[%1]").arg(i);
        data[KEY_VALUE] = static_cast<int>(lblc_gb_gain * 1024 + 0.5);
        data[KEY_VALUE_TYPE] = TYPE_INT;
        data[KEY_REG_OP_TYPE] = OP_TYPE_AUTO;
        data_array.append(data);

        data[KEY_REG_KEY] = QString("lblc_bb_gain[%1]").arg(i);
        data[KEY_VALUE] = static_cast<int>(lblc_bb_gain * 1024 + 0.5);;
        data[KEY_VALUE_TYPE] = TYPE_INT;
        data[KEY_REG_OP_TYPE] = OP_TYPE_AUTO;
        data_array.append(data);

        data[KEY_REG_KEY] = QString("lblc_rr_offset[%1]").arg(i);
        data[KEY_VALUE] = utils::arrayPtrToJsonArray(lblc_rr_offset.get(), block_nums);
        data[KEY_VALUE_TYPE] = TYPE_ARRAY;
        data[KEY_REG_OP_TYPE] = OP_TYPE_AUTO;
        data_array.append(data);

        data[KEY_REG_KEY] = QString("lblc_gr_offset[%1]").arg(i);
        data[KEY_VALUE] = utils::arrayPtrToJsonArray(lblc_gr_offset.get(), block_nums);
        data[KEY_VALUE_TYPE] = TYPE_ARRAY;
        data[KEY_REG_OP_TYPE] = OP_TYPE_AUTO;
        data_array.append(data);

        data[KEY_REG_KEY] = QString("lblc_gb_offset[%1]").arg(i);
        data[KEY_VALUE] = utils::arrayPtrToJsonArray(lblc_gb_offset.get(), block_nums);
        data[KEY_VALUE_TYPE] = TYPE_ARRAY;
        data[KEY_REG_OP_TYPE] = OP_TYPE_AUTO;
        data_array.append(data);

        data[KEY_REG_KEY] = QString("lblc_bb_offset[%1]").arg(i);
        data[KEY_VALUE] = utils::arrayPtrToJsonArray(lblc_bb_offset.get(), block_nums);
        data[KEY_VALUE_TYPE] = TYPE_ARRAY;
        data[KEY_REG_OP_TYPE] = OP_TYPE_AUTO;
        data_array.append(data);
    }

    return data_array;
}

bool LBLCCalibrationPageWidget::checkShallExport()
{
    int list_size = 0;
    if (quickMode) {
        list_size = isoItem_list.size();
    } else {
        list_size = iso_list_widget->getDisplayISOList().size();
    }

    if (list_size >= 1 && list_size <= LBLC_MAX_SIZE)
        return true;

    return false;
}

void LBLCCalibrationPageWidget::initISOListUI()
{
    iso_list_widget = new CalibrationISOLBlcListWidget;
    param_layout->addWidget(iso_list_widget);

    connect(this, &BaseCalibrationPageWidget::sigCalibration, [=]() {
        if (iso_list_widget->getISOList().size() == CalibrationISOLBlcListWidget::MAX_LBLC_ISO_ITEM_NUM) {
            QMessageBox message(QMessageBox::NoIcon, "Error", "Can only add " +
                                QString::number(CalibrationISOLBlcListWidget::MAX_LBLC_ISO_ITEM_NUM) + " iso items");
            message.exec();
            return;
        }

        if (this->raw_img_item_vec.isEmpty()) {
            QMessageBox message(QMessageBox::NoIcon, "Error", "Please import raw first");
            message.exec();
            return;
        }
        RawImageItem *raw_img_item = this->raw_img_item_vec.first();
        ISOItem *iso_item = new ISOItem(raw_img_item->getFileName());
        iso_item->setISO(GetIsoFromFileName(raw_img_item->getFileName()));
        bool success = iso_list_widget->addTableItem(iso_item);
        if (!success) {
            QMessageBox message(QMessageBox::NoIcon, "Error", "Add iso item failed");
            message.exec();
            return;
        }
        iso_item->setLBLC(lblc_rr_gain, lblc_gr_gain, lblc_gb_gain, lblc_bb_gain, lblc_rr_offset.get(),
                          lblc_gr_offset.get(), lblc_gb_offset.get(), lblc_bb_offset.get(), block_nums);
        iso_item->setDisplay(Qt::CheckState::Checked);
    });
}

QJsonObject LBLCCalibrationPageWidget::GetModuleObjectToNetworkJsonrpc()
{
    qDebug("LBLCCalibrationPageWidget::GetModuleObjectToNetworkJsonrpc.");
    QJsonObject module_obj;
    QStringList struct_list = getStructList();
    QMap<QString, QString> struct_set_api_map = getStructSetApiMap();
    CALIBRATION_MODULE_ID id = static_cast<CALIBRATION_MODULE_ID>(property("cali_id").toInt());
    QString module_name = getCaliModuleName(id);

    NetworkClientHelper::getInstance()->clearIdInfoJsonrpc();
    int jsonrpc_id = GlobalData::getInstance()->getJsonrpcId();
    QString struct_id = struct_list[0];

    QList<ISOItem *> iso_list;
    if (quickMode) {
        iso_list = isoItem_list;
    } else {
        iso_list = iso_list_widget->getDisplayISOList();
    }
    std::sort(iso_list.begin(), iso_list.end(), compareData);

    QJsonObject send_obj;
    QJsonObject params_obj;
    QJsonArray Lblc_array;
    for (int i = 0; i < iso_list.size(); i++) {
        QJsonObject Lblc_obj;
        iso_list[i]->getLBLC(lblc_rr_gain, lblc_gr_gain, lblc_gb_gain, lblc_bb_gain,
                           lblc_rr_offset.get(), lblc_gr_offset.get(), lblc_gb_offset.get(), lblc_bb_offset.get());

        Lblc_obj["iso"] = iso_list[i]->getISO();
        Lblc_obj["lblcOffsetR"] = utils::arrayPtrToJsonArray(lblc_rr_offset.get(), block_nums);
        Lblc_obj["lblcOffsetGr"] = utils::arrayPtrToJsonArray(lblc_gr_offset.get(), block_nums);
        Lblc_obj["lblcOffsetGb"] = utils::arrayPtrToJsonArray(lblc_gb_offset.get(), block_nums);
        Lblc_obj["lblcOffsetB"] = utils::arrayPtrToJsonArray(lblc_bb_offset.get(), block_nums);
        Lblc_array.append(Lblc_obj);
    }
    params_obj["size"] = iso_list.size();
    params_obj["lblcLut"] = Lblc_array;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_METHOD] = struct_set_api_map[struct_id];
    send_obj[MSG_KEY_ID] = jsonrpc_id;
    send_obj[MSG_KEY_PARAMS] = params_obj;
    module_obj[struct_id] = send_obj;

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_WRITE_LBLC_CALIBRATION;
    id_info[MSG_KEY_MODULE] = module_name;
    id_info[MSG_KEY_STRUCT] = struct_id;
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(jsonrpc_id, id_info);

    return module_obj;
}

int LBLCCalibrationPageWidget::GetIsoFromFileName(QString fileName)
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

int LBLCCalibrationPageWidget::QuickCalibrationLoadData(QString path, bool externRequest)
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
        if (rawItem.size() >= LBLC_MAX_SIZE) {
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
        return -1;
    }

    return 0;
}

void LBLCCalibrationPageWidget::HandleCalibrationRequst(QJsonObject params, RawConfigItem config)
{
    QString path;
    int ret = 0;
    if (params.contains("path") && !(params["path"].toString().isEmpty())) {
        path = params["path"].toString();
    }

    if (path.isEmpty()) {
        emit sigCaliStatus(CALIBRATION_MODULE_ID::LBLC, CALIBRATION_STATUS::CAL_ERROR, "Image path is empty!");
        return;
    }

    QDir dir(path);
    if (!dir.exists()) {
        emit sigCaliStatus(CALIBRATION_MODULE_ID::LBLC, CALIBRATION_STATUS::CAL_ERROR, "Image path not exists!");
        return;
    }

    if (cali_status == CALIBRATION_STATUS::CALIBRATION) {
        emit sigCaliStatus(CALIBRATION_MODULE_ID::LBLC, CALIBRATION_STATUS::CAL_ERROR, "Calibration is busy!");
        return;
    }

    this->config = config;

    ret = QuickCalibrationLoadData(path, true);
    if (ret < 0) {
        emit sigCaliStatus(CALIBRATION_MODULE_ID::LBLC, CALIBRATION_STATUS::CAL_ERROR, "Load file fail!");
        return;
    }

    ret = handleQuickCalibration(false);
    if (ret == -1) {
        emit sigCaliStatus(CALIBRATION_MODULE_ID::LBLC, CALIBRATION_STATUS::CAL_ERROR, "Calibration fail!");
        return;
    } else if (ret == -2) {
        emit sigCaliStatus(CALIBRATION_MODULE_ID::LBLC, CALIBRATION_STATUS::WARNING, "Write data fail!");
        return;
    }

    emit sigCaliStatus(CALIBRATION_MODULE_ID::LBLC, CALIBRATION_STATUS::READY, "Calibration finish!");
}
