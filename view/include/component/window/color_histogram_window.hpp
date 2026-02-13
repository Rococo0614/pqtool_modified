#ifndef COLOR_HISTOGRAM_WINDOW_H
#define COLOR_HISTOGRAM_WINDOW_H

#include "global_data.hpp"
#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QtCharts/QLineSeries>
#include <QtCharts/QAreaSeries>
#include <QComboBox>

const int COLOR_HIST_WINDOW_WIDTH = 640;
const int COLOR_HIST_WINDOW_HEIGHT = 480;

class ColorHistogramWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ColorHistogramWindow(QWidget *parent = nullptr);
    ~ColorHistogramWindow();
    void initUI();
    void changeEvent(QEvent *event);
    void setImage(QPixmap *image);

protected:
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void retranslateUi();

public slots:
    void handleUpdatePoints(const QPixmap *image);

private slots:
    void handleSelectColor(const QString &text);

private:
    QWidget *main_widget;
    QGridLayout *main_layout;
    QList<QString> color_list;
    QComboBox *color_combobox;
    QPixmap *image;
    QtCharts::QChart *chart;
    QtCharts::QLineSeries *r_series;
    QtCharts::QLineSeries *g_series;
    QtCharts::QLineSeries *b_series;
    QtCharts::QAreaSeries *r_area_series;
    QtCharts::QAreaSeries *g_area_series;
    QtCharts::QAreaSeries *b_area_series;
    QMap<int, int> r_count_map;
    QMap<int, int> g_count_map;
    QMap<int, int> b_count_map;
};

#endif // COLOR_HISTOGRAM_WINDOW_H
