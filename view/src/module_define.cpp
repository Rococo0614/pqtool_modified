#include "module_define.hpp"

QString getModuleName(MODULE_ID module_id)
{
    QString name;

    switch(module_id)
    {
        case MODULE_ID::REGISTER_LIST:
            name = QObject::tr("Register List");
            break;
        case MODULE_ID::AWB_ATTR:
            name = QObject::tr("AWB attr");
            break;
        case MODULE_ID::AWB_INFO:
            name = QObject::tr("WB Info");
            break;
        case MODULE_ID::AE_INFO:
            name = QObject::tr("AE info");
            break;
        case MODULE_ID::LSC:
            name = QObject::tr("LSC");
            break;
        case MODULE_ID::DPC:
            name = QObject::tr("DPC");
            break;
        case MODULE_ID::HDR_FUSION:
            name = QObject::tr("HDR-Fusion");
            break;
        case MODULE_ID::HDR_DRC:
            name = QObject::tr("HDR-DRC");
            break;
        case MODULE_ID::BNR:
            name = QObject::tr("BNR");
            break;
        case MODULE_ID::GAMMA:
            name = QObject::tr("Gamma");
            break;
        case MODULE_ID::DEHAZE:
            name = QObject::tr("Dehaze");
            break;
        case MODULE_ID::HSV_OLD:
            name = QObject::tr("HSV_OLD");
            break;
        case MODULE_ID::LUT_3D:
            name = QObject::tr("LUT 3D");
            break;
        case MODULE_ID::NR3D:
            name = QObject::tr("3DNR");
            break;
        case MODULE_ID::YNR:
            name = QObject::tr("YNR");
            break;
        case MODULE_ID::CNR:
            name = QObject::tr("CNR");
            break;
        case MODULE_ID::DCI:
            name = QObject::tr("DCI");
            break;
        case MODULE_ID::Y_SHARPEN:
            name = QObject::tr("Y Sharpen");
            break;
        case MODULE_ID::PRESHARPEN:
            name = QObject::tr("PreSharpen");
            break;
        case MODULE_ID::CAC:
            name = QObject::tr("CAC");
            break;
        case MODULE_ID::DEMOSAIC:
            name = QObject::tr("Demosaic");
            break;
        case MODULE_ID::EXPOSURE_ATTR:
            name = QObject::tr("Exposure attr");
            break;
        case MODULE_ID::CCM:
            name = QObject::tr("CCM");
            break;
        case MODULE_ID::EXPOSURE_INFO:
            name = QObject::tr("Exposure Info");
            break;
        case MODULE_ID::WB_ATTR:
            name = QObject::tr("WB Attr");
            break;
        case MODULE_ID::AE_ROUTE:
            name = QObject::tr("AE Route");
            break;
        case MODULE_ID::WDR_EXPOSURE_ATTR:
            name = QObject::tr("WDR Exposure Attr");
            break;
        case MODULE_ID::CROSS_TALK:
            name = QObject::tr("Crosstalk");
            break;
        case MODULE_ID::BLACK_LEVEL:
            name = QObject::tr("BLC");
            break;
        case MODULE_ID::LBLC:
            name = QObject::tr("LBLC");
            break;
        case MODULE_ID::FSWDR:
            name = QObject::tr("FSWDR");
            break;
        case MODULE_ID::DRC:
            name = QObject::tr("DRC");
            break;
        case MODULE_ID::SATURATION:
            name = QObject::tr("Saturation");
            break;
        case MODULE_ID::VPSS_ADJUSTMENT:
            name = QObject::tr("VPSS Adjustment");
            break;
        case MODULE_ID::STATISTICS_CONFIG:
            name = QObject::tr("Statistics Config");
            break;
        case MODULE_ID::TOP:
            name = QObject::tr("Top");
            break;
        case MODULE_ID::HSV:
            name = QObject::tr("HSV");
            break;
        case MODULE_ID::NOISE_PROFILE:
            name = QObject::tr("Noise Profile");
            break;
        case MODULE_ID::DIS:
            name = QObject::tr("DIS");
            break;
        case MODULE_ID::LDC:
            name = QObject::tr("LDC");
            break;
        case MODULE_ID::MONO:
            name = QObject::tr("Mono");
            break;
        case MODULE_ID::Y_CONTRAST:
            name = QObject::tr("Y Contrast");
            break;
        case MODULE_ID::RLSC:
            name = QObject::tr("RLSC");
            break;
        case MODULE_ID::PUB_ATTR:
            name = QObject::tr("Pub Attr");
            break;
        case MODULE_ID::RGBCAC:
            name = QObject::tr("RGBCAC");
            break;
         case MODULE_ID::CLUT:
            name = QObject::tr("CLUT");
            break;
        case MODULE_ID::GOP_MODE:
            name = QObject::tr("Gop Mode");
            break;
        case MODULE_ID::CODING_PARAM:
            name = QObject::tr("Coding Param");
            break;
        case MODULE_ID::RC_ATTR:
            name = QObject::tr("RC Attr");
            break;
        case MODULE_ID::RC_PARAM:
            name = QObject::tr("RC Param");
            break;
        case MODULE_ID::LCAC:
            name = QObject::tr("LCAC");
            break;
        case MODULE_ID::LDCI:
            name = QObject::tr("LDCI");
            break;
        case MODULE_ID::CA:
            name = QObject::tr("CA");
            break;
        case MODULE_ID::CA_LITE:
            name = QObject::tr("CA Lite");
            break;
        case MODULE_ID::ISPTOVC:
            name = QObject::tr("ISPtoVC");
            break;
        case MODULE_ID::VO_GAMMA:
            name = QObject::tr("VO Gamma");
            break;
        case MODULE_ID::SMART_EXPOSURE_ATTR:
            name = QObject::tr("Smart Exposure Attr");
            break;
        case MODULE_ID::CSC:
            name = QObject::tr("CSC");
            break;
        case MODULE_ID::IRIS:
            name = QObject::tr("IRIS");
            break;
        case MODULE_ID::RGBIR:
            name = QObject::tr("RGBIR");
            break;
        case MODULE_ID::AI_DRC:
            name = QObject::tr("TEAISP.drc");
            break;
        case MODULE_ID::AI_BNR:
            name = QObject::tr("TEAISP.bnr");
            break;
        case MODULE_ID::AI_PQ:
            name = QObject::tr("TEAISP.pq");
            break;
        case MODULE_ID::BNR_LITE:
            name = QObject::tr("BNR Lite");
            break;
        case MODULE_ID::FSHDR:
            name = QObject::tr("FSHDR");
            break;
        case MODULE_ID::PFR:
            name = QObject::tr("PFR");
            break;
        default:
            name = QObject::tr("Base Page");
            break;
    };

    return name;
}

