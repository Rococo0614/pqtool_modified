#include "main_window.hpp"
#include "module_define.hpp"
#include "global_data.hpp"
#include "cmodel_image_fetch_window.hpp"
#include "cmodel_ctrl_window.hpp"
#include "gamma_page_widget.hpp"
#include "vo_gamma_page_widget.hpp"
#include "network_client_helper.hpp"
#include "hsv_page_widget.hpp"
#include "raw_image_fetch_window.hpp"
#include "hdr_fusion_page_widget.hpp"
#include "calibration_window.hpp"
#include "system_console_dialog.hpp"
#include "awb_attr_page_widget.hpp"
#include "template_page_widget.hpp"
#include "clut_online_page_widget.hpp"
#include "ini_helper.hpp"
#include "capture_window.hpp"
#include "bin_util_window.hpp"
#include "fswdr_page_widget.hpp"
#include "drc_page_widget.hpp"
#include "utils.hpp"
#include "raw_analyzer_window.hpp"

#include <QApplication>
#include <QDesktopWidget>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <QPlainTextEdit>
#include <QGuiApplication>
#include <functional>
#include <QShortcut>

const double WINDOW_AREA_SCALE = 0.85;

using std::bind;

MODULE_ID page_oder_182xA[] = {
    MODULE_ID::REGISTER_LIST,
    MODULE_ID::AE_INFO,
    MODULE_ID::AWB_ATTR,
    MODULE_ID::HDR_FUSION,
    MODULE_ID::HSV_OLD,
    MODULE_ID::HDR_DRC,
    MODULE_ID::LUT_3D,
    MODULE_ID::TOP,
    MODULE_ID::PUB_ATTR,
    MODULE_ID::NOISE_PROFILE,
    MODULE_ID::EXPOSURE_ATTR,
    MODULE_ID::WDR_EXPOSURE_ATTR,
    MODULE_ID::SMART_EXPOSURE_ATTR,
    MODULE_ID::AE_ROUTE,
    MODULE_ID::IRIS,
    MODULE_ID::EXPOSURE_INFO,
    MODULE_ID::BLACK_LEVEL,
    MODULE_ID::LBLC,
    MODULE_ID::RGBIR,
    MODULE_ID::BNR_LITE,
    MODULE_ID::DPC,
    MODULE_ID::CROSS_TALK,
    MODULE_ID::LSC,
    MODULE_ID::BNR,
    MODULE_ID::AI_DRC,
    MODULE_ID::AI_BNR,
    MODULE_ID::AI_PQ,
    MODULE_ID::WB_ATTR,
    MODULE_ID::AWB_INFO,
    MODULE_ID::DEMOSAIC,
    MODULE_ID::RGBCAC,
    MODULE_ID::LCAC,
    MODULE_ID::CCM,
    MODULE_ID::FSWDR,
    MODULE_ID::DRC,
    MODULE_ID::GAMMA,
    MODULE_ID::DEHAZE,
    MODULE_ID::CLUT,
    MODULE_ID::CSC,
    MODULE_ID::MONO,
    MODULE_ID::PRESHARPEN,
    MODULE_ID::NR3D,
    MODULE_ID::YNR,
    MODULE_ID::CNR,
    MODULE_ID::CAC,
    MODULE_ID::Y_SHARPEN,
    MODULE_ID::DCI,
    MODULE_ID::LDCI,
    MODULE_ID::CA,
    MODULE_ID::CA_LITE,
    MODULE_ID::Y_CONTRAST,
    MODULE_ID::STATISTICS_CONFIG,
    MODULE_ID::DIS,
    MODULE_ID::VPSS_ADJUSTMENT,
    MODULE_ID::VO_GAMMA,
    MODULE_ID::ISPTOVC,
    MODULE_ID::SATURATION,
    MODULE_ID::LDC,
    MODULE_ID::HSV,
    MODULE_ID::RLSC,
    MODULE_ID::RC_ATTR,
    MODULE_ID::RC_PARAM,
    MODULE_ID::CODING_PARAM,
    MODULE_ID::FSHDR,
    MODULE_ID::PFR,
    MODULE_ID::GOP_MODE
};

MODULE_ID page_oder_182x[] = {
    MODULE_ID::REGISTER_LIST,
    MODULE_ID::AE_INFO,
    MODULE_ID::AWB_ATTR,
    MODULE_ID::HDR_FUSION,
    MODULE_ID::HSV_OLD,
    MODULE_ID::HDR_DRC,
    MODULE_ID::LUT_3D,
    MODULE_ID::TOP,
    MODULE_ID::PUB_ATTR,
    MODULE_ID::NOISE_PROFILE,
    MODULE_ID::EXPOSURE_ATTR,
    MODULE_ID::WDR_EXPOSURE_ATTR,
    MODULE_ID::SMART_EXPOSURE_ATTR,
    MODULE_ID::AE_ROUTE,
    MODULE_ID::IRIS,
    MODULE_ID::EXPOSURE_INFO,
    MODULE_ID::BLACK_LEVEL,
    MODULE_ID::DPC,
    MODULE_ID::CROSS_TALK,
    MODULE_ID::WB_ATTR,
    MODULE_ID::AWB_INFO,
    MODULE_ID::BNR,
    MODULE_ID::DEMOSAIC,
    MODULE_ID::RGBCAC,
    MODULE_ID::LSC,
    MODULE_ID::CCM,
    MODULE_ID::FSWDR,
    MODULE_ID::DRC,
    MODULE_ID::GAMMA,
    MODULE_ID::DEHAZE,
    MODULE_ID::CLUT,
    MODULE_ID::CSC,
    MODULE_ID::MONO,
    MODULE_ID::DCI,
    MODULE_ID::LCAC,
    MODULE_ID::LDCI,
    MODULE_ID::CA,
    MODULE_ID::CA_LITE,
    MODULE_ID::PRESHARPEN,
    MODULE_ID::NR3D,
    MODULE_ID::YNR,
    MODULE_ID::CNR,
    MODULE_ID::CAC,
    MODULE_ID::SATURATION,
    MODULE_ID::Y_SHARPEN,
    MODULE_ID::Y_CONTRAST,
    MODULE_ID::STATISTICS_CONFIG,
    MODULE_ID::DIS,
    MODULE_ID::LDC,
    MODULE_ID::HSV,
    MODULE_ID::RLSC,
    MODULE_ID::VPSS_ADJUSTMENT,
    MODULE_ID::VO_GAMMA,
    MODULE_ID::ISPTOVC,
    MODULE_ID::RC_ATTR,
    MODULE_ID::RC_PARAM,
    MODULE_ID::CODING_PARAM,
    MODULE_ID::GOP_MODE
};

MODULE_ID page_oder_183x[] = {
    MODULE_ID::REGISTER_LIST,
    MODULE_ID::AE_INFO,
    MODULE_ID::AWB_ATTR,
    MODULE_ID::HDR_FUSION,
    MODULE_ID::HSV_OLD,
    MODULE_ID::HDR_DRC,
    MODULE_ID::LUT_3D,
    MODULE_ID::TOP,
    MODULE_ID::PUB_ATTR,
    MODULE_ID::NOISE_PROFILE,
    MODULE_ID::EXPOSURE_ATTR,
    MODULE_ID::WDR_EXPOSURE_ATTR,
    MODULE_ID::SMART_EXPOSURE_ATTR,
    MODULE_ID::AE_ROUTE,
    MODULE_ID::IRIS,
    MODULE_ID::EXPOSURE_INFO,
    MODULE_ID::BLACK_LEVEL,
    MODULE_ID::DPC,
    MODULE_ID::CROSS_TALK,
    MODULE_ID::LSC,
    MODULE_ID::WB_ATTR,
    MODULE_ID::AWB_INFO,
    MODULE_ID::FSWDR,
    MODULE_ID::DRC,
    MODULE_ID::BNR,
    MODULE_ID::DEMOSAIC,
    MODULE_ID::CCM,
    MODULE_ID::GAMMA,
    MODULE_ID::DEHAZE,
    MODULE_ID::HSV,
    MODULE_ID::CLUT,
    MODULE_ID::CSC,
    MODULE_ID::MONO,
    MODULE_ID::CA,
    MODULE_ID::PRESHARPEN,
    MODULE_ID::NR3D,
    MODULE_ID::YNR,
    MODULE_ID::CNR,
    MODULE_ID::RGBCAC,
    MODULE_ID::CAC,
    MODULE_ID::SATURATION,
    MODULE_ID::DCI,
    MODULE_ID::Y_SHARPEN,
    MODULE_ID::Y_CONTRAST,
    MODULE_ID::STATISTICS_CONFIG,
    MODULE_ID::DIS,
    MODULE_ID::LDC,
    MODULE_ID::RLSC,
    MODULE_ID::VPSS_ADJUSTMENT,
    MODULE_ID::VO_GAMMA,
    MODULE_ID::ISPTOVC,
    MODULE_ID::RC_ATTR,
    MODULE_ID::RC_PARAM,
    MODULE_ID::CODING_PARAM,
    MODULE_ID::GOP_MODE
};

void IniReadWorker::run()
{
    QList<std::tuple<QString, QString, QString>> reg_list = IniHelper::getInstance()->readAllRegs();
    for (auto reg : reg_list) {
        QString section = std::get<0>(reg);
        QString key = std::get<1>(reg);
        QString value_str = std::get<2>(reg);

        // Signal to main thread to update UI
        emit sigUpdateReg(section, key, value_str);
    }

    emit sigFinishRead();
}

shared_ptr<MainWindow> MainWindow::m_instance = nullptr;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    resize(QDesktopWidget().availableGeometry(this).size() * WINDOW_AREA_SCALE);

    initMenu();
    initUI();
    initEvent();
    retranslateUi();
}

MainWindow::~MainWindow() {
    // remove temp dir when close main window
    QDir dir(GroupItemWidget::getTempDirPath());
    dir.removeRecursively();
}

void MainWindow::setRegisterModified(bool modified)
{
    setWindowModified(modified);
}

void MainWindow::initMenu()
{
    // window menu
    open_file_action = new QAction(this);
    connect(open_file_action, &QAction::triggered, this, std::bind(&MainWindow::openFile, this, "", true));
    menuBar()->addAction(open_file_action);

    save_file_action = new QAction(this);
    // connect(save_file_action, &QAction::triggered, this, std::bind(&MainWindow::saveFile, this, ""));
    connect(save_file_action, &QAction::triggered, this, std::bind(&MainWindow::saveFileJsonrpc, this, ""));
    menuBar()->addAction(save_file_action);

    connect_device_action = new QAction(this);
    connect(connect_device_action, &QAction::triggered, this, &MainWindow::connectDevice);
    menuBar()->addAction(connect_device_action);

    disconnect_device_action = new QAction(this);
    connect(disconnect_device_action, &QAction::triggered, this, &MainWindow::disconnectDevice);
    menuBar()->addAction(disconnect_device_action);

    raw_data_action = new QAction(this);
    connect(raw_data_action, &QAction::triggered, this, &MainWindow::fetchRawData);
    menuBar()->addAction(raw_data_action);

    capture_action = new QAction(this);
    connect(capture_action, &QAction::triggered, this, &MainWindow::showCaptureWindow);
    menuBar()->addAction(capture_action);

    bin_util_action = new QAction(this);
    connect(bin_util_action, &QAction::triggered, this, &MainWindow::showBinUtilWindow);
    menuBar()->addAction(bin_util_action);

    cmodel_action = new QAction(this);
    connect(cmodel_action, &QAction::triggered, this, &MainWindow::showCModelWindow);
    //menuBar()->addAction(cmodel_action);

    cmodel_ctrl_action = new QAction(this);
    connect(cmodel_ctrl_action, &QAction::triggered, this, &MainWindow::showCModelCtrlWindow);
    menuBar()->addAction(cmodel_ctrl_action);
    cmodel_ctrl_action->setVisible(false);

    initCalibrtionToolsMenu();

    register_action = new QAction(this);
    connect(register_action, &QAction::triggered, this, &MainWindow::toggleShowRegisterPanel);
    //menuBar()->addAction(register_action);

    //initLanguageMenu();

    initAssitToolsMenu();

    theme_action = new QAction(this);
    connect(theme_action, &QAction::triggered, this, &MainWindow::handleToggleTheme);
    // menuBar()->addAction(theme_action);

    undo_action = new QAction(this);
    undo_action->setDisabled(true);
    connect(undo_action, &QAction::triggered, this, &MainWindow::undoAction);
    menuBar()->addAction(undo_action);

    redo_action = new QAction(this);
    redo_action->setDisabled(true);
    connect(redo_action, &QAction::triggered, this, &MainWindow::redoAction);
    menuBar()->addAction(redo_action);
}

