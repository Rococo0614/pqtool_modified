#include "base_calibration_page_widget.hpp"
#include "global_data.hpp"
#include "network_client_helper.hpp"
#include "calibration_ct_list_widget.hpp"
#include <QFileDialog>
#include <QMessageBox>

BaseCalibrationPageWidget::BaseCalibrationPageWidget(int module_id, QWidget *parent) : BasePageWidget("", parent)
{
    setProperty("cali_id", QVariant(module_id));
    initData();
    initUI();
    main_layout->setStretch(0, 1);
    main_layout->setStretch(1, 3);
    if (static_cast<CALIBRATION_MODULE_ID>(module_id) == CALIBRATION_MODULE_ID::BLC) {
        struct_list.append("ISP_BLACK_LEVEL_ATTR_S");
        struct_get_api_map.insert("ISP_BLACK_LEVEL_ATTR_S", "CVI_ISP_GetBlackLevelAttr");
        struct_set_api_map.insert("ISP_BLACK_LEVEL_ATTR_S", "CVI_ISP_SetBlackLevelAttr");
    } else if (static_cast<CALIBRATION_MODULE_ID>(module_id) == CALIBRATION_MODULE_ID::LSC) {
        struct_list.append("ISP_MESH_SHADING_GAIN_LUT_ATTR_S");
        struct_get_api_map.insert("ISP_MESH_SHADING_GAIN_LUT_ATTR_S", "CVI_ISP_GetMeshShadingGainLutAttr");
        struct_set_api_map.insert("ISP_MESH_SHADING_GAIN_LUT_ATTR_S", "CVI_ISP_SetMeshShadingGainLutAttr");
        QString soc = GlobalData::getInstance()->getCvipqToolSoc();
        if (soc == CV184X_SOC) {
            struct_list.append("ISP_RADIAL_SHADING_ATTR_S");
            struct_get_api_map.insert("ISP_RADIAL_SHADING_ATTR_S", "CVI_ISP_GetRadialShadingAttr");
            struct_set_api_map.insert("ISP_RADIAL_SHADING_ATTR_S", "CVI_ISP_SetRadialShadingAttr");
            struct_list.append("ISP_RADIAL_SHADING_GAIN_LUT_ATTR_S");
            struct_get_api_map.insert("ISP_RADIAL_SHADING_GAIN_LUT_ATTR_S", "CVI_ISP_GetRadialShadingGainLutAttr");
            struct_set_api_map.insert("ISP_RADIAL_SHADING_GAIN_LUT_ATTR_S", "CVI_ISP_SetRadialShadingGainLutAttr");
            struct_list.append("ISP_MESH_SHADING_ATTR_S");
            struct_get_api_map.insert("ISP_MESH_SHADING_ATTR_S", "CVI_ISP_GetMeshShadingAttr");
            struct_set_api_map.insert("ISP_MESH_SHADING_ATTR_S", "CVI_ISP_SetMeshShadingAttr");
        }
    } else if (static_cast<CALIBRATION_MODULE_ID>(module_id) == CALIBRATION_MODULE_ID::AWB) {
        struct_list.append("ISP_AWB_Calibration_Gain_S");
        struct_get_api_map.insert("ISP_AWB_Calibration_Gain_S", "CVI_ISP_GetWBCalibration");
        struct_set_api_map.insert("ISP_AWB_Calibration_Gain_S", "CVI_ISP_SetWBCalibration");
        struct_list.append("ISP_WB_ATTR_S");
        struct_get_api_map.insert("ISP_WB_ATTR_S", "CVI_ISP_GetWBAttr");
        struct_set_api_map.insert("ISP_WB_ATTR_S", "CVI_ISP_SetWBAttr");
    } else if (static_cast<CALIBRATION_MODULE_ID>(module_id) == CALIBRATION_MODULE_ID::CCM) {
        struct_list.append("ISP_CCM_ATTR_S");
        struct_get_api_map.insert("ISP_CCM_ATTR_S", "CVI_ISP_GetCCMAttr");
        struct_set_api_map.insert("ISP_CCM_ATTR_S", "CVI_ISP_SetCCMAttr");
    } else if (static_cast<CALIBRATION_MODULE_ID>(module_id) == CALIBRATION_MODULE_ID::NOISE_PROFILE) {
        struct_list.append("ISP_CMOS_NOISE_CALIBRATION_S");
        struct_list.append("TEAISP_BNR_NP_S");
        struct_get_api_map.insert("ISP_CMOS_NOISE_CALIBRATION_S", "CVI_ISP_GetNoiseProfileAttr");
        struct_get_api_map.insert("TEAISP_BNR_NP_S", "CVI_TEAISP_BNR_GetNoiseProfileAttr");
        struct_set_api_map.insert("ISP_CMOS_NOISE_CALIBRATION_S", "CVI_ISP_SetNoiseProfileAttr");
        struct_set_api_map.insert("TEAISP_BNR_NP_S", "CVI_TEAISP_BNR_SetNoiseProfileAttr");
    } else if (static_cast<CALIBRATION_MODULE_ID>(module_id) == CALIBRATION_MODULE_ID::CLUT) {
        struct_list.append("ISP_CLUT_ATTR_S");
        struct_get_api_map.insert("ISP_CLUT_ATTR_S", "CVI_ISP_GetClutAttr");
        struct_set_api_map.insert("ISP_CLUT_ATTR_S", "CVI_ISP_SetClutAttr");
    } else if (static_cast<CALIBRATION_MODULE_ID>(module_id) == CALIBRATION_MODULE_ID::DISTORTION) {
    } else if (static_cast<CALIBRATION_MODULE_ID>(module_id) == CALIBRATION_MODULE_ID::LBLC) {
        struct_list.append("ISP_LBLC_LUT_ATTR_S");
        struct_get_api_map.insert("ISP_LBLC_LUT_ATTR_S", "CVI_ISP_GetLblcLutAttr");
        struct_set_api_map.insert("ISP_LBLC_LUT_ATTR_S", "CVI_ISP_SetLblcLutAttr");
    } else if (static_cast<CALIBRATION_MODULE_ID>(module_id) == CALIBRATION_MODULE_ID::RGBIR) {
        struct_list.append("ISP_RGBIR_ATTR_S");
        struct_get_api_map.insert("ISP_RGBIR_ATTR_S", "CVI_ISP_GetRgbirAttr");
        struct_set_api_map.insert("ISP_RGBIR_ATTR_S", "CVI_ISP_SetRgbirAttr");
    }
}

