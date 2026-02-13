#include "quick_calibration_page_widget.hpp"
#include "utils.hpp"
#include "otp_pdaf_cali.h"
#include "network_client_helper.hpp"
#include "raw_image_format_dialog.hpp"
#include <QGroupBox>
#include <QMessageBox>
#include <QHeaderView>
#include <QScrollBar>

QuickCalibrationPageWidget::QuickCalibrationPageWidget(QWidget *parent) : BaseCalibrationPageWidget(static_cast<int>(CALIBRATION_MODULE_ID::QUICK_CALIRATION), parent)
{
    timer = new QTimer;
    timer->setInterval(1000);
    timer->setSingleShot(true);
    initUI();
    initEvent();
}

QuickCalibrationPageWidget::~QuickCalibrationPageWidget()
{
}

void QuickCalibrationPageWidget::initUI()
{
    QVBoxLayout *vlay = new QVBoxLayout;

    QGroupBox *setting = CreateSettingGroupbox();
    QGroupBox *cali = CreateCalibrationGroupbox();
    QGroupBox *showlog = CreateLogGroupbox();

    vlay->addWidget(setting, 4);
    vlay->addWidget(cali, 2);
    vlay->addWidget(showlog, 4);

    main_layout->addLayout(vlay);
}

void QuickCalibrationPageWidget::initEvent()
{
    connect(timer, &QTimer::timeout, this, &QuickCalibrationPageWidget::handleTimeOut);
}

