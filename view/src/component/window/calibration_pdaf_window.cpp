#include "calibration_pdaf_window.hpp"

#define SUCCESS (0)
#define FAILURE (1)
#define MIN_WINDOW_WIDTH (1080)
#define MIN_WINDOW_HEIGHT (720)
#define WIDGET_ITEM_WIDTH (125)
#define GAIN_MAPS_YML "gain_maps.yml"


CalibrationPdafWindow::CalibrationPdafWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle(tr("PDAF Calibration"));
    initUI();
    initEvent();
}

void CalibrationPdafWindow::initUI()
{
    main_widget = new QWidget();
    main_widget->setMinimumSize(QSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT));
    main_layout = new QHBoxLayout();
    left_layout = new QGridLayout();

    QHBoxLayout *raw_width_layout = new QHBoxLayout;
    QLabel *raw_width_lab = new QLabel("raw width:");
    raw_width_lab->setFixedSize(WIDGET_ITEM_WIDTH, 25);
    raw_width_edit  = new QLineEdit();
    raw_width_edit->setText("0");
    raw_width_edit->setFixedWidth(100);
    raw_width_layout->addWidget(raw_width_lab, Qt::AlignLeft);
    raw_width_layout->addWidget(raw_width_edit, Qt::AlignLeft);

    QHBoxLayout *raw_height_layout = new QHBoxLayout;
    QLabel *raw_height_lab = new QLabel("raw height:");
    raw_height_lab->setFixedSize(WIDGET_ITEM_WIDTH, 25);
    raw_height_edit  = new QLineEdit();
    raw_height_edit->setText("0");
    raw_height_edit->setFixedWidth(100);
    raw_height_layout->addWidget(raw_height_lab, Qt::AlignLeft);
    raw_height_layout->addWidget(raw_height_edit, Qt::AlignLeft);

    QHBoxLayout *black_level_layout = new QHBoxLayout;
    QLabel *black_level_lab = new QLabel("black level:");
    black_level_lab->setFixedSize(WIDGET_ITEM_WIDTH, 25);
    black_level_edit  = new QLineEdit();
    black_level_edit->setText("0");
    black_level_edit->setFixedWidth(100);
    black_level_layout->addWidget(black_level_lab, Qt::AlignLeft);
    black_level_layout->addWidget(black_level_edit, Qt::AlignLeft);

    QHBoxLayout *numBlocksX_layout = new QHBoxLayout;
    QLabel *numBlocksX_lab = new QLabel("numBlocksX:");
    numBlocksX_lab->setFixedSize(WIDGET_ITEM_WIDTH, 25);
    numBlocksX_edit  = new QLineEdit();
    numBlocksX_edit->setText("10");
    numBlocksX_edit->setFixedWidth(100);
    numBlocksX_layout->addWidget(numBlocksX_lab, Qt::AlignLeft);
    numBlocksX_layout->addWidget(numBlocksX_edit, Qt::AlignLeft);

    QHBoxLayout *numBlocksY_layout = new QHBoxLayout;
    QLabel *numBlocksY_lab = new QLabel("numBlocksY:");
    numBlocksY_lab->setFixedSize(WIDGET_ITEM_WIDTH, 25);
    numBlocksY_edit  = new QLineEdit();
    numBlocksY_edit->setText("10");
    numBlocksY_edit->setFixedWidth(100);
    numBlocksY_layout->addWidget(numBlocksY_lab, Qt::AlignLeft);
    numBlocksY_layout->addWidget(numBlocksY_edit, Qt::AlignLeft);

    QHBoxLayout *is_gainMap_block_layout = new QHBoxLayout;
    QLabel *is_gainMap_block_lab = new QLabel("block gainMap:");
    is_gainMap_block_lab->setFixedSize(WIDGET_ITEM_WIDTH, 25);
    is_gainMap_block_cbox  = new QCheckBox();
    is_gainMap_block_cbox->setChecked(false);
    is_gainMap_block_cbox->setFixedWidth(100);
    is_gainMap_block_layout->addWidget(is_gainMap_block_lab, Qt::AlignLeft);
    is_gainMap_block_layout->addWidget(is_gainMap_block_cbox, Qt::AlignLeft);

    QHBoxLayout *bayer_layout = new QHBoxLayout;
    QLabel *bayer_label = new QLabel("bayer id:");
    bayer_label->setFixedSize(WIDGET_ITEM_WIDTH, 25);
    bayer_combobox = new QComboBox;
    bayer_combobox->setFixedWidth(100);
    for (int i = utils::BAYER_ID_BEGIN; i < utils::BAYER_ID_RGB; i++) {
        bayer_combobox->addItem(utils::getBayerName(i), i);
    }
    bayer_combobox->setCurrentIndex(utils::BAYER_ID_BG);
    bayer_layout->addWidget(bayer_label, Qt::AlignLeft);
    bayer_layout->addWidget(bayer_combobox, Qt::AlignLeft);

    QHBoxLayout *is_ZNCC_layout = new QHBoxLayout;
    QLabel *is_ZNCC_lab = new QLabel("ZNCC phase:");
    is_ZNCC_lab->setFixedSize(WIDGET_ITEM_WIDTH, 25);
    is_ZNCC_cbox  = new QCheckBox();
    is_ZNCC_cbox->setChecked(true);
    is_ZNCC_cbox->setFixedWidth(100);
    is_ZNCC_layout->addWidget(is_ZNCC_lab, Qt::AlignLeft);
    is_ZNCC_layout->addWidget(is_ZNCC_cbox, Qt::AlignLeft);

    shared_ptr<GlobalData> global_data = GlobalData::getInstance();
    QString save_dir = global_data->getSettings(SETTINGS_SECTION, KEY_CALI_CAPTURE_SAVE_PATH).toString();
    QString exe_dir = QCoreApplication::applicationDirPath();
    save_btn = new QPushButton(tr("Save Path"));
    save_path = new QLineEdit();
    save_path->setReadOnly(true);
    if (!save_dir.isEmpty()) {
        QDir dir(save_dir);
        if(dir.exists()) {
            save_path->setText(save_dir);
        } else {
            save_path->setText(exe_dir);
            global_data->setSettings(SETTINGS_SECTION, KEY_CAPTURE_SAVE_PATH, "");
            global_data->saveSettings();
            QMessageBox::warning(this, tr("Warning"), tr("Can't find \"" + save_dir.toUtf8() + "\" directory! Use default save path."), QMessageBox::Ok);
        }
    } else {
        save_path->setText(exe_dir);
    }
    QHBoxLayout *filepath_layout = new QHBoxLayout;
    filepath_layout->addWidget(save_btn);
    filepath_layout->addWidget(save_path);
    filepath_layout->setContentsMargins(0, 0, 0, 10);
    connect(save_btn, &QPushButton::clicked, this, &CalibrationPdafWindow::OpenSaveFilePath);

    QHBoxLayout *rawpath_layout = new QHBoxLayout;
    open_raw_btn = new QPushButton(tr("Open Raw"));
    raw_file_path = new QLineEdit();
    raw_file_path->setReadOnly(true);
    rawpath_layout->addWidget(open_raw_btn);
    rawpath_layout->addWidget(raw_file_path);
    connect(open_raw_btn, &QPushButton::clicked, this, &CalibrationPdafWindow::OpenRawFilePath);

    QHBoxLayout *openpath_layout = new QHBoxLayout;
    open_btn = new QPushButton(tr("Open Path"));
    open_path = new QLineEdit();
    open_path->setReadOnly(true);
    openpath_layout->addWidget(open_btn);
    openpath_layout->addWidget(open_path);
    connect(open_btn, &QPushButton::clicked, this, &CalibrationPdafWindow::OpenRawsFilePath);

    QHBoxLayout *calibrate_layout1 = new QHBoxLayout;
    QHBoxLayout *calibrate_layout2 = new QHBoxLayout;
    calibrate_GainMap_btn = new QPushButton(tr("Calibrate GainMap"));
    calibrate_DCC_btn = new QPushButton(tr("Calibrate DCC"));
    calibrate_layout1->addWidget(calibrate_GainMap_btn);
    calibrate_layout2->addWidget(calibrate_DCC_btn);
    calibrate_layout1->setContentsMargins(0, 0, 0, 10);
    calibrate_layout2->setContentsMargins(0, 0, 0, 20);
    connect(calibrate_GainMap_btn, &QPushButton::clicked, this, &CalibrationPdafWindow::calibrateGainMap);
    connect(calibrate_DCC_btn, &QPushButton::clicked, this, &CalibrationPdafWindow::calibrateDCC);

    QVBoxLayout *log_layout = new QVBoxLayout;
    log_edit = new QPlainTextEdit;
    log_edit->setFixedHeight(200);
    log_edit->setReadOnly(true);
    QHBoxLayout *log_opt_layout = new QHBoxLayout;
    QPushButton *clear_btn = new QPushButton("Clear");
    clear_btn->setFixedSize(80, 25);
    connect(clear_btn, &QPushButton::clicked, [=]{
        log_edit->clear();
    });
    QLabel *log_level_lab = new QLabel("Log Level:");
    log_level_lab->setFixedSize(80, 25);
    log_level_box = new QComboBox;
    log_level_box->setFixedSize(120, 25);
    QStringList level_str_list = {"LOG_ERROR", "LOG_WARNING", "LOG_INFO", "LOG_DEBUG"};
    log_level_box->addItems(level_str_list);
    log_level_box->setCurrentIndex(2);
    log_opt_layout->addWidget(clear_btn, Qt::AlignLeft);
    log_opt_layout->addStretch(1);
    log_opt_layout->addWidget(log_level_lab, Qt::AlignLeft);
    log_opt_layout->addWidget(log_level_box, Qt::AlignLeft);
    log_opt_layout->addStretch(1);
    log_layout->addLayout(log_opt_layout);
    log_layout->addWidget(log_edit);

    left_layout->addLayout(raw_width_layout, 0, 0, Qt::AlignTop);
    left_layout->addLayout(raw_height_layout, 1, 0, Qt::AlignTop);
    left_layout->addLayout(black_level_layout, 2, 0, Qt::AlignTop);
    left_layout->addLayout(numBlocksX_layout, 3, 0, Qt::AlignTop);
    left_layout->addLayout(numBlocksY_layout, 4, 0, Qt::AlignTop);
    left_layout->addLayout(bayer_layout, 5, 0, Qt::AlignTop);
    left_layout->addLayout(is_gainMap_block_layout, 6, 0, Qt::AlignTop);
    left_layout->addLayout(is_ZNCC_layout, 7, 0, Qt::AlignTop);
    left_layout->addLayout(filepath_layout, 8, 0, Qt::AlignTop);
    left_layout->addLayout(rawpath_layout, 9, 0, Qt::AlignTop);
    left_layout->addLayout(calibrate_layout1, 10, 0, Qt::AlignTop);
    left_layout->addLayout(openpath_layout, 11, 0, Qt::AlignTop);
    left_layout->addLayout(calibrate_layout2, 12, 0, Qt::AlignTop);
    left_layout->addLayout(log_layout, 13, 0, Qt::AlignBottom);
    left_layout->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);

    img_viewer = new MultiImageViewerWidget();
    img_viewer->setFilePathHidden();
    img_viewer->showLevelMeterBtn();
    img_viewer->showRawStatisticBtn();
    image_label = new ImageLabel(this, ImageLabel::NORMAL);
    connect(image_label, &ImageLabel::sigPositionChange, img_viewer, &MultiImageViewerWidget::slotPositionChange);
    img_viewer->setImageLabel(image_label);
    img_viewer->setMinimumHeight(600);

    main_layout->addLayout(left_layout, 1);
    main_layout->addWidget(img_viewer, 2);

    setCentralWidget(main_widget);
    centralWidget()->setLayout(main_layout);
}