void BaseCalibrationPageWidget::addRawImgItem(RawImageItem *raw_img_item)
{
    CALIBRATION_MODULE_ID id = static_cast<CALIBRATION_MODULE_ID>(property("cali_id").toInt());

    if (raw_img_item != nullptr) {
        this->raw_img_item_vec.push_back(raw_img_item);
    }

    if (id == CALIBRATION_MODULE_ID::AWB) {
        emit sigImportAwbRaw();
    } else {
        displayInput();
        display_tab_widget->setCurrentWidget(input_img);
    }
}
void BaseCalibrationPageWidget::clearAllRawImgItem()
{
    this->raw_img_item_vec.clear();
}

bool BaseCalibrationPageWidget::calibration()
{
    return true;
}

void BaseCalibrationPageWidget::exportData(QString file_path)
{
    QJsonObject data;
    QJsonArray items = getCalibrationData();
    CALIBRATION_MODULE_ID id = static_cast<CALIBRATION_MODULE_ID>(property("cali_id").toInt());
    data[PREFIX_CALI_STR + getCaliModuleName(id)] = items;
    if (id == CALIBRATION_MODULE_ID::LSC) {
        bool is_export_rlsc = false;
        QString soc = GlobalData::getInstance()->getCvipqToolSoc();

        is_export_rlsc = (CV182X_SOC == soc) || (CV184X_SOC == soc);
        utils::saveLscJson(file_path, data, is_export_rlsc);
    } else if (id == CALIBRATION_MODULE_ID::LBLC) {
        utils::saveLblcJson(file_path, data);
    } else {
        utils::saveJson(file_path, data);
    }

}

void BaseCalibrationPageWidget::saveData(QString file_Path)
{
    if (base_cali_item_list.isEmpty()) {
        base_cali_item_list = this->findChildren<BaseCalibrationItemWidget *>();
    }

    QJsonObject data;
    QJsonObject items;
    for (auto cali_item : base_cali_item_list) {
        QString key = cali_item->getKey();
        if (key.isEmpty()) {
            continue;
        }

        QJsonObject reg;
        reg[KEY_VALUE] = cali_item->getData().toInt();
        items[key] = reg;
    }

    CALIBRATION_MODULE_ID id = static_cast<CALIBRATION_MODULE_ID>(property("cali_id").toInt());
    data[PREFIX_CALI_STR + getCaliModuleName(id)] = items;
    utils::saveJson(file_Path, data);
}

QJsonArray BaseCalibrationPageWidget::getCalibrationData()
{
    return QJsonArray();
}

void BaseCalibrationPageWidget::writeToIni()
{
    ///TODO: write calibration result to ini
}

