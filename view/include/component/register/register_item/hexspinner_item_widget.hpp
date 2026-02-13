#ifndef HEX_SPINNER_ITEM_WIDGET_H
#define HEX_SPINNER_ITEM_WIDGET_H

#include "base_item_widget.hpp"
#include <QWidget>
#include <QSpinBox>
#include <QSlider>
#include "hexspinbox.hpp"

class HexSpinnerItemWidget : public BaseItemWidget
{
    Q_OBJECT

public:
    explicit HexSpinnerItemWidget(QString module, QString title, QWidget *parent = nullptr);

    virtual void setParam(QVariant params) override;
    virtual void setData(QVariant data, bool sync_to_global = false) override;
    virtual QVariant getData() const override;
    void setRange(int min, int max);
    void updateTooltip();
    virtual void setChmod(int mod) override;

private:
    void setValue(int value);
    void initValueWidget();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void valueChanged(int value);
    void spinnerValueChanged(int value);
    void sliderValueChanged(int value);

private:
    HexSpinBox *spinner;
    QSlider *slider;
};

#endif // HEX_SPINNER_ITEM_WIDGET_H