void MainWindow::initCalibrtionToolsMenu()
{
    cali_tools_menu = new QMenu;
    menuBar()->addMenu(cali_tools_menu);

    calibration_capture_action = new QAction(this);
    connect(calibration_capture_action, &QAction::triggered, this, &MainWindow::showCalibrationCaptureWindow);
    cali_tools_menu->addAction(calibration_capture_action);

    calibration_action = new QAction(this);
    connect(calibration_action, &QAction::triggered, this, &MainWindow::showCalibrationWindow);
    cali_tools_menu->addAction(calibration_action);

    distortion_action = new QAction(this);
    connect(distortion_action, &QAction::triggered, this, &MainWindow::showCalibrationDistortioWindow);
    cali_tools_menu->addAction(distortion_action);

    import_calibration_action = new QAction(this);
    connect(import_calibration_action, &QAction::triggered, this, &MainWindow::showImportCalibrationJsonWindow);
    cali_tools_menu->addAction(import_calibration_action);

    calibration_pdaf_action = new QAction(this);
    connect(calibration_pdaf_action, &QAction::triggered, this, &MainWindow::showCalibrationPdafWindow);
    cali_tools_menu->addAction(calibration_pdaf_action);
}

void MainWindow::initAssitToolsMenu()
{
    assit_tools_menu = new QMenu;
    menuBar()->addMenu(assit_tools_menu);

    fish_eye_action = new QAction(this);
    connect(fish_eye_action, &QAction::triggered, this, &MainWindow::showFishEyeWindow);
    //assit_tools_menu->addAction(fish_eye_action);

    auto_analyser_action = new QAction(this);
    connect(auto_analyser_action, &QAction::triggered, this, &MainWindow::showAutoAnalyserWindow);
    assit_tools_menu->addAction(auto_analyser_action);

    focus_assistant_action = new QAction(this);
    connect(focus_assistant_action, &QAction::triggered, this, &MainWindow::showFocusAssistantWindow);
    assit_tools_menu->addAction(focus_assistant_action);

    yuv_util_action = new QAction(this);
    connect(yuv_util_action, &QAction::triggered, this, &MainWindow::showYuvUtilWindow);
    //assit_tools_menu->addAction(yuv_util_action);

    bracketing_action = new QAction(this);
    connect(bracketing_action, &QAction::triggered, this, &MainWindow::showBracketingWindow);
    assit_tools_menu->addAction(bracketing_action);

    assit_tools_menu->addSeparator();
    continuous_raw_action = new QAction(this);
    connect(continuous_raw_action, &QAction::triggered, this, &MainWindow::showContinuousRawWindow);
    assit_tools_menu->addAction(continuous_raw_action);

    raw_util_action = new QAction(this);
    connect(raw_util_action, &QAction::triggered, this, &MainWindow::showRawUtilWindow);
    assit_tools_menu->addAction(raw_util_action);
    raw_util_action->setVisible(false);

    raw_analyzer_action = new QAction(this);
    connect(raw_analyzer_action, &QAction::triggered, this, &MainWindow::showAnalyzerWindow);
    assit_tools_menu->addAction(raw_analyzer_action);

    assit_tools_menu->addSeparator();
    drc_tone_viewer_action = new QAction(this);
    connect(drc_tone_viewer_action, &QAction::triggered, this, &MainWindow::showDrcToneViewerWindow);
    assit_tools_menu->addAction(drc_tone_viewer_action);

    i2c_editor_action = new QAction(this);
    connect(i2c_editor_action, &QAction::triggered, this, &MainWindow::showI2cEditorWindow);
    assit_tools_menu->addAction(i2c_editor_action);

    reg_tool_action = new QAction(this);
    connect(reg_tool_action, &QAction::triggered, this, &MainWindow::showRegToolWindow);
    assit_tools_menu->addAction(reg_tool_action);

    spi_editor_action = new QAction(this);
    connect(spi_editor_action, &QAction::triggered, this, &MainWindow::showSpiEditorWindow);
    assit_tools_menu->addAction(spi_editor_action);
}

void MainWindow::changeEvent(QEvent *event)
{
    switch(event->type()) {
    case QEvent::LanguageChange:
        retranslateUi();
        break;
    default:
        break;
    }
    QMainWindow::changeEvent(event);
}

void MainWindow::retranslateUi()
{
    for (int i = 0; i < module_settings_tab_widget->count(); i++) {
        BasePageWidget *page = qobject_cast<BasePageWidget *>(module_settings_tab_widget->widget(i));

        QString title = page->property("module_id").toString();
        module_settings_tab_widget->setTabText(i, title);
    }

    open_file_action->setText(tr("&Open"));
    open_file_action->setStatusTip(tr("Open settings file"));

    save_file_action->setText(tr("&Save"));
    save_file_action->setStatusTip(tr("Save settings file"));

    connect_device_action->setText(tr("&Connect"));
    connect_device_action->setStatusTip(tr("Connect device"));

    disconnect_device_action->setText(tr("&Disconnect"));
    disconnect_device_action->setStatusTip(tr("Disconnect device"));

    raw_data_action->setText(tr("&Preview"));
    raw_data_action->setStatusTip(tr("Preview"));

    capture_action->setText(tr("&Capture"));
    capture_action->setStatusTip(tr("Capture"));

    raw_util_action->setText(tr("&Raw Utility"));
    raw_util_action->setToolTip(tr("Raw Utility"));

    yuv_util_action->setText(tr("&Yuv Utility"));
    yuv_util_action->setToolTip(tr("Yuv Utility"));

    bin_util_action->setText(tr("&Bin Utility"));
    bin_util_action->setStatusTip(tr("Bin Utility"));

    fish_eye_action->setText(tr("&Fish Eye Tool"));
    fish_eye_action->setStatusTip(tr("Fish Eye Tool"));

    auto_analyser_action->setText(tr("&3A Analyser"));
    auto_analyser_action->setStatusTip(tr("3A Analyser"));

    focus_assistant_action->setText(tr("&Focus Assistant"));
    focus_assistant_action->setStatusTip(tr("Focus Assistant"));

    bracketing_action->setText(tr("&Bracketing"));
    bracketing_action->setStatusTip(tr("Bracketing"));

    continuous_raw_action->setText(tr("&Continuous Raw"));
    continuous_raw_action->setStatusTip(tr("Continuous Raw"));

    cmodel_action->setText(tr("&CModel"));
    cmodel_action->setStatusTip(tr("CModel"));

    cmodel_ctrl_action->setText(tr("&CModelCtrl"));
    cmodel_ctrl_action->setStatusTip(tr("CModelCtrl"));

    calibration_action->setText(tr("&Calibration"));
    calibration_action->setStatusTip(tr("Calibration"));

    distortion_action->setText(tr("&Distortion Calibration"));
    distortion_action->setStatusTip(tr("Distortion Calibration"));

    import_calibration_action->setText(tr("&Import Calibration Data"));
    import_calibration_action->setStatusTip(tr("Import Calibration Data"));

    calibration_capture_action->setText(tr("&Capture Raw"));
    calibration_capture_action->setStatusTip(tr("Capture Raw"));

    calibration_pdaf_action->setText(tr("&PDAF Calibration"));
    calibration_pdaf_action->setStatusTip(tr("PDAF Calibration"));

    register_action->setText(tr("&Register"));
    register_action->setStatusTip(tr("Register"));

    theme_action->setText(tr("&Dark Theme"));
    theme_action->setStatusTip(tr("Toggle Theme"));

    drc_tone_viewer_action->setText(tr("&DRC Tone Viewer"));
    drc_tone_viewer_action->setStatusTip(tr("DRC Tone Viewer"));

    raw_analyzer_action->setText(tr("&RAW Analyzer"));
    raw_analyzer_action->setStatusTip(tr("RAW Analyzer"));

    i2c_editor_action->setText(tr("&I2C Editor"));
    i2c_editor_action->setStatusTip(tr("I2C Editor"));

    reg_tool_action->setText(tr("&Register Tool"));
    reg_tool_action->setStatusTip(tr("Register Tool"));

    spi_editor_action->setText(tr("&SPI Editor"));
    spi_editor_action->setStatusTip(tr("SPI Editor"));

    assit_tools_menu->setTitle(tr("&Extra Utilities"));
    cali_tools_menu->setTitle(tr("&Calibration"));

    undo_action->setText(tr("&Undo"));
    undo_action->setStatusTip(tr("Undo Operation"));

    redo_action->setText(tr("&Redo"));
    redo_action->setStatusTip(tr("Redo Operation"));
}

void MainWindow::initUI()
{
    main_widget = new QWidget;
    main_layout = new QVBoxLayout;

    // register
    register_widget = new RegisterEditWidget();
    register_widget->setVisible(false);

    // status log
    status_log_widget = new StatusLogWidget();
    status_log_drawer = new DrawerWidget(status_log_widget);
    connect(NetworkClientHelper::getInstance().get(), &NetworkClientHelper::sigStatusLog,
        status_log_widget, &StatusLogWidget::handleAppenLog);
    connect(GlobalData::getInstance().get(), &GlobalData::sigStatusLog,
        status_log_widget, &StatusLogWidget::handleAppenLog);
    connect(this, &MainWindow::sigStatusLog, status_log_widget, &StatusLogWidget::handleAppenLog);

    // module settings tab
    module_settings_tab_widget = new ModuleTabWidget;
    connect(module_settings_tab_widget, &QTabWidget::currentChanged, this, &MainWindow::matchMenuItemofSelectedPage);

    // module list
    search_box_widget  = new SearchBoxWidget(this);
    connect(search_box_widget, &SearchBoxWidget::sigSearchItem, this, &MainWindow::handleSearchItem);
    module_list_widget = new ModuleListWidget(ready_page_list);
    connect(module_list_widget, &ModuleListWidget::selectModule, this, &MainWindow::selectModule);

    QWidget *frequency_wgt = new QWidget;
    QHBoxLayout *frequency_layout = new QHBoxLayout(frequency_wgt);
    QLabel *frequency_lab = new QLabel("Frequency:");
    module_frequency_box = new QComboBox;
    QStringList str_list = {"ALL"};
    module_frequency_box->addItems(str_list);
    module_frequency_box->setCurrentText("ALL");
    module_frequency_box->setEnabled(false);
    frequency_layout->addWidget(frequency_lab);
    frequency_layout->addWidget(module_frequency_box);
    module_list_widget->insertWidget(0, frequency_wgt);
    connect(module_frequency_box, &QComboBox::currentTextChanged, this, &MainWindow::soltModuleListUpdate);
    DrawerWidget *module_list_drawer = new DrawerWidget(module_list_widget, search_box_widget, DRAWER_DIRECTION::HORIZONTAL_LEFT);

    // layout
    QGridLayout *center_layout = new QGridLayout;
    center_layout->setColumnStretch(0, 0);
    center_layout->setColumnStretch(1, 8);
    center_layout->setRowStretch(0, 9);
    center_layout->setRowStretch(1, 0);
    center_layout->addWidget(module_list_drawer, 0, 0, 2, 1);
    center_layout->addWidget(module_settings_tab_widget, 0, 1);
    center_layout->addWidget(status_log_drawer, 1, 1);
    main_layout->addWidget(register_widget);
    main_layout->addLayout(center_layout);
    main_layout->setStretch(0, 1);
    main_layout->setStretch(1, 9);
    setCentralWidget(main_widget);
    centralWidget()->setLayout(main_layout);

    // shortcuts
    handleShortCut();

    // AutoAnalyserWindow
    auto_analyser_window = new AutoAnalyserWindow();
    connect(this, &MainWindow::sigToggleTheme, auto_analyser_window, &AutoAnalyserWindow::handleUpdateTheme);

    // FocusAssistantWindow
    focus_assistant_window = new FocusAssistantWindow();
    focus_assistant_window->move(120, 8);
    connect(this, &MainWindow::sigToggleTheme, focus_assistant_window, &FocusAssistantWindow::handleUpdateTheme);

    calibration_capture_window = new CalibrationCaptureWindow();

    QApplication *app = static_cast<QApplication *>(QApplication::instance());
    app->setStyleSheet(QString("QAbstractSpinBox QMenu::item:selected { background-color: #1A72BB;}") +
                QString("QAbstractSpinBox QMenu::item:disabled:selected { background-color: #EEEEEE;}"));
}

void MainWindow::handleSearchItem(QString item)
{
    QStringList page_order = GlobalData::getInstance()->getPageOrderInfo();
    int page_order_idx = 0;

    QString page_name = page_order[0];

    if (module_settings_tab_widget->count()) {
        BasePageWidget* page_widget = qobject_cast<BasePageWidget *>(module_settings_tab_widget->currentWidget());
        page_name = page_widget->getTitle();
    }

    for (page_order_idx = 0; page_order_idx < page_order.size(); page_order_idx++) {
        if (page_name == page_order[page_order_idx]) {
            GlobalData::getInstance()->setSearchPageOrderIndx(page_order_idx);
            break;
        }
    }

    searchItem(page_order_idx, item);
}

bool MainWindow::SearchPageItem(QString module, QString name)
{
    QJsonObject obj = GlobalData::getInstance()->getPageInfo(module);
    if (obj.isEmpty()) {
        return false;
    }

    int level = GlobalData::getInstance()->getLevel();

    if (module.contains(name, Qt::CaseInsensitive)) {
        return true;
    } else {
        QJsonArray groups_json = obj["groups"].toArray();
        for (int i = 0; i < groups_json.count(); i++) {
            QJsonObject group_obj = groups_json[i].toObject();
            QJsonArray items_json = group_obj["content"].toArray();
            for (int j = 0; j < items_json.count(); j++) {
                QJsonObject item_json = items_json[j].toObject();
                QString title = item_json["title"].toString();
                int level_item = item_json[KEY_REG_LEVEL].toInt();
                if ((level >= level_item) && (title.contains(name, Qt::CaseInsensitive))) {
                    return true;
                }
            }
        }
    }

    return false;
}

