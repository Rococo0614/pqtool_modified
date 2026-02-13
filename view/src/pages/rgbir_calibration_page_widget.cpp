#include "rgbir_calibration_page_widget.hpp"
#include "utils.hpp"
#include "otp_pdaf_cali.h"
#include "rgbir_cali.h"
#include "calibration_ct_list_widget.hpp"
#include "ccm_lab_window.hpp"
#include <QMessageBox>
#include <QHeaderView>


RGBIRCalibrationPageWidget::RGBIRCalibrationPageWidget(QWidget *parent) : BaseCalibrationPageWidget(static_cast<int>(CALIBRATION_MODULE_ID::RGBIR), parent)
{
    setAcceptScene(SCENE::COLOR24);
    initLeftArea();
    roi_control_widget = createRoiControlUI(this->input_image_label);
    initEvent();
    retranslateUi();
}

bool RGBIRCalibrationPageWidget::calibration()
{
    using utils::RAW_BAYER_ID;
    using utils::RAW_BITS;
    using utils::RAW_PACK_TYPE;

    #define ROI_NUM_OF_ROI 6
    #define DEBUG_ROI_IMAGES
    if (raw_img_item_vec.isEmpty()) {
        return false;
    }

    RawImageItem * raw_img_item = this->raw_img_item_vec.first();
    int bayer_id = raw_img_item->getBayer().toInt();
    int width = raw_img_item->getWidth();
    int height = raw_img_item->getHeight();
    int ir_position = raw_img_item->getIrPosition().toInt();

    bayer_id = ir_position * 4 + bayer_id;
    if (ir_position == utils::IR_NO) {
        QMessageBox message(QMessageBox::NoIcon, "Error", "Not is RGBIR type raw!!!");
        message.exec();
        return false;
    }

    std::vector<unique_ptr<int []>> image_vec = utils::readMultiRawIntData(raw_img_item->getFilePath(), width, height);

    // calibration params
    int roi_width = 16;
    int roi_height = 16;
    int num_of_images = static_cast<int>(image_vec.size());
    vector< vector<int*> > raw_roi_list;
    RGBIR_CALI_RESULT rgbir_cali_result;
    QVector<QRectF> color_rects = this->input_image_label->getColorCheckerRect();
    double scale_factor = this->input_img->getScaleFactor();
    if (color_rects.size() != 24) {
        return false;
    }

    if (num_of_images != 1) {
        QMessageBox message(QMessageBox::NoIcon, "Error", "Number of raw isn't equal to 1!!!");
        message.exec();
        return false;
    }

    // init data
    roi_width = qFloor(color_rects.first().width()/scale_factor);
    roi_height = qFloor(color_rects.first().height()/scale_factor);
    int*** roi_images = new int** [ROI_NUM_OF_ROI];

    for (int k = 0; k < ROI_NUM_OF_ROI; k++) {
        roi_images[k] = new int* [num_of_images];
        for (int j = 0; j < num_of_images; j++) {
            roi_images[k][j] = new int[roi_width * roi_height];
            memset(roi_images[k][j], 0, sizeof(int) * static_cast<unsigned int>(roi_width * roi_height));
        }
    }

    raw_roi_list.resize(static_cast<unsigned int>(ROI_NUM_OF_ROI));
    for (unsigned int k = 0; k < static_cast<unsigned int>(ROI_NUM_OF_ROI); k++) {
       raw_roi_list[k].resize(static_cast<unsigned int>(num_of_images));
    }

    for (unsigned int k = 0; k < static_cast<unsigned int>(ROI_NUM_OF_ROI); k++) {
        QRectF rect = color_rects[static_cast<int>(18 + k)];
        // map back to raw position
        if (scale_factor > 0) {
            rect.setX(qFloor(rect.x()/scale_factor));
            rect.setY(qFloor(rect.y()/scale_factor));
        }

        // color rect position need 4
        if (qFloor(rect.x()) % 4 == 1) {
            rect.setX(qFloor(rect.x()) + 1);
        }
        if (qFloor(rect.y()) % 4 == 1) {
            rect.setY(qFloor(rect.y()) + 1);
        }

        for (unsigned int j = 0; j < static_cast<unsigned int>(num_of_images); j++) {
            int i = 0;
            for (unsigned int rect_j = static_cast<unsigned int>(rect.y()); rect_j < (rect.y() + roi_height); rect_j++) {
                for (unsigned int rect_i = static_cast<unsigned int>(rect.x()); rect_i < (rect.x() + roi_width); rect_i++) {
                    unsigned int rect_pos = rect_i + rect_j * static_cast<unsigned int>(width);
                    int raw_pixel_val = image_vec[j][rect_pos];
                    if (raw_pixel_val < 0) raw_pixel_val = 0;
                    roi_images[k][j][i++] = raw_pixel_val;
                }

            }

        #ifdef DEBUG_ROI_IMAGES
            qDebug("roi width:%d height:%d", roi_width, roi_height);
            utils::write_raw(string("roi_" + std::to_string(k) + ".raw").c_str(), roi_width, roi_height, roi_images[k][j]);
        #endif

            raw_roi_list[k][j] = roi_images[k][j];
        }
    }

    qDebug("bayer_id:%d blc_b_offset:%d blc_gb_offset:%d blc_gr_offset:%d blc_r_offset:%d", bayer_id, blc_b_offset, blc_gb_offset, blc_gr_offset, blc_r_offset);
    qDebug("fCaliSlopeB 0: %.6f\n", rgbir_cali_result.fCaliSlopeB);
    qDebug("fCaliSlopeG 0: %.6f\n", rgbir_cali_result.fCaliSlopeG);
    qDebug("fCaliSlopeR 0: %.6f\n", rgbir_cali_result.fCaliSlopeR);

    if (!quickmode) {
        calibrated = false;
        CaliRgbirResult.clear();
    }

    if (!quickmode){
        cali_rgbir_ir_remove(raw_roi_list, roi_width, roi_height, ROI_NUM_OF_ROI, num_of_images, bayer_id, blc_b_offset, blc_gb_offset, blc_gr_offset, blc_r_offset,
                        rgbir_cali_result.fCaliSlopeB, rgbir_cali_result.fCaliSlopeG, rgbir_cali_result.fCaliSlopeR);

    // Relesase Memory
    for (int k = 0; k < ROI_NUM_OF_ROI; k++) {
        for (int j = 0; j < num_of_images; j++) {
            delete[] roi_images[k][j];
        }
        delete[] roi_images[k];
    }
    delete[] roi_images;
    CaliRgbirResult.insert(raw_img_item->getISO(), rgbir_cali_result);

    qDebug("fCaliSlopeB 0: %.6f\n", rgbir_cali_result.fCaliSlopeB);
    qDebug("fCaliSlopeG 0: %.6f\n", rgbir_cali_result.fCaliSlopeG);
    qDebug("fCaliSlopeR 0: %.6f\n", rgbir_cali_result.fCaliSlopeR);
    qDebug("fCaliSlopeB 0: %.6f\n", rgbir_cali_result.fCaliSlopeB * 1024);
    qDebug("fCaliSlopeG 0: %.6f\n", rgbir_cali_result.fCaliSlopeG * 1024);
    qDebug("fCaliSlopeR 0: %.6f\n", rgbir_cali_result.fCaliSlopeR * 1024);

// display result img
    RAW_PACK_TYPE raw_pack_type = raw_img_item->getPacked();
    int bayerId = raw_img_item->getBayer().toInt();
    RAW_BITS bits = static_cast<RAW_BITS>(raw_img_item->getBits().toInt());
    unsigned char *r = utils::readRawImage(raw_img_item->getFilePath(), static_cast<unsigned int>(width), static_cast<unsigned int>(height),
        static_cast<RAW_BAYER_ID>(bayerId), bits, raw_pack_type);

    if (r == nullptr) {
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

        emit sigShowOutputRawImage(errorImg);

        calibrated = false;
        return false;
    }

    QImage img(r, width, height, QImage::Format_RGB32);


    emit sigShowOutputRawImage(img);

    free(r);
    }
    calibrated = true;
    return true;
}

