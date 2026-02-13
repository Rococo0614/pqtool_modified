#include "hist_chart_view.hpp"
#include <QtCore>
#include <QToolTip>
#include <QtGlobal>
#include "global_data.hpp"
#include "utils.hpp"

using namespace QtCharts;

HistChartView::HistChartView(QWidget *parent) :
    QChartView (parent)
{
    setRenderHint(QPainter::Antialiasing);
    initChart();
}

HistChartView::~HistChartView()
{
    for (auto reg_section_key : reg_section_key_list) {
        GlobalData::getInstance()->unSubscribeObserver(reg_section_key, this);
    }
}

void HistChartView::createSeries(int series_count)
{
    QtCharts::QLineSeries *s = createSeries(QColor(0, 0, 0));
    s->setUseOpenGL(true);
    y_chart->addSeries(s);
    series_list.append(s);
}

void HistChartView::createAreaSerie()
{
    QtCharts::QLineSeries *s = createSeries(QColor(0, 0, 0));
    s->setUseOpenGL(true);
    area_serie = new QtCharts::QAreaSeries(s);
    y_chart->addSeries(area_serie);
    series_list.append(s);
}

void HistChartView::initChart()
{
    y_chart = new QChart();
    setChart(y_chart);
    y_chart->legend()->hide();
    y_chart->setPlotAreaBackgroundVisible(true);
}

void HistChartView::setDisplayRange(double min, double max)
{
    QList<QPointF> display_points;
    int total_num = stored_points.size();
    for (int i = 0; i < total_num; i++)
    {
        if (stored_points[i].x() >= min && stored_points[i].x() <= max)
        {
            display_points.append(stored_points[i]);
        }
        else if (stored_points[i].x() > max)
        {
            break;
        }
    }

    //if (!fixed_axis)
    {
        axis_x->setRange(min, max);
    }

    setPoints(0, display_points, false);
}

void HistChartView::setFixedAxisX(bool en)
{
    fixed_axis_x = en;
}

void HistChartView::setFixedAxisY(bool en)
{
    fixed_axis_y = en;
}

void HistChartView::updateAxis(double new_x, double new_y)
{
//    double x_range = new_x;
//    if (new_x > x_max || x_range < x_max) {
//        x_max = x_range;
//        axis_x->setRange(0, x_max);
//    }

    if(fixed_axis_y == false) {
        double y_range = new_y*1.5;
        if (new_y > y_max || y_range < y_max) {
            y_max = y_range;
            axis_y->setRange(0, y_max);
        }
    }
    
}

void HistChartView::updateSeriesPoint(QtCharts::QLineSeries *series, int index, const QPointF &point)
{
    int new_y = static_cast<int>(point.y());
    series->replace(index, series->at(index).x(), new_y);
}

bool HistChartView::compare(qreal v1, qreal v2)
{
    return qFuzzyCompare(v1, v2);
}

void HistChartView::initHist()
{
    axis_x = new QValueAxis();
    QtCharts::QValueAxis *axis_y = new QValueAxis();

    axis_x->setLabelFormat("%d");
    axis_y->setLabelFormat("%d");
    axis_x->setTickCount(5);
    axis_y->setTickCount(9);
    x_max = 255;
    axis_x->setRange(0, x_max);
    y_max = 1920 * 1080;
    axis_y->setRange(0, y_max);

    createAreaSerie();
    area_serie->setPen(QColor(0, 0, 255));
    area_serie->setBrush(QColor(0, 0, 255, 200));

    this->axis_y = axis_y;
    attachAxis();
}

void HistChartView::initWdrHist()
{
    axis_x = new QValueAxis();
    QtCharts::QValueAxis *axis_y = new QValueAxis();

    axis_x->setLabelFormat("%d");
    axis_y->setLabelFormat("%d");
    axis_x->setTickCount(5);
    axis_y->setTickCount(9);
    x_max = 65536;
    axis_x->setRange(0, x_max);
    y_max = 65536;
    axis_y->setRange(0, y_max);

    createAreaSerie();
    area_serie->setPen(QColor(0, 255, 0));
    area_serie->setBrush(QColor(0, 255, 0, 50));

    this->axis_y = axis_y;
    attachAxis();
}

