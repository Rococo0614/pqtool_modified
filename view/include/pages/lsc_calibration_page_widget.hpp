#ifndef LSC_CALIBRATION_PAGE_WIDGET_HPP
#define LSC_CALIBRATION_PAGE_WIDGET_HPP

#include "base_calibration_page_widget.hpp"
#include "lineedit_calibration_item_widget.hpp"
#include "checkbox_calibration_item_widget.hpp"
#include "combobox_calibration_item_widget.hpp"
#include "calibration_utils.hpp"
#include "image_viewer_widget.hpp"
#include "calibration_utils.hpp"
#include <QPushButton>
#include <QGroupBox>
#include <memory>

class LSCCalibrationPageWidget : public BaseCalibrationPageWidget
{
    Q_OBJECT

public:
    explicit LSCCalibrationPageWidget(QWidget *parent = nullptr);
    bool calibration() override;
    QJsonArray getCalibrationData() override;
    virtual bool checkShallExport() override;
    virtual void initCTListUI() override;
    void HandleCalibrationRequst(QJsonObject params, RawConfigItem config);
    int QuickCalibrationLoadData(QString path, bool externRequest = false);
    int loadBlcData(QString path);
    int readCalibrationData(bool showMessage = true) override;
    void handleReceiveRlscCalibration(int id);
    void handleReceiveMlscCalibration(int id);
    //bool getCv184xMlscMode() {return is_cv184x_mlsc_chroma_luma;}
    void exportData(QString file_path) override;

signals:
    int sigCv184xMlscChromaLumaFlag(bool flag);

protected:
    void initLeftArea() override;
    void retranslateUi() override;
    void initEvent();
    void ClearData();
    void initQuickCalibrationUI();
    void OpenFilePath();
    int handleQuickCalibration(bool showMessage = true);
    void handleShorCut();

    virtual QJsonObject GetModuleObjectToNetworkJsonrpc() override;

protected slots:
    void handleImportData(QString name, QJsonObject data);
    void handleLscSetting();

private:
    LineEditCaliItemWidget *grid_number_x_item;
    LineEditCaliItemWidget *grid_number_y_item;
    ComboboxCaliItemWidget *calib_type;
    ComboboxCaliItemWidget *len_type;
    ComboboxCaliItemWidget *cv184x_calibration_algo {nullptr};
    QPushButton *analyze_fisheye_btn;
    LineEditCaliItemWidget *center_x_item;
    LineEditCaliItemWidget *center_y_item;
    LineEditCaliItemWidget *radius_item;
    QPushButton *open_btn;
    QLineEdit *path_edit;
    QPushButton *quick_btn;
    int blc_r_offset = 0;
    int blc_gr_offset = 0;
    int blc_gb_offset = 0;
    int blc_b_offset = 0;
    float blc_r_gain = 0.0;
    float blc_gr_gain = 0.0;
    float blc_gb_gain = 0.0;
    float blc_b_gain = 0.0;
    // calibration result
    int center_x = 0;
    int center_y = 0;
    int radius = 0;
    int norm = 0;
    unique_ptr<int []> mlsc_r_gain{new int [MLSC_TABLE_SIZE]{0}};
    unique_ptr<int []> mlsc_g_gain{new int [MLSC_TABLE_SIZE]{0}};
    unique_ptr<int []> mlsc_b_gain{new int [MLSC_TABLE_SIZE]{0}};
    unique_ptr<int []> rlsc_r_gain{new int [RLSC_TABLE_SIZE]{0}};
    unique_ptr<int []> rlsc_g_gain{new int [RLSC_TABLE_SIZE]{0}};
    unique_ptr<int []> rlsc_b_gain{new int [RLSC_TABLE_SIZE]{0}};
    unique_ptr<int []> rlsc_ir_gain{new int [RLSC_TABLE_SIZE]{0}};
    QList<RawImageItem *> rawItem;
    QList<CTItem *> ctItem_list,ctItem1_list;
    bool quickMode = false;
    QJsonObject radius_param_obj;
    QJsonObject mesh_param_obj;
    //bool is_cv184x_mlsc_chroma_luma {false};

    RawConfigItem config;
    CALIBRATION_STATUS cali_status = CALIBRATION_STATUS::READY;
};

#endif // LSC_CALIBRATION_PAGE_WIDGET_HPP