void RGBIRCalibrationPageWidget::initLeftArea()
{
    initImportRawUI();
    initImportBlcUI();

    edit_roi_btn = new QPushButton;
    param_layout->addWidget(edit_roi_btn);

    // events
    connect(edit_roi_btn, &QPushButton::clicked, [=]() {
        if (roi_control_widget != nullptr) {
            roi_control_widget->show();
        }
    });

    initCaliUI();
    initWriteDataUI();
    initImportDataUI();
    initExportDataUI();
    initQuickCalibrationUI();
    param_layout->setAlignment(Qt::AlignTop);
}


void RGBIRCalibrationPageWidget::initQuickCalibrationUI()
{
    QGroupBox *quick_group = new QGroupBox(tr("Quick Calibration"));
    QVBoxLayout *v_layout = new QVBoxLayout;
    quick_group->setLayout(v_layout);
    quick_group->setMaximumHeight(120);

    QHBoxLayout *path_layout = new QHBoxLayout;
    QHBoxLayout *path_layout_blc = new QHBoxLayout;

    open_rgbir_btn = new QPushButton("Open rgbir path");
    // connect(open_rgbir_btn, &QPushButton::clicked, this, &RGBIRCalibrationPageWidget::OpenRgbirFilePath);
    path_edit_rgbir = new QLineEdit;
    path_edit_rgbir->setPlaceholderText("For example, D:/../RGBIR");
    path_edit_rgbir->setReadOnly(true);

    open_blc_btn = new QPushButton("Open blc path");
    // connect(open_rgbir_btn, &QPushButton::clicked, this, &RGBIRCalibrationPageWidget::OpenBlcFilePath);
    path_edit_blc = new QLineEdit;
    path_edit_blc->setPlaceholderText("For example, D:/../BLC");
    path_edit_blc->setReadOnly(true);

    path_layout->addWidget(open_rgbir_btn);
    path_layout_blc->addWidget(open_blc_btn);


    path_layout->addWidget(path_edit_rgbir);
    path_layout_blc->addWidget(path_edit_blc);

    quick_rgbir_btn = new QPushButton("Calibration");
    // connect(quick_rgbir_btn, &QPushButton::clicked, this, &RGBIRCalibrationPageWidget::handleQuickCalibration);

    v_layout->addLayout(path_layout, Qt::AlignTop);
    v_layout->addLayout(path_layout_blc, Qt::AlignTop);
    v_layout->addWidget(quick_rgbir_btn, Qt::AlignTop);
    param_layout->addWidget(quick_group);
}