void HistChartView::initGlobalToneCurve(double x_max, double y_max)
{
    axis_x = new QValueAxis();
    QtCharts::QValueAxis *axis_y = new QValueAxis();

    axis_x->setLabelFormat("%d");
    axis_y->setLabelFormat("%d");
    axis_x->setTickCount(5);
    axis_y->setTickCount(9);
    this->x_max = x_max;
    axis_x->setRange(0, x_max);
    this->y_max = y_max;
    axis_y->setRange(0, y_max);

    createSeries(1);
    setSeriesColor(0, QColor(255, 0, 0));

    this->axis_y = axis_y;
    attachAxis();
}

void HistChartView::initDarkToneCurve(double x_max, double y_max)
{
    axis_x = new QValueAxis();
    QtCharts::QValueAxis *axis_y = new QValueAxis();

    axis_x->setLabelFormat("%d");
    axis_y->setLabelFormat("%d");
    axis_x->setTickCount(5);
    axis_y->setTickCount(9);
    this->x_max = x_max;
    axis_x->setRange(0, x_max);
    this->y_max = y_max;
    axis_y->setRange(0, y_max);

    createSeries(1);
    setSeriesColor(0, QColor(255, 0, 0));

    this->axis_y = axis_y;
    attachAxis();
}

void HistChartView::initBrightToneCurve(double x_max, double y_max)
{
    axis_x = new QValueAxis();
    QtCharts::QValueAxis *axis_y = new QValueAxis();

    axis_x->setLabelFormat("%d");
    axis_y->setLabelFormat("%d");
    axis_x->setTickCount(5);
    axis_y->setTickCount(9);
    this->x_max = x_max;
    axis_x->setRange(0, x_max);
    this->y_max = y_max;
    axis_y->setRange(0, y_max);

    createSeries(1);
    setSeriesColor(0, QColor(255, 0, 0));

    this->axis_y = axis_y;
    attachAxis();
}

void HistChartView::initAEHist()
{
    axis_x = new QValueAxis();
    QtCharts::QValueAxis *axis_y = new QValueAxis();

    axis_x->setLabelFormat("%d");
    axis_y->setLabelFormat("%d");
    axis_x->setTickCount(9);
    axis_y->setTickCount(5);
    axis_x->setTitleText("Part");
    axis_y->setTitleText("Hist Value");

    x_max = 256;
    axis_x->setRange(0, x_max);
    y_max = 10000;
    axis_y->setRange(0, y_max);

    createAreaSerie();
    area_serie->setPen(QColor(0, 0, 255));
    area_serie->setBrush(QColor(0, 0, 255, 50));

    this->axis_y = axis_y;
    attachAxis();
}

void HistChartView::setData(QVariant data)
{
    QVariantList data_array = data.toList();
    if (data_array.isEmpty()) return;

    if (!data_array.first().toList().isEmpty()) {
        // is 2d array
        for (int i = 0; (i < data_array.size()) && (i < this->series_list.size()); i++) {
            QVariantList series_data_list = data_array[i].toList();
            QList<QPointF> points = samplePointFromList(series_data_list);
            setPoints(i, points, false);
        }
    } else {
        // is 1d array
        storePointsFromList(data_array);
        QList<QPointF> points = samplePointFromList(data_array);
        setPoints(0, points, false);
    }
}

void HistChartView::hideSeries()
{
    for (int i = 0; i < series_list.size(); i++) {
        series_list.at(i)->hide();
    }

    if (area_serie != NULL)
        area_serie->hide();
}

void HistChartView::showSeries()
{
    for (int i = 0; i < series_list.size(); i++) {
        series_list.at(i)->show();
    }

    if (area_serie != NULL)
        area_serie->show();
}

