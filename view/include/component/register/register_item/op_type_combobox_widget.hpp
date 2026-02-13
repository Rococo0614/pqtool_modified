#ifndef OP_TYPE_COMBOBOX_WIDGET_HPP
#define OP_TYPE_COMBOBOX_WIDGET_HPP

#include "base_item_widget.hpp"
#include <QWidget>
#include <QComboBox>

enum class OP_TYPE_IDX {
    AUTO,
    MANUAL
};

const QMap<OP_TYPE_IDX, QString> config_map = {
    {OP_TYPE_IDX::AUTO, "OP_TYPE_AUTO"},
    {OP_TYPE_IDX::MANUAL, "OP_TYPE_MANUAL"},
};

class OpTypeComboBoxWidget : public BaseItemWidget
{
    Q_OBJECT

public:
    explicit OpTypeComboBoxWidget(QString module, QString title = "OpType", QWidget *parent = nullptr);

    void setConfigSection(QString section);
    static QString convertOpTypeName(OP_TYPE_IDX idx);
    virtual QVariant getData() const override;
    //virtual QJsonObject getJsonData() const override;
    virtual void setChmod(int mod) override;
    virtual void setData(QVariant, bool sync_to_global = false) override;

private:
    void addValue(QString text, int value);
    void setValue(int value);
    void initValueWidget();

private slots:
    void valueChanged(int index);

private:
    QComboBox *combobox;
    QMap<int, int> *valueToIndexMap;
    QString config_section;
};

#endif // OP_TYPE_COMBOBOX_WIDGET_HPP