QJsonObject BaseCalibrationPageWidget::GetModuleObjectToNetwork()
{
    QJsonObject send_obj;
    QJsonObject data_obj;
    QJsonArray items = getCalibrationData();

    if (!items.isEmpty()) {
        send_obj[MSG_KEY_CMD] = CMD_WRITE_MODULE;
        CALIBRATION_MODULE_ID id = static_cast<CALIBRATION_MODULE_ID>(property("cali_id").toInt());
        if (id == CALIBRATION_MODULE_ID::AWB) {
            // cali awb same as awb info
            data_obj[MSG_KEY_MODULE] = PREFIX_CALI_STR + getCaliModuleName(CALIBRATION_MODULE_ID::AWB);
            data_obj[MSG_KEY_OP_TYPE] = OP_TYPE_MANUAL;
        } else {
            data_obj[MSG_KEY_MODULE] = PREFIX_CALI_STR + getCaliModuleName(id);
            data_obj[MSG_KEY_OP_TYPE] = OP_TYPE_MANUAL;
        }
        data_obj[MSG_KEY_ITEMS] = items;
        send_obj[MSG_KEY_DATA] = data_obj;
    }

    return send_obj;
}

void BaseCalibrationPageWidget::writeToNetwork()
{
    CALIBRATION_MODULE_ID id = static_cast<CALIBRATION_MODULE_ID>(property("cali_id").toInt());

    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        emit sigStatusLog(getTitle() + " Page: Failed to write. Network error.", LOG_LEVEL::LOG_ERROR);
        return;
    }

    QJsonObject send_obj;
    send_obj = GetModuleObjectToNetwork();
    if (send_obj.contains(MSG_KEY_CMD)) {
        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
    }
}

void BaseCalibrationPageWidget::initLeftArea()
{
    CALIBRATION_MODULE_ID module_id = static_cast<CALIBRATION_MODULE_ID>(property("cali_id").toInt());

    if (module_id == CALIBRATION_MODULE_ID::AWB || module_id == CALIBRATION_MODULE_ID::QUICK_CALIRATION) {
        return;
    }
    param_layout = new QVBoxLayout;
    main_layout->addLayout(param_layout);
}

void BaseCalibrationPageWidget::initImportRawUI()
{
    QPushButton *import_raw_btn = new QPushButton(tr("Import selected raw"));
    param_layout->addWidget(import_raw_btn);
    connect(import_raw_btn, &QPushButton::clicked, [&]() {
        emit sigImportRaw();
    });
}

void BaseCalibrationPageWidget::initImportJpgUI()
{
    QHBoxLayout *input_image_layout = new QHBoxLayout;
    QPushButton *open_source_image_btn = new QPushButton(tr("Open Source Image"));
    connect(open_source_image_btn, &QPushButton::clicked, [=](){
        emit sigImportSourceJpg();
    });
    input_image_layout->addWidget(open_source_image_btn);
    QPushButton *open_target_image_btn = new QPushButton(tr("Open Target Image"));
    connect(open_target_image_btn, &QPushButton::clicked, [=](){
        emit sigImportTargetJpg();
    });
    input_image_layout->addWidget(open_target_image_btn);
    param_layout->addLayout(input_image_layout);
}

QPushButton *BaseCalibrationPageWidget::initImportBlcUI()
{
    QPushButton *import_black_level_cali_btn = new QPushButton(tr("Import BLC calibration data"));
    param_layout->addWidget(import_black_level_cali_btn);
    connect(import_black_level_cali_btn, &QPushButton::clicked, this, &BaseCalibrationPageWidget::handleImportData);

    return import_black_level_cali_btn;
}

QPushButton *BaseCalibrationPageWidget::initImportLscUI()
{
    QPushButton *import_lsc_cali_btn = new QPushButton(tr("Import LSC calibration data"));
    param_layout->addWidget(import_lsc_cali_btn);
    connect(import_lsc_cali_btn, &QPushButton::clicked, this, &BaseCalibrationPageWidget::handleImportData);

    return import_lsc_cali_btn;
}

QPushButton *BaseCalibrationPageWidget::initImportRgbirUI()
{
    QPushButton *import_rgbir_cali_btn = new QPushButton(tr("Import RGBIR calibration data"));
    param_layout->addWidget(import_rgbir_cali_btn);
    connect(import_rgbir_cali_btn, &QPushButton::clicked, this, &BaseCalibrationPageWidget::handleImportData);

    return import_rgbir_cali_btn;
}

