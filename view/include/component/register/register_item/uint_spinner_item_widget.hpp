#ifndef UINT_SPINNER_ITEM_WIDGET_H
#define UINT_SPINNER_ITEM_WIDGET_H

#include "base_item_widget.hpp"
#include <QWidget>
#include <QSpinBox>
#include <QSlider>
#include "uint_spinbox.hpp"

class UIntSpinnerItemWidget : public BaseItemWidget
{
    Q_OBJECT

public:
    explicit UIntSpinnerItemWidget(QString module, QString title, QWidget *parent = nullptr);

    virtual void setParam(QVariant params) override;
    virtual void setData(QVariant data, bool sync_to_global = false) override;
    virtual QVariant getData() const override;
    void setRange(uint32_t min, uint32_t max);
    void updateTooltip();
    virtual void setChmod(int mod) override;
    virtual void setDisplayIntBase(int base) override;

private:
    void setValue(uint32_t value);
    void initValueWidget();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void valueChanged(int value);
    void spinnerValueChanged(int value);
    void sliderValueChanged(int value);
    void getFinishedValue();

private:
    UIntSpinBox *spinner;
    QSlider *slider;
    uint32_t min{0};
    uint32_t max{4096};
    uint offset{0};

    inline unsigned int u(int i) const
    {
        return *reinterpret_cast<unsigned int*>(&i);
    }

    inline int i(unsigned int u) const
    {
        return *reinterpret_cast<int *>(&u);
    }
};

#endif // UINT_SPINNER_ITEM_WIDGET_H
