#include <QPixmap>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>
#include <QtCore>

#include "dewarp_engine_helper.hpp"
#include "utils.hpp"

#define minmax(a,b,c)  (((a)<(b))? (b):((a)>(c))? (c):(a))

shared_ptr<DewarpEngineHelper> DewarpEngineHelper::m_instance = nullptr;

DewarpEngineHelper::DewarpEngineHelper()
{
//    src_file_path = QDir::currentPath() + PATH_SEPARATOR + ".." + PATH_SEPARATOR +
//            "sample_data" + PATH_SEPARATOR + "cvi_ceiling.ppm";
    updateFiles();
}

DewarpEngineHelper::~DewarpEngineHelper()
{
    QString result_dir = QDir::currentPath() + PATH_SEPARATOR + ".." + PATH_SEPARATOR + "Results";
    QDir dir(result_dir);
    dir.removeRecursively();
}

shared_ptr<DewarpEngineHelper> DewarpEngineHelper::getInstance()
{
    if (nullptr == DewarpEngineHelper::m_instance) {
        DewarpEngineHelper::m_instance = make_shared<DewarpEngineHelper>();
    }

    return DewarpEngineHelper::m_instance;
}

void DewarpEngineHelper::updateFiles()
{
//    QString root_dir_path = QDir::currentPath();
}

void DewarpEngineHelper::setSrcFilePath(QString path)
{
    if (path.indexOf(" ") > 0) {
        QMessageBox msgBox;
        msgBox.setText(tr("File path can't include space."));
        msgBox.exec();
        return;
    }
    src_file_path = path;
}

QString DewarpEngineHelper::getSrcFilePath()
{
    return src_file_path;
}

FISH_EYE_MODE DewarpEngineHelper::getMode() const
{
    return FISHEYE_CONFIG.UsageMode;
}

void DewarpEngineHelper::setMode(const FISH_EYE_MODE &value)
{
    FISHEYE_CONFIG.UsageMode = value;
}

void DewarpEngineHelper::setMountMode(const MOUNTMODE &mount)
{
    FISHEYE_CONFIG.MntMode = mount;
}

void DewarpEngineHelper::loadSrcFile()
{
    string infile_str = src_file_path.toStdString();
    if (infile_str.empty()) {
        return;
    }

    if (src_img) {
        free(src_img);
        src_img = NULL;
    }

    if (src_rois) {
        free(src_rois);
        src_rois = NULL;
    }

    src_img = ReadPPM(infile_str.c_str(), &src_width, &src_height);
    src_rois = ReadPPM(infile_str.c_str(), &src_width, &src_height);
    x0 = src_width / 2;
    y0 = src_height / 2;
    r = 674;  // hard code

    dst_width = 1920;
    dst_height = 1080;

    //FISHEYE_CONFIG.MntMode		= CEILING;
    //FISHEYE_CONFIG.UsageMode	= MODE_02_1O4R;
    INITIAL_CONFIG(&FISHEYE_CONFIG, FISHEYE_REGION, dst_height, dst_width, src_width, src_height, x0, y0, r);

    if (dst_img) {
        free(dst_img);
        dst_img = NULL;
    }

    dst_img = (unsigned char*)malloc(FISHEYE_CONFIG.OutW_disp * FISHEYE_CONFIG.OutH_disp * 3 * sizeof(unsigned char));
    memset(dst_img, 0, FISHEYE_CONFIG.OutW_disp * FISHEYE_CONFIG.OutH_disp * 3 * sizeof(unsigned char));
}

void DewarpEngineHelper::dumpSubWindowImg(int rgn_idx)
{
    if (FISHEYE_CONFIG.UsageMode == MODE_01_1O)
    {
        return;
    }

    PROJECTION_MODE ViewModeType = FISHEYE_REGION[rgn_idx].ViewMode;

    int view_w = FISHEYE_REGION[rgn_idx].OutW;
    int view_h = FISHEYE_REGION[rgn_idx].OutH;

    // draw image
    unsigned char* rgn_img;
    rgn_img = (unsigned char*)malloc(view_w * view_h * 3 * sizeof(unsigned char));

    if (ViewModeType == PROJECTION_REGION)
    {
        _RegionView(src_img, rgn_img, src_rois, &FISHEYE_CONFIG, FISHEYE_REGION, rgn_idx, x0, y0, r);
    }
    else if (ViewModeType == PROJECTION_PANORAMA_360)
    {
        _Panorama360View(src_img, rgn_img, src_rois, &FISHEYE_CONFIG, FISHEYE_REGION, rgn_idx, x0, y0, r);
    }
    else if (ViewModeType == PROJECTION_PANORAMA_180)
    {
        _Panorama180View(src_img, rgn_img, src_rois, &FISHEYE_CONFIG, FISHEYE_REGION, rgn_idx, x0, y0, r);
    }
    else
    {
        qDebug("ERROR!!! THIS CASE SHOULD NOT HAPPEN!!!!!!\n\r");
        return;
    }

#if 0 //for test
        char filename[256];
        sprintf(filename, "rgn_window_%02d.ppm", rgn_idx);
        RGB888Image2PPM(rgn_img, view_w, view_h, filename);
#endif

    //qDebug("view_w:%d, view_h:%d", view_w, view_h);
    QImage *img = new QImage(rgn_img, view_w, view_h, QImage::Format_RGB888);
    sigDataUpdated(rgn_idx, img);

    free(rgn_img);
}

void DewarpEngineHelper::loadFishEyeImage()
{
    loadSrcFile();
    updateImages();
}

