#include "calibration_distortion_window.hpp"
#include "raw_image_format_dialog.hpp"
#include "utils.hpp"
#include "value_validator.hpp"
#include "global_data.hpp"
#include "main_window.hpp"
#include "module_define.hpp"
#include <windows.h>
#include <QLabel>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QScrollBar>
#include <QImageReader>

#define MIN_WINDOW_WIDTH (1200)
#define MIN_WINDOW_HEIGHT (800)

CalibrationDistortionWindow::CalibrationDistortionWindow(QWidget *parent) :
    BaseCalibrationPageWidget(static_cast<int>(CALIBRATION_MODULE_ID::DISTORTION), parent)
{
    char *pversion = GetDLLVersionInfo(1);
    QString title = tr("Distortion Calibration");
    if (pversion) {
        title += QString(" - v%1").arg(pversion);
    }
    setWindowTitle(title);
    setWindowFlags(Qt::WindowMinMaxButtonsHint|Qt::WindowCloseButtonHint);
    initLeftArea();
    initCenterArea();
    initEvent();
}

void CalibrationDistortionWindow::initLeftArea()
{
    QScrollArea *scrol = new QScrollArea(this);
    scrol->setMinimumSize(400, 600);
    scrol->setWidgetResizable(true);
    scrol->setStyleSheet("background-color:rgb(245, 245, 245);");

    left_widget = new QWidget();
    left_layout = new QVBoxLayout(left_widget);
    scrol->setWidget(left_widget);

    QGroupBox *get_img_group = createGetImageGroupbox();
    QGroupBox *board_info_group = createBoardInfoGroupbox();
    QGroupBox *calib_param_group = createCalibParamGroupbox();
    QGroupBox *img_calib_group = createCalibrationGroupbox();
    QGroupBox *convert_path_group = createConvertPathInfoGroupbox();
    // QGroupBox *write_data_group = createWriteDataGroupbox();
    QGroupBox *image_list_group = createImageListGroupbox();

    left_layout->addWidget(get_img_group, 0, Qt::AlignTop);
    left_layout->addWidget(board_info_group, 0, Qt::AlignTop);
    left_layout->addWidget(calib_param_group, 0, Qt::AlignTop);
    left_layout->addWidget(img_calib_group, 0, Qt::AlignTop);
    left_layout->addWidget(convert_path_group, 0, Qt::AlignTop);
    // left_layout->addWidget(write_data_group, 0, Qt::AlignTop);
    left_layout->addWidget(image_list_group, 4, Qt::AlignTop);
    left_layout->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);
    left_layout->setAlignment(Qt::AlignTop);
    left_layout->setSpacing(20);

    param_layout->addWidget(scrol, 1);
}

void CalibrationDistortionWindow::initCenterArea()
{
    center_tab_widget = new QTabWidget;

    // input image display
    input_img = new MultiImageViewerWidget;
    input_image_label = new ImageLabel(this, ImageLabel::GRID);
    input_image_label->enableGrid(false);
    input_image_label->enableRect(false);
    input_image_label->setGridSize(3, 3);
    input_img->setImageLabel(input_image_label);
    input_img->setFilePathHidden();
    input_img->setFileButtonHidden();
    input_img->hideSimplePipeBtn();
    input_img->setColorHistogramButtonHidden();
    input_img->showEnGridCheckBox();
    center_tab_widget->addTab(input_img, "Input");
    connect(input_image_label, &ImageLabel::sigPositionChange, input_img, &MultiImageViewerWidget::slotPositionChange);
    input_img->setMinimumHeight(600);

    // ouput image display
    output_img = new MultiImageViewerWidget;
    output_image_label = new ImageLabel(this, ImageLabel::NORMAL);
    output_img->setImageLabel(output_image_label);
    output_img->setFilePathHidden();
    output_img->setFileButtonHidden();
    output_img->hideSimplePipeBtn();
    output_img->setColorHistogramButtonHidden();
    center_tab_widget->addTab(output_img, "Output");
    connect(output_image_label, &ImageLabel::sigPositionChange, output_img, &MultiImageViewerWidget::slotPositionChange);
    output_img->setMinimumHeight(600);

    center_tab_widget->setCurrentWidget(input_img);
    main_layout->addWidget(center_tab_widget, 3);
}


void CalibrationDistortionWindow::initEvent()
{
    shared_ptr<NetworkClientHelper> network_helper = NetworkClientHelper::getInstance();

    connect(network_helper.get(), &NetworkClientHelper::sigGetYuvStatus, this, &CalibrationDistortionWindow::handleMultiYuvStatus);
    connect(network_helper.get(), &NetworkClientHelper::sigGetYuvData, this, &CalibrationDistortionWindow::slotGotYuvImg);
}

QGroupBox *CalibrationDistortionWindow::createGetImageGroupbox()
{
    QGroupBox *groupbox = new QGroupBox();
    groupbox->setTitle("Calibration Source");

    //main layout
    QVBoxLayout *img_v_layout = new QVBoxLayout;

    //get image info layout
    QHBoxLayout *cap_src_info_layout = new QHBoxLayout;
    QLabel *cap_src_label = new QLabel;
    cap_src_label->setText(tr("eye Type:"));
    cap_src_label->setFixedSize(85, 30);
    cap_src_label->setToolTip("select type of the calibrated and capture eye");
    cap_src_image_box = new QComboBox();
    cap_src_image_box->setFixedSize(125, 30);
    QStringList str_list = {"Left eye", "Right eye", "Stereo eye"};
    cap_src_image_box->addItems(str_list);
    cap_src_image_box->setCurrentIndex(0);
    connect(cap_src_image_box, &QComboBox::currentTextChanged, this, &CalibrationDistortionWindow::slotUpdateEyeType);
    cap_src_info_layout->addWidget(cap_src_label, 0);
    cap_src_info_layout->addWidget(cap_src_image_box, 0);
    cap_src_info_layout->addStretch(1);

    QLabel *yuv_src_label = new QLabel;
    yuv_src_label->setText(tr("From:"));
    yuv_src_label->setFixedSize(45, 30);
    yuv_src_box = new QComboBox();
    yuv_src_box->setFixedSize(70, 30);
    QStringList src_list = {"VI", "VPSS"};
    yuv_src_box->addItems(src_list);
    yuv_src_box->setCurrentIndex(0);
    cap_src_info_layout->addWidget(yuv_src_label, 0);
    cap_src_info_layout->addWidget(yuv_src_box, 0);
    cap_src_info_layout->addStretch(1);

    QLabel *img_frames_label = new QLabel(tr("Frames:"));
    img_frames_label->setFixedSize(60, 30);
    QLabel *img_frames_tips = new QLabel("(1-50)");
    img_frames_tips->setFixedSize(50, 30);
    img_frames_edit = new QLineEdit;
    img_frames_edit->setFixedSize(60, 30);
    img_frames_edit->setText("1");
    connect(img_frames_edit, &QLineEdit::textChanged, this, std::bind(&CalibrationDistortionWindow::cellEdited, this, 0));
    cap_src_info_layout->addWidget(img_frames_label, 0);
    cap_src_info_layout->addWidget(img_frames_edit, 0);
    cap_src_info_layout->addWidget(img_frames_tips, 0);
    cap_src_info_layout->addStretch(1);

    //get image button layout
    QHBoxLayout *button_layout = new QHBoxLayout;

    capture_img_btn = new QPushButton;
    capture_img_btn->setText(tr("Capture"));
    capture_img_btn->setMaximumSize(200, 38);
    connect(capture_img_btn, &QPushButton::clicked,
         this, &CalibrationDistortionWindow::handleCaptureYuvImg);

    cancel_img_btn = new QPushButton(tr("Cancel"));
    cancel_img_btn->setMaximumSize(200, 38);
    cancel_img_btn->setEnabled(false);
    connect(cancel_img_btn, &QPushButton::clicked,
             this, &CalibrationDistortionWindow::handleCancelEvent);
    button_layout->addWidget(capture_img_btn);
    button_layout->addWidget(cancel_img_btn);

    //block layout
    img_v_layout->addLayout(cap_src_info_layout);
    img_v_layout->addLayout(button_layout);

    groupbox->setLayout(img_v_layout);

    return groupbox;
}

