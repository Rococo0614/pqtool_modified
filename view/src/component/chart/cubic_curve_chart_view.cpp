#include "cubic_curve_chart_view.hpp"
#include <QtCore>
#include <QToolTip>
#include <QtGlobal>
#include "global_data.hpp"
#include "utils.hpp"


using namespace QtCharts;

#define GLOBAL_TONE_CURVE_NODE_NUM (1025)
#define DARK_TONE_CURVE_NODE_NUM (257)
#define BRIT_TONE_CURVE_NODE_NUM (513)

namespace {

template<typename T>
inline bool is_positive(const T &x) {
    return (x > 0) ? true : false;
}

bool chckxy(const std::vector<int> &x_in, const std::vector<int> &y_in) {
    // check the tinput length should be > 1
    if (x_in.size() < 2) {
        qDebug("[error] the size of x_in should be >= 2");
        return true;
    }

    // check the input length are equal
    if (x_in.size() != y_in.size()) {
        qDebug("[error] the sizes of x_in and y_in should be equal");
        return true;
    }

    // check x_in is strictly increasing
    for (decltype(x_in.size()) i = 0; i < (x_in.size() - 1); ++i) {
        auto diff = x_in.at(i + 1) - x_in.at(i);
        if (diff < 0) {
            qDebug("[error] x_in should be strectly increasing");
            return true;
        }
    }

    // check y_in is strectly increasing
    for (decltype(y_in.size()) i = 0; i < (y_in.size() - 1); ++i) {
        auto diff = y_in.at(i + 1) - y_in.at(i);
        if (diff < 0) {
            qDebug("[error] y_in should be strectly increasing" );
            return true;
        }
    }

    return false;
}

}


std::vector<int> interp1_pchip(const std::vector<int> &x_in, const std::vector<int> &y_in) {

    if (chckxy(x_in, y_in)) {return {};}

    const auto &n = static_cast<int>(x_in.size());

    // initial y_out size
    int y_out_size = x_in.back() - x_in.front() + 1;
    std::vector<int> y_out(y_out_size, 0);

    // (decltype(a.size())
    // get interval length h
    std::vector<float> h(n, 0);
    for (auto i = 0; i < (n - 1); i++){
        h.at(i) = x_in.at(i+1) - x_in.at(i);
    }

    // get delta value
    std::vector<float> delta(n, 0);
    for (auto i = 0; i < (n - 1); i++){
        delta.at(i) = (y_in.at(i+1) - y_in.at(i)) / h.at(i);
    }

    // special case for n = 2
    if (n == 2) {
        for (auto i = 0; i < y_out_size; i++) {
           auto tmp = i * delta.at(0);
           y_out.at(i) = std::min(std::max(static_cast<int>(std::round(tmp)), 0), 4095);
        }
        return y_out;
    }

    // get slopes
    std::vector<float> slopes(n, 0);

    // Slopes at interior points.
    for (auto i = 1; i < n - 1; i++) {
        if ((is_positive(delta.at(i-1)) && is_positive(delta.at(i)))) {
            float w1 = h.at(i-1) + 2 * h.at(i);
            float w2 = 2 * h.at(i-1) + h.at(i);
            slopes.at(i) = (w1 + w2) / (w1 / delta.at(i-1) + w2 / delta.at(i));
        }
    }

    // Slopes at start points.
    slopes.at(0) = ((2 * h.at(0) + h.at(1)) * delta.at(0) - h.at(0) * delta.at(1)) / (h.at(0) + h.at(1));
    if (is_positive(slopes.at(0)) != is_positive(delta.at(0)) ) {
        slopes.at(0) = 0.0f;
    } else if ((is_positive(delta.at(0)) != is_positive(delta.at(1))) &&
               (std::abs(slopes.at(0)) > std::abs(3 * delta.at(0)))) {
        slopes.at(0) = 3.0f * delta.at(0);
    }

    // Slopes at end points.
    slopes.at(n-1) = ((2 * h.at(n-2) + h.at(n-3)) * delta.at(n-2) - h.at(n-2) * delta.at(n-3)) / (h.at(n-2) + h.at(n-3));
    if (is_positive(slopes.at(n-1)) != is_positive(delta.at(n-2)) ) {
        slopes.at(n-1) = 0.0f;
    } else if ((is_positive(delta.at(n-2)) != is_positive(delta.at(n-3))) &&
               (std::abs(slopes.at(n-1)) > std::abs(3 * delta.at(n-2))) ) {
        slopes.at(n -1) = 3.0f * delta.at(n-2);
    }

    // Compute piecewise cubic Hermite interpolant to those values and slopes
    std::vector<int> k(y_out_size, 0);
    for (auto i = 0; i < (n - 1); i++) {
        for (auto j = 0; j < y_out_size; j++) {
            if (x_in.at(i) <= (j + x_in.at(0))) {
                k.at(j) = i;
            }
        }
    }

    // Piecewise polynomial coefficients
    for (auto i = 0; i < y_out_size; i++) {
        const auto &idx = k.at(i);
        float c = (3 * delta.at(idx) - 2 * slopes.at(idx) - slopes.at(idx + 1)) / h.at(idx);
        float b = (slopes.at(idx) - 2 * delta.at(idx) + slopes.at(idx + 1)) / (h.at(idx) * h.at(idx));
        float s = i + x_in.at(0) - x_in.at(idx);
        float tmp = y_in.at(idx) + s * (slopes.at(idx) + s * (c + s * b));
        y_out.at(i) = std::min(std::max(static_cast<int>(std::round(tmp)), 0), 4095);
    }

    return y_out;
}