void CalibrationPdafWindow::initEvent()
{
}

bool CalibrationPdafWindow::CheckSaveFilePath() {
    if (!save_path->text().isEmpty()) {
        QDir dir(save_path->text());
        if(!dir.exists()) {
            showLog("Can't find \"" + save_path->text().toUtf8() + "\" directory! Please check save path.", LOG_LEVEL::LOG_ERROR);
            return false;
        }
    } else {
       showLog( "Save path is empty!", LOG_LEVEL::LOG_ERROR);
        return false;
    }

    return true;
}

bool CalibrationPdafWindow::creatDir()
{
    if (!CheckSaveFilePath()) {
      return false;
    }

    showLog("CalibrationPdafWindow::creatDir ", LOG_LEVEL::LOG_DEBUG);

    if (directory_str.isEmpty()) {
        dateTime_str = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
        directory_str = save_path->text() + "/PDAF_" + dateTime_str + "/";
        QDir().mkdir(directory_str);
    }

    return true;
}

void CalibrationPdafWindow::showLog(const QString &log, LOG_LEVEL log_level)
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

void CalibrationPdafWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
    QDir dir(directory_str + "/temporary");
    if (dir.exists()) {
        dir.removeRecursively();
    }

    log_edit->clear();
    directory_str.clear();
    image_label->clear();
    img_viewer->updatePrompt(utils::IMAGE_TYPE::IMAGE_UNKNOWN, 0, 0);
}

void CalibrationPdafWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        this->close();
    }
}

void CalibrationPdafWindow::OpenSaveFilePath()
{
    QString dir_path = QFileDialog::getExistingDirectory(this, "Please Select Image Save Path", "./");
    qDebug("Opened Save Path: %s", qUtf8Printable(dir_path));
    if (!dir_path.isEmpty()) {
        save_path->setText(dir_path);
        shared_ptr<GlobalData> global_data = GlobalData::getInstance();
        global_data->setSettings(SETTINGS_SECTION, KEY_CALI_CAPTURE_SAVE_PATH, dir_path);
        global_data->saveSettings();
    }
}

void CalibrationPdafWindow::OpenRawsFilePath()
{
    QString dir_path = QFileDialog::getExistingDirectory(this, "Please Select Image Save Path", "./");
    qDebug("Opened Raws File Path: %s", qUtf8Printable(dir_path));
    if (!dir_path.isEmpty()) {
        open_path->setText(dir_path);
    } else {
        showLog(QString("raw files path must be valid!"), LOG_LEVEL::LOG_ERROR);
        return;
    }

    QString rawDir = R"(C:\Users\yiming.xu\PDAF\Calib\x64\Debug\PDraw)";

    QDir dir(open_path->text());
    dir.setFilter(QDir::Files | QDir::NoSymLinks | QDir::Readable);

    QStringList nameFilters;
    nameFilters << "*.raw";
    QFileInfoList entries = dir.entryInfoList(nameFilters);

    raw_files.clear();

    for (const QFileInfo& fi : entries) {
        const QString name = fi.fileName();

        if (!name.startsWith("PD"))
            continue;

        if (!name.contains("_reshape"))
            continue;

        raw_files.push_back(fi.absoluteFilePath());
        // raws.push_back(fi.fileName());
    }

    std::sort(raw_files.begin(), raw_files.end());

    int raw_width = raw_width_edit->text().toInt();
    int raw_height = raw_height_edit->text().toInt();
    int black_level_value = black_level_edit->text().toInt();

    if (raw_width <=0 || raw_height <= 0) {
        showLog(QString("raw width and  raw height must bigger than 0 !"), LOG_LEVEL::LOG_ERROR);
        return;
    }

    if (black_level_value < 0) {
        showLog(QString("black level value must bigger than 0 !"), LOG_LEVEL::LOG_ERROR);
        return;
    }

    uint16_t *pixels = (uint16_t*)malloc(raw_height * raw_width * sizeof(uint16_t));
    QByteArray raw_image;

    raw_image.clear();
    for (int idx = 0; idx < raw_files.size(); ++idx) {
        if (processRawImage(raw_files[idx], raw_width, raw_height, black_level_value, pixels) != SUCCESS) {
            showLog(QString("Process raw image failure!"), LOG_LEVEL::LOG_ERROR);
            return;
        }
        raw_image.append((char *)pixels, raw_height * raw_width * 2);
    }
    img_viewer->setRawImgData(raw_image, raw_width, raw_height, (utils::RAW_BAYER_ID) bayer_combobox->currentIndex(),
                              utils::RAW_BITS::BITS_12, utils::RAW_PACK_TYPE::RAW_UNPACKED);
    free(pixels);
}

void CalibrationPdafWindow::OpenRawFilePath()
{
    QString raw_path = QFileDialog::getOpenFileName(this, tr("Open File"), "", "*.jpg *.png *.raw *.tiff *.yuv");
    qDebug("Opened Raws File Path: %s", qUtf8Printable(raw_path));
    if (!raw_path.isEmpty()) {
        raw_file_path->setText(raw_path);
    } else {
        showLog(QString("raw file path must be valid!"), LOG_LEVEL::LOG_ERROR);
        return;
    }

    int raw_width = raw_width_edit->text().toInt();
    int raw_height = raw_height_edit->text().toInt();
    int black_level_value = black_level_edit->text().toInt();

    if (raw_width <=0 || raw_height <= 0) {
        showLog(QString("raw width and  raw height must bigger than 0 !"), LOG_LEVEL::LOG_ERROR);
        return;
    }

    if (black_level_value < 0) {
        showLog(QString("black level value must bigger than 0 !"), LOG_LEVEL::LOG_ERROR);
        return;
    }

    uint16_t *pixels = (uint16_t*)malloc(raw_height * raw_width * sizeof(uint16_t));
    QByteArray raw_image;

    if (processRawImage(raw_file_path->text(), raw_width, raw_height, black_level_value, pixels) != SUCCESS) {
        showLog(QString("Process raw image failure!"), LOG_LEVEL::LOG_ERROR);
        return;
    }

    raw_image.clear();
    raw_image.append((char *)pixels, raw_height * raw_width * 2);
    img_viewer->setRawImgData(raw_image, raw_width, raw_height, (utils::RAW_BAYER_ID) bayer_combobox->currentIndex(),
                              utils::RAW_BITS::BITS_12, utils::RAW_PACK_TYPE::RAW_UNPACKED);
    free(pixels);
}

