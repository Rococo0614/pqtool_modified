#include "awb_calibration_page_widget.hpp"
#include "otp_pdaf_cali.h"
#include "network_client_helper.hpp"
#include <QMessageBox>
#include <QFileDialog>


static bool compareData(GainItem *item1, GainItem *item2)
{
    if (item1->getTemp() < item2->getTemp()) {
        return true;
    }

    return false;
}

AWBCalibrationPageWidget::AWBCalibrationPageWidget(QWidget *parent) : BaseCalibrationPageWidget(static_cast<int>(CALIBRATION_MODULE_ID::AWB), parent)
{
    setAcceptScene(SCENE::UNIFORM_COLOR);
    initUI();
    initEvent();
    retranslateUi();
}

void AWBCalibrationPageWidget::ClearData()
{
    for (int i=0; i<rawItem.size(); i++) {
        if (rawItem[i]) {
            delete rawItem[i];
        }
    }
    rawItem.clear();
    for (int i=0; i<gain_item.size(); i++) {
        if (gain_item[i]) {
            delete gain_item[i];
        }
    }
    gain_item.clear();
    raw_img_item_vec.clear();
    quickMode = false;
    showMessage = true;
    blc_imported = false;
    gain_select_item.clear();
    cali_status = CALIBRATION_STATUS::READY;
}

void AWBCalibrationPageWidget::initUI()
{
    QVBoxLayout *awb_layout = new QVBoxLayout;

    QHBoxLayout *chart_layout = new QHBoxLayout;
    CHAR_PROPERTY property;
    property.title = "White Pixel Distrubition";
    property.title_x = "R/G";
    property.title_y = "B/G";
    property.rang_x[0] = 0;
    property.rang_x[1] = 2;
    property.rang_y[0] = 0;
    property.rang_y[1] = 2;
    pixel_curve = new ChartWidget(property);
    pixel_curve->setSeriesVisible(1, false);

    CHAR_PROPERTY property1;
    property1.title = "Planckian Curve";
    property1.title_x = "R/G";
    property1.title_y = "B/G";
    property1.rang_x[0] = 0;
    property1.rang_x[1] = 2;
    property1.rang_y[0] = 0;
    property1.rang_y[1] = 2;
    planck_curve = new ChartWidget(property1);

    chart_layout->addWidget(pixel_curve);
    chart_layout->addWidget(planck_curve);

    QGridLayout *opt_layout = new QGridLayout;
    import_raw_btn = new QPushButton("Import Selected RAW");
    import_raw_btn->setFixedWidth(160);
    connect(import_raw_btn, &QPushButton::clicked, [=]() {
        emit sigImportRaw();
    });
    import_blc_btn = new QPushButton("Import BLC Data");
    import_blc_btn->setFixedWidth(160);
    connect(import_blc_btn, &QPushButton::clicked, [=]() {
        BaseCalibrationPageWidget::handleImportData();
    });
    write_btn = new QPushButton("Write Data");
    write_btn->setFixedWidth(120);
    connect(write_btn, &QPushButton::clicked, this, std::bind(&BaseCalibrationPageWidget::writeCalibrationData, this, true));
    import_btn = new QPushButton("Import Data");
    import_btn->setFixedWidth(120);
    connect(import_btn, &QPushButton::clicked, [=]() {
        handleImportAwbData();
    });
    export_btn = new QPushButton("Export Data");
    export_btn->setFixedWidth(120);
    connect(export_btn, &QPushButton::clicked, [=]() {
        handleExportAwbData("");
    });
    calibration_btn = new QPushButton("Calibration");
    calibration_btn->setFixedWidth(120);
    connect(calibration_btn, &QPushButton::clicked, this, &AWBCalibrationPageWidget::calibration);

    QLabel *rgain_label = new QLabel("R Gain:");
    rgain_label->setFixedWidth(60);
    rgain_edit = new QLineEdit;
    rgain_edit->setText("0");
    rgain_edit->setFixedWidth(100);
    rgain_edit->setReadOnly(true);
    QLabel *bgain_label = new QLabel("B Gain:");
    bgain_label->setFixedWidth(60);
    bgain_edit = new QLineEdit;
    bgain_edit->setText("0");
    bgain_edit->setFixedWidth(100);
    bgain_edit->setReadOnly(true);
    QLabel *p1_label = new QLabel("P1:");
    p1_label->setFixedWidth(60);
    p1_edit = new QLineEdit;
    p1_edit->setText("0");
    p1_edit->setFixedWidth(150);
    p1_edit->setReadOnly(true);
    QLabel *p2_label = new QLabel("P2:");
    p2_label->setFixedWidth(60);
    p2_edit = new QLineEdit;
    p2_edit->setText("0");
    p2_edit->setFixedWidth(150);
    p2_edit->setReadOnly(true);
    QLabel *q_label = new QLabel("Q:");
    q_label->setFixedWidth(60);
    q_edit = new QLineEdit;
    q_edit->setText("0");
    q_edit->setFixedWidth(150);
    q_edit->setReadOnly(true);
    QLabel *a_label = new QLabel("A:");
    a_label->setFixedWidth(60);
    a_edit = new QLineEdit;
    a_edit->setText("0");
    a_edit->setFixedWidth(150);
    a_edit->setReadOnly(true);
    QLabel *b_label = new QLabel("B:");
    b_label->setFixedWidth(60);
    b_edit = new QLineEdit;
    b_edit->setText("0");
    b_edit->setFixedWidth(150);
    b_edit->setReadOnly(true);
    QLabel *c_label = new QLabel("C:");
    c_label->setFixedWidth(60);
    c_edit = new QLineEdit;
    c_edit->setText("0");
    c_edit->setFixedWidth(150);
    c_edit->setReadOnly(true);

    opt_layout->addWidget(import_blc_btn, 0, 0);
    opt_layout->addWidget(import_btn, 0, 1);
    opt_layout->addWidget(rgain_label, 0, 2);
    opt_layout->addWidget(rgain_edit, 0, 3);
    opt_layout->addWidget(p1_label, 0, 4);
    opt_layout->addWidget(p1_edit, 0, 5);
    opt_layout->addWidget(p2_label, 0, 6);
    opt_layout->addWidget(p2_edit, 0, 7);
    opt_layout->addWidget(q_label, 0, 8);
    opt_layout->addWidget(q_edit, 0, 9);
    opt_layout->addWidget(calibration_btn, 0, 10);
    opt_layout->addWidget(import_raw_btn, 1, 0);
    opt_layout->addWidget(export_btn, 1, 1);
    opt_layout->addWidget(bgain_label, 1, 2);
    opt_layout->addWidget(bgain_edit, 1, 3);
    opt_layout->addWidget(a_label, 1, 4);
    opt_layout->addWidget(a_edit, 1, 5);
    opt_layout->addWidget(b_label, 1, 6);
    opt_layout->addWidget(b_edit, 1, 7);
    opt_layout->addWidget(c_label, 1, 8);
    opt_layout->addWidget(c_edit, 1, 9);
    opt_layout->addWidget(write_btn, 1, 10);
    opt_layout->setHorizontalSpacing(20);
    opt_layout->setAlignment(Qt::AlignCenter);

    gain_list_widget = new CalibrationGainListWidget;
    gain_list_widget->setFixedHeight(200);

    awb_layout->addLayout(chart_layout);
    awb_layout->addLayout(opt_layout);
    awb_layout->addWidget(gain_list_widget);
    main_layout->addLayout(awb_layout);

}

