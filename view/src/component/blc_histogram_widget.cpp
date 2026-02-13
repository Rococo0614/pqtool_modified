#include "blc_histogram_widget.hpp"
#include <QtCharts/QChart>
#include <QtCharts/QChartView>

BlcHistogramWidget::BlcHistogramWidget(QWidget *parent) :
	QWidget(parent)
{
	initUI();
}

void BlcHistogramWidget::initUI()
{
	main_layout = new QHBoxLayout;
	color_list.append(tr("R"));
	color_list.append(tr("Gb"));
    color_list.append(tr("Gr"));
	color_list.append(tr("B"));

    //histogram A
    QVBoxLayout *histogram_layout = new QVBoxLayout;
    QHBoxLayout *color_layout = new QHBoxLayout;
    QLabel *color_title_label = new QLabel(tr("Choose color"));
    color_combobox = new QComboBox;
    color_combobox->setMaximumSize(100, 30);
	for (auto &color_name : color_list) {
        color_combobox->addItem(color_name);
	}
    color_layout->addWidget(color_title_label);
    color_layout->addWidget(color_combobox);
    connect(color_combobox, &QComboBox::currentTextChanged, this, &BlcHistogramWidget::handleSelectColor);

    r_offset_series_mean = new QtCharts::QLineSeries();
    r_offset_area_series_mean = new QtCharts::QAreaSeries(r_offset_series_mean);
    gr_offset_series_mean = new QtCharts::QLineSeries();
    gr_offset_area_series_mean = new QtCharts::QAreaSeries(gr_offset_series_mean);
    gb_offset_series_mean = new QtCharts::QLineSeries();
    gb_offset_area_series_mean = new QtCharts::QAreaSeries(gb_offset_series_mean);
    b_offset_series_mean = new QtCharts::QLineSeries();
    b_offset_area_series_mean = new QtCharts::QAreaSeries(b_offset_series_mean);
    r_offset_series_median = new QtCharts::QLineSeries();
    r_offset_area_series_median = new QtCharts::QAreaSeries(r_offset_series_median);
    gr_offset_series_median = new QtCharts::QLineSeries();
    gr_offset_area_series_median = new QtCharts::QAreaSeries(gr_offset_series_median);
    gb_offset_series_median = new QtCharts::QLineSeries();
    gb_offset_area_series_median = new QtCharts::QAreaSeries(gb_offset_series_median);
    b_offset_series_median = new QtCharts::QLineSeries();
    b_offset_area_series_median = new QtCharts::QAreaSeries(b_offset_series_median);

    r_series = new QtCharts::QLineSeries();
    gr_series = new QtCharts::QLineSeries();
    gb_series = new QtCharts::QLineSeries();
    b_series = new QtCharts::QLineSeries();
    r_area_series = new QtCharts::QAreaSeries(r_series);
    gr_area_series = new QtCharts::QAreaSeries(gr_series);
    gb_area_series = new QtCharts::QAreaSeries(gb_series);
    b_area_series = new QtCharts::QAreaSeries(b_series);

    r_area_series->setPen(QColor(255, 0, 0));
    r_area_series->setBrush(QColor(255, 0, 0, 50));
    gr_area_series->setPen(QColor(100, 255, 0));
    gr_area_series->setBrush(QColor(100, 255, 0, 50));
    gb_area_series->setPen(QColor(0, 255, 100));
    gb_area_series->setBrush(QColor(0, 255, 100, 50));
    b_area_series->setPen(QColor(0, 0, 255));
    b_area_series->setBrush(QColor(0, 0, 255, 50));

    r_offset_area_series_mean->setPen(QColor(255, 0, 255));
    r_offset_area_series_mean->setBrush(QColor(255, 0, 255, 50));
    gr_offset_area_series_mean->setPen(QColor(255, 0, 255));
    gr_offset_area_series_mean->setBrush(QColor(255, 0, 255, 50));
    gb_offset_area_series_mean->setPen(QColor(255, 0, 255));
    gb_offset_area_series_mean->setBrush(QColor(255, 0, 255, 50));
    b_offset_area_series_mean->setPen(QColor(255, 0, 255));
    b_offset_area_series_mean->setBrush(QColor(255, 0, 255, 50));
    r_offset_area_series_median->setPen(QColor(0, 255, 255));
    r_offset_area_series_median->setBrush(QColor(0, 255, 255, 50));
    gr_offset_area_series_median->setPen(QColor(0, 255, 255));
    gr_offset_area_series_median->setBrush(QColor(0, 255, 255, 50));
    gb_offset_area_series_median->setPen(QColor(0, 255, 255));
    gb_offset_area_series_median->setBrush(QColor(0, 255, 255, 50));
    b_offset_area_series_median->setPen(QColor(0, 255, 255));
    b_offset_area_series_median->setBrush(QColor(0, 255, 255, 50));

    r_area_series->setName("R histgram");
    gr_area_series->setName("Gr histgram");
    gb_area_series->setName("Gb histgram");
    b_area_series->setName("B histgram");
    r_offset_area_series_mean->setName("R offset mean");
    gr_offset_area_series_mean->setName("Gr offset mean");
    gb_offset_area_series_mean->setName("Gb offset mean");
    b_offset_area_series_mean->setName("B offset mean");
    r_offset_area_series_median->setName("R offset median");
    gr_offset_area_series_median->setName("Gr offset median");
    gb_offset_area_series_median->setName("Gb offset median");
    b_offset_area_series_median->setName("B offset median");

    r_offset_area_series_mean->setPointLabelsFormat("@xPoint");
    r_offset_area_series_mean->setPointLabelsFont(QFont("Times",6));
    r_offset_area_series_mean->setPointLabelsVisible(true);
    gr_offset_area_series_mean->setPointLabelsFormat("@xPoint");
    gr_offset_area_series_mean->setPointLabelsFont(QFont("Times",6));
    gr_offset_area_series_mean->setPointLabelsVisible(true);
    gb_offset_area_series_mean->setPointLabelsFormat("@xPoint");
    gb_offset_area_series_mean->setPointLabelsFont(QFont("Times",6));
    gb_offset_area_series_mean->setPointLabelsVisible(true);
    b_offset_area_series_mean->setPointLabelsFormat("@xPoint");
    b_offset_area_series_mean->setPointLabelsFont(QFont("Times",6));
    b_offset_area_series_mean->setPointLabelsVisible(true);
    r_offset_area_series_median->setPointLabelsFormat("@xPoint");
    r_offset_area_series_median->setPointLabelsFont(QFont("Times",6));
    r_offset_area_series_median->setPointLabelsVisible(true);
    gr_offset_area_series_median->setPointLabelsFormat("@xPoint");
    gr_offset_area_series_median->setPointLabelsFont(QFont("Times",6));
    gr_offset_area_series_median->setPointLabelsVisible(true);
    gb_offset_area_series_median->setPointLabelsFormat("@xPoint");
    gb_offset_area_series_median->setPointLabelsFont(QFont("Times",6));
    gb_offset_area_series_median->setPointLabelsVisible(true);
    b_offset_area_series_median->setPointLabelsFormat("@xPoint");
    b_offset_area_series_median->setPointLabelsFont(QFont("Times",6));
    b_offset_area_series_median->setPointLabelsVisible(true);

    r_area_series->setVisible(true);
    gr_area_series->setVisible(false);
    gb_area_series->setVisible(false);
    b_area_series->setVisible(false);
    r_offset_area_series_mean->setVisible(true);
    gr_offset_area_series_mean->setVisible(false);
    gb_offset_area_series_mean->setVisible(false);
    b_offset_area_series_mean->setVisible(false);
    r_offset_area_series_median->setVisible(true);
    gr_offset_area_series_median->setVisible(false);
    gb_offset_area_series_median->setVisible(false);
    b_offset_area_series_median->setVisible(false);

    chart = new QtCharts::QChart();
    chart->addSeries(r_area_series);
    chart->addSeries(gr_area_series);
    chart->addSeries(gb_area_series);
    chart->addSeries(b_area_series);
    chart->addSeries(r_offset_area_series_mean);
    chart->addSeries(gr_offset_area_series_mean);
    chart->addSeries(gb_offset_area_series_mean);
    chart->addSeries(b_offset_area_series_mean);
    chart->addSeries(r_offset_area_series_median);
    chart->addSeries(gr_offset_area_series_median);
    chart->addSeries(gb_offset_area_series_median);
    chart->addSeries(b_offset_area_series_median);
    chart->setTitle("color histogram");
    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setRange(0, 4095);
    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);

    histogram_layout->addLayout(color_layout);
    histogram_layout->addWidget(chartView);
    main_layout->addLayout(histogram_layout);
	setLayout(main_layout);
}

