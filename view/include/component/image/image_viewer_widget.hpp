#ifndef IMAGE_VIEWER_WIDGET_H
#define IMAGE_VIEWER_WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QComboBox>
#include "image_label.hpp"
#include "calibration_utils.hpp"
#include "utils.hpp"
#include "color_histogram_window.hpp"
#include "raw_image_format_dialog.hpp"

class ImageViewerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImageViewerWidget(QWidget *parent = nullptr);

    void setFilePathHidden();
    void setImg(QPixmap img);
    void setImageLabel(ImageLabel *label);
    double getScaleFactor();
    void triggerGrid3AEnable(bool en);
    void setFvTable(QVector<QVector<int>> &fv);
    void zoomImage(double factor);
    void openImg();
    void openJpg();
    QString getPath();
    void openImgPath(QString img_path);
    void setPathHidden();
    void setSaveBtnHidden();
    void setHistogramName(QString btn_name);
    void setSeletedRect(QRectF rect);
    void updateLevelMeterHistogram(const QPixmap *image);
    void setSceneLabelState(bool state);
    void modifyChooseFileBtnName(QString name);
    void showRawStatisticBtn();
    void showEnGridCheckBox();

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
    bool isSupportedFileExtension(QString extension);
    void scaleByFactor(double factor);
    void setRawImageInfo(QString rawfilename, unsigned int width, unsigned int height, utils::RAW_BAYER_ID bayer, utils::RAW_BITS bits);
    void setLoadRawFailImg();

public slots:
    void handleShowRawItem(RawImageItem *raw_item);
    void handleShowImage(QImage &image);
    void handleSaveImage();
    void slotSelectFactor(QString text);
    void slotEnableGrid(int state);

private slots:
    void handleOpenColorHistogram();

signals:
    void sigSetImage(const QPixmap *image);

private:
    double scale_factor;
    QLineEdit *file_path;
    QPushButton *choose_file_btn;
    QCheckBox *en_grid;
    ImageLabel *image_label;
    QLabel *factor_label;
    QScrollArea *scrollArea;
    QPushButton *zoom_in_btn;
    QPushButton *zoom_out_btn;
    QComboBox *zoom_box;
    QPushButton *save_btn;
    QPushButton *color_histogram_btn;

    QString color_histogram_btn_name = "Color Histogram";
    QRectF selected_rect;

    QString raw_file_name = "";
    QString last_file_name = "";
    int image_width;
    int image_height;
    utils::RAW_BAYER_ID bayer;
    utils::RAW_BITS bits;
    unsigned char *praw_data{nullptr};
    RawImageFormatDialog *formt_dialog{nullptr};
    ColorHistogramWindow *color_hist_window{nullptr};
};

#endif // IMAGE_VIEWER_WIDGET_H
