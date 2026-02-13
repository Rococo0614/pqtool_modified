#include "line_chart_view.hpp"
#include <QtCore>
#include <QToolTip>
#include <QtGlobal>
#include "global_data.hpp"
#include "utils.hpp"

using namespace QtCharts;

LineChartView::LineChartView(int interpolation_num, QWidget *parent, CHART_TYPE type) : QChartView (parent),
    fix_end_point(false),
    multifier(1.0),
    interpolation_num(interpolation_num),
    type(type)
{
    setRenderHint(QPainter::Antialiasing);
    initChart();
}

LineChartView::~LineChartView()
{
    for (auto reg_section_key : reg_section_key_list) {
        GlobalData::getInstance()->unSubscribeObserver(reg_section_key, this);
    }
}

void LineChartView::createSeries(int series_count)
{
    for (int i = 0; i < series_count; i++) {
        QtCharts::QLineSeries *s = createSeries(QColor(0, 0, 0));
        y_chart->addSeries(s);

        connect(s, &QLineSeries::hovered,
                this, std::bind(&LineChartView::pointHovered, this, s, std::placeholders::_1, std::placeholders::_2));
        connect(s, &QLineSeries::pressed,
                this, std::bind(&LineChartView::pointPressed, this, s, std::placeholders::_1));
        connect(s, &QLineSeries::released,
                this, std::bind(&LineChartView::pointReleased, this, s, std::placeholders::_1));

        series_list.append(s);
    }
}

void LineChartView::initChart()
{
    y_chart = new QChart();
    setChart(y_chart);
    y_chart->legend()->hide();
    y_chart->setPlotAreaBackgroundVisible(true);
}

void LineChartView::pointPressed(QtCharts::QLineSeries *series, const QPointF &point)
{
    int idx = getCloestPointIndex(series, point);

    focus_series = series;
    focus_point_idx = idx;

    selectLineSeries(series_list.indexOf(focus_series));

    emit sigFocusPointUpdated(focus_series->at(focus_point_idx).x(), focus_series->at(focus_point_idx).y());
}

void LineChartView::updateAxis(int new_point_x)
{
    int x_range = new_point_x*2;
    if (x_range < x_max) {
        axis_x->setRange(0, x_range);
    } else {
        axis_x->setRange(0, x_max);
    }
}

void LineChartView::pointReleased(QtCharts::QLineSeries *series, QPointF point)
{
    QPointF new_position = mapFromGlobal(QCursor::pos());
    QPointF new_point = series->chart()->mapToValue(new_position);

    int idx = getCloestPointIndex(series, point);

    if (idx == -1) {
        return;
    }

    if (fix_end_point) {
        if (idx == 0 || idx == series->points().count() - 1) {
            return;
        }
    }

    updateSeriesPoint(series, idx, new_point);

    focus_series = series;
    focus_point_idx = idx;

    emit sigFocusPointUpdated(focus_series->at(focus_point_idx).x(), focus_series->at(focus_point_idx).y());
}

void LineChartView::updateSeriesPoint(QtCharts::QLineSeries *series, int index, const QPointF &point)
{
    int new_y = static_cast<int>(point.y());
    series->replace(index, series->at(index).x(), new_y);
}

bool LineChartView::compare(qreal v1, qreal v2)
{
    return qFuzzyCompare(v1, v2);
}

void LineChartView::initGamma()
{
    axis_x = new QValueAxis();
    QtCharts::QValueAxis *axis_y = new QValueAxis();

    axis_x->setLabelFormat("%d");
    axis_y->setLabelFormat("%d");
    axis_x->setTickCount(5);
    axis_y->setTickCount(9);
    axis_x->setRange(0, 256);
    axis_y->setRange(0, 4095);

    createSeries(1);
    setSeriesColor(0, QColor(255, 0, 0));

    this->axis_y = axis_y;
    attachAxis();
    fix_end_point = true;
}

