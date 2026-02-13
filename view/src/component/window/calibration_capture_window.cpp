#include "calibration_capture_window.hpp"
#include "raw_image_format_dialog.hpp"
#include "color_block_detect.hpp"
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


#define BLC_NOTE_STR ( "Note:\n1.请先把镜头遮住；\n")
#define LSC_AWB_NOTE_STR ("Note:\n1.暗房关灯；\n2.镜头紧贴大灯箱中心；\n\
3.分别拍摄5种灯光，D75、D65、D50、TL84、A光源；\n4.操作过程需要使用照度计实际测量色温并填写；\n\
5.raw亮度不合适可修改'Luma'值再重试。\n")
#define CCM_NOTE_STR ("Note:\n1.暗房关灯；\n2.大灯箱,镜头平行24色卡进行拍摄，色卡占画面70%；\n\
3.分别拍摄5种灯光，D75、D65、D50、TL84、A光源；\n4.操作过程需要使用照度计实际测量色温并填写；\n\
5.raw亮度不合适可修改'Luma'值再重试。\n")
#define NOISE_NOTE_STR ("Note:\n1.实验室关灯；\n2.大灯箱，镜头平行24色卡进行拍摄，色卡占画面70%；\n\
3.建议尽量使用D65光源；\n4.raw亮度不合适可修改'Luma'值再重试。\n")

const QStringList iso_str_list = {"ISO100", "ISO200", "ISO400", "ISO800", "ISO1600", "ISO3200", "ISO6400", "ISO12800"
    , "ISO25600","ISO51200", "ISO102400", "ISO204800", "ISO409600", "ISO819200", "ISO1638400", "ISO3276800"};

const int weight[15][17] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0},
    {0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0},
    {0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0},
    {0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0},
    {0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0},
    {0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0},
    {0,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

#define MIN_WINDOW_WIDTH (1080)
#define MIN_WINDOW_HEIGHT (720)

#define MODE_LINEAR 0
#define MODE_WDR 1
#define STD_IMAGE_WIDTH (1920)
#define STD_IMAGE_HEIGHT (1080)

#define RAW_LINE_BYTE (3840)
#define CV183X_LINE_BUFFER_WIDTH 2688
#define CV183X_TILE_START_INDEX 1920
#define CV182X_LINE_BUFFER_WIDTH 2304
#define CV182X_TILE_START_INDEX 1536

#define NP_CAPTURE_FRAMES 20

CalibrationCaptureWindow::CalibrationCaptureWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle(tr("Capture Raw"));
    setWindowFlags(Qt::WindowMaximizeButtonHint|Qt::WindowCloseButtonHint);
    timer = new QTimer(this);
    initUI();
    initEvent();
}

void CalibrationCaptureWindow::initUI()
{
    main_widget = new QWidget();
    main_widget->setMinimumSize(QSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT));
    main_layout = new QHBoxLayout();
    left_layout = new QGridLayout();

    QHBoxLayout *module_layout = new QHBoxLayout;
    QLabel *module_lab = new QLabel("Module Select:");
    module_lab->setFixedSize(120, 25);
    QStringList module_str_list = {"BLC", "LSC&AWB", "CCM", "NOISE"};
    module_box = new QComboBox;
    module_box->addItems(module_str_list);
    module_box->setProperty("module", "BLC");
    module_box->setFixedSize(120,25);
    connect(module_box, &QComboBox::currentTextChanged, this, &CalibrationCaptureWindow::selectedModuleChanged);
    module_layout->addWidget(module_lab, Qt::AlignLeft);
    module_layout->addWidget(module_box, Qt::AlignLeft);

    note_edit = new QPlainTextEdit;
    note_edit->setReadOnly(true);
    note_edit->setFixedHeight(150);
    note_edit->appendPlainText(BLC_NOTE_STR);

    blc_group = createBlcGroupbox();
    lsc_awb_group = createLscAwbGroupbox();
    ccm_group = createCcmGroupbox();
    noise_group = createNoiseGroupbox();

    img_viewer = new MultiImageViewerWidget();
    img_viewer->setFilePathHidden();
    img_viewer->showLevelMeterBtn();
    img_viewer->showRawStatisticBtn();
    image_label = new ImageLabel(this, ImageLabel::NORMAL);
    connect(image_label, &ImageLabel::sigPositionChange, img_viewer, &MultiImageViewerWidget::slotPositionChange);
    img_viewer->setImageLabel(image_label);
    img_viewer->setMinimumHeight(600);

    QString exe_dir = QCoreApplication::applicationDirPath();
    open_btn = new QPushButton(tr("Save Path"));
    file_path = new QLineEdit();
    file_path->setReadOnly(true);
    file_path->setText(exe_dir);
    QHBoxLayout *filepath_layout = new QHBoxLayout;
    filepath_layout->addWidget(open_btn);
    filepath_layout->addWidget(file_path);
    connect(open_btn, &QPushButton::clicked, this, &CalibrationCaptureWindow::OpenSaveFilePath);


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

    left_layout->addLayout(module_layout, 0, 0, Qt::AlignTop);
    left_layout->addLayout(filepath_layout, 1, 0, Qt::AlignTop);
    left_layout->addWidget(note_edit, 2, 0, Qt::AlignTop);
    left_layout->addWidget(blc_group, 3, 0, Qt::AlignTop);
    left_layout->addLayout(log_layout, 4, 0, Qt::AlignTop);
    left_layout->setSizeConstraint(QLayout::SizeConstraint::SetFixedSize);

    main_layout->addLayout(left_layout, 1);
    main_layout->addWidget(img_viewer, 2);

    setCentralWidget(main_widget);
    centralWidget()->setLayout(main_layout);
}

void CalibrationCaptureWindow::initEvent()
{
    shared_ptr<NetworkClientHelper> network_helper = NetworkClientHelper::getInstance();
    connect(network_helper.get(), &NetworkClientHelper::sigCaliRawStatus, this, &CalibrationCaptureWindow::handleRawStatus);
    connect(network_helper.get(), &NetworkClientHelper::sigCaliBinInfo, this, &CalibrationCaptureWindow::handleRecvBinInfo);
    connect(network_helper.get(), &NetworkClientHelper::sigCaliBinData, this, &CalibrationCaptureWindow::handleRecvBinData);
    connect(network_helper.get(), &NetworkClientHelper::sigCaliRawData, this, &CalibrationCaptureWindow::handleRawData);
    connect(GlobalData::getInstance().get(), &GlobalData::sigModuleRWResult, this, &CalibrationCaptureWindow::handleModuleRWResult);
    connect(timer, &QTimer::timeout, this, &CalibrationCaptureWindow::handleTimeOutEvent);
}

QGroupBox *CalibrationCaptureWindow::createBlcGroupbox()
{
    QGroupBox * groupbox = new QGroupBox();
    groupbox->setFixedHeight(100);
    QVBoxLayout *layout = new QVBoxLayout;

    QHBoxLayout *iso_layout = new QHBoxLayout;
    QLabel *iso_lab = new QLabel("Max ISO:");
    iso_lab->setFixedSize(110, 25);
    blc_iso_box = new QComboBox;
    blc_iso_box->setFixedSize(110, 25);
    blc_iso_box->addItems(iso_str_list);
    blc_iso_box->setCurrentIndex(15);
    blc_check_btn = new QPushButton(tr("Check"));
    blc_check_btn->setFixedSize(110, 25);
    connect(blc_check_btn, &QPushButton::clicked,
            this, &CalibrationCaptureWindow::handleBlcCheckClicked);
    iso_layout->addWidget(iso_lab);
    iso_layout->addWidget(blc_iso_box);
    iso_layout->addWidget(blc_check_btn);

    QHBoxLayout *operate_layout = new QHBoxLayout;
    blc_capture_btn = new QPushButton(tr("Capture"));
    blc_capture_btn->setFixedSize(110, 25);
    connect(blc_capture_btn, &QPushButton::clicked,
            this, &CalibrationCaptureWindow::handleBlcCaptureClicked);
    blc_cancel_btn = new QPushButton("Cancel");
    blc_cancel_btn->setFixedSize(110, 25);
    blc_cancel_btn->setEnabled(false);
    connect(blc_cancel_btn, &QPushButton::clicked,
            this, &CalibrationCaptureWindow::handleBlcCancelClicked);
    blc_reset_btn = new QPushButton(tr("Reset"));
    blc_reset_btn->setFixedSize(110, 25);
    connect(blc_reset_btn, &QPushButton::clicked,
            this, &CalibrationCaptureWindow::handleBlcResetClicked);

    operate_layout->addWidget(blc_capture_btn);
    operate_layout->addWidget(blc_cancel_btn);
    operate_layout->addWidget(blc_reset_btn);
    operate_layout->addStretch(1);

    layout->addLayout(iso_layout);
    layout->addLayout(operate_layout);
    groupbox->setLayout(layout);
    return groupbox;
}

QGroupBox *CalibrationCaptureWindow::createLscAwbGroupbox()
{
    QGroupBox * groupbox = new QGroupBox();
    groupbox->setFixedHeight(150);
    QVBoxLayout *layout = new QVBoxLayout;

    QHBoxLayout *light_layout = new QHBoxLayout;
    QLabel *light_lab = new QLabel("Light Source:");
    light_lab->setFixedSize(120, 25);
    QStringList light_str_list = {"D75", "D65", "D50", "TL84", "A"};
    lsc_awb_light_box = new QComboBox;
    lsc_awb_light_box->addItems(light_str_list);
    lsc_awb_light_box->setFixedSize(120, 25);
    connect(lsc_awb_light_box, &QComboBox::currentTextChanged, [=]{
        int idx = lsc_awb_light_box->currentIndex();
        selectedLightChanged(idx, 0);
    });
    light_layout->addWidget(light_lab);
    light_layout->addWidget(lsc_awb_light_box);
    light_layout->addStretch(1);

    QHBoxLayout *ct_layout = new QHBoxLayout;
    QLabel *ct_lab = new QLabel("Color Temp(K):");
    ct_lab->setFixedSize(120, 25);
    lsc_awb_ct_edit = new QLineEdit;
    lsc_awb_ct_edit->setText("0");
    lsc_awb_ct_edit->setFixedSize(120, 25);
    connect(lsc_awb_ct_edit, &QLineEdit::editingFinished, [=]{
        int ct = lsc_awb_ct_edit->text().toInt();
        ct = qBound(0, ct, 999999);
        lsc_awb_ct_edit->setText(QString::number(ct));
        int idx = lsc_awb_light_box->currentIndex();
        lsc_awb_ct[idx] = ct;
    });
    ct_layout->addWidget(ct_lab);
    ct_layout->addWidget(lsc_awb_ct_edit);
    ct_layout->addStretch(1);

    QHBoxLayout *luma_layout = new QHBoxLayout;
    QLabel *luma_lab = new QLabel("Luma:");
    luma_lab->setFixedSize(120, 25);
    lsc_awb_luma_edit = new QLineEdit;
    lsc_awb_luma_edit->setMaxLength(8);
    lsc_awb_luma_edit->setText("170");
    lsc_awb_luma_edit->setFixedSize(120, 25);
    connect(lsc_awb_luma_edit, &QLineEdit::editingFinished, [=]{
        int luma = lsc_awb_luma_edit->text().toInt();
        luma = qBound(0, luma, 255);
        lsc_awb_luma_edit->setText(QString::number(luma));
    });
    luma_layout->addWidget(luma_lab);
    luma_layout->addWidget(lsc_awb_luma_edit);
    luma_layout->addStretch(1);

    QHBoxLayout *capture_layout = new QHBoxLayout;
    lsc_awb_capture_btn = new QPushButton("Capture");
    lsc_awb_capture_btn->setFixedSize(115, 25);
    connect(lsc_awb_capture_btn, &QPushButton::clicked,
            this, &CalibrationCaptureWindow::handleLscAwbCaptureClicked);
    lsc_awb_cancel_btn = new QPushButton("Cancel");
    lsc_awb_cancel_btn->setFixedSize(115, 25);
    lsc_awb_cancel_btn->setEnabled(false);
    connect(lsc_awb_cancel_btn, &QPushButton::clicked,
            this, &CalibrationCaptureWindow::handleLscAwbCancelClicked);
    lsc_awb_reset_btn = new QPushButton(tr("Reset"));
    lsc_awb_reset_btn->setFixedSize(115, 25);
    connect(lsc_awb_reset_btn, &QPushButton::clicked,
            this, &CalibrationCaptureWindow::handleLscAwbResetClicked);
    capture_layout->addWidget(lsc_awb_capture_btn);
    capture_layout->addWidget(lsc_awb_cancel_btn);
    capture_layout->addWidget(lsc_awb_reset_btn);
    capture_layout->addStretch(1);

    layout->addLayout(light_layout);
    layout->addLayout(ct_layout);
    layout->addLayout(luma_layout);
    layout->addLayout(capture_layout);

    groupbox->setLayout(layout);
    return groupbox;
}

