#ifndef CAPTURE_WINDOW_HPP
#define CAPTURE_WINDOW_HPP

#include "multi_image_viewer_widget.hpp"
#include "network_client_helper.hpp"
#include "status_log_widget.hpp"
#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QPixmap>
#include <QGroupBox>
#include <QCheckBox>

typedef enum {
    MODE_NORMAL,
    MODE_AE10RAW_BATCH,
} DUMPALL_MODE;

class CaptureWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CaptureWindow(QWidget *parent = nullptr);
    void initMenu();
    void initUI();
    void initEvent();
    void refresh();
    bool isDumpBusy() {return busy_flag;}

protected:
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;
    QGroupBox *createYuvImageGroupbox();
    QGroupBox *createRawImageGroupbox();
    QGroupBox *createDumpAllGroupbox();
    QGroupBox *createCaptureTimeGroupbox();
    int validateValue(int v, int min, int max);
    void setRawFileName(RAW_HEADER &header, int framNum);
    void saveYuvFile(QByteArray &data, YUV_HEADER &header, int total_received_frame);
    void saveRawFile(QByteArray &data, RAW_HEADER &header, int raw_type, int frameNum);
    void stopCapture();
    void disableComponents();
    void enableComponents();
    void handleShortCut();
    void handleGetWDRFusionFrame();
    void handleNoRawData();
    void check_file_validity();

public slots:
    void handleDumpAll(DUMPALL_MODE mode = DUMPALL_MODE::MODE_NORMAL,
        QString dirPath = "", QString fileName = "");
    void handleReadAllFinished();

private slots:
    void handleCaptureYuvImg();
    void handleMultiYuvStatus(int id);
    void handleCaptureRawImg();
    void handleMultiRawStatus(int id);
    void handleRecvBinInfo(int id);
    void handleRecvBinData(QString cmd);
    void handleCancelEvent();
    void slotCheckRawParams(QString text);
    void OpenSaveFilePath();
    bool CheckSaveFilePath();
    //frame by frame
    void slotGotYuvImg(QString cmd);
    void slotGotRawImg(QString cmd);
    void cellEdited(int idx);
    //by Capture timing
    void startTimingEvent();
    void stopTimingEvent();
    void startTimingCaptureEvent();

signals:
    void sigCaptureRawEnd();
    void sigCaptureYuvEnd();
    void fetchCompleted();
    void fetchStart();
    void sigStatusLog(QString message, LOG_LEVEL log_level);
private:
    QWidget *main_widget;
    QHBoxLayout *main_layout;
    QPixmap *pixmap;
    MultiImageViewerWidget *img_viewer;

    QGroupBox *yuv_image_group;
    QComboBox *yuv_cap_src_box;
    QLineEdit *yuv_frames_edit;
    QPushButton *yuv_capture_btn;
    QPushButton *yuv_cancel_btn;
    QCheckBox *save_bmp_file_checkbox;

    QGroupBox *raw_image_group;
    QComboBox *raw_type_box;
    QComboBox *raw_bits_box;
    QComboBox *raw_mode_box;
    QGroupBox *capture_group;
    QLineEdit *raw_frames_edit;
    QLabel *raw_frames_tips;
    QPushButton *raw_capture_btn;
    QPushButton *raw_cancel_btn;

    QPushButton *open_btn;
    QLineEdit *file_path;

    QGroupBox *dump_all_group;
    QPushButton *dump_btn;
    QPushButton *dump_cancel_btn;
    QPushButton *start_btn;
    QPushButton *stop_btn;
    QCheckBox *dump_all_repeatedly_checkbox;
    QLineEdit *interval_edit;
    QLineEdit *time_edit;
    QComboBox *type_cap_src_box;
    QTimer *start_timer;

    QCheckBox *wdr_raw_separately_checkbox;
    QCheckBox *raw_tightly_checkbox;
    QCheckBox *raw_checkbox;

    QByteArray receive_yuv_data;
    QByteArray receive_raw_data;
    QList<QByteArray> wdr_raw_data_list;

    int recvSize = 0;
    int rawSize = 0;
    int fusion_Frame{1};
    int total_frame{0};
    int current_frame{0};
    int capture_from{0};
    bool start_capture{false};
    bool start_dump{false};
    bool dump_all_repeatedly{false};
    bool capture_busy_state{false};
    QString directory_str = {};
    QString dateTime_str = {};
    QString file_name_linear = {};
    QString file_name_wdr = {};
    QString file_name_wdr_le = {};
    QString file_name_wdr_se = {};
    QString raw_info_log = {};
    QString reg_dump_log = {};
    QString ae_log = {};
    QByteArray ae_bin = {};
    QString awb_log = {};
    QByteArray awb_bin = {};
    RAW_HEADER rawHeader;

    // For AE10RAW Batch Mode Repaly
    DUMPALL_MODE dumpall_mode = DUMPALL_MODE::MODE_NORMAL;
    bool busy_flag = false;
    bool dumpAllFlag = false;
};

#endif // CAPTURE_WINDOW_HPP
