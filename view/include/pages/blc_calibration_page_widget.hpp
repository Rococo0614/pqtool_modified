#ifndef BLC_CALIBRATION_PAGE_WIDGET_HPP
#define BLC_CALIBRATION_PAGE_WIDGET_HPP

#include "base_calibration_page_widget.hpp"
#include "calibration_utils.hpp"
#include <QPushButton>
#include <QFileDialog>

typedef struct {
    int offset_r;
    int offset_gr;
    int offset_gb;
    int offset_b;
    float gain_r;
    float gain_gr;
    float gain_gb;
    float gain_b;
} BLC_CALI_RESULT;

class BLCCalibrationPageWidget : public BaseCalibrationPageWidget
{
    Q_OBJECT

public:
    explicit BLCCalibrationPageWidget(QWidget *parent = nullptr);
    bool calibration() override;
    QJsonArray getCalibrationData() override;
    void initWriteDataUI() override;
    virtual bool checkShallExport() override;
    void HandleCalibrationRequst(QJsonObject params, RawConfigItem config);

protected:
    void initLeftArea() override;
    void initEvent();
    void ClearData();
    virtual QJsonObject GetModuleObjectToNetwork() override;
    void handleImportData(QString name, QJsonObject data);
    void initQuickCalibrationUI();
    void OpenFilePath();
    int QuickCalibrationLoadData(QString path, bool externRequest = false);
    int handleQuickCalibration(bool showMessage = true);
    int GetIsoFromFileName(QString fileName);

    int readCalibrationData(bool showMessage = true) override;
    void handleReceiveBlcCalibration(int id);
    virtual QJsonObject GetModuleObjectToNetworkJsonrpc() override;

private:
    bool calibrated = false;
    QComboBox *optype_combox;
    QComboBox *method_combox;
    QPushButton *open_btn;
    QLineEdit *path_edit;
    QPushButton *quick_btn;

    QMap<int, BLC_CALI_RESULT> caliResultMean;
    QMap<int, BLC_CALI_RESULT> caliResultMedian;

    QList<RawImageItem *> rawItem;
    bool quickMode = false;
    RawConfigItem config;
    CALIBRATION_STATUS cali_status = CALIBRATION_STATUS::READY;
    QMap<QString, QJsonObject> params;

};

#endif // BLC_CALIBRATION_PAGE_WIDGET_HPP