void BaseCalibrationPageWidget::initCaliUI()
{
    QPushButton *calibration_btn = new QPushButton(tr("Calibration"));
    connect(calibration_btn, &QPushButton::clicked, [&]() {
        CALIBRATION_MODULE_ID module_id = static_cast<CALIBRATION_MODULE_ID>(property("cali_id").toInt());
        if (module_id == CALIBRATION_MODULE_ID::CLUT) {
            sigChangeCaliStatus(CALIBRATION_STATUS::CALIBRATION);
            bool cali_success = calibration();
            if (cali_success != true) {
                sigChangeCaliStatus(CALIBRATION_STATUS::CAL_ERROR);
                return;
            }
            sigChangeCaliStatus(CALIBRATION_STATUS::READY);
        } else {
            if (this->raw_img_item_vec.isEmpty()) {
                QMessageBox message(QMessageBox::NoIcon, "Error", "Please import raw first");
                message.exec();
                return;
            }

            if (module_id == CALIBRATION_MODULE_ID::LSC || module_id == CALIBRATION_MODULE_ID::NOISE_PROFILE
                || module_id == CALIBRATION_MODULE_ID::CCM || module_id == CALIBRATION_MODULE_ID::RGBIR) {
                if (!blc_imported) {
                    QMessageBox message(QMessageBox::NoIcon, "Error", "Must import BLC data first!");
                    message.exec();
                    return;
                }
            }

            sigChangeCaliStatus(CALIBRATION_STATUS::CALIBRATION);
            if (module_id != CALIBRATION_MODULE_ID::NOISE_PROFILE) {
                display_tab_widget->setCurrentWidget(output_img);
            }
            bool cali_success = calibration();
            if (cali_success != true) {
                sigChangeCaliStatus(CALIBRATION_STATUS::CAL_ERROR);
                return;
            }

            emit sigCalibration();
        }
        sigChangeCaliStatus(CALIBRATION_STATUS::READY);
        QMessageBox::information(this, "information", "Calibration Success!", QMessageBox::Ok, QMessageBox::NoButton);
    });

    param_layout->addWidget(calibration_btn);
}

void BaseCalibrationPageWidget::initDisplayDataUI()
{
    /*QPushButton *display_btn = new QPushButton(tr("Display calibration data"));
    param_layout->addWidget(display_btn);*/
}

void BaseCalibrationPageWidget::initWriteDataUI()
{
    QPushButton *write_data_btn = new QPushButton(tr("Write calibration data"));
    connect(write_data_btn, &QPushButton::clicked, this, std::bind(&BaseCalibrationPageWidget::writeCalibrationData, this, true));
    param_layout->addWidget(write_data_btn);
}

void BaseCalibrationPageWidget::initExportDataUI()
{
    QPushButton *export_btn = new QPushButton(tr("Export calibration data"));
    connect(export_btn, &QPushButton::clicked, this, &BaseCalibrationPageWidget::handleExportData);
    param_layout->addWidget(export_btn);
}

void BaseCalibrationPageWidget::initImportDataUI()
{
    QPushButton *import_btn = new QPushButton(tr("Import calibration data"));
    connect(import_btn, &QPushButton::clicked, this, &BaseCalibrationPageWidget::handleImportData);
    param_layout->addWidget(import_btn);
}

void BaseCalibrationPageWidget::initCTListUI()
{
    ct_list_widget = new CalibrationCTListWidget;
    param_layout->addWidget(ct_list_widget);

    connect(this, &BaseCalibrationPageWidget::sigCalibration, [=]() {
        if (ct_list_widget->getCTList().size() == CalibrationCTListWidget::MAX_CT_ITEM_NUM) {
            QMessageBox message(QMessageBox::NoIcon, "Error", "Can only add " +
                                QString::number(CalibrationCTListWidget::MAX_CT_ITEM_NUM) + " ct items");
            message.exec();
            return;
        }

        if (this->raw_img_item_vec.isEmpty()) {
            QMessageBox message(QMessageBox::NoIcon, "Error", "Please import raw first");
            message.exec();
            return;
        }

        CTItem *ct_item = new CTItem(this->raw_img_item_vec.first()->getFileName());
        bool success = ct_list_widget->addTableItem(ct_item);
        if (!success) {
            QMessageBox message(QMessageBox::NoIcon, "Error", "Add ct item failed");
            message.exec();
            return;
        }
        ct_item->setInputPixmap(*this->input_image_label->pixmap());
        ct_item->setOutputPixmap(*this->output_image_label->pixmap());
        ct_item->setDisplay(Qt::CheckState::Checked);
    });

    connect(ct_list_widget, &CalibrationCTListWidget::sigSetInputPixmap, [=](QPixmap *pixmap) {
        if (pixmap != nullptr) this->input_img->setImg(*pixmap);
    });

    connect(ct_list_widget, &CalibrationCTListWidget::sigSetOutputPixmap, [=](QPixmap *pixmap) {
        if (pixmap != nullptr) this->output_img->setImg(*pixmap);
    });
}

