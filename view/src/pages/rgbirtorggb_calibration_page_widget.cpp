#include "rgbirtorggb_calibration_page_widget.hpp"
#include <QMessageBox>
#include <QHeaderView>
#include "otp_pdaf_cali.h"
#include "rgbir_cali.h"
#include <QPainter>
#include <QFileDialog>

// 在rgbir模块的快速标定暂时不支持，只有手动导入标定


RGBIRToRGGBCalibrationPageWidget::RGBIRToRGGBCalibrationPageWidget(QWidget *parent) : BaseCalibrationPageWidget(static_cast<int>(CALIBRATION_MODULE_ID::RGBIRToRGGB), parent)
{
    setAcceptScene(SCENE::COLOR24);
    initLeftArea();
    initEvent();
    retranslateUi();
}
// inherit BaseCali page to init common UI
void RGBIRToRGGBCalibrationPageWidget::initLeftArea()
{
    initImportRawUI();
    initImportBlcUI();

    QLabel *label = new QLabel("ISO: ");
    iso_item = new QComboBox;
    for (int i = 0; i < iso_list.size(); i++) {
        iso_item->addItem(QString("%1").arg(iso_list[i]));
    }
    iso_item->setCurrentIndex(0);
    QHBoxLayout *h_layout = new QHBoxLayout;
    h_layout->addWidget(label);
    h_layout->addWidget(iso_item);
    param_layout->addLayout(h_layout);
    initImportRgbirUI();

    //initCaliUI();
    initConvertRaw();
    param_layout->setAlignment(Qt::AlignTop);
}

void RGBIRToRGGBCalibrationPageWidget::initConvertRaw()
{
    QGroupBox *quick_group = new QGroupBox(tr("Convert RGBIR To RGGB"));
    QVBoxLayout *v_layout = new QVBoxLayout;
    quick_group->setLayout(v_layout);
    quick_group->setMaximumHeight(120);

    QHBoxLayout *path_layout = new QHBoxLayout;
    open_btn = new QPushButton("Open Save path");
    connect(open_btn, &QPushButton::clicked, this, &RGBIRToRGGBCalibrationPageWidget::OpenSaveFilePath);
    path_edit = new QLineEdit;
    path_edit->setPlaceholderText("Select save path");
    path_edit->setReadOnly(true);
    path_layout->addWidget(open_btn);
    path_layout->addWidget(path_edit);

    convertRaw_btn = new QPushButton("Convert");
    connect(convertRaw_btn, &QPushButton::clicked, this, &RGBIRToRGGBCalibrationPageWidget::handlConvertRgbirRaw);

    v_layout->addLayout(path_layout, Qt::AlignTop);
    v_layout->addWidget(convertRaw_btn, Qt::AlignTop);

    param_layout->addWidget(quick_group);

}

void RGBIRToRGGBCalibrationPageWidget::initEvent()
{
    connect(this, &BaseCalibrationPageWidget::sigImportData, this, &RGBIRToRGGBCalibrationPageWidget::handleImportData);
}

