#ifndef BRACKETING_WINDOW_HPP
#define BRACKETING_WINDOW_HPP

#include "multi_image_viewer_widget.hpp"
#include "double_slider_calibration_item_widget.hpp"
#include "combobox_calibration_item_widget.hpp"
#include "lineedit_calibration_item_widget.hpp"
#include "network_client_helper.hpp"
#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QPixmap>
#include <QGroupBox>
#include <QCheckBox>

class BracketingWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BracketingWindow(QWidget *parent = nullptr);
    void initMenu();
    void initUI();
    void initEvent();
    void refresh();

protected:
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void closeEvent(QCloseEvent *event) override;
    QGroupBox *createBracketingGroupbox();
    void saveBracketingFile(QByteArray &data, RAW_HEADER &header, int frameStep, int mode, int index, double leEV, double seEV);
    void saveAe10BracketingFile(RAW_HEADER &header);
    void stopCapture();
    void disableComponents();
    void enableComponents();
    QByteArray cropUnpackRaw(QByteArray src, RAW_HEADER &header);
    void handleGetSingleYuv();
    void saveYuvFile(QByteArray &data, YUV_HEADER &header, int frames, int index);
    void handleGetSingleRaw();
    void saveRawFile(QByteArray &data, RAW_HEADER &header, int raw_type, int frameNum, int index);
    void get10AeRaw();

private slots:
    void handleGetBracket();
    void handleBracketStatus(int id);
    void handleCancelEvent();
    void OpenSaveFilePath();
    //frame by frame
    void handleBracketData(QString cmd);
    void handleSingleYuvData(QString cmd);
    void handleSingleRawStatus(int id);
    void handleSingleRawData(QString cmd);
    void handledump();
    void handleRecvBinInfo(int id);
    void handleRecvBinData(QString cmd);
    void handleSingleYuvStatus(int id);

signals:
    void sigCaptureRawEnd();
    void sigDumpEnd();

private:
    QWidget *main_widget;
    QHBoxLayout *main_layout;
    QPixmap *pixmap;
    MultiImageViewerWidget *img_viewer;

    QPushButton *open_btn;
    QLineEdit *file_path;
    QByteArray receive_raw_data;
    QList<QByteArray> wdr_raw_data_list;
    QList<QByteArray> yuv_head_list;
    QList<QByteArray> yuv_data_list;

    DoubleSliderCaliItemWidget *le_range_max;
    DoubleSliderCaliItemWidget *le_range_min;
    DoubleSliderCaliItemWidget *le_step;
    DoubleSliderCaliItemWidget *se_range_max;
    DoubleSliderCaliItemWidget *se_range_min;
    DoubleSliderCaliItemWidget *se_step;
    LineEditCaliItemWidget *scene_item;
    LineEditCaliItemWidget *frames_per_step_item;
    QGroupBox *bracketing_group;
    QPushButton *bracketing_capture_btn;
    QPushButton *bracketing_cancel_btn;
    QCheckBox *save_bmp_file_checkbox;
    QPushButton *ae_btn;

    bool start_capture{false};
    bool start_dump{false};
    bool ae10raw = false;
    QString dateTime_str;
    QString directory_str;
    QList<QString> raw_info_log;
    QList<QString> reg_dump_log;
    QList<QString> ae_log;
    QList<QString> awb_log;
    QList<QByteArray> awb_bin;
    QList<QByteArray> ae_bin;
    QList<QString> yuv_bmp_path;

    QList<int> leEV;
    QList<int> seEV;
    int totalStep{0};
    int currentStep{0};
    int framesPerStep{0};

    int recvSize = 0;
    int rawSize = 0;
};


#define HIST_PER_EV	(128)
#define HIST_SIZE	(30*HIST_PER_EV)	//-10 ~ 20 EV
#define EV_OFFSET	(10)

#endif
