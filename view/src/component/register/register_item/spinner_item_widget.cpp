#include "spinner_item_widget.hpp"
#include <QSignalBlocker>
#include "global_data.hpp"

constexpr int SPINNER_MAX_WIDTH = 100;

SpinnerItemWidget::SpinnerItemWidget(QString module, QString title, QWidget *parent) :
    BaseItemWidget(module, title, parent)
{
    initValueWidget();
}

void SpinnerItemWidget::setRange(int min, int max)
{
    const QSignalBlocker spinner_block(spinner);
    spinner->setRange(min, max);
    const QSignalBlocker slider_block(slider);
    slider->setRange(min, max);
}

void SpinnerItemWidget::setValue(int value)
{
    spinner->setValue(value);
    slider->setValue(value);
}

void SpinnerItemWidget::initValueWidget()
{
    spinner = new QSpinBox();
    slider = new QSlider(Qt::Horizontal);
    slider->setMaximumWidth(SPINNER_MAX_WIDTH);
    slider->installEventFilter(this);
    spinner->installEventFilter(this);

    spinner->setRange(0, 255);
    slider->setRange(0, 255);

    connect(spinner, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SpinnerItemWidget::spinnerValueChanged);

    connect(slider, &QSlider::valueChanged,
            this, &SpinnerItemWidget::sliderValueChanged);

    connect(this, &SpinnerItemWidget::sigValueChanged,
            this, &SpinnerItemWidget::handleUiChanged);

    value_layout->addWidget(spinner);
    value_layout->addWidget(slider);
}

void SpinnerItemWidget::valueChanged(int value)
{
    syncValueChanged(value);
    updateTooltip();
}

int SpinnerItemWidget::checkValue(int value)
{
    int upper_bound = 0;
    int lower_bound = 0;
    int fixed_value = value;

    if (!lt_key.first.isNull() && !lt_key.first.isEmpty()) {
        upper_bound = GlobalData::getInstance()->getRegValue(lt_key.first, lt_key.second).toInt();
        if (fixed_value > upper_bound) {
            fixed_value = upper_bound;
        }
    }

    if (!mt_key.first.isNull() && !mt_key.first.isEmpty()) {
        lower_bound = GlobalData::getInstance()->getRegValue(mt_key.first, mt_key.second).toInt();
        if (fixed_value < lower_bound) {
            fixed_value = lower_bound;
        }
    }

    return fixed_value;
}

void SpinnerItemWidget::spinnerValueChanged(int value)
{
    int fixed_value = checkValue(value);
    const QSignalBlocker slider_block(slider);
    slider->setValue(fixed_value);

    const QSignalBlocker spinner_block(spinner);
    spinner->setValue(fixed_value);
    valueChanged(value);
}

void SpinnerItemWidget::sliderValueChanged(int value)
{
    int fixed_value = checkValue(value);

    const QSignalBlocker spinner_block(spinner);
    spinner->setValue(fixed_value);

    const QSignalBlocker slider_block(slider);
    slider->setValue(fixed_value);
    valueChanged(value);
}

void SpinnerItemWidget::setParam(QVariant params)
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

    QString reg_section = m[KEY_REG_SECTION].toString();
    if (m.contains("lt_key"))
    {
        QString key = m["lt_key"].toString();
        if (!key.isEmpty()) {
            SecKeyType sk(reg_section, key);
            this->setLTKey(sk);
        }
    }

    if (m.contains("mt_key"))
    {
        QString key = m["mt_key"].toString();
        if (!key.isEmpty()) {
            SecKeyType sk(reg_section, key);
            this->setMTKey(sk);
        }
    }
}

QVariant SpinnerItemWidget::getData() const
{
    return spinner->value();
}

void SpinnerItemWidget::setData(QVariant data, bool sync_to_global)
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

void SpinnerItemWidget::updateTooltip()
{
    value_widget->setToolTip(QString::number(getData().toInt()) + " [" +
                             QString::number(this->spinner->minimum()) + "~" +
                             QString::number(this->spinner->maximum()) + "]");
}

bool SpinnerItemWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == slider) {
        if (event->type() == QEvent::Wheel) {
            // if not focus, skip slider wheel event
            if ((slider->hasFocus()) || (spinner->hasFocus())) {
                return false;
            }
            return true;
        } else if (event->type() == QEvent::MouseButtonRelease) {
            sigValueChangedCpt(getData());
            return false;
        } else {
            return false;
        }
    } else if (obj == spinner) {
        if (event->type() == QEvent::FocusOut) {
            sigValueChangedCpt(getData());
        }
        return false;
    } else {
        // pass the event on to the parent class
        return BaseItemWidget::eventFilter(obj, event);
    }
}

void SpinnerItemWidget::setChmod(int mod)
{
    BaseItemWidget::setChmod(mod);

    bool disabled = (mod == MOD_RO);
    spinner->setReadOnly(disabled);
    slider->setDisabled(disabled);
}

void SpinnerItemWidget::setDisplayIntBase(int base)
{
    BaseItemWidget::setDisplayIntBase(base);

    if (base == 16)
    {
        spinner->setPrefix("0x");
    }
    spinner->setDisplayIntegerBase(base);
}
