#include "iso_chart_view.hpp"
#include <QtCore>
#include <QToolTip>
#include <QtGlobal>
#include <QColor>
#include "global_data.hpp"
#include "utils.hpp"

using namespace QtCharts;

#define X_AXIS_RANGE (4096)
#define Y_AXIS_RANGE (256)

ISOChartView::ISOChartView(int interpolation_num, QWidget *parent, CHART_TYPE type) : LineChartView (interpolation_num, parent, type)
{
    setRenderHint(QPainter::Antialiasing);
    initChart();
}

ISOChartView::~ISOChartView()
{
    for (auto reg_section_key : reg_section_key_list) {
        GlobalData::getInstance()->unSubscribeObserver(reg_section_key, this);
    }
}

void ISOChartView::initChart()
{
    y_chart = new QChart();
    setChart(y_chart);
    y_chart->legend()->hide();
    y_chart->setPlotAreaBackgroundVisible(true);

    scatter_series = new QtCharts::QScatterSeries();
    scatter_series->append(0, 0);
    scatter_series->setBorderColor(Qt::black);
    scatter_series->setBrush(QBrush(Qt::black));
    scatter_series->setMarkerSize(6);
    scatter_series->hide();
}

void ISOChartView::updateSeriesPoint(QtCharts::QLineSeries *series, int index, const QPointF &point)
{
    int new_y = static_cast<int>(point.y());
    new_y = qBound(0, new_y, y_max);
    int new_x = static_cast<int>(point.x());
    new_x = qBound(0, new_x, x_max);

    updateAxis(new_x);
    series->replace(index, new_x, new_y);
}

void ISOChartView::initISO(int x_max, int y_max)
{
    axis_x = new QValueAxis();
    QtCharts::QValueAxis *axis_y = new QValueAxis();

    axis_x->setLabelFormat("%d");
    axis_y->setLabelFormat("%d");
    axis_x->setTickCount(5);
    axis_y->setTickCount(5);
    this->x_max = x_max;
    this->y_max = y_max;
    axis_x->setRange(0, x_max);
    axis_y->setRange(0, y_max);

    createSeries(1);
    setSeriesColor(0, QColor(255, 0, 0));

    this->axis_y = axis_y;
    attachAxis();
    fix_end_point = true;
}

void ISOChartView::initManual(int x_max, int y_max)
{
    axis_x = new QValueAxis();
    QtCharts::QValueAxis *axis_y = new QValueAxis();

    axis_x->setLabelFormat("%d");
    axis_y->setLabelFormat("%d");
    axis_x->setTickCount(5);
    axis_y->setTickCount(5);
    this->x_max = x_max;
    this->y_max = y_max;
    axis_x->setRange(0, x_max);
    axis_y->setRange(0, y_max);

    createSeries(1);
    setSeriesColor(0, QColor(255, 0, 0));

    this->axis_y = axis_y;
    attachAxis();
    fix_end_point = true;
}

void ISOChartView::setData(QVariant data)
{
    QVariantList data_array = data.toList();
    if (data_array.isEmpty()) return;
    if (data_array.size() != interpolation_num*2) return;
    // s1_x, s2_x ... s1_y, s2_y

    if (!data_array.first().toList().isEmpty()) {
        // is 2d array
        int point_counts = this->interpolation_num;
        int series_cnt = series_list.size();
        QList<QList<QPointF>> points_series;
        for (int idx = 0; idx < series_cnt; idx++) {
            points_series.append(QList<QPointF>());
        }

        for (int i = 0, j = point_counts; i < point_counts && j < data_array.size(); i++, j++) {
            QVariantList series_data_list_x = data_array[i].toList();
            QVariantList series_data_list_y = data_array[j].toList();
            QList<QPointF> points = sampleChartPointsFromList(series_data_list_x, series_data_list_y);
            //setPoints(i, points, false);
            for(int idx; idx < series_cnt; idx++) {
                points_series[idx].append(points[idx]);
            }
        }

        for (int idx = 0; idx < series_cnt; idx++) {
            setPoints(idx, points_series[idx], false);
        }
    }
}

void ISOChartView::setPointByIdx(int series_idx, int index, QPointF point)
{
    if (series_list.size() == 0)
    {
        qDebug("error:series list empty!");
        return;
    }

    if (index < 0 || index > interpolation_num || series_idx < 0 || series_idx > series_list.size()) return;

    series_list.at(series_idx)->replace(index+1, point);
}

void ISOChartView::updateChartPoints(QList<QPointF> &points)
{
    setPoints(0, points, false);
}