void CalibrationPdafWindow::calibrateGainMap()
{
    creatDir();

    int raw_width = raw_width_edit->text().toInt();
    int raw_height = raw_height_edit->text().toInt();
    int black_level_value = black_level_edit->text().toInt();
    int width = raw_width / 2;
    int height = raw_height;
    bool is_block = is_gainMap_block_cbox->isChecked();

    if (raw_width <=0 || raw_height <= 0) {
        showLog(QString("raw width and  raw height must bigger than 0 !"), LOG_LEVEL::LOG_ERROR);
        return;
    }
    if (black_level_value < 0) {
        showLog(QString("black level value must bigger than 0 !"), LOG_LEVEL::LOG_ERROR);
        return;
    }

    uint16_t *processed_raw = (uint16_t*)malloc(raw_height * raw_width * sizeof(uint16_t));
    uint16_t *calib_L_raw = (uint16_t*)malloc(raw_height * raw_width / 2 * sizeof(uint16_t));
    uint16_t *calib_R_raw = (uint16_t*)malloc(raw_height * raw_width / 2 * sizeof(uint16_t));

    if (processRawImage(raw_file_path->text(), raw_width, raw_height, black_level_value, processed_raw) != SUCCESS) {
        showLog(QString("Process raw image failure!"), LOG_LEVEL::LOG_ERROR);
        return;
    }
    if (getLRImage(processed_raw, raw_width, raw_height, calib_L_raw, calib_R_raw) != SUCCESS) {
        showLog(QString("getLRImage failure!"), LOG_LEVEL::LOG_ERROR);
        return;
    }

    cv::Mat calib_L(height, width, CV_16UC1, calib_L_raw);
    cv::Mat calib_R(height, width, CV_16UC1, calib_R_raw);

    int centerHeight = static_cast<int> (height * 0.01);
    int centerWidth = static_cast<int>(width * 0.01);

    // Calculate the starting coordinates of the center block
    int startX = (width - centerWidth) / 2;
    int startY = (height - centerHeight) / 2;

    // Calculate the average pixel value of the central block
    float centerBlockAvg_L = calculateBlockAverage(calib_L, startX, startY, centerWidth, centerHeight);
    float centerBlockAvg_R = calculateBlockAverage(calib_R, startX, startY, centerWidth, centerHeight);
    float centerBlockAvg = (centerBlockAvg_L + centerBlockAvg_R) / 2;

    if (is_block) {
        int numBlocksX = numBlocksX_edit->text().toInt();
        int numBlocksY = numBlocksY_edit->text().toInt();    // reference sony 8*11
        int calib_blockWidth, calib_blockHeight, calib_paddingLeft, calib_paddingRight, calib_paddingTop, calib_paddingBottom;
        calculateBlockParameters(width, height, numBlocksX, numBlocksY, calib_blockWidth, calib_blockHeight, calib_paddingLeft,
                                 calib_paddingRight, calib_paddingTop, calib_paddingBottom);

        cv::Mat gainMap_L(numBlocksY, numBlocksX, CV_32F);
        cv::Mat gainMap_R(numBlocksY, numBlocksX, CV_32F);

        // Traverse all points and calculate the gain ratio
        for (int i = 0; i < numBlocksX; ++i) {
            for (int j = 0; j < numBlocksY; ++j) {

                int startX = calib_paddingLeft + i * calib_blockWidth;
                int startY = calib_paddingTop + j * calib_blockHeight;

                // Dynamically adjust the width and height of the block
                int currentBlockWidth = (i == numBlocksX - 1) ? calib_blockWidth + calib_paddingRight : calib_blockWidth;
                int currentBlockHeight = (j == numBlocksY - 1) ? calib_blockHeight + calib_paddingBottom : calib_blockHeight;

                // Ensure that the block does not exceed the image range
                if (startX + currentBlockWidth > width) currentBlockWidth = width - startX;
                if (startY + currentBlockHeight > height) currentBlockHeight = height - startY;

                // Calculate the average value of the current block
                float blockAvg_L = calculateBlockAverage(calib_L, startX, startY, currentBlockWidth, currentBlockHeight);
                float blockAvg_R = calculateBlockAverage(calib_R, startX, startY, currentBlockWidth, currentBlockHeight);

                if (blockAvg_L == 0.0) blockAvg_L = 1.0;
                if (blockAvg_R == 0.0) blockAvg_R = 1.0;

                // Calculate the gain ratio
                gainMap_L.at<float>(j, i) = MIN(centerBlockAvg / blockAvg_L, 4.0) * 64;
                gainMap_R.at<float>(j, i) = MIN(centerBlockAvg / blockAvg_R, 4.0) * 64;
            }
        }
        saveGainMap(gainMap_L, directory_str + QString("gainmap_l_64_64"));
        saveGainMap(gainMap_R, directory_str + QString("gainmap_r_64_64"));
    } else {
        // Full-graph gain calculation
        cv::Mat gainMap_L_pdaf(height, width, CV_32F);
        cv::Mat gainMap_R_pdaf(height, width, CV_32F);
        cv::Mat gainMap_L(height, width, CV_32F);
        cv::Mat gainMap_R(height, width, CV_32F);

        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                uint16_t value_L = calib_L.at<uint16_t>(j, i);
                uint16_t value_R = calib_R.at<uint16_t>(j, i);

                if (value_L == 0.0) value_L = 1.0;
                if (value_R == 0.0) value_R = 1.0;

                gainMap_L_pdaf.at<float>(j, i) = MIN(centerBlockAvg / value_L, 4.0) * 64;
                gainMap_R_pdaf.at<float>(j, i) = MIN(centerBlockAvg / value_R, 4.0) * 64;

                gainMap_L.at<float>(j, i) = centerBlockAvg / value_L;
                gainMap_R.at<float>(j, i) = centerBlockAvg / value_R;
            }
        }
        saveGainMap(gainMap_L_pdaf, directory_str + QString("resize_gainmap_L"));
        saveGainMap(gainMap_R_pdaf, directory_str + QString("resize_gainmap_R"));
        // Yaml used for calibrating DCC
        saveGainMaptoYaml(gainMap_L, gainMap_R, directory_str + QString(GAIN_MAPS_YML));
    }

    if (processed_raw != nullptr)
        free(processed_raw);
    if (calib_L_raw != nullptr)
        free(calib_L_raw);
    if (calib_R_raw != nullptr)
        free(calib_R_raw);

    showLog(QString("The pixel mean value of the L central region is: %1").arg(centerBlockAvg_L), LOG_LEVEL::LOG_INFO);
    showLog(QString("The pixel mean value of the R central region is: %1").arg(centerBlockAvg_R), LOG_LEVEL::LOG_INFO);
    showLog(QString("gainmap files have been saved to %1.").arg(directory_str), LOG_LEVEL::LOG_INFO);
    showLog(QString("Calibrate GainMap seccess!"), LOG_LEVEL::LOG_INFO);
}

