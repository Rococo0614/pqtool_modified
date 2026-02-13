#include "double_spinner_item_widget.hpp"
#include <QSignalBlocker>
#include "global_data.hpp"

DoubleSpinnerItemWidget::DoubleSpinnerItemWidget(QString module, QString title, QWidget *parent) :
    BaseItemWidget(module, title, parent)
{
    initValueWidget();
}

void DoubleSpinnerItemWidget::setRange(double min, double max)
{
    double_spinner->setRange(min, max);
    slider->setRange(static_cast<int>(min * 100), static_cast<int>(max * 100));
}

void DoubleSpinnerItemWidget::setValue(double value)
{
    double_spinner->setValue(value);
    slider->setValue(static_cast<int>(value * 100));
}

void DoubleSpinnerItemWidget::initValueWidget()
{
    slider = new QSlider(Qt::Horizontal);
    slider->installEventFilter(this);
    double_spinner = new QDoubleSpinBox();

    slider->setRange(0, 255);
    double_spinner->setRange(0, 1);
    double_spinner->setSingleStep(0.01);

    connect(slider, &QSlider::valueChanged,
            this, QOverload<int>::of(&DoubleSpinnerItemWidget::valueChanged));

    connect(double_spinner, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, QOverload<double>::of(&DoubleSpinnerItemWidget::valueChanged));

    connect(this, &DoubleSpinnerItemWidget::sigValueChanged,
            this, &DoubleSpinnerItemWidget::handleUiChanged);

    value_layout->addWidget(double_spinner);
    value_layout->addWidget(slider);
}

void DoubleSpinnerItemWidget::valueChanged(int value)
{
    qDebug("SpinnerItemWidget int value changed [%d]", value);
    QSignalBlocker slider_block(slider);
    QSignalBlocker spinner_block(double_spinner);

    setValue(value / 100.0);

    emit sigValueChanged(value / 100.0);
    syncValueChanged(value / 100.0);
}

void DoubleSpinnerItemWidget::valueChanged(double value)
{
    qDebug("SpinnerItemWidget double value changed [%f]", value);
    QSignalBlocker slider_block(slider);
    QSignalBlocker spinner_block(double_spinner);

    setValue(value);
    syncValueChanged(value);
}

void DoubleSpinnerItemWidget::setParam(QVariant params)
{
    QMap<QString, QVariant> m = params.toMap();
    if (m.count() == 0) {
        return;
    }

    if (m.contains("min") && m.contains("max")) {
        int min = m["min"].toInt();
        int max = m["max"].toInt();

        setRange(min, max);
    }
}

QVariant DoubleSpinnerItemWidget::getData() const
{
    return double_spinner->value();
}

void DoubleSpinnerItemWidget::setData(QVariant data, bool sync_to_global)
{
    if (data == getData()) {
        return;
    }

    if (sync_to_global == false) {
        double_spinner->blockSignals(true);
        slider->blockSignals(true);
    }

    double value = data.toDouble() + 0.005;
    setValue(value);

    if (sync_to_global == false) {
        double_spinner->blockSignals(false);
        slider->blockSignals(false);
    }
}

bool DoubleSpinnerItemWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == slider) {
        if (event->type() == QEvent::Wheel) {
            // if not focus, skip slider wheel event
            if ((slider->hasFocus()) || (double_spinner->hasFocus())) {
                return false;
            }

            return true;
        } else {
            return false;
        }
    } else {
        // pass the event on to the parent class
        return BaseItemWidget::eventFilter(obj, event);
    }
}

void DoubleSpinnerItemWidget::setChmod(int mod)
{
    BaseItemWidget::setChmod(mod);

    bool disabled = (mod == MOD_RO);
    double_spinner->setReadOnly(disabled);
    slider->setDisabled(disabled);
}