QGroupBox *CalibrationDistortionWindow::createBoardInfoGroupbox()
{
    QGroupBox * groupbox = new QGroupBox();
    groupbox->setTitle("Pattern Info");

   //main layout
    QVBoxLayout *board_v_layout = new QVBoxLayout;

    //Pattern Type
    QHBoxLayout *param_h0_layout = new QHBoxLayout;
    QLabel *board_type = new QLabel("Type:");
    board_type->setFixedSize(90, 30);
    board_type_box = new QComboBox;
    board_type_box->setFixedSize(135, 30);
    const QStringList board_str_list = {"chessboard", "circleboard"};
    board_type_box->addItems(board_str_list);
    board_type_box->setCurrentIndex(0);

    //Pattern interval
    QLabel *interval_label = new QLabel("Interval:");
    interval_label->setFixedSize(90, 30);
    interval_label->setToolTip("interval length of calibration plate grid. (unit: m)");
    corner_interval = new QLineEdit;
    QRegExp double_interval("10|([0-9]{0,2}[.][0-9]{1,4})");
    corner_interval->setValidator(new QRegExpValidator(double_interval,this));
    corner_interval->setFixedSize(135, 30);
    corner_interval->setText("0.0224");
    corner_interval->setEnabled(false);
    QLabel *unit_tips = new QLabel("(m)");
    unit_tips->setFixedSize(30, 30);
    param_h0_layout->addWidget(board_type);
    param_h0_layout->addWidget(board_type_box);
    param_h0_layout->addStretch(1);
    param_h0_layout->addWidget(interval_label);
    param_h0_layout->addWidget(corner_interval);
    param_h0_layout->addWidget(unit_tips);
    param_h0_layout->addStretch(1);

    //Pattern size
    QHBoxLayout *param_h1_layout = new QHBoxLayout;
    QLabel *wide_label = new QLabel("Pattern w:");
    wide_label->setFixedSize(90, 30);
    wide_label->setToolTip("grid number of calibration plate width");
    pattern_wide = new QLineEdit;
    QRegExp double_size("100|([0-9]{0,2})");
    pattern_wide->setValidator(new QRegExpValidator(double_size,this));
    pattern_wide->setFixedSize(135, 30);
    pattern_wide->setText("9");
    QLabel *high_label = new QLabel("Pattern h:");
    high_label->setFixedSize(90, 30);
    high_label->setToolTip("grid number of calibration plate height");
    pattern_high = new QLineEdit;
    pattern_high->setValidator(new QRegExpValidator(double_size,this));
    pattern_high->setFixedSize(135, 30);
    pattern_high->setText("6");
    QLabel *empty_tips = new QLabel("   ");
    empty_tips->setFixedSize(30, 30);

    param_h1_layout->addWidget(wide_label);
    param_h1_layout->addWidget(pattern_wide);
    param_h1_layout->addStretch(1);
    param_h1_layout->addWidget(high_label);
    param_h1_layout->addWidget(pattern_high);
    param_h1_layout->addWidget(empty_tips);
    param_h1_layout->addStretch(1);

    //Pattern info main layout
    board_v_layout->addLayout(param_h0_layout);
    board_v_layout->addLayout(param_h1_layout);

    groupbox->setLayout(board_v_layout);

    return groupbox;
}

QGroupBox *CalibrationDistortionWindow::createConvertPathInfoGroupbox()
{
    QGroupBox * groupbox = new QGroupBox("Convert");
    QVBoxLayout *convert_v_layout = new QVBoxLayout;

    QHBoxLayout *wh_layout = new QHBoxLayout;
    QLabel *imgW_lab = new QLabel("ImgW:");
    imgW_lab->setFixedWidth(80);
    imgW_edit = new QLineEdit;
    imgW_edit->setFixedWidth(80);
    imgW_edit->setText("0");
    QLabel *imgH_lab = new QLabel("ImgH:");
    imgH_lab->setFixedWidth(80);
    imgH_edit = new QLineEdit;
    imgH_edit->setFixedWidth(80);
    imgH_edit->setText("0");
    wh_layout->addWidget(imgW_lab, 0, Qt::AlignLeft);
    wh_layout->addWidget(imgW_edit, 0, Qt::AlignLeft);
    wh_layout->addWidget(imgH_lab, 0, Qt::AlignLeft);
    wh_layout->addWidget(imgH_edit, 0, Qt::AlignLeft);
    wh_layout->addStretch(2);

    QHBoxLayout *param_layout = new QHBoxLayout;
    const QStringList str_list = {"16x", "32x", "64x"};
    QLabel *meshH = new QLabel("meshH:");
    meshH->setFixedWidth(80);
    meshH_box = new QComboBox;
    meshH_box->setFixedWidth(80);
    meshH_box->addItems(str_list);
    meshH_box->setCurrentIndex(0);
    QLabel *meshW = new QLabel("meshW:");
    meshW->setFixedWidth(80);
    meshW_box = new QComboBox;
    meshW_box->setFixedWidth(80);
    meshW_box->addItems(str_list);
    meshW_box->setCurrentIndex(0);
    param_layout->addWidget(meshW, 0, Qt::AlignLeft);
    param_layout->addWidget(meshW_box, 0, Qt::AlignLeft);
    param_layout->addWidget(meshH, 0, Qt::AlignLeft);
    param_layout->addWidget(meshH_box, 0, Qt::AlignLeft);
    param_layout->addStretch(2);

    //root path info
    QHBoxLayout *path_layout = new QHBoxLayout;
    convert_path_btn = new QPushButton(tr("Convert Path"));
    convert_path_btn->setFixedSize(150,30);
    connect(convert_path_btn, &QPushButton::clicked, this, &CalibrationDistortionWindow::openConvertRootPath);
    shared_ptr<GlobalData> global_data = GlobalData::getInstance();
    QString exe_dir = QCoreApplication::applicationDirPath();
    convert_root_dir = global_data->getSettings(SETTINGS_SECTION, KEY_CALI_CONVERT_ROOT_PATH).toString();
    convert_path_pos = new QLineEdit();
    convert_path_pos->setReadOnly(true);
    if (!convert_root_dir.isEmpty()) {
        QDir dir(convert_root_dir);
        if(dir.exists()) {
            convert_path_pos->setText(convert_root_dir);
        } else {
            convert_path_pos->setText(exe_dir);
            global_data->setSettings(SETTINGS_SECTION, KEY_CALI_CONVERT_ROOT_PATH, exe_dir);
            global_data->saveSettings();
            convert_root_dir = exe_dir;
        }
    } else {
        convert_path_pos->setText(exe_dir);
        global_data->setSettings(SETTINGS_SECTION, KEY_CALI_CONVERT_ROOT_PATH, exe_dir);
        global_data->saveSettings();
        convert_root_dir = exe_dir;
    }
    convert_bin_btn = new QPushButton(tr("Convert mesh.bin"));
    connect(convert_bin_btn, &QPushButton::clicked,
        this, &CalibrationDistortionWindow::startMeshBinConvert);

    path_layout->addWidget(convert_path_btn);
    path_layout->addWidget(convert_path_pos);
    convert_v_layout->addLayout(wh_layout);
    convert_v_layout->addLayout(param_layout);
    convert_v_layout->addLayout(path_layout);
    convert_v_layout->addWidget(convert_bin_btn);

    groupbox->setLayout(convert_v_layout);

    return groupbox;
}

