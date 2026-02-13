#ifndef RGBIRTORGGB_CALIBRATION_PAGE_WIDGET_H
#define RGBIRTORGGB_CALIBRATION_PAGE_WIDGET_H

#include "ccm_lab_window.hpp"
#include "base_calibration_page_widget.hpp"
#include "image_viewer_widget.hpp"
#include "combobox_calibration_item_widget.hpp"
#include "double_slider_calibration_item_widget.hpp"
#include "lineedit_calibration_item_widget.hpp"
#include "calibration_iso_list_widget.hpp"
#include "otp_pdaf_cali.h"
#include "rgbir_cali.h"
#include <QComboBox>
#include <QGroupBox>
#include <QFileDialog>

class RGBIRToRGGBCalibrationPageWidget : public BaseCalibrationPageWidget
{
    Q_OBJECT
public:
    explicit RGBIRToRGGBCalibrationPageWidget(QWidget *parent = nullptr);
    void setDir_path(QString select_dir_path);

protected:
    void handlConvertRgbirRaw();
    void OpenSaveFilePath();
    void initEvent();
    void initLeftArea() override;
    void initConvertRaw();
    void handleImportData(QString name, QJsonObject data);
    void saveRawFile(QByteArray data, QString &raw_file_name, int frameNum);
    QString get_save_raw_path(QVector<QString> &raw_info_vec,  QString bayerid_convert);
    QJsonArray getCalibrationData() override;
    virtual QJsonObject GetModuleObjectToNetworkJsonrpc() override;
    QByteArray convertRawImageToQByteArray(int* rawImage, int width, int height);

private:
    QPushButton *convertRaw_btn;
    QPushButton *open_btn;

    QPushButton *quick_btn;
    QComboBox *iso_item;

    QLineEdit *path_edit;
    QString dir_path;
    QByteArray raw_data;
    QList<RawImageItem *> rawItem;
    float fCaliSlopeB = 0.0;
    float fCaliSlopeG = 0.0;
    float fCaliSlopeR = 0.0;

    int blc_r_offset = 0;
    int blc_gr_offset = 0;
    int blc_gb_offset = 0;
    int blc_b_offset = 0;
    float blc_r_gain = 0;
    float blc_gr_gain = 0;
    float blc_gb_gain = 0;
    float blc_b_gain = 0;
};

#endif // RGBIRTORGGB_CALIBRATION_PAGE_WIDGET_H

