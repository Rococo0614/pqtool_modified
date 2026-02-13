#ifndef CALIBRATION_CAPTURE_WINDOW_HPP
#define CALIBRATION_CAPTURE_WINDOW_HPP

#include "multi_image_viewer_widget.hpp"
#include "network_client_helper.hpp"
#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QGroupBox>
#include <QCheckBox>

typedef struct _EXP_ATTR {
    int opType;
    int expTimeOpType;
    int gainType;
    int isoNUmOpType;
    int expTime;
    int isoNum;
    int isorangeMin;
    int isorangeMax;
    int antiflicker;
    int exptimeRangeMax;
    int target_compensation;
    int evbias;
    QVariant targetMin;
    QVariant targetMax;
} EXP_ATTR;

typedef struct _EXP_INFO_C{
    int iso;
    int expTime;
    int aveLum;
    int ispDgain;
} EXP_INFO_C;

typedef struct _STA_CFG {
    QVariant weight;
} STA_CFG;

enum FUNCTION_INDEX
{
    FUNCTION_NONE,
    FUNCTION_BLC_CAPTURE,
    FUNCTION_BLC_CHECK,
    FUNCTION_LSC_AWB_CAPTURE,
    FUNCTION_CCM_CAPTURE,
    FUNCTION_NOISE_CAPTURE,
};

class CalibrationCaptureWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CalibrationCaptureWindow(QWidget *parent = nullptr);
    void initUI();
    void initEvent();

protected:
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void closeEvent(QCloseEvent *event) override;
    QGroupBox *createBlcGroupbox();
    QGroupBox *createLscAwbGroupbox();
    QGroupBox *createCcmGroupbox();
    QGroupBox *createNoiseGroupbox();
    void setRawFileName(RAW_HEADER &header, int index);
    void saveRawFile(QByteArray &data, RAW_HEADER &header, int mode, int index);
    void saveTemporaryRawFile(QByteArray &data);
    void switchImageView();
    void stopCapture();
    void disableComponents();
    void enableComponents();
    QByteArray cropUnpackRaw(QByteArray src, RAW_HEADER &header);
    void selectedModuleChanged(QString text);
    void selectedLightChanged(int index, int module);
    void handleReadExpInfoSuccess();
    void handleReadExpAttrSuccess();
    void handleReadStaCfgSuccess();
    void handleWriteExpAttrSuccess();
    void handleWriteStaCfgSuccess();
    void getExpAttrData(EXP_ATTR *data);
    void getStaCfgData(STA_CFG *data);
    void getExpInfoData(EXP_INFO_C *data);
    void readStaCfgData();
    void readExpInfoData();
    void readExpAttrData();
    void writeExpAttrData(EXP_ATTR data, bool isEnd = false);
    void writeStaCfgData(STA_CFG data, bool isEnd = false);
    void handleBlcCaptureClicked();
    void handleBlcResetClicked();
    void handleBlcCancelClicked();
    void handleBlcCheckClicked();
    void handleLscAwbCaptureClicked();
    void handleLscAwbCancelClicked();
    void handleLscAwbResetClicked();
    void handleCcmCaptureClicked();
    void handleCcmCancelClicked();
    void handleCcmResetClicked();
    void handleNoiseCaptureClicked();
    void handleNoiseCancelClicked();
    void handleNoiseResetClicked();
    void funcCcmDelete();
    void funcCcmCapture();
    void funcLscAwbDelete();
    void funcLscAwbCapture();
    void blcCaptureRecvExpAttrData();
    void blcCheckRecvExpAttrData();
    void lscAwbRecvExpAttrData();
    void ccmRecvExpAttrData();
    void noiseRecvExpAttrData();
    void blcCaptureRaw();
    void blcCheckRaw();
    void noiseCaptureRaw();
    void ccmCaptureRaw();
    void lscAwbCaptureRaw();
    void initData();
    void showLog(const QString &log, LOG_LEVEL log_level);
    bool creatDir(QString module);
    void handleTimeOutEvent();
    void saveNpIsoRawData(RAW_HEADER *pstHeader, bool is_wdr_mode);
private slots:
    void handleModuleRWResult(QString module, QString cmd, QString result);
    void handleRawStatus(int id);
    void handleRecvBinInfo(int id);
    void handleRecvBinData(QString cmd);
    void handleRawData(QString cmd);
    void OpenSaveFilePath();
    bool CheckSaveFilePath();
private:
    QWidget *main_widget;
    QHBoxLayout *main_layout;
    QGridLayout *left_layout;
    MultiImageViewerWidget *img_viewer;
    ImageLabel *image_label;

    QComboBox *module_box;
    QPushButton *open_btn;
    QLineEdit *file_path;
    QPlainTextEdit *note_edit;
    QPlainTextEdit *log_edit;
    QComboBox *log_level_box;

    QGroupBox *blc_group;
    QPushButton *blc_capture_btn;
    QPushButton *blc_reset_btn;
    QPushButton *blc_cancel_btn;
    QPushButton *blc_check_btn;
    QComboBox *blc_iso_box;

    QGroupBox *lsc_awb_group;
    QComboBox *lsc_awb_light_box;
    QLineEdit *lsc_awb_ct_edit;
    QLineEdit *lsc_awb_luma_edit;
    QPushButton *lsc_awb_capture_btn;
    QPushButton *lsc_awb_cancel_btn;
    QPushButton *lsc_awb_reset_btn;

    QGroupBox *ccm_group;
    QComboBox *ccm_light_box;
    QLineEdit *ccm_ct_edit;
    QLineEdit *ccm_luma_edit;
    QPushButton *ccm_capture_btn;
    QPushButton *ccm_cancel_btn;
    QPushButton *ccm_reset_btn;

    QGroupBox *noise_group;
    QLineEdit *noise_luma_edit;
    QPushButton *noise_capture_btn;
    QPushButton *noise_cancel_btn;
    QPushButton *noise_reset_btn;
    QComboBox *noise_iso_box;

    QByteArray linear_data;
    QList<QByteArray> wdr_data;

    int rawSize{0};
    int raw_mode{0};
    int total_frame{0};
    int current_frame{0};
    int noise_frame_step{0};
    bool start_capture{false};
    QString directory_str;
    QString dateTime_str;
    QString file_name_linear;
    QString file_name_wdr;
    QList<QString> info_log;
    QList<QString> ae_log;
    QList<QString> awb_log;
    QList<QByteArray> ae_bin;
    QList<QByteArray> awb_bin;

    QTimer *timer;
    int invalidCount{0};
    int ccm_capture[5]{0};
    int lsc_awb_capture[5]{0};
    int ccm_ct[5]{0};
    int lsc_awb_ct[5]{0};
    int funcIdx{FUNCTION_NONE};
    EXP_ATTR expAttrDefault;
    EXP_ATTR expAttr;
    STA_CFG staCfgDefault;
    STA_CFG staCfg;
    EXP_INFO_C expInfo;
    int width{1920};
    int height{1080};
    int bayer{3};
    bool expAttrEnd{false};
    bool staCfgEnd{false};
};

#endif // CAPTURE_WINDOW_HPP
