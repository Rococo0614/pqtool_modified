#ifndef RAW_STATISTICS_WINDOW_H
#define RAW_STATISTICS_WINDOW_H

#include "global_data.hpp"
#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QtCharts/QLineSeries>
#include <QtCharts/QAreaSeries>
#include <QComboBox>
#include <QtCharts>
#include <QGroupBox>
#include "utils.hpp"
#include "qcustomplot.h"

class RawStatisticsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RawStatisticsWindow(QWidget *parent = nullptr);
    ~RawStatisticsWindow();
    void initUI();
    void changeEvent(QEvent *event);
    void handleUpdatePointsInfo(unsigned char *praw, unsigned int width, unsigned int height, utils::RAW_BAYER_ID bayer, utils::RAW_BITS bits, QRectF rect, bool is_update_color=false);
    void handleUpdatePointsInfo(const QPixmap *image, QRectF rect, bool is_update_colortype=false);
    void setStatusBarState(QString text);
    void closeWindow();

protected:
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void retranslateUi();
    void closeEvent(QCloseEvent *event);
    QCustomPlot *createChartView();
    QGroupBox *createControlShowGroupBox();
    void calculateMidValue(QMap<int, int> r_map, QMap<int, int> gr_map, QMap<int, int> gb_map, QMap<int, int> b_map, int x_min, int x_max);
    std::tuple<int, int, int> getMaxMinValue(int x_min, int x_max, int y_max);
    void showMidMaxLine(int mean_value_pixel, int max_cnt_pixel, int number);
    int getCurMidValue();
    void updateStatisticsShow(int x_min, int x_max, int y_max);
    int getMeanValue(QMap<int, int> &r_count, QMap<int, int> &gr_count, QMap<int, int> &gb_count, QMap<int, int> &b_count, int x_min, int x_max);
    std::tuple<int, int, int> findMaxPixelCnt(QMap<int, int> &r_count, QMap<int, int> &gr_count, QMap<int, int> &gb_count, QMap<int, int> &b_count, int x_min, int x_max);
    void setPixelChannelShowState(QCPBars *ch_bar, int color_type, int status);
    int getMidValueIndex(QMap<int, int> channel_map, int min_pixel, int max_pixel);

private slots:
    void slotColorShowChangeEvent(int index);

signals:
    void sigChildWindowEvent(int window_type);

private:
    QtCharts::QChart *chart;
    QCustomPlot *customPlot;
    QCPBars *r_bar, *gr_bar, *gb_bar, *b_bar;
    QLabel *raw_info_label;
    QComboBox *color_type;
    QLabel *status_label;
    QLabel *color_label_r;
    QLabel *color_label_b;
    QLabel *color_label_gr;
    QLabel *color_label_gb;
    QLabel *color_block_r;
    QLabel *color_block_b;
    QLabel *color_block_gr;
    QLabel *color_block_gb;

    QList<int> max_list;
    QList<int> min_list;
    QList<int> y_max_list;
    QMap<int, int> r_count_map;
    QMap<int, int> gr_count_map;
    QMap<int, int> gb_count_map;
    QMap<int, int> b_count_map;
    utils::IMAGE_TYPE img_type{utils::IMAGE_TYPE::IMAGE_YUV};

    bool is_image_loaded{false};
    bool is_statistic_ready{true};
    unsigned int max_R = 0, max_Gr = 0, max_Gb = 0, max_B = 0;
    unsigned int min_R = 0, min_Gr = 0, min_Gb = 0, min_B = 0;
    int y_r_max = 0, y_gr_max = 0, y_gb_max = 0, y_b_max = 0;
    int mid_rgb_index = 0;
    int mid_r_index = 0;
    int mid_gr_index = 0;
    int mid_gb_index = 0;
    int mid_b_index = 0;
    int overall_x_min = 0;
    int overall_x_max = 0;
    int overall_y_max = 0;

    int total_r_pixel_cnt = 0;
    int total_gr_pixel_cnt = 0;
    int total_gb_pixel_cnt = 0;
    int total_b_pixel_cnt = 0;
    int crop_width;
    int crop_height;
};

#endif