void AWBCalibrationPageWidget::initEvent()
{
    connect(this, &BaseCalibrationPageWidget::sigImportData, this, &AWBCalibrationPageWidget::handleImportData);
    connect(this, &BaseCalibrationPageWidget::sigImportAwbRaw, this, &AWBCalibrationPageWidget::handleImportAwbRaw);
    connect(gain_list_widget, &CalibrationGainListWidget::sigDelPoint, this, &AWBCalibrationPageWidget::handleDelPoint);

    shared_ptr<NetworkClientHelper> network_helper = NetworkClientHelper::getInstance();
    connect(network_helper.get(), &NetworkClientHelper::sigReceiveAWBCalibration,
        this, &AWBCalibrationPageWidget::handleReceiveAWBCalibration);
}

void AWBCalibrationPageWidget::handleDelPoint(int idx)
{
    pixel_curve->handleDelScatterPoint(idx);
}

bool AWBCalibrationPageWidget::calibration()
{
    qDebug("AWBCalibrationPageWidget::calibration:%d",quickMode);
    if (showMessage) {
        emit sigChangeCaliStatus(CALIBRATION_STATUS::CALIBRATION);
    }

    QList<GainItem *> list;
    if (quickMode) {
        list = gain_item;
    } else {
        list = gain_list_widget->getGainList();
    }
    if ((list.size() < 3) || (list.size() > 7)) {
        if (showMessage) {
            emit sigChangeCaliStatus(CALIBRATION_STATUS::CAL_ERROR);
            QMessageBox message(QMessageBox::NoIcon, "Error", "Please import 3~7 RAW!");
            message.exec();
        }
        return false;
    }

    for (int i = 0; i < list.size(); i++) {
        int temp = list[i]->getTemp();
        if (temp == 0) {
            if (showMessage) {
                emit sigChangeCaliStatus(CALIBRATION_STATUS::CAL_ERROR);
                QMessageBox message(QMessageBox::NoIcon, "Error", "Please update \"In Temp.(K)\" columns!");
                message.exec();
            }
            return false;
        }
    }

    QList<GainItem *> list_sel;
    if (quickMode) {
        list_sel = gain_select_item;
    } else {
        list_sel = gain_list_widget->getSelectGainList();
    }
    if (list_sel.size() != 3) {
        if (showMessage) {
            emit sigChangeCaliStatus(CALIBRATION_STATUS::CAL_ERROR);
            QMessageBox message(QMessageBox::NoIcon, "Error", "Must select 3 items");
            message.exec();
        }
        return false;
    }

    cali_status = CALIBRATION_STATUS::CALIBRATION;

    int num = list.size();
    float goldenR, goldenB;
    float r_gain[MAX_CALIB_PT_NUM];
    float b_gain[MAX_CALIB_PT_NUM];
    float temp_k[MAX_CALIB_PT_NUM];
    unsigned char wgt[MAX_CALIB_PT_NUM];

    for (int i = 0; i < num; i++) {
        r_gain[i] = list[i]->getRgain();
        b_gain[i] = list[i]->getBgain();
        temp_k[i] = list[i]->getTemp();
        wgt[i] = list[i]->getWeight();
    }

    std::sort(list_sel.begin(), list_sel.end(), compareData);
    goldenR = list_sel[1]->getRgain();
    goldenB = list_sel[1]->getBgain();
    rgain[0] = list_sel[0]->getRgain();
    bgain[0] = list_sel[0]->getBgain();
    tempK[0] = list_sel[0]->getTemp();
    rgain[1] = list_sel[1]->getRgain();
    bgain[1] = list_sel[1]->getBgain();
    tempK[1] = list_sel[1]->getTemp();
    rgain[2] = list_sel[2]->getRgain();
    bgain[2] = list_sel[2]->getBgain();
    tempK[2] = list_sel[2]->getTemp();

    StaticWB[0] = list_sel[1]->getRgain();
    StaticWB[1] = list_sel[1]->getGgain();
    StaticWB[2] = list_sel[1]->getGgain();
    StaticWB[3] = list_sel[1]->getBgain();

    cali_awb_BuildCurve(goldenR, goldenB, r_gain, b_gain, temp_k, wgt, num, paraK);
    if (!quickMode) {
        rgain_edit->setText(QString::number(goldenR));
        bgain_edit->setText(QString::number(goldenB));
        p1_edit->setText(QString::number(paraK[0]));
        p2_edit->setText(QString::number(paraK[1]));
        q_edit->setText(QString::number(paraK[2]));
        a_edit->setText(QString::number(paraK[3]));
        b_edit->setText(QString::number(paraK[4]));
        c_edit->setText(QString::number(paraK[5]));

        planck_curve->clearSeries(0);
        planck_curve->resetAxisRang(0, 2, 0, 2);

        float pt_x[200], pt_y[200];
        int i;
        cali_awb_getCurve(pt_y, rgain, bgain);
        for (i = 0; i < 200 ; i++) {
            pt_x[i] = i / 100.0;
        }
        planck_curve->handleUpdateLinePoints(i, pt_x, pt_y);

        for (int n = 0; n < num; n++) {
            float x, y;
            if (r_gain[n] > 0) {
                x = 1024.0/r_gain[n];
            } else {
                x = 0;
            }
            if (b_gain[n] > 0) {
                y = 1024.0/b_gain[n];
            } else {
                y = 0;
            }
            planck_curve->handleAddScatterPoint(x, y);
        }

        emit sigChangeCaliStatus(CALIBRATION_STATUS::READY);
    }
    cali_status = CALIBRATION_STATUS::READY;

    return true;
}

