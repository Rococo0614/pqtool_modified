#include "hue_line_chart_view.hpp"
#include <QtCore>
#include <QtCharts>
#include "utils.hpp"
#include "global_data.hpp"

using namespace QtCharts;

HueLineChartView::HueLineChartView(QWidget *parent) :
    LineChartView(16, parent)
{
    multifier = 128;
}

void HueLineChartView::resizeEvent(QResizeEvent *event)
{
    QChartView::resizeEvent(event);

    QImage *img = nullptr;
    if (background_image_type == 1) {
        img = getSbyHBackgroundImage();
    } else if (background_image_type == 2) {
        img = getVbyHBackgroundImage();
    }

    if (!img) {
        return;
    }

    QBrush brush = QBrush(*img);
    y_chart->setPlotAreaBackgroundBrush(brush);
}

void HueLineChartView::initSByH()
{
    background_image_type = 1;
    initHue();
}

void HueLineChartView::initVByH()
{
    background_image_type = 2;
    initHue();
}

void HueLineChartView::initHue()
{
    axis_x = new QValueAxis();
    QLogValueAxis *axis_log_y = new QLogValueAxis();

    axis_x->setLabelFormat("%d");
    axis_log_y->setLabelFormat("%.4f");
    axis_x->setTickCount(17);
    axis_x->setRange(0, 48);
    axis_log_y->setBase(2);
    axis_log_y->setRange(0.0625, 8);

    createSeries(1);
    axis_y = axis_log_y;
    attachAxis();
}

void HueLineChartView::setBackgroundImageType(int type)
{
    background_image_type = type;
}

QImage *HueLineChartView::getVbyHBackgroundImage()
{
    int w = static_cast<int>(y_chart->plotArea().width());
    int h = static_cast<int>(y_chart->plotArea().height());
    QImage *img = new QImage(w, h, QImage::Format_RGB32);

    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            QColor color;
            double hue = (int(i - y_chart->plotArea().x() - 1) % w + w) % w / double(w);
            double v = 1 - (int(j - y_chart->plotArea().y() - 1) % h + h) % h / double(h);
            color.setHsvF(hue, 1, v);
            img->setPixelColor(i, j, color);
        }
    }

    return img;
}

QImage *HueLineChartView::getSbyHBackgroundImage()
{
    int w = static_cast<int>(y_chart->plotArea().width());
    int h = static_cast<int>(y_chart->plotArea().height());
    QImage *img = new QImage(w, h, QImage::Format_RGB32);

    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            QColor color;
            double hue = (int(i - y_chart->plotArea().x() - 1) % w + w) % w / double(w);
            double s = 1 - (int(j - y_chart->plotArea().y() - 1) % h + h) % h / double(h);
            color.setHsvF(hue, s, 1);
            img->setPixelColor(i, j, color);
        }
    }

    return img;
}

QString HueLineChartView::getTooltipText(const QPointF &point)
{
    QString text;
    text.sprintf("%d, %.4f", static_cast<int>(point.x()), static_cast<double>(point.y()));
    return text;
}

qreal HueLineChartView::getYByX(qreal x)
{
    qreal y = 0;

    for (int i = 0; i < series_list.at(0)->count(); i++) {
        if (compare(series_list.at(0)->at(i).x(), x)) {
            y = series_list.at(0)->at(i).y();
        }
    }

    return y;
}

void HueLineChartView::updateYByX(qreal x, qreal new_y)
{
    for (int i = 0; i < series_list.at(0)->count(); i++) {
        if (compare(series_list.at(0)->at(i).x(), x)) {
            series_list.at(0)->replace(i, series_list.at(0)->at(i).x(), new_y);
        }
    }
}

void HueLineChartView::updateSeriesPoint(QtCharts::QLineSeries *series, int index, const QPointF &point)
{
    double new_y = static_cast<double>(point.y());
    series->replace(index, series->at(index).x(), new_y);
}

void HueLineChartView::refreshData(const SecKeyType&)
{
    QVariant data = GlobalData::getInstance()->getRegValue(reg_section_key_list[0].first, reg_section_key_list[0].second, "");
    QList<QVariant> all_points = utils::jsonArrayToList(data.toJsonArray());
    QList<QPointF> points = samplePointFromList(all_points, interpolation_num);

    for (int i = 0; i < points.size(); i++) {
        points[i] = QPointF(i, points[i].y());
    }
    setPoints(0, points, false);
}