QGroupBox *CalibrationDistortionWindow::createCalibrationGroupbox()
{
    QGroupBox * groupbox = new QGroupBox("Calibration");
    QVBoxLayout *calibrate_v_layout = new QVBoxLayout;

    //root path info
    QHBoxLayout *path_layout = new QHBoxLayout;
    save_path_btn = new QPushButton(tr("Save Root Path"));
    save_path_btn->setFixedSize(150,30);
    connect(save_path_btn, &QPushButton::clicked, this, &CalibrationDistortionWindow::openSaveRootPath);
    shared_ptr<GlobalData> global_data = GlobalData::getInstance();
    QString exe_dir = QCoreApplication::applicationDirPath();
    root_dir = global_data->getSettings(SETTINGS_SECTION, KEY_CALI_SAVE_ROOT_PATH).toString();
    root_path_pos = new QLineEdit();
    root_path_pos->setReadOnly(true);
    if (!root_dir.isEmpty()) {
        QDir dir(root_dir);
        if(dir.exists()) {
            root_path_pos->setText(root_dir);
        } else {
            root_path_pos->setText(exe_dir);
            global_data->setSettings(SETTINGS_SECTION, KEY_CALI_SAVE_ROOT_PATH, exe_dir);
            global_data->saveSettings();
            root_dir = exe_dir;
        }
    } else {
        root_path_pos->setText(exe_dir);
        root_dir = exe_dir;
    }

     //start calibration button
    calibration_btn = new QPushButton(tr("Start calibration"));
    connect(calibration_btn, &QPushButton::clicked,
        this, &CalibrationDistortionWindow::calibration);

    path_layout->addWidget(save_path_btn);
    path_layout->addWidget(root_path_pos);
    calibrate_v_layout->addLayout(path_layout);
    calibrate_v_layout->addWidget(calibration_btn);

    groupbox->setLayout(calibrate_v_layout);

    return groupbox;
}

QGroupBox *CalibrationDistortionWindow::createWriteDataGroupbox()
{
    QGroupBox * groupbox = new QGroupBox();
    groupbox->setTitle("Write data");

    //main layout
    QVBoxLayout *calibration_v_layout = new QVBoxLayout;

    //write calibration data button
    write_calib_btn = new QPushButton(tr("Write calibration data"));
    write_calib_btn->hide();
    // connect(write_calib_btn, &QPushButton::clicked,
    //     this, &CaptureWindow::handleCaptureYuvImg);

    calibration_v_layout->addWidget(write_calib_btn);

    groupbox->setLayout(calibration_v_layout);

    return groupbox;
}

QGroupBox *CalibrationDistortionWindow::createCalibParamGroupbox()
{
    QGroupBox *groupbox = new QGroupBox();
    groupbox->setTitle("Image Calibrate Param");

    //get image info layout
    QVBoxLayout *calib_param_layout = new QVBoxLayout;

    QHBoxLayout *param_h0_layout = new QHBoxLayout;
    QLabel *fisheye_label = new QLabel(tr("Fisheye:"));
    fisheye_label->setFixedSize(95, 30);
    fisheye_label->setToolTip("fisheye len is true, otherwise it's false.");
    fisheye_box = new QComboBox;
    fisheye_box->setFixedSize(80, 30);
    QStringList str_list = {"False", "True"};
    fisheye_box->addItems(str_list);
    fisheye_box->setCurrentIndex(0);
    connect(fisheye_box, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CalibrationDistortionWindow::updateOpencvFlag2);
    QLabel *annotation0_label = new QLabel(tr("whether it's a fisheye lens."));
    annotation0_label->setStyleSheet("color:rgb(128,128,128)");
    param_h0_layout->addWidget(fisheye_label);
    param_h0_layout->addWidget(fisheye_box);
    param_h0_layout->addWidget(annotation0_label);
    param_h0_layout->addStretch(1);

    //Opencv_flag & flag1 set
    QHBoxLayout *param_h1_layout = new QHBoxLayout;
    flags_label = new QLabel(tr("Cv Flag:"));
    flags_label->setFixedSize(95, 30);
    flags_label->setToolTip("set openCV param as needed, the default is 0");
    opencv_flags = new QLineEdit;
    opencv_flags->setFixedSize(80, 30);
    opencv_flags->setText("0");
    opencv_flags->setValidator(new QIntValidator(0, 1000000, this));
    connect(opencv_flags, &QLineEdit::textChanged, this, &CalibrationDistortionWindow::readCvFlagValue);
    annotation_flags = new QLabel(tr("opencv control correction parameter."));
    annotation_flags->setStyleSheet("color:rgb(128,128,128)");
    param_h1_layout->addWidget(flags_label);
    param_h1_layout->addWidget(opencv_flags);
    param_h1_layout->addWidget(annotation_flags);
    param_h1_layout->addStretch(1);

    QHBoxLayout *param_h2_layout = new QHBoxLayout;
    QLabel *flag2_label = new QLabel(tr("Cv Flag2:"));
    flag2_label->setFixedSize(95, 30);
    flag2_label->setToolTip("For stereo eye only");
    opencv_flag2 = new QComboBox;
    opencv_flag2->setFixedSize(80, 30);
    QStringList str_param = {"0", "1024"};
    opencv_flag2->addItems(str_param);
    opencv_flag2->setCurrentIndex(0);
    opencv_flag2->setEnabled(false);
    QLabel *annotation_flag2 = new QLabel(tr("opencv control stereo rectify parameter."));
    annotation_flag2->setStyleSheet("color:rgb(128,128,128)");
    param_h2_layout->addWidget(flag2_label);
    param_h2_layout->addWidget(opencv_flag2);
    param_h2_layout->addWidget(annotation_flag2);
    param_h2_layout->addStretch(1);

    QHBoxLayout *param_h3_layout = new QHBoxLayout;
    QLabel *alpha_label = new QLabel(tr("Edge alpha:"));
    alpha_label->setFixedSize(95, 30);
    black_edge_alpha = new QLineEdit;
    connect(black_edge_alpha, &QLineEdit::editingFinished, this, std::bind(&CalibrationDistortionWindow::cellEdited, this, 1));
    black_edge_alpha->setFixedSize(80, 30);
    black_edge_alpha->setText("0.0");
    alpha_label->setToolTip("0: no black edge. 1: original image");
    QLabel *range_label = new QLabel(tr("(0.0-1.0)"));
    range_label->setFixedSize(80, 30);
    QLabel *annotation2_label = new QLabel(tr("whether black edge is reserved."));
    annotation2_label->setStyleSheet("color:rgb(128,128,128)");
    param_h3_layout->addWidget(alpha_label);
    param_h3_layout->addWidget(black_edge_alpha);
    param_h3_layout->addWidget(annotation2_label);
    param_h3_layout->addWidget(range_label);
    param_h3_layout->addStretch(1);

    QHBoxLayout *param_h4_layout = new QHBoxLayout;
    QLabel *homo_label = new QLabel(tr("Homo mtx:"));
    homo_label->setFixedSize(95, 30);
    homo_label->setToolTip("input homo mtx param please!");
    homo_mtx_value = new QLineEdit;
    homo_mtx_value->setFixedSize(450, 30);
    homo_mtx_value->setText("0 0 1280 720 1280 720 32 32 1280 720 0 0");
    param_h4_layout->addWidget(homo_label);
    param_h4_layout->addWidget(homo_mtx_value);
    param_h4_layout->addStretch(1);

    calib_param_layout->addLayout(param_h0_layout);
    calib_param_layout->addLayout(param_h1_layout);
    calib_param_layout->addLayout(param_h2_layout);
    calib_param_layout->addLayout(param_h3_layout);
    calib_param_layout->addLayout(param_h4_layout);

    groupbox->setLayout(calib_param_layout);

    return groupbox;
}

