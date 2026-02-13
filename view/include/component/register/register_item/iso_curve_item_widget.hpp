#ifndef ISO_CURVE_ITEM_WIDGET_HPP
#define ISO_CURVE_ITEM_WIDGET_HPP

#include "base_item_widget.hpp"
#include "iso_table.hpp"
#include "register_table_window.hpp"
#include "iso_chart_view.hpp"
#include "iso_curve.hpp"
#include "iso_table.hpp"

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QJsonObject>

class IsoCurveItemWidget : public BaseItemWidget
{
    Q_OBJECT

public:
    explicit IsoCurveItemWidget(QString module, QString title, QWidget *parent = nullptr);
    ~IsoCurveItemWidget();

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
    virtual void handleUpdateTheme(bool dark_theme_enabled) override;

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
    QList<QVariant> edit_labels;
    IsoCurve *curve{nullptr};
    IsoTable *iso_table{nullptr};
};
#endif // ISO_CURVE_ITEM_WIDGET_HPP