void CalibrationPdafWindow::calibrateDCC()
{
    creatDir();
    if (!QFile::exists(directory_str + QString(GAIN_MAPS_YML))) {
        showLog(QString("Can't find %1 !").arg(GAIN_MAPS_YML), LOG_LEVEL::LOG_ERROR);
        showLog(QString("Please calibrate GainMap by none block !"), LOG_LEVEL::LOG_ERROR);
        return;
    }

    int raw_width = raw_width_edit->text().toInt();
    int raw_height = raw_height_edit->text().toInt();
    int black_level_value = black_level_edit->text().toInt();
    int width = raw_width / 2;
    int height = raw_height;
    int numBlocksX_phase = numBlocksX_edit->text().toInt();
    int numBlocksY_phase = numBlocksY_edit->text().toInt();
    int blocks_per_img = numBlocksX_phase * numBlocksY_phase;

    if (raw_width <=0 || raw_height <= 0) {
        showLog(QString("raw width and  raw height must bigger than 0 !"), LOG_LEVEL::LOG_ERROR);
        return;
    }
    if (black_level_value < 0) {
        showLog(QString("black level value must bigger than 0 !"), LOG_LEVEL::LOG_ERROR);
        return;
    }
    if (numBlocksX_phase <=0 || numBlocksY_phase <= 0) {
        showLog(QString("numBlocksX and  numBlocksY must bigger than 0 !"), LOG_LEVEL::LOG_ERROR);
        return;
    }

    int NumofRAWfiles = (int)raw_files.size();
    showLog(QString("%1 correctly named RAW files.").arg(NumofRAWfiles), LOG_LEVEL::LOG_INFO);

    if (NumofRAWfiles <= 0) {
        showLog(QString("Calibrate DCC failure!"), LOG_LEVEL::LOG_ERROR);
        return;
    }

    cv::Mat PDvalueMat = cv::Mat::zeros(NumofRAWfiles, blocks_per_img, CV_32F);
    vector<string> PDlabels; PDlabels.reserve(NumofRAWfiles);

    for (int idx = 0; idx < NumofRAWfiles; ++idx) {
        // Extract label, for example "PD16.1_reshape.raw" -> "16.1"
        QFileInfo fi(raw_files[idx]);
        string fname = fi.fileName().toStdString(); // PD16.1_reshape
        auto p1 = fname.find("PD");
        auto p2 = fname.find("_");
        string label = fname.substr(p1 + 2, p2 - (p1 + 2));
        PDlabels.push_back(label);

        uint16_t *processed_raw = (uint16_t*)malloc(raw_height * raw_width * sizeof(uint16_t));
        uint16_t *new_L_raw = (uint16_t*)malloc(raw_height * raw_width / 2 * sizeof(uint16_t));
        uint16_t *new_R_raw = (uint16_t*)malloc(raw_height * raw_width / 2 * sizeof(uint16_t));

        if (processRawImage(raw_files[idx], raw_width, raw_height, black_level_value, processed_raw) != SUCCESS) {
            showLog(QString("Process raw image failure!"), LOG_LEVEL::LOG_ERROR);
            return;
        }
        if (getLRImage(processed_raw, raw_width, raw_height, new_L_raw, new_R_raw) != SUCCESS) {
            showLog(QString("getLRImage failure!"), LOG_LEVEL::LOG_ERROR);
            return;
        }

        cv::Mat new_L(height, width, CV_16UC1, new_L_raw);
        cv::Mat new_R(height, width, CV_16UC1, new_R_raw);

        cv::Mat L_raw_8bit, R_raw_8bit;
        new_L.convertTo(L_raw_8bit, CV_8U, 255.0 / 4095.0);
        new_R.convertTo(R_raw_8bit, CV_8U, 255.0 / 4095.0);

        cv::Mat loadedGainMap_L, loadedGainMap_R;
        loadGainMap(loadedGainMap_L, loadedGainMap_R, directory_str + QString(GAIN_MAPS_YML));

        cv::Mat adjusted_L = new_L.clone();
        cv::Mat adjusted_R = new_R.clone();

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                float gain_L = loadedGainMap_L.at<float>(y, x);
                float gain_R = loadedGainMap_R.at<float>(y, x);

                adjusted_L.at<uint16_t>(y, x) = cv::saturate_cast<uint16_t>(new_L.at<uint16_t>(y, x) * gain_L);
                adjusted_R.at<uint16_t>(y, x) = cv::saturate_cast<uint16_t>(new_R.at<uint16_t>(y, x) * gain_R);
            }
        }

        int blockWidth, blockHeight, paddingLeft, paddingRight, paddingTop, paddingBottom;
        calculateBlockParameters(width, height, numBlocksX_phase, numBlocksY_phase, blockWidth, blockHeight, paddingLeft, paddingRight, paddingTop, paddingBottom);

        // Create DCC block coordinate files
        // Create block coordinate files only for the first file
        if (idx == 0) {
            std::ofstream fileX_txt((directory_str + "DCC_XAddress.txt").toStdString());
            std::ofstream fileY_txt((directory_str + "DCC_YAddress.txt").toStdString());
            std::ofstream fileX_bin((directory_str + "dcc_xaddress.bin").toStdString(), std::ios::binary);
            std::ofstream fileY_bin((directory_str + "dcc_yaddress.bin").toStdString(), std::ios::binary);
            if (!fileX_txt || !fileY_txt || !fileX_bin || !fileY_bin) {
                showLog(QString("Cannot open output file!"), LOG_LEVEL::LOG_ERROR);
                return;
            }

            for (int i = 0; i < numBlocksX_phase; ++i) {
                int centerX = paddingLeft + i * blockWidth + blockWidth / 2;
                fileX_txt << centerX
                    << (i + 1 < numBlocksX_phase ? "," : "\n");
                uint16_t cx = static_cast<uint16_t>(centerX);
                fileX_bin.write(reinterpret_cast<char*>(&cx), sizeof(cx));
            }

            for (int j = 0; j < numBlocksY_phase; ++j) {
                int centerY = paddingTop + j * blockHeight + blockHeight / 2;
                fileY_txt << centerY
                    << (j + 1 < numBlocksY_phase ? "," : "\n");
                uint16_t cy = static_cast<uint16_t>(centerY);
                fileY_bin.write(reinterpret_cast<char*>(&cy), sizeof(cy));
            }

            fileX_txt.close();
            fileY_txt.close();
            fileX_bin.close();
            fileY_bin.close();
        }
        showLog(QString("processing No. %1 raw file...").arg(idx), LOG_LEVEL::LOG_ERROR);

        // Block phase difference calculation
        for (int j = 0; j < numBlocksY_phase; ++j) {
            for (int i = 0; i < numBlocksX_phase; ++i) {
                int startX = paddingLeft + i * blockWidth;
                int startY = paddingTop + j * blockHeight;

                int max_shift = blockWidth / 8;
                double optimal_shift = calculate_optimal_shift(adjusted_L, adjusted_R, startX, startY, blockWidth, blockHeight, max_shift);
                int b = j * numBlocksY_phase + i;

                PDvalueMat.at<float>(idx, b) = optimal_shift;
            }
        }

        //// Get the maximum and minimum values ​​in the corrected image
        // double minVal, maxVal;
        // Point minLoc, maxLoc;

        // minMaxLoc(adjusted_L, &minVal, &maxVal, &minLoc, &maxLoc);
        // printf("minVal in adjusted_L is %f, at(%d, %d)\n", minVal, minLoc.x, minLoc.y);
        // printf("maxVal in adjusted_L is %f, at(%d, %d)\n", maxVal, maxLoc.x, maxLoc.y);

        // minMaxLoc(adjusted_R, &minVal, &maxVal, &minLoc, &maxLoc);
        // printf("minVal in adjusted_R is %f, at(%d, %d)\n", minVal, minLoc.x, minLoc.y);
        // printf("maxVal in adjusted_R is %f, at(%d, %d)\n", maxVal, maxLoc.x, maxLoc.y);


        //// Display results
        /*imshow("Left Image with Phase Differences", left_image_color);
        imshow("Corrected Right", adjusted_R_8bit);
        imshow("Corrected Left", adjusted_L_8bit);
        imshow("Original_Right", R_raw_8bit);
        imshow("Original_Left", L_raw_8bit);
        waitKey(0);*/

        // imwrite("correcter_left.jpg", adjusted_L_8bit);
        // imwrite("correcter_right.jpg", adjusted_R_8bit);

        free(processed_raw);
        free(new_L_raw);
        free(new_R_raw);
    }
    // save PDvalueMat
    ofstream fMat((directory_str + QString("PDvalueMat.txt")).toStdString());
    for (int i = 0; i < PDvalueMat.rows; i++) {
        for (int j = 0; j < PDvalueMat.cols; j++) {
            double v = PDvalueMat.type() == CV_32F
                ? PDvalueMat.at<float>(i, j)
                : PDvalueMat.at<double>(i, j);
            fMat << v;
            if (j + 1 < PDvalueMat.cols) fMat << ",";  // Comma between columns
        }
        fMat << "\n";  // Line break
    }
    fMat.close();
    showLog("PDvalueMat.txt has been saved successfully.", LOG_LEVEL::LOG_INFO);

    // save PDlabels
    ofstream fLbl((directory_str + QString("PDlabels.txt")).toStdString());
    for (auto lab : PDlabels) {
        fLbl << lab << "\n";
    }
    fLbl.close();
    showLog("PDlabels.txt has been saved successfully.", LOG_LEVEL::LOG_INFO);

    //// calculate slope ////
    int N = PDvalueMat.rows;    // sample nums
    int M = PDvalueMat.cols;    // blocks_per_img

    // 1) Prepare ys: label*1000 as the vertical coordinate
    vector<double> ys(N);
    for (int i = 0; i < N; ++i) {
        ys[i] = stod(PDlabels[i]) * 1000.0;
    }

    // 2) Perform linear regression on each block column and find the slope k
    //    Here: xᵢ = –PDvalueMat(i,b), yᵢ = ys[i]
    vector<double> slopes(M, 0.0);
    for (int b = 0; b < M; ++b) {
        double sumx = 0, sumy = 0, sumxy = 0, sumx2 = 0;
        for (int i = 0; i < N; ++i) {
            double x = -(PDvalueMat.type() == CV_32F
                ? PDvalueMat.at<float>(i, b)
                : PDvalueMat.at<double>(i, b));
            double y = ys[i];
            sumx += x;
            sumy += y;
            sumxy += x * y;
            sumx2 += x * x;
        }
        double denom = N * sumx2 - sumx * sumx;
        if (fabs(denom) < 1e-12) {
            slopes[b] = 0;
        }
        else {
            // k = (N*Σ(xᵢ yᵢ) − Σxᵢ·Σyᵢ) / (N*Σ(xᵢ²) − (Σxᵢ)²)
            slopes[b] = (N * sumxy - sumx * sumy) / denom;
        }
    }

    // 3) Write on the same line, separated by comma
    //ofstream foutTxt("slopes.txt");
    //ofstream foutBin("dcc.bin", std::ios::binary);
    //for (int b = 0; b < M; ++b) {
    //    foutTxt << slopes[b];
    //    if (b + 1 < M) foutTxt << ",";
    //    foutBin.write(
    //        reinterpret_cast<const char*>(&slopes[b]),
    //        sizeof(slopes[b])
    //    );
    //}
    //foutTxt << "\n";
    //foutTxt.close();
    //foutBin.close();

    //printf("Done! %d slopes written to slopes.txt and dcc.bin\n", M);
    //return 0;

    ofstream foutTxt((directory_str + QString("dcc.txt")).toStdString());
    for (int b = 0; b < M; ++b) {
        foutTxt << slopes[b]
            << (b + 1 < M ? "," : "\n");
    }
    foutTxt.close();

    // 4) Write Binary —— using uint16_t
    ofstream foutBin((directory_str + QString("dcc.bin")).toStdString(), ios::binary);
    //for (int b = 0; b < M; ++b) {
    //    uint16_t slope_i = static_cast<uint16_t>(slopes[b]);
    //    foutBin.write(reinterpret_cast<const char*>(&slope_i),
    //        sizeof(slope_i));
    //}
    //foutBin.close();

    // Precompute the representable range of long long (to avoid undefined behavior of double → long long)
    constexpr double LL_MIN_D =
        static_cast<double>(std::numeric_limits<long long>::min());
    constexpr double LL_MAX_D =
        static_cast<double>(std::numeric_limits<long long>::max());

    for (int b = 0; b < M; ++b) {
        double v = slopes[b];

        if (!std::isfinite(v)) {
            v = 0.0;
        }

        v = std::trunc(v);

        // 3) Limit it to the representable range of long long to avoid UB of static_cast<long long>
        if (v < LL_MIN_D) v = LL_MIN_D;
        if (v > LL_MAX_D) v = LL_MAX_D;

        // 4) Convert to signed integer
        long long iv = static_cast<long long>(v);

        // 5) Finally convert to uint16_t and perform modulo operation according to 2^16
        uint16_t slope_i = static_cast<uint16_t>(iv);

        foutBin.write(reinterpret_cast<const char*>(&slope_i),
            sizeof(slope_i));
    }

    foutBin.close();
    showLog(QString("Done! %1 slopes written to slopes.txt and dcc.bin.").arg(M), LOG_LEVEL::LOG_INFO);
    showLog(QString("Calibrate DCC seccess!"), LOG_LEVEL::LOG_INFO);

}