void LineChartView::initHByH()
{
    axis_x = new QValueAxis();
    QtCharts::QValueAxis *axis_y = new QValueAxis();

    axis_x->setLabelFormat("%d");
    axis_y->setLabelFormat("%d");
    axis_x->setTickCount(5);
    axis_y->setTickCount(9);
    axis_x->setRange(0, 769);
    axis_y->setRange(0, 4095);

    createSeries(1);
    this->axis_y = axis_y;
    attachAxis();
}

void LineChartView::initSByS()
{
    axis_x = new QValueAxis();
    QtCharts::QValueAxis *axis_y = new QValueAxis();

    axis_x->setLabelFormat("%d");
    axis_y->setLabelFormat("%d");
    axis_x->setTickCount(5);
    axis_y->setTickCount(9);
    axis_x->setRange(0, 513);
    axis_y->setRange(0, 4095);

    createSeries(1);
    this->axis_y = axis_y;
    attachAxis();
}

void LineChartView::initHDR()
{
    axis_x = new QValueAxis();
    QtCharts::QValueAxis *axis_y = new QValueAxis();

    axis_x->setLabelFormat("%d");
    axis_y->setLabelFormat("%d");
    axis_x->setTickCount(5);
    axis_y->setTickCount(9);
    axis_x->setRange(0, 256);
    axis_y->setRange(0, 4095);

    createSeries(3);
    setSeriesColor(0, QColor(255, 0, 0));
    setSeriesColor(1, QColor(0, 255, 0));
    setSeriesColor(2, QColor(0, 0, 255));

    this->axis_y = axis_y;
    attachAxis();
    fix_end_point = true;

    if (type == TYPE_HDR) {
        // select first line by default
        selectLineSeries(0);
    }
}

void LineChartView::initAWB()
{
    axis_x = new QValueAxis();
    QtCharts::QValueAxis *axis_y = new QValueAxis();

    axis_x->setLabelFormat("%d");
    axis_y->setLabelFormat("%d");
    axis_x->setTickCount(5);
    axis_y->setTickCount(9);
    axis_x->setRange(0, 15000);
    axis_y->setRange(0, 15000);

    createSeries(3);
    setSeriesColor(0, QColor(255, 0, 0));

    this->axis_y = axis_y;
    attachAxis();
    fix_end_point = true;
}

void LineChartView::setData(QVariant data)
{
    QVariantList data_array = data.toList();
    if (data_array.isEmpty()) return;

    if (!data_array.first().toList().isEmpty()) {
        // is 2d array
        for (int i = 0; (i < data_array.size()) && (i < this->series_list.size()); i++) {
            QVariantList series_data_list = data_array[i].toList();
            QList<QPointF> points = samplePointFromList(series_data_list, interpolation_num);
            setPoints(i, points, false);
        }
    } else {
        // is 1d array
        QList<QPointF> points = samplePointFromList(data_array, interpolation_num);
        setPoints(0, points, false);
    }
}

void LineChartView::setPoints(int series_idx, QList<QPointF> points, bool update_global_data)
{
    if (series_list.size() == 0)
    {
        qDebug("error:series list empty!");
        return;
    }

    series_list.at(series_idx)->replace(points);

    if (update_global_data) {
        SecKeyType reg_section_key = reg_section_key_list[series_idx];
        if (reg_section_key.first.length() > 0 && reg_section_key.second.length() > 0) {
            updateToGlobalData(reg_section_key);
        }
    }
}

QList<QPointF> LineChartView::getPoints(int series_idx)
{
    return series_list.at(series_idx)->points();
}

QList<qreal> LineChartView::getData(int series_index)
{
    QList<qreal> values;

    for (int i = 0; i < series_list[series_index]->count(); i++) {
        qreal value = series_list[series_index]->at(i).y();

        values.append(value);
    }

    return values;
}

void LineChartView::addRegSectionKey(QString section, QString key)
{
    if (section.isEmpty() || key.isEmpty()) {
        return;
    }

    SecKeyType section_key = {section, key};
    reg_section_key_list.append(section_key);

    GlobalData::getInstance()->subscribeObserver(section_key, this);

    connect(GlobalData::getInstance().get(), &GlobalData::sigDataLoaded,
                     this, [=]() {
        refreshData(section_key);
    });

    connect(this, &LineChartView::sigFocusPointUpdated,
            [=](qreal, qreal) {
        updateToGlobalData(section_key);
    });
}

