#include "double_slider_calibration_item_widget.hpp"
#include <QVariantMap>

DoubleSliderCaliItemWidget::DoubleSliderCaliItemWidget(QString title, QWidget *parent) :
    BaseCalibrationItemWidget(title, parent)
{
    initValueWidget();
    retranslateUi();
}

void DoubleSliderCaliItemWidget::initValueWidget()
{
    double_spinner = new QDoubleSpinBox();
    double_spinner->setSingleStep(step);
    double_spinner->setSingleStep(0.1);
    slider = new QSlider(Qt::Horizontal);

    value_layout->addWidget(double_spinner);
    value_layout->addWidget(slider);

    connect(slider, &QSlider::valueChanged,
            this, &DoubleSliderCaliItemWidget::sliderValueChanged);
    connect(double_spinner, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, QOverload<double>::of(&DoubleSliderCaliItemWidget::spinnerValueChanged));
}

QVariant DoubleSliderCaliItemWidget::getData()
{
    return (slider->value()/scaler);
}

void DoubleSliderCaliItemWidget::setData(QVariant value)
{
    slider->setValue(static_cast<int>(value.toDouble() * scaler));
}

void DoubleSliderCaliItemWidget::setRange(double min, double max)
{
    this->max = max;
    this->min = min;
    double_spinner->setRange(min, max);
    slider->setRange(static_cast<int>(min * scaler), static_cast<int>(max * scaler));
}

void DoubleSliderCaliItemWidget::setSpinnerSingleStep(double step)
{
    this->step = step;
    this->scaler = 1.0 / step;
    double_spinner->setSingleStep(step);
    slider->setRange(static_cast<int>(min * scaler), static_cast<int>(max * scaler));
}

void DoubleSliderCaliItemWidget::updateTooltip()
{
    value_widget->setToolTip(QString::number(getData().toDouble()) + " [" +
                             QString::number(this->slider->minimum()/scaler) + "~" +
                             QString::number(this->slider->maximum()/scaler) + "]");
}

void DoubleSliderCaliItemWidget::setSpinnerValue(double value)
{
    double_spinner->setValue(value);
}

void DoubleSliderCaliItemWidget::setSliderValue(int value)
{
    slider->setValue(value);
}

void DoubleSliderCaliItemWidget::sliderValueChanged(int value)
{
    setSpinnerValue(value / scaler);
    // updateTooltip();
}

void DoubleSliderCaliItemWidget::spinnerValueChanged(double value)
{
    setSliderValue(static_cast<int>(value * scaler));
    // updateTooltip();
}

void DoubleSliderCaliItemWidget::retranslateUi()
{
}

void DoubleSliderCaliItemWidget::setEnabled(bool enabled)
{
    double_spinner->setEnabled(enabled);
    slider->setEnabled(enabled);
}

