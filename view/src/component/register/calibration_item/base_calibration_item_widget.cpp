#include "base_calibration_item_widget.hpp"

#include <QtCore>
#include <QHBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include "global_data.hpp"

const int MAX_LINE_EDIT_WIDTH = 150;

BaseCalibrationItemWidget::BaseCalibrationItemWidget(QString title, QWidget *parent) :
    QWidget(parent)
{
    layout = new QHBoxLayout(this);
    title_label = new QLabel(title);
    value_widget = new QWidget();
    value_layout = new QHBoxLayout(value_widget);
    value_widget->setMaximumWidth(MAX_LINE_EDIT_WIDTH);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(title_label);
    layout->addWidget(value_widget);
    value_layout->setContentsMargins(0, 0, 0, 0);
}

void BaseCalibrationItemWidget::changeEvent(QEvent *event)
{
    switch(event->type()) {
    case QEvent::LanguageChange:
        retranslateUi();
        break;
    default:
        break;
    }
    QWidget::changeEvent(event);
}

void BaseCalibrationItemWidget::retranslateUi()
{
}

void BaseCalibrationItemWidget::setKey(QString key)
{
    this->key = key;
}

QString BaseCalibrationItemWidget::getKey()
{
    return this->key;
}

void BaseCalibrationItemWidget::setDesp(QString desp)
{
    this->desp = desp;
}

QString BaseCalibrationItemWidget::getDesp()
{
    return this->desp;
}

void BaseCalibrationItemWidget::setParam(QVariant)
{
}

QVariant BaseCalibrationItemWidget::getData()
{
    return QVariant("");
}

void BaseCalibrationItemWidget::setData(QVariant)
{
}

void BaseCalibrationItemWidget::setEnable(bool enable)
{
    this->value_widget->setEnabled(enable);
}