void HistChartView::setPoints(int series_idx, QList<QPointF> points, bool update_global_data)
{
    if (series_list.size() == 0)
    {
        qDebug("error:series list empty!");
        return;
    }

    double x = 0, y = 0;
    for(int i=0; i<points.size(); i++) {
        if (x < points[i].x()) {
            x = points[i].x();
        }

        if (y < points[i].y()) {
            y = points[i].y();
        }
    }
    //qDebug("x:%f, y:%f", x, y);
    updateAxis(x, y);

    series_list.at(series_idx)->replace(points);

#if 0
    if (update_global_data) {
        SecKeyType reg_section_key = reg_section_key_list[series_idx];
        if (reg_section_key.first.length() > 0 && reg_section_key.second.length() > 0) {
            updateToGlobalData(reg_section_key);
        }
    }
#endif
}

QList<QPointF> HistChartView::getPoints(int series_idx)
{
    return series_list.at(series_idx)->points();
}

QList<qreal> HistChartView::getData(int series_index)
{
    QList<qreal> values;

    for (int i = 0; i < series_list[series_index]->count(); i++) {
        qreal value = series_list[series_index]->at(i).y();

        values.append(value);
    }

    return values;
}

void HistChartView::addRegSectionKey(QString section, QString key)
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
}

/*
void HistChartView::updateToGlobalData(SecKeyType reg_section_key)
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
}*/

SecKeyType HistChartView::getSectionKey()
{
    return reg_section_key_list[0];
}

void HistChartView::setTitle(QString title)
{
    y_chart->setTitle(title);
}

void HistChartView::setSeriesColor(int series_idx, QColor color)
{
    QPen pen = series_list.at(series_idx)->pen();
    pen.setColor(color);
    pen.setWidthF(1);
    series_list.at(series_idx)->setPen(pen);
}

void HistChartView::attachAxis()
{
    y_chart->addAxis(axis_x, Qt::AlignBottom);
    y_chart->addAxis(axis_y, Qt::AlignLeft);
    for (int i = 0; i < y_chart->series().count(); i++) {
        y_chart->series()[i]->attachAxis(axis_x);
        y_chart->series()[i]->attachAxis(axis_y);
    }
}

QtCharts::QLineSeries *HistChartView::createSeries(QColor color)
{
    QLineSeries *series = new QLineSeries();
    QPen pen = series->pen();
    pen.setColor(color);
    pen.setWidth(2);

    series->setPen(pen);
    series->setPointsVisible(true);

    return series;
}

void HistChartView::storePointsFromList(QList<QVariant> points)
{
    stored_points.clear();
    if (points.isEmpty())
        return;

    for (int idx = 0; idx < points.size(); idx++) {
        stored_points.append(points[idx].toPointF());
    }
}

void HistChartView::setDispWindowSize(int n)
{
    if (n < 512)
        disp_window = 512;
    else
        disp_window = n;
}

QList<QPointF> HistChartView::samplePointFromList(QList<QVariant> points)
{
    QList<QPointF> sample_points;
    if (points.isEmpty()) return sample_points;

    int interval = 1;
    if (points.size() > disp_window) {
        interval = points.size() / disp_window;
    }

    int idx;
    int i;
    for (idx = 0, i = 0; idx < points.size(); idx += interval, i++) {
        sample_points.append(QPointF(idx, points[idx].toDouble()));
    }

    if (idx >= points.size()) {
        idx = points.size() - 1;
       sample_points.append(QPointF(idx, points[idx].toDouble()));
    }
    return sample_points;
}

void HistChartView::refreshData(const SecKeyType &reg_section_key)
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

        storePointsFromList(all_points);
        QList<QPointF> points = samplePointFromList(all_points);
        setPoints(i, points, false);
    }
}

int HistChartView::getSeriesCount()
{
    return series_list.size();
}

void HistChartView::handleUpdateTheme(bool dark_theme_enabled)
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
