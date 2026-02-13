#ifndef SPINNER_ITEM_WIDGET_H
#define SPINNER_ITEM_WIDGET_H

#include "base_item_widget.hpp"
#include <QWidget>
#include <QSpinBox>
#include <QSlider>

class SpinnerItemWidget : public BaseItemWidget
{
    Q_OBJECT

public:
    explicit SpinnerItemWidget(QString module, QString title, QWidget *parent = nullptr);

    virtual void setParam(QVariant params) override;
    virtual void setData(QVariant data, bool sync_to_global = false) override;
    virtual QVariant getData() const override;
    void setRange(int min, int max);
    void updateTooltip();
    virtual void setChmod(int mod) override;
    virtual void setDisplayIntBase(int base) override;

signals:
    void sigValueChangedCpt(QVariant value);

private:
    void setValue(int value);
    void initValueWidget();
    int checkValue(int value);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void valueChanged(int value);
    void spinnerValueChanged(int value);
    void sliderValueChanged(int value);

private:
    QSpinBox *spinner;
    QSlider *slider;
};

#endif // SPINNER_ITEM_WIDGET_H
