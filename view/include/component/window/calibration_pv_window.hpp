#ifndef CALIBRATION_PV_WINDOW_H
#define CALIBRATION_PV_WINDOW_H

#include "image_viewer_widget.hpp"
#include "base_calibration_page_widget.hpp"
#include "otp_pdaf_cali.h"
#include <QDialog>
#include <QComboBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QGroupBox>
#include <QMainWindow>


class CalibrationPVWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CalibrationPVWindow(QWidget *parent = nullptr);
    void initUI();
    QWidget *createRoiControlUI(ImageLabel *img_label);
    void GetImgRgb(double *r, double *g, double *b);
    void  OpenImage(RawImageItem *item);
protected:
    void initEvent();
signals:
    void sigLabImgOk();
private slots:
    void handleOk();
    void handleRectToCropInfo(QRectF rect, double factor);
    void handleCropInfoToRect();

private:
    QVBoxLayout *main_layout;
    QWidget *main_widget;
    QPushButton *ok_btn;
    QPushButton *cancel_btn;
    ImageViewerWidget *input_img;
    ImageLabel * img_label;
    QLineEdit *width_edit;
    QLineEdit *height_edit;
    QLineEdit *cropX_edit;
    QLineEdit *cropY_edit;
    QLineEdit *cropW_edit;
    QLineEdit *cropH_edit;
    RawImageItem *currItem{nullptr};
};

#endif // ISP_INFO_DIALOG_H
