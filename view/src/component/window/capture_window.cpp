#include "capture_window.hpp"
#include "raw_image_format_dialog.hpp"
#include "utils.hpp"
#include "value_validator.hpp"
#include "global_data.hpp"
#include "main_window.hpp"
#include <windows.h>

#include <QLabel>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QMessageBox>
#include <QShortcut>
#include <QPainter>

const int MIN_WINDOW_WIDTH = 1000;
const int MIN_WINDOW_HEIGHT = 800;
const int LINEAR_RAW = 0;
const int WDR_RAW = 1;
const int WDR_LE_RAW = 2;
const int WDR_SE_RAW = 3;

#define RAW_MODE_LINEAR (0)
#define RAW_MODE_WDR (1)
#define RAW_MODE_AUTO (2)

#define STD_IMAGE_WIDTH (1920)
#define STD_IMAGE_HEIGHT (1080)

#define RAW_LINE_BYTE (3840)

CaptureWindow::CaptureWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle(tr("Capture Tool"));

    initMenu();
    initUI();
    initEvent();
    handleShortCut();
}

void CaptureWindow::initMenu()
{
}

void CaptureWindow::handleShortCut()
{
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Y), this, SLOT(handleCaptureYuvImg()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_R), this, SLOT(handleCaptureRawImg()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_D), this, SLOT(handleDumpAll()));
}

void CaptureWindow::initUI()
{
    main_widget = new QWidget();
    //main_widget->setStyleSheet("background-color: #ffffff;");
    main_widget->setMinimumSize(QSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT));
    main_layout = new QHBoxLayout();
    QWidget *widget = new QWidget();
    QVBoxLayout *left_layout = new QVBoxLayout();
    widget->setLayout(left_layout);

    yuv_image_group = createYuvImageGroupbox();
    raw_image_group = createRawImageGroupbox();
    dump_all_group = createDumpAllGroupbox();
    capture_group = createCaptureTimeGroupbox();
    img_viewer = new MultiImageViewerWidget();
    img_viewer->setFilePathHidden();
    img_viewer->showRawStatisticBtn();
    img_viewer->showLevelMeterBtn();
    ImageLabel *image_label = new ImageLabel(this, ImageLabel::NORMAL);
    connect(image_label, &ImageLabel::sigPositionChange, img_viewer, &MultiImageViewerWidget::slotPositionChange);
    connect(image_label, &ImageLabel::sigUpdateSingleRect, img_viewer, &MultiImageViewerWidget::handleRectToCropInfo);
    img_viewer->setImageLabel(image_label);
    img_viewer->setMinimumHeight(600);
    pixmap = new QPixmap;

    shared_ptr<GlobalData> global_data = GlobalData::getInstance();
    QString save_dir = global_data->getSettings(SETTINGS_SECTION, KEY_CAPTURE_SAVE_PATH).toString();
    QString exe_dir = QCoreApplication::applicationDirPath();
    open_btn = new QPushButton(tr("Open Save Path"));
    file_path = new QLineEdit();
    file_path->setReadOnly(true);
    if (!save_dir.isEmpty()) {
        QDir dir(save_dir);
        if(dir.exists()) {
            file_path->setText(save_dir);
        } else {
            file_path->setText(exe_dir);
            global_data->setSettings(SETTINGS_SECTION, KEY_CAPTURE_SAVE_PATH, "");
            global_data->saveSettings();
            QMessageBox::warning(this, tr("Warning"), tr("Can't find \"" + save_dir.toUtf8() + "\" directory! Use default save path."), QMessageBox::Ok);
        }
    } else {
        file_path->setText(exe_dir);
    }

    QHBoxLayout *filepath_layout = new QHBoxLayout;
    filepath_layout->addWidget(open_btn);
    filepath_layout->addWidget(file_path);
    connect(open_btn, &QPushButton::clicked, this, &CaptureWindow::OpenSaveFilePath);

    QVBoxLayout *checkbox_layout = new QVBoxLayout;
    wdr_raw_separately_checkbox = new QCheckBox("Save WDR LE and SE raws individually");
    checkbox_layout->addWidget(wdr_raw_separately_checkbox);

    raw_tightly_checkbox = new QCheckBox("Capture raw tightly");
    QString soc = GlobalData::getInstance()->getCvipqToolSoc();
    if (CV183X_SOC == soc) {
        raw_tightly_checkbox->setChecked(true);
    } else {
        raw_tightly_checkbox->setChecked(false);
    }
    checkbox_layout->addWidget(raw_tightly_checkbox);

    save_bmp_file_checkbox = new QCheckBox("Save BMP file");
    save_bmp_file_checkbox->setChecked(true);
    checkbox_layout->addWidget(save_bmp_file_checkbox);

    raw_checkbox = new QCheckBox("dump raw");
    raw_checkbox->setChecked(true);
    checkbox_layout->addWidget(raw_checkbox);

    left_layout->addWidget(yuv_image_group, 1, Qt::AlignTop);
    left_layout->addWidget(raw_image_group, 1, Qt::AlignTop);
    left_layout->addWidget(dump_all_group, 1, Qt::AlignTop);
    left_layout->addWidget(capture_group, 1, Qt::AlignTop);
    left_layout->addLayout(filepath_layout, 0);
    left_layout->addLayout(checkbox_layout, 0);
    left_layout->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);

    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setMinimumWidth(560);
    scrollArea->setMaximumWidth(600);
    scrollArea->setWidget(widget);

    main_layout->addWidget(scrollArea, 1);
    main_layout->addWidget(img_viewer, 2);

    setCentralWidget(main_widget);
    centralWidget()->setLayout(main_layout);
}

QGroupBox *CaptureWindow::createYuvImageGroupbox()
{
    QGroupBox * groupbox = new QGroupBox();
    groupbox->setTitle("YUV Image");

    QWidget *cap_src_widget = new QWidget;
    QHBoxLayout *cap_src_layout = new QHBoxLayout(cap_src_widget);
    QLabel *cap_src_label = new QLabel;
    cap_src_label->setText(tr("Capture from:"));
    cap_src_label->setFixedSize(120, 24);
    yuv_cap_src_box = new QComboBox();
    yuv_cap_src_box->setFixedSize(70, 24);
    QStringList str_list = {"VI", "VPSS"};
    yuv_cap_src_box->addItems(str_list);
    yuv_cap_src_box->setCurrentIndex(0);
    cap_src_layout->addWidget(cap_src_label, 0);
    cap_src_layout->addWidget(yuv_cap_src_box, 0);

    QWidget *yuv_frame_widget = new QWidget;
    QHBoxLayout *yuv_frame_layout = new QHBoxLayout(yuv_frame_widget);
    QLabel *yuv_frames_label = new QLabel(tr("Frames:"));
    yuv_frames_label->setFixedSize(60, 24);
    QLabel *yuv_frames_tips = new QLabel("(1-90)");
    yuv_frames_tips->setFixedSize(50, 24);
    yuv_frames_edit = new QLineEdit;
    yuv_frames_edit->setFixedSize(45, 24);
    yuv_frames_edit->setText("1");
    connect(yuv_frames_edit, &QLineEdit::textChanged, this, std::bind(&CaptureWindow::cellEdited, this, 0));
    yuv_frame_layout->addWidget(yuv_frames_label, 0);
    yuv_frame_layout->addWidget(yuv_frames_edit, 0);
    yuv_frame_layout->addWidget(yuv_frames_tips, 0);

    QHBoxLayout *line1_layout = new QHBoxLayout;
    line1_layout->addWidget(cap_src_widget);
    line1_layout->addWidget(yuv_frame_widget);

    yuv_capture_btn = new QPushButton;
    yuv_capture_btn->setText(tr("Capture"));
    yuv_capture_btn->setMaximumSize(200, 38);
    connect(yuv_capture_btn, &QPushButton::clicked,
        this, &CaptureWindow::handleCaptureYuvImg);

    yuv_cancel_btn = new QPushButton(tr("Cancel"));
    yuv_cancel_btn->setMaximumSize(200, 38);
    yuv_cancel_btn->setEnabled(false);
    connect(yuv_cancel_btn, &QPushButton::clicked,
            this, &CaptureWindow::handleCancelEvent);


    QHBoxLayout *line2_layout = new QHBoxLayout;
    line2_layout->addWidget(yuv_capture_btn);
    line2_layout->addWidget(yuv_cancel_btn);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(line1_layout);
    layout->addLayout(line2_layout);

    // Image format selection (JPG / PNG)
    QWidget *format_widget = new QWidget;
    QHBoxLayout *format_layout = new QHBoxLayout(format_widget);
    QLabel *format_label = new QLabel(tr("Image format:"));
    format_label->setFixedSize(80, 24);
    image_format_box = new QComboBox();
    image_format_box->setFixedSize(80, 24);
    image_format_box->addItems(QStringList{"JPG", "PNG"});
    // load persisted setting if available
    QString savedFmt = GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_CAPTURE_IMAGE_FORMAT, "JPG").toString();
    int idx = image_format_box->findText(savedFmt, Qt::MatchFixedString);
    if (idx >= 0) image_format_box->setCurrentIndex(idx);
    connect(image_format_box, &QComboBox::currentTextChanged, [=](const QString &s){
        GlobalData::getInstance()->setSettings(SETTINGS_SECTION, KEY_CAPTURE_IMAGE_FORMAT, s);
        GlobalData::getInstance()->saveSettings();
    });
    format_layout->addWidget(format_label, 0);
    format_layout->addWidget(image_format_box, 0);
    layout->addWidget(format_widget);

    groupbox->setLayout(layout);
    return groupbox;
}

