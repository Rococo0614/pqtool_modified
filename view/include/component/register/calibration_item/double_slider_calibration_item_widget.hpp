#ifndef SLIDER_CALIBRATION_ITEM_WIDGET_H
#define SLIDER_CALIBRATION_ITEM_WIDGET_H

#include "base_calibration_item_widget.hpp"
#include <QSlider>
#include <QDoubleSpinBox>

class DoubleSliderCaliItemWidget : public BaseCalibrationItemWidget
{
    Q_OBJECT

public:
    explicit DoubleSliderCaliItemWidget(QString title, QWidget *parent = nullptr);

    void setData(QVariant data) override;
    void setRange(double min, double max);
    void setSpinnerSingleStep(double step);
    QVariant getData() override;
    void setEnabled(bool enabled);

protected:
    virtual void retranslateUi() override;

private:
    void initValueWidget();
    void updateTooltip();
    void setSpinnerValue(double value);
    void setSliderValue(int value);

private slots:
    void sliderValueChanged(int value);
    void spinnerValueChanged(double value);

private:
    double step = 0.1;
    double scaler = 10;
    double max = 1;
    double min = 0;
    QDoubleSpinBox *double_spinner;
    QSlider *slider;
};

#endif // SLIDER_CALIBRATION_ITEM_WIDGET_H
