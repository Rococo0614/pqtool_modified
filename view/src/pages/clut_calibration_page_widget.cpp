#include "clut_calibration_page_widget.hpp"
#include "combobox_calibration_item_widget.hpp"
#include "lineedit_calibration_item_widget.hpp"
#include "network_client_helper.hpp"
#include <QHeaderView>
#include <string>

CLUTCalibrationPageWidget::CLUTCalibrationPageWidget(QWidget *parent) : BaseCalibrationPageWidget(static_cast<int>(CALIBRATION_MODULE_ID::CLUT), parent)
{
    initLeftArea();
    initEvent();
    source_roi_control_widget = createRoiControlUI(this->input_jpg_label);
    target_roi_control_widget = createRoiControlUI(this->output_image_label);
    retranslateUi();
    initClutTable();
}

void CLUTCalibrationPageWidget::initLeftArea()
{
    input_label = new QLabel();
    param_layout->addWidget(input_label);
    //initImportRawUI();
    initImportJpgUI();

    QHBoxLayout *edit_ori_layout = new QHBoxLayout;
    edit_source_roi_btn = new QPushButton;
    connect(edit_source_roi_btn, &QPushButton::clicked, [=]() {
        if (source_roi_control_widget != nullptr) {
            source_roi_control_widget->show();
        }
    });
    edit_ori_layout->addWidget(edit_source_roi_btn);
    edit_target_roi_btn = new QPushButton;
    connect(edit_target_roi_btn, &QPushButton::clicked, [=]() {
        if (target_roi_control_widget != nullptr) {
            target_roi_control_widget->show();
        }
    });
    edit_ori_layout->addWidget(edit_target_roi_btn);
    param_layout->addLayout(edit_ori_layout);


    color_space_item = new ComboboxCaliItemWidget("Color space");
    color_space_item->addItem("sRGB");
    color_space_item->addItem("BT2020");
    color_space_item->setEnable(false);
    //param_layout->addWidget(color_space_item);
    isp_gamma_item = new ComboboxCaliItemWidget("ISP gamma");
    isp_gamma_item->addItem("sRGB");
    isp_gamma_item->addItem("BT2020");
    isp_gamma_item->setEnable(false);
    //param_layout->addWidget(isp_gamma_item);
    target_gamma_item = new ComboboxCaliItemWidget("Target gamma");
    target_gamma_item->addItem("sRGB");
    target_gamma_item->addItem("BT2020");
    target_gamma_item->setEnable(false);
    //param_layout->addWidget(target_gamma_item);

    QHBoxLayout *ccm_cali_layout = new QHBoxLayout;
    ccm_matrix_label = new QLabel();
    clut_import_ccm_cali_btn = new QPushButton;
    clut_import_ccm_cali_btn->setEnabled(false);
    ccm_cali_layout->addWidget(ccm_matrix_label);
    ccm_cali_layout->addWidget(clut_import_ccm_cali_btn);
    //param_layout->addLayout(ccm_cali_layout);

    clut_matrix_table = new QTableWidget();
    clut_matrix_table->setStyleSheet("border: none;");
    clut_matrix_table->setRowCount(MATRIX_ROW);
    clut_matrix_table->setColumnCount(MATRIX_ROW);
    clut_matrix_table->setEnabled(false);
    clut_matrix_table->horizontalHeader()->hide();
    clut_matrix_table->verticalHeader()->hide();
    for (int i = 0; i < clut_matrix_table->columnCount(); i++) {
        clut_matrix_table->setColumnWidth(i, 40);
        for (int j = 0; j < clut_matrix_table->rowCount(); j++) {
            clut_matrix_table->setRowHeight(j, 30);
            clut_matrix_table->setItem(j, i, new QTableWidgetItem("1"));
        }
    }
    //param_layout->addWidget(clut_matrix_table);

    QGroupBox *hsl_group = createHslAdjustGroupbox();
    param_layout->addWidget(hsl_group);

    rgb_generate_label = new QLabel();
    param_layout->addWidget(rgb_generate_label);
    color_check_free_btn = new QPushButton;
    connect(color_check_free_btn, &QPushButton::clicked, [=]() {
        generateRgbPairs();
    });
    param_layout->addWidget(color_check_free_btn);

    lut_operation_label = new QLabel();
    param_layout->addWidget(lut_operation_label);
    luminance_invariant_check = new QCheckBox(tr("Luminance Invariant"));
    luminance_invariant_check->setChecked(false);
    gray_scale_invariant_check = new  QCheckBox(tr("Gray Scale Invariant"));
    gray_scale_invariant_check->setChecked(false);
    param_layout->addWidget(luminance_invariant_check);
    param_layout->addWidget(gray_scale_invariant_check);
    QString soc = GlobalData::getInstance()->getCvipqToolSoc();
    if (CV183X_SOC == soc) {
        luminance_invariant_check->setEnabled(false);
        gray_scale_invariant_check->setEnabled(false);
    }

    mode_item = new ComboboxCaliItemWidget("Mode");
    mode_item->addItem("Linear for SDR");
    mode_item->addItem("CLUT HDR for HDR");
    mode_item->setEnable(false);
    // param_layout->addWidget(mode_item);
    smoothness_item = new LineEditCaliItemWidget("Smoothness");
    smoothness_item->setKey("smoothness");
    smoothness_item->setData(200);
    smoothness_item->setEnable(false);
    // param_layout->addWidget(smoothness_item);

    initCaliUI();
    initWriteDataUI();
    initImportDataUI();
    initExportDataUI();
}