void MainWindow::searchItem(int page_order_idx, QString& item)
{
    qDebug("MainWindow::searchItem, %s,id:%d",item.toStdString().c_str(),page_order_idx);
    QStringList page_order = GlobalData::getInstance()->getPageOrderInfo();
    QString name = page_order[page_order_idx];
    int i = 0, next_page_idx = 0;
    bool search_string_changed = false;
    BasePageWidget* page_widget;
    QList<BaseItemWidget *> base_item_list;

    if (item != GlobalData::getInstance()->getPreviousSearchString()) {
        GlobalData::getInstance()->setPreviousSearchString(item);
        GlobalData::getInstance()->setSearchTimesAtSamePage(0);
        GlobalData::getInstance()->setMatchSearchStringTimes(0);
        GlobalData::getInstance()->resetSearchRecursiveCallTimes();
        search_string_changed = true;
    }

    bool bFind = SearchPageItem(name, item);
    if (bFind) {
        selectModule(name);
        page_widget = getPageWidget(name);
        base_item_list = page_widget->getBaseItemList();
    } else {
        GlobalData::getInstance()->incSearchRecursiveCallTimes();
        if (GlobalData::getInstance()->getSearchRecursiveCallTimes() >= page_order.size() &&
                GlobalData::getInstance()->getMatchSearchStringTimes() == 0) {
            GlobalData::getInstance()->resetSearchState();
            QMessageBox::warning(this, tr("Warning"), tr("Not Found!"), QMessageBox::Ok);
            return;
        }
        next_page_idx = page_order_idx + 1;
        if (next_page_idx >= page_order.size()) {
            next_page_idx = 0;
        }
        GlobalData::getInstance()->setSearchTimesAtSamePage(0);
        searchItem(next_page_idx, item);
        return;
    }

    for (i = GlobalData::getInstance()->getSearchTimesAtSamePage(); i < base_item_list.size(); i++) {
        BaseItemWidget *itemWidget = base_item_list.at(i);
        GlobalData::getInstance()->setSearchTimesAtSamePage(i+1);
        if ((GlobalData::getInstance()->getLevel() >= itemWidget->getLevel()) && !(itemWidget->isHidden())) {
            if (itemWidget->getTitle().contains(item, Qt::CaseInsensitive)) {
                GlobalData::getInstance()->incMatchSearchStringTimes();
                if (GlobalData::getInstance()->getCurrentHighlightItem() != nullptr ) {
                     if ((GlobalData::getInstance()->getCurrentHighlightItem() == itemWidget) && !search_string_changed) {
                         GlobalData::getInstance()->resetSearchRecursiveCallTimes();
                         QMessageBox::warning(this, tr("Warning"), tr("No other matched items are found!"), QMessageBox::Ok);
                         return;
                     }
                    GlobalData::getInstance()->getCurrentHighlightItem()->updateHighlight(false);
                }
                itemWidget->updateHighlight(true);
                page_widget->getPageScrollArea()->ensureWidgetVisible(itemWidget, 10, 10);
                GlobalData::getInstance()->setCurrentHighlightItem(itemWidget);
                return;
            }
        }
    }

    if (i == base_item_list.size()) {
        next_page_idx = page_order_idx + 1;
        if (next_page_idx >= page_order.size()) {
            next_page_idx = 0;
        }
        GlobalData::getInstance()->setSearchTimesAtSamePage(0);
        searchItem(next_page_idx, item);
    }
}
void MainWindow::matchMenuItemofSelectedPage(int index)
{
    QString module_name = module_settings_tab_widget->tabText(index);

    module_list_widget->selectedMenuItem(module_name);
}

void MainWindow::handleShortCut()
{
    new QShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S), this, SLOT(handleShowSystemConsole()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_1), this, SLOT(openFile()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_2), this, SLOT(saveFile()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_3), this, SLOT(connectDevice()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_4), this, SLOT(disconnectDevice()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_5), this, SLOT(fetchRawData()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_6), this, SLOT(showCaptureWindow()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_7), this, SLOT(showBinUtilWindow()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_8), this, SLOT(showCalibrationWindow()));
    new QShortcut(QKeySequence(Qt::CTRL +  Qt::SHIFT + Qt::Key_1), this, SLOT(showAutoAnalyserWindow()));
    new QShortcut(QKeySequence(Qt::CTRL +  Qt::SHIFT + Qt::Key_2), this, SLOT(showFocusAssistantWindow()));
    new QShortcut(QKeySequence(Qt::CTRL +  Qt::SHIFT + Qt::Key_3), this, SLOT(showBracketingWindow()));
    new QShortcut(QKeySequence(Qt::CTRL +  Qt::SHIFT + Qt::Key_4), this, SLOT(showContinuousRawWindow()));
    new QShortcut(QKeySequence(Qt::CTRL +  Qt::SHIFT + Qt::Key_9), this, SLOT(showRawUtilWindow()));
}

void MainWindow::initEvent()
{
    ini_read_worker = new IniReadWorker();
    connect(ini_read_worker, &IniReadWorker::sigUpdateReg, this, &MainWindow::handleIniUpdateReg);
    connect(ini_read_worker, &IniReadWorker::sigFinishRead, this, &MainWindow::handleIniFinishReadAllData);
    connect(NetworkClientHelper::getInstance().get(), &NetworkClientHelper::sigChipInfoStatus, this, &MainWindow::handleGetChipInfo);
    connect(GlobalData::getInstance().get(), &GlobalData::sigCheckGainSepStatus, this, &MainWindow::handleCheckGainSepStatus);
    connect(NetworkClientHelper::getInstance().get(), &NetworkClientHelper::sigReadAllPageData, this, &MainWindow::readAllData);
    connect(NetworkClientHelper::getInstance().get(), &NetworkClientHelper::sigEnableAllPageButtons, this, &MainWindow::enableAllPageButtons);
    connect(NetworkClientHelper::getInstance().get(), &NetworkClientHelper::sigReceiveEvValue, this, &MainWindow::updateAllPageEvValue);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::handleTimeOutEvent);
    ev_show_state = GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_EV_SHOW, 0).toBool();
    if (ev_show_state) {
        timer->start(REFRESH_PERIOD_MS);
    }

    auto_apply_timer = new QTimer(this);
    auto_apply_timer->setSingleShot(false);
    connect(auto_apply_timer, &QTimer::timeout, this, &MainWindow::handleAutoApplyTimeout);
}

void MainWindow::initPages()
{
    QStringList ids = GlobalData::getInstance()->getPageInfo();
    for (int i = 0; i < ids.size(); i++) {
        BasePageWidget *page = nullptr;
        QString id = ids[i];
        QJsonObject obj = GlobalData::getInstance()->getPageInfo(id);
        if (!obj.isEmpty()) {
            page = createPage(id);
            if (page != nullptr) {
                page_map[id] = page;
                GlobalData::getInstance()->setPageStatus(id, true);
            }
        }
    }
}

BasePageWidget * MainWindow::initModulePage(QString id)
{
    BasePageWidget *page = nullptr;
    if (page_map.contains(id) && page_map[id] != nullptr) {
        page = page_map[id];
    } else {
        QJsonObject obj = GlobalData::getInstance()->getPageInfo(id);
        if (!obj.isEmpty()) {
            page = createPage(id);
            if (page != nullptr) {
                page_map[id] = page;
                GlobalData::getInstance()->setPageStatus(id, true);
                GlobalData::getInstance()->loadPageData(id);
            }
        }
    }
    return page;
}

void MainWindow::updateWindowTitle()
{
    QString hostName;
    QString connect_type = GlobalData::getInstance()->getSettings(DEVICE_SECTION, KEY_CONNECT_TYPE).toString();
    if (connect_type == CONNECT_TYPE_NETWORK){
        hostName = GlobalData::getInstance()->getSettings(DEVICE_SECTION, KEY_IP_ADDRESS).toString();
    } else if (connect_type == CONNECT_TYPE_UART) {
        hostName = GlobalData::getInstance()->getSettings(DEVICE_SECTION, KEY_UART_PORT).toString();
    }
    QString soc = GlobalData::getInstance()->getCvipqToolSoc();
    QString appName = QGuiApplication::applicationDisplayName();
    QString templateName = GlobalData::getInstance()->getCvipqToolTemplate();
    QString windowTitle = QString("[*]%1 - %2 - %3 - %4 - %5").arg(soc, hostName, appName, CVIPQ_TOOL_VERSION, templateName);
    setWindowTitle(windowTitle);
}

void MainWindow::selectModule(QString module)
{
    for (int i = 0; i < module_settings_tab_widget->count(); i++) {
        if (module_settings_tab_widget->tabText(i) == module) {
            module_settings_tab_widget->setCurrentIndex(i);
            return;
        }
    }

    BasePageWidget *page = getPageWidget(module);

    if (page == nullptr) {
        return;
    }

    module_settings_tab_widget->addTab(page, page->getTitle());
    module_settings_tab_widget->setCurrentWidget(page);
    updateTabBarWidth();
}

void MainWindow::updateTabBarWidth()
{
    QTabBar* tab_bar = module_settings_tab_widget->tabBar();
    if (tab_bar->count() <= 0) {
        return;
    }

    if (tab_bar->count() > 0 && baseTabBarW == 0) {
        int w = tab_bar->tabRect(0).width();
        QString text = tab_bar->tabText(0);
        int textW = module_settings_tab_widget->fontMetrics().boundingRect(text).width();
        baseTabBarW = w - textW;
    }

    int maxW = 0;
    for (int i = 0; i < tab_bar->count(); i++) {
        QString text = tab_bar->tabText(i);
        int textW = module_settings_tab_widget->fontMetrics().boundingRect(text).width();
        maxW = textW > maxW ? textW : maxW;
    }

    tab_bar->setStyleSheet(QString("QTabBar::tab{width:%1}").arg(maxW + baseTabBarW));
}

void MainWindow::handleCheckGainSepStatus()
{
    int GainSepStatus = 0;

    // Get GainSepStatus from Exposure Info
    BasePageWidget *exposure_info_page = page_map[MODULE_NAME_EXP_INFO];
    if (exposure_info_page == nullptr) return;

    QJsonArray data_array = exposure_info_page->GetModuleObjectToLocal();
    for (int i = 0; i < data_array.count(); i++) {
        QJsonObject data_obj = data_array[i].toObject();
        if (data_obj[KEY_REG_KEY].toString() == "GainSepStatus") {
            GainSepStatus = data_obj[KEY_VALUE].toInt();
            break;
        }
    }

    // Set AEGainSepCfg of Exoposure Attr
    BasePageWidget *exposure_attr_page = page_map[MODULE_NAME_EXP_ATTR];
    if (exposure_attr_page == nullptr) return;

    QList<BaseItemWidget *> list = exposure_attr_page->getBaseItemList();
    for (int i = 0; i < list.size(); i++) {
        BaseItemWidget *item = list[i];
        if (item->getTitle() == "AEGainSepCfg") {
            if (GainSepStatus == 0) {
                item->setChmod(MOD_RO);
            } else {
                item->setChmod(MOD_RW);
            }
        }
    }
}

BasePageWidget *MainWindow::createPage(QString id)
{
    BasePageWidget *page = nullptr;
    if (id == MODULE_NAME_REG_LIST) {
        page = new RegisterPageWidget;
    } else if (id == MODULE_NAME_AWB_ATTR) {
        page = new AWBAttrPageWidget;
    } else if (id == MODULE_NAME_HDR) {
        page = new HDRFusionPageWidget;
    } else if (id == MODULE_NAME_BNR) {
        page = new BNRPageWidget;
    } else if (id == MODULE_NAME_GAMMA) {
        page = new GammaPageWidget;
    } else if (id == MODULE_NAME_VO_GAMMA) {
        page = new VOGammaPageWidget;
    } else if (id == MODULE_NAME_HSV_OLD) {
        page = new HSVPageWidget;
    } else if (id == MODULE_NAME_FSWDR) {
        page = new FSWDRPageWidget;
    } else if (id == MODULE_NAME_DRC) {
        page = new DRCPageWidget;
    } else if (id == MODULE_NAME_CLUT + QString(" Online")) {
        page = new ClutOnlinePageWidget(id);
    } else {
        page = new TemplatePageWidget(id);
    };

    connect(page, &BasePageWidget::sigEnvirValue, this, &MainWindow::updateEvShowState);
    connect(page, &BasePageWidget::sigReadAll, this, &MainWindow::readAllData);
    connect(page, &BasePageWidget::sigWriteAll, this, &MainWindow::writeAllData);
    connect(page, &BasePageWidget::sigStatusLog, status_log_widget, &StatusLogWidget::handleAppenLog);
    connect(this, &MainWindow::sigToggleTheme, status_log_widget, &StatusLogWidget::handleUpdateTheme);
    connect(this, &MainWindow::sigToggleTheme, page, &BasePageWidget::paegUpdateTheme);
    status_log_widget->appenLog(page->getTitle() + " page initialize successfully ", LOG_LEVEL::LOG_INFO);
    this->repaint();

    return page;
}

