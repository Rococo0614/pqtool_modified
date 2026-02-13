#ifndef LBLC_CALIBRATION_PAGE_WIDGET_HPP
#define LBLC_CALIBRATION_PAGE_WIDGET_HPP

#include "base_calibration_page_widget.hpp"
#include "calibration_iso_list_lblc_widget.hpp"
#include "lineedit_calibration_item_widget.hpp"
#include "checkbox_calibration_item_widget.hpp"
#include "combobox_calibration_item_widget.hpp"
#include "calibration_utils.hpp"
#include "image_viewer_widget.hpp"
#include "calibration_utils.hpp"
#include <QPushButton>
#include <QCheckBox>
#include <QGroupBox>
#include <memory>

class LBLCCalibrationPageWidget : public BaseCalibrationPageWidget
{
    Q_OBJECT

public:
    explicit LBLCCalibrationPageWidget(QWidget *parent = nullptr);
    bool calibration() override;
    QJsonArray getCalibrationData() override;
    virtual bool checkShallExport() override;
    void initISOListUI();
    void HandleCalibrationRequst(QJsonObject params, RawConfigItem config);

protected:
    void initLeftArea() override;
    void retranslateUi() override;
    void initEvent();
    void initQuickCalibrationUI();
    void OpenFilePath();
    void ClearData();
    int handleQuickCalibration(bool showMessage = true);
    int GetIsoFromFileName(QString fileName);
    virtual QJsonObject GetModuleObjectToNetworkJsonrpc() override;
    int QuickCalibrationLoadData(QString path, bool externRequest = false);

protected slots:
    void handleImportData(QString name, QJsonObject data);

private:
    CalibrationISOLBlcListWidget *iso_list_widget;
    QPushButton *open_btn;
    QLineEdit *path_edit;
    QPushButton *quick_btn;
    // calibration result
    QList<RawImageItem *> rawItem;
    QList<ISOItem *> isoItem_list, isoItem1_list;
    bool calibrated = false;
    bool quickMode = false;

    float lblc_rr_gain = 0.0;
    float lblc_gr_gain = 0.0;
    float lblc_gb_gain = 0.0;
    float lblc_bb_gain = 0.0;
    int block_nums = LBLC_BLOCK_NUMS;
    unique_ptr<int []> lblc_rr_offset{new int [LBLC_BLOCK_NUMS]{0}};
    unique_ptr<int []> lblc_gr_offset{new int [LBLC_BLOCK_NUMS]{0}};
    unique_ptr<int []> lblc_gb_offset{new int [LBLC_BLOCK_NUMS]{0}};
    unique_ptr<int []> lblc_bb_offset{new int [LBLC_BLOCK_NUMS]{0}};

    RawConfigItem config;
    CALIBRATION_STATUS cali_status = CALIBRATION_STATUS::READY;
};

#endif // LBLCCalibrationPageWidget
