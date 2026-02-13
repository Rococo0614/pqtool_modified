#ifndef NR_CALIBRATION_PAGE_WIDGET_H
#define NR_CALIBRATION_PAGE_WIDGET_H

#include "base_calibration_page_widget.hpp"
#include "lineedit_calibration_item_widget.hpp"
#include "calibration_iso_list_widget.hpp"

class NoiseProfileCalibrationPageWidget : public BaseCalibrationPageWidget
{
    Q_OBJECT

public:
    explicit NoiseProfileCalibrationPageWidget(QWidget *parent = nullptr);
    virtual void addRawImgItem(RawImageItem *raw_img_item) override;
    virtual bool calibration() override;
    QJsonArray getCalibrationData() override;
    QString getCalibrationHeaderData();
    virtual void exportData(QString file_path) override;
    virtual bool checkShallExport() override;
    void HandleCalibrationRequst(QJsonObject params, RawConfigItem config);

protected:
    void initLeftArea() override;
    void retranslateUi() override;
    void initEvent();

    int readCalibrationData(bool showMessage = true) override;
    void handleReceiveNoiseProfileCalibration(int id);
    virtual QJsonObject GetModuleObjectToNetworkJsonrpc() override;
    virtual bool getCalibrationBnrBoxState() override;
    int loadBlcData(QString path, int iso);
    int GetColorBlocks(int idx);
    void handleExportData(QString path);
    int QuickCalibrationLoadData(QString path);
    int handleQuickCalibration(QString path);
    void ClearData();

protected slots:
    void handleImportData(QString name, QJsonObject data);

private:
    double calculateMean(QList<double> values);
    double calculateStandardDeviation(QList<double> values);
    void solveLeastMeanSquare(QList<double> means, QList<double> stds, double &m, double &c);
    void calibrateBNR();
    void calibrateYNR();
    void initIsoListUI();

private:
    QPushButton *edit_roi_btn;
    QPushButton *import_roi_btn;
    QPushButton *export_roi_btn;
    QCheckBox *TEAISP_bnr_checkbox{nullptr};
    LineEditCaliItemWidget *frame_item;
    QComboBox *iso_item;
    CalibrationIsoListWidget *iso_list_widget;
    QWidget *roi_control_widget{nullptr};
    bool bnr_check_state = false;

    int blc_r_offset = 0;
    int blc_gr_offset = 0;
    int blc_gb_offset = 0;
    int blc_b_offset = 0;
    float blc_r_gain = 0.0;
    float blc_gr_gain = 0.0;
    float blc_gb_gain = 0.0;
    float blc_b_gain = 0.0;

    // calibration result
    float fCaliSlopeB = 0;
    float fCaliInterceptB = 0;
    float fCaliSlopeGb = 0;
    float fCaliInterceptGb = 0;
    float fCaliSlopeGr = 0;
    float fCaliInterceptGr = 0;
    float fCaliSlopeR = 0;
    float fCaliInterceptR = 0;
    float fCaliSlopeMax = 0.0;
    float fCaliInterceptMax = 0.0;
    int frames_cnt = 0;

    QJsonArray CalibrationCoef_array;
    QJsonArray CalibrationCoef_max_array;

    bool quickMode = false;
    CALIBRATION_STATUS cali_status = CALIBRATION_STATUS::READY;
    RawConfigItem config;
    QList<RawImageItem *> rawItem;
    QList<IsoItem *> iso_item_list;
    ISP_ROI_S color_block;
};

#endif // NR_CALIBRATION_PAGE_WIDGET_H
