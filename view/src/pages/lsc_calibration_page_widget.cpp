#include "lsc_calibration_page_widget.hpp"
#include "calibration_ct_list_widget.hpp"
#include "otp_pdaf_cali.h"
#include "utils.hpp"
#include "raw_image_format_dialog.hpp"
#include "network_client_helper.hpp"
#include <QMessageBox>
#include <QFileDialog>
#include <QShortcut>

LSCCalibrationPageWidget::LSCCalibrationPageWidget(QWidget *parent) : BaseCalibrationPageWidget(static_cast<int>(CALIBRATION_MODULE_ID::LSC), parent)
{
    setAcceptScene(SCENE::UNIFORM_COLOR);
    std::fill_n(mlsc_r_gain.get(), MLSC_TABLE_SIZE, 512);
    std::fill_n(mlsc_g_gain.get(), MLSC_TABLE_SIZE, 512);
    std::fill_n(mlsc_b_gain.get(), MLSC_TABLE_SIZE, 512);
    std::fill_n(rlsc_r_gain.get(), RLSC_TABLE_SIZE, 512);
    std::fill_n(rlsc_g_gain.get(), RLSC_TABLE_SIZE, 512);
    std::fill_n(rlsc_b_gain.get(), RLSC_TABLE_SIZE, 512);
    std::fill_n(rlsc_ir_gain.get(), RLSC_TABLE_SIZE, 512);
    initLeftArea();
    initEvent();
    retranslateUi();
    handleShorCut();
}

void LSCCalibrationPageWidget::ClearData()
{
    for (int i = 0; i < rawItem.size(); i++) {
        if (rawItem[i] != nullptr) {
            delete rawItem[i];
        }
    }
    rawItem.clear();
    for (int i = 0; i < ctItem_list.size(); i++) {
        if (ctItem_list[i] != nullptr) {
            delete ctItem_list[i];
        }
    }
    ctItem_list.clear();
    ctItem1_list.clear();
    blc_imported = false;
    raw_img_item_vec.clear();
    quickMode = false;
    cali_status = CALIBRATION_STATUS::READY;
}

void LSCCalibrationPageWidget::initLeftArea()
{
    initImportRawUI();
    initImportBlcUI();

    grid_number_x_item = new LineEditCaliItemWidget("Grid number X");
    grid_number_x_item->setKey("grid_number_x");
    grid_number_x_item->setData(37);
    grid_number_x_item->setEnable(false);
   // param_layout->addWidget(grid_number_x_item);
    grid_number_y_item = new LineEditCaliItemWidget("Grid number Y");
    grid_number_y_item->setKey("grid_number_y");
    grid_number_y_item->setData(37);
    grid_number_y_item->setEnable(false);
    //param_layout->addWidget(grid_number_y_item);
    /*add calib type option*/
    calib_type = new ComboboxCaliItemWidget("Calibration type:");
    QStringList calib_type_list = {"All", "Chroma", "Luma"};
    for (int i = 0; i < calib_type_list.size(); i++) {
        calib_type->addItem(calib_type_list[i]);
    }
    calib_type->setCurrentIndex(0);
    param_layout->addWidget(calib_type);
    /*add fisheye type option*/
    len_type = new ComboboxCaliItemWidget("Lens type:");
    QStringList len_type_list = {"No fisheye", "Fisheye"};
    for (int i = 0; i < len_type_list.size(); i++) {
        len_type->addItem(len_type_list[i]);
    }
    len_type->setCurrentIndex(0);
    param_layout->addWidget(len_type);
    // mars3 calibration algo type
    cv184x_calibration_algo = new ComboboxCaliItemWidget("cv184x lsc calibration algo:");
    QStringList lsc_algo_type_ls = {"mars3", "mars", "mars_debug"};
    for (int i = 0; i < lsc_algo_type_ls.size(); i++) {
        cv184x_calibration_algo->addItem(lsc_algo_type_ls[i]);
    }
    cv184x_calibration_algo->setCurrentIndex(0);
    param_layout->addWidget(cv184x_calibration_algo);
    cv184x_calibration_algo->hide();
    /***/
    analyze_fisheye_btn = new QPushButton;
    //param_layout->addWidget(analyze_fisheye_btn);
    center_x_item = new LineEditCaliItemWidget("Center X");
    center_x_item->setKey("center_x");
    center_x_item->setData(200);
    //param_layout->addWidget(center_x_item);
    center_y_item = new LineEditCaliItemWidget("Center Y");
    center_y_item->setKey("center_y");
    center_y_item->setData(200);
    //param_layout->addWidget(center_y_item);
    radius_item = new LineEditCaliItemWidget("Radius");
    radius_item->setKey("radius");
    radius_item->setData(200);
    //param_layout->addWidget(radius_item);

    initCaliUI();
    initWriteDataUI();
    initImportDataUI();
    initExportDataUI();
    initQuickCalibrationUI();
    initCTListUI();
}

void LSCCalibrationPageWidget::handleShorCut()
{
     new QShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S), this, SLOT(handleLscSetting()));
}

void LSCCalibrationPageWidget::initQuickCalibrationUI()
{
    QGroupBox *quick_group = new QGroupBox(tr("Quick Calibration"));
    QVBoxLayout *v_layout = new QVBoxLayout;
    quick_group->setLayout(v_layout);
    quick_group->setMaximumHeight(120);

    QHBoxLayout *path_layout = new QHBoxLayout;
    open_btn = new QPushButton("Open path");
    connect(open_btn, &QPushButton::clicked, this, &LSCCalibrationPageWidget::OpenFilePath);
    path_edit = new QLineEdit;
    path_edit->setPlaceholderText("For example, D:/../LSC&AWB");
    path_edit->setReadOnly(true);
    path_layout->addWidget(open_btn);
    path_layout->addWidget(path_edit);

    quick_btn = new QPushButton("Calibration");
    connect(quick_btn, &QPushButton::clicked, this, std::bind(&LSCCalibrationPageWidget::handleQuickCalibration, this, true));

    v_layout->addLayout(path_layout, Qt::AlignTop);
    v_layout->addWidget(quick_btn, Qt::AlignTop);

    param_layout->addWidget(quick_group);
}

void LSCCalibrationPageWidget::OpenFilePath()
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