QGroupBox *CaptureWindow::createRawImageGroupbox()
{
    QGroupBox * groupbox = new QGroupBox();
    groupbox->setTitle("RAW Image");

    QWidget *raw_type_widget = new QWidget;
    QHBoxLayout *raw_type_layout = new QHBoxLayout(raw_type_widget);
    QLabel *raw_type_label = new QLabel(tr("Type:"));
    raw_type_label->setFixedSize(60, 24);
    raw_type_box = new QComboBox;
    raw_type_box->addItem("Bayer");
    raw_type_box->setCurrentIndex(0);
    raw_type_box->setDisabled(true);
    raw_type_box->setFixedSize(90, 24);
    raw_type_layout->setSpacing(1);
    raw_type_layout->addWidget(raw_type_label);
    raw_type_layout->addWidget(raw_type_box);
    raw_type_layout->setAlignment(Qt::AlignLeft);

    QWidget *raw_bits_widget = new QWidget;
    QHBoxLayout *raw_bits_layout = new QHBoxLayout(raw_bits_widget);
    QLabel *raw_bits_label = new QLabel(tr("Bits:"));
    raw_bits_label->setFixedSize(60, 24);
    QStringList raw_bits_list = {"8 bits", "10 bits", "12 bits", "16 bits"};
    raw_bits_box = new QComboBox;
    raw_bits_box->addItems(raw_bits_list);
    raw_bits_box->setCurrentIndex(2);
    raw_bits_box->setFixedSize(90, 24);
    raw_bits_box->setEnabled(false);
    raw_bits_layout->setSpacing(1);
    raw_bits_layout->addWidget(raw_bits_label);
    raw_bits_layout->addWidget(raw_bits_box);
    raw_bits_layout->setAlignment(Qt::AlignLeft);

    QHBoxLayout *line1_layout = new QHBoxLayout;
    line1_layout->addWidget(raw_type_widget);
    line1_layout->addWidget(raw_bits_widget);

    QWidget *raw_mode_widget = new QWidget;
    QHBoxLayout *raw_mode_layout = new QHBoxLayout(raw_mode_widget);
    QLabel *raw_mode_label = new QLabel(tr("Mode:"));
    raw_mode_label->setFixedSize(60, 24);
    raw_mode_box = new QComboBox;
    QStringList raw_mode_list{"LINEAR", "WDR_2TO1", "WDR_3TO1","WDR_4TO1"};
    raw_mode_box->addItems(raw_mode_list);
    raw_mode_box->setCurrentIndex(0);
    raw_mode_box->setFixedSize(100, 24);
    raw_mode_layout->setSpacing(1);
    raw_mode_layout->addWidget(raw_mode_label);
    raw_mode_layout->addWidget(raw_mode_box);
    raw_mode_layout->setAlignment(Qt::AlignLeft);


    QWidget *raw_frames_widget = new QWidget;
    QHBoxLayout *raw_frames_layout = new QHBoxLayout(raw_frames_widget);
    QLabel *raw_frames_label = new QLabel(tr("Frames:"));
    raw_frames_label->setFixedSize(60, 24);
    raw_frames_tips = new QLabel("(1-90)");
    raw_frames_tips->setFixedSize(50, 24);
    raw_frames_edit = new QLineEdit;
    raw_frames_edit->setFixedSize(45, 24);
    raw_frames_edit->setText("1");
    connect(raw_frames_edit, &QLineEdit::textChanged, this, std::bind(&CaptureWindow::cellEdited, this, 1));
    raw_frames_layout->setSpacing(1);
    raw_frames_layout->addWidget(raw_frames_label, 0);
    raw_frames_layout->addWidget(raw_frames_edit, 0);
    raw_frames_layout->addWidget(raw_frames_tips, 0);

    connect(raw_mode_box, &QComboBox::currentTextChanged, this, &CaptureWindow::slotCheckRawParams);

    QHBoxLayout *line2_layout = new QHBoxLayout;
    line2_layout->addWidget(raw_mode_widget);
    line2_layout->addWidget(raw_frames_widget);

    raw_capture_btn = new QPushButton;
    raw_capture_btn->setText(tr("Capture"));
    raw_capture_btn->setMaximumSize(200, 38);
    connect(raw_capture_btn, &QPushButton::clicked,
            this, &CaptureWindow::handleCaptureRawImg);

    raw_cancel_btn = new QPushButton(tr("Cancel"));
    raw_cancel_btn->setMaximumSize(200, 38);
    raw_cancel_btn->setEnabled(false);
    connect(raw_cancel_btn, &QPushButton::clicked,
            this, &CaptureWindow::handleCancelEvent);

    QHBoxLayout *line3_layout = new QHBoxLayout;
    line3_layout->addWidget(raw_capture_btn);
    line3_layout->addWidget(raw_cancel_btn);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(line1_layout);
    layout->addLayout(line2_layout);
    layout->addLayout(line3_layout);

    groupbox->setLayout(layout);
    return groupbox;
}

QGroupBox *CaptureWindow::createDumpAllGroupbox()
{
    QGroupBox * groupbox = new QGroupBox();
    groupbox->setTitle("Dump All");

    QWidget *repeatedly_widget = new QWidget;
    QHBoxLayout *repeatedly_layout = new QHBoxLayout(repeatedly_widget);
    QLabel *interval_label = new QLabel(tr("Interval:"));
    interval_label->setFixedSize(80, 24);
    QLabel *interval_tips = new QLabel("sec(0-300)");
    interval_tips->setFixedSize(100, 24);
    interval_edit = new QLineEdit;
    interval_edit->setFixedSize(45, 24);
    interval_edit->setText("3");
    interval_edit->setEnabled(false);
    connect(interval_edit, &QLineEdit::textChanged, this, std::bind(&CaptureWindow::cellEdited, this, 2));
    dump_all_repeatedly_checkbox = new QCheckBox("Dump all repeatedly");
    connect(dump_all_repeatedly_checkbox, &QCheckBox::clicked,
        [=] () {
            if (dump_all_repeatedly_checkbox->isChecked()) {
                interval_edit->setEnabled(true);
            } else {
                interval_edit->setEnabled(false);
            }
        });
    repeatedly_layout->addWidget(dump_all_repeatedly_checkbox, 0);
    repeatedly_layout->addWidget(interval_label, 0);
    repeatedly_layout->addWidget(interval_edit, 0);
    repeatedly_layout->addWidget(interval_tips, 0);

    dump_btn = new QPushButton;
    dump_btn->setText(tr("Capture"));
    dump_btn->setMaximumSize(200, 38);
    connect(dump_btn, &QPushButton::clicked, this,
        std::bind(&CaptureWindow::handleDumpAll, this, DUMPALL_MODE::MODE_NORMAL, "", ""));

    dump_cancel_btn = new QPushButton(tr("Cancel"));
    dump_cancel_btn->setMaximumSize(200, 38);
    dump_cancel_btn->setEnabled(false);
    connect(dump_cancel_btn, &QPushButton::clicked, this, &CaptureWindow::handleCancelEvent);

    QWidget *capture_widget = new QWidget;
    QHBoxLayout *capture_layout = new QHBoxLayout(capture_widget);
    capture_layout->addWidget(dump_btn);
    capture_layout->addWidget(dump_cancel_btn);

    QVBoxLayout *dump_all_layout = new QVBoxLayout;
    dump_all_layout->addWidget(repeatedly_widget);
    dump_all_layout->addWidget(capture_widget);

    groupbox->setLayout(dump_all_layout);

    return groupbox;
}

