#ifndef LINE_EDIT_CALIBRATION_ITEM_WIDGET_H
#define LINE_EDIT_CALIBRATION_ITEM_WIDGET_H

#include "base_calibration_item_widget.hpp"
#include <QWidget>
#include <QLineEdit>

class LineEditCaliItemWidget : public BaseCalibrationItemWidget
{
    Q_OBJECT

public:
    explicit LineEditCaliItemWidget(QString title, QWidget *parent = nullptr);

    virtual void setData(QVariant data) override;
    virtual QVariant getData() override;
    void setReadOnly(bool enable);

protected:
    virtual void retranslateUi() override;

private:
    void initValueWidget();

private:
    QLineEdit *value_line_edit;
};

#endif // LINE_EDIT_CALIBRATION_ITEM_WIDGET_H