QGroupBox *CLUTCalibrationPageWidget::createHslAdjustGroupbox()
{
    QGroupBox * groupbox = new QGroupBox();
    groupbox->setTitle("HSL Adjust");
    QVBoxLayout *layout = new QVBoxLayout;
    groupbox->setLayout(layout);

    hsl_enable = new BinaryItemWidget("", "Enable");
    hsl_enable->setData(false);

    sigma_spinner = new DoubleSpinnerItemWidget("", "Sigma");
    sigma_spinner->setRange(0, 64);
    sigma_spinner->setData(1);

    QVariantList defvalue;
    QJsonObject matrix_attr;
    matrix_attr["min"] = 0;
    matrix_attr["max"] = 100;
    sbys_matrix = new UniversalMatrixItemWidget("", "SByS");
    matrix_attr["count"] = SAT_LENGTH;
    sbys_matrix->setParam(matrix_attr);
    sbys_matrix->addRegSectionKey("Clut Calibration HSL Adjust", "SByS");
    for (int i=0; i < SAT_LENGTH; i++) {
        defvalue.append(50);
    }
    sbys_matrix->setData(defvalue);

    matrix_attr["count"] = HUE_LENGTH;
    defvalue.clear();
    for (int i=0; i < HUE_LENGTH; i++) {
        defvalue.append(50);
    }
    sbyh_matrix = new UniversalMatrixItemWidget("", "SByH");
    sbyh_matrix->setParam(matrix_attr);
    sbyh_matrix->addRegSectionKey("Clut Calibration HSL Adjust", "SByH");
    sbyh_matrix->setData(defvalue);
    lbyh_matrix = new UniversalMatrixItemWidget("", "LByH");
    lbyh_matrix->setParam(matrix_attr);
    lbyh_matrix->addRegSectionKey("Clut Calibration HSL Adjust", "LByH");
    lbyh_matrix->setData(defvalue);
    matrix_attr["min"] = -30;
    matrix_attr["max"] = 30;
    hbyh_matrix = new UniversalMatrixItemWidget("", "HByH");
    hbyh_matrix->setParam(matrix_attr);
    hbyh_matrix->addRegSectionKey("Clut Calibration HSL Adjust", "HByH");

    layout->addWidget(hsl_enable);
    layout->addWidget(sigma_spinner);
    layout->addWidget(hbyh_matrix);
    layout->addWidget(sbyh_matrix);
    layout->addWidget(lbyh_matrix);
    layout->addWidget(sbys_matrix);

    return groupbox;
}

void CLUTCalibrationPageWidget::initEvent()
{
    connect(this, &BaseCalibrationPageWidget::sigImportData, this, &CLUTCalibrationPageWidget::handleImportData);
}