QGroupBox *CalibrationCaptureWindow::createCcmGroupbox()
{
    QGroupBox * groupbox = new QGroupBox();
    groupbox->setFixedHeight(150);
    QVBoxLayout *layout = new QVBoxLayout;

    QHBoxLayout *light_layout = new QHBoxLayout;
    QLabel *light_lab = new QLabel("Light Source:");
    light_lab->setFixedSize(120, 25);
    QStringList light_str_list = {"D75", "D65", "D50", "TL84", "A"};
    ccm_light_box = new QComboBox;
    ccm_light_box->addItems(light_str_list);
    ccm_light_box->setFixedSize(120, 25);
    connect(ccm_light_box, &QComboBox::currentTextChanged, [=]{
        int idx = ccm_light_box->currentIndex();
        selectedLightChanged(idx, 1);
    });
    light_layout->addWidget(light_lab);
    light_layout->addWidget(ccm_light_box);
    light_layout->addStretch(1);

    QHBoxLayout *ct_layout = new QHBoxLayout;
    QLabel *ct_lab = new QLabel("Color Temp(K):");
    ct_lab->setFixedSize(120, 25);
    ccm_ct_edit = new QLineEdit;
    ccm_ct_edit->setFixedSize(120, 25);
    ccm_ct_edit->setText("0");
    connect(ccm_ct_edit, &QLineEdit::editingFinished, [=]{
        int ct = ccm_ct_edit->text().toInt();
        ct = qBound(0, ct, 999999);
        ccm_ct_edit->setText(QString::number(ct));
        int idx = ccm_light_box->currentIndex();
        ccm_ct[idx] = ct;
    });
    ct_layout->addWidget(ct_lab);
    ct_layout->addWidget(ccm_ct_edit);
    ct_layout->addStretch(1);

    QHBoxLayout *luma_layout = new QHBoxLayout;
    QLabel *luma_lab = new QLabel("Luma:");
    luma_lab->setFixedSize(120, 25);
    ccm_luma_edit = new QLineEdit;
    ccm_luma_edit->setMaxLength(8);
    ccm_luma_edit->setFixedSize(120, 25);
    ccm_luma_edit->setText("70");
    connect(ccm_luma_edit, &QLineEdit::editingFinished, [=]{
        int luma = ccm_luma_edit->text().toInt();
        luma = qBound(0, luma, 255);
        ccm_luma_edit->setText(QString::number(luma));
    });
    luma_layout->addWidget(luma_lab);
    luma_layout->addWidget(ccm_luma_edit);
    luma_layout->addStretch(1);

    QHBoxLayout *capture_layout = new QHBoxLayout;
    ccm_capture_btn = new QPushButton("Capture");
    ccm_capture_btn->setFixedSize(115, 25);
    connect(ccm_capture_btn, &QPushButton::clicked,
            this, &CalibrationCaptureWindow::handleCcmCaptureClicked);
    ccm_cancel_btn = new QPushButton("Cancel");
    ccm_cancel_btn->setFixedSize(115, 25);
    ccm_cancel_btn->setEnabled(false);
    connect(ccm_cancel_btn, &QPushButton::clicked,
            this, &CalibrationCaptureWindow::handleCcmCancelClicked);
    ccm_reset_btn = new QPushButton(tr("Reset"));
    ccm_reset_btn->setFixedSize(115, 25);
    connect(ccm_reset_btn, &QPushButton::clicked,
            this, &CalibrationCaptureWindow::handleCcmResetClicked);
    capture_layout->addWidget(ccm_capture_btn);
    capture_layout->addWidget(ccm_cancel_btn);
    capture_layout->addWidget(ccm_reset_btn);
    capture_layout->addStretch(1);

    layout->addLayout(light_layout);
    layout->addLayout(ct_layout);
    layout->addLayout(luma_layout);
    layout->addLayout(capture_layout);

    groupbox->setLayout(layout);
    return groupbox;
}

QGroupBox *CalibrationCaptureWindow::createNoiseGroupbox()
{
    QGroupBox * groupbox = new QGroupBox();
    groupbox->setFixedHeight(120);
    QVBoxLayout *layout = new QVBoxLayout;

    QHBoxLayout *iso_layout = new QHBoxLayout;
    QLabel *iso_lab = new QLabel("Max ISO:");
    iso_lab->setFixedSize(110, 25);
    noise_iso_box = new QComboBox;
    noise_iso_box->setFixedSize(110, 25);
    noise_iso_box->addItems(iso_str_list);
    noise_iso_box->setCurrentIndex(15);
    iso_layout->addWidget(iso_lab);
    iso_layout->addWidget(noise_iso_box);
    iso_layout->addStretch(1);

    QHBoxLayout *luma_layout = new QHBoxLayout;
    QLabel *luma_lab = new QLabel("Luma:");
    luma_lab->setFixedSize(110, 25);
    noise_luma_edit = new QLineEdit;
    noise_luma_edit->setMaxLength(8);
    noise_luma_edit->setFixedSize(110, 25);
    noise_luma_edit->setText("58");
    connect(noise_luma_edit, &QLineEdit::editingFinished, [=]{
        int luma = noise_luma_edit->text().toInt();
        luma = qBound(0, luma, 255);
        noise_luma_edit->setText(QString::number(luma));
    });
    luma_layout->addWidget(luma_lab);
    luma_layout->addWidget(noise_luma_edit);
    luma_layout->addStretch(1);

    QHBoxLayout *operate_layout = new QHBoxLayout;
    noise_capture_btn = new QPushButton(tr("Capture"));
    noise_capture_btn->setFixedSize(110, 25);
    connect(noise_capture_btn, &QPushButton::clicked,
            this, &CalibrationCaptureWindow::handleNoiseCaptureClicked);
    noise_cancel_btn = new QPushButton("Cancel");
    noise_cancel_btn->setFixedSize(110, 25);
    noise_cancel_btn->setEnabled(false);
    connect(noise_cancel_btn, &QPushButton::clicked,
            this, &CalibrationCaptureWindow::handleNoiseCancelClicked);
    noise_reset_btn = new QPushButton(tr("Reset"));
    noise_reset_btn->setFixedSize(110, 25);
    connect(noise_reset_btn, &QPushButton::clicked,
            this, &CalibrationCaptureWindow::handleNoiseResetClicked);

    operate_layout->addWidget(noise_capture_btn);
    operate_layout->addWidget(noise_cancel_btn);
    operate_layout->addWidget(noise_reset_btn);

    layout->addLayout(iso_layout);
    layout->addLayout(luma_layout);
    layout->addLayout(operate_layout);
    groupbox->setLayout(layout);
    return groupbox;
}

void CalibrationCaptureWindow::selectedModuleChanged(QString text)
{
    QString module = module_box->property("module").toString();
    if (module == text) {
        return;
    }

    if (module == "BLC") {
        left_layout->removeWidget(blc_group);
        blc_group->hide();
    } else if (module == "LSC&AWB") {
        left_layout->removeWidget(lsc_awb_group);
        lsc_awb_group->hide();
    } else if (module == "CCM") {
        left_layout->removeWidget(ccm_group);
        ccm_group->hide();
    } else if (module == "NOISE") {
        left_layout->removeWidget(noise_group);
        noise_group->hide();
    }

    note_edit->clear();

    if (text == "BLC") {
        left_layout->addWidget(blc_group, 3, 0, Qt::AlignTop);
        blc_group->show();
        module_box->setProperty("module", "BLC");
        note_edit->appendPlainText(BLC_NOTE_STR);
    } else if (text == "LSC&AWB") {
        left_layout->addWidget(lsc_awb_group, 3, 0, Qt::AlignTop);
        lsc_awb_group->show();
        module_box->setProperty("module", "LSC&AWB");
        note_edit->appendPlainText(LSC_AWB_NOTE_STR);
    } else if (text == "CCM") {
        left_layout->addWidget(ccm_group, 3, 0, Qt::AlignTop);
        ccm_group->show();
        module_box->setProperty("module", "CCM");
        note_edit->appendPlainText(CCM_NOTE_STR);
    } else if (text == "NOISE") {
        left_layout->addWidget(noise_group, 3, 0, Qt::AlignTop);
        noise_group->show();
        module_box->setProperty("module", "NOISE");
        note_edit->appendPlainText(NOISE_NOTE_STR);
    }

    switchImageView();
}

void CalibrationCaptureWindow::switchImageView()
{
    QString path = directory_str + "/temporary";
    QString module = module_box->currentText();
    if (module == "BLC") {
        path += "/blc.raw";
    } else if (module == "LSC&AWB") {
        path += "/lsc_awb_" + QString::number(lsc_awb_light_box->currentIndex()) + ".raw";
    } else if (module == "CCM") {
        path += "/ccm_" + QString::number(ccm_light_box->currentIndex()) + ".raw";
    } else if (module == "NOISE") {
        path += "/noise.raw";
    }

    image_label->clear();
    img_viewer->updatePrompt(utils::IMAGE_TYPE::IMAGE_UNKNOWN, 0, 0);

    QFile rawFile(path);
    if (rawFile.exists()) {
        rawFile.open(QIODevice::ReadOnly);
        QByteArray data = rawFile.readAll();
        img_viewer->setRawImgData(data, width, height, (utils::RAW_BAYER_ID)bayer, utils::RAW_BITS::BITS_12,
                                  utils::RAW_PACK_TYPE::RAW_UNPACKED);
    }
}

void CalibrationCaptureWindow::selectedLightChanged(int index, int module)
{
    if ((index < 0) || (index > 4)) {
        return;
    }

    if (module == 0) {
        if (lsc_awb_capture[index]) {
            lsc_awb_capture_btn->setText("Delete");
        } else {
            lsc_awb_capture_btn->setText("Capture");
        }
        lsc_awb_ct_edit->setText(QString::number(lsc_awb_ct[index]));
    } else if (module == 1) {
        if (ccm_capture[index]) {
            ccm_capture_btn->setText("Delete");
        } else {
            ccm_capture_btn->setText("Capture");
        }
        ccm_ct_edit->setText(QString::number(ccm_ct[index]));
    }

    switchImageView();
}