QGroupBox *CaptureWindow::createCaptureTimeGroupbox()
{
    QGroupBox * groupbox = new QGroupBox();
    groupbox->setTitle("Capture Timing");

    QWidget *cap_time_widget = new QWidget(this);
    QHBoxLayout *cap_time_layout = new QHBoxLayout(cap_time_widget);
    QLabel *time_label = new QLabel(tr("Time:"));
    time_label->setFixedSize(60, 24);
    QLabel *Time_tips = new QLabel("sec(1-60)");
    Time_tips->setFixedSize(100, 24);
    time_edit = new QLineEdit;
    time_edit->setFixedSize(45, 24);
    time_edit->setText("10");
    connect(time_edit, &QLineEdit::textChanged, this, std::bind(&CaptureWindow::cellEdited, this, 3));
    start_btn = new QPushButton;
    start_btn->setText(tr("Start"));
    start_btn->setMaximumSize(200, 38);
    connect(start_btn, &QPushButton::clicked, this, &CaptureWindow::startTimingEvent);
    stop_btn = new QPushButton(tr("Stop"));
    stop_btn->setMaximumSize(200, 38);
    stop_btn->setEnabled(false);
    connect(stop_btn, &QPushButton::clicked, this, &CaptureWindow::handleCancelEvent);
    cap_time_layout->addWidget(start_btn, 0);
    cap_time_layout->addWidget(stop_btn, 0);

    //selete timing obj
    QWidget *cap_type_widget = new QWidget;
    QHBoxLayout *cap_type_layout = new QHBoxLayout(cap_type_widget);
    QLabel *cap_mode_label = new QLabel;
    cap_mode_label->setText(tr("Capture mode:"));
    cap_mode_label->setFixedSize(120, 24);
    type_cap_src_box = new QComboBox();
    type_cap_src_box->setFixedSize(115, 24);
    QStringList str_list = {"YUV Image", "RAW Image", "Dump All"};
    type_cap_src_box->addItems(str_list);
    type_cap_src_box->setCurrentIndex(0);
    cap_type_layout->addWidget(cap_mode_label, 0);
    cap_type_layout->addWidget(type_cap_src_box, 0);
    cap_type_layout->addWidget(time_label, 0);
    cap_type_layout->addWidget(time_edit, 0);
    cap_type_layout->addWidget(Time_tips, 0);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(cap_type_widget);
    layout->addWidget(cap_time_widget);
    // Option: save periodic captures to the same folder
    QWidget *same_folder_widget = new QWidget;
    QHBoxLayout *same_folder_layout = new QHBoxLayout(same_folder_widget);
    use_same_folder_checkbox = new QCheckBox(tr("Use same folder for periodic captures"));
    bool useSame = GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_CAPTURE_PERIODIC_SAME_FOLDER, false).toBool();
    use_same_folder_checkbox->setChecked(useSame);
    connect(use_same_folder_checkbox, &QCheckBox::clicked, [=](bool checked){
        GlobalData::getInstance()->setSettings(SETTINGS_SECTION, KEY_CAPTURE_PERIODIC_SAME_FOLDER, checked);
        GlobalData::getInstance()->saveSettings();
    });
    same_folder_layout->addWidget(use_same_folder_checkbox, 0);
    layout->addWidget(same_folder_widget);

    // Filename mode: Timestamp / Append number / Custom overwrite
    QWidget *fname_widget = new QWidget;
    QHBoxLayout *fname_layout = new QHBoxLayout(fname_widget);
    QLabel *fname_label = new QLabel(tr("Filename mode:"));
    fname_label->setFixedSize(100, 24);
    filename_mode_box = new QComboBox();
    filename_mode_box->setFixedSize(140, 24);
    filename_mode_box->addItems(QStringList{"Timestamp", "Append number", "Custom (overwrite)"});
    int savedMode = GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_CAPTURE_FILENAME_MODE, 0).toInt();
    if (savedMode >=0 && savedMode <=2) filename_mode_box->setCurrentIndex(savedMode);
    custom_base_edit = new QLineEdit();
    custom_base_edit->setFixedSize(140, 24);
    QString savedBase = GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_CAPTURE_CUSTOM_BASE, "").toString();
    custom_base_edit->setText(savedBase);
    custom_base_edit->setEnabled(filename_mode_box->currentIndex() == 2);
    connect(filename_mode_box, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int idx){
        GlobalData::getInstance()->setSettings(SETTINGS_SECTION, KEY_CAPTURE_FILENAME_MODE, idx);
        GlobalData::getInstance()->saveSettings();
        custom_base_edit->setEnabled(idx == 2);
    });
    connect(custom_base_edit, &QLineEdit::textChanged, [=](const QString &txt){
        GlobalData::getInstance()->setSettings(SETTINGS_SECTION, KEY_CAPTURE_CUSTOM_BASE, txt);
        GlobalData::getInstance()->saveSettings();
    });
    fname_layout->addWidget(fname_label, 0);
    fname_layout->addWidget(filename_mode_box, 0);
    fname_layout->addWidget(custom_base_edit, 0);
    layout->addWidget(fname_widget);
    groupbox->setLayout(layout);

    return groupbox;
}

void CaptureWindow::initEvent()
{
    shared_ptr<NetworkClientHelper> network_helper = NetworkClientHelper::getInstance();
    connect(GlobalData::getInstance().get(), &GlobalData::sigGetWDRFusionFrame, this,
        &CaptureWindow::handleGetWDRFusionFrame);
    connect(network_helper.get(), &NetworkClientHelper::sigCaptueYuvStatus, this,
        &CaptureWindow::handleMultiYuvStatus);
    connect(network_helper.get(), &NetworkClientHelper::sigCaptureYuvData, this,
            &CaptureWindow::slotGotYuvImg);
    connect(network_helper.get(), &NetworkClientHelper::sigCaptueRawStatus, this,
        &CaptureWindow::handleMultiRawStatus);
    connect(network_helper.get(), &NetworkClientHelper::sigCaptureBinInfo, this,
        &CaptureWindow::handleRecvBinInfo);
    connect(network_helper.get(), &NetworkClientHelper::sigCaptureBinData, this,
        &CaptureWindow::handleRecvBinData);
    connect(network_helper.get(), &NetworkClientHelper::sigCaptureRawData, this,
            &CaptureWindow::slotGotRawImg);
    connect(this, &CaptureWindow::sigCaptureYuvEnd, this, &CaptureWindow::handleCaptureRawImg);
    connect(this, &CaptureWindow::sigCaptureRawEnd, this,
        std::bind(&CaptureWindow::handleDumpAll, this, DUMPALL_MODE::MODE_NORMAL, "", ""));

    start_timer = new QTimer();
    connect(start_timer, &QTimer::timeout, this, &CaptureWindow::startTimingCaptureEvent);
}

void CaptureWindow::closeEvent(QCloseEvent *event)
{
    shared_ptr<NetworkClientHelper> network_helper = NetworkClientHelper::getInstance();
    event->accept();
    QMainWindow::closeEvent(event);
    //TODO handle close event
}

void CaptureWindow::showEvent(QShowEvent *event)
{
    handleGetWDRFusionFrame();
    event->accept();
}

void CaptureWindow::refresh()
{
}