void CLUTCalibrationPageWidget::generateRgbPairs()
{
    int src_roi_width = 60, det_roi_width = 60;
    int src_roi_height = 60, det_roi_height = 60;

    QVector<QRectF> src_color_rects = this->input_jpg_label->getColorCheckerRect();
    double src_scale_factor = this->input_jpg->getScaleFactor();
    QVector<QRectF> det_color_rects = this->output_image_label->getColorCheckerRect();
    double det_scale_factor = this->output_img->getScaleFactor();
    if ((src_color_rects.size() != 24)  || (det_color_rects.size() != 24)) {
        return;
    }
    src_roi_width = qFloor(src_color_rects.first().width()/src_scale_factor);
    src_roi_height = qFloor(src_color_rects.first().height()/src_scale_factor);
    det_roi_width = qFloor(det_color_rects.first().width()/det_scale_factor);
    det_roi_height = qFloor(det_color_rects.first().height()/det_scale_factor);

    qDebug("source_roi_width:%d source_roi_height:%d target_roi_width:%d target_roi_height:%d",
    src_roi_width, src_roi_height, det_roi_width, det_roi_height);

    QString src_jpg_path = input_jpg->getPath();
    QString dst_jpg_path = output_img->getPath();
    if (src_jpg_path.isEmpty() || dst_jpg_path.isEmpty()) {
        return;
    }

    QImage src(src_jpg_path);
    QImage dst(dst_jpg_path);
    QImage src_img, dst_img;
    if (src.format() != QImage::Format_RGB888) {
        src_img = src.convertToFormat(QImage::Format_RGB888);
    } else {
        src_img = src;
    }

    if (dst.format() != QImage::Format_RGB888) {
        dst_img = dst.convertToFormat(QImage::Format_RGB888);
    } else {
        dst_img = dst;
    }

    coordinate src_coordinate[24] = {};
    coordinate dst_coordinate[24] = {};
    ROI_INFO src_roi_info[24] = {};
    ROI_INFO dst_roi_info[24] = {};
    unsigned char *src_img_uchar = new unsigned char[src_img.width() * src_img.height() * 3];
    unsigned char *dst_img_uchar = new unsigned char[dst_img.width() * dst_img.height() * 3];

    qDebug("source width:%d source height:%d target width:%d target height:%d  src format:%d det format:%d",
    src_img.width(), src_img.height(), dst_img.width(), dst_img.height(), src.format(), dst.format());

    for (int y = 0; y < src_img.height(); y++) {
        int idx = y * src_img.width() * 3;
        memcpy(&src_img_uchar[idx], src_img.scanLine(y), src_img.width() * 3);
    }
    for (int y = 0; y < dst_img.height(); y++) {
        int idx = y * dst_img.width() * 3;
        memcpy(&dst_img_uchar[idx], dst_img.scanLine(y), dst_img.width() * 3);
    }

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 6; x++) {
            QRectF src_rect = src_color_rects[x + y * 6];
            QRectF det_rect = det_color_rects[x + y * 6];

            if (src_scale_factor > 0) {
                src_rect.setX(qFloor(src_rect.x()/src_scale_factor));
                src_rect.setY(qFloor(src_rect.y()/src_scale_factor));
            }
            // color rect position need even
            if (qFloor(src_rect.x()) % 2 == 1) {
                src_rect.setX(qFloor(src_rect.x()) + 1);
            }
            if (qFloor(src_rect.y()) % 2 == 1) {
                src_rect.setY(qFloor(src_rect.y()) + 1);
            }
            if (src_rect.x() < 0) src_rect.setX(0);
            if (src_rect.y() < 0) src_rect.setY(0);

            if (det_scale_factor > 0) {
                det_rect.setX(qFloor(det_rect.x()/det_scale_factor));
                det_rect.setY(qFloor(det_rect.y()/det_scale_factor));
            }
            // color rect position need even
            if (qFloor(det_rect.x()) % 2 == 1) {
                det_rect.setX(qFloor(det_rect.x()) + 1);
            }
            if (qFloor(det_rect.y()) % 2 == 1) {
                det_rect.setY(qFloor(det_rect.y()) + 1);
            }
            if (det_rect.x() < 0) det_rect.setX(0);
            if (det_rect.y() < 0) det_rect.setY(0);

            src_roi_info[x + y * 6].win_width = src_roi_width;
            src_roi_info[x + y * 6].win_height = src_roi_height;
            src_roi_info[x + y * 6].str_x = src_rect.x();
            src_roi_info[x + y * 6].str_y = src_rect.y();
            dst_roi_info[x + y * 6].win_width = det_roi_width;
            dst_roi_info[x + y * 6].win_height = det_roi_height;
            dst_roi_info[x + y * 6].str_x = det_rect.x();
            dst_roi_info[x + y * 6].str_y = det_rect.y();
        }
    }

    cali_roi_rgb_cal(src_img_uchar, src_img.width(), src_img.height(), src_coordinate, src_roi_info, 24);
    cali_roi_rgb_cal(dst_img_uchar, dst_img.width(), dst_img.height(), dst_coordinate, dst_roi_info, 24);

    for (int i = 0; i < 24; i++) {
        qDebug("src_coordinate:%f, %f, %f, %d, %d ",
        src_coordinate[i].x, src_coordinate[i].y, src_coordinate[i].z, src_coordinate[i].valid, src_coordinate[i].create_idx);
        qDebug("dst_coordinate:%f, %f, %f, %d, %d ",
        dst_coordinate[i].x, dst_coordinate[i].y, dst_coordinate[i].z, dst_coordinate[i].valid, dst_coordinate[i].create_idx);
    }

    rgb_list_widget->clearRgbTable();
    for (int i = 0; i < 24; i++) {
        RGB_PAIRS rgb;
        rgb.ori_r = (unsigned char)(src_coordinate[i].x + 0.5);
        rgb.ori_g = (unsigned char)(src_coordinate[i].y + 0.5);
        rgb.ori_b = (unsigned char)(src_coordinate[i].z + 0.5);
        rgb.ori_valid = src_coordinate[i].valid;
        rgb.ori_create_idx = src_coordinate[i].create_idx;
        rgb.est_r = (unsigned char)(dst_coordinate[i].x + 0.5);
        rgb.est_g = (unsigned char)(dst_coordinate[i].y + 0.5);
        rgb.est_b = (unsigned char)(dst_coordinate[i].z + 0.5);
        rgb.est_valid = dst_coordinate[i].valid;
        rgb.est_create_idx = dst_coordinate[i].create_idx;
        RGBItem * item = new RGBItem(rgb);
        qDebug("add item i:%d", i);
        rgb_list_widget->addTableItem(item);
    }

    delete [] src_img_uchar;
    delete [] dst_img_uchar;

}

void CLUTCalibrationPageWidget::initClutTable()
{
    QString soc = GlobalData::getInstance()->getCvipqToolSoc();

    if (CV183X_SOC == soc) {
        h_lut = new int[CLUT_SIZE_183X]();
        s_lut = new int[CLUT_SIZE_183X]();
        v_lut = new int[CLUT_SIZE_183X]();
    } else {
        r_lut = new int[CLUT_SIZE]();
        g_lut = new int[CLUT_SIZE]();
        b_lut = new int[CLUT_SIZE]();

        for (int i = 0; i < 17; i++) {
            for (int j = 0; j < 17; j++) {
                for (int k = 0; k < 17; k++) {
                    int idx = 17 * 17 * i + 17 * j + k;
                    r_lut[idx] = qBound(0, 64 * k, MAX_CLUT_VALUE);
                    g_lut[idx] = qBound(0, 64 * j, MAX_CLUT_VALUE);
                    b_lut[idx] = qBound(0, 64 * i, MAX_CLUT_VALUE);
                }
            }
        }
    }
}

void CLUTCalibrationPageWidget::releaseClutTable()
{
    if (h_lut != nullptr) {
        delete [] h_lut;
        h_lut = nullptr;
    }

    if (s_lut != nullptr) {
        delete [] s_lut;
        s_lut = nullptr;
    }

    if (v_lut != nullptr) {
        delete [] v_lut;
        v_lut = nullptr;
    }

    if (r_lut != nullptr) {
        delete [] r_lut;
        r_lut = nullptr;
    }

    if (g_lut != nullptr) {
        delete [] g_lut;
        g_lut = nullptr;
    }

    if (b_lut != nullptr) {
        delete [] b_lut;
        b_lut = nullptr;
    }
}


