#ifndef ISP_INFO_DIALOG_H
#define ISP_INFO_DIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QGroupBox>

struct RawIspInfo {
    int ISO;
    int LightValue;
    int ColorTemp;
    int AGain;
    int DGain;
    int ISPDGain;
    int ExpTime[2];
    int Ratio;
};

struct RoiInfo {
    bool cropChange;
    int frames;
    int width;
    int height;
    int cropX;
    int cropY;
    int cropWidth;
    int cropHeight;
};

class IspInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IspInfoDialog(QWidget *parent = nullptr, int frame_num = 0, bool roiMode = false);
    void initUI();
    void loadIspInfo(int frame_num, QList<RawIspInfo> RawIspInfo_list);
    QList<RawIspInfo> getIspInfo();
    QList<int> getBlackLvl();
    void updateIspInfoDisp();
    void loadBlackLvlInfo(QList<int> black_lvl_list);
    void updateBlackLvlInfoDisp();
    void loadRoiInfo(RoiInfo roiInfo);
    void updateRoiInfo();
    RoiInfo getRoiInfo();

private:
    void retranslateUi();
    void initEvent();

private slots:
    void handleOk();
    void slotGetBlackLvl(QString cmd);
    void cellEdited(int idx, QLineEdit *);

protected:
    QVBoxLayout *main_layout;
    QGroupBox *isp_ifo_group;
    QGroupBox *blacklvl_group;
    QPushButton *save_btn;
    QPushButton *cancel_btn;
    QList<RawIspInfo> RawIspInfo_list;
    QList<int> black_lvl;
    int frame_num{0};
    int cur_idx{0};
    QSlider *frame_slider;
    QLineEdit *totalframes_line;
    QLineEdit *frame_line;
    QLineEdit *again_line;
    QLineEdit *dgain_line;
    QLineEdit *ispdgain_line;
    QLineEdit *exp_line;
    QLineEdit *exp_line1;
    QLineEdit *ratio_line;
    QLineEdit *iso_line;
    QLineEdit *ct_line;
    QLineEdit *lv_line;

    QLineEdit *blc1;
    QLineEdit *blc2;
    QLineEdit *blc3;
    QLineEdit *blc4;

    QLineEdit *roi_frame_line;
    QLineEdit *roi_width_line;
    QLineEdit *roi_height_line;
    QLineEdit *cropX_line;
    QLineEdit *cropY_line;
    QLineEdit *crop_width_line;
    QLineEdit *crop_height_line;
    RoiInfo roiInfo;
    bool roiMode = false;
};

#endif // ISP_INFO_DIALOG_H