void BlcHistogramWidget::handleUpdatePoints(int *rr_hist, int *gr_hist, int *gb_hist, int *bb_hist, int r_offset_mean, int gr_offset_mean,
                    int gb_offset_mean, int b_offset_mean, int r_offset_median, int gr_offset_median, int gb_offset_median, int b_offset_median)
{
    int y_max = 1;
    r_series->clear();
    gr_series->clear();
    gb_series->clear();
    b_series->clear();
    r_offset_series_mean->clear();
    gr_offset_series_mean->clear();
    gb_offset_series_mean->clear();
    b_offset_series_mean->clear();
    r_offset_series_median->clear();
    gr_offset_series_median->clear();
    gb_offset_series_median->clear();
    b_offset_series_median->clear();

    for (int i = 0; i < BLC_BAYER_MAX_VALUE; i++) {
        if (rr_hist[i] > y_max) y_max = rr_hist[i];
        if (gr_hist[i] > y_max) y_max = gr_hist[i];
        if (gb_hist[i] > y_max) y_max = gb_hist[i];
        if (rr_hist[i] > y_max) y_max = rr_hist[i];
        *r_series << QPointF(i, rr_hist[i]);
        *gr_series << QPointF(i, gr_hist[i]);
        *gb_series << QPointF(i, gb_hist[i]);
        *b_series << QPointF(i, bb_hist[i]);
    }

    *r_offset_series_mean << QPointF(r_offset_mean, 0);
    *gr_offset_series_mean << QPointF(gr_offset_mean, 0);
    *gb_offset_series_mean << QPointF(gb_offset_mean, 0);
    *b_offset_series_mean << QPointF(b_offset_mean, 0);
    *r_offset_series_median << QPointF(r_offset_median, 0);
    *gr_offset_series_median << QPointF(gr_offset_median, 0);
    *gb_offset_series_median << QPointF(gb_offset_median, 0);
    *b_offset_series_median << QPointF(b_offset_median, 0);
    *r_offset_series_mean << QPointF(r_offset_mean, y_max);
    *gr_offset_series_mean << QPointF(gr_offset_mean, y_max);
    *gb_offset_series_mean << QPointF(gb_offset_mean, y_max);
    *b_offset_series_mean << QPointF(b_offset_mean, y_max);
    *r_offset_series_median << QPointF(r_offset_median, y_max);
    *gr_offset_series_median << QPointF(gr_offset_median, y_max);
    *gb_offset_series_median << QPointF(gb_offset_median, y_max);
    *b_offset_series_median << QPointF(b_offset_median, y_max);

    chart->axes(Qt::Vertical).first()->setRange(0, y_max);
}

