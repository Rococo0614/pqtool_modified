#ifndef RGBIR_CALIBRATION_PAGE_WIDGET_H
#define RGBIR_CALIBRATION_PAGE_WIDGET_H

#include "base_calibration_page_widget.hpp"
#include "calibration_utils.hpp"
#include "calibration_iso_list_widget.hpp"
#include <QComboBox>
#include <QGroupBox>
#include <QFileDialog>

typedef struct {
    float fCaliSlopeR;
    float fCaliSlopeG;
    float fCaliSlopeB;
} RGBIR_CALI_RESULT;

class RGBIRCalibrationPageWidget : public BaseCalibrationPageWidget
{
    Q_OBJECT

public:
    explicit RGBIRCalibrationPageWidget(QWidget *parent = nullptr);
    virtual void initWriteDataUI() override;
    bool calibration() override;
    QJsonArray getCalibrationData() override;
    //virtual bool checkShallExport() override;

protected:
    void initLeftArea() override;
    void retranslateUi() override;
    void initEvent();
    virtual QJsonObject GetModuleObjectToNetwork() override;
    void initQuickCalibrationUI();
    void OpenBlcFilePath();
    void OpenRgbirFilePath();
    // void initIsoListUI();
    int readCalibrationData(bool showMessage = true) override;
    void handleReceiveRgbirCalibration(int id);
    virtual QJsonObject GetModuleObjectToNetworkJsonrpc() override;

protected slots:
    void handleImportData(QString name, QJsonObject data);

private:
    QPushButton *edit_roi_btn;
    QWidget *roi_control_widget{nullptr};
    QComboBox *optype_combox;

    QPushButton *open_rgbir_btn;
    QPushButton *open_blc_btn;
    QLineEdit *path_edit_rgbir;
    QLineEdit *path_edit_blc;
    QPushButton *quick_rgbir_btn;
    float fCaliSlopeB = 0;
    float fCaliSlopeG = 0;
    float fCaliSlopeR = 0;
    const int RGB_ARRAY_SIZE = 1369;
    int blc_r_offset = 0;
    int blc_gr_offset = 0;
    int blc_gb_offset = 0;
    int blc_b_offset = 0;

    // 存储多个iso下的标定结果
    QMap<int, RGBIR_CALI_RESULT> CaliRgbirResult;
    QJsonArray rg_gain_array;
    QJsonArray bg_gain_array;
    QJsonArray gbr_gain_array;
    QJsonArray gir_gain_array;

    QList<RawImageItem *> rawItem;
    bool calibrated = false;
    bool quickmode = false;

};

#endif // RGBIR_CALIBRATION_PAGE_WIDGET_H