void ISOChartView::setPoints(int series_idx, QList<QPointF> points, bool update_global_data)
{
    if (series_list.size() == 0)
    {
        qDebug("error:series list empty!");
        return;
    }

    QPointF start, mid, end;
    //Append two endpoints
    if (curve_type == CURVE_TYPE::NORMAL) {
        start.setX(0);
        start.setY(points.first().y());
        end.setX(x_max);
        end.setY(points.last().y());
        points.prepend(start);
        points.append(end);
    } else {
        start.setX(0);
        start.setY(0);
        mid.setX(819);
        mid.setY(819);
        end.setX(8192);
        end.setY(8192);
        points.prepend(start);
        points.insert(5, mid);
        points.append(end);
        scatter_series->replace(0, 819, 819);
        scatter_series->show();
    }

    series_list.at(series_idx)->replace(points);

    int x = 0;
    for(int i=0; i<points.size(); i++) {
        if (x < points[i].x())
            x = static_cast<int>(points[i].x());
    }

    updateAxis(x);
}

QList<QPointF> ISOChartView::getPoints(int series_idx)
{
    QList<QPointF> ps = series_list.at(series_idx)->points();
    ps.removeFirst();
    ps.removeLast();
    return ps;
}

QList<QPointF> ISOChartView::sampleChartPointsFromList(QList<QVariant> points_x, QList<QVariant> points_y)
{
    QList<QPointF> sample_points;
    if (points_x.isEmpty() || points_y.isEmpty()) return sample_points;

    int idx;
    for (idx = 0; idx < points_x.size() && idx < points_y.size(); idx++) {
        sample_points.append(QPointF(points_x[idx].toDouble(), points_y[idx].toDouble()));
    }

    return sample_points;
}

void ISOChartView::refreshData(const SecKeyType &reg_section_key)
{
    int idx = reg_section_key_list.indexOf(reg_section_key);
    SecKeyType reg_section_key_x;
    SecKeyType reg_section_key_y;
    if (idx < interpolation_num) {
        reg_section_key_x = reg_section_key;
        reg_section_key_y = reg_section_key_list[idx + interpolation_num];
    }else {
        reg_section_key_y = reg_section_key;
        reg_section_key_x = reg_section_key_list[idx + interpolation_num];
    }

    QVariant data = GlobalData::getInstance()->getRegValue(reg_section_key_x.first, reg_section_key_x.second, "");
    QList<QVariant> points_x = utils::jsonArrayToList(data.toJsonArray());
    QVariant data2 = GlobalData::getInstance()->getRegValue(reg_section_key_y.first, reg_section_key_y.second, "");
    QList<QVariant> points_y = utils::jsonArrayToList(data.toJsonArray());

    QList<QPointF> points = sampleChartPointsFromList(points_x, points_y);
}

QList<qreal> ISOChartView::getInterpolationDataList(int series_idx)
{
    return QList<qreal>();
}

QList<qreal> ISOChartView::getDataList(int idx)
{
    QList<qreal> values;

    int series_index = idx % interpolation_num;
    for (int i=1; series_list[series_index]->count() - 1; i++) {
        if (idx < interpolation_num)
            values.append(series_list[series_index]->at(i).x());
        else {
            values.append(series_list[series_index]->at(i).y());
        }
    }

    return values;
}

QList<QPointF> ISOChartView::samplePointFromList(QList<QVariant> points, int interval)
{
    return QList<QPointF>();
}

void ISOChartView::selectLineSeries(int series_idx)
{

}

void ISOChartView::updateToGlobalData(SecKeyType reg_section_key)
{
}

void ISOChartView::createSeries(int series_count)
{
    for (int i = 0; i < series_count; i++) {
        QtCharts::QLineSeries *s = createSeries(QColor(0, 0, 0));
        y_chart->addSeries(s);

        connect(s, &QLineSeries::hovered,
                this, std::bind(&ISOChartView::pointHovered, this, s, std::placeholders::_1, std::placeholders::_2));
        connect(s, &QLineSeries::pressed,
                this, std::bind(&ISOChartView::pointPressed, this, s, std::placeholders::_1));
        connect(s, &QLineSeries::released,
                this, std::bind(&ISOChartView::pointReleased, this, s, std::placeholders::_1));

        series_list.append(s);
    }

    y_chart->addSeries(scatter_series);
}

QtCharts::QLineSeries *ISOChartView::createSeries(QColor color)
{
    QLineSeries *series = new QLineSeries();
    QPen pen = series->pen();
    pen.setColor(color);
    pen.setWidth(2);

    series->setPen(pen);
    series->setPointsVisible(true);

    return series;
}

void ISOChartView::pointReleased(QtCharts::QLineSeries *series, QPointF point)
{
    LineChartView::pointReleased(series, point);
    QList<QPointF> ps = series->points();
    double new_start_y = ps[1].y();
    double new_end_y = ps[ps.size() - 2].y();
    series->replace(0, series->at(0).x(), new_start_y);
    series->replace(ps.size() - 1, series->at(ps.size() - 1).x(), new_end_y);

    emit sigPointsUpdate(series->points());

    emit sigValueChanged(QVariant());
    // if auto write, trigger write data
    if (GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_AUTO_WRITE).toInt()) {
        emit sigPageWriteData();
    }
}

void ISOChartView::setCurveType(CURVE_TYPE curve_type)
{
    this->curve_type = curve_type;
}
