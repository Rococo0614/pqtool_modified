#include "hsv_page_widget.hpp"
#include "module_define.hpp"
#include "global_data.hpp"
#include <QtCore>
#include <QSignalBlocker>

#include <QtCharts/QSplineSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QToolTip>
#include <QHBoxLayout>
#include "ini_helper.hpp"
#include "utils.hpp"

using namespace QtCharts;

HSVPageWidget::HSVPageWidget(QWidget *parent) : BasePageWidget(MODULE_NAME_HSV_OLD, parent)
{
    initData();
    initUI();
}

void HSVPageWidget::initLeftArea()
{
}

QHBoxLayout *HSVPageWidget::addLabel(QWidget *widget, QString title)
{
    QLabel *label = new QLabel(title);
    QHBoxLayout *h = new QHBoxLayout;

    h->addWidget(label);
    h->addWidget(widget);

    return h;
}

void HSVPageWidget::writeToIni()
{
    IniHelper::getInstance()->writeReg(hsv_s_tune_enable->getSectionKeyList().first().first,
                                       hsv_s_tune_enable->getSectionKeyList().first().second, hsv_s_tune_enable->getData().toString());
    IniHelper::getInstance()->writeReg(hsv_h_tune_enable->getSectionKeyList().first().first,
                                       hsv_h_tune_enable->getSectionKeyList().first().second, hsv_h_tune_enable->getData().toString());
    IniHelper::getInstance()->writeReg(hsv_s_gain_enable->getSectionKeyList().first().first,
                                       hsv_s_gain_enable->getSectionKeyList().first().second, hsv_s_gain_enable->getData().toString());
    IniHelper::getInstance()->writeReg(hsv_v_gain_enable->getSectionKeyList().first().first,
                                       hsv_v_gain_enable->getSectionKeyList().first().second, hsv_v_gain_enable->getData().toString());

    SecKeyType value_section_key = s_by_s_chart->getSectionKey();
    QString value_str = s_by_s_chart->getInterpolationDataString();
    IniHelper::getInstance()->writeReg(value_section_key.first, value_section_key.second, value_str);

    value_str = h_by_h_chart->getInterpolationDataString();
    value_section_key = h_by_h_chart->getSectionKey();
    IniHelper::getInstance()->writeReg(value_section_key.first, value_section_key.second, value_str);

    value_str = v_by_h_chart->getInterpolationDataString();
    value_section_key = v_by_h_chart->getSectionKey();
    IniHelper::getInstance()->writeReg(value_section_key.first, value_section_key.second, value_str);

    value_str = s_by_h_chart->getInterpolationDataString();
    value_section_key = s_by_h_chart->getSectionKey();
    IniHelper::getInstance()->writeReg(value_section_key.first, value_section_key.second, value_str);
}

QWidget *HSVPageWidget::initHueWidget(QSpinBox *& hue_spinner, QDoubleSpinBox *&hue_shift_spinner, LineChartView *chart)
{
    hue_spinner = new QSpinBox;
    hue_shift_spinner = new QDoubleSpinBox;

    hue_spinner->setRange(0, POINT_HUE_NUM - 1);
    hue_shift_spinner->setRange(0.0625, 8);
    hue_shift_spinner->setSingleStep(0.0025);

    QVBoxLayout *v = new QVBoxLayout;
    v->setAlignment(Qt::AlignBottom);
    v->addLayout(addLabel(hue_spinner, tr("Hue") + ":"));
    v->addLayout(addLabel(hue_shift_spinner, tr("Shift") + ":"));

    QHBoxLayout *h = new QHBoxLayout;
    h->addLayout(v);
    h->addWidget(chart);

    QWidget *w = new QWidget();
    w->setLayout(h);
    return w;
}

