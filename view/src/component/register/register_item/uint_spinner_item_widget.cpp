#include "uint_spinner_item_widget.hpp"
#include <QSignalBlocker>
#include "global_data.hpp"

constexpr int SPINNER_MAX_WIDTH = 100;

UIntSpinnerItemWidget::UIntSpinnerItemWidget(QString module, QString title, QWidget *parent) :
    BaseItemWidget(module, title, parent)
{
    initValueWidget();
}

void UIntSpinnerItemWidget::setRange(uint32_t min, uint32_t max)
{
    int half = max / 2 - min / 2 + 1;
    offset = min + half;
    int start = 0 - half;
    int end = max - offset;

    const QSignalBlocker spinner_block(spinner);
    spinner->setRange(start, end);
    spinner->setOffset(offset);
    const QSignalBlocker slider_block(slider);
    slider->setRange(start, end);
}

void UIntSpinnerItemWidget::setValue(uint32_t value)
{
    int v = i(value - offset);

    spinner->setValue(v);
    slider->setValue(v);
}

void UIntSpinnerItemWidget::initValueWidget()
{
    spinner = new UIntSpinBox();
    slider = new QSlider(Qt::Horizontal);
    slider->setMaximumWidth(SPINNER_MAX_WIDTH);
    slider->installEventFilter(this);

    spinner->setRange(0, 255);
    slider->setRange(0, 255);

    connect(spinner, QOverload<int>::of(&UIntSpinBox::valueChanged),
            this, &UIntSpinnerItemWidget::spinnerValueChanged);
    connect(spinner, &QSpinBox::editingFinished,
            this, &UIntSpinnerItemWidget::getFinishedValue);

    connect(slider, &QSlider::valueChanged,
            this, &UIntSpinnerItemWidget::sliderValueChanged);
    connect(slider, &QSlider::sliderReleased,
            this, &UIntSpinnerItemWidget::getFinishedValue);

    connect(this, &UIntSpinnerItemWidget::sigValueChanged,
            this, &UIntSpinnerItemWidget::handleUiChanged);

    value_layout->addWidget(spinner);
    value_layout->addWidget(slider);
}

void UIntSpinnerItemWidget::valueChanged(int value)
{
    uint v = value < 0? offset - qAbs(value) : offset + value;
    qDebug("SpinnerItemWidget value changed [%u]", v);
    syncValueChanged(v);
    updateTooltip();
}

void UIntSpinnerItemWidget::spinnerValueChanged(int value)
{
    const QSignalBlocker slider_block(slider);
    slider->setValue(value);
}

void UIntSpinnerItemWidget::sliderValueChanged(int value)
{
    const QSignalBlocker spinner_block(spinner);
    spinner->setValue(value);
}

void UIntSpinnerItemWidget::getFinishedValue()
{
    valueChanged(spinner->value());
}

void UIntSpinnerItemWidget::setParam(QVariant params)
{
    QMap<QString, QVariant> m = params.toMap();
    if (m.count() == 0) {
        return;
    }

    if (m.contains("min") && m.contains("max")) {
        min = m["min"].toUInt();
        max = m["max"].toUInt();

        setRange(min, max);
        updateTooltip();
    }
}

QVariant UIntSpinnerItemWidget::getData() const
{
    return slider->value() < 0 ? offset - qAbs(slider->value()) : offset + slider->value();
}

void UIntSpinnerItemWidget::setData(QVariant data, bool sync_to_global)
{
    if (data == getData()) {
        return;
    }

    if (sync_to_global == false) {
        spinner->blockSignals(true);
        slider->blockSignals(true);
    }

    setValue(data.toUInt());
    updateTooltip();

    if (sync_to_global == false) {
        spinner->blockSignals(false);
        slider->blockSignals(false);
    }
}

void UIntSpinnerItemWidget::updateTooltip()
{
    value_widget->setToolTip(QString::number(getData().toUInt()) + " [" +
                             QString::number(this->min) + "~" +
                             QString::number(this->max) + "]");
}

bool UIntSpinnerItemWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == slider) {
        if (event->type() == QEvent::Wheel) {
            // if not focus, skip slider wheel event
            if ((slider->hasFocus()) || (spinner->hasFocus())) {
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

void UIntSpinnerItemWidget::setChmod(int mod)
{
    BaseItemWidget::setChmod(mod);

    bool disabled = (mod == MOD_RO);
    spinner->setReadOnly(disabled);
    slider->setDisabled(disabled);
}

void UIntSpinnerItemWidget::setDisplayIntBase(int base)
{
    BaseItemWidget::setDisplayIntBase(base);

    if (base == 16)
    {
        spinner->setPrefix("0x");
    }
    spinner->setDisplayIntegerBase(base);
}
