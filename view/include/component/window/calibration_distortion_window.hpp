#ifndef CALIBRATION_DISTORTION_WINDOW_HPP
#define CALIBRATION_DISTORTION_WINDOW_HPP

#include "image_viewer_widget.hpp"
#include "base_calibration_page_widget.hpp"
#include "multi_image_viewer_widget.hpp"
#include "network_client_helper.hpp"
#include "jpg_image_list_widget.hpp"
#include "iCamCalib.h"
#include "sophldc.h"
#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QGroupBox>
#include <QCheckBox>

class CalibrationDistortionWindow : public BaseCalibrationPageWidget
{
    Q_OBJECT

public:
    explicit CalibrationDistortionWindow(QWidget *parent = nullptr);
    void initEvent();

private:
    QGroupBox *createGetImageGroupbox();
    QGroupBox *createBoardInfoGroupbox();
    QGroupBox *createWriteDataGroupbox();
    QGroupBox *createCalibrationGroupbox();
    QGroupBox *createCalibParamGroupbox();
    QGroupBox *createConvertPathInfoGroupbox();
    QGroupBox *createImageListGroupbox();
    void cellEdited(int idx);
    void disableComponents();
    void enableComponents();
    void handleCancelEvent();
    void stopCapture();
    void handleCaptureYuvImg();
    void readCvFlagValue();
    void updateCvFlagUI();
    void startMeshBinConvert();
    QString getSavePath(int src, int type, QString name = "");
    QString getErrorCodeDescribe(int id_code);
    QList<QImage> monoCalibration(cv::Mat *images, double *total, QList<double> *rms, cv::Size size, int num);
    QList<QImage> stereoCalibration(cv::Mat *imagesLeft, cv::Mat *imagesRight, double *total, QList<double> *rms, cv::Size size, int num);
    void initLeftArea() override;
    void initCenterArea() override;
    bool calibration() override;
    void saveImgFile(QList<QImage> list, int type, QString path = "");

private slots:
    void handleMultiYuvStatus(int id);
    void slotGotYuvImg(QString cmd);
    void slotUpdateEyeType();
    void openSaveRootPath();
    void openConvertRootPath();
    void updateOpencvFlag2(int index);
    void handleImgListUpdate(SIGNAL_TYPE type, int id);
    void updateRMS(double total, QList<double> *rms, int type);

private:
    QWidget *left_widget;
    QVBoxLayout *left_layout;
    QTabWidget *center_tab_widget;
    MultiImageViewerWidget *input_img;
    MultiImageViewerWidget *output_img;
    ImageLabel *input_image_label;
    ImageLabel *output_image_label;
    QLineEdit  *corner_interval;
    QLineEdit  *pattern_wide;
    QLineEdit  *pattern_high;
    QLineEdit  *left_img_path;
    QLineEdit  *right_img_path;
    QLineEdit  *stereo_img_path;
    QLineEdit  *img_frames_edit;
    QLineEdit  *root_path_pos;
    QLineEdit  *convert_root_path_pos;
    QLineEdit  *convert_path_pos;
    QLineEdit  *black_edge_alpha;
    QLineEdit  *opencv_flags;
    QComboBox  *opencv_flag2;
    QComboBox *board_type_box;
    QComboBox *image_format_box;
    QComboBox *cap_src_image_box;
    QComboBox *fisheye_box;
    QComboBox *meshH_box;
    QComboBox *meshW_box;
    QLineEdit  *imgW_edit;
    QLineEdit  *imgH_edit;
    QPushButton *capture_img_btn;
    QPushButton *cancel_img_btn;
    QPushButton *calibration_btn;
    QPushButton *write_calib_btn;
    QPushButton *import_calib_data_btn;
    QPushButton *export_calib_data_btn;
    QPushButton *save_path_btn;
    QPushButton *convert_path_btn;
    QComboBox *yuv_src_box;
    QByteArray receive_yuv_data;
    QLabel *flags_label;
    QLabel *annotation_flags;
    QLineEdit *homo_mtx_value;
    QPushButton *convert_bin_btn;
    JpgImageListWidget *left_list_widget;
    JpgImageListWidget *right_list_widget;

    int last_mono_flag = 0;
    int last_stereo_flag = 1;
    int total_frame = 0;
    int current_frame = 0;
    int recvSize = 0;
    bool start_capture{false};
    int capture_from{0};
    int image_src_index = 0;
    int last_img_src_index = 0;
    int cur_src_index = 0;
    QString root_dir = "";
    QString image_dir = "";
    QString result_dir = "";
    QString convert_root_dir = "";
    QList<QImage> image_list_left;
    QList<QImage> image_list_right;
};

#endif // CalibrationDistortionWindow
