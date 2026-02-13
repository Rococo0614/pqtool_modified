#ifndef CCM_LAB_IMAGE_WINDOW_H
#define CCM_LAB_IMAGE_WINDOW_H

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


class CcmLabImageWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CcmLabImageWindow(QWidget *parent = nullptr);
    void initUI();
    QWidget *createRoiControlUI(ImageLabel *img_label);
    void GetImgRgb(double *r, double *g, double *b);
    void  OpenImage(QString path);
protected:
    void retranslateUi();
    void initEvent();
signals:
    void sigLabImgOk();
private slots:
    void handleOk();

private:
    QVBoxLayout *main_layout;
    QWidget *main_widget;
    QPushButton *edit_roi_btn;
    QPushButton *ok_btn;
    QPushButton *cancel_btn;
    ImageViewerWidget *input_img;
    ImageLabel *input_img_label;
    QWidget *roi_control_widget{nullptr};
    double red[24];
    double green[24];
    double blue[24];
    QString path;
};

#endif // ISP_INFO_DIALOG_H
