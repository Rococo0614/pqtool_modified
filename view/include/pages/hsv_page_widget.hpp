#ifndef HSV_PAGE_WIDGET_H
#define HSV_PAGE_WIDGET_H

#include "base_page_widget.hpp"
#include "binary_item_widget.hpp"
#include "combobox_item_widget.hpp"
#include "matrix_item_widget.hpp"
#include "base_page_widget.hpp"
#include "double_spinner_item_widget.hpp"
#include "matrix_item_widget.hpp"
#include "line_chart_view.hpp"
#include "hue_line_chart_view.hpp"

#include <QWidget>
#include <QRadioButton>
#include <QSpinBox>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>

class HSVPageWidget : public BasePageWidget
{
    Q_OBJECT

public:
    explicit HSVPageWidget(QWidget *parent = nullptr);

    virtual void updateUI() override;
    virtual void writeData() override;

protected:
    void initLeftArea() override;
    void initCenterArea() override;
    void paintEvent(QPaintEvent *event) override;

private:
    void writeToIni() override;
    QHBoxLayout *addLabel(QWidget *widget, QString title);
    QGridLayout *initItemWidget();
    void initChart();
    QWidget *initHueWidget(QSpinBox *&v_by_h_spinner, QDoubleSpinBox *&v_by_h_shift_spinner, LineChartView *chart);
    void initConnection();
    QList<QPointF> initHuePoints();
    QList<QPointF> initSbySPoints();
    QList<QPointF> intHbyHPoints();
    void setSpinnerValue(QSpinBox *s, qreal value);
    void setDoubleSpinnerValue(QDoubleSpinBox *s, qreal value);

private:
    const int POINT_HUE_NUM = (48 + 1);

    LineChartView *s_by_s_chart;
    LineChartView *h_by_h_chart;
    HueLineChartView *v_by_h_chart;
    HueLineChartView *s_by_h_chart;

    QSpinBox *v_by_h_spinner;
    QDoubleSpinBox *v_by_h_shift_spinner;
    QSpinBox *s_by_h_spinner;
    QDoubleSpinBox *s_by_h_shift_spinner;

    BinaryItemWidget *hsv_s_tune_enable;
    BinaryItemWidget *hsv_h_tune_enable;
    BinaryItemWidget *hsv_s_gain_enable;
    BinaryItemWidget *hsv_v_gain_enable;

    MatrixItemWidget *hsv_s_curve;
    MatrixItemWidget *hsv_h_curve;
    MatrixItemWidget *hsv_s_gain_curve;
    MatrixItemWidget *hsv_v_gain_curve;
};

#endif // HSV_PAGE_WIDGET_H
