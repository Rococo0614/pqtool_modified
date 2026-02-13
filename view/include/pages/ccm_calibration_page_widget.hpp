#ifndef CCM_CALIBRATION_PAGE_WIDGET_H
#define CCM_CALIBRATION_PAGE_WIDGET_H

#include "ccm_lab_window.hpp"
#include "base_calibration_page_widget.hpp"
#include "image_viewer_widget.hpp"
#include "combobox_calibration_item_widget.hpp"
#include "double_slider_calibration_item_widget.hpp"
#include "lineedit_calibration_item_widget.hpp"
#include "otp_pdaf_cali.h"
#include <QComboBox>
#include <QGroupBox>
#include <QFileDialog>

class CCMCalibrationPageWidget : public BaseCalibrationPageWidget
{
    Q_OBJECT

public:
    explicit CCMCalibrationPageWidget(QWidget *parent = nullptr);
    virtual void initWriteDataUI() override;
    virtual void initCTListUI() override;
    bool calibration() override;
    QJsonArray getCalibrationData() override;
    virtual bool checkShallExport() override;
    virtual void SetCalibrationBtnState(bool state) override;
    void HandleCalibrationRequst(QJsonObject params, RawConfigItem config);
    int readCalibrationData(bool showMessage = true) override;
    void handleReceiveCcmCalibration(int id);

public slots:
    void handleCv184xMlscMode(bool flag) {is_cv184x_mlsc_chroma_luma = flag;}

protected:
    void initLeftArea() override;
    void retranslateUi() override;
    void initEvent();
    int loadBlcData(QString path);
    void loadLscData(int idx);
    int QuickCalibrationLoadData(QString path);
    int handleQuickCalibration();
    virtual QJsonObject GetModuleObjectToNetworkJsonrpc() override;
    int GetColorBlocks(int idx);
    void handleExportData(QString path);
    void ClearData();

protected slots:
    void handleImportData(QString name, QJsonObject data);
    void handleCustomizeIspGamma();
    void handleCustomizeLabReference();
    void handleCustomLabData();

private:
    void SetCustomDefaultLab();

private:
    const int PATCH_WEIGHT_ROW = 4;
    const int PATCH_WEIGHT_COL = 6;
    QPushButton *edit_roi_btn;
    QPushButton *import_blc_cali_btn;
    QPushButton *import_lsc_cali_btn;
    DoubleSliderCaliItemWidget *strength_slider{nullptr};
    QTableWidget *weight_table{nullptr};
    QGroupBox *delta_group;
    QLineEdit *mean_Eab;
    QLineEdit *mean_Cab;
    QLineEdit *max_Eab;
    QLineEdit *max_Cab;
    QLineEdit *mean_E94;
    QLineEdit *mean_C94;
    QLineEdit *max_E94;
    QLineEdit *max_C94;
    ComboboxCaliItemWidget *isp_gamma_item{nullptr};
    ComboboxCaliItemWidget *lab_ref_item{nullptr};
    QWidget *roi_control_widget{nullptr};
    CCMLabWindow *lab{nullptr};

    const int RGB_ARRAY_SIZE = 1369;
    const int ISP_GAMMA_SIZE = 4096;
    const int LAB_REFERENCE_SIZE = 24;
    int blc_r_offset = 0;
    int blc_gr_offset = 0;
    int blc_gb_offset = 0;
    int blc_b_offset = 0;
    float blc_r_gain = 0.0;
    float blc_gr_gain = 0.0;
    float blc_gb_gain = 0.0;
    float blc_b_gain = 0.0;
    int center_x = 0;
    int center_y = 0;
    unique_ptr<int []> lsc_r_gain{new int [RGB_ARRAY_SIZE]{0}};
    unique_ptr<int []> lsc_g_gain{new int [RGB_ARRAY_SIZE]{0}};
    unique_ptr<int []> lsc_b_gain{new int [RGB_ARRAY_SIZE]{0}};
    unique_ptr<int []> isp_gamma{new int [ISP_GAMMA_SIZE]{0}};
    unique_ptr<double []> lab_ref_l{new double [LAB_REFERENCE_SIZE]{0}};
    unique_ptr<double []> lab_ref_a{new double [LAB_REFERENCE_SIZE]{0}};
    unique_ptr<double []> lab_ref_b{new double [LAB_REFERENCE_SIZE]{0}};
    unique_ptr<int []> rlsc_r_gain{new int [RLSC_TABLE_SIZE]{0}};
    unique_ptr<int []> rlsc_g_gain{new int [RLSC_TABLE_SIZE]{0}};
    unique_ptr<int []> rlsc_b_gain{new int [RLSC_TABLE_SIZE]{0}};
    unique_ptr<int []> rlsc_ir_gain{new int [RLSC_TABLE_SIZE]{0}};

    bool is_cv184x_mlsc_chroma_luma {false};
    LSC_TYPE lsc_type;

    // calibration result
    unique_ptr<float []> ccm_result{new float [CCM_MATRIX_SIZE]{0}};

    bool quickMode = false;
    QMap<QString, QJsonObject> params;
    CALIBRATION_STATUS cali_status = CALIBRATION_STATUS::READY;
    RawConfigItem config;
    QList<RawImageItem *> rawItem;
    ISP_ROI_S color_block;
    QList<CTItem *> ct_item_list;
};

#endif // CCM_CALIBRATION_PAGE_WIDGET_H
