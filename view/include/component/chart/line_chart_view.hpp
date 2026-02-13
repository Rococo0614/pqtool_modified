#ifndef LINE_CHART_VIEW_HPP
#define LINE_CHART_VIEW_HPP

#include <QChartView>
#include <QtCore>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLogValueAxis>
#include "i_observer.hpp"

typedef enum {
    TYPE_NORMAL = 0,
    TYPE_HDR,
} CHART_TYPE;

class LineChartView : public QtCharts::QChartView, public IObserver
{
    Q_OBJECT

public:
    explicit LineChartView(int interpolation_num, QWidget *parent = nullptr, CHART_TYPE type = TYPE_NORMAL);
    ~LineChartView() override;

    void initGamma();
    void initHByH();
    void initSByS();
    void initHDR();
    void initAWB();

    void setData(QVariant data);
    void setPoints(int series_idx, QList<QPointF> points, bool update_global_data = true);
    QList<QPointF> getPoints(int series_idx);
    void setTitle(QString title);
    QList<QPointF> samplePointFromList(QList<QVariant> points, int interval);

    QList<qreal> getData(int series_index);
    void addRegSectionKey(QString section, QString key) override;
    SecKeyType getSectionKey();

    virtual void refreshData(const SecKeyType &reg_section_key) override;

    QString getInterpolationDataString(int series_idx = 0, QString delimeter = ",");
    QList<qreal> getInterpolationDataList(int series_idx = 0);
    virtual void selectLineSeries(int series_idx);
    int getSeriesCount();
    void setInterpolationNum(int interpolation_num);
    int getInterpolationNum();
    void handleUpdateTheme(bool dark_theme_enabled);

signals:
    void sigFocusPointUpdated(qreal x, qreal y);
    void sigSelectCurve(int idx);
    void sigMouseMove(double x, double y);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

    void pointHovered(QtCharts::QLineSeries *series, const QPointF &point, bool state);
    void pointPressed(QtCharts::QLineSeries *series, const QPointF &point);
    virtual void pointReleased(QtCharts::QLineSeries *series, QPointF point);

    virtual void createSeries(int series_count);

    void attachAxis();
    void setSeriesColor(int series_idx, QColor color);
    bool compare(qreal v1, qreal v2);
    virtual void updateToGlobalData(SecKeyType reg_section_key);
    void updateAxis(int new_point_x);

private:
    void initChart();
    QtCharts::QLineSeries *createSeries(QColor color);
    int getCloestPointIndex(QtCharts::QLineSeries *series, const QPointF &point);

    virtual QString getTooltipText(const QPointF &point);
    virtual void updateSeriesPoint(QtCharts::QLineSeries *series, int index, const QPointF &point);

protected:
    bool fix_end_point;
    double multifier;
    int interpolation_num;
    QList<QtCharts::QLineSeries *> series_list;
    QtCharts::QValueAxis *axis_x;
    QtCharts::QAbstractAxis *axis_y;
    QtCharts::QChart *y_chart;
    int focus_point_idx;
    QtCharts::QLineSeries *focus_series;
    CHART_TYPE type;
    double m_x;
    double m_y;
    int x_max{4096};
    int y_max{256};
};

#endif // LINE_CHART_VIEW_HPP
