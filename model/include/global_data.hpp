#ifndef GLOBAL_DATA_H
#define GLOBAL_DATA_H

#include <string>
#include <memory>
#include <QMap>
#include <QCoreApplication>
#include <QObject>
#include <QJsonObject>
#include "i_observer.hpp"
#include "module_define.hpp"
#include "base_item_widget.hpp"
#include "status_log_widget.hpp"

using namespace std;

#define TCP_SOCKET_CLIENT
const QString TEMPLATES_FOLDER = "templates/";

const QString CV183X_SOC = "CV183X";
const QString CV182X_SOC = "CV182X";
const QString CV181X_SOC = "CV181X";
const QString CV180X_SOC = "CV180X";
const QString CV186X_SOC = "CV186X";
const QString CV184X_SOC = "CV184X";
const QString CVIPQ_TOOL_VERSION = "v3.1.4.2";
const QString SOC_STRING = "SoC";
const QString CVIPQ_TOOL_VERSION_STRING = "CviPQ Tool Version";
const QString PARAM_VERSION_STRING = "Parameters Version";
const QString DATE_TIME_STRING = "DateTime";
const QString DEFAULT_SETTINGS_FILE_NAME = "settings.json";
const QString DEFAULT_DATA_FILE_NAME = "data.txt";

// board info keys
const QString KEY_CHIP_ID = "Chip ID";

// file keys
const QString KEY_CONNECT_TYPE = "connect_type";
const QString KEY_IP_ADDRESS = "ip_address";
const QString KEY_SOC = "soc";
const QString KEY_TEMPLATE = "template";
const QString KEY_PORT = "port";
const QString KEY_REMEMBER_SETTINGS = "remember_settings";
const QString KEY_VERSION = "version";
const QString KEY_MIN_VERSION = "min_version";
const QString KEY_VALUE = "value";
const QString KEY_SIZE = "size";
const QString KEY_VALUE_TYPE = "value_type";
const QString KEY_STRUCTURE = "structure";
const QString KEY_REG_SECTION = "section";
const QString KEY_REG_KEY = "key";
const QString KEY_REG_ADDRESS = "address";
const QString KEY_REG_BITMASK = "bitmask";
const QString KEY_REG_DESP = "desp";
const QString KEY_REG_OP_TYPE = "optype";
const QString KEY_REG_VALUE_TYPE = "reg_value_type";
const QString KEY_REG_LEVEL = "level";
const QString KEY_REG_GROUP = "group";
const QString KEY_LANG = "language";
const QString KEY_AUTO_WRITE = "auto_write";
const QString KEY_TITLE_LABEL_WIDTH = "title_width";
const QString KEY_CAPTURE_SAVE_PATH = "capture_save_path";
const QString KEY_BRACKET_SAVE_PATH = "bracket_save_path";
const QString KEY_CONTINUOUS_SAVE_PATH = "continuous_save_path";
const QString KEY_CALI_SAVE_PATH = "cali_save_path";
const QString KEY_UART_PORT = "uart_port";
const QString KEY_BAUD_RATE = "baud_rate";
const QString KEY_MAX_LUT_SHOW_COUNT = "max_lut_show_count";
const QString COLOR_BLOCKS_DETECT_LOWTHR = "color_blocks_detect_lowThr";
const QString COLOR_BLOCKS_DETECT_HIGHTHR = "color_blocks_detect_highThr";

const QString KEY_OPEN_PATH = "open_path";
const QString KEY_SAVE_PATH = "save_path";
const QString KEY_EV_SHOW = "ev_value_show";
const QString KEY_WDR_MODE = "wdrmode";
const QString KEY_CALI_CAPTURE_SAVE_PATH = "cali_capture_save_path";
const QString KEY_CALI_SAVE_ROOT_PATH = "cali_save_root_path";
const QString KEY_CALI_CONVERT_ROOT_PATH = "cali_convert_root_path";
const QString KEY_CALI_IMPORT_IMG_PATH = "cali_import_img_path";
const QString KEY_AUTO_APPLY_JSON = "auto_apply_json";
const QString KEY_AUTO_APPLY_JSON_PATH = "auto_apply_json_path";
const QString KEY_AUTO_APPLY_INTERVAL_MS = "auto_apply_interval_ms";
const QString KEY_CAPTURE_IMAGE_FORMAT = "capture_image_format";
const QString CONNECT_TYPE_CMODEL = "Offline";
const QString CONNECT_TYPE_NETWORK = "Network";
const QString CONNECT_TYPE_UART = "Uart";
const QString UART_PORT_COM1 = "COM1";
const QString BAUD_RATE_115200 = "115200";