QWidget *BaseCalibrationPageWidget::createRoiControlUI(ImageLabel *img_label)
{
    if (img_label == nullptr) return nullptr;

    QWidget *roi_control_widget = new QWidget();
    roi_control_widget->setWindowFlags(Qt::WindowCloseButtonHint|Qt::WindowStaysOnTopHint);
    roi_control_widget->setFixedSize(250, 400);

    QPushButton *up = new QPushButton();
    up->setIcon(QIcon(":/icons/control_arrow_up.png"));
    QPushButton *down = new QPushButton();
    down->setIcon(QIcon(":/icons/control_arrow_down.png"));
    QPushButton *left = new QPushButton();
    left->setIcon(QIcon(":/icons/control_arrow_left.png"));
    QPushButton *right = new QPushButton();
    right->setIcon(QIcon(":/icons/control_arrow_right.png"));
    QPushButton *scale_up = new QPushButton();
    scale_up->setIcon(QIcon(":/icons/zoom_in.png"));
    QPushButton *scale_down = new QPushButton();
    scale_down->setIcon(QIcon(":/icons/zoom_out.png"));

    QPushButton *v_space_up = new QPushButton("+");
    QPushButton *v_space_down = new QPushButton("-");
    QPushButton *h_space_up = new QPushButton("+");
    QPushButton *h_space_down = new QPushButton("-");
    QPushButton *reset_btn = new QPushButton("reset");
    QPushButton *h_interval_up = new QPushButton("+");
    QPushButton *v_interval_up = new QPushButton("+");
    QPushButton *h_interval_down = new QPushButton("-");
    QPushButton *v_interval_down = new QPushButton("-");

    connect(up, &QPushButton::clicked, this, [=]() {
        img_label->up();
    });
    connect(down, &QPushButton::clicked, this, [=]() {
        img_label->down();
    });
    connect(left, &QPushButton::clicked, this, [=]() {
        img_label->left();
    });
    connect(right, &QPushButton::clicked, this, [=]() {
        img_label->right();
    });
    connect(scale_up, &QPushButton::clicked, this, [=]() {
        img_label->large();
    });
    connect(scale_down, &QPushButton::clicked, this, [=]() {
        img_label->small();
    });

    connect(v_space_up, &QPushButton::clicked, this, [=]() {
        img_label->vspaceLarge();
    });
    connect(v_space_down, &QPushButton::clicked, this, [=]() {
        img_label->vspaceSmall();
    });

    connect(h_space_up, &QPushButton::clicked, this, [=]() {
        img_label->hspaceLarge();
    });
    connect(h_space_down, &QPushButton::clicked, this, [=]() {
        img_label->hspaceSmall();
    });

    connect(reset_btn, &QPushButton::clicked, this, [=]() {
        img_label->reset();
    });

    connect(h_interval_down, &QPushButton::clicked, this, [=]() {
        img_label->hIntervalSmall();
    });

    connect(h_interval_up, &QPushButton::clicked, this, [=]() {
        img_label->hIntervalLarge();
    });

    connect(v_interval_down, &QPushButton::clicked, this, [=]() {
        img_label->vIntervalSmall();
    });

    connect(v_interval_up, &QPushButton::clicked, this, [=]() {
        img_label->vIntervalLarge();
    });

    QHBoxLayout *h_layout = new QHBoxLayout();
    QVBoxLayout *v_layout = new QVBoxLayout();
    h_layout->setAlignment(Qt::AlignCenter);
    v_layout->setAlignment(Qt::AlignCenter);

    v_layout->addWidget(up);
    v_layout->addWidget(scale_up);
    v_layout->addWidget(scale_down);
    v_layout->addWidget(down);

    h_layout->addWidget(left);
    h_layout->addLayout(v_layout);
    h_layout->addWidget(right);

    v_layout = new QVBoxLayout();
    v_layout->setAlignment(Qt::AlignCenter);
    v_layout->addLayout(h_layout);

    QHBoxLayout *vspace_layout = new QHBoxLayout;
    QLabel *label_vspace = new QLabel("v space   ");
    vspace_layout->addWidget(label_vspace);
    vspace_layout->addWidget(v_space_down);
    vspace_layout->addWidget(v_space_up);
    v_layout->addLayout(vspace_layout);

    QHBoxLayout *hspace_layout = new QHBoxLayout;
    QLabel *label_hspace = new QLabel("h space   ");
    hspace_layout->addWidget(label_hspace);
    hspace_layout->addWidget(h_space_down);
    hspace_layout->addWidget(h_space_up);
    v_layout->addLayout(hspace_layout);

    QHBoxLayout *vinterval_layout = new QHBoxLayout;
    QLabel *label_vinterval = new QLabel("v interval");
    vinterval_layout->addWidget(label_vinterval);
    vinterval_layout->addWidget(v_interval_down);
    vinterval_layout->addWidget(v_interval_up);
    v_layout->addLayout(vinterval_layout);

    QHBoxLayout *hinterval_layout = new QHBoxLayout;
    QLabel *label_hinterval = new QLabel("h interval");
    hinterval_layout->addWidget(label_hinterval);
    hinterval_layout->addWidget(h_interval_down);
    hinterval_layout->addWidget(h_interval_up);
    v_layout->addLayout(hinterval_layout);

    v_layout->addWidget(reset_btn);

    v_layout->addSpacing(10);

    roi_control_widget->setLayout(v_layout);

    return roi_control_widget;
}

