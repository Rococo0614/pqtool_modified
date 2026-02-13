#ifndef UNIVERSAL_MATRIX_CURVE_ITEM_WIDGET_HPP
#define UNIVERSAL_MATRIX_CURVE_ITEM_WIDGET_HPP

#include "universal_matrix_item_widget.hpp"
#include "table_curve.hpp"


enum TABLE_CURVE_TYPE {
    SIMPLE_TYPE,
    METHOD_TYPE
};

class UniversalMatrixCurveItemWidget : public UniversalMatrixItemWidget
{
    Q_OBJECT

public:
    explicit UniversalMatrixCurveItemWidget(QString module, QString title, TABLE_CURVE_TYPE type, QWidget *parent = nullptr);
    ~UniversalMatrixCurveItemWidget();

    virtual void setParam(QVariant param) override;
    virtual void setRegSectionKeyList(QList<SecKeyType> &&reg_section_key_list) override;
    virtual void setData(QVariant data, bool sync_to_global = false) override;

private:
    virtual void initValueWidget() override;
    virtual void retranslateUi() override;

private:
    SimpleTableCurve *table_curve;
    QWidget *pageWidget;
    TABLE_CURVE_TYPE type;
};

#endif