void DewarpEngineHelper::updatePanTilt(int rgn_idx, double delta_Pan, double delta_Tilt)
{
    double set_Pan;
    double set_Tilt;
    if ((FISHEYE_REGION[rgn_idx].ViewMode == PROJECTION_REGION
         || FISHEYE_REGION[rgn_idx].ViewMode == PROJECTION_PANORAMA_180
         || FISHEYE_REGION[rgn_idx].ViewMode == PROJECTION_PANORAMA_360)
            && FISHEYE_REGION[rgn_idx].RegionValid == true)
    {
        set_Pan = delta_Pan + FISHEYE_REGION[rgn_idx].Pan;
        set_Tilt = delta_Tilt + FISHEYE_REGION[rgn_idx].Tilt;

        FISHEYE_REGION[rgn_idx].Pan = minmax(set_Pan, 0, 359);
        FISHEYE_REGION[rgn_idx].Tilt = minmax(set_Tilt, 0, 359);

        updateImagesByRgnIdx(rgn_idx);
    }
}

void DewarpEngineHelper::updateTheta(int rgn_idx, double delta_ThetaX, double delta_ThetaY, double delta_ThetaZ)
{
    if ((FISHEYE_REGION[rgn_idx].ViewMode != PROJECTION_REGION)
            || FISHEYE_REGION[rgn_idx].RegionValid == false)
    {
        qDebug("Set Region Is Not PROJECTION Region or this Region is no activated, Invalid Command");
        return;
    }

    double set_ThetaX = FISHEYE_REGION[rgn_idx].ThetaX + delta_ThetaX;
    double set_ThetaY = FISHEYE_REGION[rgn_idx].ThetaY + delta_ThetaY;
    double set_ThetaZ = FISHEYE_REGION[rgn_idx].ThetaZ + delta_ThetaZ;

    FISHEYE_REGION[rgn_idx].ThetaX = minmax(set_ThetaX, 0, 359);
    FISHEYE_REGION[rgn_idx].ThetaY = minmax(set_ThetaY, 0, 359);
    FISHEYE_REGION[rgn_idx].ThetaZ = minmax(set_ThetaZ, 0, 359);
    updateImagesByRgnIdx(rgn_idx);
}

void DewarpEngineHelper::updateZoom(int rgn_idx, double delta_ZoomH, double delta_ZoomV)
{
    if ((FISHEYE_REGION[rgn_idx].ViewMode == PROJECTION_REGION
         || FISHEYE_REGION[rgn_idx].ViewMode == PROJECTION_PANORAMA_180)
             && FISHEYE_REGION[rgn_idx].RegionValid == true)
    {
        double set_ZoomH = FISHEYE_REGION[rgn_idx].ZoomH + delta_ZoomH;
        double set_ZoomV = FISHEYE_REGION[rgn_idx].ZoomV + delta_ZoomV;

        FISHEYE_REGION[rgn_idx].ZoomH = minmax(set_ZoomH, 0, 4095);
        FISHEYE_REGION[rgn_idx].ZoomV = minmax(set_ZoomV, 0, 4095);
        updateImagesByRgnIdx(rgn_idx);
    }
    else if (FISHEYE_REGION[rgn_idx].ViewMode == PROJECTION_PANORAMA_360
        && FISHEYE_REGION[rgn_idx].RegionValid == true)
    {
        double set_ZoomV = FISHEYE_REGION[rgn_idx].ZoomV + delta_ZoomV;
        FISHEYE_REGION[rgn_idx].ZoomV = minmax(set_ZoomV, 0, 4095);
        updateImagesByRgnIdx(rgn_idx);
    }
}

void DewarpEngineHelper::updateImagesByRgnIdx(int rgn_idx)
{
    if (rgn_idx < 0 || rgn_idx >= FISHEYE_CONFIG.RgnNum)
        return;

    if (FISHEYE_REGION[rgn_idx].RegionValid)
    {
        dumpSubWindowImg(rgn_idx);
    }
}

void DewarpEngineHelper::updateImages()
{
    for (int rgn_idx = 0; rgn_idx < FISHEYE_CONFIG.RgnNum; rgn_idx++)
    {
        // check region valid first
        if (!FISHEYE_REGION[rgn_idx].RegionValid)
            break;
        // Destination Mesh-Info Allocation
        int view_w = FISHEYE_REGION[rgn_idx].OutW;
        int view_h = FISHEYE_REGION[rgn_idx].OutH;
        int mesh_horcnt = FISHEYE_REGION[rgn_idx].MeshHor;
        int mesh_vercnt = FISHEYE_REGION[rgn_idx].MeshVer;

        // get & store region mesh info.
        //_get_region_dst_mesh_list(FISHEYE_REGION, view_w, view_h, mesh_horcnt, mesh_vercnt, rgn_idx);
        // Get Source Mesh-Info Projected from Destination by Differet ViewModw.
        //_get_region_src_mesh_list(FISHEYE_CONFIG.MntMode, FISHEYE_REGION, mesh_horcnt, mesh_vercnt, rgn_idx, x0, y0, r);
        // UI need to output region image separately.
        //_dump_draw_sub_window_img(&FISHEYE_CONFIG, FISHEYE_REGION, rgn_idx, x0, y0, r, src_img, src_rois);

        dumpSubWindowImg(rgn_idx);
        // debug msg
        qDebug("REGION %d done. \n", rgn_idx);
    }

    //for test
#if 0
    char filename[256];
    sprintf(filename, "src_rois.ppm");
    RGB888Image2PPM(src_rois, src_width, src_height, filename);
#endif

    if (FISHEYE_CONFIG.UsageMode == MODE_02_1O4R || FISHEYE_CONFIG.UsageMode == MODE_01_1O) {
        QImage *roi = new QImage(src_img, src_width, src_height, QImage::Format_RGB888);
        sigDataUpdated(SECTOR_1O, roi);
    }
}