void RGBIRCalibrationPageWidget::OpenBlcFilePath()
{
    // To do
}

void RGBIRCalibrationPageWidget::OpenRgbirFilePath()
{
    // To do
}

void RGBIRCalibrationPageWidget::initEvent()
{
    connect(this, &BaseCalibrationPageWidget::sigImportData, this, &RGBIRCalibrationPageWidget::handleImportData);
    shared_ptr<NetworkClientHelper> network_helper = NetworkClientHelper::getInstance();
    connect(network_helper.get(), &NetworkClientHelper::sigReceiveRgbirCalibration,
    this, &RGBIRCalibrationPageWidget::handleReceiveRgbirCalibration);
}

void RGBIRCalibrationPageWidget::initWriteDataUI()
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

    QPushButton *write_data_btn = new QPushButton(tr("Write calibration data"));
    connect(write_data_btn, &QPushButton::clicked, this, std::bind(&BaseCalibrationPageWidget::writeCalibrationData, this, true));

    QVBoxLayout *v_layout = new QVBoxLayout;
    v_layout->addLayout(layout, Qt::AlignTop);
    v_layout->addWidget(write_data_btn, Qt::AlignTop);
    QGroupBox *write_group = new QGroupBox(tr("Write Calibration Data"));
    write_group->setLayout(v_layout);
    write_group->setMaximumHeight(150);
    param_layout->addWidget(write_group);
}
QJsonArray RGBIRCalibrationPageWidget::getCalibrationData()
{
    qDebug()<<"*******************getCalibrationData()********************: "<<endl;
    QJsonArray data_array;
    QJsonObject data;

    int idx = optype_combox->currentText().toInt();

    RGBIR_CALI_RESULT temp = CaliRgbirResult.first();

    qDebug()<<"*******************idx********************: "<<idx<<endl;
    qDebug()<<"*******************temp.fCaliSlopeR********************: "<<temp.fCaliSlopeR<<endl;
    data[KEY_REG_KEY] = "ISO";
    data[KEY_VALUE] = idx;
    data[KEY_VALUE_TYPE] = TYPE_DOUBLE;
    data[KEY_REG_OP_TYPE] = OP_TYPE_AUTO;
    data_array.append(data);

    data[KEY_REG_KEY] = "fCaliSlopeB";
    data[KEY_VALUE] = temp.fCaliSlopeB;
    data[KEY_VALUE_TYPE] = TYPE_DOUBLE;
    data[KEY_REG_OP_TYPE] = OP_TYPE_AUTO;
    data_array.append(data);

    data[KEY_REG_KEY] = "fCaliSlopeG";
    data[KEY_VALUE] = temp.fCaliSlopeG;
    data[KEY_VALUE_TYPE] = TYPE_DOUBLE;
    data[KEY_REG_OP_TYPE] = OP_TYPE_AUTO;
    data_array.append(data);

    data[KEY_REG_KEY] = "fCaliSlopeR";
    data[KEY_VALUE] = temp.fCaliSlopeR;
    data[KEY_VALUE_TYPE] = TYPE_DOUBLE;
    data[KEY_REG_OP_TYPE] = OP_TYPE_AUTO;
    qDebug()<<"*******************temp.fCaliSlopeB********************: "<<temp.fCaliSlopeB<<endl;
    data_array.append(data);

    return data_array;
}