void MainWindow::readAllData()
{
    QString connect_type = GlobalData::getInstance()->getSettings(DEVICE_SECTION, KEY_CONNECT_TYPE).toString();

    if (CONNECT_TYPE_NETWORK == connect_type || CONNECT_TYPE_UART == connect_type) {
        int num = 0;

        this->rw_all_page_flag = true;
        for (int i = 0; i < ready_page_list.size(); i++) {
            if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
                break;
            }

            QString id = ready_page_list[i];
            if (id == MODULE_NAME_LDC) {
                continue;
            }

            if (page_map.contains(id) && page_map[id] != nullptr) {
                BasePageWidget *page = page_map[id];
                if (page->getReadyFlag()) {
                    page->readData();
                    num++;
                }
            } else {
                readPageManual(id);
            }
        }
        GlobalData::getInstance()->setReadingPagesNumber(num);
    } else if (CONNECT_TYPE_CMODEL == connect_type) {
        foreach (auto page, page_map) {
            page->setStatusBusy(true);
            page->clearDirty();
        }
        // load ini use ini helper thread to reduce loading
        ini_read_worker->start();
    }
    this->rw_all_page_flag = false;
}

void MainWindow::readPageManual(QString id)
{
    NetworkClientHelper::getInstance()->clearIdInfoJsonrpc();
    NetworkClientHelper::getInstance()->setCallerInfo(0, NULL);
    QJsonObject page_obj = GlobalData::getInstance()->getPageData(id);
    QJsonArray struct_arr = page_obj["struct"].toArray();
    QJsonArray method_arr = page_obj["method"].toArray();
    QJsonObject mapping_obj = page_obj["mapping"].toObject();
    int n = 0;
    if (id == MODULE_NAME_GAMMA) {
        n = -3;
    }
    for (int i = n; i < struct_arr.size(); i++) {
        QJsonObject send_obj;
        QJsonObject id_info;
        QString struct_id;
        QString method;
        QString cmd_str;
        if (i < 0) {
            method = "CVI_ISP_GetGammaCurveByType";
            cmd_str = CMD_READ_GAMMA;
            struct_id = "ISP_GAMMA_ATTR_S";
            QJsonObject curve_type_obj;
            curve_type_obj["curveType"] = i + 3;
            send_obj["params"] = curve_type_obj;
            id_info["curve_type"] = i + 3;
        } else {
            cmd_str = CMD_READ_MODULE;
            struct_id = struct_arr[i].toString();
            for (int j = 0; j < method_arr.size(); j++) {
                QJsonObject method_obj = method_arr[i].toObject();
                if (method_obj["STRUCT"] == struct_id) {
                    method = method_obj["GET"].toString();
                    break;
                }
            }
        }

        if (method.isEmpty()) {
            continue;
        }

        int jsonrpc_id = GlobalData::getInstance()->getJsonrpcId();
        send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
        send_obj[MSG_KEY_METHOD] = method;
        send_obj[MSG_KEY_ID] = jsonrpc_id;
        id_info[MSG_KEY_CMD] = cmd_str;
        id_info[MSG_KEY_MODULE] = id;
        id_info[MSG_KEY_STRUCT] = struct_id;
        if (mapping_obj.contains(struct_id)) {
            id_info[MSG_KEY_MAPPING] = mapping_obj[struct_id];
        }

        NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(jsonrpc_id, id_info);
        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));

        bool gotResponse = false;
        QTime stopTime;
        int waitTime = READ_WRITE_TIMEOUT;
        if (CONNECT_TYPE_UART == GlobalData::getInstance()->getSettings(DEVICE_SECTION, KEY_CONNECT_TYPE).toString())
            waitTime *= 5;
        stopTime = QTime::currentTime().addSecs(waitTime);
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
            QString message = id + QString(" Page: Failed to read %1. Timeout: Can't get reponse from board within %2 seconds.").arg(struct_id).arg(waitTime);
            emit sigStatusLog(message, LOG_LEVEL::LOG_ERROR);
            break;
        }
    }
}

void MainWindow::writeAllData()
{
    this->rw_all_page_flag = true;
    for (int i = 0; i < ready_page_list.size(); i++) {
        if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
            break;
        }

        QString id = ready_page_list[i];
        if (id == MODULE_NAME_LDC) {
            continue;
        }

        if (page_map.contains(id) && page_map[id] != nullptr) {
            BasePageWidget *page = page_map[id];
            if (page->getReadyFlag()) {
                page->writeData();
            }
        } else {
            writePageManual(id);
        }
    }
    this->rw_all_page_flag = false;
    doRecordAction();
}

void MainWindow::writePageManual(QString id)
{

    QJsonObject page_obj = GlobalData::getInstance()->getPageData(id);
    QJsonArray struct_arr = page_obj["struct"].toArray();
    if (page_obj["ro"].toBool()) {
        emit sigStatusLog(id + " page is read-only.", LOG_LEVEL::LOG_INFO);
        return;
    }

    QJsonObject module_obj = GetModuleObjectToNetworkJsonrpc(id);

    for (int i = 0; i < struct_arr.size(); i++) {
        QString struct_id = struct_arr[i].toString();
        QJsonObject send_obj = module_obj[struct_id].toObject();

        if ((id == MODULE_NAME_CLUT && struct_id == QString("ISP_CLUT_ATTR_S"))
            || (id == MODULE_NAME_LSC && struct_id == QString("ISP_MESH_SHADING_GAIN_LUT_ATTR_S"))) {
            send_obj["params"] = utils::stringFromObject(send_obj["params"].toObject());
        }
        if ((id == MODULE_NAME_NP_PRO) && (struct_id == QString("ISP_CMOS_NOISE_CALIBRATION_S"))) {
            send_obj["params"] = utils::revertNoiseProfileParamFormat(send_obj["params"].toObject());
        }

        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));

        bool gotResponse = false;
        int waitTime = READ_WRITE_TIMEOUT;
        if (CONNECT_TYPE_UART == GlobalData::getInstance()->getSettings(DEVICE_SECTION, KEY_CONNECT_TYPE).toString())
            waitTime *= 10;
        QTime stopTime = QTime::currentTime().addSecs(waitTime);
        while (QTime::currentTime() < stopTime) {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
            if (!GlobalData::getInstance()->getWaitForResponse()) {
                gotResponse = true;
                break;
            }
        }

        if (!gotResponse) {
            GlobalData::getInstance()->setWaitForResponse(false);
            QString message = id + QString(" Page: Failed to write %1. Timeout: Can't get reponse from board within %2 seconds.").arg(struct_id).arg(waitTime);
            emit sigStatusLog(message, LOG_LEVEL::LOG_ERROR);
            break;
        }
    }
}

QJsonObject MainWindow::GetModuleObjectToNetworkJsonrpc(QString id)
{
    QJsonObject module_obj;
    QJsonObject page_obj = GlobalData::getInstance()->getPageData(id);
    QJsonArray struct_arr = page_obj["struct"].toArray();
    QJsonArray method_arr = page_obj["method"].toArray();

    NetworkClientHelper::getInstance()->clearIdInfoJsonrpc();
    for (int i = 0; i < struct_arr.size(); i++) {
        QString struct_id = struct_arr[i].toString();
        QString method;
        for (int j = 0; j < method_arr.size(); j++) {
            QJsonObject method_obj = method_arr[i].toObject();
            if (method_obj["STRUCT"] == struct_id) {
                method = method_obj["SET"].toString();
                break;
            }
        }

        QJsonObject send_obj;
        QJsonObject id_info;
        int jsonrpc_id = GlobalData::getInstance()->getJsonrpcId();
        send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
        send_obj[MSG_KEY_METHOD] = method;
        send_obj[MSG_KEY_ID] = jsonrpc_id;
        id_info[MSG_KEY_CMD] = CMD_WRITE_MODULE;
        id_info[MSG_KEY_MODULE] = id;
        id_info[MSG_KEY_STRUCT] = struct_id;

        NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(jsonrpc_id, id_info);

        QJsonObject pagedata = page_obj["data"].toObject();
        QJsonObject groupData = pagedata[struct_id].toObject();
        QStringList keys = groupData.keys();
        QJsonObject params_obj;
        for (int j = 0; j < keys.size(); j++) {
            QString path = keys[j];
            QJsonObject itemObj = groupData[path].toObject();
            QJsonValue value;
            if (itemObj.contains("RO")) {
                continue;
            }
            QStringList path_split = path.split(QLatin1Char('.'));
            QJsonObject mapping_obj = page_obj["mapping"].toObject()[struct_id].toObject();

            if (path_split.count() < 2) {
                continue;
            }

            if (itemObj.contains("value_type") && (itemObj["value_type"] == TYPE_DOUBLE)){
                if (itemObj["VALUE"].isArray()) {
                    QJsonArray tmpList = itemObj["VALUE"].toArray();
                    QJsonArray tmpArr;
                    for (int i = 0; i < tmpList.size(); i++) {
                        QString strValue = QString::number(tmpList[i].toDouble(), 'f', 10);
                        tmpArr.append(QJsonValue(strValue));
                    }
                    value = QJsonValue(tmpArr);
                } else {
                    QString strValue = QString::number(itemObj["VALUE"].toDouble(), 'f', 2);
                    value = QJsonValue(strValue);
                }
            } else {
                value = itemObj["VALUE"];
            }

            path_split.removeFirst();
            utils::createParamJsonrpc(path_split, params_obj, value, mapping_obj);
        }
        send_obj[MSG_KEY_PARAMS] = params_obj;
        module_obj[struct_id] = send_obj;
    }

    return module_obj;
}

void MainWindow::readPageData(QString id)
{
    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        return;
    }

    BasePageWidget *page = nullptr;
    if (page_map.contains(id) && page_map[id] != nullptr) {
        page = page_map[id];
    } else {
        page = initModulePage(id);
    }

    if ((page != nullptr) && (page->getReadyFlag())) {
        page->readData();
    }
}

void MainWindow::writePageData(QString id)
{
    if (!NetworkClientHelper::getInstance()->isNormalStatus()) {
        return;
    }

    BasePageWidget *page = nullptr;
    if (page_map.contains(id) && page_map[id] != nullptr) {
        page = page_map[id];
    } else {
        page = initModulePage(id);
    }
    if ((page != nullptr) && (page->getReadyFlag())) {
        page->writeData();
    }
}

QPair<int, int> MainWindow::getChildPageInfo()
{
    QPair<int, int> page_info;
    int page_total_num = 0;
    int only_page_num = 0;
    QString id;

    foreach (auto page, page_map) {
        id = page->property("module_id").toString();
        if (id == MODULE_NAME_LDC) {
            continue;
        }
        if (page->getReadyFlag())
            page_total_num++;
        if (page->getPageOnlyReadProperty()) {
            only_page_num++;
        }

    }
    page_info.first = page_total_num;
    page_info.second = only_page_num;

    return page_info;
}

void MainWindow::enableAllPageButtons(QString module, QString cmd, BUTTON_MODE mode)
{
    if (mode == BUTTON_MODE::MODE_COUNT) {
        if (!this->rw_all_page_flag) {
            return;
        }
        this->module_list.append(module);
        utils::deleteListSameMember(this->module_list);
        if (cmd == CMD_READ_MODULE || cmd == CMD_READ_GAMMA) {
            if (this->module_list.size() != this->page_info.first) {
                return;
            }
        }
        if (cmd == CMD_WRITE_MODULE) {
            if (this->module_list.size() != (this->page_info.first - this->page_info.second)) {
                return;
            }
        }
    }
    foreach (auto page, page_map) {
        if (page->getReadyFlag())
            page->enableButtons();
    }
    this->module_list.clear();
}

void MainWindow::disableAllPageButtons()
{
    foreach (auto page, page_map) {
        if (page->getReadyFlag())
            page->disableButtons();
    }
}

void MainWindow::handleTimeOutEvent()
{
    if (!NetworkClientHelper::getInstance()->isNormalStatus() || this->rw_all_page_flag) {
        return;
    }
    foreach (auto page, page_map) {
         if (page->getReadyFlag()) {
            if (page->getPageBusyState()) {
                return;
            }
        }
    }

    QJsonObject id_info;
    QJsonObject send_obj;
    int id = GlobalData::getInstance()->getJsonrpcId();

    id_info[MSG_KEY_CMD] = CMD_GET_ISO_LV_VALUE;
    NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

    send_obj[MSG_KEY_METHOD] = METHOD_CVI_ISP_QueryExposureInfo;
    send_obj[MSG_KEY_ID] = id;
    send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;
    NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
}

void MainWindow::updateAllPageEvValue(int id)
{
    QJsonObject data_obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);

    if (!data_obj.isEmpty()) {
        QJsonObject result_obj = data_obj[MSG_KEY_RESULT].toObject();
        QJsonObject params_obj;
        int iso_value = 0;
        double lv_value = 0.0;

        if (!result_obj.isEmpty()) {
            params_obj = result_obj[MSG_KEY_PARAMS].toObject();
            if (params_obj.contains("fLightValue")) {
                lv_value = params_obj["fLightValue"].toDouble();
            }
            if (params_obj.contains("u32ISO")) {
                iso_value = params_obj["u32ISO"].toInt();
            }
        }

        foreach (auto page, page_map) {
            if (page->getReadyFlag()) {
                page->updateEvShowState(true, QString("ISO:%1").arg(QString::number(iso_value)),
                    QString(" LV:%1").arg(QString::number(lv_value, 'f', 2)));
            }
        }
    }
}