int CalibrationPdafWindow::processRawImage(const QString file_path, int width, int height, int black_level_value, uint16_t* pixels)
{
    int num_pixels = width * height;
    int num_bytes = (num_pixels * 3) / 2;

    if (!pixels) {
        showLog(QString("processing raw image is null! "), LOG_LEVEL::LOG_ERROR);
        return FAILURE;
    }

    FILE* file = nullptr;
    errno_t err = fopen_s(&file, file_path.toStdString().c_str(), "rb");
    if (err != 0 || !file) {
        showLog(QString("Can't open raw file: ") + file_path, LOG_LEVEL::LOG_ERROR);
        return FAILURE;
    }

    uint8_t* raw_data = (uint8_t*)malloc(num_bytes);
    if (!raw_data) {
        showLog(QString("Memory allocation failure!"), LOG_LEVEL::LOG_ERROR);
        fclose(file);
        return FAILURE;
    }

    size_t bytes_read = fread(raw_data, 1, num_bytes, file);
    fclose(file);

    if (bytes_read != (size_t)num_bytes) {
        showLog(QString("The file size does not match the image dimensions! Read %1 bytes, but expected %2 bytes.").arg(bytes_read).arg(num_bytes),
                LOG_LEVEL::LOG_ERROR);
        free(raw_data);
        return FAILURE;
    }

    for (int i = 0; i < num_pixels; i += 2) {
        int j = (i / 2) * 3;
        uint8_t byte1 = raw_data[j];
        uint8_t byte2 = raw_data[j + 1];
        uint8_t byte3 = raw_data[j + 2];

        pixels[i] = ((byte1 << 4) | (byte3 & 0x0F));
        pixels[i + 1] = ((byte2 << 4) | (byte3 >> 4));
    }

    // Black level correction
    for (int i = 0; i < num_pixels; ++i) {
        int corrected_value = pixels[i] - black_level_value;
        pixels[i] = static_cast<uint16_t>(max(corrected_value, 0)); // 防止负值
    }

    free(raw_data);
    return SUCCESS;
}

