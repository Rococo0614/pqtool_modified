#ifndef DOUBLE_SPINNER_ITEM_WIDGET_H
#define DOUBLE_SPINNER_ITEM_WIDGET_H

#include "base_item_widget.hpp"
#include <QWidget>
#include <QSpinBox>
#include <QSlider>
#include <QDoubleSpinBox>

class DoubleSpinnerItemWidget : public BaseItemWidget
{
    Q_OBJECT

public:
    explicit DoubleSpinnerItemWidget(QString module, QString title, QWidget *parent = nullptr);

    void setRange(double min, double max);
    virtual void setParam(QVariant params) override;
    virtual QVariant getData() const override;
    virtual void setData(QVariant data, bool sync_to_global = false) override;
    virtual void setChmod(int mod) override;

private:
    void setValue(double value);

protected:
    virtual void initValueWidget();
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void valueChanged(int value);
    void valueChanged(double value);

private:
    QDoubleSpinBox *double_spinner;
    QSlider *slider;
};

#endif // DOUBLE_SPINNER_ITEM_WIDGET_H