void CalibrationCaptureWindow::getExpAttrData(EXP_ATTR *data)
{
    BasePageWidget * expAttrPage = MainWindow::getInstance()->getPageWidget(MODULE_NAME_EXP_ATTR);
    QList<BaseItemWidget *> itemList = expAttrPage->getBaseItemList();
    for (int i = 0; i < itemList.size(); i++) {
        BaseItemWidget *item = itemList[i];
        if (item->getTitle() == "OpType") {
            data->opType = item->getData().toUInt();
        } else if (item->getTitle() == "ExpTimeOpType") {
            data->expTimeOpType = item->getData().toUInt();
        } else if (item->getTitle() == "GainType") {
            data->gainType = item->getData().toUInt();
        } else if (item->getTitle() == "ISONumOpType") {
            data->isoNUmOpType = item->getData().toUInt();
        } else if (item->getTitle() == "ExpTime") {
            data->expTime = item->getData().toUInt();
        } else if (item->getTitle() == "ISONum") {
            data->isoNum = item->getData().toUInt();
        } else if (item->getTitle() == "ISONumRange.Max") {
            data->isorangeMax = item->getData().toUInt();
        } else if (item->getTitle() == "ISONumRange.Min") {
            data->isorangeMin = item->getData().toUInt();
        } else if (item->getTitle() == "AdjustTargetMin") {
            data->targetMin = item->getData();
        } else if (item->getTitle() == "AdjustTargetMax") {
            data->targetMax = item->getData();
        } else if (item->getTitle() == "Antiflicker.Enable") {
            data->antiflicker = item->getData().toInt();
        } else if (item->getTitle() == "ExpTimeRange.Max") {
            data->exptimeRangeMax = item->getData().toInt();
        } else if (item->getTitle() == "Compensation") {
            data->target_compensation = item->getData().toInt();
        } else if (item->getTitle() == "EVBias") {
            data->evbias = item->getData().toInt();
        }
    }
}

void CalibrationCaptureWindow::writeExpAttrData(EXP_ATTR data, bool isEnd)
{
    expAttrEnd = isEnd;
    BasePageWidget * expAttrPage = MainWindow::getInstance()->getPageWidget(MODULE_NAME_EXP_ATTR);
    QList<BaseItemWidget *> itemList = expAttrPage->getBaseItemList();
    for (int i = 0; i < itemList.size(); i++) {
        BaseItemWidget *item = itemList[i];
        if (item->getTitle() == "OpType") {
            item->setData(data.opType, true);
        } else if (item->getTitle() == "ExpTimeOpType") {
            item->setData(data.expTimeOpType, true);
        } else if (item->getTitle() == "GainType") {
            item->setData(data.gainType, true);
        } else if (item->getTitle() == "ISONumOpType") {
            item->setData(data.isoNUmOpType, true);
        } else if (item->getTitle() == "ExpTime") {
            item->setData(data.expTime, true);
        } else if (item->getTitle() == "ISONum") {
            item->setData(data.isoNum, true);
        } else if (item->getTitle() == "ISONumRange.Max") {
            item->setData(data.isorangeMax, true);
        } else if (item->getTitle() == "ISONumRange.Min") {
            item->setData(data.isorangeMin, true);
        } else if (item->getTitle() == "AdjustTargetMin") {
            item->setData(data.targetMin, true);
        } else if (item->getTitle() == "AdjustTargetMax") {
            item->setData(data.targetMax, true);
        } else if (item->getTitle() == "Antiflicker.Enable") {
            item->setData(data.antiflicker, true);
        } else if (item->getTitle() == "ExpTimeRange.Max") {
            item->setData(data.exptimeRangeMax, true);
        } else if (item->getTitle() == "Compensation") {
            item->setData(data.target_compensation, true);
        } else if (item->getTitle() == "EVBias") {
            item->setData(data.evbias, true);
        }
    }

    QJsonObject module_obj = expAttrPage->GetModuleObjectToNetworkJsonrpc();
    QJsonObject send_obj = module_obj["ISP_EXPOSURE_ATTR_S"].toObject();
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj), false);
}

void CalibrationCaptureWindow::getStaCfgData(STA_CFG *data)
{
    BasePageWidget * staCfgPage = MainWindow::getInstance()->getPageWidget(MODULE_NAME_STS_CFG);
    QList<BaseItemWidget *> itemList = staCfgPage->getBaseItemList();
    for (int i = 0; i < itemList.size(); i++) {
        BaseItemWidget *item = itemList[i];
        if (item->getTitle() == "AECfg.Weight") {
            data->weight = item->getData();
            break;
        }
    }
}

void CalibrationCaptureWindow::writeStaCfgData(STA_CFG data, bool isEnd)
{
    staCfgEnd = isEnd;
    BasePageWidget * staCfgPage = MainWindow::getInstance()->getPageWidget(MODULE_NAME_STS_CFG);
    QList<BaseItemWidget *> itemList = staCfgPage->getBaseItemList();
    for (int i = 0; i < itemList.size(); i++) {
        BaseItemWidget *item = itemList[i];
        if (item->getTitle() == "AECfg.Weight") {
            item->setData(data.weight, true);
            break;
        }
    }

    QJsonObject module_obj = staCfgPage->GetModuleObjectToNetworkJsonrpc();
    QJsonObject send_obj = module_obj["ISP_STATISTICS_CFG_S"].toObject();
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj), false);
}

void CalibrationCaptureWindow::getExpInfoData(EXP_INFO_C *data)
{
    BasePageWidget * expInfoPage = MainWindow::getInstance()->getPageWidget(MODULE_NAME_EXP_INFO);
    QList<BaseItemWidget *> itemList = expInfoPage->getBaseItemList();
    for (int i = 0; i < itemList.size(); i++) {
        BaseItemWidget *item = itemList[i];
        if (item->getTitle() == "ISO") {
            data->iso = item->getData().toUInt();
        } else if (item->getTitle() == "ExpTime") {
            data->expTime = item->getData().toUInt();
        } else if (item->getTitle() == "AveLum") {
            data->aveLum = item->getData().toUInt();
        } else if (item->getTitle() == "ISPDGain") {
            data->ispDgain = item->getData().toUInt();
        }
    }
}

void CalibrationCaptureWindow::readStaCfgData()
{
    MainWindow::getInstance()->initModulePage(MODULE_NAME_STS_CFG);
    QJsonObject send_obj;
    QJsonObject id_info;
    int jsonrpc_id = GlobalData::getInstance()->getJsonrpcId();
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_METHOD] = "CVI_ISP_GetStatisticsConfig";
    send_obj[MSG_KEY_ID] = jsonrpc_id;
    id_info[MSG_KEY_CMD] = CMD_READ_MODULE;
    id_info[MSG_KEY_MODULE] = "Statistics Config";
    id_info[MSG_KEY_STRUCT] = "ISP_STATISTICS_CFG_S";

    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(jsonrpc_id, id_info);
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj), false);
}

void CalibrationCaptureWindow::readExpInfoData()
{
    MainWindow::getInstance()->initModulePage(MODULE_NAME_EXP_INFO);
    QJsonObject send_obj;
    QJsonObject id_info;
    int jsonrpc_id = GlobalData::getInstance()->getJsonrpcId();
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_METHOD] = "CVI_ISP_QueryExposureInfo";
    send_obj[MSG_KEY_ID] = jsonrpc_id;
    id_info[MSG_KEY_CMD] = CMD_READ_MODULE;
    id_info[MSG_KEY_MODULE] = "Exposure Info";
    id_info[MSG_KEY_STRUCT] = "ISP_EXP_INFO_S";

    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(jsonrpc_id, id_info);
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj), false);
}

void CalibrationCaptureWindow::readExpAttrData()
{
    MainWindow::getInstance()->initModulePage(MODULE_NAME_EXP_ATTR);
    QJsonObject send_obj;
    QJsonObject id_info;
    int jsonrpc_id = GlobalData::getInstance()->getJsonrpcId();
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_METHOD] = "CVI_ISP_GetExposureAttr";
    send_obj[MSG_KEY_ID] = jsonrpc_id;
    id_info[MSG_KEY_CMD] = CMD_READ_MODULE;
    id_info[MSG_KEY_MODULE] = "Exposure attr";
    id_info[MSG_KEY_STRUCT] = "ISP_EXPOSURE_ATTR_S";

    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(jsonrpc_id, id_info);
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj), false);
}

void CalibrationCaptureWindow::initData()
{
    funcIdx = FUNCTION_NONE;
    start_capture = false;
    total_frame = 0;
    current_frame = 0;
    linear_data.clear();
    wdr_data.clear();
    info_log.clear();
    ae_log.clear();
    awb_log.clear();
    awb_bin.clear();
    invalidCount = 0;
}

bool CalibrationCaptureWindow::creatDir(QString module)
{
    if (!CheckSaveFilePath()) {
      return false;
    }

    if (module.isEmpty()) {
        showLog("Module is empty!", LOG_LEVEL::LOG_ERROR);
        return false;
    }

    showLog("CalibrationCaptureWindow::creatDir,module " + module, LOG_LEVEL::LOG_DEBUG);

    if (directory_str.isEmpty()) {
        dateTime_str = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
        directory_str = file_path->text() + "/" + dateTime_str;
        QDir().mkdir(directory_str);
        QString module_str = directory_str + "/" + module;
        QDir().mkdir(module_str);
        QDir().mkdir(directory_str + "/temporary");
        // update Save Path LineEdit
        file_path->setText(directory_str);
    } else {
        QString module_str = directory_str + "/" + module;
        QDir dir(module_str);
        if (dir.exists()) {
            if (module == "BLC" || module == "NOISE") {
                dir.removeRecursively();
                QDir().mkdir(module_str);
            }
        } else {
            QDir().mkdir(module_str);
        }

        QDir tempDir(directory_str + "/temporary");
        if (!tempDir.exists()) {
            QDir().mkdir(directory_str + "/temporary");
        }
    }

    return true;
}

void CalibrationCaptureWindow::handleBlcCaptureClicked()
{
    showLog("Start capture blc raw!", LOG_LEVEL::LOG_INFO);

    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        showLog("Network is not connected!", LOG_LEVEL::LOG_ERROR);
        return;
    }

    if (!creatDir("BLC")) {
        showLog("Creat dir \"BLC\" falied!", LOG_LEVEL::LOG_ERROR);
        return;
    }

    disableComponents();
    initData();

    funcIdx = FUNCTION_BLC_CAPTURE;
    total_frame = blc_iso_box->currentIndex() + 1;
    start_capture = true;

    readExpAttrData();
}

void CalibrationCaptureWindow::handleBlcCancelClicked()
{
    // for blc, cancel just stop capture. Do not delete the existed dir!
    blc_cancel_btn->setEnabled(false);
    if (start_capture) {
        writeExpAttrData(expAttrDefault, true);
    }

    enableComponents();
    blc_cancel_btn->setEnabled(true);
}

void CalibrationCaptureWindow::handleBlcResetClicked()
{
    blc_reset_btn->setEnabled(false);
    if (start_capture) {
        writeExpAttrData(expAttrDefault, true);
    }

    if (!directory_str.isEmpty()) {
        QDir dir(directory_str + "/BLC");
        if (dir.exists()) {
            dir.removeRecursively();
        }
    }

    enableComponents();
    blc_reset_btn->setEnabled(true);
}

void CalibrationCaptureWindow::handleBlcCheckClicked()
{
    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        showLog("Network is not connected!", LOG_LEVEL::LOG_ERROR);
        return;
    }

    if (!creatDir("BLC")) {
        showLog("Creat dir \"BLC\" falied!", LOG_LEVEL::LOG_ERROR);
        return;
    }

    disableComponents();
    initData();

    funcIdx = FUNCTION_BLC_CHECK;
    total_frame = 1;
    start_capture = true;

    readExpAttrData();
}

void CalibrationCaptureWindow::handleLscAwbCaptureClicked()
{
    int idx = lsc_awb_light_box->currentIndex();
    if (idx < 0 || idx > 4) {
        showLog("Light source select error!", LOG_LEVEL::LOG_ERROR);
        return;
    }

    if (lsc_awb_capture[idx]) {
        lsc_awb_capture[idx] = 0;
        funcLscAwbDelete();
    } else {
        funcLscAwbCapture();
    }
}

void CalibrationCaptureWindow::handleLscAwbCancelClicked()
{
    lsc_awb_cancel_btn->setEnabled(false);
    if (start_capture) {
        writeExpAttrData(expAttrDefault, true);
    }

    if (!directory_str.isEmpty()) {
        QString ls = lsc_awb_light_box->currentText();
        int ct = lsc_awb_ct_edit->text().toInt();
        QString path = directory_str + "/LSC&AWB" + "/"+ ls + QString("(%1K)").arg(ct);
        QDir dir(path);
        if (dir.exists()) {
            dir.removeRecursively();
        }
    }

    enableComponents();
    int idx = lsc_awb_light_box->currentIndex();
    lsc_awb_capture[idx] = 0;
    lsc_awb_capture_btn->setText("Capture");
    lsc_awb_capture_btn->setEnabled(true);

}