void CaptureWindow::saveYuvFile(QByteArray &data, YUV_HEADER &header, int total_received_frame)
{
    int n = 0, i = 0, idx = 0;
    unsigned int frame_start = 0, u_start = 0, v_start = 0, uv_start = 0;
    const char *pData = data.data();

    if (!directory_str.isEmpty()) {
        QString yuv_path, bmp_path, jpg_path;
        QString path = directory_str + "/" + QString("%1X%2_").arg(header.width).arg(header.height);
        if (header.pixelFormat == PIXEL_FORMAT_NV21) {
            path = path + "NV21_";
        } else if (header.pixelFormat == PIXEL_FORMAT_NV12) {
            path = path + "NV12_";
        } else  if (header.pixelFormat == PIXEL_FORMAT_YUV_PLANAR_420) {
            path = path + "YUV420_";
        } else {
            path = path + "NULL_";
        }

        // Determine filename suffix according to selected mode
        int fnameMode = GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_CAPTURE_FILENAME_MODE, 0).toInt();
        QString customBase = GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_CAPTURE_CUSTOM_BASE, "").toString();
        QString usedDateTimeStr = periodic_mode_active ? QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz") : dateTime_str;
        QString suffix;
        if (fnameMode == 0) {
            suffix = usedDateTimeStr;
        } else if (fnameMode == 1) {
            // append numeric sequence
            QString seq = QString::number(periodic_file_seq).rightJustified(4, '0');
            // include base time for readability
            suffix = usedDateTimeStr + "_" + seq;
        } else if (fnameMode == 2) {
            // custom base - overwrite
            suffix = customBase;
        } else {
            suffix = usedDateTimeStr;
        }

        yuv_path = path + QString("-bits=8_-frame=%1_") .arg(total_received_frame) + suffix + ".yuv";
        bmp_path = path + "-bits=8_-frame=1_" + suffix + ".bmp";
        QString imgExt = "jpg";
        if (image_format_box) {
            imgExt = image_format_box->currentText().toLower();
        }
        QString img_path = path + "-bits=8_-frame=1_" + suffix + "." + imgExt;

        QFile fileMultiYuv(yuv_path);
        fileMultiYuv.open(QIODevice::WriteOnly);
        QImage image = utils::convertYUV2RGBImage(data, &header);
        QPainter p(&image);
        p.setPen(QPen(Qt::white));
        p.setFont(QFont("Times", 24, QFont::Bold));
        p.drawText(image.rect(), Qt::AlignLeft, "CVITEK " + dateTime_str);
        // save using selected format (JPG or PNG)
        image.save(img_path, image_format_box ? image_format_box->currentText().toUtf8().constData() : "JPG");
        if (save_bmp_file_checkbox->isChecked()) {
            image.save(bmp_path, "BMP");
        }

        if (header.pixelFormat == PIXEL_FORMAT_NV21 || header.pixelFormat == PIXEL_FORMAT_NV12) {
            for(n = 0; n < total_received_frame; n++) {
                uv_start = header.stride[0] * header.height;
                for(i = 0, idx = frame_start; i < header.height; i++) {
                    fileMultiYuv.write(pData + idx, header.width);
                    idx += header.stride[0];
                }

                for(i = 0, idx = frame_start + uv_start; i < header.height / 2; i++) {
                    fileMultiYuv.write(pData + idx, header.width);
                    idx += header.stride[1];
                }

                frame_start += header.size;
            }
        } else if (header.pixelFormat == PIXEL_FORMAT_YUV_PLANAR_420) {
            for(n = 0; n < total_received_frame; n++) {
                u_start = header.stride[0] * header.height;
                v_start = u_start + header.stride[1] * header.height / 2;
                for(i = 0, idx = frame_start; i < header.height; i++) {
                    fileMultiYuv.write(pData + idx, header.width);
                    idx += header.stride[0];
                }

                for(i = 0, idx = frame_start + u_start; i < header.height / 2; i++) {
                    fileMultiYuv.write(pData + idx, header.width / 2);
                    idx += header.stride[1];
                }

                for(i = 0, idx = frame_start + v_start; i < header.height / 2; i++) {
                    fileMultiYuv.write(pData + idx, header.width / 2);
                    idx += header.stride[2];
                }

                frame_start += header.size;
            }
        }  else {
            QMessageBox::warning(this, tr("Error"), tr("YUV pixel format is not supported!"), QMessageBox::Ok);
        }

        fileMultiYuv.close();
        // if using sequential filenames in periodic mode, increment sequence
        if (periodic_mode_active) {
            int fnameModeLocal = GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_CAPTURE_FILENAME_MODE, 0).toInt();
            if (fnameModeLocal == 1) {
                periodic_file_seq++;
            }
        }
    } else {
        QMessageBox::warning(this, tr("Error"), tr("Save directory path is empty!"), QMessageBox::Ok);
    }
}

void CaptureWindow::setRawFileName(RAW_HEADER &header, int frameNum)
{
    if (!directory_str.isEmpty()) {
        // widht, height --> ex: 3840X1080
        file_name_linear = directory_str + "/" + QString("%1X%2_").arg(header.cropWidth).arg(header.cropHeight);
        file_name_wdr = directory_str + "/" + QString("%1X%2_").arg(header.cropWidth * 2).arg(header.cropHeight);
        file_name_wdr_le = directory_str + "/" + QString("%1X%2_").arg(header.cropWidth * 2).arg(header.cropHeight);
        file_name_wdr_se = directory_str + "/" + QString("%1X%2_").arg(header.cropWidth *2).arg(header.cropHeight);

        // bayer string, type string --> ex: RGGB_WDR
        file_name_linear += MW_BAYER_FORMAT_STRING[header.bayerID] + "_Linear_";
        file_name_wdr += MW_BAYER_FORMAT_STRING[header.bayerID] + "_WDR_";
        file_name_wdr_le += MW_BAYER_FORMAT_STRING[header.bayerID] + "_WDR_LE_";
        file_name_wdr_se += MW_BAYER_FORMAT_STRING[header.bayerID] + "_WDR_SE_";


        // bayerID, bits, frames --> ex: color=3_-bits=12_-frame=5
        file_name_linear += QString("-color=%1_-bits=12_-frame=%2_").arg(header.bayerID).arg(frameNum);
        file_name_wdr += QString("-color=%1_-bits=12_-frame=%2_").arg(header.bayerID).arg(frameNum);
        file_name_wdr_le += QString("-color=%1_-bits=12_-frame=%2_").arg(header.bayerID).arg(frameNum);
        file_name_wdr_se += QString("-color=%1_-bits=12_-frame=%2_").arg(header.bayerID).arg(frameNum);

        // isWdr, ISO --> ex: hdr=1_ISO=100
        file_name_linear += QString("-hdr=%1_ISO=%2_").arg(header.fusionFrame).arg(header.iso);
        file_name_wdr += QString("-hdr=%1_ISO=%2_").arg(header.fusionFrame).arg(header.iso);
        file_name_wdr_le += QString("-hdr=%1_ISO=%2_").arg(header.fusionFrame).arg(header.iso);
        file_name_wdr_se += QString("-hdr=%1_ISO=%2_").arg(header.fusionFrame).arg(header.iso);

    // date, time --> 20210314122032
    int fnameMode = GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_CAPTURE_FILENAME_MODE, 0).toInt();
    QString customBase = GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_CAPTURE_CUSTOM_BASE, "").toString();
    QString usedTimeStr = periodic_mode_active ? QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz") : dateTime_str;
    QString suffix;
    if (fnameMode == 0) {
        suffix = usedTimeStr;
    } else if (fnameMode == 1) {
        QString seq = QString::number(periodic_file_seq).rightJustified(4, '0');
        suffix = usedTimeStr + "_" + seq;
    } else if (fnameMode == 2) {
        suffix = customBase;
    } else {
        suffix = usedTimeStr;
    }

    file_name_linear += suffix;
    file_name_wdr += suffix;
    file_name_wdr_le += suffix;
    file_name_wdr_se += suffix;

    } else {
        QMessageBox::warning(this, tr("Error"), tr("Save directory path is empty!"), QMessageBox::Ok);
    }
}

void CaptureWindow::check_file_validity()
{
    QDir dir(directory_str);
    QStringList file_list = dir.entryList(QDir::Files);
    QString file_path = directory_str + '/';
    bool exist_empty_file_flag = false;
    bool exist_ae_log_file_flag = false;

    for (int i = 0; i < file_list.count(); i++) {
        QString file_name = file_list.at(i);
        QFileInfo file_info(file_path + file_name);

        if (file_info.size() == 0) {
            exist_empty_file_flag = true;
            QMessageBox::warning(this, tr("Warning"), tr("Exist empty file, please capture again!"), QMessageBox::Ok);
            break;
        }
        if (file_name.contains("aelog.txt")) {
            exist_ae_log_file_flag = true;
            break;
        }
    }
    if (!exist_empty_file_flag && !exist_ae_log_file_flag) {
        QMessageBox::warning(this, tr("Warning"), tr("Miss aelog file, please capture again!"), QMessageBox::Ok);
    }
}