int CalibrationPdafWindow::getLRImage(uint16_t* processed_raw, int width, int height, uint16_t* image_L, uint16_t* image_R)
{
    if (image_L == nullptr || image_R == nullptr) {
        showLog(QString("ImageLR can't be nullptr! "), LOG_LEVEL::LOG_ERROR);
        return FAILURE;
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x % 2 == 0) {
                image_L[y * (width / 2) + (x / 2)] = processed_raw[y * width + x];
            }
            else {
                image_R[y * (width / 2) + (x / 2)] = processed_raw[y * width + x];
            }
        }
    }
    return SUCCESS;
}

// Calculate the average pixel value of the block
float CalibrationPdafWindow::calculateBlockAverage(cv::Mat& image, int startX, int startY, int blockWidth, int blockHeight)
{
    cv::Scalar sum = cv::sum(image(cv::Rect(startX, startY, blockWidth, blockHeight)));
    float avg = static_cast<float>(sum[0]) / (blockWidth * blockHeight);
    return avg;
}

// Save the gain results to a file
void CalibrationPdafWindow::saveGainMap(const cv::Mat& gainMap, const QString file_path)
{
    ofstream file(file_path.toStdString() + ".txt");
    ofstream foutBin(file_path.toStdString() + ".bin", ios::binary);
    for (int i = 0; i < gainMap.rows; ++i) {
        for (int j = 0; j < gainMap.cols; ++j) {
            file << (int)gainMap.at<float>(i, j);
            file << ",";
            float f = gainMap.at<float>(i, j);
            uint8_t   v = static_cast<uint8_t>(f);
            foutBin.write(reinterpret_cast<const char*>(&v), sizeof(v));
        }
    }
    file.close();
    foutBin.close();
}

void CalibrationPdafWindow::loadGainMap(cv::Mat& gainMap_L, cv::Mat& gainMap_R, const QString file_path) {
    cv::FileStorage fs(file_path.toStdString(), cv::FileStorage::READ);
    fs["GainMap_L"] >> gainMap_L;
    fs["GainMap_R"] >> gainMap_R;
    fs.release();
}

// Save the Yaml file used for DCC calibration
void CalibrationPdafWindow::saveGainMaptoYaml(const cv::Mat& gainMap_L, const cv::Mat& gainMap_R, const QString file_path)
{
    cv::FileStorage fs(file_path.toStdString(), cv::FileStorage::WRITE);
    fs << "GainMap_L" << gainMap_L;
    fs << "GainMap_R" << gainMap_R;
    fs.release();
}

void CalibrationPdafWindow::calculateBlockParameters(int width, int height, int numBlocksX, int numBlocksY, int& blockWidth, int& blockHeight,
int& paddingLeft, int& paddingRight, int& paddingTop, int& paddingBottom)
{
    blockWidth = width / numBlocksX;
    blockHeight = height / numBlocksY;

    int extraWidth = width % numBlocksX;
    int extraHeight = height % numBlocksY;

    paddingLeft = extraWidth / 2;
    paddingRight = extraWidth - paddingLeft;
    paddingTop = extraHeight / 2;
    paddingBottom = extraHeight - paddingTop;
}