void BaseCalibrationPageWidget::initCenterArea()
{
    CALIBRATION_MODULE_ID module_id = static_cast<CALIBRATION_MODULE_ID>(property("cali_id").toInt());

    if (module_id == CALIBRATION_MODULE_ID::AWB
        || module_id == CALIBRATION_MODULE_ID::DISTORTION
        || module_id == CALIBRATION_MODULE_ID::QUICK_CALIRATION) {
        return;
    } else if (module_id == CALIBRATION_MODULE_ID::CLUT) {
        QGridLayout *clut_layout = new QGridLayout;

        // input image display
        input_display_tab_widget = new QTabWidget;

        /*input_raw = new ImageViewerWidget();
        input_raw_label = new ImageLabel(this, ImageLabel::COLOR_CHECKER);
        input_raw->setImageLabel(input_raw_label);
        connect(this, &BaseCalibrationPageWidget::sigShowInputRawItem, input_raw, &ImageViewerWidget::handleShowRawItem);
        input_raw->setFilePathHidden();
        input_display_tab_widget->addTab(input_raw, "InputRaw");*/

        input_jpg = new ImageViewerWidget();
        input_jpg_label = new ImageLabel(this, ImageLabel::COLOR_CHECKER);
        input_jpg->setImageLabel(input_jpg_label);
        connect(this, &BaseCalibrationPageWidget::sigImportSourceJpg, input_jpg, &ImageViewerWidget::openJpg);
        input_jpg->setFilePathHidden();
        input_display_tab_widget->addTab(input_jpg, "InputJpg");

        clut_layout->addWidget(input_display_tab_widget, 0, 0);
        // ouput image display
        output_display_tab_widget = new QTabWidget;

        output_img = new ImageViewerWidget();
        output_image_label = new ImageLabel(this, ImageLabel::COLOR_CHECKER);
        connect(this, &BaseCalibrationPageWidget::sigImportTargetJpg, output_img, &ImageViewerWidget::openJpg);
        //connect(this, &BaseCalibrationPageWidget::sigShowOutputRawImage, output_img, &ImageViewerWidget::handleShowImage);
        output_img->setImageLabel(output_image_label);
        output_img->setFilePathHidden();
        output_display_tab_widget->addTab(output_img, "Target");

        clut_layout->addWidget(output_display_tab_widget, 0, 1);

        rgb_list_widget = new CalibrationRGBListWidget;
        clut_layout->addWidget(rgb_list_widget, 1, 0, 3, 2);

        main_layout->addLayout(clut_layout);
    } else {
        display_tab_widget = new QTabWidget;
        // input image display
        input_img = new ImageViewerWidget();
        if ((module_id == CALIBRATION_MODULE_ID::NOISE_PROFILE) ||
            (module_id == CALIBRATION_MODULE_ID::CCM) || (module_id == CALIBRATION_MODULE_ID::RGBIR)) {
            input_image_label = new ImageLabel(this, ImageLabel::COLOR_CHECKER);
        } else {
            input_image_label = new ImageLabel(this, ImageLabel::NORMAL);
        }
        input_img->setImageLabel(input_image_label);
        connect(this, &BaseCalibrationPageWidget::sigShowInputRawItem, input_img, &ImageViewerWidget::handleShowRawItem);
        input_img->setFilePathHidden();
        display_tab_widget->addTab(input_img, "Input");
        // ouput image display
        if (module_id == CALIBRATION_MODULE_ID::BLC) {
            blc_output_img_median = new ImageViewerWidget();
            blc_output_image_label_median = new ImageLabel(this, ImageLabel::NORMAL);
            connect(this, &BaseCalibrationPageWidget::sigShowOutputRawImage, blc_output_img_median, &ImageViewerWidget::handleShowImage);
            blc_output_img_median->setImageLabel(blc_output_image_label_median);
            blc_output_img_median->setFilePathHidden();
            display_tab_widget->addTab(blc_output_img_median, "Output Median");

            blc_output_img_mean = new ImageViewerWidget();
            blc_output_image_label_mean = new ImageLabel(this, ImageLabel::NORMAL);
            connect(this, &BaseCalibrationPageWidget::sigShowOutputBlcImage, blc_output_img_mean, &ImageViewerWidget::handleShowImage);
            blc_output_img_mean->setImageLabel(blc_output_image_label_mean);
            blc_output_img_mean->setFilePathHidden();
            display_tab_widget->addTab(blc_output_img_mean, "Output Mean");

            blc_histogram = new BlcHistogramWidget();
            display_tab_widget->addTab(blc_histogram, "Histogram");
        } else {
            if (module_id != CALIBRATION_MODULE_ID::LBLC) {
                output_img = new ImageViewerWidget();
                output_image_label = new ImageLabel(this, ImageLabel::NORMAL);
                connect(this, &BaseCalibrationPageWidget::sigShowOutputRawImage, output_img, &ImageViewerWidget::handleShowImage);
                output_img->setImageLabel(output_image_label);
                output_img->setFilePathHidden();
                display_tab_widget->addTab(output_img, "Output");
            }
        }
        main_layout->addWidget(display_tab_widget);
    }
}