const QString TYPE_ARRAY = "array";
const QString TYPE_ARRAY_FLOAT = "array_float";
const QString TYPE_ARRAY_2D = "array_2d";
const QString TYPE_ARRAY_FLOAT_2D = "array_float_2d";
const QString TYPE_INT = "int";
const QString TYPE_DOUBLE = "double";

const QString OP_TYPE = "OpType";
const QString OP_TYPE_AUTO = "Auto";
const QString OP_TYPE_MANUAL = "Manual";

const QString DEFAULT_SECTION = "";
const QString SETTINGS_SECTION = "settings";
const QString DEVICE_SECTION = "device";
const QString REGISTERS_SECTION = "registers";

const QString VERSION = "1.0.2";
const QString MIN_VERSION = "1.0.2";

const int DEFAULT_REG_VALUE = 0;

const QString NAME_ISO = "ISO";
const QList<int> iso_list {100, 200, 400, 800, 1600, 3200, 6400, 12800, 25600, 51200, 102400, 204800, 409600, 819200, 1638400, 3276800};
const QString NAME_LV = "LV";
const QList<int> lv_list {-5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

const QStringList ccm_horizon_headers {"R", "G", "B", "SUM"};
const QStringList ccm_vertical_headers {"R", "G", "B"};
const QString KEY_REG_CHMOD = "chmod";
const int MOD_RW = 0;
const int MOD_RO = 1;
const QString KEY_REG_INT_BASE = "int_base";
const int TOOL_REL_MODE = 0;
const int TOOL_ENGINEER_MODE = 1;
const int MAX_CONTROL_PT_NUM = 32;
const int MIN_CONTROL_PT_NUM = 2;
const QString CAPTURE_SRC_VI = "VI";
const QString CAPTURE_SRC_VPSS = "VPSS";
const QString CAPTURE_SRC_VO = "VO";
const QString ISO_DEFAULT_VALUE = "ISO:0000000";
const QString LV_DEFAULT_VALUE = " LV:0000000";

const QStringList bits_list = {"8bits", "10bits", "12bits", "16bits"};
const QStringList bayer_strlist =  {"BGGR", "GBRG", "GRBG", "RGGB","NULL","NULL","NULL","NULL","GRGBI","RGBGI","GBGRI","BGRGI","IGRGB","IRGBG","IBGGR","IGBGR","BUTT"};
const QStringList mode_strlist = { "Linear", "WDR" };
const QStringList bitwidth_list = {"8bits", "10bits"};
const QStringList pixeltype_list = {"YUYV", "NV21", "YUV420", "NV12"};
const QStringList videotype_list = {"LINEAR", "64x16", "16x8", "LINEAR_DISCRETE"};
const QStringList pixeltype_short_list = {"422", "420sp", "420p"};
const QStringList videotype_short_list = {"linear", "64x16", "16x8", "linear_dis"};
const QStringList MW_BAYER_FORMAT_STRING =  {"BGGR", "GBRG", "GRBG", "RGGB","NULL","NULL","NULL","NULL","GRGBI","RGBGI","GBGRI","BGRGI","IGRGB","IRGBG","IBGGR","IGBGR","BUTT"};
const QStringList baud_rate_list = {"1200", "2400", "4800", "9600", "19200", "38400", "57600", "115200", "230400"};

const int LSC_MAX_SIZE = 7;
const int MLSC_TABLE_SIZE = 1369;
const int RLSC_TABLE_SIZE = 32;
const int RLSC_TABLE_NUMBER = 4;  //R, G, B, IR
const int REFRESH_PERIOD_MS = 1000;
const int LBLC_BLOCK_SIZE = 16;
const int LBLC_BLOCK_NUMS = 64;
const int LBLC_MAX_SIZE = 7;
const int RECORD_JSON_MAX_SIZE = 50;

#define DATA_BLOCK_SIZE (256 * 1024)

enum class CHIP_ID {
    CVI1835 = 1,
	CVI1832 = 2,
	CVI1838 = 3,
	CVI1822 = 4,
	CVI1829 = 5,
	CVI1826 = 6,
    CVI1821 = 7,
    CVI1820 = 8,
    CVI1823 = 9,
    CVI1825 = 10
};

enum class MARS_CHIP_ID {
    CVI1822 = 0x0,
    CVI1832 = 0x1,
    CVI1835 = 0x2,
    CVI1838 = 0x3,
    CVI1829 = 0x4,
    CVI1826 = 0x5,
    CVI1821 = 0x6,
    CVI1820 = 0x7,
    CVI1823 = 0x8,
    CVI1825 = 0x9,
    CV1820A = 10,
    CV1821A = 11,
    CV1822A = 12,
    CV1823A = 13,
    CV1825A = 14,
    CV1826A = 15,
    CV1810C = 16,
    CV1811C = 17,
    CV1812C = 18,
    CV1811H = 19,
    CV1812H = 20,
    CV1813H = 21,
// cv180 chips
    CV1800B = 22,
    CV1801B = 23,
    CV1800C = 24,
    CV1801C = 25,
// athena2
    BM1686 = 26,
    CV186AH = 27,
// mars3
    CV184AH = 28
};

extern QMap<CHIP_ID, QString> chip_string_map;
extern QMap<MARS_CHIP_ID, QString> mars_chip_string_map;

enum MW_BAYER_FORMAT {
    BAYER_FORMAT_BG = 0,
	BAYER_FORMAT_GB,
	BAYER_FORMAT_GR,
	BAYER_FORMAT_RG,
    BAYER_FORMAT_MAX,
};

enum MW_PIXEL_FORMAT {
    PIXEL_FORMAT_RGB_888 = 0,
	PIXEL_FORMAT_BGR_888,
	PIXEL_FORMAT_RGB_888_PLANAR,
	PIXEL_FORMAT_BGR_888_PLANAR,

	PIXEL_FORMAT_ARGB_1555, // 4,
	PIXEL_FORMAT_ARGB_4444,
	PIXEL_FORMAT_ARGB_8888,

	PIXEL_FORMAT_RGB_BAYER_8BPP, // 7,
	PIXEL_FORMAT_RGB_BAYER_10BPP,
	PIXEL_FORMAT_RGB_BAYER_12BPP,
	PIXEL_FORMAT_RGB_BAYER_14BPP,
	PIXEL_FORMAT_RGB_BAYER_16BPP,

	PIXEL_FORMAT_YUV_PLANAR_422, // 12,
	PIXEL_FORMAT_YUV_PLANAR_420,
	PIXEL_FORMAT_YUV_PLANAR_444,
	PIXEL_FORMAT_YUV_400,

	PIXEL_FORMAT_HSV_888, // 16,
	PIXEL_FORMAT_HSV_888_PLANAR,

	PIXEL_FORMAT_NV12, // 18,
	PIXEL_FORMAT_NV21,
	PIXEL_FORMAT_NV16,
	PIXEL_FORMAT_NV61,
	PIXEL_FORMAT_YUYV,
	PIXEL_FORMAT_UYVY,
	PIXEL_FORMAT_YVYU,
	PIXEL_FORMAT_VYUY,

	PIXEL_FORMAT_FP32_C1 = 32, // 32
	PIXEL_FORMAT_FP32_C3_PLANAR,
	PIXEL_FORMAT_INT32_C1,
	PIXEL_FORMAT_INT32_C3_PLANAR,
	PIXEL_FORMAT_UINT32_C1,
	PIXEL_FORMAT_UINT32_C3_PLANAR,
	PIXEL_FORMAT_BF16_C1,
	PIXEL_FORMAT_BF16_C3_PLANAR,
	PIXEL_FORMAT_INT16_C1,
	PIXEL_FORMAT_INT16_C3_PLANAR,
	PIXEL_FORMAT_UINT16_C1,
	PIXEL_FORMAT_UINT16_C3_PLANAR,
	PIXEL_FORMAT_INT8_C1,
	PIXEL_FORMAT_INT8_C3_PLANAR,
	PIXEL_FORMAT_UINT8_C1,
	PIXEL_FORMAT_UINT8_C3_PLANAR,

    PIXEL_FORMAT_MAX,
};

enum MW_COPRESS_MODE {
    COMPRESS_MODE_NONE = 0,
	COMPRESS_MODE_TILE,
	COMPRESS_MODE_LINE,
	COMPRESS_MODE_FRAME,
    COMPRESS_MODE_MAX,
};

typedef enum {
    NONE_BIN_DATA = 0,
    AE_BIN_DATA,
    AWB_BIN_DATA,
    TUNING_BIN_DATA,
    RAW_BIN_DATA,
    TOOL_JSON_DATA,
    VI_LDC_BIN_DATA,
    VPSS_LDC_BIN_DATA
} BinDataType;

typedef enum {
    JSON_WAIT = 0,
    JSON_NONE,
    JSON_EXIST
} ToolJsonState;

typedef enum _WDR_MODE_E {
    WDR_MODE_NONE = 0,
    WDR_MODE_BUILT_IN,
    WDR_MODE_QUDRA,

    WDR_MODE_2To1_LINE,
    WDR_MODE_2To1_FRAME,
    WDR_MODE_2To1_FRAME_FULL_RATE,

    WDR_MODE_3To1_LINE,
    WDR_MODE_3To1_FRAME,
    WDR_MODE_3To1_FRAME_FULL_RATE,

    WDR_MODE_4To1_LINE,
    WDR_MODE_4To1_FRAME,
    WDR_MODE_4To1_FRAME_FULL_RATE,

    WDR_MODE_MAX,
} WDR_MODE_E;

typedef struct _YUV_HEADER {
  quint8 numFrame;
  quint16 width;
  quint16 height;
  quint16 stride[3];
  quint8 pixelFormat;
  quint8 curFrame;
  quint32 size;
} YUV_HEADER;

typedef struct _RAW_HEADER {
  quint8 numFrame;
  quint16 width;
  quint16 height;
  quint16 stride[3];
  quint8 curFrame;
  quint8 bayerID;
  quint8 compress;
  quint16 cropX;
  quint16 cropY;
  quint8 fusionFrame;
  quint32 exposure[4];
  quint32 ispDGain;
  quint32 iso;
  quint32 colorTemp;
  quint16 wbRGain;
  quint16 wbBGain;
  qint16 ccm[9];
  quint16 blcOffset[4];
  quint16 blcGain[4];
  quint32 exposureRatio;
  quint32 exposureAGain;
  quint32 exposureDGain;
  quint16 cropWidth;
  quint16 cropHeight;
  quint32 size;
} RAW_HEADER;

typedef struct _RECT_S {
    int x;
    int y;
    int w;
    int h;
} RECT_S;

typedef struct _RAW_INFO {
    int isValid;
    int numFrame;
    int curFrame;
    int width;
    int height;
    int bayerID;
    int enWDR;
    int ISO;
    float lightValue;
    int colorTemp;
    int ispDGain;
    int exposureRatio;
    int exposureAGain;
    int exposureDGain;
    int longExposure;
    int shortExposure;
    int WB_RGain;
    int WB_GGain;
    int WB_BGain;
    int CCM[9];
    int BLC_Offset[4];
    int BLC_Gain[4];
    int size;
    int roiFrameNum;
    RECT_S roiRect;
    int roiFrameSize;
    int op_mode;
    int AGainSF;
    int DGainSF;
    int ispDGainSF;
} RAW_INFO;

typedef struct _EXP_INFO {
    int ISO;
    float lightValue;
    int ispDGain_le;
    int ispDGain_se;
    int time_le;
    int time_se;
    int aGain_le;
    int aGain_se;
    int dGain_le;
    int dGain_se;
} EXP_INFO;

typedef enum {
    RAW_NORMAL,
    RAW_ROI
} RAW_TYPE;

using SecKeyType = QPair<QString, QString>;
using ModuleKeyType = QPair<QString, QString>;
using ItemValueType = QMap<QString, QVariant>;
using ClassModulesType = QPair<QString, QStringList>;

class GlobalData : public QObject
{
    Q_OBJECT

public:
    explicit GlobalData();
    static shared_ptr<GlobalData> getInstance();

    void setRegValue(const QString &section, const QString &key, const QVariant &value, bool publish = true, IObserver *from = nullptr);
    void setRegValueByIdx(const QString &section, const QString &key, int value_idx, const QVariant &value, bool publish = true);
    void setRegValueByIdx(const QString &section, const QString &key, const QString &op_type, int value_idx, const QVariant &value, bool publish = true);
    void setIsoRegValue(const QString &section, const QString &key, const QList<QVariant> &value_list, bool publish = true);
    void setIsoRegValueByIso(const QString &section, const QString &key, int iso, const QVariant &value, bool publish = true);
    void setIsoRegValueByIso(const QString &section, const QString &key, const QString &op_type, int iso, const QVariant &value, bool publish = true);
    void initIsoRegValue(const QString &section, const QString &key, int array_size, const QVariant &value);
    void initJsonArrayHelper(QJsonObject &json, const QString &section, const QString &key, int array_size, QJsonValue value);
    QVariant getRegValue(const QString &section, const QString &key, const QString default_value = "");
    QVariant getRegValue(const QString &section, const QString &key, int value_index, const QString default_value = "");
    QVariant getIsoRegValue(const QString &section, const QString &key, int iso, const QString default_value = "");
    QJsonObject getRegJson(const QString &section, const QString &key) const;
    QJsonObject getRegisters() const noexcept;
    void applyRegsValue(QJsonObject regs);
    bool loadData(const QString &file_path);

    void loadSettings();
    void saveSettings();

    void setSettings(const QString &section, const QString &key, const QVariant &value);
    QVariant getSettings(const QString &section, const QString &key, const QVariant default_value = "");

    void dump() const;

    void subscribeObserver(SecKeyType reg_section_key, IObserver *observer);
    void unSubscribeObserver(SecKeyType reg_section_key, IObserver *observer);
    void subscribeItem(ModuleKeyType &&item_module_key, BaseItemWidget *item);
    void unSubscribeItem(ModuleKeyType item_module_key, BaseItemWidget *item);
    void subscribeItemJsonrpc(QString path, BaseItemWidget *item);
    void unsubscribeItemJsonrpc(QString path, BaseItemWidget *item);

    void setLevel(int level);
    int getLevel();

    void setCvipqToolSoc(const QString &soc);
    QString getCvipqToolSoc();
    void setCvipqToolTemplate(const QString &template_name);
    QString getCvipqToolTemplate();
    void setCvipqToolBranch(const QString &branch);
    QString getCvipqToolBranch();

    void setPageMode(int mode);
    int getPageMode();
    void setStatusTemplate(bool status);
    bool getStatusTemplate();

    QJsonObject getPageInfo(QString id);
    QStringList getPageInfo();
    void insertPageInfo(QString id, QJsonObject page_info_obj);

    void setRestoreFlag(bool flag);
    bool getRestoreFlag();
    void setReadingPagesNumber(int number);
    int getReadingPagesNumber();

    int initPageInfoJsonrpc();
    void setJsonrpcId(int id) { jsonrpc_id = id; }
    int getJsonrpcId() { return ++jsonrpc_id; }
    void setWaitForResponse(bool enable) { waitForResponse = enable; }
    bool getWaitForResponse() { return waitForResponse; }
    void initUinvMatrixRegValue(const QString &section, const QString &key, const QJsonValue &value);
    QJsonValue getUnivMatrixRegValue(const QString &section, const QString &key);
    void setUnivMatrixRegValue(const QString &section, const QString &key, QJsonValue value, bool publish = true);
    quint8 GetWDRFusionFrame();
    quint8 CalculateWDRFusionFrame(WDR_MODE_E eWDRMode);
    void getToolJson();
    void handleToolJsonStatus(int id);
    void handleToolJsonData(QString cmd);
    void publishItemJsonrpc(QString path, QVariant data);
    void getChipInfo();
    void setCurrentHighlightItem(BaseItemWidget *item);
    BaseItemWidget* getCurrentHighlightItem();
    void setPreviousSearchString(QString str);
    QString& getPreviousSearchString();
    int getSearchTimesAtSamePage();
    void setSearchTimesAtSamePage(int search_times);
    int getSearchPageOrderIndx();
    void setSearchPageOrderIndx(int index);
    int getMatchSearchStringTimes();
    void setMatchSearchStringTimes(int times);
    void incMatchSearchStringTimes();
    QStringList getPageOrderInfo();
    void setPageOrderInfo(QStringList list);
    void incSearchRecursiveCallTimes();
    void resetSearchRecursiveCallTimes();
    int getSearchRecursiveCallTimes();
    void setBoardTooljsonState(ToolJsonState state);
    ToolJsonState getBoardTooljsonState();
    QList<ClassModulesType> getModuleOrderInfo();
    QStringList getModuleOrderInfo(QString key);
    void setModuleOrderInfo(QList<ClassModulesType> order);
    void resetSearchState();
    QStringList getPageReadyList();
    QJsonObject getPageData(QString id);
    void setPageData(QString id, QJsonObject data);
    QJsonObject getGammaData();
    void setGammaData(QJsonObject data);
    bool getPageStatus(QString id);
    void setPageStatus(QString id, bool status);
    void savePageData(QString id, QString struct_id, QJsonObject data);
    void loadPageData(QString id);
    QMap<QString, QPair<QString, QString>> getTopModuleTitleMap();

private:
    void initData();
    void setJsonHelper(QJsonObject &json, const QString &section, const QString &key, QJsonValue value);
    void setJsonArrayHelper(QJsonObject &json, const QString &section, const QString &key, int index, QJsonValue value);
    void setJsonValueHelper(QJsonObject &json, const QString &section, const QString &key, QJsonValue value);
    void setJsonValueHelper(QJsonObject &json, const QString &section, const QString &key, int value_index, QJsonValue value);
    QJsonValue getJsonHelper(QJsonObject &json, const QString &section, const QString &key, QJsonValue default_value = "");
    QJsonValue getJsonArrayHelper(QJsonObject &json, const QString &section, const QString &key, int index, QJsonValue default_value = "");
    QJsonValue getJsonValueHelper(QJsonObject &json, const QString &section, const QString &key, QJsonValue default_value = "");
    QJsonValue getJsonValueHelper(QJsonObject &json, const QString &section, const QString &key, int value_idx, QJsonValue default_value = "");

    void publishObserver(SecKeyType reg_section_key, QString op_type = "", IObserver *from = nullptr);
    void publishItem(ModuleKeyType item_module_key, QVariant data, QString op_type = "");
    void creatModuleOrderInfo(QJsonObject &tool_json);
    void creatTopModuleTitleMap(QJsonObject &tool_json);
    void creatModuleDefaultInfo(QJsonArray &obj);

private:
    static shared_ptr<GlobalData> m_instance;
    static QString settings_file_path;
    static QString cvipq_tool_soc;
    static QString cvipq_tool_template;
    static QString template_folder;
    static quint8 fusionFrame;
    static QString cvipq_tool_branch;
    static bool changetemplatestatus;
    BaseItemWidget *current_highlight_item = nullptr;
    QString previous_search_string;
    int search_same_page_times;
    int search_page_order_idx;
    QStringList page_order;
    int search_recursive_call_times = 0;
    int match_string_times = 0;

    QJsonObject data;
    QJsonObject iso_matrix_data;
    //QMap<int, QJsonObject> iso_data;
    QJsonObject settings;
    QMap<QString, QJsonObject> page_info;
    QMap<QString, QJsonObject> page_data;
    QJsonObject gammaObj;
    QMap<QString, bool> page_status;
    QList<ClassModulesType> module_order_info;
    QStringList page_ready_list;
    QMap<SecKeyType, QList<IObserver *>> observer_subscribe_list;
    QMap<ModuleKeyType, QList<BaseItemWidget *>> item_subscribe_list;
    QMap<QString, QPair<QString, QString>> topTitle_moduleTitle_map;

    int level{0};
    int page_mode{0};
    bool restore_flag = false;
    int reading_pages_number = 0;

    int jsonrpc_id = 0;
    bool waitForResponse = false;
    ToolJsonState boardExistTooljson = JSON_WAIT;
    QMap<QString, QList<BaseItemWidget *>> path_to_item_map;

signals:
    void sigDataLoaded();
    void sigDataModified(SecKeyType reg_section_key);
    void sigStatusLog(QString message, LOG_LEVEL log_level);
    void sigCheckGainSepStatus();
    void sigGetWDRFusionFrame();

    void sigUpdateTemplate(QString file_full_name);
    void sigModuleRWResult(QString module, QString cmd, QString result);
    void sigReceiveGamma();

private slots:
    void handleDataChanged(QString cmd);
    void handleReceiveWDRMode(int id);
    void handleDataChangedJsonrpc(int id);
    void handleReceiveGammaJsonrpc(int id);
};

#endif // GLOBAL_DATA_H
