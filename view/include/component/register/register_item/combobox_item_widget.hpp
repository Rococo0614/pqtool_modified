#ifndef COMBOBOX_ITEM_WIDGET_H
#define COMBOBOX_ITEM_WIDGET_H

#include "base_item_widget.hpp"

#include <QWidget>
#include <QComboBox>

class ComboBoxItemWidget : public BaseItemWidget
{
    Q_OBJECT

public:
    explicit ComboBoxItemWidget(QString module, QString title, QWidget *parent = nullptr);

    virtual void setParam(QVariant params) override;
    virtual void setData(QVariant data, bool sync_to_global = false) override;
    virtual QVariant getData() const override;
    virtual void setChmod(int mod) override;
    void getValue(int *value);
    void setValue(int value);
    int getComboboxCount() { return combobox->count(); };

private:
    void addValue(QString text, int value);
    void initValueWidget();

private slots:
    void valueChanged(int index);

private:
    QComboBox *combobox;
    QMap<int, int> *valueToIndexMap;
};

#endif // COMBOBOX_ITEM_WIDGET_H