void LineChartView::updateToGlobalData(SecKeyType reg_section_key)
{
    for (int i = 0; i < reg_section_key_list.size(); i++) {
        if (reg_section_key != reg_section_key_list[i]) {
            continue;
        }

        QList<qreal> all_points = getInterpolationDataList(i);
        QJsonArray value_array;

        for (int j = 0; j < all_points.size(); j++) {
            value_array.append(all_points[j]);
        }

        if (value_array.size() == 0) {
            continue;
        }

        GlobalData::getInstance()->setRegValue(reg_section_key.first, reg_section_key.second, value_array);
    }
}

SecKeyType LineChartView::getSectionKey()
{
    return reg_section_key_list[0];
}

void LineChartView::setTitle(QString title)
{
    y_chart->setTitle(title);
}

void LineChartView::mouseMoveEvent(QMouseEvent *event)
{
    QChartView::mouseMoveEvent(event);
    m_x = y_chart->mapToValue(event->pos()).x();
    m_y = y_chart->mapToValue(event->pos()).y();
    emit sigMouseMove(m_x, m_y);
}

void LineChartView::mouseReleaseEvent(QMouseEvent *event)
{
    focus_series = nullptr;
    focus_point_idx = -1;

    QChartView::mouseReleaseEvent(event);
}

void LineChartView::keyPressEvent(QKeyEvent *event)
{
    QChartView::keyPressEvent(event);

    int key = event->key();

    if (focus_series) {
        if (key == Qt::Key_Up || key == Qt::Key_Down) {
            int offset = (key == Qt::Key_Up) ? 10 : -10;
            focus_series->replace(focus_point_idx, focus_series->at(focus_point_idx).x(), focus_series->at(focus_point_idx).y() + offset);
        } else if (key == Qt::Key_Left || key == Qt::Key_Right) {
            int offset = (key == Qt::Key_Left) ? -1 : 1;
            focus_point_idx += offset;
        }

        if (fix_end_point) {
            focus_point_idx = qBound(1, focus_point_idx, focus_series->points().count() - 2);
        } else {
            focus_point_idx = qBound(0, focus_point_idx, focus_series->points().count() - 1);
        }

        emit sigFocusPointUpdated(focus_series->at(focus_point_idx).x(), focus_series->at(focus_point_idx).y());
    }
}

void LineChartView::focusOutEvent(QFocusEvent *event)
{
    QChartView::focusOutEvent(event);

    focus_series = nullptr;
    focus_point_idx = -1;

    if (type == TYPE_NORMAL) {
        // clear line selected
        selectLineSeries(-1);
    }
}

void LineChartView::setSeriesColor(int series_idx, QColor color)
{
    QPen pen = series_list.at(series_idx)->pen();
    pen.setColor(color);
    series_list.at(series_idx)->setPen(pen);
}

int LineChartView::getCloestPointIndex(QtCharts::QLineSeries *series, const QPointF &point)
{
    QPointF point_position = series->chart()->mapToPosition(point);
    QList<QPointF> points = series->points();

    for (int i = 0; i < points.count(); i++) {
        QPointF p = points[i];
        QPointF position = series->chart()->mapToPosition(p);

        double diff_x = abs(point_position.x() - position.x());
        double diff_y = abs(point_position.y() - position.y());

        if (diff_x <= 8 && diff_y <= 8) {
            return i;
        }
    }

    return -1;
}

QString LineChartView::getTooltipText(const QPointF &point)
{
    QString text;
    text.sprintf("%d, %d", static_cast<int>(point.x()), static_cast<int>(point.y()));
    return text;
}