void CaptureWindow::saveRawFile(QByteArray &data, RAW_HEADER &header, int raw_type, int frameNum)
{
    if (!directory_str.isEmpty()) {
        QString raw_info_name, raw_file_name, reg_dump_name,
            ae_log_name, awb_log_name, awb_bin_name, ae_bin_name;
        if (dumpall_mode != MODE_AE10RAW_BATCH) {
            setRawFileName(header, frameNum);
        }

        if (raw_type == LINEAR_RAW) {
            raw_info_name = file_name_linear + ".txt";
            raw_file_name = file_name_linear + ".raw";
            reg_dump_name = file_name_linear + ".json";
            ae_log_name = file_name_linear + "-aelog.txt";
            awb_log_name = file_name_linear + "-awblog.txt";
            awb_bin_name = file_name_linear + "-awb.wbin";
            ae_bin_name = file_name_linear + "-ae.bin";
        } else if (raw_type == WDR_RAW) {
            raw_info_name = file_name_wdr + ".txt";
            raw_file_name = file_name_wdr + ".raw";
            reg_dump_name = file_name_wdr + ".json";
            ae_log_name = file_name_wdr + "-aelog.txt";
            awb_log_name = file_name_wdr + "-awblog.txt";
            awb_bin_name = file_name_wdr + "-awb.wbin";
            ae_bin_name = file_name_wdr + "-ae.bin";
        } else if (raw_type == WDR_LE_RAW) {
            raw_info_name = file_name_wdr_le + ".txt";
            raw_file_name = file_name_wdr_le + ".raw";
            reg_dump_name = file_name_wdr_le + ".json";
            ae_log_name = file_name_wdr_le + "-aelog.txt";
            awb_log_name = file_name_wdr_le + "-awblog.txt";
            awb_bin_name = file_name_wdr_le + "-awb.wbin";
            ae_bin_name = file_name_wdr_le + "-ae.bin";
        } else if (raw_type == WDR_SE_RAW) {
            raw_info_name = file_name_wdr_se + ".txt";
            raw_file_name = file_name_wdr_se + ".raw";
            reg_dump_name = file_name_wdr_se + ".json";
            ae_log_name = file_name_wdr_se + "-aelog.txt";
            awb_log_name = file_name_wdr_se + "-awblog.txt";
            awb_bin_name = file_name_wdr_se + "-awb.wbin";
            ae_bin_name = file_name_wdr_se + "-ae.bin";
        }

        if (!raw_info_log.isEmpty()) {
            QFile raw_info_file(raw_info_name);
            raw_info_file.open(QIODevice::WriteOnly);
            raw_info_file.write(raw_info_log.toUtf8());
            raw_info_file.close();
        }

        if (!data.isEmpty()) {
            QFile raw_file(raw_file_name);
            raw_file.open(QIODevice::WriteOnly);
            raw_file.write(data);
            raw_file.close();
        }

        if (dumpall_mode != MODE_AE10RAW_BATCH) {
            if (!reg_dump_log.isEmpty()) {
                QFile reg_dump_file(reg_dump_name);
                reg_dump_file.open(QIODevice::WriteOnly);
                reg_dump_file.write(reg_dump_log.toUtf8());
                reg_dump_file.close();
            }
        }

        if (!ae_log.isEmpty()) {
            QFile ae_log_file(ae_log_name);
            ae_log_file.open(QIODevice::WriteOnly);
            ae_log_file.write(ae_log.toUtf8());
            ae_log_file.close();
        }

        if (!awb_log.isEmpty()) {
            QFile awb_log_file(awb_log_name);
            awb_log_file.open(QIODevice::WriteOnly);
            awb_log_file.write(awb_log.toUtf8());
            awb_log_file.close();
        }

        if (!awb_bin.isEmpty()) {
            QFile awb_bin_file(awb_bin_name);
            awb_bin_file.open(QIODevice::WriteOnly);
            awb_bin_file.write(awb_bin);
            awb_bin_file.close();
        }

        if (!ae_bin.isEmpty()) {
            QFile ae_bin_file(ae_bin_name);
            ae_bin_file.open(QIODevice::WriteOnly);
            ae_bin_file.write(ae_bin);
            ae_bin_file.close();
        }

        check_file_validity();
        // advance sequence for periodic sequential filename mode
        if (periodic_mode_active) {
            int fnameModeLocal = GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_CAPTURE_FILENAME_MODE, 0).toInt();
            if (fnameModeLocal == 1) {
                periodic_file_seq++;
            }
        }
    } else {
        QMessageBox::warning(this, tr("Error"), tr("Save directory path is empty!"), QMessageBox::Ok);
    }
}

bool CaptureWindow::CheckSaveFilePath() {
    if (!file_path->text().isEmpty()) {
        QDir dir(file_path->text());
        if(!dir.exists()) {
            QMessageBox::warning(this, tr("Error"), tr("Can't find \"" + file_path->text().toUtf8() + "\" directory! Please check save path."), QMessageBox::Ok);
            return false;
        }
    } else {
        QMessageBox::warning(this, tr("Error"), tr("Save path is empty!"), QMessageBox::Ok);
        return false;
    }

    return true;
}

void CaptureWindow::handleCaptureYuvImg()
{
    qDebug("CaptureWindow::handleCaptureYuvImg\n");

    if (yuv_frames_edit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("Please Input Frame Numbers!"), QMessageBox::Ok);
        return;
    }
    emit fetchStart();
    emit sigStatusLog("Start capture yuv image", LOG_LEVEL::LOG_INFO);
    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        return;
    }

    if (!CheckSaveFilePath()) {
        return;
    }

    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);

    if (!start_dump && !periodic_mode_active) {
        disableComponents();
        yuv_cancel_btn->setEnabled(true);
        dateTime_str = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
        directory_str = file_path->text() + "/yuv_" + dateTime_str;
        QDir().mkdir(directory_str);
    }

    start_capture = true;
    total_frame = yuv_frames_edit->text().toInt();
    current_frame = 0;
    recvSize = 0;
    receive_yuv_data.clear();

    QString from = yuv_cap_src_box->currentText();
    if (from == "VI") {
        capture_from = 0;
    } else if (from == "VPSS") {
        capture_from = 1;
    }

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_CAPTURE_GET_MULTI_YUV;
    int id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

    QJsonObject param_obj;
    param_obj["from"] = capture_from;
    if (raw_tightly_checkbox->isChecked()) {
        param_obj["frames"] = total_frame;
        param_obj["tightly"] = true;
    } else {
        param_obj["frames"] = 1;
        param_obj["tightly"] = false;
    }
    param_obj["getRawReplayYuv"] = false;
    param_obj["getRawReplayYuvId"] = 0;
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_MULTI_YUV;
    send_obj[MSG_KEY_ID] = id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_PARAMS] = param_obj;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void CaptureWindow::handleMultiYuvStatus(int id)
{
    qDebug("CaptureWindow::handleMultiYuvStatus\n");

    QJsonObject obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    if(!start_capture) {
        return;
    }

    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();
    if (result_obj.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("Capture yuv failed!"), QMessageBox::Ok);
        return;
    }

    QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();
    recvSize = params_obj["Data size"].toInt();
    NetworkClientHelper::getInstance()->setCallerInfo(recvSize, CMD_CAPTURE_GET_MULTI_YUV);
    qDebug("Set receive size:%d\n", recvSize);

    int temp_id = GlobalData::getInstance()->getJsonrpcId();
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_IMAGE_DATA;
    send_obj[MSG_KEY_ID] = temp_id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void CaptureWindow::slotGotYuvImg(QString cmd)
{
    qDebug("CaptureWindow::slotGotYuvImg\n");

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

    if (pstHeader->curFrame < (pstHeader->numFrame - 1)) {
        //send command to get next frame
        NetworkClientHelper::getInstance()->setCallerInfo(recvSize, CMD_CAPTURE_GET_MULTI_YUV);
        qDebug("Set receive size:%d\n", recvSize);

        int temp_id = GlobalData::getInstance()->getJsonrpcId();
        QJsonObject send_obj;
        send_obj[MSG_KEY_METHOD] = METHOD_GET_IMAGE_DATA;
        send_obj[MSG_KEY_ID] = temp_id;
        send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
    } else if (!raw_tightly_checkbox->isChecked() && (current_frame < total_frame - 1)) {
        current_frame++;
        QJsonObject id_info;
        id_info[MSG_KEY_CMD] = CMD_CAPTURE_GET_MULTI_YUV;
        int id = GlobalData::getInstance()->getJsonrpcId();
        NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

        QJsonObject param_obj;
        param_obj["from"] = capture_from;
        param_obj["frames"] = 1;
        param_obj["tightly"] = false;
        param_obj["getRawReplayYuv"] = false;
        param_obj["getRawReplayYuvId"] = 0;
        QJsonObject send_obj;
        send_obj[MSG_KEY_METHOD] = METHOD_GET_MULTI_YUV;
        send_obj[MSG_KEY_ID] = id;
        send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
        send_obj[MSG_KEY_PARAMS] = param_obj;
        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
    } else {
        // save file and set for preview
        int received_frames = receive_yuv_data.size() / (pstHeader->size);
        saveYuvFile(receive_yuv_data, *pstHeader, received_frames);
        if (start_dump) {
            start_capture = false;
            receive_yuv_data.clear();
            emit sigCaptureYuvEnd();
        } else {
            img_viewer->setYUVImgData(receive_yuv_data, pstHeader);
            start_capture = false;
            receive_yuv_data.clear();
            yuv_cancel_btn->setEnabled(false);
            capture_busy_state = false;
            if (periodic_mode_active) {
                // schedule next capture after configured interval (do not re-enable UI or show modal)
                int interval_ms = time_edit->text().toInt() * 1000;
                QTimer::singleShot(interval_ms, this, &CaptureWindow::startTimingCaptureEvent);
            } else {
                enableComponents();
                stopTimingEvent();
                QMessageBox message(QMessageBox::NoIcon, "Notice", QString("Capture %1 yuv frames successfully!").arg(received_frames));
                message.exec();
            }
        }
    }
    if (!dumpAllFlag){
        emit fetchCompleted();
    }else{
        dumpAllFlag = false;
    }
    emit sigStatusLog("Capture YUV end", LOG_LEVEL::LOG_INFO);
}