bool CLUTCalibrationPageWidget::calibration()
{
    QList<RGBItem *> rgb_item_list = rgb_list_widget->getRGBList();
    if (rgb_item_list.isEmpty()) {
        return false;
    }

    QString soc = GlobalData::getInstance()->getCvipqToolSoc();
    int size = rgb_item_list.size();
    if (CV183X_SOC == soc) {
        size += 2;
    }
    qDebug("rgb list size :%d\n", size);
    coordinate src_coordinate[CLUT_RGB_MAX_COLOR_CELL];
    coordinate dst_coordinate[CLUT_RGB_MAX_COLOR_CELL];

    for (int i = 0; i < rgb_item_list.size(); i++) {
        src_coordinate[i].x = rgb_item_list[i]->getOriR();
        src_coordinate[i].y = rgb_item_list[i]->getOriG();
        src_coordinate[i].z = rgb_item_list[i]->getOriB();
        src_coordinate[i].valid = rgb_item_list[i]->getOriValid();
        src_coordinate[i].create_idx = rgb_item_list[i]->getOriCreateIdx();
        dst_coordinate[i].x = rgb_item_list[i]->getEstR();
        dst_coordinate[i].y = rgb_item_list[i]->getEstG();
        dst_coordinate[i].z = rgb_item_list[i]->getEstB();
        dst_coordinate[i].valid = rgb_item_list[i]->getEstValid();
        dst_coordinate[i].create_idx = rgb_item_list[i]->getEstCreateIdx();
    }

    if (CV183X_SOC == soc) {
        // Todo: Algo wiil add white and black to clut calibration algo.
        src_coordinate[size-2].x = src_coordinate[size-2].y = src_coordinate[size-2].z = 0;
        src_coordinate[size-1].x = src_coordinate[size-1].y = src_coordinate[size-1].z = 255;
        src_coordinate[size-2].valid = src_coordinate[size-1].valid = true;
        src_coordinate[size-2].create_idx = src_coordinate[size-1].create_idx = 0;
        dst_coordinate[size-2].x = dst_coordinate[size-2].y = dst_coordinate[size-2].z = 0;
        dst_coordinate[size-1].x = dst_coordinate[size-1].y = dst_coordinate[size-1].z = 255;
        dst_coordinate[size-2].valid = dst_coordinate[size-1].valid = false;
        dst_coordinate[size-2].create_idx = dst_coordinate[size-1].create_idx = 0;
    }

    for (int i = 0; i < size; i++) {
        qDebug("[%2d] %3.0lf, %3.0lf, %3.0lf, %d, %d, %3.0lf, %3.0lf, %3.0lf, %d, %d ", i,
            src_coordinate[i].x, src_coordinate[i].y, src_coordinate[i].z, src_coordinate[i].valid, src_coordinate[i].create_idx,
            dst_coordinate[i].x, dst_coordinate[i].y, dst_coordinate[i].z, dst_coordinate[i].valid, dst_coordinate[i].create_idx);
    }

    if (CV183X_SOC == soc) {
        cali_clut_calc(src_coordinate, dst_coordinate, h_lut, s_lut, v_lut);
    } else {
        if (luminance_invariant_check->isChecked()) {
            qDebug("Call cali_roi_rgb_adjust");
            cali_roi_rgb_adjust(src_coordinate, dst_coordinate, src_coordinate, size, 0);
        }

        if (gray_scale_invariant_check->isChecked()) {
            qDebug("Call cali_roi_rgb_interpolation");
            point_interpolation_coef	interpolation_coef;
            int final_color_pair_count;

            #define MAX_CLUT_INTERPOLATION_PAIRS (10)

            interpolation_coef.gray_point_std_div_distance = 8; // 8 bit
            interpolation_coef.init_max_int_distance = 20;
            interpolation_coef.min_to_max_ratio = 0.6f;
            interpolation_coef.max_add_points = min(MAX_CLUT_INTERPOLATION_PAIRS, CLUT_RGB_MAX_COLOR_CELL - size);

            cali_roi_rgb_interpolation(src_coordinate, dst_coordinate, size, &interpolation_coef, &final_color_pair_count);

            size = final_color_pair_count;
        }

        cali_clut_calc_rgb(src_coordinate, dst_coordinate, size, r_lut, g_lut, b_lut);
    }

    qDebug("clut calibration success");

    return true;
}

