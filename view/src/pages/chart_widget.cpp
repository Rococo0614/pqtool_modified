#include "chart_widget.hpp"
#include <QtCharts/QChart>
#include <QtCharts/QChartView>

ChartWidget::ChartWidget(CHAR_PROPERTY property, QWidget *parent) :
    QWidget(parent)
{
    setProperty(property);
    initUI();
}

void ChartWidget::setProperty(CHAR_PROPERTY property)
{
    this->title = property.title;
    this->title_x = property.title_x;
    this->title_y = property.title_y;
    this->rang_x[0] = property.rang_x[0];
    this->rang_x[1] = property.rang_x[1];
    this->rang_y[0] = property.rang_y[0];
    this->rang_y[1] = property.rang_y[1];
}

void ChartWidget::initUI()
{
    main_layout = new QHBoxLayout;

    scatter_series = new QtCharts::QScatterSeries();
    scatter_series->setMarkerShape(QtCharts::QScatterSeries::MarkerShapeRectangle);
    scatter_series->setBorderColor(Qt::black);
    scatter_series->setBrush(QBrush(Qt::blue));
    scatter_series->setMarkerSize(8);

    QPen pen;
    pen.setColor(Qt::darkRed);
    pen.setWidth(3);
    line_series = new QtCharts::QLineSeries();
    line_series->setPen(pen);

    // set axes for chart
    chart = new QtCharts::QChart();
    chart->addSeries(scatter_series);
    chart->addSeries(line_series);
    chart->setTitle(title);
    axis_x = new QtCharts::QValueAxis();
    axis_y = new QtCharts::QValueAxis();
    axis_x->setRange(rang_x[0], rang_x[1]);
    axis_y->setRange(rang_y[0], rang_y[1]);
    axis_x->setTitleText(title_x);
    axis_y->setTitleText(title_y);
    chart->setAxisX(axis_x, scatter_series);
    chart->setAxisY(axis_y, scatter_series);
    chart->setAxisX(axis_x, line_series);
    chart->setAxisY(axis_y, line_series);
    QtCharts::QChartView *chartview = new QtCharts::QChartView(chart);
    chartview->setRenderHint(QPainter::Antialiasing);

    main_layout->addWidget(chartview);
	setLayout(main_layout);
}

void ChartWidget::handleAddScatterPoint(float x, float y)
{
    scatter_series->append(x, y);
    if (x > rang_x[1]) {
        rang_x[1] = x;
    } else if (x < rang_x[0]) {
        rang_x[0] = x;
    }
    if (y > rang_y[1]) {
        rang_y[1] = y;
    } else if (y < rang_y[0]) {
        rang_y[0] = y;
    }
    axis_x->setRange(rang_x[0], rang_x[1]);
    axis_y->setRange(rang_y[0], rang_y[1]);
}

void ChartWidget::handleDelScatterPoint(int idx)
{
    scatter_series->remove(idx);
}


void ChartWidget::handleUpdateLinePoints(int num, float *x, float *y)
{
    line_series->clear();
    for (int i = 0; i < num; i++) {
        line_series->append(x[i], y[i]);
        if (x[i] > rang_x[1]) {
            rang_x[1] = x[i];
        } else if (x[i] < rang_x[0]) {
            rang_x[0] = x[i];
        }
        if (y[i] > rang_y[1]) {
            rang_y[1] = y[i];
        } else if (y[i] < rang_y[0]) {
            rang_y[0] = y[i];
        }
    }
    axis_x->setRange(rang_x[0], rang_x[1]);
    axis_y->setRange(rang_y[0], rang_y[1]);
}

void ChartWidget::setSeriesVisible(int idx, bool state)
{
    if (idx == 0) {
        scatter_series->setVisible(state);
    } else if (idx == 1) {
        line_series->setVisible(state);
    }
}

void ChartWidget::clearSeries(int idx)
{
    if (idx == 0) {
        scatter_series->clear();
    } else if (idx == 1) {
        line_series->clear();
    }
}

void ChartWidget::resetAxisRang(float xmin, float xmax, float ymin, float ymax)
{
    rang_x[0] = xmin;
    rang_x[1] = xmax;
    rang_y[0] = ymin;
    rang_y[1] = ymax;
    axis_x->setRange(xmin, xmax);
    axis_y->setRange(ymin, ymax);
}