void RGBIRCalibrationPageWidget::handleImportData(QString name, QJsonObject data)
{
    qDebug()<<"RGBIRCalibrationPageWidget import file: "<<name;

    // import blc data
    QString blc_module_name = PREFIX_CALI_STR + getCaliModuleName(CALIBRATION_MODULE_ID::BLC);  // PREFIX_CALI_STR = Cail
    if (data.contains(blc_module_name)) {
        blc_imported = true;
        QJsonArray blc_data_array = data[blc_module_name].toArray();
        for (auto blc_data : blc_data_array) {
            QJsonObject blc_data_obj = blc_data.toObject();
            if (blc_data_obj[KEY_REG_KEY] == "blc_r_offset") {
                blc_r_offset = blc_data_obj[KEY_VALUE].toInt();
            } else if (blc_data_obj[KEY_REG_KEY] == "blc_gr_offset") {
                blc_gr_offset = blc_data_obj[KEY_VALUE].toInt();
            } else if (blc_data_obj[KEY_REG_KEY] == "blc_gb_offset") {
                blc_gb_offset = blc_data_obj[KEY_VALUE].toInt();
            } else if (blc_data_obj[KEY_REG_KEY] == "blc_b_offset") {
                blc_b_offset= blc_data_obj[KEY_VALUE].toInt();
            }
        }
    }

    QString rgbir_module_name = PREFIX_CALI_STR + getCaliModuleName(CALIBRATION_MODULE_ID::RGBIR);
    if (data.contains(rgbir_module_name)) {
        blc_imported = true;
        QJsonArray rgbir_data_array = data[rgbir_module_name].toArray();
        int iso = 100;
        RGBIR_CALI_RESULT temp = {};

        for (auto rgbir_data : rgbir_data_array) {
            QJsonObject rgbir_data_obj = rgbir_data.toObject();
            if (rgbir_data_obj[KEY_REG_KEY] == "ISO") {
                iso = rgbir_data_obj[KEY_VALUE].toInt();
            } else if (rgbir_data_obj[KEY_REG_KEY] == "fCaliSlopeB") {
                temp.fCaliSlopeB = rgbir_data_obj[KEY_VALUE].toDouble();
            } else if (rgbir_data_obj[KEY_REG_KEY] == "fCaliSlopeG") {
                temp.fCaliSlopeG = rgbir_data_obj[KEY_VALUE].toDouble();
            } else if (rgbir_data_obj[KEY_REG_KEY] == "fCaliSlopeR") {
                temp.fCaliSlopeR = rgbir_data_obj[KEY_VALUE].toDouble();
            }
        }
        CaliRgbirResult.insert(iso, temp);
        calibrated = true;
    }
}

void RGBIRCalibrationPageWidget::retranslateUi()
{
    BaseCalibrationPageWidget::retranslateUi();
    edit_roi_btn->setText(tr("Edit ROIs"));
}