void CaptureWindow::handleGetWDRFusionFrame()
{
    fusion_Frame = GlobalData::getInstance()->GetWDRFusionFrame();
    if (fusion_Frame == 1) {
        raw_mode_box->setCurrentIndex(0);
    } else if (fusion_Frame == 2) {
        raw_mode_box->setCurrentIndex(1);
    } else if (fusion_Frame == 3) {
        raw_mode_box->setCurrentIndex(2);
    } else if (fusion_Frame == 4) {
        raw_mode_box->setCurrentIndex(3);
    }
    raw_mode_box->setEnabled(false);

    qDebug("CaptureWindow::handleGetWDRFusionFrame, fusion_Frame:%d\n", fusion_Frame);
}

void CaptureWindow::handleCaptureRawImg()
{
    qDebug("CaptureWindow::handleCaptureRawImg\n");

    if (raw_frames_edit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("Please Input Frame Numbers!"), QMessageBox::Ok);
        return;
    }
    emit fetchStart();
    emit sigStatusLog("Start capture raw image", LOG_LEVEL::LOG_INFO);
    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        return;
    }

    if (!CheckSaveFilePath()) {
        return;
    }

    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);

    if (!start_dump && !periodic_mode_active) {
        disableComponents();
        raw_cancel_btn->setEnabled(true);
        dateTime_str = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
        directory_str = file_path->text() + "/raw_" + dateTime_str;
        QDir().mkdir(directory_str);
    }

    start_capture = true;
    total_frame = raw_frames_edit->text().toInt();
    current_frame = 0;
    recvSize = 0;
    rawSize = 0;
    receive_raw_data.clear();
    wdr_raw_data_list.clear();

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_CAPTURE_GET_MULTI_RAW;
    int id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

    QJsonObject param_obj;
    if (raw_tightly_checkbox->isChecked()) {
        param_obj["frames"] = total_frame;
        param_obj["tightly"] = true;
    } else {
        param_obj["frames"] = 1;
        param_obj["tightly"] = false;
    }
    if (dumpall_mode != MODE_AE10RAW_BATCH) {
        param_obj["dump_reg"] = true;
    } else {
        param_obj["dump_reg"] = false;
    }
    if (raw_checkbox->isChecked()) {
        param_obj["dump_raw"] = true;
    } else {
        param_obj["dump_raw"] = false;
    }
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_MULTI_RAW;
    send_obj[MSG_KEY_ID] = id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_PARAMS] = param_obj;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void CaptureWindow::handleMultiRawStatus(int id)
{
    qDebug("CaptureWindow::handleMultiRawStatus\n");

    QJsonObject obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    if(!start_capture) {
        return;
    }

    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();
    if (result_obj.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("Capture raw failed!"), QMessageBox::Ok);
        return;
    }

    // get logs
    QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();
    reg_dump_log = params_obj["Reg dump log"].toString();
    raw_info_log = params_obj["Raw info log"].toString();
    ae_log = params_obj["AE log"].toString();
    awb_log = params_obj["AWB log"].toString();
    rawSize = params_obj["Data size"].toInt();
    if (rawSize == 0) {
        rawHeader.bayerID = params_obj["Bayer ID"].toInt();
        rawHeader.iso = params_obj["Iso"].toInt();
        rawHeader.cropWidth = params_obj["Crop Width"].toInt();
        rawHeader.cropHeight = params_obj["Crop Height"].toInt();
    }

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_CAPTURE_GET_AE_BIN;
    int temp_id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(temp_id, id_info);

    QJsonObject param_obj;
    param_obj["content"] = AE_BIN_DATA;
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_BIN_INFO;
    send_obj[MSG_KEY_ID] = temp_id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_PARAMS] = param_obj;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void CaptureWindow::handleRecvBinInfo(int id)
{
    qDebug("CaptureWindow::handleRecvBinInfo\n");

    QJsonObject obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    if(!start_capture) {
        return;
    }

    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();
    if (result_obj.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("Get bin info failed!"), QMessageBox::Ok);
        return;
    }

    QJsonObject info_obj = obj[MSG_KEY_ID_INFO].toObject();
    QString cmd = info_obj[MSG_KEY_CMD].toString();
    QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();
    recvSize = params_obj["Data size"].toInt();

    BinDataType type = NONE_BIN_DATA;
    if (cmd == CMD_CAPTURE_GET_AE_BIN) {
        type = AE_BIN_DATA;
    } else if (cmd == CMD_CAPTURE_GET_AWB_BIN) {
        type = AWB_BIN_DATA;
    }

    NetworkClientHelper::getInstance()->setCallerInfo(recvSize, cmd);
    qDebug("Set receive size:%d\n", recvSize);

    int temp_id = GlobalData::getInstance()->getJsonrpcId();
    QJsonObject param_obj;
    param_obj["content"] = type;
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_BIN_DATA;
    send_obj[MSG_KEY_ID] = temp_id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_PARAMS] = param_obj;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void CaptureWindow::handleRecvBinData(QString cmd)
{
    qDebug("CaptureWindow::handleRecvBinData\n");

    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);
    QByteArray data = NetworkClientHelper::getInstance()->popCmdData(cmd);
    if(!start_capture) {
        return;
    }

    if (cmd == CMD_CAPTURE_GET_AE_BIN) {
        ae_bin = data;
        QJsonObject id_info;
        id_info[MSG_KEY_CMD] = CMD_CAPTURE_GET_AWB_BIN;
        int id = GlobalData::getInstance()->getJsonrpcId();
        NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

        QJsonObject param_obj;
        param_obj["content"] = AWB_BIN_DATA;
        QJsonObject send_obj;
        send_obj[MSG_KEY_METHOD] = METHOD_GET_BIN_INFO;
        send_obj[MSG_KEY_ID] = id;
        send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
        send_obj[MSG_KEY_PARAMS] = param_obj;
        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
    } else if (cmd == CMD_CAPTURE_GET_AWB_BIN) {
        awb_bin = data;
        if (rawSize == 0) {
            handleNoRawData();
            return;
        }
        NetworkClientHelper::getInstance()->setCallerInfo(rawSize, CMD_CAPTURE_GET_MULTI_RAW);
        qDebug("Set receive size:%d\n", rawSize);

        int id = GlobalData::getInstance()->getJsonrpcId();
        QJsonObject send_obj;
        send_obj[MSG_KEY_METHOD] = METHOD_GET_IMAGE_DATA;
        send_obj[MSG_KEY_ID] = id;
        send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
    }
}

void CaptureWindow::handleNoRawData()
{
    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);
    QByteArray data;
    int mode = (raw_mode_box->currentIndex() > 0) ? WDR_RAW : LINEAR_RAW;
    int frame = raw_frames_edit->text().toInt();

    saveRawFile(data, rawHeader, mode, frame);
    start_capture = false;
    current_frame = 0;
    total_frame = 0;
    enableComponents();
    if (start_dump) {
        start_dump = false;
        dump_cancel_btn->setEnabled(false);
        QMessageBox message(QMessageBox::NoIcon, "Notice", "Dump all successfully!");
        message.exec();
    } else {
        raw_cancel_btn->setEnabled(false);
        stopTimingEvent();
        capture_busy_state = false;
        QMessageBox message(QMessageBox::NoIcon, "Notice", "Capture raw successfully!");
        message.exec();
    }
    emit fetchCompleted();
    emit sigStatusLog("Capture no raw successfully!", LOG_LEVEL::LOG_INFO);
}