void CalibrationCaptureWindow::handleLscAwbResetClicked()
{
    lsc_awb_reset_btn->setEnabled(false);
    if (start_capture) {
        writeExpAttrData(expAttrDefault, true);
    }
    // step 0: warning message box, decide if reset
    QMessageBox::StandardButton reply;
    reply = QMessageBox::warning(this,
             "Warning",
             QString("重置和删除：%1/%2?\n")
             .arg(directory_str).arg("LSC&AWB"),
             QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // step 1: delete the dir
        if (!directory_str.isEmpty()) {
            QDir dir(directory_str + "/LSC&AWB");
            if (dir.exists()) {
                dir.removeRecursively();
            }
        }
        // step 2: reset the capture/delete flag
        lsc_awb_capture_btn->setText("Capture");
        for (int i = 0; i < 5; ++i) {
            lsc_awb_capture[i] = 0;
        }
    }
    enableComponents();
    lsc_awb_reset_btn->setEnabled(true);
}

void CalibrationCaptureWindow::funcLscAwbDelete()
{
    disableComponents();
    lsc_awb_capture_btn->setEnabled(false);
    lsc_awb_cancel_btn->setEnabled(true);

    if (start_capture) {
        writeExpAttrData(expAttrDefault, true);
    }

    if (!directory_str.isEmpty()) {
        QString ls = lsc_awb_light_box->currentText();
        int ct = lsc_awb_ct_edit->text().toInt();
        QString path = directory_str + "/LSC&AWB" + "/"+ ls + QString("(%1K)").arg(ct);
        QDir dir(path);
        if (dir.exists()) {
            dir.removeRecursively();
            showLog("Delete dir, " + path, LOG_LEVEL::LOG_INFO);
        }
    }

    enableComponents();
    lsc_awb_capture_btn->setText("Capture");
    lsc_awb_capture_btn->setEnabled(true);
    lsc_awb_cancel_btn->setEnabled(false);
}

void CalibrationCaptureWindow::funcLscAwbCapture()
{
    showLog("Start capture lsc&awb raw!", LOG_LEVEL::LOG_INFO);

    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        showLog("Network is not connected!", LOG_LEVEL::LOG_ERROR);
        return;
    }

    if (!creatDir("LSC&AWB")) {
        showLog("Creat dir \"LSC&AWB\" falied!", LOG_LEVEL::LOG_ERROR);
        return;
    }

    disableComponents();
    lsc_awb_capture_btn->setEnabled(false);
    lsc_awb_cancel_btn->setEnabled(true);

    initData();
    funcIdx = FUNCTION_LSC_AWB_CAPTURE;
    total_frame = 1;
    start_capture = true;

    readStaCfgData();
}

void CalibrationCaptureWindow::handleCcmCaptureClicked()
{
    int idx = ccm_light_box->currentIndex();
    if (idx < 0 || idx > 4) {
        showLog("Light source select error!", LOG_LEVEL::LOG_ERROR);
        return;
    }

    if (ccm_capture[idx]) {
        ccm_capture[idx] = 0;
        funcCcmDelete();
    } else {
        funcCcmCapture();
    }
}

void CalibrationCaptureWindow::handleCcmCancelClicked()
{
    ccm_cancel_btn->setEnabled(false);
    if (start_capture) {
        writeExpAttrData(expAttrDefault, true);
    }

    if (!directory_str.isEmpty()) {
        QString ls = ccm_light_box->currentText();
        int ct = ccm_ct_edit->text().toInt();
        QString path = directory_str + "/CCM" + "/"+ ls + QString("(%1K)").arg(ct);
        QDir dir(path);
        if (dir.exists()) {
            dir.removeRecursively();
        }
    }

    enableComponents();
    int idx = ccm_light_box->currentIndex();
    ccm_capture[idx] = 0;
    ccm_capture_btn->setText("Capture");
    ccm_capture_btn->setEnabled(true);
}

void CalibrationCaptureWindow::handleCcmResetClicked()
{
    ccm_reset_btn->setEnabled(false);
    if (start_capture) {
        writeExpAttrData(expAttrDefault, true);
    }
    // step 0: warning message box, decide if reset
    QMessageBox::StandardButton reply;
    reply = QMessageBox::warning(this,
             "Warning",
             QString("重置和删除：%1/%2?\n")
             .arg(directory_str).arg("CCM"),
             QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // step 1: delete the dir
        if (!directory_str.isEmpty()) {
            QDir dir(directory_str + "/CCM");
            if (dir.exists()) {
                dir.removeRecursively();
            }
        }
        // step 2: reset the capture/delete flag
        ccm_capture_btn->setText("Capture");
        for (int i = 0; i < 5; ++i) {
            ccm_capture[i] = 0;
        }
    }

    enableComponents();
    ccm_reset_btn->setEnabled(true);
}

void CalibrationCaptureWindow::funcCcmDelete()
{
    disableComponents();
    ccm_capture_btn->setEnabled(false);
    ccm_cancel_btn->setEnabled(true);

    if (start_capture) {
        writeExpAttrData(expAttrDefault, true);
    }

    if (!directory_str.isEmpty()) {
        QString ls = ccm_light_box->currentText();
        int ct = ccm_ct_edit->text().toInt();
        QString path = directory_str + "/CCM" + "/"+ ls + QString("(%1K)").arg(ct);
        QDir dir(path);
        if (dir.exists()) {
            dir.removeRecursively();
            showLog("Delete dir, " + path, LOG_LEVEL::LOG_INFO);
        }
    }

    enableComponents();
    ccm_capture_btn->setText("Capture");
    ccm_capture_btn->setEnabled(true);
    ccm_cancel_btn->setEnabled(false);
}

void CalibrationCaptureWindow::funcCcmCapture()
{
    showLog("Start capture ccm raw!", LOG_LEVEL::LOG_INFO);

    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        showLog("Network is not connected!", LOG_LEVEL::LOG_ERROR);
        return;
    }

    if (!creatDir("CCM")) {
        showLog("Creat dir \"CCM\" falied!", LOG_LEVEL::LOG_ERROR);
        return;
    }

    disableComponents();
    ccm_capture_btn->setEnabled(false);
    ccm_cancel_btn->setEnabled(true);

    initData();
    funcIdx = FUNCTION_CCM_CAPTURE;
    total_frame = 1;
    start_capture = true;

    readExpAttrData();
}

void CalibrationCaptureWindow::handleNoiseCaptureClicked()
{
    showLog("Start capture noise raw!", LOG_LEVEL::LOG_INFO);

    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
         showLog("Network is not connected!", LOG_LEVEL::LOG_ERROR);
        return;
    }

    if (!creatDir("NOISE")) {
        showLog("Creat dir \"NOISE\" falied!", LOG_LEVEL::LOG_ERROR);
        return;
    }

    disableComponents();
    noise_cancel_btn->setEnabled(true);
    initData();
    funcIdx = FUNCTION_NOISE_CAPTURE;
    total_frame = noise_iso_box->currentIndex() + 1;
    noise_frame_step = 0;
    start_capture = true;

    readExpAttrData();
}

void CalibrationCaptureWindow::handleNoiseCancelClicked()
{
    // for np, cancel just stop capture. Do not delete the existed dir!
    noise_cancel_btn->setEnabled(false);
    if (start_capture) {
        writeExpAttrData(expAttrDefault, true);
    }

    enableComponents();
    noise_cancel_btn->setEnabled(true);
}

void CalibrationCaptureWindow::handleNoiseResetClicked()
{
    noise_reset_btn->setEnabled(false);
    if (start_capture) {
        writeExpAttrData(expAttrDefault, true);
    }

    if (!directory_str.isEmpty()) {
        QDir dir(directory_str + "/NOISE");
        if (dir.exists()) {
            dir.removeRecursively();
        }
    }

    enableComponents();
    noise_reset_btn->setEnabled(true);
}

void CalibrationCaptureWindow::handleModuleRWResult(QString module, QString cmd, QString result)
{
    if ((module != "Exposure Info") && (module != "Exposure attr") && (module != "Statistics Config")) {
        return;
    }

    qDebug()<<module + cmd + result;

    if (result != "success") {
        showLog(module + cmd + result, LOG_LEVEL::LOG_ERROR);
        return;
    }

    if (cmd == CMD_READ_MODULE) {
        if (module == "Exposure Info") {
            handleReadExpInfoSuccess();
        } else if (module == "Exposure attr") {
            handleReadExpAttrSuccess();
        } else if (module == "Statistics Config") {
            handleReadStaCfgSuccess();
        }
    } else if (cmd == CMD_WRITE_MODULE) {
        if (module == "Exposure attr") {
            handleWriteExpAttrSuccess();
        } else if (module == "Statistics Config") {
            handleWriteStaCfgSuccess();
        }
    }
}

void CalibrationCaptureWindow::handleReadExpAttrSuccess()
{
    qDebug("CalibrationCaptureWindow::handleRecvExpAttrData, funcIdx:%d", funcIdx);
    if (!start_capture) {
        return;
    }

    if (funcIdx == FUNCTION_BLC_CAPTURE) {
        blcCaptureRecvExpAttrData();
    } else if (funcIdx == FUNCTION_BLC_CHECK) {
        blcCheckRecvExpAttrData();
    } else if (funcIdx == FUNCTION_NOISE_CAPTURE) {
        noiseRecvExpAttrData();
    } else if (funcIdx == FUNCTION_CCM_CAPTURE) {
        ccmRecvExpAttrData();
    } else if (funcIdx == FUNCTION_LSC_AWB_CAPTURE) {
        lscAwbRecvExpAttrData();
    } else {
        return;
    }
}

void CalibrationCaptureWindow::handleReadExpInfoSuccess()
{
    qDebug("CalibrationCaptureWindow::handleRecvExpInfoData, funcIdx:%d", funcIdx);
    if (!start_capture) {
        return;
    }

    if (funcIdx == FUNCTION_BLC_CAPTURE) {
        blcCaptureRaw();
    } else if (funcIdx == FUNCTION_BLC_CHECK) {
        blcCheckRaw();
    } else if (funcIdx == FUNCTION_CCM_CAPTURE) {
        ccmCaptureRaw();
    } else if (funcIdx == FUNCTION_LSC_AWB_CAPTURE) {
        lscAwbCaptureRaw();
    } else if (funcIdx == FUNCTION_NOISE_CAPTURE) {
        noiseCaptureRaw();
    } else {
        return;
    }
}

void CalibrationCaptureWindow::handleReadStaCfgSuccess()
{
    qDebug("CalibrationCaptureWindow::handleRecvStaCfgData, funcIdx:%d", funcIdx);
    if (!start_capture) {
        return;
    }

    if (funcIdx == FUNCTION_LSC_AWB_CAPTURE) {
        getStaCfgData(&staCfgDefault);
        staCfg = staCfgDefault;
        QVariantList data;
        for (int row=0; row<15; row++) {
            for (int col=0; col<17; col++) {
                data.append(weight[row][col]);
            }
        }
        staCfg.weight = data;
        writeStaCfgData(staCfg);
    } else {
        showLog("Don't support!", LOG_LEVEL::LOG_INFO);
        return;
    }

}

void CalibrationCaptureWindow::handleWriteStaCfgSuccess()
{
    qDebug("CalibrationCaptureWindow::handleWriteStaCfgSuccess, funcIdx:%d", funcIdx);
    if (!start_capture) {
        return;
    }

    if (!staCfgEnd) {
        if (funcIdx == FUNCTION_LSC_AWB_CAPTURE) {
            readExpAttrData();
        }
    } else {
        initData();
    }

}