int LSCCalibrationPageWidget::handleQuickCalibration(bool showMessage)
{
    if (rawItem.isEmpty()) {
        if (showMessage) {
            QMessageBox::warning(this, tr("Warning"), tr("Please open the file directory first!"), QMessageBox::Ok);
        }
        return -1;
    }

    if (!blc_imported) {
        if (showMessage) {
            QMessageBox::warning(this, tr("Warning"), tr("Please import blc calibration data first!"), QMessageBox::Ok);
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
        QString path = rawItem[i]->getFilePath();
        QString name = rawItem[i]->getFileName();
        QStringList strList = path.split("/");
        QString dir_name = strList[strList.size()-2];
        int startIdx = dir_name.indexOf("(") + 1;
        int endIdx = dir_name.indexOf("K");
        int ct = dir_name.mid(startIdx, endIdx-startIdx).toInt();
        path.remove(path.length()-name.length(), name.length());
        path += "lsc.json";
        qDebug()<<path;
        ctItem_list.clear();
        CTItem *item = new CTItem(rawItem[i]->getFileName());
        item->setLSC(mlsc_r_gain.get(), mlsc_g_gain.get(), mlsc_b_gain.get(), rlsc_r_gain.get(),
                     rlsc_g_gain.get(), rlsc_b_gain.get(), rlsc_ir_gain.get(), center_x, center_y, norm);
        item->setCT(ct);
        ctItem_list.append(item);
        ctItem1_list.append(item);
        QJsonObject data;
        QJsonArray items = getCalibrationData();
        data["Cali LSC"] = items;
        utils::saveLscJson(path, data);
    }

    ctItem_list.clear();
    ctItem_list = ctItem1_list;
    int ret = writeCalibrationData(showMessage);
    ClearData();
    path_edit->clear();
    if (showMessage) {
        emit sigChangeCaliStatus(CALIBRATION_STATUS::READY);
        QMessageBox::information(this, tr("Info"), "Calibration finished!", QMessageBox::Ok);
    }

    if (ret < 0) {
        return -2;
    }

    return 0;
}

void LSCCalibrationPageWidget::retranslateUi()
{
    BaseCalibrationPageWidget::retranslateUi();

    analyze_fisheye_btn->setText(tr("Analyze fisheye param"));

    QString soc = GlobalData::getInstance()->getCvipqToolSoc();

    if (soc == CV184X_SOC && cv184x_calibration_algo->isHidden()) {
        cv184x_calibration_algo->show();
    }
}

void LSCCalibrationPageWidget::initEvent()
{
    connect(this, &BaseCalibrationPageWidget::sigImportData, this, &LSCCalibrationPageWidget::handleImportData);
    shared_ptr<NetworkClientHelper> network_helper = NetworkClientHelper::getInstance();
    connect(network_helper.get(), &NetworkClientHelper::sigReceiveRlscCalibration,
        this, &LSCCalibrationPageWidget::handleReceiveRlscCalibration);
    connect(network_helper.get(), &NetworkClientHelper::sigReceiveMlscCalibration,
        this, &LSCCalibrationPageWidget::handleReceiveMlscCalibration);

    connect(cv184x_calibration_algo->getCombobox(),  QOverload<int>::of(&QComboBox::currentIndexChanged),
                     [this](int index) {
        // for export calibration data
        //is_cv184x_mlsc_chroma_luma = (index > 0);
        // for ccm import lsc flag
        emit this->sigCv184xMlscChromaLumaFlag(index > 0);
    });
}

void LSCCalibrationPageWidget::handleImportData(QString name, QJsonObject data)
{
    qDebug()<<"LSCCalibrationPageWidget import file: "<<name;

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

    QString lsc_module_name = PREFIX_CALI_STR + getCaliModuleName(CALIBRATION_MODULE_ID::LSC);
    if (data.contains(lsc_module_name)) {
        int ct = 1000;
        QJsonArray lsc_data_array = data[lsc_module_name].toArray();
        for (auto lsc_data : lsc_data_array) {
            QJsonObject lsc_data_obj = lsc_data.toObject();
            if (lsc_data_obj[KEY_REG_KEY] == "CT") {
                ct = lsc_data_obj[KEY_VALUE].toInt();
            } else if (lsc_data_obj[KEY_REG_KEY] == "lsc_r_gain") {
                QJsonArray value_array = lsc_data_obj[KEY_VALUE].toArray();
                int *lsc_r_gain_array = mlsc_r_gain.get();
                for (int i = 0; i < value_array.size(); i++) {
                    if (i > MLSC_TABLE_SIZE) break;
                    lsc_r_gain_array[i] = value_array[i].toInt();
                }
            } else if (lsc_data_obj[KEY_REG_KEY] == "lsc_g_gain") {
                QJsonArray value_array = lsc_data_obj[KEY_VALUE].toArray();
                int *lsc_g_gain_array = mlsc_g_gain.get();
                for (int i = 0; i < value_array.size(); i++) {
                    if (i > MLSC_TABLE_SIZE) break;
                    lsc_g_gain_array[i] = value_array[i].toInt();
                }
            } else if (lsc_data_obj[KEY_REG_KEY] == "lsc_b_gain") {
                QJsonArray value_array = lsc_data_obj[KEY_VALUE].toArray();
                int *lsc_b_gain_array = mlsc_b_gain.get();
                for (int i = 0; i < value_array.size(); i++) {
                    if (i > MLSC_TABLE_SIZE) break;
                    lsc_b_gain_array[i] = value_array[i].toInt();
                }
            } else if (lsc_data_obj[KEY_REG_KEY] == "center_x") {
                center_x = lsc_data_obj[KEY_VALUE].toInt();
            } else if (lsc_data_obj[KEY_REG_KEY] == "center_y") {
                center_y = lsc_data_obj[KEY_VALUE].toInt();
            } else if (lsc_data_obj[KEY_REG_KEY] == "norm") {
                norm = lsc_data_obj[KEY_VALUE].toInt();
            } else if (lsc_data_obj[KEY_REG_KEY] == "rlsc_r_gain") {
                QJsonArray value_array = lsc_data_obj[KEY_VALUE].toArray();
                int *lsc_r_gain_array = rlsc_r_gain.get();
                for (int i = 0; i < value_array.size(); i++) {
                    if (i > RLSC_TABLE_SIZE) break;
                    lsc_r_gain_array[i] = value_array[i].toInt();
                }
            } else if (lsc_data_obj[KEY_REG_KEY] == "rlsc_g_gain") {
                QJsonArray value_array = lsc_data_obj[KEY_VALUE].toArray();
                int *lsc_g_gain_array = rlsc_g_gain.get();
                for (int i = 0; i < value_array.size(); i++) {
                    if (i > RLSC_TABLE_SIZE) break;
                    lsc_g_gain_array[i] = value_array[i].toInt();
                }
            } else if (lsc_data_obj[KEY_REG_KEY] == "rlsc_b_gain") {
                QJsonArray value_array = lsc_data_obj[KEY_VALUE].toArray();
                int *lsc_b_gain_array = rlsc_b_gain.get();
                for (int i = 0; i < value_array.size(); i++) {
                    if (i > RLSC_TABLE_SIZE) break;
                    lsc_b_gain_array[i] = value_array[i].toInt();
                }
            } else if (lsc_data_obj[KEY_REG_KEY] == "rlsc_ir_gain") {
                QJsonArray value_array = lsc_data_obj[KEY_VALUE].toArray();
                int *lsc_ir_gain_array = rlsc_ir_gain.get();
                for (int i = 0; i < value_array.size(); i++) {
                    if (i > RLSC_TABLE_SIZE) break;
                    lsc_ir_gain_array[i] = value_array[i].toInt();
                }
            }
        }
        CTItem *ct_item = new CTItem(name);
        ct_item->setLSC(mlsc_r_gain.get(), mlsc_g_gain.get(), mlsc_b_gain.get(),
                        rlsc_r_gain.get(), rlsc_g_gain.get(), rlsc_b_gain.get(), rlsc_ir_gain.get(),
                        center_x, center_y, norm);
        ct_item->setCT(ct);
        ct_item->setDisplay(Qt::CheckState::Checked);
        bool success = ct_list_widget->addTableItem(ct_item);
        if (!success) {
            QMessageBox message(QMessageBox::NoIcon, "Error", "Import lsc calibration data, Add ct item failed");
            message.exec();
            return;
        }
    }
}


bool LSCCalibrationPageWidget::calibration()
{
    if (raw_img_item_vec.isEmpty()) {
        return false;
    }

    using utils::RAW_BAYER_ID;
    using utils::RAW_BITS;

    RawImageItem * raw_img_item = this->raw_img_item_vec.first();
    int order = raw_img_item->getBayer().toInt();
    int width = raw_img_item->getWidth();
    int height = raw_img_item->getHeight();

    QString raw_path = raw_img_item->getFilePath();
    bool wdr_mode = raw_img_item->getFrameMode() == FRAME_MODE::WDR ? true : false;
    unique_ptr<int []> image = utils::readRawIntData(raw_path, width, height, wdr_mode);
    if (image == nullptr) {
        return false;
    }

    cali_status = CALIBRATION_STATUS::CALIBRATION;

    // calibration params
    int NumKnotX = grid_number_x_item->getData().toInt(),
        NumKnotY = grid_number_y_item->getData().toInt();
    int calib_flag = calib_type->currentIndex();
    int len_flag = len_type->currentIndex();
    int fisheye_flag = 0;
    int *lsc_radius_gain = new int [RLSC_TABLE_SIZE * RLSC_TABLE_NUMBER];

    if (len_flag == 1) {
        calib_flag = 0;
        fisheye_flag = 1;
    }
    center_x = 0;
    center_y = 0;
    radius = 0;
    norm = 0;

    // calibration
    int *raw_image = image.get();
    QString soc = GlobalData::getInstance()->getCvipqToolSoc();
    if (wdr_mode) width /= 2;

    if (soc == CV184X_SOC) {
        int cv184x_lsc_algo_type = cv184x_calibration_algo->currentIndex();

        if (cv184x_lsc_algo_type == 0) {
            // mars3
            cali_lsc_calc(&raw_image, width, height, order, NumKnotX, NumKnotY, 1, fisheye_flag,
                blc_r_offset, blc_gr_offset, blc_gb_offset, blc_b_offset,
                &center_x, &center_y, &radius, &norm,
                mlsc_r_gain.get(), mlsc_g_gain.get(), mlsc_b_gain.get(), lsc_radius_gain);

            int mlsc_r[MLSC_TABLE_SIZE], mlsc_g[MLSC_TABLE_SIZE], mlsc_b[MLSC_TABLE_SIZE];
            cali_lsc_calc(&raw_image, width, height, order, NumKnotX, NumKnotY, 2, fisheye_flag,
                  blc_r_offset, blc_gr_offset, blc_gb_offset, blc_b_offset,
                  &center_x, &center_y, &radius, &norm,
                  mlsc_r, mlsc_g, mlsc_b, lsc_radius_gain);
        } else {
            // same as mars, mesh do luma and chroma
            cali_lsc_calc(&raw_image, width, height, order, NumKnotX, NumKnotY, calib_flag, fisheye_flag,
                      blc_r_offset, blc_gr_offset, blc_gb_offset, blc_b_offset,
                      &center_x, &center_y, &radius, &norm,
                      mlsc_r_gain.get(), mlsc_g_gain.get(), mlsc_b_gain.get(), lsc_radius_gain);
        }
    } else {
        cali_lsc_calc(&raw_image, width, height, order, NumKnotX, NumKnotY, calib_flag, fisheye_flag,
                      blc_r_offset, blc_gr_offset, blc_gb_offset, blc_b_offset,
                      &center_x, &center_y, &radius, &norm,
                      mlsc_r_gain.get(), mlsc_g_gain.get(), mlsc_b_gain.get(), lsc_radius_gain);
    }

    if (!quickMode) {
        // verification
        int *raw_lsc_image = new int [width * height];
        int *raw_rlsc_image = new int [width * height];
        int *tmp = NULL;

        if (soc == CV184X_SOC) {
            int cv184x_lsc_algo_type = cv184x_calibration_algo->currentIndex();

            if (cv184x_lsc_algo_type == 0) {
                // mars3, verify call once!
                cali_lsc_verify_v2(&raw_image, width, height, order, NumKnotX, NumKnotY, calib_flag,
                    blc_r_offset, blc_gr_offset, blc_gb_offset, blc_b_offset,
                    blc_r_gain, blc_gr_gain, blc_gb_gain, blc_b_gain,
                    center_x, center_y, radius, norm,
                    mlsc_r_gain.get(), mlsc_g_gain.get(), mlsc_b_gain.get(), lsc_radius_gain,
                    &raw_lsc_image, &raw_rlsc_image);
                // mars3 old verify type
                // if (calib_flag == 0) {
                //     // luma + chroma
                //     cali_lsc_verify(&raw_image, width, height, order, NumKnotX, NumKnotY, 1,
                //         blc_r_offset, blc_gr_offset, blc_gb_offset, blc_b_offset,
                //         blc_r_gain, blc_gr_gain, blc_gb_gain, blc_b_gain,
                //         center_x, center_y, radius, norm,
                //         mlsc_r_gain.get(), mlsc_g_gain.get(), mlsc_b_gain.get(), NULL,
                //         &raw_lsc_image, &tmp);
                //     cali_lsc_verify(&raw_lsc_image, width, height, order, NumKnotX, NumKnotY, 2,
                //         0, 0, 0, 0,
                //         1, 1, 1, 1,
                //         center_x, center_y, radius, norm,
                //         NULL, NULL, NULL, lsc_radius_gain,
                //         &tmp, &raw_rlsc_image);
                //     tmp = raw_rlsc_image;
                //     raw_rlsc_image = raw_lsc_image;
                //     raw_lsc_image = tmp;
                // } else if (calib_flag == 1) {
                //     // chroma: mesh
                //     cali_lsc_verify(&raw_image, width, height, order, NumKnotX, NumKnotY, 1,
                //         blc_r_offset, blc_gr_offset, blc_gb_offset, blc_b_offset,
                //         blc_r_gain, blc_gr_gain, blc_gb_gain, blc_b_gain,
                //         center_x, center_y, radius, norm,
                //         mlsc_r_gain.get(), mlsc_g_gain.get(), mlsc_b_gain.get(), NULL,
                //         &raw_lsc_image, &tmp);
                // } else {
                //     // luma: radius
                //     cali_lsc_verify(&raw_image, width, height, order, NumKnotX, NumKnotY, 2,
                //         blc_r_offset, blc_gr_offset, blc_gb_offset, blc_b_offset,
                //         blc_r_gain, blc_gr_gain, blc_gb_gain, blc_b_gain,
                //         center_x, center_y, radius, norm,
                //         NULL, NULL, NULL, lsc_radius_gain,
                //         &tmp, &raw_rlsc_image);
                // }
            } else {
                // mars: mesh do luma and chroma
                cali_lsc_verify(&raw_image, width, height, order, NumKnotX, NumKnotY, calib_flag,
                    blc_r_offset, blc_gr_offset, blc_gb_offset, blc_b_offset,
                    blc_r_gain, blc_gr_gain, blc_gb_gain, blc_b_gain,
                    center_x, center_y, radius, norm,
                    mlsc_r_gain.get(), mlsc_g_gain.get(), mlsc_b_gain.get(), lsc_radius_gain,
                    &raw_lsc_image, &raw_rlsc_image);
            }
        } else {
            cali_lsc_verify(&raw_image, width, height, order, NumKnotX, NumKnotY, calib_flag,
                        blc_r_offset, blc_gr_offset, blc_gb_offset, blc_b_offset,
                        blc_r_gain, blc_gr_gain, blc_gb_gain, blc_b_gain,
                        center_x, center_y, radius, norm,
                        mlsc_r_gain.get(), mlsc_g_gain.get(), mlsc_b_gain.get(), lsc_radius_gain,
                        &raw_lsc_image, &raw_rlsc_image);
        }

        // deal output raw image
        int *output_img;

        if (soc == CV184X_SOC && cv184x_calibration_algo->currentIndex() == 0) {
            // cv184x lsc mars3 algo, not whether what clia_flag, just see the raw_iamge
            output_img = raw_image;
        } else {
            output_img = (calib_flag == 2) ? raw_rlsc_image : raw_lsc_image;
        }

        RAW_BAYER_ID bayer_id = static_cast<RAW_BAYER_ID>(order);
        using namespace utils;
        COLOR_RGB e_color[2][2];
        if (bayer_id == BAYER_ID_RG) {
            e_color[0][0] = COLOR_R;
            e_color[0][1] = COLOR_G;
            e_color[1][0] = COLOR_G;
            e_color[1][1] = COLOR_B;
        } else if (bayer_id == BAYER_ID_GR) {
            e_color[0][0] = COLOR_G;
            e_color[0][1] = COLOR_R;
            e_color[1][0] = COLOR_B;
            e_color[1][1] = COLOR_G;
        } else if (bayer_id == BAYER_ID_BG) {
            e_color[0][0] = COLOR_B;
            e_color[0][1] = COLOR_G;
            e_color[1][0] = COLOR_G;
            e_color[1][1] = COLOR_R;
        } else {
            e_color[0][0] = COLOR_G;
            e_color[0][1] = COLOR_B;
            e_color[1][0] = COLOR_R;
            e_color[1][1] = COLOR_G;
        }
        // apply awb
        int roi_width, roi_height;
        int roi_x, roi_y;
        float sum_r, sum_b, sum_g;
        int wb_rgain, wb_bgain;

        sum_r = sum_b = sum_g = 0.0f;

        if (width * 9 == height * 16) {
            roi_width = width / 8 >> 1 << 1;
            roi_height = height / 9 >> 1;
        } else {
            roi_width = width / 8 >> 1 << 1;
            roi_height = height / 6 >> 1 << 1;
        }
        roi_x = (width - roi_width) / 2;
        roi_y = (height - roi_height) / 2;

        for (int j = roi_y; j < roi_y + roi_height; j++) {
            for (int i = roi_x; i < roi_x + roi_width; i++) {
                COLOR_RGB color_id = e_color[j%2][i%2];
                switch (color_id) {
                case COLOR_R:
                    sum_r += output_img[j*width+i];
                    break;
                case COLOR_G:
                    sum_g += output_img[j*width+i];
                    break;
                case COLOR_B:
                    sum_b += output_img[j*width+i];
                    break;
                default:
                    break;
                }
            }
        }

        sum_r = sum_r / (roi_width * roi_height / 4);
        sum_g = sum_g / (roi_width * roi_height / 2);
        sum_b = sum_b / (roi_width * roi_height / 4);

        wb_rgain = (int)((sum_g / sum_r) * 1024 + 0.5f);
        wb_bgain = (int)((sum_g / sum_b) * 1024 + 0.5f);

        qDebug() << "lsc raw: rgain: " << wb_rgain;
        qDebug() << "lsc raw: bgain: " << wb_bgain;
        QString awb_gain_log = QString("lsc calibration, rgain: %1, bgain: %2").arg(wb_rgain).arg(wb_bgain);
        qDebug() << awb_gain_log;
        emit sigStatusLog(awb_gain_log, LOG_LEVEL::LOG_INFO);

        for (int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++) {
                COLOR_RGB color_id = e_color[j%2][i%2];
                switch (color_id) {
                case COLOR_R:
                    output_img[j*width+i] = output_img[j*width+i] * wb_rgain >> 10;
                    break;
                case COLOR_B:
                    output_img[j*width+i] = output_img[j*width+i] * wb_bgain >> 10;
                    break;
                default:
                    break;
                }
            }
        }

        // display result img
        RAW_BITS bits = static_cast<RAW_BITS>(raw_img_item->getBits().toInt());
        unsigned char *result_img_rbg = utils::rawIntData2RGBData(output_img, width, height, bayer_id, bits);

        QImage result_img(result_img_rbg, width, height, QImage::Format_RGB32);
        emit sigShowOutputRawImage(result_img);

        delete [] raw_lsc_image;
        delete [] raw_rlsc_image;
        delete [] result_img_rbg;
    }

    for (int i = 0; i < RLSC_TABLE_SIZE; i++) {
        rlsc_r_gain[i] = lsc_radius_gain[i];
        rlsc_g_gain[i] = lsc_radius_gain[RLSC_TABLE_SIZE + i];
        rlsc_b_gain[i] = lsc_radius_gain[RLSC_TABLE_SIZE * 2 + i];
        rlsc_ir_gain[i] = lsc_radius_gain[RLSC_TABLE_SIZE * 3 + i];
    }

    delete [] lsc_radius_gain;

    cali_status = CALIBRATION_STATUS::READY;
    qDebug("center_x = %d, center_y = %d, radius = %d, norm = %d", center_x, center_y, radius, norm);
    return true;
}