void BlcHistogramWidget::handleSelectColor(const QString &text)
{
    int color_idx = color_list.indexOf(text);
    if (color_idx == 1) {
        r_area_series->hide();
        gb_area_series->show();
        gr_area_series->hide();
        b_area_series->hide();
        r_offset_area_series_mean->hide();
        gr_offset_area_series_mean->hide();
        gb_offset_area_series_mean->show();
        b_offset_area_series_mean->hide();
        r_offset_area_series_median->hide();
        gr_offset_area_series_median->hide();
        gb_offset_area_series_median->show();
        b_offset_area_series_median->hide();
    } else if (color_idx == 2) {
        r_area_series->hide();
        gb_area_series->hide();
        gr_area_series->show();
        b_area_series->hide();
        r_offset_area_series_mean->hide();
        gr_offset_area_series_mean->show();
        gb_offset_area_series_mean->hide();
        b_offset_area_series_mean->hide();
        r_offset_area_series_median->hide();
        gr_offset_area_series_median->show();
        gb_offset_area_series_median->hide();
        b_offset_area_series_median->hide();
    } else if (color_idx == 3) {
        r_area_series->hide();
        gb_area_series->hide();
        gr_area_series->hide();
        b_area_series->show();
        r_offset_area_series_mean->hide();
        gr_offset_area_series_mean->hide();
        gb_offset_area_series_mean->hide();
        b_offset_area_series_mean->show();
        r_offset_area_series_median->hide();
        gr_offset_area_series_median->hide();
        gb_offset_area_series_median->hide();
        b_offset_area_series_median->show();
    } else {
        r_area_series->show();
        gb_area_series->hide();
        gr_area_series->hide();
        b_area_series->hide();
        r_offset_area_series_mean->show();
        gr_offset_area_series_mean->hide();
        gb_offset_area_series_mean->hide();
        b_offset_area_series_mean->hide();
        r_offset_area_series_median->show();
        gr_offset_area_series_median->hide();
        gb_offset_area_series_median->hide();
        b_offset_area_series_median->hide();
    }
}