CubicCurveChartView::CubicCurveChartView(QWidget *parent) : QChartView (parent),
    fix_end_point(false)
{
    setRenderHint(QPainter::Antialiasing);
    initChart();
}

CubicCurveChartView::~CubicCurveChartView()
{
    for (auto reg_section_key : reg_section_key_list) {
        GlobalData::getInstance()->unSubscribeObserver(reg_section_key, this);
    }
}

void CubicCurveChartView::createSeries()
{
    line_serie = createLineSeries(QColor(255, 0, 0));
    line_serie->setName("User");
    y_chart->addSeries(line_serie);
    connect(line_serie, &QLineSeries::hovered,
            this, std::bind(&CubicCurveChartView::pointHovered, this, line_serie, std::placeholders::_1, std::placeholders::_2));

    scatter_serie = createScatterSeries(QColor(255, 0, 0));
    scatter_serie->setName("Points");
    y_chart->addSeries(scatter_serie);
    connect(scatter_serie, &QScatterSeries::hovered,
            this, &CubicCurveChartView::scatterPointHovered);
    //connect(scatter_serie, &QScatterSeries::hovered,
            //this, std::bind(&CubicCurveChartView::scatterPointHovered, this, scatter_serie, std::placeholders::_1, std::placeholders::_2));
    connect(scatter_serie, &QScatterSeries::pressed,
            this, std::bind(&CubicCurveChartView::scatterPointPressed, this, scatter_serie, std::placeholders::_1));
    connect(scatter_serie, &QScatterSeries::released,
            this, std::bind(&CubicCurveChartView::scatterPointReleased, this, scatter_serie, std::placeholders::_1));
}

void CubicCurveChartView::initChart()
{
    y_chart = new QChart();
    setChart(y_chart);
    y_chart->setPlotAreaBackgroundVisible(true);
}

void CubicCurveChartView::scatterPointPressed(QtCharts::QScatterSeries *series, QPointF point)
{
    if (user_define_mode) {
        int idx = getCloestPointIndex(series, point);

        if (idx > -1 && idx < x_max)
        {
            emit sigPressedScatterPt(QPointF(series->at(idx).x(), series->at(idx).y()));
            holdIndex = idx;
            bHoldCurve = true;
        }
    }
}

void CubicCurveChartView::scatterPointReleased(QtCharts::QScatterSeries *series, QPointF point)
{
    if (!user_define_mode)
        return;

    QPointF new_position = mapFromGlobal(QCursor::pos());
    QPointF new_point = series->chart()->mapToValue(new_position);

    m_x = point.x();
    m_y = new_point.y();

    if (bHoldCurve && (holdIndex >= 0 && holdIndex < x_max)) {
        if (fix_end_point) {
            if (holdIndex == scatter_serie->points().count() - 1) {
                return;
            }
        }
        double max_y = m_y;
        double min_y = m_y;
        if (force_correct) {
            if (holdIndex + 1 < control_ps.size()) {
                max_y = control_ps[holdIndex + 1].y();
            }

            if (holdIndex - 1 >= 0) {
                min_y = control_ps[holdIndex - 1].y();
            }
            if (max_y < min_y) min_y = max_y;
        }
        updateSeriesPoint(scatter_serie, holdIndex, QPointF(m_x, qBound(min_y, m_y, max_y)));
    }

    holdIndex = -1;
    bHoldCurve = false;
}