QGroupBox *CalibrationDistortionWindow::createImageListGroupbox()
{
    QGroupBox * groupbox = new QGroupBox("Image List");
    QVBoxLayout *group_layout = new QVBoxLayout;

    left_list_widget = new JpgImageListWidget;
    connect(left_list_widget, &JpgImageListWidget::imageListChanged, this, [=](SIGNAL_TYPE type){
        handleImgListUpdate(type, 0);
    });
    right_list_widget = new JpgImageListWidget;
    connect(right_list_widget, &JpgImageListWidget::imageListChanged, this, [=](SIGNAL_TYPE type){
        handleImgListUpdate(type, 1);
    });

    group_layout->addWidget(left_list_widget);
    group_layout->addWidget(right_list_widget);
    groupbox->setLayout(group_layout);

    return groupbox;
}

void CalibrationDistortionWindow::handleImgListUpdate(SIGNAL_TYPE type, int id)
{
    if (id == 0) {
        image_list_left.clear();
    } else {
        image_list_right.clear();
    }

    if (type != DEL_ALL) {
        QList<JpgImageListItem *> item_list;
        if (id == 0) {
            item_list = left_list_widget->getJpgImageList();
        } else {
            item_list = right_list_widget->getJpgImageList();
        }

        QList<QImage> list;
        for (int i = 0; i < item_list.size(); i++) {
            QString path = item_list[i]->getFilePath();
            QImageReader reader(path);
            QImage img = reader.read();
            list.append(img);
        }

        if (id == 0) {
            image_list_left = list;
        } else {
            image_list_right = list;
        }
    }

    input_img->clearImageData();
    output_img->clearImageData();
    QList<QImage> list_gray;
    list_gray.append(image_list_left);
    list_gray.append(image_list_right);
    center_tab_widget->setCurrentIndex(0);
    input_img->setJPGImgData(utils::RGB2Grayscale(list_gray));
}

void CalibrationDistortionWindow::updateCvFlagUI()
{
    if (image_src_index == 2) {
        flags_label->setText("Cv Flag1:");
        flags_label->setToolTip("set openCV param as needed, the default is 1");
        opencv_flags->setText(QString::number(last_stereo_flag));
        annotation_flags->setText(tr("opencv control stereo calibrate parameter"));
    } else {
        flags_label->setText("Cv Flag:");
        flags_label->setToolTip("set openCV param as needed, the default is 0");
        opencv_flags->setText(QString::number(last_mono_flag));
        annotation_flags->setText(tr("opencv control mono calibrate parameter."));
    }
}

void CalibrationDistortionWindow::readCvFlagValue()
{
    if (image_src_index == 2) {
        last_stereo_flag = opencv_flags->text().toInt();
    } else {
        last_mono_flag = opencv_flags->text().toInt();
    }
}

void CalibrationDistortionWindow::updateOpencvFlag2(int index)
{
    if (index == 0) {
        opencv_flag2->setCurrentIndex(0);
    } else {
        opencv_flag2->setCurrentIndex(1);
    }
}

void CalibrationDistortionWindow::startMeshBinConvert()
{
    QString srcPath = convert_path_pos->text();
    QStringList type_list = {"*.bmp"};
    QStringList bmp_file_list = utils::getDirSpecifyTypeFileNameList(srcPath, type_list);
    int w = 0, h = 0;
    bool ctl = false;
    cv::Mat grayimg;

    if(bmp_file_list.isEmpty()) {
        w = imgW_edit->text().toInt();
        h = imgH_edit->text().toInt();
    } else {
        QString bmp_path = srcPath + "/" + bmp_file_list[0];
        grayimg = cv::imread(bmp_path.toStdString(), 0);
        if (!grayimg.empty()) {
            w = grayimg.cols;
            h = grayimg.rows;
            ctl = true;
        }
    }

    if (w <= 0 || h <= 0) {
        QMessageBox::warning(this, tr("warning"), tr("Please set correct ImgW and ImgH, or put correct image in the directory!"), QMessageBox::Ok);
        return;
    }

    uint ttl = w * h;
    QString path_mapX = srcPath + "/" + "mapX.bin";
    QFileInfo infoX(path_mapX);
    uint sizeX = infoX.size();
    if ((sizeX >> 2) != ttl) {
        QMessageBox::warning(this, tr("warning"), tr("The input size doesn't match mapX.bin, plz check!"), QMessageBox::Ok);
        return;
    }
    QString path_mapY = srcPath + "/" + "mapY.bin";
    QFileInfo infoY(path_mapY);
    uint sizeY = infoY.size();
    if ((sizeY >> 2) != ttl) {
        QMessageBox::warning(this, tr("warning"), tr("The input size doesn't match mapY.bin, plz check!"), QMessageBox::Ok);
        return;
    }

    float *pmapx = new float[ttl];
    float *pmapy = new float[ttl];
    qDebug("Read file:%s\n", qUtf8Printable(path_mapX));
    FILE *fp = fopen(path_mapX.toStdString().c_str(), "rb");
    if (fp == NULL) {
        delete[]pmapx;
        delete[]pmapy;
        QString log = QString("Read file:%1 fail!\n").arg(qUtf8Printable( path_mapX));
        QMessageBox::warning(this, tr("Error"), log, QMessageBox::Ok);
        return;
    }
    fread(pmapx, sizeof(float), ttl, fp);
    fclose(fp);

    qDebug("Read file:%s\n", qUtf8Printable(path_mapY));
    FILE *fp1 = fopen(path_mapY.toStdString().c_str(), "rb");
    if (fp1 == NULL) {
        delete[]pmapx;
        delete[]pmapy;
        QString log = QString("Read file:%1 fail!\n").arg(qUtf8Printable( path_mapY));
        QMessageBox::warning(this, tr("Error"), log, QMessageBox::Ok);
        return;
    }
    fread(pmapy, sizeof(float), ttl, fp1);
    fclose(fp1);

    disableComponents();
    QCoreApplication::processEvents(QEventLoop::AllEvents, 10);

    ldcdata stldcdata;
    setzero_ldcdata(&stldcdata);
    stldcdata.alignnbr = 64; // fixed parameter, do not change
    stldcdata.meshw = 16 * (1 << meshW_box->currentIndex()); // changable number, 16x: 16/32/64
    stldcdata.meshh = 16 * (1 << meshH_box->currentIndex()); // changable number, 16x: 16/32/64
    stldcdata.binit = true;
    int bytes = 0;
    if (ctl) {
        bytes = align_copyto_buf(grayimg.data, pmapx, pmapy, w, h, &stldcdata, NULL, 0, 0, 0);
    } else {
        bytes = align_copyto_buf(NULL, pmapx, pmapy, w, h, &stldcdata, NULL, 0, 0, 0);
    }
    qDebug("\nCalculate memory cost, %d KB\n", bytes);
    delete[]pmapx;
    delete[]pmapy;
    qDebug("Generate mesh.bin\n");
    QString szPath = srcPath + "/";
    do_ldc_training_buf(&stldcdata, (char *)szPath.toStdString().c_str());

    // save mesh data for check
    szPath = srcPath + "/" + "mesh_MW.bin";
    FILE *fp2x_bin = fopen(szPath.toStdString().c_str(), "wb");
    fwrite(stldcdata.pmeshbin, 1, (stldcdata.mesh_1st_size + stldcdata.mesh_2nd_size), fp2x_bin);
    fclose(fp2x_bin);

    szPath = srcPath + "/" + "srcx_1st_mesh_MW.bin";
    fp2x_bin = fopen(szPath.toStdString().c_str(), "wb");
    fwrite(stldcdata.pmeshbin, 1, stldcdata.mesh_1st_size, fp2x_bin);
    fclose(fp2x_bin);

    szPath = srcPath + "/" + "srcx_2nd_mesh_MW.bin";
    fp2x_bin = fopen(szPath.toStdString().c_str(), "wb");
    fwrite(stldcdata.pmeshbin + stldcdata.mesh_1st_size, 1, stldcdata.mesh_2nd_size, fp2x_bin);
    fclose(fp2x_bin);
    free_ldcdata(&stldcdata);

    QMessageBox::information(this, tr("Info"), tr("Convert mesh bin file successfully!"), QMessageBox::Ok);
    enableComponents();
}