void BaseCalibrationPageWidget::initRightArea()
{
}

void BaseCalibrationPageWidget::retranslateUi()
{
    if (this->title.isEmpty()) {
        CALIBRATION_MODULE_ID id = static_cast<CALIBRATION_MODULE_ID>(property("cali_id").toInt());
        QString title = getCaliModuleName(id);
        setTitle(title);
    } else {
        setTitle(this->title);
    }
}

void BaseCalibrationPageWidget::displayInput()
{
    if (raw_img_item_vec.isEmpty()) return;
    emit sigShowInputRawItem(raw_img_item_vec.first());
}

void BaseCalibrationPageWidget::displayOutput()
{
}

int BaseCalibrationPageWidget::writeCalibrationData(bool showMessage)
{
    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        if (showMessage) {
            QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        }
        return -1;
    }

    CALIBRATION_MODULE_ID module_id = static_cast<CALIBRATION_MODULE_ID>(property("cali_id").toInt());
    if (!checkShallExport()) {
        QString log;
        switch (module_id) {
        case CALIBRATION_MODULE_ID::NOISE_PROFILE:
            log = "Please check selected calibration items. Only support 1~16 different ISO items.";
            break;
        case CALIBRATION_MODULE_ID::AWB:
            log = "Please check selected calibration items. Only support 3 different CT items";
            break;
        case CALIBRATION_MODULE_ID::CCM:
            log = "Please check selected calibration items. Only support 3~7 different CT items";
            break;
        case CALIBRATION_MODULE_ID::LSC:
            log = "Please check selected calibration items.. Only support 1~7 different CT items";
            break;
        default:
            log = "Please calibrate first.";
            break;
        }

        if (showMessage) {
            QMessageBox message(QMessageBox::NoIcon, "Error", log);
            message.exec();
        }
        return -1;
    }

    setStatusBusy(true);

    int ret = sendDataToNetwork(showMessage);

    setStatusBusy(false);

    return ret;
}

void BaseCalibrationPageWidget::handleImportData()
{
    QString import_file_path = QFileDialog::getOpenFileName(this,
          tr("Open Data File"), "", "*.json");
    QJsonObject data;
    QString error_msg;
    if (utils::loadJson(import_file_path, data, error_msg) != 0) {
        QMessageBox::critical(this, "Error", error_msg, QMessageBox::Ok);
        return;
    }
    QStringList strList = import_file_path.split("/");
    QString name = strList[strList.size()-1];

    emit sigImportData(name, data);
}