// Calculate the mean value of the given region
double CalibrationPdafWindow::mean(const cv::Mat& img, int x, int y, int width, int height)
{
    double sum = 0.0;
    for (int i = y; i < y + height; ++i) {
        for (int j = x; j < x + width; ++j) {
            sum += img.at<uint16_t>(i, j);
        }
    }
    return sum / (width * height);
}

// Calculate the standard deviation of the given region
double CalibrationPdafWindow::stddev(const cv::Mat& img, int x, int y, int width, int height, double mean)
{
    double sum = 0.0;
    for (int i = y; i < y + height; ++i) {
        for (int j = x; j < x + width; ++j) {
            double diff = img.at<uint16_t>(i, j) - mean;
            sum += diff * diff;
        }
    }
    return std::sqrt(sum / (width * height));
}

// Calculate the ZNCC values of two regions
double CalibrationPdafWindow::zncc(const cv::Mat& img1, int x1, int y1, const cv::Mat& img2, int x2, int y2, int width, int height)
{
    double mean1 = mean(img1, x1, y1, width, height);
    double mean2 = mean(img2, x2, y2, width, height);
    double stddev1 = stddev(img1, x1, y1, width, height, mean1);
    double stddev2 = stddev(img2, x2, y2, width, height, mean2);

    if (stddev1 == 0 || stddev2 == 0) {
        return -1.0;
    }

    double zncc_value = 0.0;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            double diff1 = img1.at<uint16_t>(y1 + i, x1 + j) - mean1;
            double diff2 = img2.at<uint16_t>(y2 + i, x2 + j) - mean2;
            zncc_value += (diff1 * diff2) / (stddev1 * stddev2);
        }
    }
    return zncc_value / (width * height);
}

// Calculate the SAD values of two regions
double CalibrationPdafWindow::SAD(const cv::Mat& img1, int x1, int y1, const cv::Mat& img2, int x2, int y2, int width, int height)
{
    double sad_value = 0.0;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int diff = static_cast<int>(img1.at<uint16_t>(y1 + i, x1 + j)) - static_cast<int>(img2.at<uint16_t>(y2 + i, x2 + j));
            sad_value += abs(diff);
        }
    }
    return sad_value / (width * height);
}

// Fit a parabola and find the vertex
double CalibrationPdafWindow::fit_parabola(double x1, double y1, double x2, double y2, double x3, double y3)
{
    if (x1 == x2 || x1 == x3 || x2 == x3) {
        showLog(QString("The horizontal coordinates cannot be the same when fitting a parabola! "),
                LOG_LEVEL::LOG_ERROR);
        return 99; // Error flag
    }
    double denom = (x1 - x2) * (x1 - x3) * (x2 - x3);
    double A = (x3 * (y2 - y1) + x2 * (y1 - y3) + x1 * (y3 - y2)) / denom;
    double B = (x3 * x3 * (y1 - y2) + x2 * x2 * (y3 - y1) + x1 * x1 * (y2 - y3)) / denom;
    return -B / (2 * A);
}

double CalibrationPdafWindow::calculate_optimal_shift(const cv::Mat& img1, const cv::Mat& img2, int x, int y,
int width, int height, int max_shift)
{
    double optimal_shift;
    bool is_zncc = is_ZNCC_cbox->isChecked();
    if (is_zncc) {
        double max_zncc = -1.0;
        int best_shift = 0;
        std::vector<double> zncc_values(max_shift * 2 + 1, -1.0);

        for (int shift = -max_shift; shift <= max_shift; ++shift) {
            if ((x + shift) < 0 || (x + shift + width) > img2.cols) {
                continue;
            }
            double zncc_value = zncc(img1, x, y, img2, x + shift, y, width, height);
            zncc_values[shift + max_shift] = zncc_value;
            if (zncc_value > max_zncc) {
                max_zncc = zncc_value;
                best_shift = shift;
            }
        }
        // printf("%d\t", best_shift);

        optimal_shift = best_shift;

        if (best_shift > -max_shift && best_shift < max_shift) {
            double x1, x2, x3, y1, y2, y3;

            if (best_shift - 1 > -max_shift) {
                x1 = best_shift - 1;
            }
            else {
                x1 = -max_shift;
            }

            x2 = best_shift;

            if (best_shift + 1 < max_shift) {
                x3 = best_shift + 1;
            }
            else {
                x3 = max_shift;
            }
            y1 = zncc_values[x1 + max_shift];
            y2 = zncc_values[x2 + max_shift];
            y3 = zncc_values[x3 + max_shift];

            optimal_shift = fit_parabola(x1, y1, x2, y2, x3, y3);
        }
    } else {
        double min_sad = DBL_MAX;
        int best_shift = 0;
        double* sad_values = (double*)malloc((max_shift * 2 + 1) * sizeof(double));

        if (sad_values == NULL) {
            showLog("SAD memory allocation failed!", LOG_LEVEL::LOG_ERROR);
            return 100;  // Error flag
        }

        for (int shift = -max_shift; shift <= max_shift; shift++) {
            if ((x + shift) < 0 || (x + shift + width) > img2.cols) {
                sad_values[shift + max_shift] = DBL_MAX;
                continue;
            }

            double sad_value = SAD(img1, x, y, img2, x + shift, y, width, height);
            sad_values[shift + max_shift] = sad_value;
            if (sad_value < min_sad) {
                min_sad = sad_value;
                best_shift = shift;
            }
        }

        optimal_shift = best_shift;

        if (best_shift > -max_shift && best_shift < max_shift) {

            int x1, x2, x3;
            double y1, y2, y3;
            if (best_shift - 1 > -max_shift) {
                x1 = best_shift - 1;
            }
            else {
                x1 = -max_shift;
            }

            x2 = best_shift;

            if (best_shift + 1 < max_shift) {
                x3 = best_shift + 1;
            }
            else {
                x3 = max_shift;
            }
            y1 = sad_values[x1 + max_shift];
            y2 = sad_values[x2 + max_shift];
            y3 = sad_values[x3 + max_shift];

            optimal_shift = fit_parabola(x1, y1, x2, y2, x3, y3);
        }
    }
    return optimal_shift;
}
