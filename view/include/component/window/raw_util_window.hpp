#ifndef RAW_UTIL_WINDOW_HPP
#define RAW_UTIL_WINDOW_HPP

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QPixmap>
#include <QTableView>
#include <QTableWidget>
#include <QStandardItem>
#include <QPushButton>
#include <QCheckBox>
#include <QMessageBox>
#include <QTimer>
#include "network_client_helper.hpp"
#include "raw_image_list_widget.hpp"
#include "yuv_image_list_widget.hpp"

class RawUtilWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RawUtilWindow(QWidget *parent = nullptr);
    void initMenu();
    void initUI();
    void initEvent();
    void refresh();
    void retranslateUi();

protected:
    void keyReleaseEvent(QKeyEvent *event) override;
    void getRawInfoFromFile(QString path, RAW_INFO *raw_info);
    QByteArray handleImportRoiRawData(QString path, RAW_INFO *raw_info, RoiInfo roi_info);
    void saveUnpackedRaw(QString path, QByteArray full, QByteArray roi, RAW_INFO raw_info, RoiInfo roiInfo);
    QList<EXP_INFO> getExpInfoFromFile(QString path);
    void slotGetYuv();
    void saveYuvFile(QByteArray &data, YUV_HEADER &header, int frame);
    QByteArray RawLinearToWdr(QByteArray srcLe, QByteArray srcSe, int height);
    QByteArray RawWdrToLInear(QByteArray src, int height);
    void openYuvFile(QString path);
    QByteArray yuvDataConvert(YUV_HEADER *head, QByteArray data);
    QJsonObject SetYuvHeadJson(YUV_HEADER head);
    void slotSendYuv();
    void delItemSelected();
    void delItemAll();
    void handleReceiveMessageYuv(QString cmd);
private:
    void closeEvent(QCloseEvent *event) override;
    QJsonObject SetRawHeadJson(RAW_INFO header);
    QString findFileTimeStamp(QString &file_name);
    void setFileNameFrameNum(QString &file_name, int frame_num);

private slots:
    void handleOpenImgFile();
    void slotSend();
    void slotFlush();
    void slotCancel();
    void slotBatch();
    void handleReceiveMessage(int id);
    void batch_conti();
    void batch_dumpAll();
    void batch_updateProgress();
    void handleMultiYuvStatus(int id);
    void slotGotYuvImg(QString cmd);
private:
    QWidget *main_widget;
    QGridLayout *main_layout;
    RawImageListWidget *raw_list_view;
    YuvImageListWidget *yuv_list_view = nullptr;
    QPushButton *add_img_btn;
    QPushButton *dis_select_btn;
    QPushButton *move_up_btn;
    QPushButton *move_down_btn;
    QPushButton *del_btn;
    QPushButton *del_all_btn;
    QCheckBox *stitch_flag;
    QCheckBox *isp_info;
    QCheckBox *snap_mode;
    QComboBox *flag_box;
    QCheckBox *loop;
    QPushButton *send_btn;
    QPushButton *flush_btn;
    QPushButton *cancel_btn;
    QPushButton *batch_btn;
    QPushButton *yuv_btn;
    QCheckBox *ae_sim;
    QCheckBox *save_btn;
    QCheckBox *awb_sim;
    bool disel_toggle{false};
    RAW_INFO rawHeader;
    QByteArray replay_data;
    int leCropX;
    int leCropY;
    int seCropX;
    int seCropY;
    int leWidth;
    int leHeight;
    int seWidth;
    int seHeight;
    bool findCropInfo = false;
    bool roiMode = false;
    bool isBatchMode = false;
    int batch_status = 0;
    QString batchMainTarDir;
    QString batchMainSourceDir;
    QString curBatchRawName;
    QString curBatchOutDir;
    QString curBatchOutFileNameNoExt;
    QTimer *batch_timer;
    int fStart;
    int fEnd;
    RawImgOption rawOption;
    QList<EXP_INFO> expInfoList;
    int yuvIndex;
    bool startCapture = false;
    QString dirPath;

    int dataFormat = 0;
    YuvImgOption yuvOption;
    YUV_HEADER yuvHeader;
};


typedef enum _RAWPLAY_OP_MODE {
    RAW_OP_MODE_NORMAL = 0,
    RAW_OP_MODE_AE_SIM = 1,
    RAW_OP_MODE_AWB_SIM = 2,
    RAW_OP_MODE_AF_SIM = 3,
} RAWPLAY_OP_MODE;


typedef enum _BAT_RAWPLAY_STATE {
    STS_NONE = 0,
    STS_START = 1,
    STS_SEND_RAW_OK = 2,
    STS_WAIT_IMAGE_STABLE = 3,
    STS_DUMP_ALL = 4,
    STS_WAIT_DUMP = 5,
    STS_SEND_NEXT = 6,
} BAT_RAWPLAY_STATE;
#endif // RAW_UTIL_WINDOW_HPP