void BaseCalibrationPageWidget::handleExportData()
{
    QString file_ext = "*.json";
    CALIBRATION_MODULE_ID module_id = static_cast<CALIBRATION_MODULE_ID>(property("cali_id").toInt());
    if (module_id == CALIBRATION_MODULE_ID::NOISE_PROFILE) {
        if (!checkShallExport()) {
            QMessageBox message(QMessageBox::NoIcon, "Error", "Please check selected calibration items. Only support 1~16 different ISO items.");
            message.exec();
            return;
        }
    } else if (module_id == CALIBRATION_MODULE_ID::AWB)
    {
        if (!checkShallExport()) {
            QMessageBox message(QMessageBox::NoIcon, "Error", "Please check selected calibration items. Only support 3 different CT items");
            message.exec();
            return;
        }
    } else if (module_id == CALIBRATION_MODULE_ID::CCM) {
        if (!checkShallExport()) {
            QMessageBox message(QMessageBox::NoIcon, "Error", "Please check selected calibration items. Only support 3~7 different CT items");
            message.exec();
            return;
        }
    } else if (module_id == CALIBRATION_MODULE_ID::LSC)
    {
        if (!checkShallExport()) {
            QMessageBox message(QMessageBox::NoIcon, "Error", "Please check selected calibration items.. Only support 1~7 different CT items");
            message.exec();
            return;
        }
    } else if ((module_id == CALIBRATION_MODULE_ID::BLC) || (module_id == CALIBRATION_MODULE_ID::LBLC)) {
        if (!checkShallExport()) {
            QMessageBox message(QMessageBox::NoIcon, "Error", "Please calibrate first.");
            message.exec();
            return;
        }
    } else if (module_id == CALIBRATION_MODULE_ID::DISTORTION) {
        if (!checkShallExport()) {
            QMessageBox message(QMessageBox::NoIcon, "Error", "Please calibrate first (DISTORTION).");
             }
    } else if (module_id == CALIBRATION_MODULE_ID::RGBIR) {
        if (!checkShallExport()) {
            QMessageBox message(QMessageBox::NoIcon, "Error", "Please calibrate first.");
            message.exec();
            return;
        }
    }

    // export calibration result data
    QString export_file_path = QFileDialog::getSaveFileName(this, tr("Open File"), "", file_ext);
    exportData(export_file_path);
}

void BaseCalibrationPageWidget::handleSave()
{
    // save calibration param data
    QString save_file_path = QFileDialog::getSaveFileName(this, tr("Open File"), "", "*.json");
    saveData(save_file_path);
}

void BaseCalibrationPageWidget::setAcceptScene(SCENE s)
{
    accept_scene = s;
}

SCENE BaseCalibrationPageWidget::getAcceptScene(void)
{
    return accept_scene;
}

bool BaseCalibrationPageWidget::checkShallExport()
{
    return true;
}

int BaseCalibrationPageWidget::sendDataToNetwork(bool showMessage)
{
    CALIBRATION_MODULE_ID module_id = static_cast<CALIBRATION_MODULE_ID>(property("cali_id").toInt());
    if (module_id == CALIBRATION_MODULE_ID::BLC || module_id == CALIBRATION_MODULE_ID::NOISE_PROFILE
        || module_id == CALIBRATION_MODULE_ID::AWB || module_id == CALIBRATION_MODULE_ID::RGBIR
        || module_id == CALIBRATION_MODULE_ID::CCM) {
        int ret = readCalibrationData(showMessage);
        if (ret < 0) {
            return -1;
        }
    } else if (module_id == CALIBRATION_MODULE_ID::LSC) {
        if (GlobalData::getInstance()->getCvipqToolSoc() == CV184X_SOC) {
            int ret = readCalibrationData(showMessage);
            if (ret < 0) {
                return -1;
            }
        }
    }

    // return;
    QJsonObject module_obj = GetModuleObjectToNetworkJsonrpc();
    for (auto struct_id : struct_list) {
        if (module_id == CALIBRATION_MODULE_ID::NOISE_PROFILE) {
            if (!getCalibrationBnrBoxState() && (struct_id == "TEAISP_BNR_NP_S")) {
                continue;
            }
        }

        QJsonObject send_obj = module_obj[struct_id].toObject();
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
            GlobalData::getInstance()->setWaitForResponse(false);
            if (showMessage) {
                QString message = getTitle() + QString(" Page: Failed to write %1. Timeout: Can't get reponse from board within %2 seconds.").arg(struct_id).arg(READ_WRITE_TIMEOUT);
                emit sigStatusLog(message, LOG_LEVEL::LOG_ERROR);
            }
            return -1;
        }
    }

    return 0;
}

int BaseCalibrationPageWidget::readCalibrationData(bool showMessage)
{
    qDebug("BaseCalibrationPageWidget::readCalibrationData");
    Q_UNUSED(showMessage);
    return 0;
}

QJsonObject BaseCalibrationPageWidget::GetModuleObjectToNetworkJsonrpc()
{
    qDebug("BaseCalibrationPageWidget::GetModuleObjectToNetworkJsonrpc");
}

void BaseCalibrationPageWidget::SetCalibrationBtnState(bool state)
{

}

bool BaseCalibrationPageWidget::getCalibrationBnrBoxState()
{
    return true;
}