void CalibrationCaptureWindow::handleWriteExpAttrSuccess()
{
    qDebug("CalibrationCaptureWindow::handleWriteExpAttrSuccess, funcIdx:%d", funcIdx);
    if (!start_capture) {
        return;
    }

    if (expAttrEnd) {
        if (funcIdx == FUNCTION_LSC_AWB_CAPTURE) {
            writeStaCfgData(staCfgDefault, true);
        } else {
            initData();
        }
    } else {
        if (funcIdx != FUNCTION_NONE) {
            timer->start(2000);
        }
    }
}

void CalibrationCaptureWindow::blcCaptureRecvExpAttrData()
{
    getExpAttrData(&expAttrDefault);
    expAttr = expAttrDefault;
    expAttr.opType = 1;
    expAttr.expTimeOpType = 1;
    expAttr.gainType = 1;
    expAttr.isoNUmOpType = 1;
    expAttr.expTime = 33333;
    expAttr.exptimeRangeMax = 1000000; // 1s
    expAttr.antiflicker = 0;
    expAttr.target_compensation = 56;
    expAttr.evbias = 1024;
    expAttr.isoNum = iso_list[current_frame];
    expAttr.isorangeMax = iso_list[total_frame-1];
    writeExpAttrData(expAttr);
}

void CalibrationCaptureWindow::blcCheckRecvExpAttrData()
{
    getExpAttrData(&expAttrDefault);
    expAttr = expAttrDefault;
    expAttr.opType = 1;
    expAttr.expTimeOpType = 1;
    expAttr.gainType = 1;
    expAttr.isoNUmOpType = 1;
    expAttr.expTime = 33333;
    expAttr.exptimeRangeMax = 1000000; // 1s
    expAttr.antiflicker = 0;
    expAttr.target_compensation = 56;
    expAttr.evbias = 1024;
    expAttr.isoNum = iso_list[blc_iso_box->currentIndex()];
    expAttr.isorangeMax = iso_list[blc_iso_box->currentIndex()];
    writeExpAttrData(expAttr);
}

void CalibrationCaptureWindow::noiseRecvExpAttrData()
{
    getExpAttrData(&expAttrDefault);
    expAttr = expAttrDefault;
    expAttr.opType = 0;
    expAttr.expTimeOpType = 0;
    expAttr.gainType = 1;
    expAttr.isoNUmOpType = 0;
    expAttr.exptimeRangeMax = 1000000; // 1s
    expAttr.antiflicker = 0;
    expAttr.target_compensation = 56;
    expAttr.evbias = 1024;
    expAttr.isoNum = iso_list[current_frame];
    expAttr.isorangeMax = iso_list[current_frame];
    expAttr.isorangeMin = iso_list[current_frame];
    QVariantList list;
    for (int i=0; i<lv_list.size(); i++) {
        list.append(noise_luma_edit->text().toInt());
    }
    expAttr.targetMax = list;
    expAttr.targetMin = list;
    writeExpAttrData(expAttr);
}

void CalibrationCaptureWindow::ccmRecvExpAttrData()
{
    getExpAttrData(&expAttrDefault);
    expAttr = expAttrDefault;
    expAttr.opType = 0;
    expAttr.expTimeOpType = 0;
    expAttr.isoNUmOpType = 0;
    expAttr.gainType = 1;
    expAttr.isoNum = 100;
    expAttr.isorangeMax = 100;
    expAttr.isorangeMin = 100;
    expAttr.exptimeRangeMax = 1000000; // 1s
    expAttr.antiflicker = 0;
    expAttr.target_compensation = 56;
    expAttr.evbias = 1024;
    QVariantList list;
    for (int i=0; i<lv_list.size(); i++) {
        list.append(ccm_luma_edit->text().toInt());
    }
    expAttr.targetMax = list;
    expAttr.targetMin = list;
    writeExpAttrData(expAttr);
}

void CalibrationCaptureWindow::lscAwbRecvExpAttrData()
{
    getExpAttrData(&expAttrDefault);
    expAttr = expAttrDefault;
    expAttr.opType = 0;
    expAttr.expTimeOpType = 0;
    expAttr.isoNUmOpType = 0;
    expAttr.gainType = 1;
    expAttr.isoNum = 100;
    expAttr.isorangeMax = 100;
    expAttr.isorangeMin = 100;
    expAttr.exptimeRangeMax = 1000000; // 1s
    expAttr.antiflicker = 0;
    expAttr.target_compensation = 56;
    expAttr.evbias = 1024;
    QVariantList list;
    for (int i=0; i<lv_list.size(); i++) {
        list.append(lsc_awb_luma_edit->text().toInt());
    }
    expAttr.targetMax = list;
    expAttr.targetMin = list;
    writeExpAttrData(expAttr);
}

void CalibrationCaptureWindow::blcCaptureRaw()
{
    getExpInfoData(&expInfo);
    QString log = QString("Exposure info, ISO=%1, ExpTime=%2, AveLum=%3.").arg(expInfo.iso).arg(expInfo.expTime).arg(expInfo.aveLum);
    showLog(log, LOG_LEVEL::LOG_INFO);

    if (current_frame > 0) {
        if (expInfo.iso <= iso_list[current_frame-1]) {
            invalidCount++;
        } else {
            invalidCount = 0;
        }
    }

    if (invalidCount > 0 && invalidCount < 3) {
        timer->start(3000);
        return;
    } else if (invalidCount >= 3) {
        showLog("Arrive max ISO at the board, auto stop!", LOG_LEVEL::LOG_WARNING);
        total_frame = current_frame;
    }

    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_CALI_GET_MULTI_RAW;
    int id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

    QJsonObject param_obj;
    param_obj["frames"] = 1;
    param_obj["tightly"] = false;
    param_obj["dump_reg"] = false;
    param_obj["dump_raw"] = true;
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_MULTI_RAW;
    send_obj[MSG_KEY_ID] = id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_PARAMS] = param_obj;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj), false);
    current_frame++;
}

void CalibrationCaptureWindow::blcCheckRaw()
{
    getExpInfoData(&expInfo);
    QString log = QString("Exposure info, ISO=%1, ExpTime=%2, AveLum=%3.").arg(expInfo.iso).arg(expInfo.expTime).arg(expInfo.aveLum);
    showLog(log, LOG_LEVEL::LOG_INFO);

    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_CALI_GET_MULTI_RAW;
    int id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

    QJsonObject param_obj;
    param_obj["frames"] = 1;
    param_obj["tightly"] = false;
    param_obj["dump_reg"] = false;
    param_obj["dump_raw"] = true;
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_MULTI_RAW;
    send_obj[MSG_KEY_ID] = id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_PARAMS] = param_obj;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj), false);
    current_frame++;
}

void CalibrationCaptureWindow::ccmCaptureRaw()
{
    getExpInfoData(&expInfo);
    QString log = QString("Exposure info, ISO=%1, ExpTime=%2, AveLum=%3.").arg(expInfo.iso).arg(expInfo.expTime).arg(expInfo.aveLum);
    showLog(log, LOG_LEVEL::LOG_INFO);

    int luma = ccm_luma_edit->text().toInt();
    int lumaMin = (luma - 5 > 0) ? (luma - 5) : 0;
    int lumaMax = (luma + 5 < 255) ? (luma + 5) : 255;

    if (expInfo.aveLum < lumaMin) {
        if (expInfo.expTime > 39000) {
            log = "请调亮灯光!";
            showLog(log, LOG_LEVEL::LOG_WARNING);
        }
    } else if (expInfo.aveLum > lumaMax) {
        if (expInfo.expTime < 100) {
            log = "请调暗灯光!";
            showLog(log, LOG_LEVEL::LOG_WARNING);
        }
    } else {
        int idx = ccm_light_box->currentIndex();
        ccm_capture[idx] = 1;

        NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);

        QJsonObject id_info;
        id_info[MSG_KEY_CMD] = CMD_CALI_GET_MULTI_RAW;
        int id = GlobalData::getInstance()->getJsonrpcId();
        NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

        QJsonObject param_obj;
        param_obj["frames"] = 1;
        param_obj["tightly"] = false;
        param_obj["dump_reg"] = false;
        param_obj["dump_raw"] = true;
        QJsonObject send_obj;
        send_obj[MSG_KEY_METHOD] = METHOD_GET_MULTI_RAW;
        send_obj[MSG_KEY_ID] = id;
        send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
        send_obj[MSG_KEY_PARAMS] = param_obj;
        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj), false);

        current_frame++;
        ccm_capture_btn->setText("Delete");
        ccm_capture_btn->setEnabled(true);
        ccm_cancel_btn->setEnabled(false);
        return;
    }

    timer->start(10000);
}

void CalibrationCaptureWindow::lscAwbCaptureRaw()
{
    getExpInfoData(&expInfo);
    QString log = QString("Exposure info, ISO=%1, ExpTime=%2, AveLum=%3.").arg(expInfo.iso).arg(expInfo.expTime).arg(expInfo.aveLum);
    showLog(log, LOG_LEVEL::LOG_INFO);

    int luma = lsc_awb_luma_edit->text().toInt();
    int lumaMin = (luma - 5 > 0) ? (luma - 5) : 0;
    int lumaMax = (luma + 5 < 255) ? (luma + 5) : 255;

    if (expInfo.aveLum < lumaMin) {
        if (expInfo.expTime > 39000) {
            log = "请调亮灯光!";
            showLog(log, LOG_LEVEL::LOG_WARNING);
        }
    } else if (expInfo.aveLum > lumaMax) {
        if (expInfo.expTime < 100) {
            log = "请调暗灯光!";
            showLog(log, LOG_LEVEL::LOG_WARNING);
        }
    } else {
        int idx = lsc_awb_light_box->currentIndex();
        lsc_awb_capture[idx] = 1;

        NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);

        QJsonObject id_info;
        id_info[MSG_KEY_CMD] = CMD_CALI_GET_MULTI_RAW;
        int id = GlobalData::getInstance()->getJsonrpcId();
        NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

        QJsonObject param_obj;
        param_obj["frames"] = 1;
        param_obj["tightly"] = false;
        param_obj["dump_reg"] = false;
        param_obj["dump_raw"] = true;
        QJsonObject send_obj;
        send_obj[MSG_KEY_METHOD] = METHOD_GET_MULTI_RAW;
        send_obj[MSG_KEY_ID] = id;
        send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
        send_obj[MSG_KEY_PARAMS] = param_obj;
        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj), false);

        current_frame++;
        lsc_awb_capture_btn->setText("Delete");
        lsc_awb_capture_btn->setEnabled(true);
        lsc_awb_cancel_btn->setEnabled(false);
        return;
    }

    timer->start(10000);
}

void CalibrationCaptureWindow::noiseCaptureRaw()
{
    getExpInfoData(&expInfo);
    QString log = QString("Exposure info, ISO=%1, ExpTime=%2, AveLum=%3, ispDgain=%4.").arg(expInfo.iso)
        .arg(expInfo.expTime).arg(expInfo.aveLum).arg(expInfo.ispDgain);
    showLog(log, LOG_LEVEL::LOG_INFO);

    int luma = noise_luma_edit->text().toInt();
    int lumaMin = (luma - 5 > 0) ? (luma - 5) : 0;
    int lumaMax = (luma + 5 < 255) ? (luma + 5) : 255;

    if (expInfo.aveLum < lumaMin) {
        if (expInfo.expTime > 39000) {
            log = "请调亮灯光!";
            showLog(log, LOG_LEVEL::LOG_WARNING);
        }
    } else if (expInfo.aveLum > lumaMax) {
        if (expInfo.expTime < 100) {
            log = "请调暗灯光!";
            showLog(log, LOG_LEVEL::LOG_WARNING);
        }
    } else {
        if (expInfo.ispDgain > 2048) {
            total_frame = current_frame;
            invalidCount = 3;
            showLog("Arrive max ISPDGain, auto stop!", LOG_LEVEL::LOG_WARNING);
        } else {
            if (current_frame > 0) {
                if (expInfo.iso <= iso_list[current_frame-1]) {
                    invalidCount++;
                } else {
                    invalidCount = 0;
                }
            }

            if (invalidCount > 0 && invalidCount < 3) {
                timer->start(3000);
                return;
            } else if (invalidCount >= 3) {
                showLog("Arrive max ISO at the broad, auto stop!", LOG_LEVEL::LOG_WARNING);
                total_frame = current_frame;
            }
        }

        NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);

        QJsonObject id_info;
        id_info[MSG_KEY_CMD] = CMD_CALI_GET_MULTI_RAW;
        int id = GlobalData::getInstance()->getJsonrpcId();
        NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

        QJsonObject param_obj;
        param_obj["frames"] = 1;
        param_obj["tightly"] = false;
        param_obj["dump_reg"] = false;
        param_obj["dump_raw"] = true;
        QJsonObject send_obj;
        send_obj[MSG_KEY_METHOD] = METHOD_GET_MULTI_RAW;
        send_obj[MSG_KEY_ID] = id;
        send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
        send_obj[MSG_KEY_PARAMS] = param_obj;
        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj), false);

        return;
    }

    timer->start(10000);
}