static bool compareData(CTItem *item1, CTItem *item2)
{
    if (item1->getCT() < item2->getCT()) {
        return true;
    }

    return false;
}

QJsonArray LSCCalibrationPageWidget::getCalibrationData()
{
    QJsonArray data_array;
    QJsonObject ct_data;

    QList<CTItem *> ct_list;
    if (quickMode) {
        ct_list = ctItem_list;
    } else {
        ct_list = ct_list_widget->getDisplayCTList();
    }
    std::sort(ct_list.begin(), ct_list.end(), compareData);

    int ct_array[LSC_MAX_SIZE] = {0};
    for (int i = 0; i < ct_list.size(); i++) {
        ct_array[i] = ct_list[i]->getCT();
    }

    ct_data[KEY_REG_KEY] = "CT";
    ct_data[KEY_VALUE] = utils::arrayPtrToJsonArray(ct_array, ct_list.size());
    ct_data[KEY_VALUE_TYPE] = TYPE_ARRAY;
    ct_data[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
    data_array.append(ct_data);

    QJsonObject data;
    for (int i = 0; i < ct_list.size(); i++) {
        ct_list[i]->getLSC(mlsc_r_gain.get(), mlsc_g_gain.get(), mlsc_b_gain.get(),
                           rlsc_r_gain.get(), rlsc_g_gain.get(), rlsc_b_gain.get(), rlsc_ir_gain.get(),
                           &center_x, &center_y, &norm);

        // MLSC
        data[KEY_REG_KEY] = QString("lsc_r_gain[%1]").arg(i);
        data[KEY_VALUE] = utils::arrayPtrToJsonArray(mlsc_r_gain.get(), MLSC_TABLE_SIZE);
        data[KEY_VALUE_TYPE] = TYPE_ARRAY;
        data[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
        data_array.append(data);

        data[KEY_REG_KEY] = QString("lsc_g_gain[%1]").arg(i);
        data[KEY_VALUE] = utils::arrayPtrToJsonArray(mlsc_g_gain.get(), MLSC_TABLE_SIZE);
        data[KEY_VALUE_TYPE] = TYPE_ARRAY;
        data[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
        data_array.append(data);

        data[KEY_REG_KEY] = QString("lsc_b_gain[%1]").arg(i);
        data[KEY_VALUE] = utils::arrayPtrToJsonArray(mlsc_b_gain.get(), MLSC_TABLE_SIZE);
        data[KEY_VALUE_TYPE] = TYPE_ARRAY;
        data[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
        data_array.append(data);

        // RLSC
        data[KEY_REG_KEY] = QString("center_x[%1]").arg(i);
        data[KEY_VALUE] = center_x;
        data[KEY_VALUE_TYPE] = TYPE_INT;
        data[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
        data_array.append(data);

        data[KEY_REG_KEY] = QString("center_y[%1]").arg(i);
        data[KEY_VALUE] = center_y;
        data[KEY_VALUE_TYPE] = TYPE_INT;
        data[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
        data_array.append(data);

        data[KEY_REG_KEY] = QString("norm[%1]").arg(i);
        data[KEY_VALUE] = norm;
        data[KEY_VALUE_TYPE] = TYPE_INT;
        data[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
        data_array.append(data);

        data[KEY_REG_KEY] = QString("rlsc_r_gain[%1]").arg(i);
        data[KEY_VALUE] = utils::arrayPtrToJsonArray(rlsc_r_gain.get(), RLSC_TABLE_SIZE);
        data[KEY_VALUE_TYPE] = TYPE_ARRAY;
        data[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
        data_array.append(data);

        data[KEY_REG_KEY] = QString("rlsc_g_gain[%1]").arg(i);
        data[KEY_VALUE] = utils::arrayPtrToJsonArray(rlsc_g_gain.get(), RLSC_TABLE_SIZE);
        data[KEY_VALUE_TYPE] = TYPE_ARRAY;
        data[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
        data_array.append(data);

        data[KEY_REG_KEY] = QString("rlsc_b_gain[%1]").arg(i);
        data[KEY_VALUE] = utils::arrayPtrToJsonArray(rlsc_b_gain.get(), RLSC_TABLE_SIZE);
        data[KEY_VALUE_TYPE] = TYPE_ARRAY;
        data[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
        data_array.append(data);

        data[KEY_REG_KEY] = QString("rlsc_ir_gain[%1]").arg(i);
        data[KEY_VALUE] = utils::arrayPtrToJsonArray(rlsc_ir_gain.get(), RLSC_TABLE_SIZE);
        data[KEY_VALUE_TYPE] = TYPE_ARRAY;
        data[KEY_REG_OP_TYPE] = OP_TYPE_MANUAL;
        data_array.append(data);
    }

    return data_array;
}

void LSCCalibrationPageWidget::exportData(QString file_path)
{
    QJsonObject data;
    QJsonArray items = getCalibrationData();
    CALIBRATION_MODULE_ID id = static_cast<CALIBRATION_MODULE_ID>(property("cali_id").toInt());
    data[PREFIX_CALI_STR + getCaliModuleName(id)] = items;

    bool is_export_rlsc = false;
    QString soc = GlobalData::getInstance()->getCvipqToolSoc();

    is_export_rlsc = (CV182X_SOC == soc) || (CV184X_SOC == soc && cv184x_calibration_algo->currentIndex() == 0);
    utils::saveLscJson(file_path, data, is_export_rlsc);
}

bool LSCCalibrationPageWidget::checkShallExport()
{
    int list_size = 0;
    if (quickMode) {
        list_size = ctItem_list.size();
    } else {
        list_size = ct_list_widget->getDisplayCTList().size();
    }

    if (list_size >= 1 && list_size <= 7)
        return true;

    return false;
}

void LSCCalibrationPageWidget::initCTListUI()
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
        QString path = this->raw_img_item_vec.first()->getFilePath();
        QStringList strList = path.split("/");
        QString dir_name = strList[strList.size()-2];
        int startIdx = dir_name.indexOf("(") + 1;
        int endIdx = dir_name.indexOf("K");
        int ct = dir_name.mid(startIdx, endIdx-startIdx).toInt();
        ct_item->setCT(ct);

        bool success = ct_list_widget->addTableItem(ct_item);
        if (!success) {
            QMessageBox message(QMessageBox::NoIcon, "Error", "Add ct item failed");
            message.exec();
            return;
        }

        ct_item->setLSC(mlsc_r_gain.get(), mlsc_g_gain.get(), mlsc_b_gain.get(),
                        rlsc_r_gain.get(), rlsc_g_gain.get(), rlsc_b_gain.get(), rlsc_ir_gain.get(),
                        center_x, center_y, norm);
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

QJsonObject LSCCalibrationPageWidget::GetModuleObjectToNetworkJsonrpc()
{
    qDebug("LSCCalibrationPageWidget::GetModuleObjectToNetworkJsonrpc.");
    QJsonObject module_obj;
    QStringList struct_list = getStructList();
    QMap<QString, QString> struct_set_api_map = getStructSetApiMap();
    CALIBRATION_MODULE_ID id = static_cast<CALIBRATION_MODULE_ID>(property("cali_id").toInt());
    QString module_name = getCaliModuleName(id);

    NetworkClientHelper::getInstance()->clearIdInfoJsonrpc();
    int jsonrpc_id = GlobalData::getInstance()->getJsonrpcId();
    QString struct_id = struct_list[0];

    QList<CTItem *> ct_list;
    if (quickMode) {
        ct_list = ctItem_list;
    } else {
        ct_list = ct_list_widget->getDisplayCTList();
    }
    std::sort(ct_list.begin(), ct_list.end(), compareData);

    QJsonObject send_obj;
    QJsonObject params_obj;
    QJsonArray LscGainLut_array;
    QString soc = GlobalData::getInstance()->getCvipqToolSoc();

    for (int i = 0; i < ct_list.size(); i++) {
        QJsonObject LscGainLut_obj;
        ct_list[i]->getLSC(mlsc_r_gain.get(), mlsc_g_gain.get(), mlsc_b_gain.get(),
                           rlsc_r_gain.get(), rlsc_g_gain.get(), rlsc_b_gain.get(), rlsc_ir_gain.get(),
                           &center_x, &center_y, &norm);

        // if calibration_algo_type == mars_debug, combine chroma and luma gain to reg
        if (soc == CV184X_SOC && cv184x_calibration_algo->currentIndex() == 2) {
            int luma_strength = 4095; // todo, may set by lineedit
            int idx;
            int percent_0_ggain = (4095 - luma_strength) * 512;
            int *p_ggain = mlsc_g_gain.get();
            int *p_rgain = mlsc_r_gain.get();
            int *p_bgain = mlsc_b_gain.get();
            int NumKnotX = grid_number_x_item->getData().toInt();
            int NumKnotY = grid_number_y_item->getData().toInt();

            for (int i = 0; i < NumKnotY; ++i) {
                for (int j = 0; j < NumKnotX; ++j) {
                    idx = i * NumKnotX + j;
                    p_ggain[idx] = ((p_ggain[idx] * luma_strength + percent_0_ggain + (1 << 11)) >> 12);
                    p_rgain[idx] = ((p_rgain[idx] * p_ggain[idx] + (1 << 8)) >> 9);
                    p_bgain[idx] = ((p_bgain[idx] * p_ggain[idx] + (1 << 8)) >> 9);
                }
            }
        }

        LscGainLut_obj["ColorTemperature"] = ct_list[i]->getCT();
        LscGainLut_obj["RGain"] = utils::arrayPtrToJsonArray(mlsc_r_gain.get(), MLSC_TABLE_SIZE);
        LscGainLut_obj["GGain"] = utils::arrayPtrToJsonArray(mlsc_g_gain.get(), MLSC_TABLE_SIZE);
        LscGainLut_obj["BGain"] = utils::arrayPtrToJsonArray(mlsc_b_gain.get(), MLSC_TABLE_SIZE);
        LscGainLut_array.append(LscGainLut_obj);
    }

    params_obj["Size"] = ct_list.size();
    params_obj["LscGainLut"] = LscGainLut_array;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_METHOD] = struct_set_api_map[struct_id];
    send_obj[MSG_KEY_ID] = jsonrpc_id;
    send_obj[MSG_KEY_PARAMS] = params_obj;
    module_obj[struct_id] = send_obj;

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_WRITE_LSC_CALIBRATION;
    id_info[MSG_KEY_MODULE] = module_name;
    id_info[MSG_KEY_STRUCT] = struct_id;
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(jsonrpc_id, id_info);

    if (soc == CV184X_SOC) {
        // cv184x, still write the rlsc param avoiding the lacking of the send_obj_1 and send_obj2
        int centerX = 0, centerY = 0, normTmp = 0;
        int rlsc_ggain[RLSC_TABLE_SIZE] = {0};
        QJsonArray gainArr;
        for (int i = 0; i < ct_list.size(); i++) {
            ct_list[i]->getLSC(mlsc_r_gain.get(), mlsc_g_gain.get(), mlsc_b_gain.get(),
                               rlsc_r_gain.get(), rlsc_g_gain.get(), rlsc_b_gain.get(), rlsc_ir_gain.get(),
                               &center_x, &center_y, &norm);
            centerX += center_x;
            centerY += center_y;
            normTmp += norm;
            for (int j = 0; j < RLSC_TABLE_SIZE; j++) {
                rlsc_ggain[j] += rlsc_g_gain[j];
            }
        }
        centerX = centerX / ct_list.size();
        centerY = centerY / ct_list.size();
        normTmp = normTmp / ct_list.size();
        for (int i = 0; i < RLSC_TABLE_SIZE; i++) {
            rlsc_ggain[i] = rlsc_ggain[i] / ct_list.size();
            gainArr.append(rlsc_ggain[i]);
        }
        // write rlsc attr
        jsonrpc_id = GlobalData::getInstance()->getJsonrpcId();
        QString struct_id_1 = struct_list[1];
        QJsonObject send_obj_1;
        // update radius_param_obj
        radius_param_obj["CenterX"] = centerX;
        radius_param_obj["CenterY"] = centerY;
        radius_param_obj["RadiusScaleRGB"] = normTmp;

        send_obj_1[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
        send_obj_1[MSG_KEY_METHOD] = struct_set_api_map[struct_id_1];
        send_obj_1[MSG_KEY_ID] = jsonrpc_id;
        send_obj_1[MSG_KEY_PARAMS] = radius_param_obj;
        module_obj[struct_id_1] = send_obj_1;

        QJsonObject id_info_1;
        id_info_1[MSG_KEY_CMD] = CMD_WRITE_LSC_CALIBRATION;
        id_info_1[MSG_KEY_MODULE] = module_name;
        id_info_1[MSG_KEY_STRUCT] = struct_id_1;
        NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(jsonrpc_id, id_info_1);

        // write rlsc g gain
        jsonrpc_id = GlobalData::getInstance()->getJsonrpcId();
        QString struct_id_2 = struct_list[2];
        QJsonObject send_obj_2;
        QJsonObject params_obj_2;
        params_obj_2["GGain"] = gainArr;
        send_obj_2[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
        send_obj_2[MSG_KEY_METHOD] = struct_set_api_map[struct_id_2];
        send_obj_2[MSG_KEY_ID] = jsonrpc_id;
        send_obj_2[MSG_KEY_PARAMS] = params_obj_2;
        module_obj[struct_id_2] = send_obj_2;

        QJsonObject id_info_2;
        id_info_2[MSG_KEY_CMD] = CMD_WRITE_LSC_CALIBRATION;
        id_info_2[MSG_KEY_MODULE] = module_name;
        id_info_2[MSG_KEY_STRUCT] = struct_id_2;
        NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(jsonrpc_id, id_info_2);

        // write mlsc attr
        jsonrpc_id = GlobalData::getInstance()->getJsonrpcId();
        QString struct_id_3 = struct_list[3];
        QJsonObject send_obj_3;

        send_obj_3[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
        send_obj_3[MSG_KEY_METHOD] = struct_set_api_map[struct_id_3];
        send_obj_3[MSG_KEY_ID] = jsonrpc_id;
        send_obj_3[MSG_KEY_PARAMS] = mesh_param_obj;
        module_obj[struct_id_3] = send_obj_3;

        QJsonObject id_info_3;
        id_info_3[MSG_KEY_CMD] = CMD_WRITE_LSC_CALIBRATION;
        id_info_3[MSG_KEY_MODULE] = module_name;
        id_info_3[MSG_KEY_STRUCT] = struct_id_3;
        NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(jsonrpc_id, id_info_3);
    }

    return module_obj;
}

int LSCCalibrationPageWidget::QuickCalibrationLoadData(QString path, bool externRequest)
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
            formt_dialog->setScene(SCENE::UNIFORM_COLOR);
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

    int ret = loadBlcData(path);
    if (ret < 0 && externRequest) {
        return -1;
    }

    return 0;
}

int LSCCalibrationPageWidget::loadBlcData(QString path)
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

void LSCCalibrationPageWidget::HandleCalibrationRequst(QJsonObject params, RawConfigItem config)
{
    qDebug("LSCCalibrationPageWidget::HandleCalibrationRequst");
    QString path;
    int ret = 0;
    if (params.contains("path") && !(params["path"].toString().isEmpty())) {
        path = params["path"].toString();
    }

    if (path.isEmpty()) {
        emit sigCaliStatus(CALIBRATION_MODULE_ID::LSC, CALIBRATION_STATUS::CAL_ERROR, "Image path is empty!");
        return;
    }

    QDir dir(path);
    if (!dir.exists()) {
        emit sigCaliStatus(CALIBRATION_MODULE_ID::LSC, CALIBRATION_STATUS::CAL_ERROR, "Image path not exists!");
        return;
    }

    if (cali_status == CALIBRATION_STATUS::CALIBRATION) {
        emit sigCaliStatus(CALIBRATION_MODULE_ID::LSC, CALIBRATION_STATUS::CAL_ERROR, "Calibration is busy!");
        return;
    }

    if (params.contains("type")) {
        int type = params["type"].toInt();
        calib_type->setCurrentIndex(type);
    }

    if (params.contains("lens")) {
        int lens = params["lens"].toInt();
        len_type->setCurrentIndex(lens);
    }

    this->config = config;

    ret = QuickCalibrationLoadData(path, true);
    if (ret < 0) {
        emit sigCaliStatus(CALIBRATION_MODULE_ID::LSC, CALIBRATION_STATUS::CAL_ERROR, "Load file fail!");
        return;
    }

    ret = handleQuickCalibration(false);
    if (ret == -1) {
        emit sigCaliStatus(CALIBRATION_MODULE_ID::LSC, CALIBRATION_STATUS::CAL_ERROR, "Calibration fail!");
        return;
    } else if (ret == -2) {
        emit sigCaliStatus(CALIBRATION_MODULE_ID::LSC, CALIBRATION_STATUS::WARNING, "Write data fail!");
        return;
    }

    emit sigCaliStatus(CALIBRATION_MODULE_ID::LSC, CALIBRATION_STATUS::READY, "Calibration finish!");
}

void LSCCalibrationPageWidget::handleLscSetting()
{
    if (cv184x_calibration_algo->isHidden()) {
        cv184x_calibration_algo->show();
    } else {
        cv184x_calibration_algo->hide();
    }
}

int LSCCalibrationPageWidget::readCalibrationData(bool showMessage)
{
    qDebug("LSCCalibrationPageWidget::readCalibrationData");
    // just for cv184x, get the radius shading attr
    QString soc = GlobalData::getInstance()->getCvipqToolSoc();
    if (soc != CV184X_SOC) {
        return 0;
    }
    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        if (showMessage) {
            QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        }
        return -1;
    }

    CALIBRATION_MODULE_ID module_id = static_cast<CALIBRATION_MODULE_ID>(property("cali_id").toInt());
    NetworkClientHelper::getInstance()->clearIdInfoJsonrpc();

    // read rlsc params
    QJsonObject send_obj;
    QJsonObject id_info;
    QString struct_id = "ISP_RADIAL_SHADING_ATTR_S";
    int jsonrpc_id = GlobalData::getInstance()->getJsonrpcId();
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_METHOD] = "CVI_ISP_GetRadialShadingAttr";
    send_obj[MSG_KEY_ID] = jsonrpc_id;
    id_info[MSG_KEY_CMD] = CMD_READ_RLSC_CALIBRATION;
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

    // read mlsc params
    send_obj = {};
    id_info = {};
    struct_id = "ISP_MESH_SHADING_ATTR_S";
    jsonrpc_id = GlobalData::getInstance()->getJsonrpcId();
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_METHOD] = "CVI_ISP_GetMeshShadingAttr";
    send_obj[MSG_KEY_ID] = jsonrpc_id;
    id_info[MSG_KEY_CMD] = CMD_READ_MLSC_CALIBRATION;
    id_info[MSG_KEY_MODULE] = getCaliModuleName(module_id);
    id_info[MSG_KEY_STRUCT] = struct_id;

    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(jsonrpc_id, id_info);
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));

    gotResponse = false;
    stopTime = QTime::currentTime().addSecs(READ_WRITE_TIMEOUT);
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

    return 0;
}

void LSCCalibrationPageWidget::handleReceiveRlscCalibration(int id)
{
    qDebug("LSCCalibrationPageWidget::handleReceiveRlscCalibration.");
    // just for cv184x, get the radius shading attr
    QString soc = GlobalData::getInstance()->getCvipqToolSoc();
    if (soc != CV184X_SOC) {
        return;
    }
    QJsonObject data_obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    if (!data_obj.isEmpty()) {
        QJsonObject result_obj = data_obj[MSG_KEY_RESULT].toObject();
        if (!result_obj.isEmpty()) {
            QJsonObject info_obj = data_obj[MSG_KEY_ID_INFO].toObject();
            QString struct_id = info_obj[MSG_KEY_STRUCT].toString();
            QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();
            radius_param_obj = params_obj;
            // set rlsc enable and set the str to 4095
            QJsonArray auto_radius_str = {4095, 4095,4095,4095,4095,4095,4095,4095,4095,4095,4095,4095,4095,4095,4095,4095};
            QJsonObject man_obj = radius_param_obj["stManual"].toObject();
            QJsonObject auto_obj = radius_param_obj["stAuto"].toObject();

            if (cv184x_calibration_algo->currentIndex() > 0) {
                // mars3 use mars mesh algo
                radius_param_obj["Enable"] = 0;
            } else {
                radius_param_obj["Enable"] = 1;
                man_obj["RadiusStr"] = 4095;
                auto_obj["RadiusStr"] = auto_radius_str;
                radius_param_obj["stManual"] = man_obj;
                radius_param_obj["stAuto"] = auto_obj;
            }
        } else {
            qDebug("LSCCalibrationPageWidget::handleReceiveRlscCalibration. Error: result_obj is empty.");
        }
    } else {
        qDebug("LSCCalibrationPageWidget::handleReceiveRlscCalibration. Error: data_obj is empty.");
    }
}

void LSCCalibrationPageWidget::handleReceiveMlscCalibration(int id)
{
    qDebug("LSCCalibrationPageWidget::handleReceiveMlscCalibration.");
    // just for cv184x, get the mesh shading attr
    QString soc = GlobalData::getInstance()->getCvipqToolSoc();
    if (soc != CV184X_SOC) {
        return;
    }
    QJsonObject data_obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    if (!data_obj.isEmpty()) {
        QJsonObject result_obj = data_obj[MSG_KEY_RESULT].toObject();
        if (!result_obj.isEmpty()) {
            QJsonObject info_obj = data_obj[MSG_KEY_ID_INFO].toObject();
            QString struct_id = info_obj[MSG_KEY_STRUCT].toString();
            QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();
            mesh_param_obj = params_obj;

            mesh_param_obj["Enable"] = 1;
            // index 1: mars3 mesh same as mars
            if (mesh_param_obj.contains("MeshCorrectMode")) {
                if (cv184x_calibration_algo->currentIndex() == 1) {
                mesh_param_obj["MeshCorrectMode"] = 1;
                } else {
                // index 0: mars3, index 2: mars debug mode
                mesh_param_obj["MeshCorrectMode"] = 0;
                }
            }
        } else {
            qDebug("LSCCalibrationPageWidget::handleReceiveMlscCalibration. Error: result_obj is empty.");
        }
    } else {
        qDebug("LSCCalibrationPageWidget::handleReceiveMlscCalibration. Error: data_obj is empty.");
    }
}