QJsonObject RGBIRCalibrationPageWidget::GetModuleObjectToNetwork()
{
    qDebug()<<"GetModuleObjectToNetwork";
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

int RGBIRCalibrationPageWidget::readCalibrationData(bool showMessage)
{
    qDebug("RGBIRCalibrationPageWidget::readModuleFromNetworkJsonrpc");
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
        id_info[MSG_KEY_CMD] = CMD_READ_RGBIR_CALIBRATION;
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

void RGBIRCalibrationPageWidget::handleReceiveRgbirCalibration(int id)
{
    qDebug("RgbirCalibrationPageWidget::handleReceiveRgbirCalibration.");
    QJsonObject data_obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    if (!data_obj.isEmpty()) {
        QJsonObject result_obj = data_obj[MSG_KEY_RESULT].toObject();
        if (!result_obj.isEmpty()) {
            QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();
            QJsonObject stAuto_obj = params_obj["stAuto"].toObject();
            rg_gain_array = stAuto_obj["u16RecRgGain"].toArray();
            bg_gain_array = stAuto_obj["u16RecBgGain"].toArray();
            gbr_gain_array = stAuto_obj["u16RecGbrGain"].toArray();
            gir_gain_array = stAuto_obj["u16RecGirGain"].toArray();
            qDebug()<<"RgbirCalibrationPageWidget::handleReceiveRgbirCalibration:    "<<gir_gain_array;
        } else {
            qDebug("RgbirCalibrationPageWidget::handleReceiveRgbirCalibration. Error: result_obj is empty.");
        }
    } else {
        qDebug("RgbirCalibrationPageWidget::handleReceiveRgbirCalibration Error: data_obj is empty.");
    }
}
QJsonObject RGBIRCalibrationPageWidget::GetModuleObjectToNetworkJsonrpc()
{
    qDebug("RGBIRCalibrationPageWidget::GetModuleObjectToNetworkJsonrpc.");
    QJsonObject module_obj;
    QStringList struct_list = getStructList();
    QMap<QString, QString> struct_set_api_map = getStructSetApiMap();
    CALIBRATION_MODULE_ID id = static_cast<CALIBRATION_MODULE_ID>(property("cali_id").toInt());
    QString module_name = getCaliModuleName(id);

    NetworkClientHelper::getInstance()->clearIdInfoJsonrpc();
    qDebug()<<" NetworkClientHelper::getInstance()->clearIdInfoJsonrpc()";
    int jsonrpc_id = GlobalData::getInstance()->getJsonrpcId();
    QString struct_id = struct_list[0];

    if (quickmode) {
         for (int i = 0; i < iso_list.size(); i++) {
                if (CaliRgbirResult.contains(iso_list[i])) {
                    RGBIR_CALI_RESULT temp = CaliRgbirResult.value(iso_list[i]);
                    rg_gain_array[i] = static_cast<int>(temp.fCaliSlopeR * 1024);
                    bg_gain_array[i] = static_cast<int>(temp.fCaliSlopeB * 1024);
                    gbr_gain_array[i] = static_cast<int>(temp.fCaliSlopeG * 1024);
                    gir_gain_array[i] = static_cast<int>(temp.fCaliSlopeG * 1024);
                }
            }
    }else{
        RGBIR_CALI_RESULT temp = CaliRgbirResult.first();
        int idx = optype_combox->currentIndex();
        rg_gain_array[idx] = static_cast<int>(temp.fCaliSlopeR * 1024);
        bg_gain_array[idx] = static_cast<int>(temp.fCaliSlopeB * 1024);
        gbr_gain_array[idx] = static_cast<int>(temp.fCaliSlopeG * 1024);
        gir_gain_array[idx] = static_cast<int>(temp.fCaliSlopeG * 1024);
    }
    for (const auto &value : rg_gain_array) {
        qDebug() << value.toString() << ", ";
    }

    QJsonObject send_obj;
    QJsonObject params_obj;
    QJsonObject stAuto_obj;
    stAuto_obj["u16RecRgGain"] = rg_gain_array;
    stAuto_obj["u16RecBgGain"] = bg_gain_array;
    stAuto_obj["u16RecGbrGain"] = gbr_gain_array;
    stAuto_obj["u16RecGirGain"] = gir_gain_array;
    params_obj["stAuto"] = stAuto_obj;

    // output the inforamation
    for (const auto &value : rg_gain_array) {
        qDebug() << value.toString() << ", ";
    }

    // params_obj["CalibrationSlope"] = getCalibrationData();
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_METHOD] = struct_set_api_map[struct_id];
    send_obj[MSG_KEY_ID] = jsonrpc_id;
    send_obj[MSG_KEY_PARAMS] = params_obj;
    module_obj[struct_id] = send_obj;

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_WRITE_RGBIR_CALIBRATION;
    id_info[MSG_KEY_MODULE] = module_name;
    id_info[MSG_KEY_STRUCT] = struct_id;
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(jsonrpc_id, id_info);

    return module_obj;
}
