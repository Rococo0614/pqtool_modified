#ifndef CHECKBOX_CALIBRATION_ITEM_WIDGET_H
#define CHECKBOX_CALIBRATION_ITEM_WIDGET_H

#include "base_calibration_item_widget.hpp"
#include <QWidget>
#include <QCheckBox>

class CheckboxCaliItemWidget : public BaseCalibrationItemWidget
{
    Q_OBJECT

public:
    explicit CheckboxCaliItemWidget(QString title, QWidget *parent = nullptr);

    void setData(QVariant data) override;
    QVariant getData() override;

protected:
    virtual void retranslateUi() override;

private:
    void setEnabled(bool enabled);
    void initValueWidget();

private:
    QCheckBox *value_checkbox;
};

#endif // CHECKBOX_CALIBRATION_ITEM_WIDGET_H
