#ifndef BASE_CALIBRATION_PAGE_WIDGET_HPP
#define BASE_CALIBRATION_PAGE_WIDGET_HPP

#include "base_page_widget.hpp"
#include "base_calibration_item_widget.hpp"
#include "calibration_utils.hpp"
#include "image_viewer_widget.hpp"
#include "file_dialog.hpp"
#include "global_data.hpp"
#include "utils.hpp"
#include "calibration_utils.hpp"
#include "calibration_ct_list_widget.hpp"
#include "calibration_rgb_list_widget.hpp"
#include "blc_histogram_widget.hpp"
#include "status_log_widget.hpp"
#include <QPushButton>

const QString PREFIX_CALI_STR = "Cali ";

class BaseCalibrationPageWidget : public BasePageWidget
{
    Q_OBJECT

public:
    explicit BaseCalibrationPageWidget(int module_id, QWidget *parent = nullptr);
    virtual void addRawImgItem(RawImageItem *raw_img_item);
    void clearAllRawImgItem();
    virtual void exportData(QString file_path);
    void saveData(QString file_path);
    virtual QJsonArray getCalibrationData();
    virtual bool calibration();
    virtual bool checkShallExport();
    virtual void SetCalibrationBtnState(bool state);

    // init ui functions
    void initImportRawUI();
	void initImportJpgUI();
    QPushButton *initImportBlcUI();
    QPushButton *initImportLscUI();
    QPushButton *initImportRgbirUI();
    void initCaliUI();
    void initDisplayDataUI();
    virtual void initWriteDataUI();
    void initExportDataUI();
    virtual void initCTListUI();
    QWidget *createRoiControlUI(ImageLabel *img_label);
    void setAcceptScene(SCENE s);
    SCENE getAcceptScene(void);
    void initImportDataUI();

    QStringList getStructList() { return struct_list; }
    QMap<QString, QString> getStructGetApiMap() { return struct_get_api_map; }
    QMap<QString, QString> getStructSetApiMap() { return struct_set_api_map; }

protected:
    void writeToIni() override;
    virtual QJsonObject GetModuleObjectToNetwork() override;
    virtual bool getCalibrationBnrBoxState();
    void writeToNetwork() override;
    void initLeftArea() override;
    void initCenterArea() override;
    void initRightArea() override;
    void retranslateUi() override;
    void displayInput();
    void displayOutput();

    int sendDataToNetwork(bool showMessage = true);
    virtual int readCalibrationData(bool showMessage = true);
    virtual QJsonObject GetModuleObjectToNetworkJsonrpc() override;

public slots:
    int writeCalibrationData(bool showMessage = true);
    void handleImportData();
    void handleExportData();
    void handleSave();

signals:
    void sigCalibration();
    void sigImportRaw();
	void sigImportSourceJpg();
	void sigImportTargetJpg();
    void sigShowInputRawItem(RawImageItem *raw_item);
    void sigShowOutputRawImage(QImage &image);
    void sigShowOutputBlcImage(QImage &image);
    void sigChangeCaliStatus(CALIBRATION_STATUS status);
    void sigImportData(QString name, QJsonObject data);
    void sigImportAwbRaw();
    void sigCaliStatus(CALIBRATION_MODULE_ID id, CALIBRATION_STATUS status, QString log);

protected:
    QVBoxLayout *param_layout;
    QVector<RawImageItem *> raw_img_item_vec;
    QTabWidget *display_tab_widget;
    ImageViewerWidget *input_img;
    ImageViewerWidget *output_img;
    ImageLabel *input_image_label;
    ImageLabel *output_image_label;
    CalibrationCTListWidget *ct_list_widget;
	ImageViewerWidget *blc_output_img_mean;
	ImageViewerWidget *blc_output_img_median;
	ImageLabel *blc_output_image_label_mean;
	ImageLabel *blc_output_image_label_median;
	BlcHistogramWidget *blc_histogram;
    bool blc_imported{false};
    bool lsc_imported{false};
    bool yuv_calib{false};
	//clut
	QTabWidget *input_display_tab_widget;
	QTabWidget *output_display_tab_widget;
	ImageViewerWidget *input_raw;
	ImageViewerWidget *input_jpg;
	CalibrationRGBListWidget *rgb_list_widget;
	ImageLabel *input_raw_label;
	ImageLabel *input_jpg_label;

private:
    QList<BaseCalibrationItemWidget *> base_cali_item_list;
    SCENE accept_scene;

    QStringList struct_list;
    QMap<QString, QString> struct_get_api_map;
    QMap<QString, QString> struct_set_api_map;
};

#endif // BASE_CALIBRATION_PAGE_WIDGET_HPP