void RGBIRToRGGBCalibrationPageWidget::handlConvertRgbirRaw()
{
    using utils::RAW_BAYER_ID;
    using utils::RAW_BITS;
    if (this->raw_img_item_vec.isEmpty()) {
        return ;
    }

    RawImageItem * raw_img_item = this->raw_img_item_vec.first();
    int width = raw_img_item->getWidth();
    int height = raw_img_item->getHeight();
    std::vector<unique_ptr<int []>> image_vec = utils::readMultiRawIntData(raw_img_item->getFilePath(), width, height);
    int img_cnt = static_cast<int>(image_vec.size());
    int bayer_id = raw_img_item->getBayer().toInt();
    int ir_position = raw_img_item->getIrPosition().toInt();
    QString bayer_convert = bayer_strlist[bayer_id];

    bayer_id = ir_position * 4 + bayer_id;
    if (ir_position == utils::IR_NO) {
        QMessageBox message(QMessageBox::NoIcon, "Error", "Not is RGBIR type raw!!!");
        message.exec();
    }

    unsigned int unpacked_size = width * height * sizeof(short);
    std::vector<unsigned char*> p_raw_data_vec;
    std::vector<unsigned char*> p_fill_ch_vec;

    int bayerId = raw_img_item->getBayer().toInt();
    RAW_BITS bits = static_cast<RAW_BITS>(raw_img_item->getBits().toInt());

    for (int i = 0; i < img_cnt; ++i) {
        // ir remove
        int *p_rgbir_img = image_vec[i].get();
        qDebug() << "**********************************the save_file_path is****************** " <<fCaliSlopeB<<"                  "<<fCaliSlopeG<<"      "<<fCaliSlopeR<<"          "<<endl;
        int *p_rgb_img = cali_rgbir_rgbir2rggb(&p_rgbir_img, width, height, bayer_id, fCaliSlopeB , fCaliSlopeG, fCaliSlopeR);
        qDebug() << "**********************************the save_file_path is****************** " <<fCaliSlopeB<<"                  "<<fCaliSlopeG<<"      "<<fCaliSlopeR<<"          "<<endl;
        QByteArray rggb_byte_array = convertRawImageToQByteArray(p_rgb_img, width, height);

        // convert to the show
        p_raw_data_vec.push_back(new unsigned char[unpacked_size]);
        p_fill_ch_vec.push_back(new unsigned char[width * height * sizeof(int)]);
        memcpy(p_raw_data_vec[i], rggb_byte_array.constData(), unpacked_size);
        fill_channels(p_raw_data_vec[i], p_fill_ch_vec[i], width, height, static_cast<RAW_BAYER_ID>(bayerId), bits);
        // save raw file
        if (i == img_cnt -1) {
            QVector<QString> raw_info_vec;
            raw_info_vec.push_back(bayer_strlist[bayerId]);
            raw_info_vec.push_back("frame=" + QString::number(img_cnt));
            QString save_file_path = get_save_raw_path(raw_info_vec, bayer_convert);
            qDebug() << "the save_file_path is the: " << save_file_path;
            saveRawFile(rggb_byte_array, save_file_path, img_cnt);
        }
    }

    // show the image
    if (img_cnt > 0) {
        QImage img(p_fill_ch_vec[0], width, height, QImage::Format_RGB32);
        emit sigShowOutputRawImage(img);
    }

    // release
    for (int i = 0; i < img_cnt; ++i) {
        delete[] p_raw_data_vec[i];
        delete[] p_fill_ch_vec[i];
    }

    QMessageBox::information(this, tr("Conversion"), tr("Convert success"));
}

void RGBIRToRGGBCalibrationPageWidget::OpenSaveFilePath()
{
    QString select_dir_path = QFileDialog::getExistingDirectory(this, "Select Directory", "", QFileDialog::ShowDirsOnly);

    if (select_dir_path.isEmpty()) {
        return;
    }
    else {
        setDir_path(select_dir_path);
        path_edit->setText(select_dir_path);
    }

}