QJsonArray CLUTCalibrationPageWidget::getCalibrationData()
{
    QJsonArray data_array;
    QJsonObject lut_data;

    QString soc = GlobalData::getInstance()->getCvipqToolSoc();
    if (CV183X_SOC == soc) {
        lut_data[KEY_REG_KEY] = "Hlut";
        lut_data[KEY_VALUE] = utils::arrayPtrToJsonArray(h_lut, CLUT_SIZE_183X);
        lut_data[KEY_VALUE_TYPE] = TYPE_ARRAY;
        lut_data[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
        data_array.append(lut_data);

        lut_data[KEY_REG_KEY] = "Slut";
        lut_data[KEY_VALUE] = utils::arrayPtrToJsonArray(s_lut, CLUT_SIZE_183X);
        lut_data[KEY_VALUE_TYPE] = TYPE_ARRAY;
        lut_data[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
        data_array.append(lut_data);

        lut_data[KEY_REG_KEY] = "Vlut";
        lut_data[KEY_VALUE] = utils::arrayPtrToJsonArray(v_lut, CLUT_SIZE_183X);
        lut_data[KEY_VALUE_TYPE] = TYPE_ARRAY;
        lut_data[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
        data_array.append(lut_data);
    } else {
        if(hsl_enable->getData().toBool()) {
            RGB in[CLUT_SIZE], out[CLUT_SIZE];
            int rlut[CLUT_SIZE], glut[CLUT_SIZE], blut[CLUT_SIZE];
            for (qint32 i=0; i<CLUT_SIZE; i++) {
                in[i].r = r_lut[i];
                in[i].g = g_lut[i];
                in[i].b = b_lut[i];
            }

            clut_adjust_by_hsl(in, out);

            for (qint32 i = 0; i < CLUT_SIZE; i++) {
                rlut[i] = out[i].r;
                glut[i] = out[i].g;
                blut[i] = out[i].b;
            }
            lut_data[KEY_REG_KEY] = "ClutR";
            lut_data[KEY_VALUE] = utils::arrayPtrToJsonArray(rlut, CLUT_SIZE);
            lut_data[KEY_VALUE_TYPE] = TYPE_ARRAY;
            lut_data[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
            data_array.append(lut_data);

            lut_data[KEY_REG_KEY] = "ClutG";
            lut_data[KEY_VALUE] = utils::arrayPtrToJsonArray(glut, CLUT_SIZE);
            lut_data[KEY_VALUE_TYPE] = TYPE_ARRAY;
            lut_data[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
            data_array.append(lut_data);

            lut_data[KEY_REG_KEY] = "ClutB";
            lut_data[KEY_VALUE] = utils::arrayPtrToJsonArray(blut, CLUT_SIZE);
            lut_data[KEY_VALUE_TYPE] = TYPE_ARRAY;
            lut_data[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
            data_array.append(lut_data);
        } else {
            lut_data[KEY_REG_KEY] = "ClutR";
            lut_data[KEY_VALUE] = utils::arrayPtrToJsonArray(r_lut, CLUT_SIZE);
            lut_data[KEY_VALUE_TYPE] = TYPE_ARRAY;
            lut_data[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
            data_array.append(lut_data);

            lut_data[KEY_REG_KEY] = "ClutG";
            lut_data[KEY_VALUE] = utils::arrayPtrToJsonArray(g_lut, CLUT_SIZE);
            lut_data[KEY_VALUE_TYPE] = TYPE_ARRAY;
            lut_data[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
            data_array.append(lut_data);

            lut_data[KEY_REG_KEY] = "ClutB";
            lut_data[KEY_VALUE] = utils::arrayPtrToJsonArray(b_lut, CLUT_SIZE);
            lut_data[KEY_VALUE_TYPE] = TYPE_ARRAY;
            lut_data[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
            data_array.append(lut_data);
        }
    }

    return data_array;
}

void CLUTCalibrationPageWidget::handleImportData(QString name, QJsonObject data)
{
    qDebug()<<"CLUTCalibrationPageWidget import file: "<<name;

    QString soc = GlobalData::getInstance()->getCvipqToolSoc();

    // import clut data
    QString clut_module_name = PREFIX_CALI_STR + getCaliModuleName(CALIBRATION_MODULE_ID::CLUT);
    if (data.contains(clut_module_name)) {
        QJsonArray clut_data_array = data[clut_module_name].toArray();
        for (auto clut_data : clut_data_array) {
            QJsonObject clut_data_obj = clut_data.toObject();
            if (CV183X_SOC == soc) {
                if (clut_data_obj[KEY_REG_KEY] == "Hlut") {
                    QJsonArray value_array = clut_data_obj[KEY_VALUE].toArray();
                    for (int i = 0; (i < CLUT_SIZE_183X) && (i < value_array.size()); i++) {
                        h_lut[i] = value_array[i].toInt();
                    }
                } else if (clut_data_obj[KEY_REG_KEY] == "Slut") {
                    QJsonArray value_array = clut_data_obj[KEY_VALUE].toArray();
                    for (int i = 0; (i < CLUT_SIZE_183X) && (i < value_array.size()); i++) {
                        s_lut[i] = value_array[i].toInt();
                    }
                } else if (clut_data_obj[KEY_REG_KEY] == "Vlut") {
                    QJsonArray value_array = clut_data_obj[KEY_VALUE].toArray();
                    for (int i = 0; (i < CLUT_SIZE_183X) && (i < value_array.size()); i++) {
                        v_lut[i] = value_array[i].toInt();
                    }
                }
            } else {
                if (clut_data_obj[KEY_REG_KEY] == "ClutR") {
                    QJsonArray value_array = clut_data_obj[KEY_VALUE].toArray();
                    for (int i = 0; (i < CLUT_SIZE) && (i < value_array.size()); i++) {
                        r_lut[i] = value_array[i].toInt();
                    }
                } else if (clut_data_obj[KEY_REG_KEY] == "ClutG") {
                    QJsonArray value_array = clut_data_obj[KEY_VALUE].toArray();
                    for (int i = 0; (i < CLUT_SIZE) && (i < value_array.size()); i++) {
                        g_lut[i] = value_array[i].toInt();
                    }
                } else if (clut_data_obj[KEY_REG_KEY] == "ClutB") {
                    QJsonArray value_array = clut_data_obj[KEY_VALUE].toArray();
                    for (int i = 0; (i < CLUT_SIZE) && (i < value_array.size()); i++) {
                        b_lut[i] = value_array[i].toInt();
                    }
                }
            }
        }
    }
}

void CLUTCalibrationPageWidget::retranslateUi()
{
    BaseCalibrationPageWidget::retranslateUi();

    edit_source_roi_btn->setText(tr("Edit Source ROIs"));
    edit_target_roi_btn->setText(tr("Edit Target ROIs"));
    ccm_matrix_label->setText(tr("CCM matrix"));
    clut_import_ccm_cali_btn->setText(tr("Import CCM calibration"));
    color_check_free_btn->setText(tr("Color Check 24"));
    input_label->setText(tr("Input images:"));
    rgb_generate_label->setText(tr("[Generate rgb from input jpg & target]:"));
    lut_operation_label->setText(tr("LUT operations:"));
}

CLUTCalibrationPageWidget::~CLUTCalibrationPageWidget()
{
    releaseClutTable();
}

QJsonObject CLUTCalibrationPageWidget::GetModuleObjectToNetworkJsonrpc()
{
    qDebug("CLUTCalibrationPageWidget::GetModuleObjectToNetworkJsonrpc.");
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
    if(hsl_enable->getData().toBool()) {
        RGB in[CLUT_SIZE], out[CLUT_SIZE];
        int rlut[CLUT_SIZE], glut[CLUT_SIZE], blut[CLUT_SIZE];
        for (qint32 i = 0; i < CLUT_SIZE; i++) {
            in[i].r = r_lut[i];
            in[i].g = g_lut[i];
            in[i].b = b_lut[i];
        }
        clut_adjust_by_hsl(in, out);
        for (qint32 i = 0; i < CLUT_SIZE; i++) {
            rlut[i] = out[i].r;
            glut[i] = out[i].g;
            blut[i] = out[i].b;
        }

        params_obj["ClutR"] = utils::arrayPtrToJsonArray(rlut, CLUT_SIZE);
        params_obj["ClutG"] = utils::arrayPtrToJsonArray(glut, CLUT_SIZE);
        params_obj["ClutB"] = utils::arrayPtrToJsonArray(blut, CLUT_SIZE);
    } else {
        params_obj["ClutR"] = utils::arrayPtrToJsonArray(r_lut, CLUT_SIZE);
        params_obj["ClutG"] = utils::arrayPtrToJsonArray(g_lut, CLUT_SIZE);
        params_obj["ClutB"] = utils::arrayPtrToJsonArray(b_lut, CLUT_SIZE);
    }
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_METHOD] = struct_set_api_map[struct_id];
    send_obj[MSG_KEY_ID] = jsonrpc_id;
    send_obj[MSG_KEY_PARAMS] = params_obj;
    module_obj[struct_id] = send_obj;

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_WRITE_CLUT_CALIBRATION;
    id_info[MSG_KEY_MODULE] = module_name;
    id_info[MSG_KEY_STRUCT] = struct_id;
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(jsonrpc_id, id_info);

    return module_obj;
}

void CLUTCalibrationPageWidget::rgb_to_hsl(RGB *rgb, HSL* hsl)
{
    int h, s, l;

    int r = rgb->r;
    int g = rgb->g;
    int b = rgb->b;

    int cmax = max(r, max(g, b));
    int cmin = min(r, min(g, b));
    int delta = cmax - cmin;

    l = (cmax + cmin) / 2;

    if (delta == 0) {
        h = 0;
        s = 0;
    } else {
        if (l < (MAX_LIGHTNESS_VALUE >> 1)) {
            s = delta * MAX_SATURATION_VALUE / (cmax + cmin);
        } else {
            s = delta * MAX_SATURATION_VALUE / (2 * MAX_SATURATION_VALUE -cmax - cmin);
        }

        int segment, shift;
        if (cmax == r) {
            segment = (g - b) * (MAX_HUE_VALUE / 6) / delta;
            shift = 0;
        } else if (cmax == g) {
            segment = (b - r) * (MAX_HUE_VALUE / 6) / delta;
            shift = MAX_HUE_VALUE / 3;
        } else {
            segment = (r - g) * (MAX_HUE_VALUE / 6) / delta;
            shift = 2 * MAX_HUE_VALUE / 3;
        }

        h = segment + shift;
        if (h < 0) {
            h += MAX_HUE_VALUE;
        }
    }

    hsl->h = h;
    hsl->s = s;
    hsl->l = l;
}

void CLUTCalibrationPageWidget::hsl_to_rgb(HSL *hsl, RGB *rgb)
{
    int c = (MAX_LIGHTNESS_VALUE - abs(2 * hsl->l - MAX_LIGHTNESS_VALUE)) * hsl->s / MAX_SATURATION_VALUE;
    int h_segment = (hsl->h * 6) / MAX_HUE_VALUE;
    int x = c * (1024 - abs(((hsl->h * 6 * 1024 / MAX_HUE_VALUE ) % 2048) - 1024)) / 1024;

    int m = hsl->l - c / 2;
    int r_prime, g_prime, b_prime;

    switch (h_segment) {
    case 0: r_prime = c; g_prime = x; b_prime = 0; break;
    case 1: r_prime = x; g_prime = c; b_prime = 0; break;
    case 2: r_prime = 0; g_prime = c; b_prime = x; break;
    case 3: r_prime = 0; g_prime = x; b_prime = c; break;
    case 4: r_prime = x; g_prime = 0; b_prime = c; break;
    case 5: r_prime = c; g_prime = 0; b_prime = x; break;
    default: r_prime = 0; g_prime = 0; b_prime = 0; break;
    }

    rgb->r = (r_prime + m) * MAX_RGB_VALUE / MAX_LIGHTNESS_VALUE;
    rgb->g = (g_prime + m) * MAX_RGB_VALUE / MAX_LIGHTNESS_VALUE;
    rgb->b = (b_prime + m) * MAX_RGB_VALUE / MAX_LIGHTNESS_VALUE;
}

void CLUTCalibrationPageWidget::interpolate_by_hue(int hue, HSL_ATTR_S *hsl_attr,
            qint16 *angle,  quint16 *s_scale, quint16 *l_scale)
{
    double x = 0, x0 = 0, y0 = 0, y1 = 0;
    double stride = (double)MAX_HUE_VALUE / (HUE_LENGTH - 1);

    quint16 idx0 = hue / stride;
    quint16 idx1;

    if  (idx0 >= HUE_LENGTH - 1) {
        idx1 = idx0 = HUE_LENGTH - 1;
    } else {
        idx1 = idx0 + 1;
    }

    if (idx0 == idx1) {
        *angle = hsl_attr->HByH[idx0] * 16;
        *s_scale = hsl_attr->SByH[idx0];
        *l_scale = hsl_attr->LByH[idx0];
    } else {
        x = hue;
        x0 = idx0 * stride;

        y0 = hsl_attr->HByH[idx0];
        y1 = hsl_attr->HByH[idx1];
        *angle = ((y1 - y0) * (x - x0) / stride + y0) * 16 + 0.5;
        y0 = hsl_attr->SByH[idx0];
        y1 = hsl_attr->SByH[idx1];
        *s_scale = (y1 - y0) * (x - x0) / stride + y0 + 0.5;
        y0 = hsl_attr->LByH[idx0];
        y1 = hsl_attr->LByH[idx1];
        *l_scale = (y1 - y0) * (x - x0) / stride + y0 + 0.5;
    }
}

void CLUTCalibrationPageWidget::interpolate_by_sat(int sat, HSL_ATTR_S *hsl_attr, quint16 *scale)
{
    double x = 0, x0 = 0, y0 = 0, y1 = 0;
    double stride = (double)MAX_SATURATION_VALUE / (SAT_LENGTH - 1);

    quint16 idx0 = sat / stride;
    quint16 idx1;

    if  (idx0 >= SAT_LENGTH - 1) {
        idx1 = idx0 = SAT_LENGTH - 1;
    } else {
        idx1 = idx0 + 1;
    }

    if (idx0 == idx1) {
        *scale = hsl_attr->SByS[idx0];
    } else {
        x = sat;
        x0 = idx0 * stride;
        y0 = hsl_attr->SByS[idx0];
        y1 = hsl_attr->SByS[idx1];
        *scale = (y1 - y0) * (x - x0) / stride + y0 + 0.5;
    }
}

void CLUTCalibrationPageWidget:: cal_gaussian_kernel(int *wgt, float sigma)
{
    int center = KERNEL_SIZE / 2;
    double sum = 0.0;
    double kernel[KERNEL_SIZE];

    for (int i = 0; i < KERNEL_SIZE; i++) {
        double distance = (double)(i - center);
        kernel[i] = (double)exp(-distance * distance / (2 * sigma * sigma));
        sum += kernel[i];
    }

    for (int i = 0; i < KERNEL_SIZE; i++) {
        kernel[i] /= sum;
        wgt[i] = (int)(kernel[i] * 4096);
    }
}

void CLUTCalibrationPageWidget::clut_denoise(RGB *in, RGB *out, int *wgt)
{
    int radius = KERNEL_SIZE / 2;
    int clut_x_r[CLUT_SIZE], clut_x_g[CLUT_SIZE], clut_x_b[CLUT_SIZE];
    int clut_y_r[CLUT_SIZE], clut_y_g[CLUT_SIZE], clut_y_b[CLUT_SIZE];

    for (int i = 0; i < 17; i++)
    {
        for (int j = 0; j < 17; j++)
        {
            for (int k = 0; k < 17; k++)
            {
                int weight_sum = 0;
                int sum_r = 0, sum_g = 0, sum_b = 0;
                for (int di = -radius; di <= radius; di++)
                {
                    if (i + di < 0 || i + di >= 17)
                    {
                        continue;
                    }

                    int idx = (i + di) * 17 * 17 + j * 17 + k;
                    weight_sum += wgt[di + radius];
                    sum_r += wgt[di + radius] * in[idx].r;
                    sum_g += wgt[di + radius] * in[idx].g;
                    sum_b += wgt[di + radius] * in[idx].b;
                }
                clut_x_r[i * 17 * 17 + j * 17 + k] = sum_r / weight_sum;
                clut_x_g[i * 17 * 17 + j * 17 + k] = sum_g / weight_sum;
                clut_x_b[i * 17 * 17 + j * 17 + k] = sum_b / weight_sum;
            }
        }
    }

    for (int i = 0; i < 17; i++)
    {
        for (int j = 0; j < 17; j++)
        {
            for (int k = 0; k < 17; k++)
            {
                int weight_sum = 0;
                int sum_r = 0, sum_g = 0, sum_b = 0;
                for (int dj = -radius; dj <= radius; dj++)
                {
                    if (j + dj < 0 || j + dj >= 17)
                    {
                        continue;
                    }
                    int idx = i * 17 * 17 + (j + dj) * 17 + k;
                    weight_sum += wgt[dj + radius];
                    sum_r += wgt[dj + radius] * clut_x_r[idx];
                    sum_g += wgt[dj + radius] * clut_x_g[idx];
                    sum_b += wgt[dj + radius] * clut_x_b[idx];
                }
                clut_y_r[i * 17 * 17 + j * 17 + k] = sum_r / weight_sum;
                clut_y_g[i * 17 * 17 + j * 17 + k] = sum_g / weight_sum;
                clut_y_b[i * 17 * 17 + j * 17 + k] = sum_b / weight_sum;
            }
        }
    }

    for (int i = 0; i < 17; i++)
    {
        for (int j = 0; j < 17; j++)
        {
            for (int k = 0; k < 17; k++)
            {
                int weight_sum = 0;
                int sum_r = 0, sum_g = 0, sum_b = 0;
                for (int dk = -radius; dk <= radius; dk++)
                {
                    if (k + dk < 0 || k + dk >= 17)
                    {
                        continue;
                    }
                    int idx = i * 17 * 17 + j * 17 + k + dk;
                    weight_sum += wgt[dk + radius];
                    sum_r += wgt[dk + radius] * clut_y_r[idx];
                    sum_g += wgt[dk + radius] * clut_y_g[idx];
                    sum_b += wgt[dk + radius] * clut_y_b[idx];
                }
                out[i * 17 * 17 + j * 17 + k].r = sum_r / weight_sum;
                out[i * 17 * 17 + j * 17 + k].g = sum_g / weight_sum;
                out[i * 17 * 17 + j * 17 + k].b = sum_b / weight_sum;
            }
        }
    }
}

void CLUTCalibrationPageWidget::hsl_by_hue(HSL* in, HSL *out, qint16 angle, quint16 s_scale, quint16 l_scale)
{
    int h = in->h + angle;

    if (h >= MAX_HUE_VALUE) {
        h -= MAX_HUE_VALUE;
    } else if (h < 0) {
        h += MAX_HUE_VALUE;
    }
    out->h = qBound(0, h, MAX_HUE_VALUE);

    out->s = in->s * s_scale / SCALE_UNIT;
    out->s = qBound(0, out->s, MAX_SATURATION_VALUE);

    out->l = in->l * l_scale / SCALE_UNIT;
    out->l = qBound(0, out->l, MAX_LIGHTNESS_VALUE);
}

void CLUTCalibrationPageWidget::saturation_by_saturation(HSL* in, HSL *out, quint16 scale)
{
    out->s = in->s * scale / SCALE_UNIT;
    out->s = qBound(0, out->s, MAX_SATURATION_VALUE);
}

void CLUTCalibrationPageWidget::clut_adjust_by_hsl(RGB *in, RGB *out)
{
    HSL_ATTR_S hsl_attr;
    QVariantList hh = hbyh_matrix->getData().toList();
    QVariantList sh = sbyh_matrix->getData().toList();
    QVariantList lh = lbyh_matrix->getData().toList();
    QVariantList ss = sbys_matrix->getData().toList();
    for (qint32 i = 0; i < HUE_LENGTH; i++) {
        hsl_attr.HByH[i] = hh[i].toInt();
        hsl_attr.SByH[i] = sh[i].toUInt();
        hsl_attr.LByH[i] = lh[i].toUInt();
        if (i < SAT_LENGTH) {
            hsl_attr.SByS[i] = ss[i].toUInt();
        }
    }

    RGB tmp[CLUT_SIZE];

    for (qint32 i = 0 ; i < CLUT_SIZE ; i++) {
        RGB rgb;
        HSL hsl, hslOut;
        qint16 angle;
        quint16 s_scale, l_scale;

        rgb.r = in[i].r * MAX_RGB_VALUE / MAX_CLUT_VALUE;
        rgb.g = in[i].g * MAX_RGB_VALUE / MAX_CLUT_VALUE;
        rgb.b = in[i].b * MAX_RGB_VALUE / MAX_CLUT_VALUE;

        rgb_to_hsl(&rgb, &hsl);
        interpolate_by_hue(hsl.h, &hsl_attr, &angle, &s_scale, &l_scale);
        hsl_by_hue(&hsl, &hslOut, angle, s_scale, l_scale);
        interpolate_by_sat(hslOut.s, &hsl_attr, &s_scale);

        hsl = hslOut;
        saturation_by_saturation(&hsl, &hslOut, s_scale);
        hsl_to_rgb(&hslOut, &rgb);

        tmp[i].r = rgb.r;
        tmp[i].g = rgb.g;
        tmp[i].b = rgb.b;
    }

    int wgt[KERNEL_SIZE] = {0, 0, 4095, 0, 0};
    float sigma = sigma_spinner->getData().toFloat();
    if (sigma > 0) {
        cal_gaussian_kernel(wgt, sigma);
    }

    clut_denoise(tmp, out, wgt);

    for (int i = 0; i < 17; i++)
    {
        int idx = i * 17 * 17 + i * 17 + i;
        int sum = out[idx].r + out[idx].g + out[idx].b;
        out[idx].r = sum / 3;
        out[idx].g = sum / 3;
        out[idx].b = sum / 3;
    }

    for (int i = 0; i < CLUT_SIZE; i++)
    {
        out[i].r = qBound(0, out[i].r * MAX_CLUT_VALUE / MAX_RGB_VALUE, MAX_CLUT_VALUE);
        out[i].g = qBound(0, out[i].g * MAX_CLUT_VALUE / MAX_RGB_VALUE, MAX_CLUT_VALUE);
        out[i].b = qBound(0, out[i].b * MAX_CLUT_VALUE / MAX_RGB_VALUE, MAX_CLUT_VALUE);
    }
}