void CubicCurveChartView::updateControlPoints(int num)
{
    qDebug("CubicCurveChartView::updateControlPoints");
    QList<QPointF> ps = line_serie->points();
    control_ps.clear();
    node_x.clear();
    node_y.clear();
    tc_node_x.clear();
    tc_node_y.clear();

    node_x.prepend(0);
    node_y.prepend(0);
    if (isUserDefineCurve)
    {
        tc_node_x.prepend(0);
        tc_node_y.prepend(0);
    }

    control_ps.append(ps[0]);
    qreal step = static_cast<qreal>(ps.size()) / (num - 1);
    for (int i=1; i < num - 1; i++) {
        node_x.append(ps[static_cast<int>(i*step)].x());
        node_y.append(ps[static_cast<int>(i*step)].y());
        if (isUserDefineCurve)
        {
            tc_node_x.append(ps[static_cast<int>(i*step)].x());
            tc_node_y.append(ps[static_cast<int>(i*step)].y());
        }
        control_ps.append(ps[static_cast<int>(i*step)]);
    }

    node_x.append(x_max);
    node_y.append(y_max);
    if (isUserDefineCurve)
    {
        tc_node_x.append(x_max);
        tc_node_y.append(y_max);
    }

    control_ps.append(QPointF(x_max, y_max));

    scatter_serie->replace(control_ps);
}

void CubicCurveChartView::resetControlPoints()
{

}

void CubicCurveChartView::updateToneCurve()
{
    if (isUserDefineCurve) {
        genToneCurve(tc_node_x, tc_node_y);
    }
}

void CubicCurveChartView::updateSeriesPoint(QtCharts::QXYSeries *series, int index, const QPointF &point, bool update_to_global)
{
    qDebug("CubicCurveChartView::updateSeriesPoint");
    //firstly, update control points
    int new_y = static_cast<int>(point.y());
    int new_x = static_cast<int>(series->at(index).x());
    control_ps.replace(index, QPointF(new_x, new_y));

    //secondly, update curve points, using cubic intp
    if (isUserDefineCurve) {
        tc_node_y.replace(index, new_y);
        genToneCurve(tc_node_x, tc_node_y);
        setPoints(getPCHIPCurve(tc_node_x, tc_node_y), update_to_global);
    } else {
        node_y.replace(index, new_y);
        setPoints(getPCHIPCurve(node_x, node_y), update_to_global);
    }

    //line_serie->replace(genSmoothCurve(control_point_x, control_point_y));
    scatter_serie->replace(control_ps);

    // if auto write, trigger write data
    if (GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_AUTO_WRITE).toInt()) {
        emit sigPageWriteData();
    }
}

QList<QPointF> CubicCurveChartView::genSmoothCurve(QList<qreal> &point_x, QList<qreal> &point_y)
{
    qDebug("CubicCurveChartView::genSmoothCurve");
    const double down_bound = 0;
    const double up_bound = 4095;
    QVector<qreal> intp_point_y;
    intp_point_y.resize(x_max);

    qreal x[4], y[4];
    for (int i = 0; i < x_max; i++) {
        qreal current_x = i;
        int idx = 1;
        qreal y_intp;
        for (int j = 1; j < point_x.size() - 2; j++)
        {
            if (point_x[j] <= current_x && current_x <= point_x[j + 1])
            idx = j;
        }
        x[0] = point_x[idx - 1];
        x[1] = point_x[idx - 0];
        x[2] = point_x[idx + 1];
        x[3] = point_x[idx + 2];
        y[0] = point_y[idx - 1];
        y[1] = point_y[idx - 0];
        y[2] = point_y[idx + 1];
        y[3] = point_y[idx + 2];
        utils::cubic_intp_1D( y[0], y[1], y[2], y[3], x[0], x[1], x[2], x[3], current_x, &y_intp);
        intp_point_y[i] = y_intp;
    }

    QList<QPointF> ps;
    for (int i = 0; i < x_max; i++) {
        ps.append(QPointF(i, qBound(down_bound, intp_point_y[i], up_bound)));
    }

    return ps;
}