QString RGBIRToRGGBCalibrationPageWidget::get_save_raw_path(QVector<QString> &raw_info_vec, QString bayerid_convert)
{
    QString dir_path;
    QString file_path;
    QString file_name;
    QString parent_folder_name;

    if (!raw_img_item_vec.isEmpty()) {
        RawImageItem *raw_img_item = this->raw_img_item_vec.first();
        QString ori_raw_path = raw_img_item->getFilePath();
        QFileInfo file_info(ori_raw_path);

        if (dir_path.isEmpty()) {
            dir_path = file_info.path();
        }

        QString append_file_name;
        QString file_name_base;
        QString parent_folder = file_info.absolutePath();

        int lastSlashIndex = parent_folder.lastIndexOf('/');
        parent_folder_name = parent_folder.mid(lastSlashIndex + 1);

        file_name = file_info.fileName();
        file_name_base = file_info.baseName();
        QStringList keywords = {"BGRGI", "GRGBI", "GBGRI", "RGBGI", "IGRGB", "IRGBG"};
         qDebug()<<file_name<<endl;

        if ((file_name.contains("BGRGI") || file_name.contains("GRGBI"))
            || file_name.contains("GBGRI") || file_name.contains("RGBGI")
            || file_name.contains("IGRGB") || file_name.contains("IRGBG")) {
            qDebug("success");
            qDebug()<<bayerid_convert<<endl;
            for (const QString& keyword : keywords) {
                file_name_base.replace(keyword, bayerid_convert);
                }
        }

        append_file_name += "_noir";
        append_file_name += ".raw";

        // update the file_name
        file_name = file_name_base + append_file_name;
    }

    if (this->dir_path.isEmpty()) {
        dir_path = QDir::currentPath();
    } else {
        dir_path = this->dir_path;
        dir_path = QDir::cleanPath(dir_path + QDir::separator() + parent_folder_name);
    }
    QDir dir;
    if (!dir.exists(dir_path)) {
        dir.mkpath(dir_path);
    }

    file_path = dir_path + QDir::separator() + file_name;
    path_edit->setText(file_path);

    return file_path;
}

QByteArray RGBIRToRGGBCalibrationPageWidget::convertRawImageToQByteArray(int* rawImage, int width, int height)
{

    int numPixels = width * height;
    QByteArray bggrData;

    bggrData.reserve(numPixels * 2);
    //BGGR
    // int to QByteArray
    for (int i = 0; i < numPixels; ++i) {
        uint8_t low_byte = static_cast<uint8_t>(rawImage[i] & 0xFF); // git low byte
        uint8_t high_byte = static_cast<uint8_t>((rawImage[i] >> 8) & 0xFF); // git high byte
        // add to QByteArray
        bggrData.append(low_byte);      // BGGR BorG
        bggrData.append(high_byte);     // BGGR GorR
    }

    return bggrData;
}

void RGBIRToRGGBCalibrationPageWidget::saveRawFile(QByteArray data, QString &raw_file_name, int frameNum)
{
    QFile raw_file(raw_file_name);
    if (!raw_file.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("Unable to open file"), raw_file.errorString());
        return;
    }

    if (!data.isEmpty()) {
        raw_file.write(data);
        raw_file.close();
    }
}

void RGBIRToRGGBCalibrationPageWidget::handleImportData(QString name, QJsonObject data)
{
    // import blc data
    QString blc_module_name = PREFIX_CALI_STR + getCaliModuleName(CALIBRATION_MODULE_ID::BLC);
    if (data.contains(blc_module_name)) {
        blc_imported = true;
        QJsonArray blc_data_array = data[blc_module_name].toArray();
        for (auto blc_data : blc_data_array) {
            QJsonObject blc_data_obj = blc_data.toObject();
            if (blc_data_obj[KEY_REG_KEY] == "blc_r_offset") {
                blc_r_offset = static_cast<unsigned int>(blc_data_obj[KEY_VALUE].toInt());
            } else if (blc_data_obj[KEY_REG_KEY] == "blc_gr_offset") {
                blc_gr_offset = static_cast<unsigned int>(blc_data_obj[KEY_VALUE].toInt());
            } else if (blc_data_obj[KEY_REG_KEY] == "blc_gb_offset") {
                blc_gb_offset = static_cast<unsigned int>(blc_data_obj[KEY_VALUE].toInt());
            } else if (blc_data_obj[KEY_REG_KEY] == "blc_b_offset") {
                blc_b_offset= static_cast<unsigned int>(blc_data_obj[KEY_VALUE].toInt());
            } else if (blc_data_obj[KEY_REG_KEY] == "blc_r_gain") {
                blc_r_gain = static_cast<float>(blc_data_obj[KEY_VALUE].toInt()/1024.0);
            } else if (blc_data_obj[KEY_REG_KEY] == "blc_gr_gain") {
                blc_gr_gain = static_cast<float>(blc_data_obj[KEY_VALUE].toInt()/1024.0);
            } else if (blc_data_obj[KEY_REG_KEY] == "blc_gb_gain") {
                blc_gb_gain = static_cast<float>(blc_data_obj[KEY_VALUE].toInt()/1024.0);
            } else if (blc_data_obj[KEY_REG_KEY] == "blc_b_gain") {
                blc_b_gain= static_cast<float>(blc_data_obj[KEY_VALUE].toInt()/1024.0);
            }
        }
    }

    // import rgbir data
    QString rgbir_module_name = PREFIX_CALI_STR + getCaliModuleName(CALIBRATION_MODULE_ID::RGBIR);
    if (data.contains(rgbir_module_name)) {
        blc_imported = true;
        QJsonArray rgbir_data_array = data[rgbir_module_name].toArray();
        for (auto rgbir_data : rgbir_data_array) {
            QJsonObject rgbir_data_obj = rgbir_data.toObject();
            if (rgbir_data_obj[KEY_REG_KEY] == "fCaliSlopeB") {
                fCaliSlopeB = rgbir_data_obj[KEY_VALUE].toDouble();
            } else if (rgbir_data_obj[KEY_REG_KEY] == "fCaliSlopeG") {
                fCaliSlopeG = rgbir_data_obj[KEY_VALUE].toDouble();
            } else if (rgbir_data_obj[KEY_REG_KEY] == "fCaliSlopeR") {
                fCaliSlopeR = rgbir_data_obj[KEY_VALUE].toDouble();
            }
        }
    }
}

