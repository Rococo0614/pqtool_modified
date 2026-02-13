#include "lineedit_calibration_item_widget.hpp"
#include "global_data.hpp"
#include <QVariantMap>

LineEditCaliItemWidget::LineEditCaliItemWidget(QString title, QWidget *parent) :
    BaseCalibrationItemWidget(title, parent)
{
    initValueWidget();
    retranslateUi();
}

void LineEditCaliItemWidget::initValueWidget()
{
    this->value_line_edit = new QLineEdit;

    value_layout->addWidget(value_line_edit);
}

QVariant LineEditCaliItemWidget::getData()
{
    return value_line_edit->text();
}

void LineEditCaliItemWidget::setData(QVariant data)
{
    value_line_edit->setText(data.toString());
}

void LineEditCaliItemWidget::retranslateUi()
{
}

void LineEditCaliItemWidget::setReadOnly(bool enable)
{
    value_line_edit->setReadOnly(enable);
}
