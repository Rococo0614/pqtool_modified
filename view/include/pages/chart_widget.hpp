#ifndef CHART_WIDGET_H
#define CHART_WIDGET_H

#include "global_data.hpp"
#include "otp_pdaf_cali.h"
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QtCharts/QLineSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>
#include <QComboBox>

typedef struct {
    QString title;
    QString title_x;
    QString title_y;
    float rang_x[2];
    float rang_y[2];
} CHAR_PROPERTY;

class ChartWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit ChartWidget(CHAR_PROPERTY property, QWidget *parent = nullptr);
        void initUI();
        void setProperty(CHAR_PROPERTY property);
        void setSeriesVisible(int idx, bool state);
        void handleAddScatterPoint(float x, float y);
        void handleDelScatterPoint(int idx);
        void handleUpdateLinePoints(int num, float *x, float *y);
        void clearSeries(int idx);
        void resetAxisRang(float xmin, float xmax, float ymin, float ymax);

    private:
        QHBoxLayout *main_layout;
        QtCharts::QChart *chart;
        QtCharts::QValueAxis *axis_x;
        QtCharts::QValueAxis *axis_y;
        QtCharts::QScatterSeries *scatter_series;
        QtCharts::QLineSeries *line_series;
        QString title;
        QString title_x;
        QString title_y;
        float rang_x[2];
        float rang_y[2];
};

#endif