QGroupBox *QuickCalibrationPageWidget::CreateSettingGroupbox()
{
    QGroupBox * groupbox = new QGroupBox("Setting");
    QGridLayout *layout = new QGridLayout;

    QGroupBox * blc_groupbox = new QGroupBox("blc");
    QHBoxLayout *blc_layout = new QHBoxLayout;
    blc_groupbox->setLayout(blc_layout);
    QLabel *blc_method_label = new QLabel("Method:");
    blc_method_combox = new QComboBox;
    blc_method_combox->addItems({"Median", "Mean"});
    blc_layout->addWidget(blc_method_label, 1, Qt::AlignLeft);
    blc_layout->addWidget(blc_method_combox, 1, Qt::AlignLeft);

    QGroupBox * lsc_groupbox = new QGroupBox("lsc");
    QGridLayout *lsc_layout = new QGridLayout;
    lsc_groupbox->setLayout(lsc_layout);
    QLabel *lsc_type_label = new QLabel("Calibration Type:");
    lsc_type_combox = new QComboBox;
    lsc_type_combox->addItems({"All", "Chroma", "Luma"});
    lsc_type_combox->setCurrentIndex(0);
    QLabel *lsc_lens_label = new QLabel("Lens Type:");
    lsc_lens_combox = new QComboBox;
    lsc_lens_combox->addItems({"No Fisheye", "Fisheye"});
    lsc_lens_combox->setCurrentIndex(0);
    lsc_layout->addWidget(lsc_type_label, 0, 0, Qt::AlignLeft);
    lsc_layout->addWidget(lsc_type_combox, 0, 1, Qt::AlignLeft);
    lsc_layout->addWidget(lsc_lens_label, 1, 0, Qt::AlignLeft);
    lsc_layout->addWidget(lsc_lens_combox, 1, 1, Qt::AlignLeft);

    QGroupBox * ccm_groupbox = new QGroupBox("ccm");
    QVBoxLayout *ccm_layout = new QVBoxLayout;
    ccm_groupbox->setLayout(ccm_layout);
    strength_slider = new DoubleSliderCaliItemWidget("Sat Strength:");
    strength_slider->setRange(0, 2);
    strength_slider->setSpinnerSingleStep(0.01);
    strength_slider->setData(1.0);
    isp_gamma_item = new ComboboxCaliItemWidget("ISP Gamma:");
    QStringList gamma_list = {"sRGB", LINEAR_ITEM, CUSTOMIZE_ITEM};
    for (int i = 0; i < gamma_list.size(); i++) {
        isp_gamma_item->addItem(gamma_list[i]);
    }
    isp_gamma_item->setCurrentIndex(0);
    connect(isp_gamma_item, &ComboboxCaliItemWidget::sigCustomizeSelected,
            this, &QuickCalibrationPageWidget::handleCustomizeIspGamma);
    lab_ref_item = new ComboboxCaliItemWidget("Lab Reference:");
    lab_ref_item->addItem("Xrite D50");
    lab_ref_item->addItem("Xrite D65");
    QLabel *weight_title = new QLabel("Patch Weight:");
    weight_table = new QTableWidget();
    weight_table->setStyleSheet("border: none;");
    weight_table->setRowCount(PATCH_WEIGHT_ROW);
    weight_table->setColumnCount(PATCH_WEIGHT_COL);
    weight_table->horizontalHeader()->hide();
    weight_table->verticalHeader()->hide();
    for (int i = 0; i < weight_table->columnCount(); i++) {
        weight_table->setColumnWidth(i, 40);
        for (int j = 0; j < weight_table->rowCount(); j++) {
            weight_table->setRowHeight(j, 30);
            weight_table->setItem(j, i, new QTableWidgetItem("1"));
        }
    }
    ccm_layout->addWidget(strength_slider, 1, Qt::AlignTop);
    ccm_layout->addWidget(isp_gamma_item, 1, Qt::AlignTop);
    ccm_layout->addWidget(lab_ref_item, 1, Qt::AlignTop);
    ccm_layout->addWidget(weight_title, 1, Qt::AlignTop);
    ccm_layout->addWidget(weight_table, 0, Qt::AlignTop);

    QGroupBox * noise_groupbox = new QGroupBox("Noise Profile");
    QHBoxLayout *noise_layout = new QHBoxLayout;
    noise_groupbox->setLayout(noise_layout);
    QLabel *TEAISP_bnr_label = new QLabel("Enable TEAISP.bnr:");
    TEAISP_bnr_checkbox = new QCheckBox();
    TEAISP_bnr_checkbox->setChecked(false);
    noise_layout->addWidget(TEAISP_bnr_label, 1, Qt::AlignLeft);
    noise_layout->addWidget(TEAISP_bnr_checkbox, 1, Qt::AlignLeft);
    QString tool_soc = GlobalData::getInstance()->getCvipqToolSoc();
    if (tool_soc != CV186X_SOC) {
        TEAISP_bnr_checkbox->setEnabled(false);
    }

    layout->addWidget(blc_groupbox, 0, 0, Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(lsc_groupbox, 0, 1, Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(ccm_groupbox, 0, 2, -1, 2, Qt::AlignTop | Qt::AlignLeft);
    layout->addWidget(noise_groupbox, 1, 0, Qt::AlignTop | Qt::AlignLeft);
    groupbox->setLayout(layout);

    return groupbox;
}

QGroupBox *QuickCalibrationPageWidget::CreateCalibrationGroupbox()
{
    QGroupBox *group = new QGroupBox(tr("Calibration"));
    QVBoxLayout *layout = new QVBoxLayout;

    rawConfig.setConfig("1920", "1080", "12", utils::BAYER_ID_BG, utils::RAW_PACK_TYPE::RAW_UNPACKED, utils::IR_NO);

    QHBoxLayout *path_layout = new QHBoxLayout;
    open_btn = new QPushButton("Open path");
    open_btn->setMaximumWidth(200);
    connect(open_btn, &QPushButton::clicked, this, &QuickCalibrationPageWidget::OpenFilePath);
    path_edit = new QLineEdit;
    path_edit->setMaximumWidth(800);
    path_edit->setPlaceholderText("calibration images dir path");
    path_edit->setReadOnly(true);
    path_layout->addWidget(open_btn);
    path_layout->addWidget(path_edit);
    path_layout->setAlignment(Qt::AlignLeft);

    cali_btn = new QPushButton("Calibration");
    cali_btn->setMaximumWidth(200);
    connect(cali_btn, &QPushButton::clicked, this, &QuickCalibrationPageWidget::handleCalibration);

    layout->addLayout(path_layout);
    layout->addWidget(cali_btn);
    group->setLayout(layout);

    return group;
}

QGroupBox *QuickCalibrationPageWidget::CreateLogGroupbox()
{
    QGroupBox *group = new QGroupBox(tr("Calibration"));
    QVBoxLayout *layout = new QVBoxLayout;

    log_edit = new QPlainTextEdit;
    log_edit->setReadOnly(true);
    QHBoxLayout *log_opt_layout = new QHBoxLayout;
    QPushButton *clear_btn = new QPushButton("Clear");
    clear_btn->setMaximumWidth(200);
    connect(clear_btn, &QPushButton::clicked, [=](){
        log_edit->clear();
    });
    QLabel *log_level_lab = new QLabel("Log Level:");
    log_level_lab->setMaximumWidth(200);
    log_level_box = new QComboBox;
    log_level_box->setMaximumWidth(200);
    QStringList level_str_list = {"LOG_ERROR", "LOG_WARNING", "LOG_INFO", "LOG_DEBUG"};
    log_level_box->addItems(level_str_list);
    log_level_box->setCurrentIndex(2);
    log_opt_layout->addWidget(clear_btn);
    log_opt_layout->addStretch(1);
    log_opt_layout->addWidget(log_level_lab);
    log_opt_layout->addWidget(log_level_box);
    log_opt_layout->addStretch(3);
    log_opt_layout->setAlignment(Qt::AlignLeft);

    layout->addLayout(log_opt_layout);
    layout->addWidget(log_edit);
    group->setLayout(layout);

    return group;
}

void QuickCalibrationPageWidget::EditRawConfig(QString path)
{
    RawImageFormatDialog *formt_dialog = new RawImageFormatDialog;
    formt_dialog->setSceneLabelState(false);
    formt_dialog->setData(rawConfig.getWidth(), rawConfig.getHeight(),
       rawConfig.getBits(), rawConfig.getBayer(), rawConfig.getPacked(),
       rawConfig.getIrPosition(), rawConfig.getScene(), rawConfig.getFrameMode());
    formt_dialog->setFilepath(path);
    if (QDialog::Accepted == formt_dialog->exec()) {
        std::tuple<QString, QString> size = formt_dialog->getSize();
        rawConfig.setConfig(std::get<0>(size), std::get<1>(size), formt_dialog->getBits(),
                            formt_dialog->getBayer(), formt_dialog->getPacked(),
                            formt_dialog->getIrPosition(), formt_dialog->getScene(),
                            formt_dialog->getFrameMode());
    }
    delete formt_dialog;
}

void QuickCalibrationPageWidget::handleCustomizeIspGamma()
{
    QString file_path = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text files (*.txt)"));
    QFile input_file(file_path);

    if(!input_file.open(QIODevice::ReadOnly)) {
        qDebug("Import file :%s error", qUtf8Printable(file_path));
        return;
    }

    QTextStream in(&input_file);
    std::vector<int> temp;
    int n = 0, step = 0;
    while (!in.atEnd())
    {
        QString data = in.readLine();
        QList<QVariant> data_list = utils::stringToList<QVariant>(data);
        for (int i = 0; i < data_list.size(); i++, n++) {
            temp.push_back(data_list[i].toInt());
            // qDebug("data[%d] = %d", n, temp[n]);
        }
    }

    input_file.close();
    if (n == 256 || n == 1025) {
        std::vector<int> x_in;
        std::vector<int> y_in;

        if (n == 256) {
            x_in.resize(n + 1);
            y_in.resize(n + 1);
            x_in.at(256) = 4095;
            y_in.at(256) = 4094;
            step = 16;
        } else if (n == 1025) {
            x_in.resize(n);
            y_in.resize(n);
            x_in.at(1024) = 4095;
            y_in.at(1024) = 4094;
            n = 1024;
            step = 4;
        }

        for (int i = 0; i < n; i++) {
            x_in.at(i) = i * step;
            y_in.at(i) = temp[i];
            // qDebug("x_in[%d] = %d; y_in[%d] = %d", i, x_in.at(i), i, y_in.at(i));
        }

        std::vector<int> y_out = utils::interp1_pchip(x_in, y_in);
        if (int(y_out.size()) != ISP_GAMMA_SIZE) {
            QMessageBox::critical(this, "Error", "Input data should be strectly increasing.", QMessageBox::Ok);
            return;
        } else {
            for (int i = 0; i < ISP_GAMMA_SIZE; i++) {
                isp_gamma[i] = y_out.at(i);
                // qDebug("isp_gamma[%d] = %d", i, isp_gamma[i]);
            }
        }

        QMessageBox::information(this, "Information", "Load isp gamma file successfully.", QMessageBox::Ok);
    } else if (n == 4096) {
        for (int i = 0; i < ISP_GAMMA_SIZE; i++) {
            isp_gamma[i] = temp.at(i);
            // qDebug("isp_gamma[%d] = %d", i, isp_gamma[i]);
        }
        QMessageBox::information(this, "Information", "Load isp gamma file successfully.", QMessageBox::Ok);
    } else {
        QMessageBox::critical(this, "Error", "Failed to load isp gamma file.", QMessageBox::Ok);
    }
}

void QuickCalibrationPageWidget::showLog(const QString &log, LOG_LEVEL log_level)
{
    if (log_level > (LOG_LEVEL)log_level_box->currentIndex()) {
        return;
    }

    QTextCharFormat tf = log_edit->currentCharFormat();
    QString log_prefix = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    switch (log_level) {
        case LOG_LEVEL::LOG_INFO:
            log_prefix += " [INFO] ";
            tf.setForeground(QBrush( QColor(0, 0, 0)));
            break;
        case LOG_LEVEL::LOG_WARNING:
            log_prefix += " [WARNING] ";
            tf.setForeground(QBrush(QColor(200, 0, 0)));
            break;
        case LOG_LEVEL::LOG_ERROR:
            log_prefix += " [ERROR] ";
            tf.setForeground(QBrush(QColor(255, 0, 0)));
            break;
        case LOG_LEVEL::LOG_DEBUG:
            log_prefix += " [DEBUG] ";
            tf.setForeground(QBrush(QColor(0, 0, 0)));
            break;
        default:
            log_prefix += " ";
            break;
    };
    log_edit->setCurrentCharFormat(tf);
    log_edit->appendPlainText(log_prefix + log);
    QScrollBar *scrollbar = log_edit->verticalScrollBar();
    if (scrollbar) {
        scrollbar->setSliderPosition(scrollbar->maximum());
    }
}

void QuickCalibrationPageWidget::OpenFilePath()
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
        showLog("Dir error!", LOG_LEVEL::LOG_ERROR);
        return;
    }

    path_edit->setText(dir_path);
    QString log = "Open dir:" + dir_path;
    showLog(log, LOG_LEVEL::LOG_DEBUG);

    QStringList files;
    utils::findFiles(dir_path, ".raw", files, false);
    if (files.isEmpty()) {
        showLog("Can't find raw file!", LOG_LEVEL::LOG_ERROR);
        path_edit->clear();
    } else {
        showLog(files[0], LOG_LEVEL::LOG_DEBUG);
        QString fileName = files[0].split("/").last();
        EditRawConfig(fileName);
    }
}

