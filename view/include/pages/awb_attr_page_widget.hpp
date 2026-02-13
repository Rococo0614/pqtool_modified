#ifndef AWB_ATTR_PAGE_WIDGET_H
#define AWB_ATTR_PAGE_WIDGET_H

#include "base_page_widget.hpp"

class AWBAttrPageWidget : public BasePageWidget
{
    Q_OBJECT

public:
    explicit AWBAttrPageWidget(QWidget *parent = nullptr);

protected:
    void initLeftArea() override;
    void initCenterArea() override;
    QList<QPointF> initPoints(double coefficient);

private:
    QWidget *initCalibrationWidget();
    LineChartView *cv;
};

#endif // AWB_ATTR_PAGE_WIDGET_H