MODULE_ID getModuleId(QString module_name)
{
    for (int id = static_cast<int>(MODULE_ID::REGISTER_LIST); id < static_cast<int>(MODULE_ID::END); id++) {
        if (QString::compare(module_name, getModuleName(static_cast<MODULE_ID>(id))) == 0) {
            return static_cast<MODULE_ID>(id);
        }
    }

    return MODULE_ID::END;
}

QString getCaliModuleName(CALIBRATION_MODULE_ID module_id)
{
    QString name;

    switch(module_id)
    {
        case CALIBRATION_MODULE_ID::BLC:
            name = QObject::tr("BLC");
            break;
        case CALIBRATION_MODULE_ID::DPC:
            name = QObject::tr("DPC");
            break;
        case CALIBRATION_MODULE_ID::LSC:
            name = QObject::tr("LSC");
            break;
        case CALIBRATION_MODULE_ID::BNR:
            name = QObject::tr("BNR");
            break;
        case CALIBRATION_MODULE_ID::CCM:
            name = QObject::tr("CCM");
            break;
        case CALIBRATION_MODULE_ID::NOISE_PROFILE:
            name = QObject::tr("Noise Profile");
            break;
        case CALIBRATION_MODULE_ID::AWB:
            name = QObject::tr("AWB");
            break;
        case CALIBRATION_MODULE_ID::CLUT:
            name = QObject::tr("CLUT");
            break;
        case CALIBRATION_MODULE_ID::LBLC:
            name = QObject::tr("LBLC");
            break;
        case CALIBRATION_MODULE_ID::RGBIR:
            name = QObject::tr("RGBIR");
            break;
        case CALIBRATION_MODULE_ID::RGBIRToRGGB:
            name = QObject::tr("RGBIR To RGGB");
            break;
        case CALIBRATION_MODULE_ID::QUICK_CALIRATION:
            name = QObject::tr("Quick Calibration");
            break;
        default:
            name = QObject::tr("Calibration Page");
            break;
    };

    return name;
}