QList<QPointF> CubicCurveChartView::getPCHIPCurve(QList<qreal> &point_x, QList<qreal> &point_y)
{
    qDebug("CubicCurveChartView::getPCHIPCurve");
    const double down_bound = 0;
    const double up_bound = 4095;

    int ctrl_point_num = point_x.size();
    std::vector<int> x_in(ctrl_point_num, 0);
    std::vector<int> y_in(ctrl_point_num, 0);
    int wdr_hist_num = 256 * exp_ratio;
    for (int i = 0; i < ctrl_point_num; i++) {
        x_in.at(i) = point_x[i];
        y_in.at(i) = point_y[i];
    }

    if (x_in.at(0) != 0) {
        qDebug("error: x_in start with 0");
    }

    if (x_in.back() != wdr_hist_num) {
        qDebug("error: x_in end with wdr_hist_num");
    }

    std::vector<int> y_out = interp1_pchip(x_in, y_in);
    if (y_out.size() != (wdr_hist_num + 1)) {
        qDebug("error: y_out length error");
    }

    QList<QPointF> ps;
    for (int i = 0; i < (wdr_hist_num + 1); i++) {
        ps.append(QPointF(i, qBound(down_bound, (qreal)y_out[i], up_bound)));
    }

    return ps;
}

bool CubicCurveChartView::compare(qreal v1, qreal v2)
{
    return qFuzzyCompare(v1, v2);
}

void CubicCurveChartView::initToneCurve()
{
    isUserDefineCurve = true;
    user_define_control_pt = true;

    axis_x = new QValueAxis();
    QtCharts::QValueAxis *axis_y = new QValueAxis();

    axis_x->setLabelFormat("%d");
    axis_y->setLabelFormat("%d");
    axis_x->setTickCount(5);
    axis_y->setTickCount(9);
    x_max = 1024;
    axis_x->setRange(0, x_max);
    y_max = 4095;
    axis_y->setRange(0, y_max);

    createSeries();
    setSeriesColor(QColor(255, 0, 0));

    this->axis_y = axis_y;
    attachAxis();
    fix_end_point = true;
}

void CubicCurveChartView::initGamma(int x_maximum, int y_maximum)
{
    axis_x = new QValueAxis();
    QtCharts::QValueAxis *axis_y = new QValueAxis();

    axis_x->setLabelFormat("%d");
    axis_y->setLabelFormat("%d");
    axis_x->setTickCount(5);
    axis_y->setTickCount(9);
    x_max = x_maximum;
    axis_x->setRange(0, x_max);
    y_max = y_maximum;
    axis_y->setRange(0, y_max);

    createSeries();
    setSeriesColor(QColor(255, 0, 0));

    this->axis_y = axis_y;
    attachAxis();
    fix_end_point = true;
    setExpRatio(1);
}

void CubicCurveChartView::initMapCurve(int x_maximum, int y_maximum)
{
    axis_x = new QValueAxis();
    QtCharts::QValueAxis *axis_y = new QValueAxis();

    axis_x->setLabelFormat("%d");
    axis_y->setLabelFormat("%d");
    axis_x->setTickCount(9);
    axis_y->setTickCount(5);
    x_max = x_maximum;
    axis_x->setRange(0, x_max);
    y_max = y_maximum;
    axis_y->setRange(0, y_max);

    this->axis_y = axis_y;
    attachAxis();
    fix_end_point = false;
    setExpRatio(1);
}

void CubicCurveChartView::setData(QVariant data)
{
    qDebug("CubicCurveChartView::setData");
    QVariantList data_array = data.toList();
    if (data_array.isEmpty()) return;

    QList<QPointF> points = samplePointFromList(data_array);
    setPoints(points);
    updateControlPoints(control_num);
}

void CubicCurveChartView::setPoints(QList<QPointF> points, bool update_global_data)
{
    if (line_serie == nullptr)
    {
        qDebug("error:series list empty!");
        return;
    }

    line_serie->replace(points);
    if (isUserDefineCurve) {
        global_tone_mw.clear();
        int count = line_serie->count();
        for (int i = 0; i < count; i++) {
            qreal value = line_serie->at(i).y();
            global_tone_mw.append(value);
        }
    }

    if (update_global_data) {
        foreach(SecKeyType reg_section_key, reg_section_key_list) {
            if (reg_section_key.first.length() > 0 && reg_section_key.second.length() > 0) {
                updateToGlobalData(reg_section_key);
            }
        }

        emit sigValueChanged(QVariant());
    }
}

