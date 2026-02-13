#ifndef QUICK_CALIBRATION_PAGE_WIDGET_HPP
#define QUICK_CALIBRATION_PAGE_WIDGET_HPP

#include "base_calibration_page_widget.hpp"
#include "calibration_utils.hpp"
#include "double_slider_calibration_item_widget.hpp"
#include "combobox_calibration_item_widget.hpp"
#include <QPushButton>
#include <QFileDialog>
#include <QGroupBox>
#include <QPlainTextEdit>


class QuickCalibrationPageWidget : public BaseCalibrationPageWidget
{
    Q_OBJECT

public:
    explicit QuickCalibrationPageWidget(QWidget *parent = nullptr);
    ~QuickCalibrationPageWidget();
    void handleCalibrationStatus(CALIBRATION_MODULE_ID id, CALIBRATION_STATUS status, QString log);

protected:
    void initUI() override;
    QGroupBox *CreateSettingGroupbox();
    QGroupBox *CreateCalibrationGroupbox();
    QGroupBox *CreateLogGroupbox();
    void handleCalibration();
    void initEvent();
    void handleTimeOut();
    void EditRawConfig(QString path = "");
    void handleCustomizeIspGamma();
    void showLog(const QString &log, LOG_LEVEL log_level);
    void OpenFilePath();
signals:
    void sigStartBlcCalibration(QJsonObject param, RawConfigItem config);
    void sigStartLscCalibration(QJsonObject param, RawConfigItem config);
    void sigStartAwbCalibration(QJsonObject param, RawConfigItem config);
    void sigStartCcmCalibration(QJsonObject param, RawConfigItem config);
    void sigStartNoiseCalibration(QJsonObject param, RawConfigItem config);
    void sigStartLblcCalibration(QJsonObject param, RawConfigItem config);

private:
    QComboBox *optype_combox{nullptr};
    QComboBox *blc_method_combox{nullptr};
    QComboBox *lsc_type_combox{nullptr};
    QComboBox *lsc_lens_combox{nullptr};
    DoubleSliderCaliItemWidget *strength_slider{nullptr};
    ComboboxCaliItemWidget *isp_gamma_item{nullptr};
    ComboboxCaliItemWidget *lab_ref_item{nullptr};
    QTableWidget *weight_table{nullptr};
    QCheckBox *TEAISP_bnr_checkbox{nullptr};
    QPushButton *open_btn{nullptr};
    QLineEdit *path_edit{nullptr};
    QPushButton *cali_btn{nullptr};
    QPlainTextEdit *log_edit{nullptr};
    QComboBox *log_level_box{nullptr};

    const int RGB_ARRAY_SIZE = 1369;
    const int ISP_GAMMA_SIZE = 4096;
    const int LAB_REFERENCE_SIZE = 24;
    const int PATCH_WEIGHT_ROW = 4;
    const int PATCH_WEIGHT_COL = 6;

    QTimer *timer;
    RawConfigItem rawConfig;
    unique_ptr<int []> isp_gamma{new int [ISP_GAMMA_SIZE]{0}};
    CALIBRATION_MODULE_ID curID = CALIBRATION_MODULE_ID::BLC;
    CALIBRATION_MODULE_ID maxID = CALIBRATION_MODULE_ID::NOISE_PROFILE;
};

#endif
