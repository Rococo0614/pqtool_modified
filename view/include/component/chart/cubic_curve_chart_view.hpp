#ifndef CUBIC_CURVE_CHART_VIEW_HPP
#define CUBIC_CURVE_CHART_VIEW_HPP

#include <QChartView>
#include <QtCore>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLogValueAxis>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include "i_observer.hpp"

#include "line_chart_view.hpp"

class CubicCurveChartView : public QtCharts::QChartView, public IObserver
{
    Q_OBJECT

public:
    explicit CubicCurveChartView(QWidget *parent = nullptr);
    ~CubicCurveChartView() override;

    void initGamma(int x_maximum, int y_maximum);
    void initToneCurve();
    void initMapCurve(int x_maximum, int y_maximum);

    void setData(QVariant data);
    void setPoints(QList<QPointF> points, bool update_global_data = true);
    QList<QPointF> getPoints(int series_idx);
    void setTitle(QString title);
    QList<QPointF> samplePointFromList(QList<QVariant> points);

    QList<qreal> getData(int series_index);
    void addRegSectionKey(QString section, QString key) override;
    SecKeyType getSectionKey();

    virtual void refreshData(const SecKeyType &reg_section_key) override;

    QString getInterpolationDataString(int series_idx = 0, QString delimeter = ",");
    QList<qreal> getInterpolationDataList(int series_idx = 0);
    void setControlPointsNum(int num);
    int getControlPointsNum();
    void addControlPt(double x, double y);
    void setUserDefineMode(bool m);
    double getY(double x);
    void setUserDefineCtrlPt(bool en);
    void setExpRatio(float e);
    QVariantList getGlobalTone();
    QVariantList getDarkTone();
    QVariantList getBritTone();
    void updateToneCurve();
    void handleUpdateTheme(bool dark_theme_enabled);
    void addLine(QtCharts::QLineSeries *series_add);
    void setRange(int max_x, int max_y);

signals:
    void sigFocusPointUpdated(qreal x, qreal y);
    void sigMouseMove(double x, double y);
    void sigPressedScatterPt(QPointF p);
    void sigUpdateWdrHistogram(QVariant data);
    void sigValueChanged(QVariant);
    void sigPageWriteData();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

    void createSeries();
    void attachAxis();
    void setSeriesColor(QColor color);
    bool compare(qreal v1, qreal v2);
    void updateControlPoints(int num);
    void resetControlPoints();

private:
    void initChart();
    QtCharts::QLineSeries *createLineSeries(QColor color);
    QtCharts::QScatterSeries *createScatterSeries(QColor color);

    void pointHovered(QtCharts::QLineSeries *series, const QPointF &point, bool state);
    void scatterPointHovered(const QPointF &point, bool state);
    //void scatterPointHovered(QtCharts::QScatterSeries *series, const QPointF &point, bool state);
    void scatterPointPressed(QtCharts::QScatterSeries *series, QPointF point);
    void scatterPointReleased(QtCharts::QScatterSeries *series, QPointF point);
    int getCloestPointIndex(QtCharts::QXYSeries *series, const QPointF &point);
    void updateToGlobalData(SecKeyType reg_section_key);
    virtual QString getTooltipText(const QPointF &point);
    virtual void updateSeriesPoint(QtCharts::QXYSeries *series, int index, const QPointF &point, bool update_to_global = true);
    QList<QPointF> genSmoothCurve(QList<qreal> &x, QList<qreal> &y);
    QList<QPointF> getPCHIPCurve(QList<qreal> &x, QList<qreal> &y);
    void genToneCurve(QList<qreal> &x, QList<qreal> &y);

protected:
    bool fix_end_point;
    QtCharts::QValueAxis *axis_x;
    QtCharts::QAbstractAxis *axis_y;
    QtCharts::QChart *y_chart;
    QtCharts::QLineSeries *line_serie;
    QtCharts::QScatterSeries *scatter_serie;
    QList<QPointF> control_ps;
    QList<qreal> node_x;
    QList<qreal> node_y;
    QList<qreal> tc_node_x;
    QList<qreal> tc_node_y;

    bool force_correct{true};
    bool bHoldCurve{false};
    int holdIndex{-1};
    int control_num{2};
    double m_x;
    double m_y;
    bool user_define_mode {false};
    double x_max;
    double y_max;
    bool user_define_control_pt {false};

    bool isUserDefineCurve {false};
    float exp_ratio {4};
    QVariantList global_tone_mw;
    QVariantList dark_tone_mw;
    QVariantList brit_tone_mw;
};

#endif //CUBIC_CURVE_CHART_VIEW_HPP
