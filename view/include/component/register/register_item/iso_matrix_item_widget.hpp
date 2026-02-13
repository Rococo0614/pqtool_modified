#ifndef ISO_MATRIX_ITEM_WIDGET_HPP
#define ISO_MATRIX_ITEM_WIDGET_HPP

#include "base_item_widget.hpp"
#include "iso_table.hpp"
#include "register_table_window.hpp"

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QJsonObject>

class IsoMatrixItemWidget : public BaseItemWidget
{
    Q_OBJECT

public:
    explicit IsoMatrixItemWidget(QString module, QString title, QWidget *parent = nullptr);
    ~IsoMatrixItemWidget();

    virtual void refreshData(const SecKeyType &reg_section_key) override;
    virtual void setParam(QVariant params) override;
    virtual void setData(QVariant data, bool sync_to_global = false) override;
    virtual QVariant getData() const override;
    virtual void setRegSectionKeyList(QList<SecKeyType> &&reg_section_key_list) override;
    virtual void addRegSectionKey(QString section, QString key) override;
    virtual void setOpType(QString op_type) override;
    virtual void setRegValueType(QString reg_value_type) override;
    virtual void setValueMappingVec(QVector<QVariantList> &&value_mapping_vec) override;
    virtual void setChmod(int mod) override;

protected:
    virtual void initValueWidget();
    virtual void retranslateUi() override;
    void updateValueType();

private slots:
    void handleShowWindow();
    void handleWritePageData();

private:
    QPushButton *push_button;
    RegisterTableWindow *register_window;
    IsoTable *iso_table;
    QStringList horizontal_headers;
    QStringList vertical_headers;
};

#endif // ISO_MATRIX_ITEM_WIDGET_HPP
