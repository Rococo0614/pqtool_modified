#include "combobox_calibration_item_widget.hpp"
#include "global_data.hpp"
#include <QVariantMap>

ComboboxCaliItemWidget::ComboboxCaliItemWidget(QString title, QWidget *parent) :
    BaseCalibrationItemWidget(title, parent)
{
    initValueWidget();
    retranslateUi();
}

void ComboboxCaliItemWidget::initValueWidget()
{
    this->value_combobox = new QComboBox;

    value_layout->addWidget(value_combobox);
    connect(value_combobox, QOverload<int>::of(&QComboBox::activated), this, &ComboboxCaliItemWidget::slotComboxSelected);
}

void ComboboxCaliItemWidget::addItem(QString str)
{
    this->value_combobox->addItem(str);
}

void ComboboxCaliItemWidget::addItem(QString str, QVariant v)
{
    this->value_combobox->addItem(str, v);
}

void ComboboxCaliItemWidget::setCurrentIndex(int idx)
{
    this->value_combobox->setCurrentIndex(idx);
}

int ComboboxCaliItemWidget::currentIndex()
{
    return value_combobox->currentIndex();
}

QVariant ComboboxCaliItemWidget::getData()
{
    return value_combobox->currentText();
}

void ComboboxCaliItemWidget::setData(QVariant)
{
}

void ComboboxCaliItemWidget::retranslateUi()
{
}

void ComboboxCaliItemWidget::slotComboxSelected(int index)
{
    if (value_combobox->itemText(index) == CUSTOMIZE_ITEM) {
        emit sigCustomizeSelected();
    }
}
