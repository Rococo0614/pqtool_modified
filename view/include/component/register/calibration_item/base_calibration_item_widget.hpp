#ifndef BASE_CALIBRATION_ITEM_WIDGET_HPP
#define BASE_CALIBRATION_ITEM_WIDGET_HPP

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVariant>

#include "i_observer.hpp"

class BaseCalibrationItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BaseCalibrationItemWidget(QString title, QWidget *parent = nullptr);

    void setKey(QString key);
    QString getKey();
    void setDesp(QString desp);
    QString getDesp();

    virtual void setParam(QVariant params);
    virtual QVariant getData();
    virtual void setData(QVariant data);
    virtual void setEnable(bool enable);

protected:
    void changeEvent(QEvent *event) override;
    virtual void retranslateUi();

protected:
    QHBoxLayout *layout;
    QLabel *title_label;
    QWidget *value_widget;
    QHBoxLayout *value_layout;
    QString desp;
    QString key;
};

#endif // BASE_CALIBRATION_ITEM_WIDGET_HPP