void CaptureWindow::slotGotRawImg(QString cmd)
{
    qDebug("CaptureWindow::slotGotRawImg\n");

    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);
    QByteArray data = NetworkClientHelper::getInstance()->popCmdData(cmd);
    if(!start_capture) {
        return;
    }

    bool remain = false;
    int received_frames = 0;
    int step = 0;

    QByteArray header = data.left(sizeof(RAW_HEADER));
    RAW_HEADER *pstHeader = (RAW_HEADER *) header.data();
    data.remove(0, sizeof(RAW_HEADER));

    qDebug("numframe:%d, curframe:%d, width:%d, height:%d,  compress:%d",
           pstHeader->numFrame, pstHeader->curFrame, pstHeader->width, pstHeader->height, pstHeader->compress);
    qDebug("fusionFrame:%d, bayerid:%d, cropX:%d, cropY:%d, cropWidth:%d, cropHeight:%d", pstHeader->fusionFrame,
           pstHeader->bayerID, pstHeader->cropX, pstHeader->cropY, pstHeader->cropWidth, pstHeader->cropHeight);
    qDebug("data.size:%d, pstHeader->size:%d", data.size(), pstHeader->size);

    if (data.size() != (int)pstHeader->size) {
        QMessageBox::warning(this, tr("Error"), tr("Capture Tool, The received raw data size is wrong!"), QMessageBox::Ok);
        return;
    }

    //crop & unpack
    RAW_INFO rawInfo;
    rawInfo.width = pstHeader->width;
    int tileIdx = 0;
    bool tileMode = utils::isTileMode(rawInfo, RAW_NORMAL, &tileIdx);
    QByteArray d = utils::cropUnpackRaw(data, *pstHeader, tileMode, tileIdx);
    if (d.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("Unpack Raw failed!"), QMessageBox::Ok);
        return;
    }

    step = pstHeader->fusionFrame;
    if (step == 1) {
        receive_raw_data.append(d);
    } else {
        wdr_raw_data_list.append(d);
    }

    if(pstHeader->curFrame < pstHeader->numFrame - 1) {
        remain = true;
    } else if (pstHeader->curFrame == pstHeader->numFrame - 1) {
        remain = false;
    }

    if (remain) {
        //call for remain frames
        NetworkClientHelper::getInstance()->setCallerInfo(rawSize, CMD_CAPTURE_GET_MULTI_RAW);
        qDebug("Set receive size:%d\n", rawSize);

        int id = GlobalData::getInstance()->getJsonrpcId();
        QJsonObject send_obj;
        send_obj[MSG_KEY_METHOD] = METHOD_GET_IMAGE_DATA;
        send_obj[MSG_KEY_ID] = id;
        send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
    } else if (!raw_tightly_checkbox->isChecked() && (current_frame < total_frame * step - step)) {
        current_frame += step;
        QJsonObject id_info;
        id_info[MSG_KEY_CMD] = CMD_CAPTURE_GET_MULTI_RAW;
        int id = GlobalData::getInstance()->getJsonrpcId();
        NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

        QJsonObject param_obj;
        param_obj["frames"] = 1;
        param_obj["tightly"] = false;
        if (dumpall_mode != MODE_AE10RAW_BATCH) {
            param_obj["dump_reg"] = true;
        } else {
            param_obj["dump_reg"] = false;
        }
        if (raw_checkbox->isChecked()) {
            param_obj["dump_raw"] = true;
        } else {
            param_obj["dump_raw"] = false;
        }
        QJsonObject send_obj;
        send_obj[MSG_KEY_METHOD] = METHOD_GET_MULTI_RAW;
        send_obj[MSG_KEY_ID] = id;
        send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
        send_obj[MSG_KEY_PARAMS] = param_obj;
        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
    } else {
        if (step == 1) {
            received_frames = receive_raw_data.size() / (pstHeader->cropWidth * pstHeader->cropHeight * 2);
            saveRawFile(receive_raw_data, *pstHeader, LINEAR_RAW, received_frames);
            img_viewer->setRawImgData(receive_raw_data, pstHeader->cropWidth, pstHeader->cropHeight,
                                       (utils::RAW_BAYER_ID) pstHeader->bayerID, utils::RAW_BITS::BITS_12, utils::RAW_PACK_TYPE::RAW_UNPACKED);
            receive_raw_data.clear();
        } else {
            QByteArray wdr_data, wdr_le_data, wdr_se_data;
            int frame_num = wdr_raw_data_list.count();
            int raw_line_byte = pstHeader->cropWidth * 2;
            char *line = new char[raw_line_byte];

            received_frames = frame_num / step;
            memset(line, 0, raw_line_byte);
            for (int i = 0; i < frame_num - 1; i += 2) {
                char *le = wdr_raw_data_list[i].data();
                char *se = wdr_raw_data_list[i + 1].data();
                int frame_size = wdr_raw_data_list[i].size();
                for (int j = 0; j < frame_size; j += raw_line_byte) {
                    memcpy(line, le+j, raw_line_byte);
                    wdr_data.append(line, raw_line_byte);
                    memcpy(line, se+j, raw_line_byte);
                    wdr_data.append(line, raw_line_byte);
                }
            }
            saveRawFile(wdr_data, *pstHeader, WDR_RAW, received_frames);

            if (wdr_raw_separately_checkbox->isChecked() && (step == 2)) {
                for (int i = 0; i < frame_num - 1; i += 2) {
                    int frame_size = wdr_raw_data_list[i].size();
                    wdr_le_data.append(wdr_raw_data_list[i].data(), frame_size);
                    wdr_se_data.append(wdr_raw_data_list[i + 1].data(), frame_size);
                }
                saveRawFile(wdr_le_data, *pstHeader, WDR_LE_RAW, received_frames);
                saveRawFile(wdr_se_data, *pstHeader, WDR_SE_RAW, received_frames);
            }

            img_viewer->setRawImgData(wdr_data, pstHeader->cropWidth*2, pstHeader->cropHeight,
                                       (utils::RAW_BAYER_ID) pstHeader->bayerID, utils::RAW_BITS::BITS_12, utils::RAW_PACK_TYPE::RAW_UNPACKED);
            wdr_raw_data_list.clear();
            delete [] line;
        }
        start_capture = false;
        if (start_dump) {
            if (!dump_all_repeatedly) {
                start_dump = false;
                dump_cancel_btn->setEnabled(false);
                enableComponents();
                if (dumpall_mode != MODE_AE10RAW_BATCH) {
                    stopTimingEvent();
                    capture_busy_state = false;
                    QMessageBox message(QMessageBox::NoIcon, "Notice", QString("Dump all successfully!"));
                    message.exec();
                } else {
                    busy_flag = false;
                }
            } else {
                int n = interval_edit->text().toInt();
                for (int i = n-1; i >= 0; i--) {
                    interval_edit->setText(QString::number(i));
                    this->repaint();
                    Sleep(1000);
                    if (!dump_all_repeatedly) {
                        break;
                    }
                }

                interval_edit->setText(QString::number(n));
                emit sigCaptureRawEnd();
            }
        } else {
            // For periodic mode reuse the same folder and schedule next capture
            if (periodic_mode_active) {
                raw_cancel_btn->setEnabled(false);
                capture_busy_state = false;
                int interval_ms = time_edit->text().toInt() * 1000;
                QTimer::singleShot(interval_ms, this, &CaptureWindow::startTimingCaptureEvent);
            } else {
                enableComponents();
                raw_cancel_btn->setEnabled(false);
                stopTimingEvent();
                capture_busy_state = false;
                QMessageBox message(QMessageBox::NoIcon, "Notice", QString("Capture %1 raw frames successfully!").arg(received_frames));
                message.exec();
            }
        }
    }
    emit fetchCompleted();
     emit sigStatusLog("Capture raw end!", LOG_LEVEL::LOG_INFO);
}