void LineChartView::attachAxis()
{
    y_chart->addAxis(axis_x, Qt::AlignBottom);
    y_chart->addAxis(axis_y, Qt::AlignLeft);
    for (int i = 0; i < y_chart->series().count(); i++) {
        y_chart->series()[i]->attachAxis(axis_x);
        y_chart->series()[i]->attachAxis(axis_y);
    }
}

void LineChartView::pointHovered(QLineSeries *series, const QPointF &point, bool state)
{
    if (state == false) {
        QToolTip::hideText();
        return;
    }

    int idx = getCloestPointIndex(series, point);

    if (idx != -1) {
        QPointF p = series->at(idx);
        QString text = getTooltipText(p);
        QToolTip::showText(QCursor::pos(), text);
    }
}

QtCharts::QLineSeries *LineChartView::createSeries(QColor color)
{
    QLineSeries *series = new QLineSeries();
    QPen pen = series->pen();
    pen.setColor(color);
    pen.setWidth(2);

    series->setPen(pen);
    series->setPointsVisible(true);

    return series;
}

QList<QPointF> LineChartView::samplePointFromList(QList<QVariant> points, int interval)
{
    QList<QPointF> sample_points;
    if (points.isEmpty()) return sample_points;
    int idx;
    for (idx = 0; idx < points.size(); idx += interval) {
        sample_points.append(QPointF(idx, points[idx].toDouble() / multifier));
    }
    // add last point of curve
    if (idx >= points.size()) {
        idx = points.size() - 1;
        sample_points.append(QPointF(idx, points[idx].toDouble() / multifier));
    }

    return sample_points;
}

void LineChartView::refreshData(const SecKeyType &reg_section_key)
{
    for (int i = 0; i < reg_section_key_list.size(); i++) {
        if (reg_section_key_list[i] != reg_section_key) {
            continue;
        }
        QVariant data = GlobalData::getInstance()->getRegValue(reg_section_key.first, reg_section_key.second, "");
        QList<QVariant> all_points = utils::jsonArrayToList(data.toJsonArray());

        if (all_points.size() == 0) {
            continue;
        }

        QList<QPointF> points = samplePointFromList(all_points, interpolation_num);
        setPoints(i, points, false);
    }
}

QString LineChartView::getInterpolationDataString(int series_idx, QString delimeter)
{
    QString data_str = utils::listToString(getInterpolationDataList(series_idx), std::move(delimeter));
    return data_str;
}

QList<qreal> LineChartView::getInterpolationDataList(int series_idx)
{
    QList<qreal> values = getData(series_idx);
    if (values.isEmpty()) return values;
    values = utils::interpolation(values, interpolation_num);

    for (int i = 0; i < values.count(); i++) {
        values[i] = int(values[i] * multifier);
    }

    return values;
}

void LineChartView::selectLineSeries(int series_idx)
{
    for (int i = 0; i < series_list.size(); i++) {
        if (i == series_idx) {
            this->setSeriesColor(i, QColor(255, 0, 0));
        } else {
            if (type == TYPE_HDR) {
                this->setSeriesColor(i, QColor(80, 80, 80, 80));
            } else {
                this->setSeriesColor(i, QColor(0, 0, 0));
            }
        }
    }

    if (type == TYPE_HDR) {
         emit sigSelectCurve(series_idx);
    }
}

int LineChartView::getSeriesCount()
{
    return series_list.size();
}

void LineChartView::setInterpolationNum(int interpolation_num)
{
    this->interpolation_num = interpolation_num;
}

int LineChartView::getInterpolationNum()
{
    return this->interpolation_num;
}

void LineChartView::handleUpdateTheme(bool dark_theme_enabled)
{
    if (dark_theme_enabled) {
        y_chart->setBackgroundBrush(QColor(25, 35, 45));
        axis_x->setLabelsColor(QColor(224, 225, 226));
        axis_y->setLabelsColor(QColor(224, 225, 226));
    } else {
        y_chart->setBackgroundBrush(QColor(255, 255, 255));
        axis_x->setLabelsColor(QColor(0, 0, 0));
        axis_y->setLabelsColor(QColor(0, 0, 0));
    }
}