void CalibrationCaptureWindow::handleRawStatus(int id)
{
    qDebug("CalibrationCaptureWindow::handleMultiRawStatus\n");

    QJsonObject obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    if(!start_capture) {
        showLog("Stop Calibration capture raw.", LOG_LEVEL::LOG_INFO);
        return;
    }

    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();
    if (result_obj.isEmpty()) {
        showLog("Calibration capture raw failed!", LOG_LEVEL::LOG_ERROR);
        return;
    }

    // get logs
    QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();
    info_log.append(params_obj["Raw info log"].toString());
    ae_log.append(params_obj["AE log"].toString());
    awb_log.append(params_obj["AWB log"].toString());
    rawSize = params_obj["Data size"].toInt();
    if (rawSize == 0) {
        showLog("Calibration capture raw failed, no raw data!", LOG_LEVEL::LOG_ERROR);
        return;
    }

    QJsonObject id_info;
    id_info[MSG_KEY_CMD] = CMD_CALI_GET_AE_BIN;
    int temp_id = GlobalData::getInstance()->getJsonrpcId();
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(temp_id, id_info);

    QJsonObject param_obj;
    param_obj["content"] = AE_BIN_DATA;
    QJsonObject send_obj;
    send_obj[MSG_KEY_METHOD] = METHOD_GET_BIN_INFO;
    send_obj[MSG_KEY_ID] = temp_id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    send_obj[MSG_KEY_PARAMS] = param_obj;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj), false);
}

void CalibrationCaptureWindow::handleRecvBinInfo(int id)
{
    qDebug("CalibrationCaptureWindow::handleRecvBinInfo\n");

    QJsonObject obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    if(!start_capture) {
        showLog("Stop Calibration capture raw.", LOG_LEVEL::LOG_INFO);
        return;
    }

    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();
    if (result_obj.isEmpty()) {
        showLog("Calibration capture raw failed!", LOG_LEVEL::LOG_ERROR);
        return;
    }

    QJsonObject info_obj = obj[MSG_KEY_ID_INFO].toObject();
    QString cmd = info_obj[MSG_KEY_CMD].toString();
    QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();
    int recvSize = params_obj["Data size"].toInt();

    BinDataType type = NONE_BIN_DATA;
    if (cmd == CMD_CALI_GET_AE_BIN) {
        type = AE_BIN_DATA;
    } else if (cmd == CMD_CALI_GET_AWB_BIN) {
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
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj), false);
}

void CalibrationCaptureWindow::handleRecvBinData(QString cmd)
{
    qDebug("CalibrationCaptureWindow::handleRecvBinData\n");

    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);
    QByteArray data = NetworkClientHelper::getInstance()->popCmdData(cmd);
    if(!start_capture) {
        showLog("Stop Calibration capture raw.", LOG_LEVEL::LOG_INFO);
        return;
    }

    if (cmd == CMD_CALI_GET_AE_BIN) {
        ae_bin.append(data);
        QJsonObject id_info;
        id_info[MSG_KEY_CMD] = CMD_CALI_GET_AWB_BIN;
        int id = GlobalData::getInstance()->getJsonrpcId();
        NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

        QJsonObject param_obj;
        param_obj["content"] = AWB_BIN_DATA;
        QJsonObject send_obj;
        send_obj[MSG_KEY_METHOD] = METHOD_GET_BIN_INFO;
        send_obj[MSG_KEY_ID] = id;
        send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
        send_obj[MSG_KEY_PARAMS] = param_obj;
        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj), false);
    } else if (cmd == CMD_CALI_GET_AWB_BIN) {
        awb_bin.append(data);
        NetworkClientHelper::getInstance()->setCallerInfo(rawSize, CMD_CALI_GET_MULTI_RAW);
        qDebug("Set receive size:%d\n", rawSize);

        int id = GlobalData::getInstance()->getJsonrpcId();
        QJsonObject send_obj;
        send_obj[MSG_KEY_METHOD] = METHOD_GET_IMAGE_DATA;
        send_obj[MSG_KEY_ID] = id;
        send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj), false);
    }
}

void CalibrationCaptureWindow::saveNpIsoRawData(RAW_HEADER *pstHeader, bool is_wdr_mode)
{
    qDebug("saveNpIsoRawData");
    int received_frame = 0;
    int np_frame_index = 0;

    if (current_frame == total_frame) {
        np_frame_index = total_frame - 1;
    } else {
        np_frame_index = current_frame;
    }

    if (!is_wdr_mode) {
        QByteArray tempRawData;
        int frame_size = pstHeader->cropWidth * pstHeader->cropHeight * 2 * NP_CAPTURE_FRAMES;
        int linear_data_size = linear_data.count();

        received_frame = linear_data_size / frame_size;

        if (received_frame != 1) {
            return;
        }

        QByteArray frameData = linear_data;
        tempRawData.append(frameData.mid(0, pstHeader->cropWidth * pstHeader->cropHeight * 2));
        saveRawFile(frameData, *pstHeader, MODE_LINEAR, np_frame_index);

        this->width = pstHeader->cropWidth;
        this->height = pstHeader->cropHeight;
        this->bayer = pstHeader->bayerID;
        saveTemporaryRawFile(tempRawData);
        img_viewer->setRawImgData(tempRawData, pstHeader->cropWidth, pstHeader->cropHeight,
                      (utils::RAW_BAYER_ID) pstHeader->bayerID, utils::RAW_BITS::BITS_12, utils::RAW_PACK_TYPE::RAW_UNPACKED);
        linear_data.clear();
    } else {
        QByteArray raw_data;
        QByteArray tempRawData;
        int num = wdr_data.count();
        int frame_size = pstHeader->cropWidth * pstHeader->cropHeight * 2 * 2 * NP_CAPTURE_FRAMES;

        received_frame = num / 2 / NP_CAPTURE_FRAMES;
        if (received_frame != 1) {
            return;
        }

        int raw_line_byte = pstHeader->cropWidth * 2;
        char *line = new char[raw_line_byte];

        memset(line, 0, raw_line_byte);
        for (int i = 0; i < num - 1; i += 2) {
            char *le = wdr_data[i].data();
            char *se = wdr_data[i + 1].data();
            int size = wdr_data[i].size();
            for (int j = 0; j < size; j += raw_line_byte) {
                memcpy(line, le+j, raw_line_byte);
                raw_data.append(line, raw_line_byte);
                memcpy(line, se+j, raw_line_byte);
                raw_data.append(line, raw_line_byte);
            }
        }

        tempRawData.append(raw_data.mid(0, pstHeader->cropWidth * pstHeader->cropHeight * 2 * 2));
        saveRawFile(raw_data, *pstHeader, MODE_WDR, np_frame_index);

        this->width = pstHeader->cropWidth;
        this->height = pstHeader->cropHeight;
        this->bayer = pstHeader->bayerID;
        saveTemporaryRawFile(tempRawData);
        img_viewer->setRawImgData(tempRawData, pstHeader->cropWidth*2, pstHeader->cropHeight,
                       (utils::RAW_BAYER_ID) pstHeader->bayerID, utils::RAW_BITS::BITS_12, utils::RAW_PACK_TYPE::RAW_UNPACKED);
        wdr_data.clear();
        delete [] line;
    }
}

