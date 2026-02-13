#ifndef HUE_LINE_CHART_VIEW_HPP
#define HUE_LINE_CHART_VIEW_HPP

#include "line_chart_view.hpp"

class HueLineChartView : public LineChartView
{
    Q_OBJECT

public:
    explicit HueLineChartView(QWidget *parent = nullptr);

    void initSByH();
    void initVByH();
    void setBackgroundImageType(int type);
    qreal getYByX(qreal x);
    void updateYByX(qreal x, qreal new_y);
    virtual void refreshData(const SecKeyType &reg_section_key) override;

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    virtual QString getTooltipText(const QPointF &point) override;
    virtual void updateSeriesPoint(QtCharts::QLineSeries *series, int index, const QPointF &point) override;

private:
    void initHue();
    QImage *getSbyHBackgroundImage();
    QImage *getVbyHBackgroundImage();

private:
    int background_image_type;
};

#endif // HUE_LINE_CHART_VIEW_HPP
