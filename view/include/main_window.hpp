#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QTranslator>
#include <QMessageBox>

#include "drawer_widget.hpp"
#include "module_list_widget.hpp"
#include "status_log_widget.hpp"
#include "register_page_widget.hpp"
#include "bnr_page_widget.hpp"
#include "ccm_calibration_page_widget.hpp"
#include "module_tab_widget.hpp"
#include "calibration_window.hpp"
#include "register_widget.hpp"
#include "capture_window.hpp"
#include "raw_image_fetch_window.hpp"
#include "calibration_window.hpp"
#include "cmodel_image_fetch_window.hpp"
#include "cmodel_ctrl_window.hpp"
#include "bin_util_window.hpp"
#include "fisheye_utility_window.hpp"
#include "raw_util_window.hpp"
#include "register_edit_widget.hpp"
#include "auto_analyser_window.hpp"
#include "yuv_util_window.hpp"
#include "focus_assistant_window.hpp"
#include "bracketing_window.hpp"
#include "continuous_raw_window.hpp"
#include "drc_tone_viewer_window.hpp"
#include "calibration_capture_window.hpp"
#include "calibration_distortion_window.hpp"
#include "calibration_pdaf_window.hpp"
#include "iic_editor_window.hpp"
#include "register_tool_window.hpp"
#include "spi_editor_window.hpp"
#include "search_box_widget.hpp"
#include "raw_analyzer_window.hpp"
#include "import_calibration_json_window.hpp"

class IniReadWorker : public QThread
{
    Q_OBJECT

public:
    void run() override;

signals:
    void sigUpdateReg(QString section, QString key, QString value_str);
    void sigFinishRead();
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    enum LANG {
        LANG_EN,
        LANG_ZH_TW,
        LANG_ZH_CN,
    };

public:
    explicit MainWindow(QWidget *parent = nullptr);
    static shared_ptr<MainWindow> getInstance();
    ~MainWindow() override;

    void open(QString filePath, bool showSuccessfulMessage);
    void changeLanguage(LANG lang);
    void refresh();
    void saveParamFromBoard(QString dirPath);
    void readPageData(QString id);
    void writePageData(QString id);
    void setAllPageState(bool state);
    bool getAllPageState();
    void updateAllPageAutoWriteState(bool checked_state);
    void doRecordAction();
    BasePageWidget * getPageWidget(QString name);
    StatusLogWidget * getLogWidget();
    BasePageWidget * initModulePage(QString id);

protected:
    void changeEvent(QEvent *event) override;
    virtual void closeEvent(QCloseEvent *event) override;

private:
    void initUI();
    void initMenu();
    void initEvent();
    void initPages();
    void initLanguageMenu();
    void initAssitToolsMenu();
    void initCalibrtionToolsMenu();
    void retranslateUi();
    void setRegisterModified(bool);
    void updateSystemParam(QString str);
    void updateTabPageVisible();
    void updateRecordActioVisible(bool show_log = true);

    void updateWindowTitle();
    void updatePages();
    BasePageWidget *createPage(QString id);
    void handleShortCut();
    QPair<int, int> getChildPageInfo();
    void removeTabPage(QString id);
    void updateTabBarWidth();
    QJsonObject GetModuleObjectToNetworkJsonrpc(QString id);
    bool SearchPageItem(QString module, QString name);
    QTreeWidgetItem *addChildWidgetItem(QString name, QTreeWidgetItem *item);

public slots:
    void dataUpdated();
    void IPAddressOrUartPortChanged();
    void handleUpdatePages();
    void handleShowSystemConsole();
    void handleDataModified(SecKeyType);
    void writeAllData();
    void openFile(QString filePath = "", bool showSuccessfulMessage = true);
    void saveFile(QString filePath = "");

    void saveFileJsonrpc(QString filePath = "");
    int loadDataFromOldJsonFormat(QString file_path);
    int loadDataFromNewJsonFormat(QString file_path);
    void loadParamsToBuffer(QString id, QJsonArray &value);
    void enableAllPageButtons(QString module, QString cmd, BUTTON_MODE mode);
    void disableAllPageButtons();
    void matchMenuItemofSelectedPage(int index);
    void handleSearchItem(QString item);
    QJsonArray getParamsFromBuffer(QString id);

signals:
    void openConnectDeviceWindow();
    void sigToggleTheme(bool dark_theme_enabled);
    void sigStatusLog(QString message, LOG_LEVEL log_level);

private slots:
    void readAllData();
    void readPageManual(QString id);
    void writePageManual(QString id);
    void selectModule(QString module);
    void connectDevice();
    void disconnectDevice();
    void fetchRawData();
    void showCaptureWindow();
    void showCModelWindow();
    void showCModelCtrlWindow();
    void showCalibrationWindow();
    void showCalibrationDistortioWindow();
    void showCalibrationCaptureWindow();
    void showImportCalibrationJsonWindow();
    void showCalibrationPdafWindow();
    void toggleShowRegisterPanel();
    void handleIniUpdateReg(QString section, QString key, QString value_str);
    void handleIniFinishReadAllData();
    void showBinUtilWindow();
    void showFishEyeWindow();
    void showRawUtilWindow();
    void showYuvUtilWindow();
    void showAutoAnalyserWindow();
    void showFocusAssistantWindow();
    void showBracketingWindow();
    void showContinuousRawWindow();
    void showAnalyzerWindow();
    void showI2cEditorWindow();
    void showRegToolWindow();
    void showSpiEditorWindow();
    void handleGetChipInfo(int id);
    void handleToggleTheme();
    void handleCheckGainSepStatus();
    void showDrcToneViewerWindow();
    void updateEvShowState(bool checked_state);
    void handleTimeOutEvent();
    void updateAllPageEvValue(int id);
    void handleAutoApplyTimeout();
    void searchItem(int page_order_idx, QString& title);
    void soltModuleListUpdate(QString text);
    void updateModuleList(QStringList modules);
    void onFetchCompleted();
    void fetchStarted();
    void undoAction();
    void redoAction();
    int loadCaliData(const QString &filePath, const QStringList &selectedModules);
    QJsonArray filterLscParameters(const QJsonArray &originalValue);
    QJsonArray filterCcmParameters(const QJsonArray &originalValue);
    QJsonArray filterAwbParameters(const QJsonArray &originalValue);

private:
    static shared_ptr<MainWindow> m_instance;
    QTranslator appTranslator;
    IniReadWorker *ini_read_worker;
    QList<QString> module_list;
    QPair<int, int> page_info;
    bool rw_all_page_flag = false;
    bool bAllPageReay = false;
    QString using_tool_template = "";
    int baseTabBarW = 0;
    int record_index = -1;