QJsonArray RGBIRToRGGBCalibrationPageWidget::getCalibrationData()
{
    QJsonArray data_array;
    QJsonObject data;

    data[KEY_REG_KEY] = "fCaliSlopeB";
    data[KEY_VALUE] = fCaliSlopeB;
    data[KEY_VALUE_TYPE] = TYPE_DOUBLE;
    data[KEY_REG_OP_TYPE] = OP_TYPE_AUTO;
    data_array.append(data);

    data[KEY_REG_KEY] = "fCaliSlopeG";
    data[KEY_VALUE] = fCaliSlopeG;
    data[KEY_VALUE_TYPE] = TYPE_DOUBLE;
    data[KEY_REG_OP_TYPE] = OP_TYPE_AUTO;
    data_array.append(data);

    data[KEY_REG_KEY] = "fCaliSlopeR";
    data[KEY_VALUE] = fCaliSlopeR;
    data[KEY_VALUE_TYPE] = TYPE_DOUBLE;
    data[KEY_REG_OP_TYPE] = OP_TYPE_AUTO;
    data_array.append(data);

    return data_array;
}

void RGBIRToRGGBCalibrationPageWidget::setDir_path(QString select_dir_path)
{
    this->dir_path = select_dir_path;
}

QJsonObject RGBIRToRGGBCalibrationPageWidget::GetModuleObjectToNetworkJsonrpc()
{
    qDebug("RGBIRCalibrationPageWidget::GetModuleObjectToNetworkJsonrpc.");
    QJsonObject module_obj;
    QStringList struct_list = getStructList();
    QMap<QString, QString> struct_set_api_map = getStructSetApiMap();
    CALIBRATION_MODULE_ID id = static_cast<CALIBRATION_MODULE_ID>(property("cali_id").toInt());
    QString module_name = getCaliModuleName(id);

    NetworkClientHelper::getInstance()->clearIdInfoJsonrpc();
    int jsonrpc_id = GlobalData::getInstance()->getJsonrpcId();
    QString struct_id = struct_list[0];
    QJsonObject send_obj;
    QJsonObject params_obj;

    params_obj["CalibrationSlope"] = getCalibrationData();
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_METHOD] = struct_set_api_map[struct_id];
    send_obj[MSG_KEY_ID] = jsonrpc_id;
    send_obj[MSG_KEY_PARAMS] = params_obj;
    module_obj[struct_id] = send_obj;

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_WRITE_RGBIR2RGGB_CALIBRATION;
    id_info[MSG_KEY_MODULE] = module_name;
    id_info[MSG_KEY_STRUCT] = struct_id;
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(jsonrpc_id, id_info);

    return module_obj;
}