void CalibrationDistortionWindow::handleCaptureYuvImg()
{
    qDebug("CalibrationDistortionWindow::handleCaptureYuvImg\n");

    if (img_frames_edit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("Please Input Frame Numbers!"), QMessageBox::Ok);
        return;
    }

    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        return;
    }

    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);

    disableComponents();
    cancel_img_btn->setEnabled(true);
    if (center_tab_widget->currentIndex() != 0) {
        center_tab_widget->setCurrentWidget(input_img);
    }

    start_capture = true;
    current_frame = 0;
    recvSize = 0;
    receive_yuv_data.clear();
    total_frame = img_frames_edit->text().toInt();
    image_src_index = cap_src_image_box->currentIndex();

    QString from = yuv_src_box->currentText();
    if (from == "VI") {
        capture_from = 0;
    } else if (from == "VPSS") {
        capture_from = 1;
    }

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_CALIBRATION_GET_MULTI_YUV;
    int id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

    if (image_src_index == 2) {
        cur_src_index = 0;
    } else {
        cur_src_index = image_src_index;
    }
    QJsonObject param_obj;
    param_obj["from"] = capture_from;
    param_obj["frames"] = 1;
    param_obj["tightly"] = false;
    param_obj["getRawReplayYuv"] = false;
    param_obj["getRawReplayYuvId"] = 0;
    param_obj["imageSrcIndex"] = cur_src_index;

    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_MULTI_YUV;
    send_obj[MSG_KEY_ID] = id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_PARAMS] = param_obj;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void CalibrationDistortionWindow::handleMultiYuvStatus(int id)
{
    qDebug("CalibrationDistortionWindow::handleMultiYuvStatus\n");

    QJsonObject obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    if(!start_capture) {
        return;
    }

    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();
    if (result_obj.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("Capture yuv failed!"), QMessageBox::Ok);
        handleCancelEvent();
        return;
    }

    QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();
    recvSize = params_obj["Data size"].toInt();
    NetworkClientHelper::getInstance()->setCallerInfo(recvSize, CMD_CALIBRATION_GET_MULTI_YUV);
    qDebug("Set receive size:%d\n", recvSize);

    int temp_id = GlobalData::getInstance()->getJsonrpcId();
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_IMAGE_DATA;
    send_obj[MSG_KEY_ID] = temp_id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void CalibrationDistortionWindow::slotGotYuvImg(QString cmd)
{
    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);
    QByteArray data = NetworkClientHelper::getInstance()->popCmdData(cmd);
    if(!start_capture) {
        return;
    }

    QByteArray header = data.left(sizeof(YUV_HEADER));
    YUV_HEADER * pstHeader = (YUV_HEADER *) header.data();
    data.remove(0, sizeof(YUV_HEADER));

    qDebug("numframe:%d, curframe:%d, width:%d, height:%d, stride:%d,%d,%d, format::%d\n", pstHeader->numFrame, pstHeader->curFrame,
        pstHeader->width, pstHeader->height, pstHeader->stride[0], pstHeader->stride[1], pstHeader->stride[2], pstHeader->pixelFormat);
    qDebug("data.size:%d, pstHeader->size:%d\n", data.size(), pstHeader->size);

    if (data.size() != (int)pstHeader->size) {
        QMessageBox::warning(this, tr("Error"), tr("Capture Tool, The received yuv data size is wrong!"), QMessageBox::Ok);
        return;
    }

    receive_yuv_data.append(data);

    int frames;
    if (image_src_index != 2) {
        frames = total_frame - 1;
    } else {
        frames = total_frame;
    }

    if (current_frame < frames) {
        if (image_src_index == 2) {
            if (cur_src_index == 0) {
                cur_src_index = 1;
                if (current_frame == frames - 1) {
                    current_frame++;
                }
            } else {
                cur_src_index = 0;
                current_frame++;
            }
        } else {
            current_frame++;
        }

        QJsonObject id_info;
        id_info[MSG_KEY_CMD] = CMD_CALIBRATION_GET_MULTI_YUV;
        int id = GlobalData::getInstance()->getJsonrpcId();
        NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

        QJsonObject param_obj;
        param_obj["from"] = capture_from;
        param_obj["frames"] = 1;
        param_obj["tightly"] = false;
        param_obj["getRawReplayYuv"] = false;
        param_obj["getRawReplayYuvId"] = 0;
        param_obj["imageSrcIndex"] = cur_src_index;
        QJsonObject send_obj;
        send_obj[MSG_KEY_METHOD] = METHOD_GET_MULTI_YUV;
        send_obj[MSG_KEY_ID] = id;
        send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
        send_obj[MSG_KEY_PARAMS] = param_obj;
        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
    } else {
        // save file and set for preview
        QList<QImage> list;
        int received_frames = receive_yuv_data.size() / (pstHeader->size);
        for (int i=0; i<received_frames; i++) {
            QByteArray frame = receive_yuv_data.mid(i * pstHeader->size, pstHeader->size);
            list.append(utils::convertYUV2RGBImage(frame, pstHeader));
        }
        saveImgFile(list, image_src_index);

        current_frame = 0;
        recvSize = 0;
        start_capture = false;
        receive_yuv_data.clear();
        enableComponents();
        cancel_img_btn->setEnabled(false);
        QMessageBox message(QMessageBox::NoIcon, "Notice", QString("Capture %1 frame image successfully!").arg(QString::number(total_frame)));
        message.exec();
    }
}