void CaptureWindow::handleDumpAll(DUMPALL_MODE mode, QString dirPath, QString fileName)
{
    qDebug("CaptureWindow::handleDumpAll");

    if (yuv_frames_edit->text().isEmpty() || raw_frames_edit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("Please Input Frame Numbers!"), QMessageBox::Ok);
        return;
    }

    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        return;
    }

    if (!CheckSaveFilePath()) {
        return;
    }

    if (dump_all_repeatedly_checkbox->isChecked()) {
        dump_all_repeatedly = true;
    }

    start_dump = true;
    disableComponents();
    dump_cancel_btn->setEnabled(true);
    dumpall_mode = mode;
    if (!periodic_mode_active) {
        dateTime_str = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    }
    if (dumpall_mode != MODE_AE10RAW_BATCH) {
        if (!periodic_mode_active) {
            directory_str = file_path->text() + "/dump_" + dateTime_str;
        }
    } else {
        busy_flag = true;
        file_path->setText(dirPath);
        directory_str = dirPath;
        file_name_linear = fileName;
        file_name_wdr = fileName;
        file_name_wdr_le = fileName;
        file_name_wdr_se = fileName;
    }
    if (!directory_str.isEmpty()) {
        QDir().mkdir(directory_str);
    }
    if (dumpall_mode != MODE_AE10RAW_BATCH) {
        // Todo: update saveRaramFromBoard to new param json
        // MainWindow::getInstance()->saveParamFromBoard(directory_str);
    }
    dumpAllFlag = true;
    handleCaptureYuvImg();
}

void CaptureWindow::handleCancelEvent()
{
    stopCapture();
    enableComponents();
    raw_cancel_btn->setEnabled(false);
    yuv_cancel_btn->setEnabled(false);
    dump_cancel_btn->setEnabled(false);
    start_capture = false;
    start_dump = false;
    dump_all_repeatedly = false;
    stopTimingEvent();
    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);
}

void CaptureWindow::startTimingEvent()
{
    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        QMessageBox::warning(this, tr("Warning"), tr("Network is not connected!"), QMessageBox::Ok);
        return;
    }

    disableComponents();
    stop_btn->setEnabled(true);

    // If user wants to reuse the same folder for periodic captures, create it once here
    if (use_same_folder_checkbox && use_same_folder_checkbox->isChecked()) {
        periodic_mode_active = true;
        dateTime_str = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
        // create a generic periodic folder
        directory_str = file_path->text() + "/periodic_" + dateTime_str;
        QDir().mkdir(directory_str);
        // reset sequence counter for this periodic session
        periodic_file_seq = 1;
    } else {
        periodic_mode_active = false;
    }

    start_timer->setInterval(time_edit->text().toInt() * 1000);
    start_timer->start();
}

void CaptureWindow::stopTimingEvent()
{
    start_timer->stop();
    stop_btn->setEnabled(false);
    type_cap_src_box->setEnabled(true);
    time_edit->setEnabled(true);
    start_btn->setEnabled(true);
    capture_busy_state = false;
    // leave directory_str as-is; periodic mode stops
    periodic_mode_active = false;
}

void CaptureWindow::startTimingCaptureEvent()
{
    if (capture_busy_state) {
        return;
    }

    switch (type_cap_src_box->currentIndex()) {
        case 0:
            handleCaptureYuvImg();
            break;

        case 1:
            handleCaptureRawImg();
            break;

        case 2:
            handleDumpAll();
            break;
    }
    capture_busy_state = true;
}

void CaptureWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        this->close();
    }
}

void CaptureWindow::slotCheckRawParams(QString text)
{
    if (text == "LINEAR") {
        raw_frames_tips->setText("(1-90)");
    } else if (text == "WDR_2TO1") {
        if (raw_frames_edit->text().toInt() > 45) {
            raw_frames_edit->setText("45");
        }
        raw_frames_tips->setText("(1-45)");
    } else if (text == "WDR_3TO1") {
        if (raw_frames_edit->text().toInt() > 30) {
            raw_frames_edit->setText("30");
        }
        raw_frames_tips->setText("(1-30)");
    } else if (text == "WDR_4TO1") {
        if (raw_frames_edit->text().toInt() > 20) {
            raw_frames_edit->setText("20");
        }
        raw_frames_tips->setText("(1-20)");
    }
}

void CaptureWindow::OpenSaveFilePath()
{
    shared_ptr<GlobalData> global_data = GlobalData::getInstance();
    QString open_dir = global_data->getSettings(SETTINGS_SECTION, KEY_CAPTURE_SAVE_PATH).toString();
    QString dir_path;
    if (!open_dir.isEmpty()) {
        QDir dir(open_dir);
        if(dir.exists()) {
            dir_path = QFileDialog::getExistingDirectory(this, "Please Select Image Save Path", open_dir);
        } else {
            dir_path = QFileDialog::getExistingDirectory(this, "Please Select Image Save Path", "./");
        }
    } else {
        dir_path = QFileDialog::getExistingDirectory(this, "Please Select Image Save Path", "./");
    }

    qDebug("Opened Save Path: %s", qUtf8Printable(dir_path));
    if (!dir_path.isEmpty()) {
        file_path->setText(dir_path);
        global_data->setSettings(SETTINGS_SECTION, KEY_CAPTURE_SAVE_PATH, dir_path);
        global_data->saveSettings();
    }
}

int CaptureWindow::validateValue(int v, int min, int max)
{
    if (v < min) {
        return min;
    } else if (v > max) {
        return max;
    }

    return v;
}

void CaptureWindow::cellEdited(int idx)
{
    int value = 0;
    int max = 90;

    if (idx == 0) {
        if (yuv_frames_edit->text().isEmpty()) {
            return;
        }
        value = yuv_frames_edit->text().toInt();
        yuv_frames_edit->setText(QString::number(validateValue(value, 1, max)));
    } else if (idx == 1) {
        if (raw_frames_edit->text().isEmpty()) {
            return;
        }
        if (raw_mode_box->currentText() == "WDR_2TO1") {
            max = 45;
        } else if (raw_mode_box->currentText() == "WDR_3TO1") {
            max = 30;
        } else if (raw_mode_box->currentText() == "WDR_4TO1") {
            max = 20;
        }
        value = raw_frames_edit->text().toInt();
        raw_frames_edit->setText(QString::number(validateValue(value, 1, max)));
    } else if (idx == 2) {
        max = 300;
        value = interval_edit->text().toInt();
        interval_edit->setText(QString::number(validateValue(value, 0, max)));
    } else if (idx == 3) {
        max = 60;
        value = time_edit->text().toInt();
        time_edit->setText(QString::number(validateValue(value, 1, max)));
    }
}

void CaptureWindow::stopCapture()
{
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_STOP_GET_IMAGE_DATA;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void CaptureWindow::disableComponents()
{
    yuv_cap_src_box->setEnabled(false);
    yuv_capture_btn->setEnabled(false);
    yuv_frames_edit->setEnabled(false);
    raw_capture_btn->setEnabled(false);
    raw_frames_edit->setEnabled(false);
    dump_btn->setEnabled(false);
    dump_all_repeatedly_checkbox->setEnabled(false);
    interval_edit->setEnabled(false);
    open_btn->setEnabled(false);
    file_path->setEnabled(false);
    wdr_raw_separately_checkbox->setEnabled(false);
    raw_tightly_checkbox->setEnabled(false);
    save_bmp_file_checkbox->setEnabled(false);
    img_viewer->setEnabled(false);
    start_btn->setEnabled(false);
    time_edit->setEnabled(false);
    type_cap_src_box->setEnabled(false);
    raw_checkbox->setEnabled(false);

}

void CaptureWindow::enableComponents()
{
    yuv_cap_src_box->setEnabled(true);
    yuv_capture_btn->setEnabled(true);
    yuv_frames_edit->setEnabled(true);
    raw_capture_btn->setEnabled(true);
    raw_frames_edit->setEnabled(true);
    open_btn->setEnabled(true);
    file_path->setEnabled(true);
    wdr_raw_separately_checkbox->setEnabled(true);
    raw_tightly_checkbox->setEnabled(true);
    save_bmp_file_checkbox->setEnabled(true);
    img_viewer->setEnabled(true);
    dump_btn->setEnabled(true);
    dump_all_repeatedly_checkbox->setEnabled(true);
    if (dump_all_repeatedly_checkbox->isChecked()) {
        interval_edit->setEnabled(true);
    }
   start_btn->setEnabled(true);
   time_edit->setEnabled(true);
   type_cap_src_box->setEnabled(true);
   raw_checkbox->setEnabled(true);
}

void CaptureWindow::handleReadAllFinished()
{
    // save param from board and then restore original param
    qDebug("CaptureWindow::handleReadAllFinished");
    QString template_string = GlobalData::getInstance()->getCvipqToolTemplate();
    MainWindow::getInstance()->saveFile(directory_str + "/" + template_string + "_" + dateTime_str +".json");
    MainWindow::getInstance()->openFile(directory_str + "/temp.json", false);
    QDir dir;
    dir.remove(directory_str + "/temp.json");
}