QList<QPointF> CubicCurveChartView::getPoints(int series_idx)
{
    return line_serie->points();
}

QList<qreal> CubicCurveChartView::getData(int series_index)
{
    QList<qreal> values;
    int count = line_serie->count();
    for (int i = 0; i < count && i < x_max; i++) {
        qreal value = line_serie->at(i).y();
        values.append(value);
    }

    return values;
}

void CubicCurveChartView::addRegSectionKey(QString section, QString key)
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

void CubicCurveChartView::updateToGlobalData(SecKeyType reg_section_key)
{
    QList<qreal> all_points = getInterpolationDataList(0);
    QJsonArray value_array;

    for (int j = 0; j < all_points.size(); j++) {
        value_array.append(static_cast<int>(all_points[j]));
    }

    if (value_array.size() == 0) {
        return;
    }

    //int index = reg_section_key_list.indexOf(reg_section_key);

    //GlobalData::getInstance()->setRegValue(reg_section_key.first, reg_section_key.second, static_cast<int>(all_points[index]), this);
    GlobalData::getInstance()->setRegValue(reg_section_key.first, reg_section_key.second, value_array, this);
}

SecKeyType CubicCurveChartView::getSectionKey()
{
    return reg_section_key_list[0];
}

void CubicCurveChartView::setTitle(QString title)
{
    y_chart->setTitle(title);
}

void CubicCurveChartView::mouseMoveEvent(QMouseEvent *event)
{
    QChartView::mouseMoveEvent(event);
    m_x = y_chart->mapToValue(event->pos()).x();
    m_y = y_chart->mapToValue(event->pos()).y();

    emit sigMouseMove(m_x, m_y);

    if (user_define_mode && bHoldCurve && (holdIndex >= 0 && holdIndex < x_max)) {
        if (fix_end_point) {
            if (holdIndex == scatter_serie->points().count() - 1) {
                return;
            }
        }
        double max_y = m_y;
        double min_y = m_y;
        if (force_correct) {
            if (holdIndex + 1 < control_ps.size()) {
                max_y = control_ps[holdIndex + 1].y();
            }

            if (holdIndex - 1 >= 0) {
                min_y = control_ps[holdIndex - 1].y();
            }
            if (max_y < min_y) min_y = max_y;
        }

        QPointF p(scatter_serie->at(holdIndex).x(), qBound(min_y, m_y, max_y));
        emit sigPressedScatterPt(p);
        updateSeriesPoint(scatter_serie, holdIndex, QPointF(m_x, qBound(min_y, m_y, max_y)), false);
    }
}

void CubicCurveChartView::mousePressEvent(QMouseEvent *event)
{
    QChartView::mousePressEvent(event);
}

void CubicCurveChartView::mouseReleaseEvent(QMouseEvent *event)
{
    QChartView::mouseReleaseEvent(event);
}

void CubicCurveChartView::focusOutEvent(QFocusEvent *event)
{
    QChartView::focusOutEvent(event);

}

void CubicCurveChartView::setSeriesColor(QColor color)
{
    QPen pen = line_serie->pen();
    pen.setColor(color);
    line_serie->setPen(pen);

    QPen pen2 = scatter_serie->pen();
    pen2.setColor(color);
    scatter_serie->setPen(pen2);
}

int CubicCurveChartView::getCloestPointIndex(QtCharts::QXYSeries *series, const QPointF &point)
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

QString CubicCurveChartView::getTooltipText(const QPointF &point)
{
    QString text;
    text.sprintf("%d, %d", static_cast<int>(point.x()), static_cast<int>(point.y()));
    return text;
}

void CubicCurveChartView::attachAxis()
{
    y_chart->addAxis(axis_x, Qt::AlignBottom);
    y_chart->addAxis(axis_y, Qt::AlignLeft);
    for (int i = 0; i < y_chart->series().count(); i++) {
        y_chart->series()[i]->attachAxis(axis_x);
        y_chart->series()[i]->attachAxis(axis_y);
    }
}