void MainWindow::updateEvShowState(bool checked_state)
{
    foreach (auto page, page_map) {
         if (page->getReadyFlag()) {
            page->updateEvShowState(checked_state, ISO_DEFAULT_VALUE, LV_DEFAULT_VALUE);
        }
    }

    if (checked_state) {
        if (!timer->isActive()) {
            timer->start(REFRESH_PERIOD_MS);
        }
    } else {
        timer->stop();
    }
    ev_show_state = checked_state;
    shared_ptr<GlobalData> global_data = GlobalData::getInstance();
    global_data->setSettings(SETTINGS_SECTION, KEY_EV_SHOW, ev_show_state);
    global_data->saveSettings();
}

void MainWindow::updateAllPageAutoWriteState(bool checked_state)
{
    foreach (auto page, page_map) {
        if (page->getReadyFlag()) {
            page->updatePageAutoWrite(checked_state);
        }
    }

    int state = (checked_state == true) ? 1 : 0;
    shared_ptr<GlobalData> global_data = GlobalData::getInstance();
    global_data->setSettings(SETTINGS_SECTION, KEY_AUTO_WRITE, state);
    global_data->saveSettings();
}

BasePageWidget * MainWindow::getPageWidget(QString name)
{
    if (page_map.contains(name) && page_map[name] != nullptr) {
        return page_map[name];
    } else {
        return initModulePage(name);
    }
}

void MainWindow::open(QString file_path, bool showSuccessfulMessage)
{
    int ret = 0;
    QFileInfo fileInfo(file_path);

    if (!file_path.isEmpty() && fileInfo.exists()) {
        QJsonObject load_data;
        QString error_msg;
        if (utils::loadJson(file_path, load_data, error_msg) != 0) {
            QMessageBox::critical(this, "Error", error_msg, QMessageBox::Ok);
            return;
        }

        if (load_data.contains("JSON Info")) {
            ret = loadDataFromOldJsonFormat(file_path);
        } else if (load_data.contains("SAVE DATA")){
            ret = loadDataFromNewJsonFormat(file_path);
        }

        if (showSuccessfulMessage && ret == 0) {
            QMessageBox::information(this, "Information", "Load json file successfully.", QMessageBox::Ok);
        } else if (ret != 0) {
            QMessageBox::critical(this, "Error", "Failed to load json file.", QMessageBox::Ok);
        }

        qDebug("open file:%s", qUtf8Printable(file_path));
    }
}

void MainWindow::openFile(QString filePath, bool showSuccessfulMessage)
{
    shared_ptr<GlobalData> global_data = GlobalData::getInstance();
    if (filePath.isEmpty()) {
        QString open_dir = global_data->getSettings(SETTINGS_SECTION, KEY_OPEN_PATH).toString();
        if (!open_dir.isEmpty()) {
            QDir dir(open_dir);
            if(dir.exists()) {
                filePath = QFileDialog::getOpenFileName(this, tr("Open Settings file"), open_dir, tr("Json files (*.json)"));
            } else {
                filePath = QFileDialog::getOpenFileName(this, tr("Open Settings file"), "", tr("Json files (*.json)"));
            }
        } else {
            filePath = QFileDialog::getOpenFileName(this, tr("Open Settings file"), "", tr("Json files (*.json)"));
        }
        if (!filePath.isEmpty()) {
            QFileInfo fileInfo(filePath);
            QString filDir = fileInfo.absolutePath();
            global_data->setSettings(SETTINGS_SECTION, KEY_OPEN_PATH, filDir);
            global_data->saveSettings();
        }
    }

    if (filePath.isEmpty()) {
        return;
    }

    qDebug("open file:%s", qUtf8Printable(filePath));
    open(filePath, showSuccessfulMessage);

    // Refresh Gamma Page
    if (page_map.contains(MODULE_NAME_GAMMA) && page_map[MODULE_NAME_GAMMA] != nullptr) {
        GammaPageWidget *page = static_cast<GammaPageWidget *>(page_map[MODULE_NAME_GAMMA]);
        page->slotRefreshChart();
    }

    // Refresh vo Gamma Page
    if (page_map.contains(MODULE_NAME_VO_GAMMA) && page_map[MODULE_NAME_VO_GAMMA] != nullptr) {
        VOGammaPageWidget *vo_gamma_page = static_cast<VOGammaPageWidget *>(page_map[MODULE_NAME_VO_GAMMA]);
        vo_gamma_page->slotRefreshChart();
    }
}

void MainWindow::saveFile(QString filePath)
{
    QString selectedFilter;
    shared_ptr<GlobalData> global_data = GlobalData::getInstance();

    if (filePath.isEmpty()) {
        QString save_dir = global_data->getSettings(SETTINGS_SECTION, KEY_SAVE_PATH).toString();
        if (!save_dir.isEmpty()) {
            QDir dir(save_dir);
            if(dir.exists()) {
                filePath = QFileDialog::getSaveFileName(this, tr("Save Settings file"), save_dir,
                    tr("Json files (*.json)"), &selectedFilter);
            } else {
                filePath = QFileDialog::getSaveFileName(this, tr("Save Settings file"), "",
                    tr("Json files (*.json)"), &selectedFilter);
            }
        } else {
            filePath = QFileDialog::getSaveFileName(this, tr("Save Settings file"), "",
                tr("Json files (*.json)"), &selectedFilter);
        }
        if (!filePath.isEmpty()) {
            QFileInfo fileInfo(filePath);
            QString filDir = fileInfo.absolutePath();
            global_data->setSettings(SETTINGS_SECTION, KEY_SAVE_PATH, filDir);
            global_data->saveSettings();
        }
    }

    if (filePath.isEmpty()) {
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return;
    }

    QJsonObject all_data;
    QMap<QString, BasePageWidget *>::const_iterator i = page_map.constBegin();
    while (i != page_map.constEnd()) {
        QString module_name = i.key();
        QJsonArray data = i.value()->GetModuleObjectToLocal();
        if (!data.isEmpty()) {
            all_data[module_name] = data;
        }
        i++;
    }
    //save jason info
    QString soc = GlobalData::getInstance()->getCvipqToolSoc();
    QString template_string = GlobalData::getInstance()->getCvipqToolTemplate();
    QString time = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    QJsonArray data;
    QJsonObject soc_info
    {
        {KEY_REG_KEY, SOC_STRING},
        {KEY_VALUE, soc}
    };

    QJsonObject tool_version
    {
        {KEY_REG_KEY, CVIPQ_TOOL_VERSION_STRING},
        {KEY_VALUE, CVIPQ_TOOL_VERSION}
    };

    QJsonObject param_version
    {
        {KEY_REG_KEY, PARAM_VERSION_STRING},
        {KEY_VALUE, template_string}
    };

    QJsonObject date_time
    {
        {KEY_REG_KEY, DATE_TIME_STRING},
        {KEY_VALUE, time}
    };

    data.push_back(soc_info);
    data.push_back(tool_version);
    data.push_back(param_version);
    data.push_back(date_time);
    all_data["JSON Info"] = data;


    utils::saveAllParametersJson(filePath, all_data);

    qDebug("target save file:%s %s", qUtf8Printable(filePath), qUtf8Printable(selectedFilter));
}

void MainWindow::connectDevice()
{
    emit openConnectDeviceWindow();
}

void MainWindow::disconnectDevice()
{
#ifndef TCP_SOCKET_CLIENT
    NetworkClientHelper::getInstance()->disconnect(true);
#else
    if (auto_apply_timer && auto_apply_timer->isActive()) {
        auto_apply_timer->stop();
    }
    shared_ptr<GlobalData> global_data = GlobalData::getInstance();
    QString connect_type = global_data->getSettings(DEVICE_SECTION, KEY_CONNECT_TYPE).toString();

    if (connect_type == CONNECT_TYPE_UART) {
        QJsonObject id_info;
        id_info[MSG_KEY_CMD] = CMD_DISCONNECT_FROM_UART;
        int id = GlobalData::getInstance()->getJsonrpcId();
        NetworkClientHelper::getInstance()->pushIdInfoJsonrpc(id, id_info);

        QJsonObject send_obj;
        send_obj[MSG_KEY_METHOD] = METHOD_DISCONNECT_FROM_UART;
        send_obj[MSG_KEY_ID] = id;
        send_obj[MSG_KEY_JSONRPC] = MSG_JSONRPC_VERSION;

        NetworkClientHelper::getInstance()->sendCmd(std::move(send_obj));
        NetworkClientHelper::getInstance()->waitForUartSend();
    }
    NetworkClientHelper::getInstance()->disconnectJsonrpc(true);
#endif
}

void MainWindow::IPAddressOrUartPortChanged()
{
    updateWindowTitle();
}