void CalibrationCaptureWindow::handleRawData(QString cmd)
{
    qDebug("CalibrationCaptureWindow::handleRawData\n");

    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);
    QByteArray data = NetworkClientHelper::getInstance()->popCmdData(cmd);
    if(!start_capture) {
        showLog("Stop Calibration capture raw.", LOG_LEVEL::LOG_INFO);
        return;
    }

    QString log;
    bool remain = false;
    int received_frame = 0;
    int step = 1;

    QByteArray header = data.left(sizeof(RAW_HEADER));
    RAW_HEADER *pstHeader = (RAW_HEADER *) header.data();
    data.remove(0, sizeof(RAW_HEADER));

    log = QString("Cali raw, %1X%2,crop(%3,%4,%5,%6),bayer:%7,compress:%8,size:%9, recvSize:%10").arg(pstHeader->width)
    .arg(pstHeader->height).arg(pstHeader->cropX).arg(pstHeader->cropY).arg(pstHeader->cropWidth).arg(pstHeader->cropHeight)
    .arg(pstHeader->bayerID).arg(pstHeader->compress).arg(pstHeader->size).arg(data.size());
    showLog(log, LOG_LEVEL::LOG_DEBUG);

    if (data.size() != (int)pstHeader->size) {
        showLog("The received data size is wrong!", LOG_LEVEL::LOG_ERROR);
        return;
    }

    //crop & unpack
    QByteArray d = cropUnpackRaw(data, *pstHeader);
    if (d.isEmpty()) {
        showLog("The compress mode is wrong!", LOG_LEVEL::LOG_ERROR);
        return;
    }

    if (funcIdx == FUNCTION_BLC_CAPTURE) {
        log = QString("BLC capture raw iso=%1.").arg(iso_list[current_frame-1]);
    } else if (funcIdx == FUNCTION_BLC_CHECK) {
        log = QString("BLC capture raw iso=%1.").arg(iso_list[blc_iso_box->currentIndex()]);
    } else if (funcIdx == FUNCTION_CCM_CAPTURE) {
        log = "CCM capture raw " + ccm_light_box->currentText() + QString(" ct = %1K.").arg(ccm_ct_edit->text().toInt());
    } else if (funcIdx == FUNCTION_LSC_AWB_CAPTURE) {
        log = "LSC&AWB capture raw " + lsc_awb_light_box->currentText() + QString(" ct = %1K.").arg(lsc_awb_ct_edit->text().toInt());
    } else if (funcIdx == FUNCTION_NOISE_CAPTURE) {
        if (current_frame == total_frame) {
            log = QString("NOISE capture raw iso=%1, index=%2.").arg(iso_list[current_frame-1]).arg(noise_frame_step);
        } else {
            log = QString("NOISE capture raw iso=%1, index=%2.").arg(iso_list[current_frame]).arg(noise_frame_step);
        }
    }
    if (invalidCount == 0) {
        showLog( log, LOG_LEVEL::LOG_INFO);
    }

    step = pstHeader->fusionFrame;
    if (step == 1) {
        linear_data.append(d);
    } else {
        wdr_data.append(d);
    }

    if(pstHeader->curFrame < pstHeader->numFrame - 1) {
        remain = true;
    } else if (pstHeader->curFrame == pstHeader->numFrame - 1) {
        remain = false;
    }

    if (remain) {
        //call for remain frames
        NetworkClientHelper::getInstance()->setCallerInfo(rawSize, CMD_CALI_GET_MULTI_RAW);
        qDebug("Set receive size:%d\n", rawSize);

        int id = GlobalData::getInstance()->getJsonrpcId();
        QJsonObject send_obj;
        send_obj[MSG_KEY_METHOD] = METHOD_GET_IMAGE_DATA;
        send_obj[MSG_KEY_ID] = id;
        send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj), false);
    } else if (current_frame < total_frame) {
        if (funcIdx == FUNCTION_BLC_CAPTURE) {
            expAttr.isoNum = iso_list[current_frame];
            writeExpAttrData(expAttr);
        } else if (funcIdx == FUNCTION_NOISE_CAPTURE) {
            if (current_frame == 0 && noise_frame_step == 0) {
                QByteArray data_tmp;
                QString raw_file_name;
                vector<ColorBlock> bloks;
                int frame_size = pstHeader->cropWidth * pstHeader->cropHeight * 2;

                setRawFileName(*pstHeader, 0);
                if (step == 1) {
                    raw_file_name = file_name_linear + ".raw";
                    data_tmp = linear_data.mid(0, frame_size);
                } else {
                    raw_file_name = file_name_wdr + ".raw";
                    data_tmp = wdr_data[0];
                }

                QFile raw_file(raw_file_name);
                raw_file.open(QIODevice::WriteOnly);
                raw_file.write(data_tmp);
                raw_file.close();

                shared_ptr<GlobalData> global_data = GlobalData::getInstance();
                int lowThr = global_data->getSettings(SETTINGS_SECTION, COLOR_BLOCKS_DETECT_LOWTHR, 10).toInt();
                int highThr = global_data->getSettings(SETTINGS_SECTION, COLOR_BLOCKS_DETECT_HIGHTHR, 40).toInt();
                bloks = ColorBlocksDetect(raw_file_name.toLocal8Bit().constData(), lowThr, highThr,
                                  pstHeader->width, pstHeader->height, bayer_strlist[pstHeader->bayerID].toStdString());

                bool color_block_detect_ret = false;
                bool color_block_detect_v2_ret = false;

                if (bloks.size() == 24) {
                    uint i = 0;
                    for (i = 0; i < bloks.size(); i++) {
                        if (bloks[i].centerX == 0 || bloks[i].centerY == 0 ||
                            bloks[i].width == 0 || bloks[i].height == 0) {
                            break;
                        }
                    }
                    if (i == bloks.size()) {
                        color_block_detect_ret = true;
                    }
                }

                bloks = ColorBlocksDetectV2(raw_file_name.toLocal8Bit().constData(), pstHeader->width,
                            pstHeader->height, static_cast<utils::RAW_BAYER_ID>(pstHeader->bayerID));

                if (bloks.size() == 24) {
                    uint i = 0;
                    for (i = 0; i < bloks.size(); i++) {
                        if (bloks[i].centerX == 0 || bloks[i].centerY == 0 ||
                            bloks[i].width == 0 || bloks[i].height == 0) {
                            break;
                        }
                    }
                    if (i == bloks.size()) {
                        color_block_detect_v2_ret = true;
                    }
                }

                if (!color_block_detect_ret && !color_block_detect_v2_ret) {
                    // all algo detect the color card fail, warning!
                    QMessageBox::StandardButton reply;
                    reply = QMessageBox::warning(this,
                                     "Warning",
                                     QString("1) 24色卡检测失败！请查看：%1。\n2) 是否继续抓NP Raw？\n3) Yes: 抓的Raw请不要使用一键标定。\n4) No：继续调整环境后重新尝试抓Raw。\n")
                                     .arg(raw_file_name),
                                     QMessageBox::Yes | QMessageBox::No);

                    if (reply == QMessageBox::No) {
                        // np 24 color card detect fail -> delete the NOISE dir
                        if (!directory_str.isEmpty()) {
                            QString module_str = directory_str + "/" + "NOISE";
                            QDir dir(module_str);
                            if (dir.exists()) {
                                dir.removeRecursively();
                            }
                        }
                        enableComponents();
                        return;
                    }
                }
            }
            noise_frame_step++;
            if (noise_frame_step < NP_CAPTURE_FRAMES) {
                QJsonObject id_info;
                id_info[MSG_KEY_CMD] = CMD_CALI_GET_MULTI_RAW;
                int id = GlobalData::getInstance()->getJsonrpcId();
                NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

                QJsonObject param_obj;
                param_obj["frames"] = 1;
                param_obj["tightly"] = false;
                param_obj["dump_reg"] = false;
                param_obj["dump_raw"] = true;
                QJsonObject send_obj;
                send_obj[MSG_KEY_METHOD] = METHOD_GET_MULTI_RAW;
                send_obj[MSG_KEY_ID] = id;
                send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
                send_obj[MSG_KEY_PARAMS] = param_obj;
                NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj), false);
                if ((current_frame == total_frame-1) && (noise_frame_step == NP_CAPTURE_FRAMES - 1)) {
                    current_frame = total_frame;
                }
            } else {
                // save current iso raw data
                saveNpIsoRawData(pstHeader, step > 1);
                // write the next iso exposure attr
                noise_frame_step = 0;
                current_frame ++;
                expAttr.isoNum = iso_list[current_frame];
                expAttr.isorangeMax = iso_list[current_frame];
                expAttr.isorangeMin = iso_list[current_frame];
                writeExpAttrData(expAttr);
            }
        }
    } else if (funcIdx == FUNCTION_NOISE_CAPTURE) {
        saveNpIsoRawData(pstHeader, step > 1);
        writeExpAttrData(expAttrDefault, true);
        enableComponents();
        noise_cancel_btn->setEnabled(false);
    } else {
        if (step == 1) {
            QByteArray tempRawData;
            int frame_size = pstHeader->cropWidth * pstHeader->cropHeight * 2;
            received_frame = linear_data.count() / frame_size;
            for (int i=0; i<received_frame; i++) {
                QByteArray frameData = linear_data.mid(frame_size*i, frame_size);
                tempRawData.append(frameData.mid(0, pstHeader->cropWidth * pstHeader->cropHeight * 2));
                saveRawFile(frameData, *pstHeader, MODE_LINEAR, i);
            }
            this->width = pstHeader->cropWidth;
            this->height = pstHeader->cropHeight;
            this->bayer = pstHeader->bayerID;
            saveTemporaryRawFile(tempRawData);
            img_viewer->setRawImgData(tempRawData, pstHeader->cropWidth, pstHeader->cropHeight,
                                      (utils::RAW_BAYER_ID) pstHeader->bayerID, utils::RAW_BITS::BITS_12, utils::RAW_PACK_TYPE::RAW_UNPACKED);
            linear_data.clear();
        } else {
            QByteArray raw_data;
            QByteArray tempRawData;
            int num = wdr_data.count();
            int raw_line_byte = pstHeader->cropWidth * 2;
            char *line = new char[raw_line_byte];

            memset(line, 0, raw_line_byte);
            for (int i = 0; i < num - 1; i += 2) {
                char *le = wdr_data[i].data();
                char *se = wdr_data[i + 1].data();
                int size = wdr_data[i].size();
                for (int j = 0; j < size; j += raw_line_byte) {
                    memcpy(line, le+j, raw_line_byte);
                    raw_data.append(line, raw_line_byte);
                    memcpy(line, se+j, raw_line_byte);
                    raw_data.append(line, raw_line_byte);
                }
            }

            received_frame = num / 2;
            int frame_size = pstHeader->cropWidth * pstHeader->cropHeight * 2 * 2;
            for (int i=0; i<received_frame; i++) {
                QByteArray frameData = raw_data.mid(frame_size*i, frame_size);
                tempRawData.append(frameData.mid(0, pstHeader->cropWidth * pstHeader->cropHeight * 2 * 2));
                saveRawFile(frameData, *pstHeader, MODE_WDR, i);
            }
            this->width = pstHeader->cropWidth;
            this->height = pstHeader->cropHeight;
            this->bayer = pstHeader->bayerID;
            saveTemporaryRawFile(tempRawData);
            img_viewer->setRawImgData(tempRawData, pstHeader->cropWidth*2, pstHeader->cropHeight,
                                       (utils::RAW_BAYER_ID) pstHeader->bayerID, utils::RAW_BITS::BITS_12, utils::RAW_PACK_TYPE::RAW_UNPACKED);
            wdr_data.clear();
            delete [] line;
        }
        writeExpAttrData(expAttrDefault, true);
        enableComponents();
    }
    if (funcIdx != FUNCTION_LSC_AWB_CAPTURE) {
        return;
    }
    // LSC: check center raw average luma
    int avg_r;
    int avg_b;
    int avg_g;

    if (img_viewer->getCenterRoiAvgLuma(avg_r, avg_g, avg_b) != 0) {
        qDebug("getCenterRoiAvgLuma fail!\n");
        return;
    }

    if (avg_r > 200 || avg_b > 200 || avg_g > 200) {
        QString log = QString("Capture LSC Raw, Raw info, Center Roi Size: 32x18, Avg R: %1, Avg G: %2, Avg B: %3\n%4")
            .arg(avg_r)
            .arg(avg_g)
            .arg(avg_b)
            .arg("Center Roi Pixel luma over 200!");
        showLog(log, LOG_LEVEL::LOG_WARNING);
        // use the message box to show the warning
        QMessageBox::warning(this,
                 "Warning",
                 QString("1) 中心32x18区域的rgb averge luma超过200。R: %1, G: %2, B: %3。\n2）shading较为严重，请确认是否减小Luma值重新capture\n")
                 .arg(avg_r).arg(avg_g).arg(avg_b), QMessageBox::Ok);
    } else {
        QString log = QString("Capture LSC Raw, Raw info, Center Roi Size: 32x16, Avg R: %1, Avg G: %2, Avg B: %3")
            .arg(avg_r)
            .arg(avg_g)
            .arg(avg_b);
        showLog(log, LOG_LEVEL::LOG_DEBUG);
    }
}

void CalibrationCaptureWindow::showLog(const QString &log, LOG_LEVEL log_level)
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

void CalibrationCaptureWindow::handleTimeOutEvent()
{
    qDebug("CalibrationCaptureWindow::handleTimeOutEvent");

    timer->stop();
    if (!start_capture) {
        return;
    }

    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        showLog("Network is not connected!", LOG_LEVEL::LOG_ERROR);
        return;
    }

    readExpInfoData();
}

void CalibrationCaptureWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
    initData();
    enableComponents();
    QDir dir(directory_str + "/temporary");
    if (dir.exists()) {
        dir.removeRecursively();
    }
    log_edit->clear();
    directory_str.clear();
    dateTime_str.clear();
    image_label->clear();
    img_viewer->updatePrompt(utils::IMAGE_TYPE::IMAGE_UNKNOWN, 0, 0);
}

void CalibrationCaptureWindow::setRawFileName(RAW_HEADER &header, int index)
{
    int iso = 100;
    int ct;
    int frame = 1;
    QString path, ls;

    QString module = module_box->currentText();
    if (module == "BLC") {
        if (funcIdx == FUNCTION_BLC_CHECK) {
            iso = iso_list[blc_iso_box->currentIndex()];;
        } else {
            iso = iso_list[index];
        }
        path = directory_str + "/" + module + "/ISO" + QString::number(iso);
    } else if (module == "NOISE") {
        frame = NP_CAPTURE_FRAMES;
        iso = iso_list[index];
        path = directory_str + "/" + module + "/ISO" + QString::number(iso);
    } else if (module == "LSC&AWB") {
        ls = lsc_awb_light_box->currentText();
        ct = lsc_awb_ct_edit->text().toInt();
        path = directory_str + "/" + module + "/" + ls + QString("(%1K)").arg(ct);
    } else if (module == "CCM") {
        ls = ccm_light_box->currentText();
        ct = ccm_ct_edit->text().toInt();
        path = directory_str + "/" + module + "/" + ls + QString("(%1K)").arg(ct);
    }

    if (path.isEmpty()) {
        showLog("setRawFileName, path is empty.", LOG_LEVEL::LOG_ERROR);
        return;
    }

    QDir dir(path);
    if (dir.exists()) {
        dir.removeRecursively();
    }
    QDir().mkdir(path);

    // widht, height --> ex: 3840X1080
    file_name_linear = path + "/" + QString("%1X%2_").arg(header.cropWidth).arg(header.cropHeight);
    file_name_wdr = path + "/" + QString("%1X%2_").arg(header.cropWidth * 2).arg(header.cropHeight);

    // bayer string, type string --> ex: RGGB_WDR
    file_name_linear += MW_BAYER_FORMAT_STRING[header.bayerID] + "_Linear_";
    file_name_wdr += MW_BAYER_FORMAT_STRING[header.bayerID] + "_WDR_";

    // bayerID, bits, frames --> ex: color=3_-bits=12_-frame=5
    file_name_linear += QString("-color=%1_-bits=12_-frame=%2_").arg(header.bayerID).arg(frame);
    file_name_wdr += QString("-color=%1_-bits=12_-frame=%2_").arg(header.bayerID).arg(frame);

    // isWdr, ISO --> ex: hdr=1_ISO=100
    file_name_linear += QString("-hdr=%1_ISO=%2_").arg(header.fusionFrame).arg(iso);
    file_name_wdr += QString("-hdr=%1_ISO=%2_").arg(header.fusionFrame).arg(iso);

    // date, time --> 20210314122032
    file_name_linear += dateTime_str;
    file_name_wdr += dateTime_str;

}

