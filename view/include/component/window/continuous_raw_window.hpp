#ifndef CONTINUOUS_RAW_WINDOW_HPP
#define CONTINUOUS_RAW_WINDOW_HPP

#include "multi_image_viewer_widget.hpp"
#include "network_client_helper.hpp"
#include "raw_dump.hpp"
#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QPixmap>
#include <QGroupBox>
#include <QCheckBox>

class ContinuousRawWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ContinuousRawWindow(QWidget *parent = nullptr);
    void initUI();
    void initEvent();

private:
    void closeEvent(QCloseEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void disableComponents();
    void enableComponents();
    int CheckRoiInfo();
    void UnpackRaw();
    void getRawInfoFromFile(QString path, RAW_INFO *raw_info);
    QByteArray unpackRawDataByFrame(QByteArray data, RAW_INFO raw_info, RAW_TYPE rawType);
    QByteArray fillRoiRaw(QByteArray fullData, QByteArray roiData, RAW_INFO rawInfo);
    void saveJpg();

private slots:
    void OpenSaveFilePath();
    void GetSingleYuv();
    void ReceiveSingleYuv(QString cmd);
    void CancelGetSingleYuv();
    void DumpStatus(int status);
    void HandelCapture();
    void StartDump();
    void RawDump();
    void CancelDump();
    void handleSingleImageStatus(int id);
    void GetRawDumpMaxFrames();
    void ReceiveRawDumpMaxFrames(int id);
signals:
    void sigGetYuvEnd();


private:
    QWidget *main_widget;
    QHBoxLayout *main_layout;
    MultiImageViewerWidget *img_viewer;
    QGroupBox *preview_group;
    QPushButton *preview_btn;
    QPushButton *preview_cancel_btn;
    QGroupBox *roi_group;
    QLineEdit *frames_edit;
    QLineEdit *w_edit;
    QLineEdit *h_edit;
    QLineEdit *cropX_edit;
    QLineEdit *cropY_edit;
    QLineEdit *cropW_edit;
    QLineEdit *cropH_edit;
    QPushButton *capture_btn;
    QPushButton *capture_cancel_btn;
    QGroupBox *save_group;
    QCheckBox *save_local;
    QCheckBox *save_sdcard;
    QPushButton *open_btn;
    QLineEdit *file_path;
    QTimer *delay;
    QLineEdit *time_edit;
    QLineEdit *maxframes_edit;

    QByteArray yuvHeader;
    bool saveLocal = true;
    QByteArray img_yuv_data;
    class RawDump rawDump;
    bool startDump = false;
    bool connected = false;
    bool dump_error = false;
    QString savePath = {};
    int leCropX;
    int leCropY;
    int seCropX;
    int seCropY;
};

#endif