void HSVPageWidget::initConnection()
{
    connect(v_by_h_spinner, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int hue) {
        qreal y = v_by_h_chart->getYByX(hue);
        setDoubleSpinnerValue(v_by_h_shift_spinner, y);
    });

    connect(v_by_h_shift_spinner, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [=](double shift) {
        int hue = v_by_h_spinner->value();
        v_by_h_chart->updateYByX(hue, shift);
    });

    connect(v_by_h_chart, &LineChartView::sigFocusPointUpdated,
            [=](qreal x, qreal y) {
        setSpinnerValue(v_by_h_spinner, x);
        setDoubleSpinnerValue(v_by_h_shift_spinner, y);
    });

    connect(s_by_h_spinner, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int hue) {
        qreal y = s_by_h_chart->getYByX(hue);
        setDoubleSpinnerValue(s_by_h_shift_spinner, y);
    });

    connect(s_by_h_shift_spinner, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [=](double shift) {
        int hue = s_by_h_spinner->value();
        s_by_h_chart->updateYByX(hue, shift);
    });

    connect(s_by_h_chart, &LineChartView::sigFocusPointUpdated,
            [=](qreal x, qreal y) {
        setSpinnerValue(s_by_h_spinner, x);
        setDoubleSpinnerValue(s_by_h_shift_spinner, y);
    });
}

void HSVPageWidget::setSpinnerValue(QSpinBox *s, qreal value)
{
    QSignalBlocker signal_blocker(s);
    s->setValue(static_cast<int>(value));
}

void HSVPageWidget::setDoubleSpinnerValue(QDoubleSpinBox *s, qreal value)
{
    QSignalBlocker signal_blocker(s);
    s->setValue(static_cast<double>(value));
}

void HSVPageWidget::initCenterArea()
{
    initChart();
    QLayout *item_layout = initItemWidget();

    QWidget *y_by_h = initHueWidget(v_by_h_spinner, v_by_h_shift_spinner, v_by_h_chart);
    QWidget *s_by_h = initHueWidget(s_by_h_spinner, s_by_h_shift_spinner, s_by_h_chart);

    QTabWidget *chart_tab = new QTabWidget();
    chart_tab->addTab(y_by_h, "V by H");
    chart_tab->addTab(s_by_h, "S by H");

    QGridLayout *grid_layout = new QGridLayout;
    grid_layout->addLayout(item_layout, 0, 0, 1, 2);
    grid_layout->addWidget(s_by_s_chart, 1, 0);
    grid_layout->addWidget(h_by_h_chart, 1, 1);
    grid_layout->addWidget(chart_tab, 2, 0, 1, 2);
    grid_layout->setRowStretch(1, 1);
    grid_layout->setRowStretch(2, 1);

    main_layout->addLayout(grid_layout);

    initConnection();
}

void HSVPageWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void HSVPageWidget::updateUI()
{
}

void HSVPageWidget::writeData()
{
    writeToIni();
}