    QWidget *main_widget;
    QVBoxLayout *main_layout;
    RegisterEditWidget *register_widget;
    ModuleListWidget *module_list_widget = nullptr;
    QVBoxLayout *module_list_layout;
    ModuleTabWidget *module_settings_tab_widget;
    StatusLogWidget *status_log_widget;
    QMap<QString, BasePageWidget *> page_map;
    DrawerWidget *status_log_drawer = nullptr;
    SearchBoxWidget *search_box_widget = nullptr;
    QTimer *timer;
    QTimer *auto_apply_timer = nullptr;
    QString auto_apply_path_cache;
    bool auto_apply_in_progress = false;
    bool ev_show_state = false;
    bool ev_show_state_prev = false;

    QAction *open_file_action;
    QAction *save_file_action;
    QAction *connect_device_action;
    QAction *disconnect_device_action;
    QAction *raw_data_action;
    QAction *capture_action;
    QAction *raw_util_action;
    QAction *yuv_util_action;
    QAction *cmodel_action;
    QAction *cmodel_ctrl_action;
    QAction *calibration_capture_action;
    QAction *distortion_action;
    QAction *calibration_action;
    QAction *import_calibration_action;
    QAction *calibration_pdaf_action;
    QAction *register_action;
    QAction *theme_action;
    QMenu *languages_menu;
    QAction *bin_util_action;
    QAction *fish_eye_action;
    QAction *auto_analyser_action;
    QAction *focus_assistant_action;
    QAction *bracketing_action;
    QAction *continuous_raw_action;
    QAction *drc_tone_viewer_action;
    QAction *raw_analyzer_action;
    QAction *i2c_editor_action;
    QAction *reg_tool_action;
    QAction *spi_editor_action;
    QMenu *assit_tools_menu;
    QMenu *cali_tools_menu;
    QAction *undo_action;
    QAction *redo_action;

    CalibrationWindow *calibration_window{nullptr};
    CalibrationCaptureWindow *calibration_capture_window{nullptr};
    CalibrationDistortionWindow *distortion_window{nullptr};
    CalibrationPdafWindow *calibration_pdaf_window{nullptr};
    RawImageFetchWindow *fetch_image_window{nullptr};
    ImportCalibrationJsonWindow *import_calibration_window{nullptr};
    CaptureWindow *capture_window{nullptr};
    CModelImageFetchWindow *cmodel_window{nullptr};
    CModelCtrlWindow *cmodel_ctrl_window{nullptr};
    BinUtilWindow *bin_util_window{nullptr};
    FishEyeWindow *fisheye_util_window{nullptr};
    QStringList ready_page_list;
    QMap<QString, QString> sub_page_map;
    RawUtilWindow *raw_util_window{nullptr};
    YuvUtilWindow *yuv_util_window{nullptr};
    AutoAnalyserWindow *auto_analyser_window{nullptr};
    FocusAssistantWindow *focus_assistant_window{nullptr};
    BracketingWindow *bracketing_window{nullptr};
    ContinuousRawWindow *continuous_raw_window{nullptr};
    DrcToneViewerWindow *drc_tone_viewer_window{nullptr};
    RawAnalyzerWindow *raw_analyzer_window{nullptr};
    IICEditorWindow *i2c_editor_window{nullptr};
    RegToolWindow *reg_tool_window{nullptr};
    SPIEditorWindow *spi_editor_window{nullptr};
    QComboBox *module_frequency_box{nullptr};
    QStringList record_json_list;
    const QStringList AWB_IMPORT_PATHS = {
    "ISP_AWB_Calibration_Gain_S.u16ColorTemperature",
    "ISP_AWB_Calibration_Gain_S.u16AvgRgain",
    "ISP_AWB_Calibration_Gain_S.u16AvgBgain",
    "ISP_WB_ATTR_S.stAuto.u16RefColorTemp",
    "ISP_WB_ATTR_S.stAuto.au16StaticWB",
    "ISP_WB_ATTR_S.stAuto.as32CurvePara"
    };
};

#endif // MAIN_WINDOW_H
