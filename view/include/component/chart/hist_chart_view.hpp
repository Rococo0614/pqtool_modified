#ifndef HIST_CHART_VIEW_HPP
#define HIST_CHART_VIEW_HPP

#include <QChartView>
#include <QtCore>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLogValueAxis>
#include <QtCharts/QAreaSeries>
#include "i_observer.hpp"

class HistChartView : public QtCharts::QChartView, public IObserver
{
    Q_OBJECT

public:
    explicit HistChartView(QWidget *parent = nullptr);
    ~HistChartView() override;

    void initHist();
    void initWdrHist();
    void initGlobalToneCurve(double x_max, double y_max);
    void initDarkToneCurve(double x_max, double y_max);
    void initBrightToneCurve(double x_max, double y_max);
    void initAEHist();

    void setData(QVariant data);
    void hideSeries();
    void showSeries();
    void setPoints(int series_idx, QList<QPointF> points, bool update_global_data = true);
    QList<QPointF> getPoints(int series_idx);
    void setTitle(QString title);
    QList<QPointF> samplePointFromList(QList<QVariant> points);
    void storePointsFromList(QList<QVariant> points);

    QList<qreal> getData(int series_index);
    void addRegSectionKey(QString section, QString key) override;
    SecKeyType getSectionKey();

    virtual void refreshData(const SecKeyType &reg_section_key) override;
    int getSeriesCount();
    void setDisplayRange(double min, double max);
    void setFixedAxisX(bool en);
    void setFixedAxisY(bool en);
    void setDispWindowSize(int n);
    void handleUpdateTheme(bool dark_theme_enabled);

protected:
    virtual void createSeries(int series_count);
    void createAreaSerie();
    void attachAxis();
    void setSeriesColor(int series_idx, QColor color);
    bool compare(qreal v1, qreal v2);
    void updateAxis(double new_x, double new_y);

private:
    void initChart();
    QtCharts::QLineSeries *createSeries(QColor color);
    virtual void updateSeriesPoint(QtCharts::QLineSeries *series, int index, const QPointF &point);

protected:
    QList<QtCharts::QLineSeries *> series_list;
    QtCharts::QAreaSeries *area_serie = NULL;
    QtCharts::QValueAxis *axis_x;
    QtCharts::QAbstractAxis *axis_y;
    QtCharts::QChart *y_chart;
    double x_max{4096};
    double y_max{256};

    QList<QPointF> stored_points;
    bool fixed_axis_x{false};
    bool fixed_axis_y{false};
    int disp_window {512};
};

#endif // HIST_CHART_VIEW_HPP
