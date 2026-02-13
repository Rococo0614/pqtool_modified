#include "checkbox_calibration_item_widget.hpp"
#include "global_data.hpp"
#include <QVariantMap>

CheckboxCaliItemWidget::CheckboxCaliItemWidget(QString title, QWidget *parent) :
    BaseCalibrationItemWidget(title, parent)
{
    initValueWidget();
    retranslateUi();
}

void CheckboxCaliItemWidget::initValueWidget()
{
    this->value_checkbox = new QCheckBox;
    value_layout->addWidget(value_checkbox, Qt::AlignLeft);
}

QVariant CheckboxCaliItemWidget::getData()
{
    return (Qt::CheckState::Checked == value_checkbox->checkState()) ? 1 : 0;
}

void CheckboxCaliItemWidget::setData(QVariant data)
{
    if (data == 1) {
        value_checkbox->setChecked(true);
    } else {
        value_checkbox->setChecked(false);
    }
}

void CheckboxCaliItemWidget::retranslateUi()
{
}
