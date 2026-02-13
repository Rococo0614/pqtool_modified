#ifndef DEWARP_ENGINE_HELPER_HPP
#define DEWARP_ENGINE_HELPER_HPP

#include <memory>
#include <QObject>
#include "dewarp.h"

using namespace std;

typedef USAGE_MODE FISH_EYE_MODE;

enum IMAGE_SECTOR_ID {
    SECTOR_RGN_0,
    SECTOR_RGN_1,
    SECTOR_RGN_2,
    SECTOR_RGN_3,
    SECTOR_PANORAMA_0,
    SECTOR_PANORAMA_1,
    SECTOR_1O,
    SECTOR_ID_END
};

class DewarpEngineHelper : public QObject
{
    Q_OBJECT

public:
    static shared_ptr<DewarpEngineHelper> getInstance();

public:
    explicit DewarpEngineHelper();
    ~DewarpEngineHelper();
    void updateFiles();
    void setSrcFilePath(QString path);
    QString getSrcFilePath();
    FISH_EYE_MODE getMode() const;
    void setMode(const FISH_EYE_MODE &value);
    void setMountMode(const MOUNTMODE &mount);

    void loadFishEyeImage();
    void updatePanTilt(int rgn_idx, double set_Pan, double set_Tilt);
    void updateTheta(int rgn_idx, double set_ThetaX, double set_ThetaY, double set_ThetaZ);
    void updateZoom(int rgn_idx, double set_ZoomH, double set_ZoomV);

private:
    void updateImagesByRgnIdx(int rgn_idx);
    void updateImages();
    void loadSrcFile();
    void dumpSubWindowImg(int rgn_idx);

signals:
    void sigDataUpdated(int idx, QImage *image);

private slots:
    //void handleFileChanged(const QString &path);

private:
    static shared_ptr<DewarpEngineHelper> m_instance;
    QString src_file_path;
    FISHEYE_ATTR FISHEYE_CONFIG;
    FISHEYE_REGION_ATTR FISHEYE_REGION[MAX_REGION_NUM];
    int src_width{0};
    int src_height{0};
    unsigned char* src_img = NULL;
    unsigned char* src_rois = NULL;
    unsigned char* dst_img = NULL;
    int x0 = 0;	// lens center cooridinate of the input image
    int y0 = 0;	// lens center cooridinate of the input image
    int r  = 1; // fisheye radius
    int dst_width	= 0;
    int dst_height  = 0;
};

#endif // DEWARP_ENGINE_HELPER_HPP
