#ifndef MULTI_IMAGE_VIEWER_WIDGET_H
#define MULTI_IMAGE_VIEWER_WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QComboBox>
#include "image_label.hpp"
#include "level_meter_histogram_window.hpp"
#include "raw_statistics_window.hpp"
#include "color_histogram_window.hpp"
#include "calibration_utils.hpp"
#include "utils.hpp"
#include "global_data.hpp"
#include "simple_pipe_dialog.hpp"

class MultiImageViewerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MultiImageViewerWidget(QWidget *parent = nullptr);

    void setFilePathHidden();
    void setFileButtonHidden();
    void setChooseFileBtnName();
    void setColorHistogramButtonHidden();
    void setImg(QPixmap img);
    void setImageLabel(ImageLabel *label);
    double getScaleFactor();
    void setYUVImgData(QByteArray &data, YUV_HEADER *header);
    int setRawImgData(QByteArray &data, unsigned int width, unsigned int height,
            utils::RAW_BAYER_ID bayer_id, utils::RAW_BITS bits, utils::RAW_PACK_TYPE);
    void setJPGImgData(QList<QImage> list);
    void setDefaultImgDisplay();
    void updatePrompt(utils::IMAGE_TYPE img_type, int cur, int total);
    void triggerGrid3AEnable(bool en);
    void setFvTable(QVector<QVector<int>> &fv);
    void saveJpgActionParam(int img_src_index);
    void showEnGridCheckBox();
    void handleSimplePipeBnt();
    void hideSimplePipeBtn();
    void updateSimplePipeAttr();
    void showRawStatisticBtn();
    void showLevelMeterBtn();
    void updateLevelMeterHistogram(const QPixmap *image);
    void updateRawStatistic(const QPixmap *image, bool is_update_colortype=false);
    void setSeletedRect(QRectF rect);
    void roiRectRefSub();
    void roiRectRefAdd();
    int getCenterRoiAvgLuma(int &r, int &g, int &b);

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void changeEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent  *event) override;

private:
    void initUI();
    void retranslateUi();
    void scaleImg();
    void zoomin();
    void zoomout();
    void openImg();
    void openImgPath(QString img_path);
    bool isSupportedFileExtension(QString extension);
    void reset();
    void scaleByFactor(double factor);
    void setLoadRawFailImg();
    void handleCropInfoToRect();

public slots:
    void handleShowRawItem(RawImageItem *raw_item);
    void handleShowImage(QImage &image);
    void handleSaveImage();
    void clearImageData();
    void handleImgNavigat(int dir);
    void slotSelectFactor(QString text);
    void handleChildWindowEvent(int window_type);
    void slotPositionChange(QString text);
    void slotEnableGrid(int state);
    void handleRectToCropInfo(QRectF rect, double factor);
    void enableRect();
    void disableRect();

private slots:
    void handleOpenColorHistogram();
    void handleOpenLevelMeter();
    void handleOpenRawStatistics();
    void handleSetImage(const QPixmap *image);

signals:
    void sigSetImage(const QPixmap *image);
    void sigClearCaptureInfo();

public:
    unsigned int mul_width;
    unsigned int mul_height;
    utils::RAW_BITS mul_bits;
    utils::RAW_BAYER_ID mul_bayer;
    utils::RAW_PACK_TYPE mul_raw_pack_type;
    QString file_path;
    int total_frame_cnt{0};
    utils::IMAGE_TYPE img_type{utils::IMAGE_TYPE::IMAGE_YUV};
    QRectF selected_rect;
    QList<QImage> img_list;
    QList<QByteArray> raw_list;
    QList<QByteArray> yuv_list;

private:
    double scale_factor;
    QLineEdit *file_path_line_edit;
    QPushButton *choose_file_btn;
    QCheckBox *en_grid;
    ImageLabel *image_label;
    QLabel *factor_label;
    QLabel *status_label;
    QScrollArea *scrollArea;
    QPushButton *zoom_in_btn;
    QPushButton *zoom_out_btn;
    QComboBox *zoom_box;
    QPushButton *save_btn;
    QPushButton *color_histogram_btn;
    QPushButton *level_meter_btn;
    QPushButton *raw_statistics_btn;
    QLabel *prompt_label;
    QPushButton *prev_btn;
    QPushButton *next_btn;
    QPushButton *simple_pipe_btn;
    int cur_idx{0};
    utils::BLC_ATTR_S blc_attr;
    utils::WBG_ATTR_S wbg_attr;
    utils::SIMPLE_PIPE_ENABLE_ATTR_S simple_pipe_enable;

    RawStatisticsWindow *raw_statistic_window;
    LevelMeterHistogramWindow *level_hist_window;
    ColorHistogramWindow *color_hist_window;
    SimplePipeDialog *simple_pipe_dialog;
    // rect
    QHBoxLayout *rect_h_layout;
    QLabel *width_lab;
    QLabel *height_lab;
    QLabel *cropX_lab;
    QLabel *cropY_lab;
    QLabel *cropW_lab;
    QLabel *cropH_lab;
    QLineEdit *width_edit;
    QLineEdit *height_edit;
    QLineEdit *cropX_edit;
    QLineEdit *cropY_edit;
    QLineEdit *cropW_edit;
    QLineEdit *cropH_edit;

    int roi_rect_ref_cnt{0};
};

#endif // IMAGE_VIEWER_WIDGET_H