void CalibrationDistortionWindow::saveImgFile(QList<QImage> list, int type, QString path)
{
    QString dateTime_str = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");

    QString jpg_path;
    QImage image;
    if (path.isEmpty()) {
        QList<JpgImageListItem *> item_list_l, item_list_r;
        JpgImageListItem *item;
        for (int i = 0; i < list.size(); i++) {
            if (type == 0) {
                jpg_path = getSavePath(0, 0) + "/left_idx=" + QString::number(i) + "_" + dateTime_str + ".jpg";
                item = new JpgImageListItem(jpg_path);
                item_list_l.append(item);
            } else if (type == 1) {
                jpg_path = getSavePath(0, 1) + "/right_idx=" + QString::number(i) + "_" + dateTime_str + ".jpg";
                item = new JpgImageListItem(jpg_path);
                item_list_r.append(item);
            } else {
                if (i%2==0) {
                    jpg_path = getSavePath(0, 2, "/left") + "/left_idx=" + QString::number(i/2) + "_" + dateTime_str + ".jpg";
                    item = new JpgImageListItem(jpg_path);
                    item_list_l.append(item);
                } else {
                    jpg_path = getSavePath(0, 2, "/right") + "/right_idx=" + QString::number(i/2) + "_" + dateTime_str + ".jpg";
                    item = new JpgImageListItem(jpg_path);
                    item_list_r.append(item);
                }
            }
            image = list.at(i);
            image.save(jpg_path, "JPG");
        }
        left_list_widget->addTableItems(item_list_l);
        right_list_widget->addTableItems(item_list_r);
    } else {
        QDir dir(path);
        if (!dir.exists()) {
            return;
        }

        QStringList files = dir.entryList(QDir::Files);
        for (int i = 0; i < files.size(); i++)
        {
            QString filePath = dir.filePath(files[i]);
            QFile::remove(filePath);
        }
        for (int i = 0; i < list.size(); i++) {
            jpg_path = path + "/undistort_idx=" + QString::number(i) + "_" + dateTime_str + ".jpg";
            image = list.at(i);
            image.save(jpg_path, "JPG");
        }
    }
}

void CalibrationDistortionWindow::cellEdited(int idx)
{
    int value = 0;
    int max = 50;

    if (idx == 0) {
        if (img_frames_edit->text().isEmpty()) {
            return;
        }
        value = img_frames_edit->text().toInt();
        img_frames_edit->setText(QString::number(utils::validateValue(value, 1, max)));
    } else if (idx == 1) {
        if (black_edge_alpha->text().isEmpty()) {
            return;
        }
        double max = 1.0;
        double value = black_edge_alpha->text().toDouble();
        black_edge_alpha->setText(QString::number(utils::validateDoubleValue(value, 0.0, max), 'f', 1));
    }
}

void CalibrationDistortionWindow::handleCancelEvent()
{
    stopCapture();
    enableComponents();
    cancel_img_btn->setEnabled(false);
    start_capture = false;
    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);
}

void CalibrationDistortionWindow::enableComponents()
{
    output_img->setEnabled(true);
    input_img->setEnabled(true);
    cap_src_image_box->setEnabled(true);
    img_frames_edit->setEnabled(true);
    capture_img_btn->setEnabled(true);
    board_type_box->setEnabled(true);
    pattern_wide->setEnabled(true);
    pattern_high->setEnabled(true);
    opencv_flags->setEnabled(true);
    calibration_btn->setEnabled(true);
    convert_bin_btn->setEnabled(true);
    // write_calib_btn->setEnabled(true);
    save_path_btn->setEnabled(true);
    root_path_pos->setEnabled(true);
    convert_path_pos->setEnabled(true);
    convert_path_btn->setEnabled(true);
    black_edge_alpha->setEnabled(true);
    fisheye_box->setEnabled(true);
    yuv_src_box->setEnabled(true);
    left_list_widget->setEnabled(true);
    right_list_widget->setEnabled(true);
    meshH_box->setEnabled(true);
    meshW_box->setEnabled(true);
    imgW_edit->setEnabled(true);
    imgH_edit->setEnabled(true);

    if (image_src_index == 2) {
        corner_interval->setEnabled(true);
    }
}

void CalibrationDistortionWindow::disableComponents()
{
    output_img->setEnabled(false);
    input_img->setEnabled(false);
    cap_src_image_box->setEnabled(false);
    img_frames_edit->setEnabled(false);
    capture_img_btn->setEnabled(false);
    board_type_box->setEnabled(false);
    pattern_wide->setEnabled(false);
    pattern_high->setEnabled(false);
    corner_interval->setEnabled(false);
    opencv_flags->setEnabled(false);
    opencv_flag2->setEnabled(false);
    calibration_btn->setEnabled(false);
    convert_bin_btn->setEnabled(false);
    // write_calib_btn->setEnabled(false);
    save_path_btn->setEnabled(false);
    root_path_pos->setEnabled(false);
    convert_path_pos->setEnabled(false);
    convert_path_btn->setEnabled(false);
    fisheye_box->setEnabled(false);
    black_edge_alpha->setEnabled(false);
    yuv_src_box->setEnabled(false);
    left_list_widget->setEnabled(false);
    right_list_widget->setEnabled(false);
    meshH_box->setEnabled(false);
    meshW_box->setEnabled(false);
    imgW_edit->setEnabled(false);
    imgH_edit->setEnabled(false);
}