void CubicCurveChartView::pointHovered(QLineSeries *series, const QPointF &point, bool state)
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

void CubicCurveChartView::scatterPointHovered(const QPointF &point, bool state)
{
//    qDebug("Scatter point(%d, %d) hovered.", point.x(), point.y());
    if (state == false) {
        QToolTip::hideText();
        return;
    }

    int idx = getCloestPointIndex(scatter_serie, point);

    if (idx != -1) {
        QPointF p = scatter_serie->at(idx);
        QString text = getTooltipText(p);
        QToolTip::showText(QCursor::pos(), text);
    }
}

QtCharts::QLineSeries *CubicCurveChartView::createLineSeries(QColor color)
{
    QLineSeries *series = new QLineSeries();
    QPen pen = series->pen();
    pen.setColor(color);
    pen.setWidth(2);

    series->setPen(pen);
    series->setPointsVisible(false);
    series->setUseOpenGL(true);

    return series;
}

QtCharts::QScatterSeries *CubicCurveChartView::createScatterSeries(QColor color)
{
    QScatterSeries *series = new QScatterSeries();
    QPen pen = series->pen();
    pen.setColor(color);
    pen.setWidth(2);
    series->setPen(pen);
    series->setPointsVisible(true);
    series->setMarkerSize(10);
    series->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    series->setBrush(QBrush(QColor(255, 127, 0)));
    series->setBorderColor(QColor(255, 127, 0));
    series->setUseOpenGL(true);

    return series;
}

QList<QPointF> CubicCurveChartView::samplePointFromList(QList<QVariant> points)
{
    QList<QPointF> sample_points;
    if (points.isEmpty()) return sample_points;
    int idx;
    for (idx = 0; idx < points.size() && idx < x_max; idx++) {
        sample_points.append(QPointF(idx, points[idx].toDouble()));
    }
    sample_points.append(QPointF(x_max, y_max));

    return sample_points;
}

void CubicCurveChartView::refreshData(const SecKeyType &reg_section_key)
{
    qDebug("CubicCurveChartView::refreshData");
    for (int i = 0; i < reg_section_key_list.size(); i++) {
        if (reg_section_key_list[i] != reg_section_key) {
            continue;
        }
        QVariant data = GlobalData::getInstance()->getRegValue(reg_section_key.first, reg_section_key.second, "");
        QList<QVariant> all_points = utils::jsonArrayToList(data.toJsonArray());

        if (all_points.size() == 0) {
            continue;
        }

        QList<QPointF> points = samplePointFromList(all_points);
        setPoints(points, false);
        if (!user_define_control_pt) {
            updateControlPoints(control_num);
        }
        break;
    }
}

QString CubicCurveChartView::getInterpolationDataString(int series_idx, QString delimeter)
{
    QString data_str = utils::listToString(getInterpolationDataList(series_idx), std::move(delimeter));
    return data_str;
}

QList<qreal> CubicCurveChartView::getInterpolationDataList(int series_idx)
{
    QList<qreal> values = getData(series_idx);
    if (values.isEmpty()) return values;

    return values;
}

void CubicCurveChartView::addControlPt(double x, double y)
{
    if (x < 0 || x > x_max || y < 0 || y > y_max)
        return;

    user_define_control_pt = true;

    QList<QPointF> ps = line_serie->points();

    int i = 0;
    bool dup = false;
    for (i = 0; i < control_ps.size(); i++) {
        if (x == control_ps[i].x()) {
            dup = true;
            break;
        } else if (x < control_ps[i].x()) {
            break;
        }
    }

    if (dup) {
        control_ps.replace(i,QPointF(x, y));
    } else {
        if (i == control_ps.size()) {
            control_ps.append(QPointF(x, y));
        } else {
            control_ps.insert(i, QPointF(x, y));
        }
    }

    node_x.clear();
    node_y.clear();
    node_x.prepend(0);
    node_y.prepend(0);
    for(int i = 0; i < control_ps.size(); i++) {
        node_x.append(control_ps[i].x());
        node_y.append(control_ps[i].y());
    }

    node_x.append(x_max);
    node_y.append(y_max);

    //update curve
    if (isUserDefineCurve) {
        genToneCurve(tc_node_x, tc_node_y);
    }
    setPoints(getPCHIPCurve(node_x, node_y), true);

    scatter_serie->replace(control_ps);
}

