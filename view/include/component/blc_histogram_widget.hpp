#ifndef blc_HISTOGRAM_WIDGET_H
#define blc_HISTOGRAM_WIDGET_H

#include "global_data.hpp"
#include "otp_pdaf_cali.h"
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QtCharts/QLineSeries>
#include <QtCharts/QAreaSeries>
#include <QComboBox>

class BlcHistogramWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit BlcHistogramWidget(QWidget *parent = nullptr);
        void initUI();

    public slots:
        void handleUpdatePoints(int *rr_hist, int *gr_hist, int *gb_hist, int *bb_hist, int r_offset_mean, int gr_offset_mean,
        int gb_offset_mean, int b_offset_mean, int r_offset_median, int gr_offset_median, int gb_offset_median, int b_offset_median);

    private slots:
        void handleSelectColor(const QString &text);

    private:
        QHBoxLayout *main_layout;
        QList<QString> color_list;
        QComboBox *color_combobox;
        QtCharts::QChart *chart;
        QtCharts::QLineSeries *r_offset_series_mean;
        QtCharts::QAreaSeries *r_offset_area_series_mean;
        QtCharts::QLineSeries *gr_offset_series_mean;
        QtCharts::QAreaSeries *gr_offset_area_series_mean;
        QtCharts::QLineSeries *gb_offset_series_mean;
        QtCharts::QAreaSeries *gb_offset_area_series_mean;
        QtCharts::QLineSeries *b_offset_series_mean;
        QtCharts::QAreaSeries *b_offset_area_series_mean;
        QtCharts::QLineSeries *r_offset_series_median;
        QtCharts::QAreaSeries *r_offset_area_series_median;
        QtCharts::QLineSeries *gr_offset_series_median;
        QtCharts::QAreaSeries *gr_offset_area_series_median;
        QtCharts::QLineSeries *gb_offset_series_median;
        QtCharts::QAreaSeries *gb_offset_area_series_median;
        QtCharts::QLineSeries *b_offset_series_median;
        QtCharts::QAreaSeries *b_offset_area_series_median;
        QtCharts::QLineSeries *r_series;
        QtCharts::QLineSeries *gr_series;
        QtCharts::QLineSeries *gb_series;
        QtCharts::QLineSeries *b_series;
        QtCharts::QAreaSeries *r_area_series;
        QtCharts::QAreaSeries *gr_area_series;
        QtCharts::QAreaSeries *gb_area_series;
        QtCharts::QAreaSeries *b_area_series;
};

#endif // blc_HISTOGRAM_WIDGET_H