void MainWindow::handleUpdatePages()
{
    QString selected_soc = GlobalData::getInstance()->getCvipqToolSoc();
    QString selected_template = GlobalData::getInstance()->getCvipqToolTemplate();
    if ((selected_template != using_tool_template) && (!using_tool_template.isEmpty())) {
        QString message = "Do you want to save all parameter values as json file?";
        if (QMessageBox::question(this, "CviPQ Tool", message, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
            saveFileJsonrpc();
            GlobalData::getInstance()->setCvipqToolSoc(selected_soc);
            GlobalData::getInstance()->setCvipqToolTemplate(selected_template);
        }
    } else if (selected_template == using_tool_template) {
        QString message = QString("Still use the current selected tool definition!");
        emit sigStatusLog(message, LOG_LEVEL::LOG_INFO);
    }

    setAllPageState(false);
    if (selected_template != using_tool_template) {
        status_log_widget->clearLog();
        GlobalData::getInstance()->setStatusTemplate(true);
        updatePages();
        using_tool_template = GlobalData::getInstance()->getCvipqToolTemplate();
        QString file_path = TEMPLATES_FOLDER + using_tool_template;
        loadDataFromNewJsonFormat(file_path);
    }
    record_index = -1;
    record_json_list.clear();
    doRecordAction();

#ifndef TCP_SOCKET_CLIENT
    NetworkClientHelper::getInstance()->connect();
#else
    this->page_info = getChildPageInfo();
    NetworkClientHelper::getInstance()->connectJsonrpc(CONNECTION_MODE::NORMAL);
#endif
}

void MainWindow::handleShowSystemConsole()
{
    SystemConsoleDialog *system_console_dialog = new SystemConsoleDialog();
    system_console_dialog->show();
    if (QDialog::Accepted == system_console_dialog->exec()) {
        QString console_input = system_console_dialog->textValue();
        updateSystemParam(console_input);
    }
}

void MainWindow::handleDataModified(SecKeyType)
{
    setRegisterModified(true);
}

void MainWindow::dataUpdated()
{
    //update items
    for (int i = 0; i < module_settings_tab_widget->count(); i++) {
        BasePageWidget *page = qobject_cast<BasePageWidget *>(module_settings_tab_widget->widget(i));
        if (page) {
            page->updateUI();
        }
    }
    //update tab pages
    updateTabPageVisible();

    //update modules
    module_list_widget->updateModuleListVisible();

    update();
}

void MainWindow::onFetchCompleted()
{
    if (ev_show_state_prev) {
        ev_show_state = ev_show_state_prev;
        ev_show_state_prev = false;
    }
    updateEvShowState(ev_show_state);
}

void MainWindow::fetchStarted()
{
    if (ev_show_state) {
        ev_show_state_prev = ev_show_state;
    }

    updateEvShowState(false);
}

void MainWindow::handleAutoApplyTimeout()
{
    if (auto_apply_in_progress) {
        return;
    }
    if (auto_apply_path_cache.isEmpty() || !QFileInfo::exists(auto_apply_path_cache)) {
        status_log_widget->handleAppenLog("Auto apply json path is invalid.", LOG_LEVEL::LOG_WARNING);
        return;
    }

    auto_apply_in_progress = true;
    openFile(auto_apply_path_cache, false);
    writeAllData();
    status_log_widget->handleAppenLog("Auto apply json executed.", LOG_LEVEL::LOG_INFO);

    QTimer::singleShot(500, this, [this]() {
        auto_apply_in_progress = false;
    });
}

void MainWindow::fetchRawData()
{
    if (fetch_image_window == nullptr) {
        fetch_image_window = new RawImageFetchWindow();
    }

    connect(fetch_image_window, &RawImageFetchWindow::fetchCompleted, this, &MainWindow::onFetchCompleted);
    connect(fetch_image_window, &RawImageFetchWindow::fetchStart, this, &MainWindow::fetchStarted);

    fetch_image_window->show();
    fetch_image_window->activateWindow();
}

void MainWindow::showCaptureWindow()
{
    if (capture_window == nullptr) {
        capture_window = new CaptureWindow();
        connect(NetworkClientHelper::getInstance().get(), &NetworkClientHelper::sigReadAllFinished,
            capture_window, &CaptureWindow::handleReadAllFinished);
    }
    connect(capture_window, &CaptureWindow::fetchCompleted, this, &MainWindow::onFetchCompleted);
    connect(capture_window, &CaptureWindow::fetchStart, this, &MainWindow::fetchStarted);
    connect(capture_window, &CaptureWindow::sigStatusLog, status_log_widget, &StatusLogWidget::handleAppenLog);

    capture_window->show();
    capture_window->activateWindow();
}

void MainWindow::showRawUtilWindow()
{
    if (raw_util_window == nullptr) {
        raw_util_window = new RawUtilWindow();
    }

    raw_util_window->show();
    raw_util_window->activateWindow();
}

void MainWindow::showYuvUtilWindow()
{
    if (yuv_util_window == nullptr)
    {
        yuv_util_window = new YuvUtilWindow();
    }

    yuv_util_window->show();
    yuv_util_window->activateWindow();
}

void MainWindow::showCModelWindow()
{
    if (cmodel_window == nullptr) {
        cmodel_window = new CModelImageFetchWindow();
    }

    cmodel_window->show();
    cmodel_window->activateWindow();
}

void MainWindow::showCalibrationDistortioWindow()
{
    if (distortion_window == nullptr) {
        distortion_window = new CalibrationDistortionWindow();
    }

    if (distortion_window->isMinimized()) {
        distortion_window->showNormal();
    }
    distortion_window->show();
    distortion_window->activateWindow();
}

void MainWindow::showCModelCtrlWindow()
{
    if (cmodel_ctrl_window == nullptr) {
        cmodel_ctrl_window = new CModelCtrlWindow();
    }

    if (cmodel_ctrl_window->isMinimized()) {
        cmodel_ctrl_window->showNormal();
    }
    cmodel_ctrl_window->show();
    cmodel_ctrl_window->activateWindow();
}

void MainWindow::showCalibrationCaptureWindow()
{
    calibration_capture_window->show();
    calibration_capture_window->activateWindow();
}

void MainWindow::showImportCalibrationJsonWindow()
{
    if (import_calibration_window == nullptr) {
        import_calibration_window = new ImportCalibrationJsonWindow();
    }

    connect(import_calibration_window,
            &ImportCalibrationJsonWindow::sigImportCalibration,
            this,
            &MainWindow::loadCaliData);

    import_calibration_window->show();
    import_calibration_window->activateWindow();

}

void MainWindow::showCalibrationPdafWindow()
{
    if (calibration_pdaf_window == nullptr) {
        calibration_pdaf_window = new CalibrationPdafWindow();
    }

    calibration_pdaf_window->show();
    calibration_pdaf_window->activateWindow();

}

void MainWindow::showCalibrationWindow()
{
    if (calibration_window == nullptr) {
        calibration_window = new CalibrationWindow();
        connect(calibration_window, &CalibrationWindow::sigCloseWindow, [=]{
            delete calibration_window;
            calibration_window = nullptr;
        });
    }

    if (calibration_window->isMinimized()) {
        calibration_window->showNormal();
    }

    calibration_window->show();
    calibration_window->activateWindow();
}

void MainWindow::toggleShowRegisterPanel()
{
    register_widget->isVisible() ? register_widget->setVisible(false) : register_widget->setVisible(true);
}

void MainWindow::handleIniUpdateReg(QString section, QString key, QString value_str)
{
    BasePageWidget::setValueToGlobalData(section, key, value_str);
}

void MainWindow::handleIniFinishReadAllData()
{
    foreach (auto page, page_map) {
        page->setStatusBusy(false);
    }
}

void MainWindow::initLanguageMenu()
{
    languages_menu = new QMenu();
    menuBar()->addMenu(languages_menu);
    QAction *test = new QAction("English");
    test->setCheckable(true);
    test->setChecked(true);
    connect(test, &QAction::triggered, this, bind(&MainWindow::changeLanguage, this, LANG_EN));
    languages_menu->addAction(test);

    test = new QAction("繁體中文");
    test->setCheckable(true);
    connect(test, &QAction::triggered, this, bind(&MainWindow::changeLanguage, this, LANG_ZH_TW));
    languages_menu->addAction(test);

    test = new QAction("简体中文");
    test->setCheckable(true);
    connect(test, &QAction::triggered, this, bind(&MainWindow::changeLanguage, this, LANG_ZH_CN));
    languages_menu->addAction(test);
}

void MainWindow::changeLanguage(LANG lang)
{
    for (int i = 0; i < languages_menu->actions().count(); i++) {
        QAction *action = languages_menu->actions()[i];

        action->setChecked(i == lang);
    }

    GlobalData::getInstance()->setSettings(SETTINGS_SECTION, KEY_LANG, QString::number(lang));
    GlobalData::getInstance()->saveSettings();

    QLocale locale;
    if (lang == LANG_EN) {
        locale = QLocale(QLocale::English);
    } else if (lang == LANG_ZH_TW) {
        locale = QLocale(QLocale::Chinese, QLocale::Taiwan);
    } else if (lang == LANG_ZH_CN) {
        locale = QLocale(QLocale::Chinese, QLocale::China);
    }

    qApp->removeTranslator(&appTranslator);
    appTranslator.load(locale, "isp_tool", ".", ":/languages");
    qApp->installTranslator(&appTranslator);
}

void MainWindow::refresh()
{
    // menu actions
    QString connect_type = GlobalData::getInstance()->getSettings(DEVICE_SECTION, KEY_CONNECT_TYPE).toString();
    if (CONNECT_TYPE_NETWORK == connect_type) {
        raw_data_action->setEnabled(true);
        capture_action->setEnabled(true);
        bin_util_action->setEnabled(true);
        fish_eye_action->setEnabled(true);
        auto_analyser_action->setEnabled(true);
        focus_assistant_action->setEnabled(true);
        bracketing_action->setEnabled(true);
        raw_util_action->setEnabled(true);
        yuv_util_action->setEnabled(true);
        cmodel_action->setDisabled(true);
    } else if (CONNECT_TYPE_CMODEL == connect_type) {
        raw_data_action->setDisabled(true);
        capture_action->setDisabled(true);
        bin_util_action->setDisabled(true);
        fish_eye_action->setDisabled(true);
        auto_analyser_action->setDisabled(true);
        focus_assistant_action->setDisabled(true);
        bracketing_action->setDisabled(true);
        raw_util_action->setDisabled(true);
        yuv_util_action->setDisabled(true);
        cmodel_action->setEnabled(true);
    }
}

void MainWindow::updateSystemParam(QString str)
{
    QStringList str_list = str.split("=");
    if (str_list.size() != 2){
        status_log_widget->appenLog("Set system param format error.", LOG_LEVEL::LOG_ERROR);
        return;
    }
    QString param = str_list[0];
    QString value = str_list[1];

    status_log_widget->appenLog("Set system param:" + str, LOG_LEVEL::LOG_INFO);

    if (param == "page_mode") {
        if (value == "0") {
            GlobalData::getInstance()->setPageMode(0);
            dataUpdated();
        } else if(value == "1") {
            GlobalData::getInstance()->setPageMode(1);
            dataUpdated();
        }
    }
    // reg level
    if (param == "reg_level") {
        if (value == "0") {
            GlobalData::getInstance()->setLevel(0);
            dataUpdated();
            raw_util_action->setVisible(false);
            cmodel_ctrl_action->setVisible(false);
        } else if (value == "1") {
            GlobalData::getInstance()->setLevel(1);
            dataUpdated();
        } else if (value == "2") {
            GlobalData::getInstance()->setLevel(2);
            dataUpdated();
            raw_util_action->setVisible(true);
            cmodel_ctrl_action->setVisible(true);
        }
    }
    // log level
    if (param == "log_level") {
        if (value == "debug") {
            status_log_widget->setLogLevel(LOG_LEVEL::LOG_DEBUG);
        } else if (value == "error") {
            status_log_widget->setLogLevel(LOG_LEVEL::LOG_ERROR);
        } else if (value == "info") {
            status_log_widget->setLogLevel(LOG_LEVEL::LOG_INFO);
        } else if (value == "warning") {
            status_log_widget->setLogLevel(LOG_LEVEL::LOG_WARNING);
        }
    }
}

void MainWindow::updateTabPageVisible()
{
    for (int i = 0; i < module_settings_tab_widget->count(); ) {
        BasePageWidget *page = qobject_cast<BasePageWidget *>(module_settings_tab_widget->widget(i));
        if (!page->getReadyFlag() && GlobalData::getInstance()->getPageMode() == 0) {
            module_settings_tab_widget->removeTab(i);
        } else {
            i++;
        }
    }
}

void MainWindow::removeTabPage(QString id)
{
    for (int i = 0; i < module_settings_tab_widget->count(); i++) {
        QString name = module_settings_tab_widget->tabText(i);
        if (name  == id) {
            module_settings_tab_widget->removeTab(i);
            break;
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (QMessageBox::question(this, tr("Quit"), tr("Are you sure to quit CviPQ Tool?"),
                              QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
    {
        event->accept();
        qApp->quit();
    }
    else {
        event->ignore();
    }
}

void MainWindow::showBinUtilWindow()
{
    if (bin_util_window == nullptr) {
        bin_util_window = new BinUtilWindow();
    }

    bin_util_window->show();
    bin_util_window->activateWindow();
}

void MainWindow::showFishEyeWindow()
{
    if (fisheye_util_window == nullptr) {
        fisheye_util_window = new FishEyeWindow();
    }

    fisheye_util_window->show();
    fisheye_util_window->activateWindow();
}

void MainWindow::showAutoAnalyserWindow()
{
    auto_analyser_window->show();
    auto_analyser_window->activateWindow();
}

void MainWindow::showFocusAssistantWindow()
{
    focus_assistant_window->show();
    focus_assistant_window->activateWindow();
}

void MainWindow::showBracketingWindow()
{
    if (bracketing_window == nullptr)
    {
        bracketing_window = new BracketingWindow();
    }

    bracketing_window->show();
    bracketing_window->activateWindow();
}

void MainWindow::showContinuousRawWindow()
{
    if (continuous_raw_window == nullptr)
    {
        continuous_raw_window = new ContinuousRawWindow();
    }

    continuous_raw_window->show();
    continuous_raw_window->activateWindow();
}

void MainWindow::showDrcToneViewerWindow()
{
    if (drc_tone_viewer_window == nullptr) {
        drc_tone_viewer_window = new DrcToneViewerWindow();
        drc_tone_viewer_window->move(120, 8);
    }

    drc_tone_viewer_window->show();
    drc_tone_viewer_window->activateWindow();
}

void MainWindow::showAnalyzerWindow()
{
    if (raw_analyzer_window == nullptr) {
        raw_analyzer_window = new RawAnalyzerWindow();
    }

    raw_analyzer_window->show();
    raw_analyzer_window->activateWindow();
}

void MainWindow::showI2cEditorWindow()
{
    if (i2c_editor_window == nullptr) {
        i2c_editor_window = new IICEditorWindow();
    }

    i2c_editor_window->show();
    i2c_editor_window->activateWindow();
}

void MainWindow::showRegToolWindow()
{
    if (reg_tool_window == nullptr) {
        reg_tool_window = new RegToolWindow();
    }

    reg_tool_window->show();
    reg_tool_window->activateWindow();
}

void MainWindow::showSpiEditorWindow()
{
    if (spi_editor_window == nullptr) {
        spi_editor_window = new SPIEditorWindow();
    }

    spi_editor_window->show();
    spi_editor_window->activateWindow();
}

void MainWindow::handleGetChipInfo(int id)
{
    int board_chip_id = -1;
    QJsonObject obj = NetworkClientHelper::getInstance()->popCmdDataJsonrpc(id);
    QJsonObject result_obj = obj[MSG_KEY_RESULT].toObject();
    QJsonObject params_obj = result_obj[MSG_KEY_PARAMS].toObject();
    if (params_obj.contains("Chip ID")) {
        board_chip_id = params_obj["Chip ID"].toInt();
    } else {
        QMessageBox::critical(this, "Error", "Failed to get chip info.", QMessageBox::Ok);
#ifndef TCP_SOCKET_CLIENT
        NetworkClientHelper::getInstance()->disconnect(true);
#else
        NetworkClientHelper::getInstance()->disconnectJsonrpc(true);
#endif
        return;
    }

    QString tool_branch = GlobalData::getInstance()->getCvipqToolBranch();
    QString board_branch = params_obj["ISP Branch"].toString();
    qDebug("tool_branch = %s, board_branch = %s", tool_branch.toStdString().c_str(), board_branch.toStdString().c_str());
    if (tool_branch != board_branch) {
        QString message = QString("Version mismatch, please update tool json!\n\nCurrent tool json is for  ''%1'',\
                                   \nbut ISP branch is  ''%2''\n").arg(tool_branch, board_branch);
        QMessageBox::warning(this, "Warning", message, QMessageBox::Ok);
    }

    bool is_matched = false;
    QString tool_soc = GlobalData::getInstance()->getCvipqToolSoc();
    qDebug("tool_soc = %s, board_chip_id = %d", tool_soc.toStdString().c_str(), board_chip_id);
    if (tool_soc == CV181X_SOC) {
        MARS_CHIP_ID id = (MARS_CHIP_ID) board_chip_id;
        if ((id >= MARS_CHIP_ID::CV1820A) && (id <= MARS_CHIP_ID::CV1813H))
        {
            is_matched = true;
        }
    } else if (tool_soc == CV180X_SOC) {
        MARS_CHIP_ID id = (MARS_CHIP_ID) board_chip_id;
        if ((id >= MARS_CHIP_ID::CV1800B) && (id <= MARS_CHIP_ID::CV1801C))
        {
            is_matched = true;
        }
    } else if (tool_soc == CV182X_SOC) {
        CHIP_ID id = (CHIP_ID) board_chip_id;
        if ((id == CHIP_ID::CVI1820) || (id == CHIP_ID::CVI1821) || (id == CHIP_ID::CVI1822) ||
            (id == CHIP_ID::CVI1823) || (id == CHIP_ID::CVI1825) || (id == CHIP_ID::CVI1826))
        {
            is_matched = true;
        }
    } else if (tool_soc == CV186X_SOC) {
        MARS_CHIP_ID id = (MARS_CHIP_ID) board_chip_id;
        if ((id >= MARS_CHIP_ID::BM1686) && (id <= MARS_CHIP_ID::CV186AH))
        {
            is_matched = true;
        }
    } else if (tool_soc == CV184X_SOC) {
        MARS_CHIP_ID id = (MARS_CHIP_ID) board_chip_id;
        if (id == MARS_CHIP_ID::CV184AH)
        {
            is_matched = true;
        }
    }

    if (is_matched == false) {
        QString chip_id_string;
        if ((tool_soc == CV180X_SOC) || (tool_soc == CV181X_SOC) || (tool_soc == CV184X_SOC) || (tool_soc == CV186X_SOC)) {
            chip_id_string = mars_chip_string_map[(MARS_CHIP_ID) board_chip_id];
        } else if (tool_soc == CV182X_SOC) {
            chip_id_string = chip_string_map[(CHIP_ID)board_chip_id];
        }

        QString message;
        if (chip_id_string.isEmpty()) {
            message = QString("SoC mismatch. Please check current template and board!\n \
                \nCurrent template is for %1\n\nBoard's SoC is invalid, its chip id: %2\n").arg(tool_soc, QString::number(board_chip_id));
        } else {
            message = QString("SoC mismatch. Please check current template and board!\n \
                \nCurrent template is for %1\n\nBoard's SoC is %2\n").arg(tool_soc, chip_id_string);
        }
        QMessageBox::critical(this, "Error", message, QMessageBox::Ok);
#ifndef TCP_SOCKET_CLIENT
        NetworkClientHelper::getInstance()->disconnect(true);
#else
        NetworkClientHelper::getInstance()->disconnectJsonrpc(true);
#endif
    } else {
        // Todo: Enable/Disable AEGainSepCfg according to GainModeStatus
        // Get GainSepStatus from Exposure Info
        // BasePageWidget *exposure_info_page = page_map[MODULE_ID::EXPOSURE_INFO];
        // exposure_info_page->readData();
        bool auto_apply = GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_AUTO_APPLY_JSON, false).toBool();
        QString auto_apply_path = GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_AUTO_APPLY_JSON_PATH, "").toString();
        int auto_apply_interval = GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_AUTO_APPLY_INTERVAL_MS, 1000).toInt();
        if (auto_apply) {
            if (auto_apply_path.isEmpty() || !QFileInfo::exists(auto_apply_path)) {
                status_log_widget->handleAppenLog("Auto apply is enabled but json path is invalid.", LOG_LEVEL::LOG_WARNING);
            } else {
                auto_apply_path_cache = auto_apply_path;
                if (auto_apply_timer) {
                    auto_apply_timer->start(qMax(100, auto_apply_interval));
                }
                QTimer::singleShot(200, this, [this]() {
                    handleAutoApplyTimeout();
                });
            }
        }
    }

    setAllPageState(true);
}

void MainWindow::soltModuleListUpdate(QString text)
{
    QStringList modules = GlobalData::getInstance()->getModuleOrderInfo(text);
    if (modules.isEmpty()) {
        return;
    }

    updateModuleList(modules);
}

void MainWindow::updateModuleList(QStringList modules)
{
    GlobalData::getInstance()->resetSearchState();
    GlobalData::getInstance()->setPageOrderInfo(modules);
    QList<QTreeWidgetItem *> item_list;
    for (int i = 0; i < modules.size(); i++) {
        QString name = modules[i];
        QTreeWidgetItem *item = new QTreeWidgetItem({name});

        if (name == MODULE_NAME_CLUT) {
            // for create online page of clut
            QTreeWidgetItem *child_item = addChildWidgetItem(name, item);
            if (child_item != nullptr)
                item_list.append(child_item);
        }

        item->setData(0, Qt::UserRole, name);
        item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
        if (!ready_page_list.contains(name)) {
            item->setTextColor(0, QColor(190, 190, 190));
        }
        item_list.append(item);
    }

    module_settings_tab_widget->clear();
    BasePageWidget *page_widget = getPageWidget(modules[0]);
    module_settings_tab_widget->addTab(page_widget, page_widget->getTitle());

    module_list_widget->updateModuleList(item_list, ready_page_list);
    module_list_widget->selectedMenuItem(modules[0]);
}

void MainWindow::updatePages()
{
    if (GlobalData::getInstance()->initPageInfoJsonrpc() != 0) {
        QString message = "Please check the contents of template json.";
        QMessageBox::critical(this, "Template Json Parsing Error", message, QMessageBox::Ok);
        return;
    }

    module_frequency_box->setEnabled(false);
    if(!page_map.isEmpty()) {
        foreach (auto page, page_map) {
            delete page;
        }

        page_map.clear();
    }

    sub_page_map.clear();
    ready_page_list.clear();
    ready_page_list = GlobalData::getInstance()->getPageReadyList();

    QStringList modules;
    QList<ClassModulesType> order_info = GlobalData::getInstance()->getModuleOrderInfo();
    QStringList frequency_list;
    if (!order_info.isEmpty()) {
        modules.clear();
        for (int i = 0; i < order_info.size(); i++) {
            if (!order_info[i].first.isEmpty() && !order_info[i].second.isEmpty()) {
                frequency_list.append(order_info[i].first);
                if (modules.isEmpty()) {
                    modules = order_info[i].second;
                }
            }
        }
    } else {
        QString tool_soc = GlobalData::getInstance()->getCvipqToolSoc();
        MODULE_ID *page_order = nullptr;
        modules = GlobalData::getInstance()->getPageInfo();
        int size = 0;
        if (tool_soc == CV183X_SOC) {
            page_order = page_oder_183x;
            size = sizeof(page_oder_183x) / sizeof(page_oder_183x[0]);
        } else if (tool_soc == CV182X_SOC) {
            page_order = page_oder_182x;
            size = sizeof(page_oder_182x) / sizeof(page_oder_182x[0]);
        } else {
            page_order = page_oder_182xA;
            size = sizeof(page_oder_182xA) / sizeof(page_oder_182xA[0]);
        }
        for (int i = 0, j = 0; i < size; i++) {
            QString name = getModuleName(page_order[i]);
            if (modules.contains(name)) {
                modules.removeAll(name);
                modules.insert(j, name);
                j++;
            }
        }
        order_info.clear();
        frequency_list.clear();
        frequency_list.append("ALL");
        order_info.append(ClassModulesType("ALL", modules));
        GlobalData::getInstance()->setModuleOrderInfo(order_info);
    }

    initModulePage(modules[0]);
    if (ready_page_list.contains(MODULE_NAME_LSC) && modules[0] != MODULE_NAME_LSC) {
        initModulePage(MODULE_NAME_LSC);
    }
    if (ready_page_list.contains(MODULE_NAME_CLUT) && modules[0] != MODULE_NAME_CLUT) {
        initModulePage(MODULE_NAME_CLUT);
    }
    if (ready_page_list.contains(MODULE_NAME_DPC) && modules[0] != MODULE_NAME_DPC) {
        initModulePage(MODULE_NAME_DPC);
    }

    module_frequency_box->clear();
    module_frequency_box->addItems(frequency_list);
    module_frequency_box->setEnabled(true);

    updateModuleList(modules);
}

bool MainWindow::getAllPageState()
{
    return this->bAllPageReay;
}

void MainWindow::setAllPageState(bool state)
{
    this->bAllPageReay = state;
}

void MainWindow::handleToggleTheme()
{
    static bool dark_theme_enabled = false;
    QFile file(":qdarkstyle/dark/style.qss");

    if (!file.exists())   {
        qDebug("Unable to set stylesheet, file not found");
    } else {
        dark_theme_enabled = !dark_theme_enabled;
        file.open(QFile::ReadOnly | QFile::Text);
        QTextStream text_stream(&file);
        QApplication *app = static_cast<QApplication *>(QApplication::instance());
        if (dark_theme_enabled) {
            app->setStyleSheet(text_stream.readAll());
            theme_action->setText(tr("&Light Theme"));
        } else {
            app->setStyleSheet("");
            theme_action->setText(tr("&Dark Theme"));
        }

        emit sigToggleTheme(dark_theme_enabled);
    }
}

void MainWindow::saveParamFromBoard(QString dirPath)
{
    saveFile(dirPath + "/temp.json");
    readAllData();
    GlobalData::getInstance()->setRestoreFlag(true);
}

shared_ptr<MainWindow> MainWindow::getInstance()
{
    if (nullptr == MainWindow::m_instance) {
        MainWindow::m_instance = make_shared<MainWindow>();
    }

    return MainWindow::m_instance;
}

void MainWindow::saveFileJsonrpc(QString filePath)
{
    QString selectedFilter;
    shared_ptr<GlobalData> global_data = GlobalData::getInstance();

    if (filePath.isEmpty()) {
        QString save_dir = global_data->getSettings(SETTINGS_SECTION, KEY_SAVE_PATH).toString();
        if (!save_dir.isEmpty()) {
            QDir dir(save_dir);
            if(dir.exists()) {
                filePath = QFileDialog::getSaveFileName(this, tr("Save Settings file"), save_dir,
                    tr("Json files (*.json)"), &selectedFilter);
            } else {
                filePath = QFileDialog::getSaveFileName(this, tr("Save Settings file"), "",
                    tr("Json files (*.json)"), &selectedFilter);
            }
        } else {
            filePath = QFileDialog::getSaveFileName(this, tr("Save Settings file"), "",
                tr("Json files (*.json)"), &selectedFilter);
        }
        if (!filePath.isEmpty()) {
            QFileInfo fileInfo(filePath);
            QString filDir = fileInfo.absolutePath();
            global_data->setSettings(SETTINGS_SECTION, KEY_SAVE_PATH, filDir);
            global_data->saveSettings();
        }
    }

    if (filePath.isEmpty()) {
        return;
    }

    QFile saveFile(filePath);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        QString message = QString("Can't open file [%1]").arg(filePath);
        QMessageBox::critical(this, "Open File Error", message, QMessageBox::Ok);
        return;
    }

    QJsonObject template_obj;
    QString error_msg;
    QString template_path = TEMPLATES_FOLDER + GlobalData::getInstance()->getCvipqToolTemplate();
    if (utils::loadJson(template_path, template_obj, error_msg) != 0) {
        QMessageBox::critical(this, "Error", error_msg, QMessageBox::Ok);
        return;
    }

    QJsonObject data_obj;
    for (int i = 0; i < ready_page_list.size(); i++) {
        QString module_name = ready_page_list[i];
        if (module_name == "WB Info" || module_name == "Exposure Info") {
            continue;
        }
        if (page_map.contains(module_name) && page_map[module_name] != nullptr) {
            QJsonArray page_save_data = page_map[module_name]->GetPageSaveData();
            if (!page_save_data.isEmpty()) {
                QMap<QString, QJsonObject> tmp_map;
                for (int j = 0; j < page_save_data.size(); j++) {
                    QJsonObject item_obj = page_save_data[j].toObject();
                    QString path = item_obj["PATH"].toString();
                    tmp_map.insert(path, item_obj);
                }
                QJsonArray page_data;
                QStringList keys = tmp_map.keys();
                for (int j = 0; j < keys.size(); j++) {
                    page_data.append(tmp_map.value(keys[j]));
                }
                data_obj[module_name] = page_data;
            }
        } else {
            QJsonArray page_save_data = getParamsFromBuffer(module_name);
            if (!page_save_data.isEmpty()) {
                data_obj[module_name] = page_save_data;
            }
        }
    }

    // utils::saveAllParametersJsonJsonrpc(filePath, all_save_data);
    utils::saveTemplateWithData(saveFile, template_obj, data_obj);
    saveFile.close();

    qDebug("target save file:%s %s", qUtf8Printable(filePath), qUtf8Printable(selectedFilter));
}

int MainWindow::loadDataFromOldJsonFormat(QString file_path)
{
    QJsonObject load_data;
    QString error_msg;
    if (utils::loadJson(file_path, load_data, error_msg) != 0) {
        QMessageBox::critical(this, "Error", error_msg, QMessageBox::Ok);
        return -1;
    }

    if (!load_data.contains("JSON Info"))
        return -2;

    for (int i = 0; i < ready_page_list.size(); i++) {
        QString module_name = ready_page_list[i];
        BasePageWidget * page = nullptr;

        if (sub_page_map.values().contains(module_name))
            continue;

        if (page_map.contains(module_name) && page_map[module_name] != nullptr) {
            page = page_map[module_name];
        } else {
            page = initModulePage(module_name);
        }

        QJsonArray value;
        if (load_data.contains(module_name)) {
            value = load_data[module_name].toArray();
        }

        if (!value.isEmpty() && page != nullptr) {
            page->readDataFromOldJsonFormat(value);
        } else {
            // read only page, skip the warning
            if (module_name != "Exposure Info" && module_name != "WB Info") {
                status_log_widget->handleAppenLog(module_name + " Page --> Can't find save data in json file.", LOG_LEVEL::LOG_WARNING);
            }
        }
    }

    return 0;
}

int MainWindow::loadDataFromNewJsonFormat(QString file_path)
{
    QJsonObject load_data;
    QString error_msg;
    if (utils::loadJson(file_path, load_data, error_msg) != 0) {
        QMessageBox::critical(this, "Error", error_msg, QMessageBox::Ok);
        return -1;
    }

    if (!load_data.contains("SAVE DATA"))
        return -2;

    QJsonArray save_data_array = load_data["SAVE DATA"].toArray();
    for (int i = 0; i < ready_page_list.size(); i++) {
        QString module_name = ready_page_list[i];
        QJsonArray value;

        if (sub_page_map.values().contains(module_name))
            continue;

        for (int i = 0; i < save_data_array.count(); i++) {
            if (save_data_array[i].toObject().contains(module_name)) {
                QJsonObject temp_obj = save_data_array[i].toObject();
                value = temp_obj[module_name].toArray();
                break;
            }
        }

        if (!value.isEmpty()) {
            if (module_name == QString("Noise Profile")) {
                QJsonObject temp_obj = value[0].toObject();
                QString path = temp_obj["PATH"].toString();
                if (path == QString("ISP_CMOS_NOISE_CALIBRATION_S.CalibrationCoef")) {
                    value.removeFirst();
                    value = utils::convertNoiseProfileParamFormat(temp_obj["VALUE"].toArray());
                }
            }
        } else {
            // read only page, skip the warning
            if (module_name != "Exposure Info" && module_name != "WB Info") {
                status_log_widget->handleAppenLog(module_name + " Page --> Can't find save data in json file.", LOG_LEVEL::LOG_WARNING);
            }
            continue;
        }

        if (page_map.contains(module_name) && page_map[module_name] != nullptr) {
            page_map[module_name]->readDataFromNewJsonFormat(value);
        } else {
            loadParamsToBuffer(module_name, value);
        }
    }

    return 0;
}

void MainWindow::loadParamsToBuffer(QString id, QJsonArray &value)
{
    QJsonObject pageObj = GlobalData::getInstance()->getPageData(id);
    QJsonObject pageData = pageObj["data"].toObject();
    QStringList struct_list = pageData.keys();
    for (int i = 0; i < struct_list.size(); i++) {
        QString struct_id = struct_list[i];
        QJsonObject groupData = pageData[struct_id].toObject();
        QStringList keys = groupData.keys();
        for (int j = 0; j < keys.size(); j++) {
            QString path = keys[j];
            QJsonObject itemObj = groupData[path].toObject();
            for (int k = 0; k < value.size(); k++) {
                QJsonObject itemData = value[k].toObject();
                QString path1 = itemData["PATH"].toString();
                if (path == path1) {
                    if (itemData["VALUE"].type() == itemObj["VALUE"].type()) {
                        if (itemData["VALUE"].isArray() &&
                                itemData["VALUE"].toArray().size() != itemObj["VALUE"].toArray().size()) {
                            break;
                        }
                        itemObj.insert("VALUE", itemData["VALUE"]);
                        groupData.insert(path, itemObj);
                    }
                    break;
                }
            }
        }
        pageData.insert(struct_id, groupData);
    }
    pageObj.insert("data", pageData);
    pageObj.insert("change", true);
    GlobalData::getInstance()->setPageData(id, pageObj);

    if (id == MODULE_NAME_GAMMA) {
        for (int i = 0; i < value.size(); i++) {
            QJsonObject itemData = value[i].toObject();
            QString path = itemData["PATH"].toString();
            if (path == "PQT_GAMMA.DeaultTable" || path == "PQT_GAMMA.sRGBTable") {
                QJsonObject gammaObj = GlobalData::getInstance()->getGammaData();
                gammaObj.insert(path, itemData["VALUE"]);
                gammaObj.insert("change", true);
                GlobalData::getInstance()->setGammaData(gammaObj);
            }
        }
    }
}

int MainWindow::loadCaliData(const QString &filePath, const QStringList &selectedModules)
{
    QJsonObject load_data;
    QString error_msg;

    if (utils::loadJson(filePath, load_data, error_msg) != 0) {
        QMessageBox::critical(this, "Error", error_msg, QMessageBox::Ok);
        return -1;
    }

    if (!load_data.contains("SAVE DATA"))
        return -2;
    QJsonArray save_data_array = load_data["SAVE DATA"].toArray();

    for (int i = 0; i < selectedModules.size(); i++) {
        if (!ready_page_list.contains(selectedModules[i])){
            QMessageBox::critical(this, "Error", "The module " + selectedModules[i] + " is not ready.", QMessageBox::Ok);
            return -3;
        }
        QString module_name = selectedModules[i];
        QJsonArray value;
        for (int i = 0; i < save_data_array.count(); i++) {
            if (save_data_array[i].toObject().contains(module_name)) {
                QJsonObject temp_obj = save_data_array[i].toObject();
                value = temp_obj[module_name].toArray();
                break;
            }
        }
        if (!value.isEmpty()) {
            if (module_name == QString("Noise Profile")) {
                QJsonObject temp_obj = value[0].toObject();
                QString path = temp_obj["PATH"].toString();
                if (path == QString("ISP_CMOS_NOISE_CALIBRATION_S.CalibrationCoef")) {
                    value.removeFirst();
                    value = utils::convertNoiseProfileParamFormat(temp_obj["VALUE"].toArray());
                }
            }
            if (module_name == QString("LSC")) {
                value = filterLscParameters(value);
            }
            if (module_name == QString("WB Attr")) {
                value = filterAwbParameters(value);
            }
            if (module_name == QString("CCM")) {
                value = filterCcmParameters(value);
            }
        } else {
            status_log_widget->handleAppenLog(module_name + " Page --> Can't find save data in json file.", LOG_LEVEL::LOG_WARNING);
            continue;
        }
        if (page_map.contains(module_name) && page_map[module_name] != nullptr) {
            page_map[module_name]->readDataFromNewJsonFormat(value);
        } else {
            loadParamsToBuffer(module_name, value);
        }
    }

    return 0;
}

QJsonArray MainWindow::getParamsFromBuffer(QString id)
{
    QJsonArray dataArr;
    QJsonObject pageObj = GlobalData::getInstance()->getPageData(id);
    QJsonObject pageData = pageObj["data"].toObject();
    QStringList struct_list = pageData.keys();
    for (int i = 0; i < struct_list.size(); i++) {
        QString struct_id = struct_list[i];
        QJsonObject groupData = pageData[struct_id].toObject();
        QStringList keys = groupData.keys();
        for (int j = 0; j < keys.size(); j++) {
            QString path = keys[j];
            QJsonObject itemObj = groupData[path].toObject();
            QJsonObject itemObj1;
            itemObj1.insert("PATH", itemObj["PATH"]);
            itemObj1.insert("VALUE", itemObj["VALUE"]);
            dataArr.append(itemObj1);
        }
    }

    if (id == MODULE_NAME_GAMMA) {
        QJsonObject gammaObj = GlobalData::getInstance()->getGammaData();
        QJsonObject itemObj, itemObj1;
        itemObj.insert("PATH", "PQT_GAMMA.DeaultTable");
        itemObj.insert("VALUE", gammaObj["PQT_GAMMA.DeaultTable"]);
        itemObj1.insert("PATH", "PQT_GAMMA.sRGBTable");
        itemObj1.insert("VALUE", gammaObj["PQT_GAMMA.sRGBTable"]);
        dataArr.append(itemObj);
        dataArr.append(itemObj1);
    }

    return dataArr;
}

StatusLogWidget * MainWindow::getLogWidget()
{
    return this->status_log_widget;
}

QJsonArray MainWindow::filterLscParameters(const QJsonArray &originalValue)
{
    QJsonArray filteredValue;
    int lscSize = 0;


    for (const auto &item : originalValue) {
        QJsonObject paramObj = item.toObject();
        QString path = paramObj["PATH"].toString();
        if (path == "ISP_MESH_SHADING_GAIN_LUT_ATTR_S.Size") {
            lscSize = paramObj["VALUE"].toInt();
            filteredValue.append(paramObj);
            break;
        }
    }
    if (lscSize > 0) {
        for (const auto &item : originalValue) {
            QJsonObject paramObj = item.toObject();
            QString path = paramObj["PATH"].toString();
            for (int i = 0; i < lscSize; i++) {
                QString basePattern = QString("ISP_MESH_SHADING_GAIN_LUT_ATTR_S.LscGainLut[%1].").arg(i);
                if (path.startsWith(basePattern) &&
                    (path.endsWith("ColorTemperature") ||
                     path.endsWith("RGain") ||
                     path.endsWith("BGain") ||
                     path.endsWith("GGain"))) {
                    filteredValue.append(paramObj);
                    break;
                }
            }
        }
    }

    return filteredValue;
}

QJsonArray MainWindow::filterAwbParameters(const QJsonArray &originalValue)
{
    QJsonArray filteredValue;
    for (const auto &item : originalValue) {
        QJsonObject paramObj = item.toObject();
        QString path = paramObj["PATH"].toString();

        if (AWB_IMPORT_PATHS.contains(path)) {
            filteredValue.append(paramObj);
        }
    }
    return filteredValue;
}

QJsonArray MainWindow::filterCcmParameters(const QJsonArray &originalValue)
{
    QJsonArray filteredValue;
    int ccmTabNum = 0;

    for (const auto &item : originalValue) {
        QJsonObject paramObj = item.toObject();
        QString path = paramObj["PATH"].toString();
        if (path == "ISP_CCM_ATTR_S.stAuto.CCMTabNum") {
            ccmTabNum = paramObj["VALUE"].toInt();
            filteredValue.append(paramObj);
            break;
        }
    }

    if (ccmTabNum > 0) {
        for (const auto &item : originalValue) {
            QJsonObject paramObj = item.toObject();
            QString path = paramObj["PATH"].toString();
            for (int i = 0; i < ccmTabNum; i++) {
                QString basePattern = QString("ISP_CCM_ATTR_S.stAuto.CCMTab[%1].").arg(i);
                if (path.startsWith(basePattern) &&
                    (path.endsWith("ColorTemp") ||
                     path.endsWith("CCM"))) {
                    filteredValue.append(paramObj);
                    break;
                }
            }
        }
    }
    return filteredValue;
}

QTreeWidgetItem *MainWindow::addChildWidgetItem(QString name, QTreeWidgetItem *item)
{
    if (ready_page_list.contains(name)) {
        QString child_name = name + QString(" Online");
        QJsonObject obj = GlobalData::getInstance()->getPageInfo(name);
        GlobalData::getInstance()->insertPageInfo(child_name, obj);

        QTreeWidgetItem *child = new QTreeWidgetItem({child_name});

        child->setData(0, Qt::UserRole, {child_name});
        child->setIcon(0, QIcon(":/icons/online.png"));
        item->addChild(child);
        if (!ready_page_list.contains(child_name)) {
            ready_page_list.append(child_name);
            sub_page_map[name] = child_name;
        }
        return child;
    }
    return nullptr;
}

void MainWindow::doRecordAction()
{
    static int num = 0;
    QString folderPath = GroupItemWidget::getTempDirPath();
    QDir dir(folderPath);

    if (!dir.exists()) {
        if(dir.mkpath(".")) {
            qDebug() << "Folder created successfully";
        } else {
            qDebug() << "Failed to create folder:" << folderPath;
            return;
        }
    }
    QString filePath = folderPath + PATH_SEPARATOR + QString("record_temp_[%1].json").arg(num++);

    saveFileJsonrpc(filePath);
    record_json_list.insert(++record_index, filePath);

    while (record_json_list.count() > RECORD_JSON_MAX_SIZE) {
        QString filePath_pop = record_json_list.front();
        QFile file(filePath_pop);

        if (file.exists()) {
            file.remove();
        }
        record_json_list.pop_front();
        record_index--;
        if (record_index < 0)
            record_index = 0;
    }
    updateRecordActioVisible();
}

void MainWindow::updateRecordActioVisible(bool show_log)
{
    if (record_index > 0 && record_json_list.count() > 1) {
        undo_action->setEnabled(true);
    } else {
        undo_action->setDisabled(true);
    }
    if (record_index < record_json_list.count() - 1 && record_json_list.count() > 1) {
        redo_action->setEnabled(true);
    } else {
        redo_action->setDisabled(true);
    }
    if (status_log_widget->getRcordLogCheck() && show_log)
        emit sigStatusLog(QString("current record: ") + record_json_list[record_index], LOG_LEVEL::LOG_INFO);
}

void MainWindow::undoAction()
{
    if (record_index == 0 || record_json_list.count() <= 1) {
        return;
    }

    openFile(record_json_list[--record_index], false);
    updateRecordActioVisible(false);
    emit sigStatusLog(QString("apply record: ") + record_json_list[record_index], LOG_LEVEL::LOG_INFO);
}

void MainWindow::redoAction()
{
    if (record_index == record_json_list.count() - 1 || record_json_list.count() <= 1) {
        return;
    }

    openFile(record_json_list[++record_index], false);
    updateRecordActioVisible(false);
    emit sigStatusLog(QString("apply record: ") + record_json_list[record_index], LOG_LEVEL::LOG_INFO);
}