void QuickCalibrationPageWidget::handleCalibration()
{
    if (path_edit->text().isEmpty()) {
        showLog("Image file path is empty!", LOG_LEVEL::LOG_ERROR);
        return;
    }

    QString tool_soc = GlobalData::getInstance()->getCvipqToolSoc();
    if (tool_soc == CV186X_SOC) {
        maxID = CALIBRATION_MODULE_ID::LBLC;
    }

    showLog("Start Calibration:", LOG_LEVEL::LOG_INFO);
    curID = CALIBRATION_MODULE_ID::BLC;
    showLog("Start BLC Calibration...", LOG_LEVEL::LOG_INFO);

    timer->start();
}

void QuickCalibrationPageWidget::handleTimeOut()
{
    qDebug("handleTimeOut,curID:%d",curID);
    QJsonObject param;
    switch (curID) {
    case CALIBRATION_MODULE_ID::BLC: {
        param["path"] = path_edit->text() + "/BLC";
        param["method"] = blc_method_combox->currentIndex();
        emit sigStartBlcCalibration(param, rawConfig);
        break;
    }
    case CALIBRATION_MODULE_ID::LSC: {
        param["path"] = path_edit->text() + "/LSC";
        param["type"] = lsc_type_combox->currentIndex();
        param["lens"] = lsc_lens_combox->currentIndex();
        emit sigStartLscCalibration(param, rawConfig);
        break;
    }
    case CALIBRATION_MODULE_ID::AWB: {
        param["path"] = path_edit->text() + "/AWB";
        emit sigStartAwbCalibration(param, rawConfig);
        break;
    }
    case CALIBRATION_MODULE_ID::CCM: {
        param["path"] = path_edit->text() + "/CCM";
        param["strength"] = strength_slider->getData().toFloat();
        param["gamma type"] = isp_gamma_item->currentIndex();
        if (isp_gamma_item->currentIndex() == 2) {
            param["gamma"] = utils::arrayPtrToJsonArray(isp_gamma.get(), ISP_GAMMA_SIZE);
        }
        param["lab type"] = lab_ref_item->currentIndex();
        QJsonArray arr;
        for (int i = 0; i < PATCH_WEIGHT_ROW; i++) {
            for (int j = 0; j < PATCH_WEIGHT_COL; j++) {
                int value = weight_table->item(i, j)->text().toInt();
                arr.append(value);
            }
        }
        param["weigth"] = arr;
        emit sigStartCcmCalibration(param, rawConfig);
        break;
    }
    case CALIBRATION_MODULE_ID::NOISE_PROFILE: {
        param["path"] = path_edit->text() + "/NOISE";
        param["enable aibnr"] = TEAISP_bnr_checkbox->isChecked();
        emit sigStartNoiseCalibration(param, rawConfig);
        break;
    }
    case CALIBRATION_MODULE_ID::LBLC: {
        param["path"] = path_edit->text() + "/LBLC";
        emit sigStartLblcCalibration(param, rawConfig);
        break;
    }
    default:
        showLog("Not Support, Calibration End!!!", LOG_LEVEL::LOG_WARNING);
        break;
    }
}

