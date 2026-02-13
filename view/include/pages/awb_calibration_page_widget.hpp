#ifndef AWB_CALIBRATION_PAGE_WIDGET_H
#define AWB_CALIBRATION_PAGE_WIDGET_H

#include "base_calibration_page_widget.hpp"
#include "lineedit_calibration_item_widget.hpp"
#include "chart_widget.hpp"
#include "calibration_gain_list_widget.hpp"

class AWBCalibrationPageWidget : public BaseCalibrationPageWidget
{
    Q_OBJECT

public:
    explicit AWBCalibrationPageWidget(QWidget *parent = nullptr);
    bool calibration() override;
    QJsonArray getCalibrationData() override;
    virtual bool checkShallExport() override;
    void HandleCalibrationRequst(QJsonObject params, RawConfigItem config);

protected:
    void initUI() override;
    void retranslateUi() override;
    void initEvent();
    void ClearData();
    bool handleImportAwbRaw();
    void handleImportAwbData();
    void handleExportAwbData(QString outpath = "");

    virtual QJsonObject GetModuleObjectToNetworkJsonrpc() override;
    int readCalibrationData(bool showMessage = true) override;

protected slots:
    void handleImportData(QString name, QJsonObject data);
    void handleDelPoint(int idx);
    void handleReceiveAWBCalibration(int id);
    int loadBlcData(QString path);
    bool selectItems();
    int QuickCalibrationLoadData(QString path);
    int handleQuickCalibration();

private:
    CalibrationGainListWidget *gain_list_widget;
    ChartWidget *pixel_curve;
    ChartWidget *planck_curve;
    QPushButton *import_raw_btn;
    QPushButton *import_blc_btn;
    QPushButton *calibration_btn;
    QPushButton *write_btn;
    QPushButton *export_btn;
    QPushButton *import_btn;
    QLineEdit *rgain_edit;
    QLineEdit *bgain_edit;
    QLineEdit *p1_edit;
    QLineEdit *p2_edit;
    QLineEdit *q_edit;
    QLineEdit *a_edit;
    QLineEdit *b_edit;
    QLineEdit *c_edit;

    // calibration result
    unsigned short wb_r_gain{0};
    unsigned short wb_g_gain{0};
    unsigned short wb_b_gain{0};
    // calibration param
    const int RGB_ARRAY_SIZE = 1369;
    int blc_r_offset = 0;
    int blc_gr_offset = 0;
    int blc_gb_offset = 0;
    int blc_b_offset = 0;
    float blc_r_gain = 0.0;
    float blc_gr_gain = 0.0;
    float blc_gb_gain = 0.0;
    float blc_b_gain = 0.0;
    unique_ptr<int []> lsc_r_gain{new int [RGB_ARRAY_SIZE]{0}};
    unique_ptr<int []> lsc_g_gain{new int [RGB_ARRAY_SIZE]{0}};
    unique_ptr<int []> lsc_b_gain{new int [RGB_ARRAY_SIZE]{0}};

    const int MAX_CALIB_PT_NUM = 7;
    float rgain[3];
    float bgain[3];
    float tempK[3];
    int paraK[6];
    int StaticWB[4];

    bool quickMode = false;
    bool showMessage = true;
    QMap<QString, QJsonObject> params;
    CALIBRATION_STATUS cali_status = CALIBRATION_STATUS::READY;
    RawConfigItem config;
    QList<RawImageItem *> rawItem;
    QList<GainItem *> gain_item;
    QList<GainItem *> gain_select_item;
};

#endif // AWB_CALIBRATION_PAGE_WIDGET_H