void CalibrationDistortionWindow::stopCapture()
{
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_STOP_GET_IMAGE_DATA;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void CalibrationDistortionWindow::slotUpdateEyeType()
{
    image_src_index = cap_src_image_box->currentIndex();
    if (last_img_src_index != image_src_index) {
        if (image_src_index == 2) {
            corner_interval->setEnabled(true);
            opencv_flag2->setEnabled(true);
        } else {
            corner_interval->setEnabled(false);
            opencv_flag2->setEnabled(false);
        }
        updateCvFlagUI();
        last_img_src_index = image_src_index;
        left_list_widget->del_all();
        right_list_widget->del_all();
    }
}

void CalibrationDistortionWindow::openSaveRootPath()
{
    shared_ptr<GlobalData> global_data = GlobalData::getInstance();
    QString dir_path;

    if (!root_dir.isEmpty()) {
        QDir dir(root_dir);
        if(dir.exists()) {
            dir_path = QFileDialog::getExistingDirectory(this, "Please Select Save Root Path", root_dir);
        } else {
            dir_path = QFileDialog::getExistingDirectory(this, "Please Select Save Root Path", "./");
        }
    } else {
        dir_path = QFileDialog::getExistingDirectory(this, "Please Select Save Path", "./");
    }

    qDebug("Opened Save Path: %s", qUtf8Printable(dir_path));
    if (!dir_path.isEmpty()) {
        root_path_pos->setText(dir_path);
        global_data->setSettings(SETTINGS_SECTION, KEY_CALI_SAVE_ROOT_PATH, dir_path);
        global_data->saveSettings();
        root_dir = dir_path;
    }
}

void CalibrationDistortionWindow::openConvertRootPath()
{
    shared_ptr<GlobalData> global_data = GlobalData::getInstance();
    QString dir_path;

    if (!convert_root_dir.isEmpty()) {
        QDir dir(convert_root_dir);
        if(dir.exists()) {
            dir_path = QFileDialog::getExistingDirectory(this, "Please Select Convert Root Path", convert_root_dir);
        } else {
            dir_path = QFileDialog::getExistingDirectory(this, "Please Select Convert Root Path", "./");
        }
    } else {
        dir_path = QFileDialog::getExistingDirectory(this, "Please Select Convert Path", "./");
    }

    qDebug("Opened Convert Path: %s", qUtf8Printable(dir_path));
    if (!dir_path.isEmpty()) {
        convert_path_pos->setText(dir_path);
        global_data->setSettings(SETTINGS_SECTION, KEY_CALI_CONVERT_ROOT_PATH, dir_path);
        global_data->saveSettings();
        convert_root_dir = dir_path;
    }
}

QString CalibrationDistortionWindow::getSavePath(int src, int type, QString name)
{
    if (src == 0) {
        if (image_dir.isEmpty()) {
            QString dateTime_str = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
            image_dir = root_dir + "/distortion_" + dateTime_str;
            QDir().mkdir(image_dir);
        }
    } else {
        if (result_dir.isEmpty()) {
            result_dir = root_dir + "/calib_pq";
            QDir().mkdir(result_dir);
        }
    }


    QString dir_path;
    if (src == 0) {
        dir_path = image_dir;
    } else {
        dir_path = result_dir;
    }
    if (type == 0) {
        dir_path += "/left";
        QDir dir(dir_path);
        if (!dir.exists()) {
            QDir().mkdir(dir_path);
        }
    } else if (type == 1) {
        dir_path += "/right";
        QDir dir(dir_path);
        if (!dir.exists()) {
            QDir().mkdir(dir_path);
        }
    } else {
        dir_path += "/stereo";
        QDir dir(dir_path);
        if (!dir.exists()) {
            QDir().mkdir(dir_path);
        }
    }
    dir_path += name;

    QDir dir(dir_path);
    if (!dir.exists()) {
        QDir().mkdir(dir_path);
    }

    return dir_path;
}

QString CalibrationDistortionWindow::getErrorCodeDescribe(int id_code)
{
    switch (id_code) {
        case 11001:
        case 12001:
            return "No input calibration images";

        case 11002:
        case 12002:
            return "Type of the rectify board | \'chessboard\" or \"circleboard\'";

        case 11003:
        case 12003:
            return "There is no valid calibration image";

        case 11004:
        case 12004:
            return "Param yaml file can not open";

        case 11005:
        case 12005:
            return "Map bin file can not open";

        case 21001:
        case 22001:
            return "Opencv findChessboardCorners error";

        case 21002:
        case 22002:
            return "Opencv findCirclesGrid error";

        case 21003:
        case 22004:
            return "Opencv calibrateCamera error";

        case 21004:
        case 22006:
            return "Opencv remap error";

        case 22003:
            return "Opencv stereoCalibrate error";

        case 22005:
            return "Opencv initUndistortRectifyMap error";

        default:
            return "Undefined description error code";
    }
}

QList<QImage> CalibrationDistortionWindow::monoCalibration(cv::Mat *images, double *total, QList<double> *rms, cv::Size size, int num)
{
    string boardType = board_type_box->currentText().toStdString();
    cv::Size patternSize(pattern_wide->text().toInt(), pattern_high->text().toInt());
    double alpha = black_edge_alpha->text().toDouble();
    int flags = opencv_flags->text().toInt();
    IMonoRectify *monoRectify = GetMonoRectify();
    bool isFisheye = fisheye_box->currentIndex() == 0 ? false : true;
    int ret = 0;
    QList<QImage> image_result_list;
    //vector<double> rms;

    QString yamlPathName = getSavePath(1, image_src_index) + "/cameraParams.yaml";
    QString binPathName = getSavePath(1, image_src_index, "/gridinfo");

    string yamlParamPathName = yamlPathName.toStdString();
    string binParamPathName = binPathName.toStdString();

    //run calibration & save calibration result
    qDebug() << "pattern_wide:" << pattern_wide->text().toInt() << endl;
    qDebug() << "pattern_high:" << pattern_high->text().toInt() << endl;
    qDebug() << "Mono calibrating, please wait......" << endl;
    ret = monoRectify->monoCalib(images, num, "jpg", boardType.c_str(), patternSize, alpha, isFisheye, flags, false);
    if (ret != 0) {
        QMessageBox::warning(this, tr("Error"), QString("Calibration failure, error code: %1\n\ndescribe: %2!").arg(QString::number(ret)).arg(getErrorCodeDescribe(ret)), QMessageBox::Ok);
        return image_result_list;
    }

    qDebug() << "Save mono camera pamram to file......" << endl;
    ret = monoRectify->monoSaveCameraParams(yamlParamPathName.c_str(), binParamPathName.c_str());
    if (ret != 0) {
        QMessageBox::warning(this, tr("Error"), QString("Save param fail, error code: %1\n\ndescribe: %2!").arg(QString::number(ret)).arg(getErrorCodeDescribe(ret)), QMessageBox::Ok);
        return image_result_list;
    }
    //save makegridinfo result
    float homo_mtx[32] = {0};
    utils::stringToInt(homo_mtx_value->text(), homo_mtx);

    generate_meshInfo(binParamPathName.c_str(), homo_mtx);

    //rectify show
    ret = monoRectify->monoReadCameraParams(yamlParamPathName.c_str());
    if (ret != 0) {
        QMessageBox::warning(this, tr("Error"), QString("Read camera params failure, error code: %1\n\ndescribe: %2!").arg(QString::number(ret)).arg(getErrorCodeDescribe(ret)), QMessageBox::Ok);
        return image_result_list;
    }
    for (int i = 0; i < num; i++) {
        cv::Mat monoRectImage(size, CV_8UC1);
        QImage result_img;

        monoRectify->monoUndistort(images[i], monoRectImage);
        result_img = utils::cvMat2QImage(monoRectImage);
        image_result_list.append(result_img);
    }

    cv::FileStorage fs(yamlParamPathName, cv::FileStorage::READ);
    if (fs.isOpened()) {
        for (int i = 0; i < num; i++) {
            QString str = "Error of " + QString("%1").arg(i) + " th calibImage";
            string title = str.toStdString();
            string value = fs[title];
            if (value.empty()) {
                rms->append(0);
            } else {
                rms->append(stod(value));
            }
        }
        *total = fs["RMS"];
    }
    fs.release();

    return image_result_list;
}

QList<QImage> CalibrationDistortionWindow::stereoCalibration(cv::Mat *imagesLeft, cv::Mat *imagesRight, double *total, QList<double> *rms, cv::Size size, int num)
{
    string boardType = board_type_box->currentText().toStdString();
    cv::Size patternSize(pattern_wide->text().toInt(), pattern_high->text().toInt());
    double alpha = black_edge_alpha->text().toDouble();
    int flag1 = opencv_flags->text().toInt();
    int flag2 = opencv_flag2->currentIndex() == 0 ? 0 : 1024;
    IStereoRectify *stereoRectify = GetStereoRectify();
    float cornerStep = corner_interval->text().toDouble();
    bool isFisheye = fisheye_box->currentIndex() == 0 ? false : true;
    QList<QImage> image_result_list;
    int ret = 0;

    QString leftParamFile = getSavePath(1, 0) + "/cameraParams.yaml";
    QString rightParamFile = getSavePath(1, 1) + "/cameraParams.yaml";
    QString stereoYamlPathName = getSavePath(1, 2) + "/cameraParams.yaml";
    QString stereoLeftBinPathName = getSavePath(1, 2, "/left_gridinfo");
    QString stereoRightBinPathName = getSavePath(1, 2, "/right_gridinfo");
    string yamlParamPathName = stereoYamlPathName.toStdString();
    string leftBinFileDir = stereoLeftBinPathName.toStdString();
    string rightBinFileDir = stereoRightBinPathName.toStdString();
    cv::Mat leftCameraMatrix, rightCameraMatrix, leftDistCoeffs, rightDistCoeffs;

    // Read left & right camera params
    qDebug() << "Reading left camera params" << endl;
    cv::FileStorage fsl(leftParamFile.toStdString(), cv::FileStorage::READ);
    if (!fsl.isOpened()) {
        QMessageBox::warning(this, tr("Warning"), tr("Left eye yaml file can't open!"), QMessageBox::Ok);
        return image_result_list;
    }
    fsl["cameraMatrix"] >> leftCameraMatrix;
    fsl["distCoeffs"] >> leftDistCoeffs;
    fsl.release();

    qDebug() << "Reading right camera params" << endl;
    cv::FileStorage fsr(rightParamFile.toStdString(), cv::FileStorage::READ);
    if (!fsr.isOpened()) {
        QMessageBox::warning(this, tr("Warning"), tr("Right eye yaml file can't open!"), QMessageBox::Ok);
        return image_result_list;
    }
    fsr["cameraMatrix"] >> rightCameraMatrix;
    fsr["distCoeffs"] >> rightDistCoeffs;
    fsr.release();

    //run calibration & save calibration result
    qDebug() << "Stereo calibrating, please wait......" << endl;
    ret = stereoRectify->stereoCalib(imagesLeft, imagesRight, num, "jpg", boardType.c_str(), patternSize,
                    cornerStep, leftCameraMatrix, leftDistCoeffs,
                    rightCameraMatrix, rightDistCoeffs, alpha, flag1, flag2, isFisheye);
    if (ret != 0) {
        QMessageBox::warning(this, tr("Error"), QString("Calibration failure,error code: %1.\n\ndescribe: %2!").arg(QString::number(ret)).arg(getErrorCodeDescribe(ret)), QMessageBox::Ok);
        return image_result_list;
    }

    qDebug() << "Save stereo camera pamram to file......" << endl;
    ret = stereoRectify->stereoSaveCameraParams(yamlParamPathName.c_str(), leftBinFileDir.c_str(), rightBinFileDir.c_str());
    if (ret != 0) {
        QMessageBox::warning(this, tr("Error"), QString("Save param fail, error code: %1.\n\ndescribe: %2!").arg(QString::number(ret)).arg(getErrorCodeDescribe(ret)), QMessageBox::Ok);
        return image_result_list;
    }

    //save makegridinfo result
    float homo_mtx[32] = {0};
    utils::stringToInt(homo_mtx_value->text(), homo_mtx);

    generate_meshInfo(leftBinFileDir.c_str(), homo_mtx);
    generate_meshInfo(rightBinFileDir.c_str(), homo_mtx);

    //rectify show
    ret = stereoRectify->stereoReadCameraParams(yamlParamPathName.c_str());
    if (ret != 0) {
        QMessageBox::warning(this, tr("Error"), QString("Read camera params failure, error code: %1\n\ndescribe: %2!").arg(QString::number(ret)).arg(getErrorCodeDescribe(ret)), QMessageBox::Ok);
        return image_result_list;
    }

    for (int i = 0; i < num; i++) {
        cv::Mat frame_left = imagesLeft[i];
        cv::Mat frame_right = imagesRight[i];
        QImage result_img;
        cv::Mat leftEpipImages(size, CV_8UC1), rightEpipImages(size, CV_8UC1);
        stereoRectify->stereoUndistort(frame_left, frame_right, leftEpipImages, rightEpipImages);
        result_img = utils::cvMat2QImage(leftEpipImages);
        image_result_list.insert(i/2, result_img);
        result_img = utils::cvMat2QImage(rightEpipImages);
        image_result_list.append(result_img);
    }

    cv::FileStorage fs(yamlParamPathName, cv::FileStorage::READ);
    if (fs.isOpened()) {
        for (int i = 0; i < num; i++) {
            QString str = "Error of " + QString("%1").arg(i) + " th pair calibImages";
            string title = str.toStdString();
            string value = fs[title];
            QStringList tmp = QString::fromStdString(value).split(" ");

            rms->append(tmp[0].toDouble());
            rms->append(tmp[1].toDouble());
        }
        *total = fs["RMS"];
    }
    fs.release();

    return image_result_list;
}

bool CalibrationDistortionWindow::calibration()
{
    int type = cap_src_image_box->currentIndex();
    if (type < 0 || type > 2) {
        QString log = "eye type err :" + QString::number(type) + " !";
        QMessageBox::warning(this, "Warning", log, QMessageBox::Ok);
        return false;
    }

    int image_num = 0;
    if (type == 0) {
        image_num = image_list_left.size();
    } else if (type == 1) {
        image_num = image_list_right.size();
    } else {
        image_num = image_list_left.size();
        if (image_num != image_list_right.size()) {
            QMessageBox::warning(this, tr("Warning"), tr("Stereo eye image data error!"), QMessageBox::Ok);
            return false;
        }
    }
    if (image_num == 0) {
        QMessageBox::warning(this, tr("Warning"), tr("image data is empty!"), QMessageBox::Ok);
        return false;
    }

    disableComponents();
    cancel_img_btn->setEnabled(false);
    if (center_tab_widget->currentIndex() != 0) {
        center_tab_widget->setCurrentWidget(input_img);
    }
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

    QList<QImage> list, list1;
    if (type == 0) {
        list = utils::RGB2Grayscale(image_list_left);
    } else if (type == 1) {
        list = utils::RGB2Grayscale(image_list_right);
    } else {
        list = utils::RGB2Grayscale(image_list_left);
        list1 = utils::RGB2Grayscale(image_list_right);
    }

    cv::Size size(list[0].width(), list[0].height());
    cv::Mat imagesLeft[image_num], imagesRight[image_num];
    for (int i = 0; i < image_num; i++) {
        if (type == 2) {
            QImage image = list.at(i);
            cv::Mat frame = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
            frame.copyTo(imagesLeft[i]);
            image = list1.at(i);
            frame = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
            frame.copyTo(imagesRight[i]);
        } else {
            QImage image = list.at(i);
            cv::Mat frame = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
            frame.copyTo(imagesLeft[i]);
        }
    }

    QList<QImage> list_result;
    QList<double> list_rms;
    double rmsTotal = 0;
    if (type == 2) {
        list_result = stereoCalibration(imagesLeft, imagesRight, &rmsTotal, &list_rms, size, image_num);
    } else {
        list_result = monoCalibration(imagesLeft, &rmsTotal, &list_rms, size, image_num);
    }

    if (!list_result.isEmpty()) {
        output_img->setJPGImgData(list_result);
        center_tab_widget->setCurrentWidget(output_img);
        QString path = getSavePath(1, type, "/undistort");
        saveImgFile(list_result, type, path);
    }

    if (!list_rms.isEmpty()) {
        updateRMS(rmsTotal, &list_rms, type);
    }

    enableComponents();

    return true;
}

void CalibrationDistortionWindow::updateRMS(double total, QList<double> *rms, int type)
{
    if (type == 0) {
        left_list_widget->updateRMS(total, rms);
    } else if (type == 1) {
        right_list_widget->updateRMS(total, rms);
    } else {
        QList<double> list_left, list_right;
        for (int i = 0; i < rms->size(); i += 2) {
            list_left.append(rms->at(i));
            list_right.append(rms->at(i+1));
        }
        left_list_widget->updateRMS(total, &list_left);
        right_list_widget->updateRMS(total, &list_right);
    }
}