QGridLayout *HSVPageWidget::initItemWidget()
{
    QString module = property("module_id").toString();
    hsv_s_tune_enable = new BinaryItemWidget(module, "HsvStuneEnable");
    hsv_h_tune_enable = new BinaryItemWidget(module, "HsvHtuneEnable");
    hsv_s_gain_enable = new BinaryItemWidget(module, "HsvSGainEnable");
    hsv_v_gain_enable = new BinaryItemWidget(module, "HsvVGainEnable");

    hsv_s_tune_enable->addRegSectionKey("isp_hsv", "reg_hsv_stune_enable");
    hsv_h_tune_enable->addRegSectionKey("isp_hsv", "reg_hsv_htune_enable");
    hsv_s_gain_enable->addRegSectionKey("isp_hsv", "reg_hsv_hsgain_enable");
    hsv_v_gain_enable->addRegSectionKey("isp_hsv", "reg_hsv_hvgain_enable");

    hsv_s_curve = new MatrixItemWidget(module, "HsvSCurve", s_by_s_chart);
    hsv_h_curve = new MatrixItemWidget(module, "HsvHCurve", h_by_h_chart);
    hsv_s_gain_curve = new MatrixItemWidget(module, "HsvSGainCurve", s_by_h_chart);
    hsv_v_gain_curve = new MatrixItemWidget(module, "HsvVGainCurve", v_by_h_chart);

    hsv_s_curve->addRegSectionKey("isp_hsv", "reg_hsv_s_lut");
    hsv_h_curve->addRegSectionKey("isp_hsv", "reg_hsv_h_lut");
    hsv_s_gain_curve->addRegSectionKey("isp_hsv", "reg_hsv_sgain_lut");
    hsv_v_gain_curve->addRegSectionKey("isp_hsv", "reg_hsv_vgain_lut");

    hsv_s_gain_curve->setDoubleValidator(0.0625, 8.0);
    hsv_v_gain_curve->setDoubleValidator(0.0625, 8.0);

    QGridLayout *item_layout = new QGridLayout();
    item_layout->setColumnStretch(0, 1);
    item_layout->setColumnStretch(1, 1);
    item_layout->addWidget(hsv_s_tune_enable, 0, 0);
    item_layout->addWidget(hsv_h_tune_enable, 1, 0);
    item_layout->addWidget(hsv_s_gain_enable, 2, 0);
    item_layout->addWidget(hsv_v_gain_enable, 3, 0);
    item_layout->addWidget(hsv_s_curve, 0, 1);
    item_layout->addWidget(hsv_h_curve, 1, 1);
    item_layout->addWidget(hsv_s_gain_curve, 2, 1);
    item_layout->addWidget(hsv_v_gain_curve, 3, 1);

    return item_layout;
}

void HSVPageWidget::initChart()
{
    v_by_h_chart = new HueLineChartView();
    v_by_h_chart->initVByH();
    v_by_h_chart->setPoints(0, initHuePoints(), false);
    v_by_h_chart->addRegSectionKey("isp_hsv", "reg_hsv_vgain_lut");
    v_by_h_chart->setBackgroundImageType(1);

    s_by_h_chart = new HueLineChartView();
    s_by_h_chart->initSByH();
    s_by_h_chart->setPoints(0, initHuePoints(), false);
    s_by_h_chart->addRegSectionKey("isp_hsv", "reg_hsv_sgain_lut");
    s_by_h_chart->setBackgroundImageType(2);

    s_by_s_chart = new LineChartView(16);
    s_by_s_chart->setTitle("S By S");
    s_by_s_chart->initSByS();
    s_by_s_chart->setPoints(0, initSbySPoints(), false);
    s_by_s_chart->addRegSectionKey("isp_hsv", "reg_hsv_s_lut");

    h_by_h_chart = new LineChartView(32);
    h_by_h_chart->setTitle("H by H");
    h_by_h_chart->initHByH();
    h_by_h_chart->setPoints(0, intHbyHPoints(), false);
    h_by_h_chart->addRegSectionKey("isp_hsv", "reg_hsv_h_lut");
}

QList<QPointF> HSVPageWidget::initHuePoints()
{
    QList<QPointF> points;
    int point_num = POINT_HUE_NUM;

    for (int i = 0; i < point_num; i++) {
        int x = i;
        int y = 1;

        points.append(QPointF(x, y));
    }

    return points;
}

QList<QPointF> HSVPageWidget::intHbyHPoints()
{
    QList<QPointF> points;
    int point_num = 25;

    for (int i = 0; i < point_num; i++) {
        int x = i * 32;
        int y = i * 100;

        points.append(QPointF(x, y));
    }

    return points;
}

QList<QPointF> HSVPageWidget::initSbySPoints()
{
    QList<QPointF> points;
    int point_num = 33;

    for (int i = 0; i < point_num; i++) {
        int x = i * 16;
        int y = i * 100;

        points.append(QPointF(x, y));
    }

    return points;
}
