#ifndef BINARY_ITEM_WIDGET_H
#define BINARY_ITEM_WIDGET_H

#include "base_item_widget.hpp"
#include <QWidget>
#include <QButtonGroup>
#include <QRadioButton>

class BinaryItemWidget : public BaseItemWidget
{
    Q_OBJECT

public:
    explicit BinaryItemWidget(QString module, QString title, QWidget *parent = nullptr);
    ~BinaryItemWidget();

    virtual void setParam(QVariant params) override;
    virtual void setData(QVariant data, bool sync_to_global = false) override;
    virtual QVariant getData() const override;
    virtual void setChmod(int mod) override;

protected:
    virtual void retranslateUi() override;

private slots:
    void valueChanged(int id);

private:
    void setValue(bool enabled);
    void initValueWidget();
    void syncModuleValueChanged(bool enabled);

private:
    int current_id{2};
    QButtonGroup *btn_group;
    QRadioButton *enable_btn;
    QRadioButton *disable_btn;
    QString text_enable{"Enable"};
    QString text_disable{"Disable"};
    static QMap<QString, QList<class BinaryItemWidget *>> map_module_binaryItems;
};

#endif // BINARY_ITEM_WIDGET_H