void QuickCalibrationPageWidget::handleCalibrationStatus(CALIBRATION_MODULE_ID id, CALIBRATION_STATUS status, QString log)
{
    QString module = getCaliModuleName(id);
    QString info = module + ":" + log;
    qDebug("handleCalibrationStatus:%s", qUtf8Printable(info));
    if (status == CALIBRATION_STATUS::CAL_ERROR) {
        showLog(info, LOG_LEVEL::LOG_ERROR);
        if (id == CALIBRATION_MODULE_ID::BLC) {
            showLog("Stop Calibration!!!", LOG_LEVEL::LOG_ERROR);
            return;
        }
    } else if (status == CALIBRATION_STATUS::WARNING) {
        showLog(info, LOG_LEVEL::LOG_WARNING);
    } else {
        showLog(info, LOG_LEVEL::LOG_INFO);
    }

    if (id < maxID) {
        switch (id) {
        case CALIBRATION_MODULE_ID::BLC:
            curID = CALIBRATION_MODULE_ID::LSC;
            showLog("Start LSC Calibration...", LOG_LEVEL::LOG_INFO);
            timer->start();
            break;
        case CALIBRATION_MODULE_ID::LSC:
            curID = CALIBRATION_MODULE_ID::AWB;
            showLog("Start AWB Calibration...", LOG_LEVEL::LOG_INFO);
            timer->start();
            break;
        case CALIBRATION_MODULE_ID::AWB:
            curID = CALIBRATION_MODULE_ID::CCM;
            showLog("Start CCM Calibration...", LOG_LEVEL::LOG_INFO);
            timer->start();
            break;
        case CALIBRATION_MODULE_ID::CCM:
            curID = CALIBRATION_MODULE_ID::NOISE_PROFILE;
            showLog("Start Noise Profile Calibration...", LOG_LEVEL::LOG_INFO);
            timer->start();
            break;
        case CALIBRATION_MODULE_ID::NOISE_PROFILE:
            curID = CALIBRATION_MODULE_ID::LBLC;
            showLog("Start LBLC Calibration...", LOG_LEVEL::LOG_INFO);
            timer->start();
            break;
        default:
            showLog("Not Support, Calibration End!!!", LOG_LEVEL::LOG_WARNING);
            break;
        }
    } else {
        showLog("Calibration finish!", LOG_LEVEL::LOG_INFO);
    }
}