void CalibrationCaptureWindow::saveRawFile(QByteArray &data, RAW_HEADER &header, int mode, int index)
{
    if (directory_str.isEmpty()) {
        showLog("Save file, directory_str is empty.", LOG_LEVEL::LOG_ERROR);
        return;
    }

    QString raw_info_name, raw_file_name, ae_bin_name,
        ae_log_name, awb_log_name, awb_bin_name;
    setRawFileName(header, index);
    if (mode == MODE_LINEAR) {
        raw_info_name = file_name_linear + ".txt";
        raw_file_name = file_name_linear + ".raw";
        ae_log_name = file_name_linear + "-aelog.txt";
        ae_bin_name = file_name_linear + "-ae.bin";
        awb_log_name = file_name_linear + "-awblog.txt";
        awb_bin_name = file_name_linear + "-awb.wbin";
    } else if (mode == MODE_WDR) {
        raw_info_name = file_name_wdr + ".txt";
        raw_file_name = file_name_wdr + ".raw";
        ae_log_name = file_name_wdr + "-aelog.txt";
        ae_bin_name = file_name_wdr + "-ae.bin";
        awb_log_name = file_name_wdr + "-awblog.txt";
        awb_bin_name = file_name_wdr + "-awb.wbin";
    }

    if (funcIdx == FUNCTION_NOISE_CAPTURE) {
        index *= NP_CAPTURE_FRAMES;
    }

    QFile raw_info_file(raw_info_name);
    raw_info_file.open(QIODevice::WriteOnly);
    raw_info_file.write(info_log[index].toUtf8());
    raw_info_file.close();

    QFile raw_file(raw_file_name);
    raw_file.open(QIODevice::WriteOnly);
    raw_file.write(data);
    raw_file.close();

    QFile ae_log_file(ae_log_name);
    ae_log_file.open(QIODevice::WriteOnly);
    ae_log_file.write(ae_log[index].toUtf8());
    ae_log_file.close();

    QFile ae_bin_file(ae_bin_name);
    ae_bin_file.open(QIODevice::WriteOnly);
    ae_bin_file.write(ae_bin[index]);
    ae_bin_file.close();

    QFile awb_log_file(awb_log_name);
    awb_log_file.open(QIODevice::WriteOnly);
    awb_log_file.write(awb_log[index].toUtf8());
    awb_log_file.close();

    QFile awb_bin_file(awb_bin_name);
    awb_bin_file.open(QIODevice::WriteOnly);
    awb_bin_file.write(awb_bin[index]);
    awb_bin_file.close();
}

void CalibrationCaptureWindow::saveTemporaryRawFile(QByteArray &data)
{
    if (directory_str.isEmpty()) {
        showLog("saveTemporaryRawFile, directory_str is empty.", LOG_LEVEL::LOG_ERROR);
        return;
    }

    QString path = directory_str + "/temporary";
    QString module = module_box->currentText();
    if (module == "BLC") {
        path += "/blc.raw";
    } else if (module == "LSC&AWB") {
        path += "/lsc_awb_" + QString::number(lsc_awb_light_box->currentIndex()) + ".raw";
    } else if (module == "CCM") {
        path += "/ccm_" + QString::number(ccm_light_box->currentIndex()) + ".raw";
    } else if (module == "NOISE") {
        path += "/noise.raw";
    }

    QFile raw_file(path);
    raw_file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    raw_file.write(data);
    raw_file.close();
}

bool CalibrationCaptureWindow::CheckSaveFilePath() {
    if (!file_path->text().isEmpty()) {
        QDir dir(file_path->text());
        if(!dir.exists()) {
            showLog("Can't find \"" + file_path->text().toUtf8() + "\" directory! Please check save path.", LOG_LEVEL::LOG_ERROR);
            return false;
        }
    } else {
       showLog( "Save path is empty!", LOG_LEVEL::LOG_ERROR);
        return false;
    }

    return true;
}

QByteArray CalibrationCaptureWindow::cropUnpackRaw(QByteArray src, RAW_HEADER &header)
{
    QByteArray src_6b, src_12b, crop_image, det_6b;
    ushort temp[4] = {0};
    int h, w, m, n, i, width;
    uchar v0, v1, v2;
    int src_stride = src.size() / header.height;
    char c = 0, zero = 0;

    if (header.compress == 0) {
        // Unpack and Crop RAW
        for (h = header.cropY; h < header.cropHeight + header.cropY; h++) {
             for (w = header.cropX, m = header.cropX / 2 * 3, i = h * src_stride; w < header.cropWidth + header.cropX; w+=2, m+=3) {
                 v0 = (uchar) src.at(i + m);
                 v1 = (uchar) src.at(i + m + 1);
                 v2 = (uchar) src.at(i + m + 2);
                 temp[0] = static_cast<ushort>((v0 << 4) | ((v2 >> 0) & 0x0f));
                 temp[1] = static_cast<ushort>((v1 << 4) | ((v2 >> 4) & 0x0f));

                 c = temp[0] & 0xFF;
                 crop_image.append(c);
                 c = (temp[0] >> 8) & 0xFF;
                 crop_image.append(c);

                 c = temp[1] & 0xFF;
                 crop_image.append(c);
                 c = (temp[1] >> 8) & 0xFF;
                 crop_image.append(c);
             }
         }
    } else if (header.compress == 1) {
        int line_buffer_width = 0xfffffff;
        int title_start_idx = 0xfffffff;
        QString soc = GlobalData::getInstance()->getCvipqToolSoc();
        if (CV182X_SOC == soc) {
            line_buffer_width = CV182X_LINE_BUFFER_WIDTH;
            title_start_idx = CV182X_TILE_START_INDEX;
        } else if (CV183X_SOC == soc) {
            line_buffer_width = CV183X_LINE_BUFFER_WIDTH;
            title_start_idx = CV183X_TILE_START_INDEX;
        }

        // Unpack RAW
        if (header.width > line_buffer_width) {
            width = header.width + 8;
        } else {
            width = header.width;
        }

        for (h = 0; h < header.height; h++) {
            for (w = 0, m = 0, i = h * src_stride; w < width; w+=4, m+=3) {
                v0 = (uchar) src.at(i + m);
                v1 = (uchar) src.at(i + m + 1);
                v2 = (uchar) src.at(i + m + 2);
                temp[0] = static_cast<ushort>(((v0 & 0x03) << 4) | (v2 & 0x0F ));
                temp[1] = static_cast<ushort>((v0 & 0xFC) >> 2);
                temp[2] = static_cast<ushort>(((v1 & 0x03) << 4) | ((v2 & 0xF0) >> 4));
                temp[3] = static_cast<ushort>((v1 & 0xFC) >> 2);

                c = temp[0] & 0xFF;
                src_6b.append(c);
                src_6b.append(zero);
                c = temp[1] & 0xFF;
                src_6b.append(c);
                src_6b.append(zero);
                c = temp[2] & 0xFF;
                src_6b.append(c);
                src_6b.append(zero);
                c = temp[3] & 0xFF;
                src_6b.append(c);
                src_6b.append(zero);
            }
        }

        if (width > line_buffer_width) {
            for (int x = 0; x < header.height; x++) {
                for (int y = 0, k = 0; y < header.width * 2; y++, k++) {
                    if (y == title_start_idx * 2) {
                        k += (8 * 2);
                    }
                    det_6b.append(src_6b.at(x * width * 2 + k));
                }
            }
        } else {
            det_6b.append(src_6b);
        }
        src_12b.resize(header.width * header.height * 2);
        utils::dpcm_rx((ushort *)det_6b.data(), (ushort *)src_12b.data(), header.width, header.height, 7);

        // Crop Raw
        ushort *pSrc_12b, *pCrop_image;
        crop_image.resize(header.cropWidth * header.cropHeight * 2);
        pSrc_12b = (ushort *)src_12b.data();
        pCrop_image = (ushort *)crop_image.data();
        for (h = 0, n = header.cropY; h < header.cropHeight; h++, n++) {
            for (w = 0, m = header.cropX; w < header.cropWidth; w++, m++) {
                pCrop_image[w + h * header.cropWidth] = pSrc_12b[m + n * header.width];
            }
        }
    } else {
        QMessageBox::warning(this, tr("Error"), tr("The received compress mode is wrong!"), QMessageBox::Ok);
    }

    return crop_image;
}

void CalibrationCaptureWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        this->close();
    }
}

void CalibrationCaptureWindow::OpenSaveFilePath()
{
    //QString dir_path = QFileDialog::getExistingDirectory(this, "Please Select Image Save Path", "./");
    QString dir_path = QFileDialog::getExistingDirectory(this, "Please Select Image Save Path", file_path->text());
    qDebug("Opened Save Path: %s", qUtf8Printable(dir_path));
    if (!dir_path.isEmpty() && dir_path != directory_str) {
        QString log = QString("Update save path: %1 -> %2")
                .arg(directory_str).arg(dir_path);
        showLog(log, LOG_LEVEL::LOG_INFO);
        file_path->setText(dir_path);
        // update directory_str
        directory_str = dir_path;
        // reset ccm and lsc&awb all capture flag
        for (int i = 0; i < 5; ++i) {
            lsc_awb_capture[i] = 0;
            ccm_capture[i] = 0;
        }
        lsc_awb_capture_btn->setText("Capture");
        ccm_capture_btn->setText("Capture");
    }
}

void CalibrationCaptureWindow::stopCapture()
{

}

void CalibrationCaptureWindow::disableComponents()
{
    module_box->setEnabled(false);
    open_btn->setEnabled(false);
    //img_viewer->setEnabled(false);
    blc_capture_btn->setEnabled(false);
    blc_check_btn->setEnabled(false);
    blc_iso_box->setEnabled(false);
    lsc_awb_light_box->setEnabled(false);
    lsc_awb_ct_edit->setEnabled(false);
    ccm_light_box->setEnabled(false);
    ccm_ct_edit->setEnabled(false);
    noise_capture_btn->setEnabled(false);
    noise_iso_box->setEnabled(false);
}

void CalibrationCaptureWindow::enableComponents()
{
    module_box->setEnabled(true);
    open_btn->setEnabled(true);
    //img_viewer->setEnabled(true);
    blc_capture_btn->setEnabled(true);
    blc_check_btn->setEnabled(true);
    blc_iso_box->setEnabled(true);
    lsc_awb_light_box->setEnabled(true);
    lsc_awb_ct_edit->setEnabled(true);
    ccm_light_box->setEnabled(true);
    ccm_ct_edit->setEnabled(true);
    noise_capture_btn->setEnabled(true);
    noise_iso_box->setEnabled(true);
}