QMap<MODULE_ID, QString> module_json_file_map = {
    {MODULE_ID::NR3D, "/3dnr.json"},
    // {MODULE_ID::AE_INFO, "ae_info.json"},
    {MODULE_ID::AE_ROUTE, "/ae_route.json"},
    {MODULE_ID::BNR, "/bnr.json"},
    {MODULE_ID::CAC, "/cac.json"},
    {MODULE_ID::CCM, "/ccm.json"},
    {MODULE_ID::CNR, "/cnr.json"},
    {MODULE_ID::CROSS_TALK, "/cross_talk.json"},
    {MODULE_ID::DCI, "/dci.json"},
    {MODULE_ID::DPC, "/dpc.json"},
    {MODULE_ID::DEHAZE, "/dehaze.json"},
    {MODULE_ID::DEMOSAIC, "/demosaic.json"},
    {MODULE_ID::EXPOSURE_ATTR, "/exposure_attr.json"},
    {MODULE_ID::EXPOSURE_INFO, "/exposure_info.json"},
    // {MODULE_ID::HDR_DRC, "/hdr_drc.json"},
    // {MODULE_ID::HDR_FUSION, "/hdr_fusion.json"},
    {MODULE_ID::LSC, "/lsc.json"},
    {MODULE_ID::AWB_INFO, "/wb_info.json"},
    {MODULE_ID::WB_ATTR, "/wb_attr.json"},
    {MODULE_ID::WDR_EXPOSURE_ATTR, "/wdr_exposure_attr.json"},
    {MODULE_ID::Y_SHARPEN, "/y_sharpen.json"},
    {MODULE_ID::PRESHARPEN, "/presharpen.json"},
    {MODULE_ID::YNR, "/ynr.json"},
    {MODULE_ID::BLACK_LEVEL, "/black_level.json"},
    {MODULE_ID::FSWDR, "/fswdr.json"},
    {MODULE_ID::DRC, "/drc.json"},
    {MODULE_ID::SATURATION, "/saturation.json"},
    {MODULE_ID::VPSS_ADJUSTMENT, "/adjustment.json"},
    {MODULE_ID::STATISTICS_CONFIG, "/statistics_config.json"},
    {MODULE_ID::TOP, "/top.json"},
    {MODULE_ID::HSV, "/hsv.json"},
    {MODULE_ID::NOISE_PROFILE, "/noise_profile.json"},
    {MODULE_ID::DIS, "/dis.json"},
    {MODULE_ID::LDC, "/ldc.json"},
    {MODULE_ID::MONO, "/mono.json"},
    {MODULE_ID::Y_CONTRAST, "/y_contrast.json"},
    {MODULE_ID::PUB_ATTR, "/pub_attr.json"},
    {MODULE_ID::GOP_MODE, "/gop_mode.json"},
    {MODULE_ID::CODING_PARAM, "/coding_param.json"},
    {MODULE_ID::CLUT, "/clut.json"},
    {MODULE_ID::RC_ATTR, "/rc_attr.json"},
    {MODULE_ID::RC_PARAM, "/rc_param.json"},
    {MODULE_ID::RLSC, "/rlsc.json"},
    {MODULE_ID::RGBCAC, "/rgbcac.json"},
    {MODULE_ID::CA, "/ca.json"},
    {MODULE_ID::ISPTOVC, "/isptovc.json"},
    {MODULE_ID::GAMMA, "/gamma.json"},
    {MODULE_ID::VO_GAMMA, "/vo_gamma.json"},
    {MODULE_ID::SMART_EXPOSURE_ATTR, "/smart_exposure_attr.json"},
    {MODULE_ID::CSC, "/csc.json"},
    {MODULE_ID::IRIS, "/iris.json"}
};