void CubicCurveChartView::setControlPointsNum(int num)
{
    qDebug("CubicCurveChartView::setControlPointsNum");
    control_num = num;
    updateControlPoints(control_num);
}

int CubicCurveChartView::getControlPointsNum()
{
    return control_num;
}

void CubicCurveChartView::setUserDefineMode(bool m)
{
    user_define_mode = m;
}

double CubicCurveChartView::getY(double x)
{
    if ( x >= 0 && x < x_max) {
        return line_serie->points().at(static_cast<int>(x)).y();
    }
}

void CubicCurveChartView::setUserDefineCtrlPt(bool en)
{
    this->user_define_control_pt = en;
}


static int user_define_curve_gen(int ctrl_point_num, QList<qreal> ctrl_point_x, QList<qreal> ctrl_point_y, float exposure_ratio, int *global_tone, int *dark_tone, int *brit_tone)
{
    std::vector<int> x_in(ctrl_point_num, 0);
    std::vector<int> y_in(ctrl_point_num, 0);
    int wdr_hist_num = 256 * exposure_ratio;
    for (int i = 0; i < ctrl_point_num; i++) {
        x_in.at(i) = ctrl_point_x[i];
        y_in.at(i) = ctrl_point_y[i];
        qDebug("ctrl_point_x[%d] = %lf, ctrl_point_y[%d] = %lf", i, ctrl_point_x[i], i, ctrl_point_y[i]);
    }

    if (x_in.at(0) != 0) {
        qDebug("error: x_in start with 0");
        return -1;
    }

    if (x_in.back() != wdr_hist_num) {
        qDebug("error: x_in end with wdr_hist_num");
        return -1;
    }

    std::vector<int> y_out = interp1_pchip(x_in, y_in);
    if (y_out.size() != (wdr_hist_num + 1)) {
        qDebug("error: y_out length error");
        return -1;
    }

    for (int i = 0; i < (wdr_hist_num + 1); i++) {
         global_tone[i] = y_out.at(i);
    }

    qDebug("global_tone[256] = %d, wdr_hist_num = %d, exposure_ratio = %lf", global_tone[256], wdr_hist_num, exposure_ratio);

    return 0;
}

void CubicCurveChartView::genToneCurve(QList<qreal> &x, QList<qreal> &y)
{
    int *global_tone = new int[GLOBAL_TONE_CURVE_NODE_NUM];
    int *dark_tone = new int[DARK_TONE_CURVE_NODE_NUM];
    int *brit_tone = new int[BRIT_TONE_CURVE_NODE_NUM];

    user_define_curve_gen(x.size(), x, y, exp_ratio, global_tone, dark_tone, brit_tone);

    global_tone_mw.clear();
    for (int i = 0; i < GLOBAL_TONE_CURVE_NODE_NUM; i++)
    {
        global_tone_mw.append(global_tone[i]);
    }

    // dark_tone_mw.clear();
    // for (int i = 0; i < DARK_TONE_CURVE_NODE_NUM; i++)
    // {
    //     dark_tone_mw.append(dark_tone[i]);
    // }

    // brit_tone_mw.clear();
    // for (int i = 0; i < BRIT_TONE_CURVE_NODE_NUM; i++)
    // {
    //     brit_tone_mw.append(brit_tone[i]);
    // }

    delete [] global_tone;
    delete [] dark_tone;
    delete [] brit_tone;
}

void CubicCurveChartView::setExpRatio(float e)
{
    exp_ratio = e;
}

QVariantList CubicCurveChartView::getGlobalTone()
{
    return global_tone_mw;
}

QVariantList CubicCurveChartView::getDarkTone()
{
    return dark_tone_mw;
}

QVariantList CubicCurveChartView::getBritTone()
{
    return brit_tone_mw;
}

void CubicCurveChartView::handleUpdateTheme(bool dark_theme_enabled)
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

void CubicCurveChartView::addLine(QtCharts::QLineSeries *series_add)
{
    y_chart->addSeries(series_add);
    attachAxis();
}

void CubicCurveChartView::setRange(int max_x, int max_y)
{
    x_max = max_x;
    y_max = max_y;
    axis_x->setRange(0, x_max);
    axis_y->setRange(0, y_max);

}
