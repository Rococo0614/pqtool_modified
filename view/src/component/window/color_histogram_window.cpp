#include "color_histogram_window.hpp"
#include <QtCharts/QChart>
#include <QtCharts/QChartView>

const int PIXEL_MAX_VALUE = 255;

ColorHistogramWindow::ColorHistogramWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setFixedSize(COLOR_HIST_WINDOW_WIDTH, COLOR_HIST_WINDOW_HEIGHT);

    initUI();
    retranslateUi();
}

ColorHistogramWindow::~ColorHistogramWindow() {}

void ColorHistogramWindow::initUI()
{
    main_widget = new QWidget;
    main_layout = new QGridLayout;

    QWidget *color_widget = new QWidget;
    QHBoxLayout *color_layout = new QHBoxLayout;
    QLabel *color_title_label = new QLabel(tr("Choose color"));
    color_widget->setLayout(color_layout);
    color_combobox = new QComboBox;
    color_list.append(tr("R,G,B"));
    color_list.append(tr("R"));
    color_list.append(tr("G"));
    color_list.append(tr("B"));
    for (auto &color_name : color_list) {
        color_combobox->addItem(color_name);
    }
    color_layout->addWidget(color_title_label);
    color_layout->addWidget(color_combobox);
    connect(color_combobox, &QComboBox::currentTextChanged, this, &ColorHistogramWindow::handleSelectColor);

    r_series = new QtCharts::QLineSeries();
    g_series = new QtCharts::QLineSeries();
    b_series = new QtCharts::QLineSeries();

    r_area_series = new QtCharts::QAreaSeries(r_series);
    g_area_series = new QtCharts::QAreaSeries(g_series);
    b_area_series = new QtCharts::QAreaSeries(b_series);

    r_area_series->setPen(QColor(255, 0, 0));
    r_area_series->setBrush(QColor(255, 0, 0, 50));
    g_area_series->setPen(QColor(0, 255, 0));
    g_area_series->setBrush(QColor(0, 255, 0, 50));
    b_area_series->setPen(QColor(0, 0, 255));
    b_area_series->setBrush(QColor(0, 0, 255, 50));

    chart = new QtCharts::QChart();
    chart->addSeries(r_area_series);
    chart->addSeries(g_area_series);
    chart->addSeries(b_area_series);
    chart->setTitle("Color Histogram");
    chart->createDefaultAxes();
    chart->axes(Qt::Horizontal).first()->setRange(0, PIXEL_MAX_VALUE);
    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);

    main_layout->addWidget(color_widget);
    main_layout->addWidget(chartView);
    setCentralWidget(main_widget);
    centralWidget()->setLayout(main_layout);
}

void ColorHistogramWindow::changeEvent(QEvent *event)
{
    switch(event->type()) {
    case QEvent::LanguageChange:
        retranslateUi();
        break;
    default:
        break;
    }
    QMainWindow::changeEvent(event);
}

void ColorHistogramWindow::setImage(QPixmap *image)
{
    this->image = image;
}

void ColorHistogramWindow::retranslateUi()
{
    setWindowTitle(tr("Color Histogram"));
}

void ColorHistogramWindow::handleUpdatePoints(const QPixmap *image)
{
    if (image->isNull()) {
        return;
    }

    r_series->clear();
    g_series->clear();
    b_series->clear();

    int y_max = 1;
    QImage img = image->toImage();

    r_count_map.clear();
    g_count_map.clear();
    b_count_map.clear();

    for (int i = 0; i < img.width(); i++) {
        for (int j = 0; j < img.height(); j++) {
            QColor color = img.pixel(i, j);
            r_count_map[color.red()]++;
            g_count_map[color.green()]++;
            b_count_map[color.blue()]++;
            if (r_count_map[color.red()] > y_max) y_max = r_count_map[color.red()];
            if (g_count_map[color.green()] > y_max) y_max = g_count_map[color.green()];
            if (b_count_map[color.blue()] > y_max) y_max = b_count_map[color.blue()];
        }
    }

    for (int i = 0; i < (PIXEL_MAX_VALUE + 1); i++) {
        *r_series << QPointF(i, r_count_map[i]);
        *g_series << QPointF(i, g_count_map[i]);
        *b_series << QPointF(i, b_count_map[i]);
    }

    chart->axes(Qt::Vertical).first()->setRange(0, y_max);
}

void ColorHistogramWindow::handleSelectColor(const QString &text)
{
    int color_idx = color_list.indexOf(text);
    if (color_idx == 1) {
        r_area_series->show();
        g_area_series->hide();
        b_area_series->hide();
    } else if (color_idx == 2) {
        r_area_series->hide();
        g_area_series->show();
        b_area_series->hide();
    } else if (color_idx == 3) {
        r_area_series->hide();
        g_area_series->hide();
        b_area_series->show();
    } else {
        r_area_series->show();
        g_area_series->show();
        b_area_series->show();
    }
}


void ColorHistogramWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        this->close();
    }
}
