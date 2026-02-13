#ifndef RAW_ANALYZER_WINDOW_H
#define RAW_ANALYZER_WINDOW_H

#include "multi_image_viewer_widget.hpp"
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


class RawAnalyzerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RawAnalyzerWindow(QWidget *parent = nullptr);
    void initUI();

protected:
    void initEvent();
    void closeEvent(QCloseEvent *event);

signals:
    void sigLabImgOk();
    void sigcloseChildWindow();

private slots:
    void handleSaveDstImage();
    void updateImageParameters();
private:
    QVector<int> parseNumberString(const QString& str);

private:
    QHBoxLayout *main_layout;
    QWidget *main_widget;
    MultiImageViewerWidget *img_viewer;
    //src
    QGroupBox *img_src_group;
    QLabel *src_filepath_label;
    QLineEdit *src_filepath_edit;
    QLabel *src_width_label;
    QLineEdit *src_width_line_edit;
    QLabel *src_height_label;
    QLineEdit *src_height_line_edit;
    QLabel *src_bits_label;
    QComboBox *src_bits_combobox;
    QLabel *src_bayer_label;
    QComboBox *src_bayer_combobox;
    QLabel *src_packed_label;
    QComboBox *src_packed_combobox;
    // dst
    QGroupBox *img_dst_group;
    QLabel *dst_filepath_label;
    QLineEdit *dst_filepath_edit;
    QLabel *dst_width_label;
    QLineEdit *dst_width_line_edit;
    QLabel *dst_height_label;
    QLineEdit *dst_height_line_edit;
    QLabel *dst_bits_label;
    QComboBox *dst_bits_combobox;
    QLabel *dst_bayer_label;
    QComboBox *dst_bayer_combobox;
    QLabel *dst_packed_label;
    QComboBox *dst_packed_combobox;
    QLabel *dst_frames_label;
    QLineEdit *dst_frames_line_edit;
    QLabel *roi_enable_label;
    QCheckBox *roi_enable_checkbox;
    // btn
    QPushButton *save_dst_btn;
};

#endif
