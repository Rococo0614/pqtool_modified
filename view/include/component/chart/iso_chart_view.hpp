#ifndef ISO_CHART_VIEW_HPP
#define ISO_CHART_VIEW_HPP

#include <QChartView>
#include <QtCore>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLogValueAxis>
#include "i_observer.hpp"
#include "line_chart_view.hpp"

typedef enum {
    NORMAL = 0,
    ZERO_MAX,
} CURVE_TYPE;

class ISOChartView : public LineChartView
{
    Q_OBJECT

public:
    explicit ISOChartView(int interpolation_num, QWidget *parent = nullptr, CHART_TYPE type = TYPE_NORMAL);
    ~ISOChartView() override;

    void initISO(int x_max, int y_max);
    void initManual(int x_max, int y_max);

    void setData(QVariant data);
    void setPoints(int series_idx, QList<QPointF> points, bool update_global_data = true);
    void setPointByIdx(int series_idx, int index, QPointF point);
    void setCurveType(CURVE_TYPE curve_type);
    QList<QPointF> getPoints(int series_idx);
    QList<QPointF> sampleChartPointsFromList(QList<QVariant> points_x, QList<QVariant> points_y);

    virtual void refreshData(const SecKeyType &reg_section_key) override;

    QList<qreal> getInterpolationDataList(int series_idx = 0);
    QList<qreal> getDataList(int idx = 0);
    QList<QPointF> samplePointFromList(QList<QVariant> points, int interval);
    virtual void selectLineSeries(int series_idx) override;
    void updateChartPoints(QList<QPointF> &points);

signals:
    void sigPointsUpdate(QList<QPointF> ps);
    void sigValueChanged(QVariant);
    void sigPageWriteData();

protected:
    virtual void updateToGlobalData(SecKeyType reg_section_key) override;
    virtual void createSeries(int series_count) override;
    virtual void pointReleased(QtCharts::QLineSeries *series, QPointF point) override;

private:
    void initChart();
    QtCharts::QLineSeries *createSeries(QColor color);
    QtCharts::QScatterSeries *scatter_series;
    virtual void updateSeriesPoint(QtCharts::QLineSeries *series, int index, const QPointF &point) override;
    CURVE_TYPE curve_type{CURVE_TYPE::NORMAL};
};

#endif // ISO_CHART_VIEW_HPP