QJsonArray AWBCalibrationPageWidget::getCalibrationData()
{
    QJsonArray export_data;
    QJsonObject data;

    data[KEY_REG_KEY] = "CT";
    data[KEY_VALUE] = utils::arrayPtrToJsonArray(tempK, 3);
    data[KEY_VALUE_TYPE] = TYPE_ARRAY;
    data[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
    export_data.append(data);

    data[KEY_REG_KEY] = "R Gain";
    data[KEY_VALUE] = utils::arrayPtrToJsonArray(rgain, 3);
    data[KEY_VALUE_TYPE] = TYPE_ARRAY;
    data[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
    export_data.append(data);

    data[KEY_REG_KEY] = "B Gain";
    data[KEY_VALUE] = utils::arrayPtrToJsonArray(bgain, 3);
    data[KEY_VALUE_TYPE] = TYPE_ARRAY;
    data[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
    export_data.append(data);

    data[KEY_REG_KEY] = "RefColorTemp";
    data[KEY_VALUE] = tempK[1];
    data[KEY_VALUE_TYPE] = TYPE_INT;
    data[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
    export_data.append(data);

    data[KEY_REG_KEY] = "StaticWB";
    data[KEY_VALUE] = utils::arrayPtrToJsonArray(StaticWB, 4);
    data[KEY_VALUE_TYPE] = TYPE_ARRAY;
    data[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
    export_data.append(data);

    data[KEY_REG_KEY] = "CurvePara";
    data[KEY_VALUE] = utils::arrayPtrToJsonArray(paraK, 6);
    data[KEY_VALUE_TYPE] = TYPE_ARRAY;
    data[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
    export_data.append(data);

    return export_data;
}

void AWBCalibrationPageWidget::retranslateUi()
{
    BaseCalibrationPageWidget::retranslateUi();
}

void AWBCalibrationPageWidget::handleImportData(QString name, QJsonObject data)
{
    qDebug()<<"AWBCalibrationPageWidget import file: "<<name;

    // import blc data
    QString blc_module_name = PREFIX_CALI_STR + getCaliModuleName(CALIBRATION_MODULE_ID::BLC);
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
}

void AWBCalibrationPageWidget::handleImportAwbData()
{
    int num = 0;
    unsigned short r_gain[MAX_CALIB_PT_NUM];
    unsigned short g_gain[MAX_CALIB_PT_NUM];
    unsigned short b_gain[MAX_CALIB_PT_NUM];
    unsigned short temp_k[MAX_CALIB_PT_NUM];
    int ki[MAX_CALIB_PT_NUM];
    int wgt[MAX_CALIB_PT_NUM];
    QString path[MAX_CALIB_PT_NUM];

    QString import_file_path = QFileDialog::getOpenFileName(this, tr("Open Data File"), "", "*.json");
    QJsonObject data;
    QString error_msg;
    if (utils::loadJson(import_file_path, data, error_msg) != 0) {
        QMessageBox::critical(this, "Error", error_msg, QMessageBox::Ok);
        return;
    }

    QString awb_module_name = PREFIX_CALI_STR + getCaliModuleName(CALIBRATION_MODULE_ID::AWB);
    if (data.contains(awb_module_name)) {
        QJsonArray awb_data_array = data[awb_module_name].toArray();
        for (auto awb_data : awb_data_array) {
            QJsonObject awb_data_obj = awb_data.toObject();
            if (awb_data_obj[KEY_REG_KEY] == "path") {
                QJsonArray value_array = awb_data_obj[KEY_VALUE].toArray();
                num = value_array.size();
                for (int i = 0; i < value_array.size(); i++) {
                    if (i > MAX_CALIB_PT_NUM) break;
                    path[i] = value_array[i].toString();
                }
            } else if (awb_data_obj[KEY_REG_KEY] == "ki") {
                QJsonArray value_array = awb_data_obj[KEY_VALUE].toArray();
                for (int i = 0; i < value_array.size(); i++) {
                    if (i > MAX_CALIB_PT_NUM) break;
                    ki[i] = value_array[i].toInt();
                }
            } else if (awb_data_obj[KEY_REG_KEY] == "weight") {
                QJsonArray value_array = awb_data_obj[KEY_VALUE].toArray();
                for (int i = 0; i < value_array.size(); i++) {
                    if (i > MAX_CALIB_PT_NUM) break;
                    wgt[i] = value_array[i].toInt();
                }
            } else if (awb_data_obj[KEY_REG_KEY] == "temp") {
                QJsonArray value_array = awb_data_obj[KEY_VALUE].toArray();
                for (int i = 0; i < value_array.size(); i++) {
                    if (i > MAX_CALIB_PT_NUM) break;
                    temp_k[i] = value_array[i].toInt();
                }
            } else if (awb_data_obj[KEY_REG_KEY] == "rgain") {
                QJsonArray value_array = awb_data_obj[KEY_VALUE].toArray();
                for (int i = 0; i < value_array.size(); i++) {
                    if (i > MAX_CALIB_PT_NUM) break;
                    r_gain[i] = value_array[i].toInt();
                }
            } else if (awb_data_obj[KEY_REG_KEY] == "ggain") {
                QJsonArray value_array = awb_data_obj[KEY_VALUE].toArray();
                for (int i = 0; i < value_array.size(); i++) {
                    if (i > MAX_CALIB_PT_NUM) break;
                    g_gain[i] = value_array[i].toInt();
                }
            } else if (awb_data_obj[KEY_REG_KEY] == "bgain") {
                QJsonArray value_array = awb_data_obj[KEY_VALUE].toArray();
                for (int i = 0; i < value_array.size(); i++) {
                    if (i > MAX_CALIB_PT_NUM) break;
                    b_gain[i] = value_array[i].toInt();
                }
            }
        }
    } else {
        QMessageBox::critical(this, "Error", "This file doesn't contain awb calibration data.", QMessageBox::Ok);
        return;
    }

    gain_list_widget->clearTable();
    pixel_curve->clearSeries(0);

    for (int i = 0; i < num; i++) {
        GAIN_INFO info;
        info.path = path[i];
        info.select = ki[i];
        info.weight = wgt[i];
        info.temp = temp_k[i];
        info.Rgain = r_gain[i];
        info.Ggain = g_gain[i];
        info.Bgain = b_gain[i];
        GainItem * item = new GainItem(info);
        bool ret = gain_list_widget->addTableItem(item);
        if (!ret) {
            return;
        }

        float pt_x,pt_y;
        if (r_gain[i] > 0) {
            pt_x = 1024.0/r_gain[i];
        } else {
            pt_x = 0;
        }
        if (b_gain[i] > 0) {
            pt_y = 1024.0/b_gain[i];
        } else {
            pt_y = 0;
        }
        pixel_curve->handleAddScatterPoint(pt_x, pt_y);
    }

}

void AWBCalibrationPageWidget::handleExportAwbData(QString outpath)
{
    QString file_path;
    if (outpath.isEmpty()) {
        file_path = QFileDialog::getSaveFileName(this, tr("Open File"), "", "*.json");
    } else {
        file_path = outpath;
    }
    QJsonObject data,tempData;
    QJsonArray items = getCalibrationData();

    QList<GainItem *> list;
    if (quickMode) {
        list = gain_item;
    } else {
        list = gain_list_widget->getGainList();
    }
    int num = list.size();
    unsigned short r_gain[MAX_CALIB_PT_NUM];
    unsigned short g_gain[MAX_CALIB_PT_NUM];
    unsigned short b_gain[MAX_CALIB_PT_NUM];
    unsigned short temp_k[MAX_CALIB_PT_NUM];
    int ki[MAX_CALIB_PT_NUM];
    int wgt[MAX_CALIB_PT_NUM];
    QString path[MAX_CALIB_PT_NUM];

    for (int i = 0; i < num; i++) {
        r_gain[i] = list[i]->getRgain();
        g_gain[i] = list[i]->getGgain();
        b_gain[i] = list[i]->getBgain();
        temp_k[i] = list[i]->getTemp();
        ki[i] = list[i]->getSelected();
        wgt[i] = list[i]->getWeight();
        path[i] = list[i]->getFilePath();
    }

    tempData[KEY_REG_KEY] = "path";
    tempData[KEY_VALUE] = utils::arrayPtrToJsonArray(path, num);
    tempData[KEY_VALUE_TYPE] = TYPE_ARRAY;
    tempData[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
    items.append(tempData);

    tempData[KEY_REG_KEY] = "ki";
    tempData[KEY_VALUE] = utils::arrayPtrToJsonArray(ki, num);
    tempData[KEY_VALUE_TYPE] = TYPE_ARRAY;
    tempData[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
    items.append(tempData);

    tempData[KEY_REG_KEY] = "weight";
    tempData[KEY_VALUE] = utils::arrayPtrToJsonArray(wgt, num);
    tempData[KEY_VALUE_TYPE] = TYPE_ARRAY;
    tempData[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
    items.append(tempData);

    tempData[KEY_REG_KEY] = "temp";
    tempData[KEY_VALUE] = utils::arrayPtrToJsonArray(temp_k, num);
    tempData[KEY_VALUE_TYPE] = TYPE_ARRAY;
    tempData[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
    items.append(tempData);

    tempData[KEY_REG_KEY] = "rgain";
    tempData[KEY_VALUE] = utils::arrayPtrToJsonArray(r_gain, num);
    tempData[KEY_VALUE_TYPE] = TYPE_ARRAY;
    tempData[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
    items.append(tempData);

    tempData[KEY_REG_KEY] = "ggain";
    tempData[KEY_VALUE] = utils::arrayPtrToJsonArray(g_gain, num);
    tempData[KEY_VALUE_TYPE] = TYPE_ARRAY;
    tempData[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
    items.append(tempData);

    tempData[KEY_REG_KEY] = "bgain";
    tempData[KEY_VALUE] = utils::arrayPtrToJsonArray(b_gain, num);
    tempData[KEY_VALUE_TYPE] = TYPE_ARRAY;
    tempData[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
    items.append(tempData);

    CALIBRATION_MODULE_ID id = static_cast<CALIBRATION_MODULE_ID>(property("cali_id").toInt());
    data[PREFIX_CALI_STR + getCaliModuleName(id)] = items;
    utils::saveJson(file_path, data);
}

bool AWBCalibrationPageWidget::handleImportAwbRaw()
{
    if (!blc_imported) {
        if (showMessage) {
            QMessageBox message(QMessageBox::NoIcon, "Error", "Must import BLC data first!");
            message.exec();
        }
        return false;
    }

    using utils::RAW_BAYER_ID;
    using utils::RAW_BITS;

    RawImageItem * raw_img_item = this->raw_img_item_vec.first();
    int order = raw_img_item->getBayer().toInt();
    int width = raw_img_item->getWidth();
    int height = raw_img_item->getHeight();
    bool wdr_mode = raw_img_item->getFrameMode() == FRAME_MODE::WDR ? true : false;

    QString raw_path = raw_img_item->getFilePath();
    unique_ptr<int []> image = utils::readRawIntData(raw_path, width, height, wdr_mode);
    if (image == nullptr) {
        return false;
    }

    cali_status = CALIBRATION_STATUS::CALIBRATION;

    // calibration params
    int NumKnotX = 37,
        NumKnotY = 37;

    for (int i = 0; i < RGB_ARRAY_SIZE; i++) {
        lsc_r_gain[i] = 512;
        lsc_b_gain[i] = 512;
        lsc_g_gain[i] = 512;
    }

    //Set ROI
    RECT_S rect = raw_img_item->getRect();
    qDebug("x = %d, y = %d, w = %d, h = %d", rect.x, rect.y, rect.w, rect.h);
    cali_awb_setROI(rect.x, rect.y, rect.w, rect.h);

    // calibration
    int *raw_image = image.get();
    if (wdr_mode) width /= 2;
    cali_awb_calc(&raw_image, width, height, order, NumKnotX, NumKnotY, lsc_r_gain.get(), lsc_g_gain.get(), lsc_b_gain.get(),
                  blc_r_offset, blc_gr_offset, blc_gb_offset, blc_b_offset, blc_r_gain, blc_gr_gain, blc_gb_gain, blc_b_gain,
                  &wb_r_gain, &wb_g_gain, &wb_b_gain);

    GAIN_INFO info;
    info.path = raw_path;
    info.select = 0;
    info.weight = 1;
    info.temp = 0;
    info.Rgain = wb_r_gain;
    info.Ggain = wb_g_gain;
    info.Bgain = wb_b_gain;
    GainItem * item = new GainItem(info);

    if (quickMode) {
        gain_item.append(item);
    } else {
        bool ret = gain_list_widget->addTableItem(item);
        if (!ret) {
            return false;
        }

        float pt_x,pt_y;
        if (wb_r_gain > 0) {
            pt_x = 1024.0/wb_r_gain;
        } else {
            pt_x = 0;
        }
        if (wb_b_gain > 0) {
            pt_y = 1024.0/wb_b_gain;
        } else {
            pt_y = 0;
        }
        pixel_curve->handleAddScatterPoint(pt_x, pt_y);
    }

    cali_status = CALIBRATION_STATUS::READY;

    return true;
}

bool AWBCalibrationPageWidget::checkShallExport()
{
    if (quickMode) {
        if (gain_select_item.size() == 3)
            return true;
    } else {
        if (gain_list_widget->getSelectGainList().size() == 3)
            return true;
    }

    return false;
}

int AWBCalibrationPageWidget::readCalibrationData(bool showMessage)
{
    qDebug("AWBCalibrationPageWidget::readModuleFromNetworkJsonrpc");
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
        id_info[MSG_KEY_CMD] = CMD_READ_AWB_CALIBRATION;
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

void AWBCalibrationPageWidget::handleReceiveAWBCalibration(int id)
{
    qDebug("handleReceiveAWBCalibration.");
    QJsonObject data_obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    if (!data_obj.isEmpty()) {
        QJsonObject result_obj = data_obj[MSG_KEY_RESULT].toObject();
        if (!result_obj.isEmpty()) {
            QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();
            QJsonObject info_obj = data_obj[MSG_KEY_ID_INFO].toObject();
            QString struct_id = info_obj[MSG_KEY_STRUCT].toString();

            QJsonObject mapping_obj;
            QJsonObject param_data;
            utils::parseParams(struct_id, params_obj, mapping_obj, param_data);
            params.insert(struct_id, param_data);
        } else {
            qDebug("handleReceiveAWBCalibration. Error: result_obj is empty.");
        }
    } else {
        qDebug("handleReceiveAWBCalibration. Error: data_obj is empty.");
    }
}

QJsonObject AWBCalibrationPageWidget::GetModuleObjectToNetworkJsonrpc()
{
    qDebug("AWBCalibrationPageWidget::GetModuleObjectToNetworkJsonrpc.");
    QJsonObject module_obj;
    QStringList struct_list = getStructList();
    QMap<QString, QString> struct_set_api_map = getStructSetApiMap();
    CALIBRATION_MODULE_ID id = static_cast<CALIBRATION_MODULE_ID>(property("cali_id").toInt());
    QString module_name = getCaliModuleName(id);

    QJsonObject obj_0 = params.value(struct_list[0]);
    QJsonArray ct = utils::arrayPtrToJsonArray(tempK, 3);
    QJsonArray rg = utils::arrayPtrToJsonArray(rgain, 3);
    QJsonArray bg = utils::arrayPtrToJsonArray(bgain, 3);
    obj_0.insert("ISP_AWB_Calibration_Gain_S.u16ColorTemperature", ct);
    obj_0.insert("ISP_AWB_Calibration_Gain_S.u16AvgRgain", rg);
    obj_0.insert("ISP_AWB_Calibration_Gain_S.u16AvgBgain", bg);
    QJsonObject params_obj_0;
    QStringList keys = obj_0.keys();
    for (int i = 0; i < keys.size(); i++) {
        QString path = keys[i];
        QJsonValue value = obj_0[path];
        QStringList path_split = path.split(QLatin1Char('.'));
        if (path_split.count() < 2) {
            qDebug("[Error] Invalid path: %s.", qUtf8Printable(path));
            continue;
        } else {
            QJsonObject mapping_obj;
            path_split.removeFirst();
            utils::createParamJsonrpc(path_split, params_obj_0, value, mapping_obj);
        }
    }

    NetworkClientHelper::getInstance()->clearIdInfoJsonrpc();
    int jsonrpc_id = GlobalData::getInstance()->getJsonrpcId();
    QString struct_id_0 = struct_list[0];
    QJsonObject send_obj_0;
    send_obj_0[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj_0[MSG_KEY_METHOD] = struct_set_api_map[struct_id_0];
    send_obj_0[MSG_KEY_ID] = jsonrpc_id;
    send_obj_0[MSG_KEY_PARAMS] = params_obj_0;
    module_obj[struct_id_0] = send_obj_0;

    QJsonObject id_info_0;
    id_info_0[MSG_KEY_CMD] = CMD_WRITE_AWB_CALIBRATION;
    id_info_0[MSG_KEY_MODULE] = module_name;
    id_info_0[MSG_KEY_STRUCT] = struct_id_0;
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(jsonrpc_id, id_info_0);

    QJsonObject obj_1 = params.value(struct_list[1]);
    QJsonArray wb = utils::arrayPtrToJsonArray(StaticWB, 4);
    QJsonArray pk = utils::arrayPtrToJsonArray(paraK, 6);
    obj_1.insert("ISP_WB_ATTR_S.stAuto.u16RefColorTemp", tempK[1]);
    obj_1.insert("ISP_WB_ATTR_S.stAuto.au16StaticWB", wb);
    obj_1.insert("ISP_WB_ATTR_S.stAuto.as32CurvePara", pk);
    QJsonObject params_obj_1;
    QStringList keys1 = obj_1.keys();
    for (int i = 0; i < keys1.size(); i++) {
        QString path = keys1[i];
        QJsonValue value = obj_1[path];
        QStringList path_split = path.split(QLatin1Char('.'));
        if (path_split.count() < 2) {
            qDebug("[Error] Invalid path: %s.", qUtf8Printable(path));
            continue;
        } else {
            QJsonObject mapping_obj;
            path_split.removeFirst();
            utils::createParamJsonrpc(path_split, params_obj_1, value, mapping_obj);
        }
    }

    jsonrpc_id = GlobalData::getInstance()->getJsonrpcId();
    QString struct_id_1 = struct_list[1];
    QJsonObject send_obj_1;
    send_obj_1[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj_1[MSG_KEY_METHOD] = struct_set_api_map[struct_id_1];
    send_obj_1[MSG_KEY_ID] = jsonrpc_id;
    send_obj_1[MSG_KEY_PARAMS] = params_obj_1;
    module_obj[struct_id_1] = send_obj_1;

    QJsonObject id_info_1;
    id_info_1[MSG_KEY_CMD] = CMD_WRITE_AWB_CALIBRATION;
    id_info_1[MSG_KEY_MODULE] = module_name;
    id_info_1[MSG_KEY_STRUCT] = struct_id_1;
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(jsonrpc_id, id_info_1);

    return module_obj;
}

bool AWBCalibrationPageWidget::selectItems()
{
    int size = gain_item.size();

    std::sort(gain_item.begin(), gain_item.end(), compareData);

    if (size < 3 || size > 7) {
        return false;
    } else if (size == 3) {
        gain_select_item = gain_item;
    } else {
        gain_select_item.append(gain_item[0]);
        gain_select_item.append(gain_item[size/2]);
        gain_select_item.append(gain_item[size-1]);
    }

    for (int i = 0; i < gain_select_item.size(); i++) {
        gain_select_item[i]->setSelected(true);
    }

    return true;
}

int AWBCalibrationPageWidget::handleQuickCalibration()
{
    if (rawItem.isEmpty()) {
        ClearData();
        return -1;
    }

    if (!blc_imported) {
        ClearData();
        return -1;
    }

    quickMode = true;
    for (int i=0; i<rawItem.size(); i++) {
        RECT_S roi_size;
        int width =  rawItem[i]->getWidth();
        int height =  rawItem[i]->getHeight();
        bool wdr_mode = rawItem[i]->getFrameMode() == FRAME_MODE::WDR ? true : false;
        if (wdr_mode) width /= 2;
        roi_size.w = (int)((width * (float)0.125) / 2) * 2;
        roi_size.h = (int)((height * (float)0.1667) / 2) * 2;
        roi_size.x = (width - roi_size.w) / 2;
        roi_size.y = (height - roi_size.h) / 2;
        rawItem[i]->setRect(roi_size);
        raw_img_item_vec.clear();
        raw_img_item_vec.append(rawItem[i]);
        bool status = handleImportAwbRaw();
        if (status) {
            QString path = rawItem[i]->getFilePath();
            QStringList strList = path.split("/");
            QString dir_name = strList[strList.size()-2];
            int startIdx = dir_name.indexOf("(") + 1;
            int endIdx = dir_name.indexOf("K");
            int ct = dir_name.mid(startIdx, endIdx-startIdx).toInt();
            qDebug()<<path;
            gain_item[i]->setTemp(ct);
        } else {
            ClearData();
            return -1;
        }
    }

    if (!selectItems()) {
        ClearData();
        return -1;
    }

    if (!calibration()) {
        ClearData();
        return -1;
    }

    QString path = rawItem[0]->getFilePath();
    QStringList strList = path.split("/");
    int len = strList[strList.size()-1].size() + strList[strList.size()-2].size() + 1;
    int index = path.size() - len;
    path.remove(index, len);
    path += "awb.json";
    handleExportAwbData(path);

    int ret = writeCalibrationData(false);
    ClearData();
    if (ret < 0) {
        return -2;
    }

    return 0;
}

int AWBCalibrationPageWidget::QuickCalibrationLoadData(QString path)
{
    ClearData();
    QDir dir(path);
    QStringList lights = {"D75", "D65", "D50", "TL84", "A"};
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for(int i = 0; i < folder_list.size(); i++) {
        QString dir_name = folder_list.at(i).fileName();
        for (int j=0; j<lights.size(); j++) {
            if (dir_name.contains(lights[j])) {
                QString dirPath = path + "/" + dir_name;
                QStringList fileList;
                utils::findFiles(dirPath, ".raw", fileList, false);
                if (!fileList.isEmpty()) {
                    qDebug() << fileList[0];
                    RawImageItem *item = new RawImageItem(fileList[0], SCENE::UNIFORM_COLOR);
                    rawItem.append(item);
                }
                break;
            }
        }
        if (rawItem.size() >= 7) {
            break;
        }
    }

    if (!rawItem.isEmpty()) {
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
        ClearData();
        return -1;
    }

    int ret = loadBlcData(path);
    if (ret < 0) {
        ClearData();
        return -1;
    }

    return 0;
}

int AWBCalibrationPageWidget::loadBlcData(QString path)
{
    blc_imported = false;
    QString blc_json_file = path + "/../BLC/ISO100/blc_iso=100.json";
    QJsonObject data;
    QString error_msg;

    if (utils::loadJson(blc_json_file, data, error_msg) != 0) {
        return -1;
    }

    handleImportData(blc_json_file, data);

    return 0;
}

void AWBCalibrationPageWidget::HandleCalibrationRequst(QJsonObject params, RawConfigItem config)
{
    QString path;
    int ret = 0;
    if (params.contains("path") && !(params["path"].toString().isEmpty())) {
        path = params["path"].toString();
    }

    if (path.isEmpty()) {
        emit sigCaliStatus(CALIBRATION_MODULE_ID::AWB, CALIBRATION_STATUS::CAL_ERROR, "Image path is empty!");
        return;
    }

    QDir dir(path);
    if (!dir.exists()) {
        emit sigCaliStatus(CALIBRATION_MODULE_ID::AWB, CALIBRATION_STATUS::CAL_ERROR, "Image path not exists!");
        return;
    }

    if (cali_status == CALIBRATION_STATUS::CALIBRATION) {
        emit sigCaliStatus(CALIBRATION_MODULE_ID::AWB, CALIBRATION_STATUS::CAL_ERROR, "Calibration is busy!");
        return;
    }

    this->config = config;
    showMessage = false;

    ret = QuickCalibrationLoadData(path);
    if (ret < 0) {
        emit sigCaliStatus(CALIBRATION_MODULE_ID::AWB, CALIBRATION_STATUS::CAL_ERROR, "Load file fail!");
        return;
    }

    ret = handleQuickCalibration();
    if (ret == -1) {
        emit sigCaliStatus(CALIBRATION_MODULE_ID::AWB, CALIBRATION_STATUS::CAL_ERROR, "Calibration fail!");
        return;
    }else if (ret == -2) {
        emit sigCaliStatus(CALIBRATION_MODULE_ID::AWB, CALIBRATION_STATUS::WARNING, "Write data fail!");
        return;
    }

    emit sigCaliStatus(CALIBRATION_MODULE_ID::AWB, CALIBRATION_STATUS::READY, "Calibration finish!");
}
