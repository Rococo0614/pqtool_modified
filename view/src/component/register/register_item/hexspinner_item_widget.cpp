#include "hexspinner_item_widget.hpp"
#include <QSignalBlocker>
#include "global_data.hpp"

constexpr int SPINNER_MAX_WIDTH = 100;

HexSpinnerItemWidget::HexSpinnerItemWidget(QString module, QString title, QWidget *parent) :
    BaseItemWidget(module, title, parent)
{
    initValueWidget();
}

void HexSpinnerItemWidget::setRange(int min, int max)
{
    const QSignalBlocker spinner_block(spinner);
    spinner->setRange(min, max);
    const QSignalBlocker slider_block(slider);
    slider->setRange(min, max);
}

void HexSpinnerItemWidget::setValue(int value)
{
    spinner->setValue(value);
    slider->setValue(value);
}

void HexSpinnerItemWidget::initValueWidget()
{
    spinner = new HexSpinBox();
    slider = new QSlider(Qt::Horizontal);
    slider->setMaximumWidth(SPINNER_MAX_WIDTH);
    slider->installEventFilter(this);

    spinner->setRange(0, 255);
    slider->setRange(0, 255);

    connect(spinner, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &HexSpinnerItemWidget::spinnerValueChanged);
    connect(slider, &QSlider::valueChanged,
            this, &HexSpinnerItemWidget::sliderValueChanged);
    connect(this, &HexSpinnerItemWidget::sigValueChanged,
            this, &HexSpinnerItemWidget::handleUiChanged);

    value_layout->addWidget(spinner);
    value_layout->addWidget(slider);
}

void HexSpinnerItemWidget::valueChanged(int value)
{
    qDebug("SpinnerItemWidget value changed [%d]", value);
    syncValueChanged(value);
    updateTooltip();
}

void HexSpinnerItemWidget::spinnerValueChanged(int value)
{
    const QSignalBlocker slider_block(slider);
    slider->setValue(value);
    valueChanged(value);
}

void HexSpinnerItemWidget::sliderValueChanged(int value)
{
    const QSignalBlocker spinner_block(spinner);
    spinner->setValue(value);
    valueChanged(value);
}

void HexSpinnerItemWidget::setParam(QVariant params)
{
    QMap<QString, QVariant> m = params.toMap();
    if (m.count() == 0) {
        return;
    }

    if (m.contains("min") && m.contains("max")) {
        int min = m["min"].toInt();
        int max = m["max"].toInt();

        setRange(min, max);
        updateTooltip();
    }
}

QVariant HexSpinnerItemWidget::getData() const
{
    return spinner->value();
}

void HexSpinnerItemWidget::setData(QVariant data, bool sync_to_global)
{
    if (data == getData()) {
        return;
    }

    if (sync_to_global == false) {
        spinner->blockSignals(true);
        slider->blockSignals(true);
    }

    setValue(data.toInt());
    updateTooltip();

    if (sync_to_global == false) {
        spinner->blockSignals(false);
        slider->blockSignals(false);
    }
}

void HexSpinnerItemWidget::updateTooltip()
{
    value_widget->setToolTip(QString::number(getData().toInt()) + " [" +
                             QString::number(this->spinner->minimum()) + "~" +
                             QString::number(this->spinner->maximum()) + "]");
}

bool HexSpinnerItemWidget::eventFilter(QObject *obj, QEvent *event)
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

void HexSpinnerItemWidget::setChmod(int mod)
{
    BaseItemWidget::setChmod(mod);

    bool disabled = (mod == MOD_RO);
    spinner->setReadOnly(disabled);
    slider->setDisabled(disabled);
}
