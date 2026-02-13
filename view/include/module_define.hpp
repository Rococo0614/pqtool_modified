#ifndef MODULE_DEFINE_H
#define MODULE_DEFINE_H

#include <QMap>
#include <QString>
#include <QObject>

const QString MODULE_NAME_EXP_ATTR = "Exposure attr";
const QString MODULE_NAME_EXP_INFO = "Exposure Info";
const QString MODULE_NAME_STS_CFG = "Statistics Config";
const QString MODULE_NAME_GAMMA = "Gamma";
const QString MODULE_NAME_VO_GAMMA = "VO Gamma";
const QString MODULE_NAME_AWB_ATTR = "AWB attr";
const QString MODULE_NAME_REG_LIST = "Register List";
const QString MODULE_NAME_CLUT = "CLUT";
const QString MODULE_NAME_LSC = "LSC";
const QString MODULE_NAME_LDC = "LDC";
const QString MODULE_NAME_NP_PRO = "Noise Profile";
const QString MODULE_NAME_BNR = "BNR";
const QString MODULE_NAME_DRC = "DRC";
const QString MODULE_NAME_FSWDR = "FSWDR";
const QString MODULE_NAME_HDR = "HDR-Fusion";
const QString MODULE_NAME_HSV_OLD = "HSV_OLD";
const QString MODULE_NAME_DPC = "DPC";

enum class BUTTON_MODE {
        MODE_NORMAL,
        MODE_COUNT,
};

enum class MODULE_ID
{
    REGISTER_LIST,
    AWB_ATTR,
    HDR_FUSION,
    HSV_OLD,
    HDR_DRC,
    BEGIN,
    TOP,
    PUB_ATTR,
    BLACK_LEVEL,
    LBLC,
    RGBIR,
    BNR_LITE,
    FSHDR,
    PFR,
    DPC,
    CROSS_TALK,
    LSC,
    WB_ATTR,
    AWB_INFO,
    FSWDR,
    WDR_EXPOSURE_ATTR,
    DRC,
    BNR,
    AI_DRC,
    AI_BNR,
    AI_PQ,
    DEMOSAIC,
    CCM,
    SATURATION,
    GAMMA,
    DEHAZE,
    LUT_3D,
    NR3D,
    YNR,
    CNR,
    DCI,
    Y_SHARPEN,
    PRESHARPEN,
    AE_INFO,
    AE_ROUTE,
    CAC,
    IRIS,
    EXPOSURE_ATTR,
    SMART_EXPOSURE_ATTR,
    EXPOSURE_INFO,
    STATISTICS_CONFIG,
    DIS,
    LDC,
    HSV,
    MONO,
    Y_CONTRAST,
    NOISE_PROFILE,
    RLSC,
    RGBCAC,
    LCAC,
    LDCI,
    CA,
    CA_LITE,
    CLUT,
    VPSS_ADJUSTMENT,
    CSC,
    VO_GAMMA,
    ISPTOVC,
    RC_ATTR,
    RC_PARAM,
    CODING_PARAM,
    GOP_MODE,
    END,
};

enum class CALIBRATION_MODULE_ID
{
    BLC,
    LSC,
    AWB,
    CCM,
    NOISE_PROFILE,
    LBLC,
    CLUT,
    BNR,
    DPC,
    DISTORTION,
    RGBIR,
    RGBIRToRGGB,
    QUICK_CALIRATION
};

QString getModuleName(MODULE_ID module_id);
MODULE_ID getModuleId(QString module_name);
QString getCaliModuleName(CALIBRATION_MODULE_ID module_id);
extern QMap<MODULE_ID, QString> module_json_file_map;
extern QMap<MODULE_ID, QString> module_json_file_map_cv1822;


#endif // MODULE_DEFINE_H
