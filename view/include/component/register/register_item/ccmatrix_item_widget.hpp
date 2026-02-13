#ifndef CC_MATRIX_ITEM_WIDGET_H
#define CC_MATRIX_ITEM_WIDGET_H

#include "base_item_widget.hpp"
#include "ccm_table.hpp"
#include "foldable_register_table_window.hpp"
#include "register_table.hpp"
#include "register_table_window.hpp"
#include "register_chart_table.hpp"
#include "line_chart_view.hpp"

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>

class CCMatrixItemWidget : public BaseItemWidget
{
    Q_OBJECT

public:
    explicit CCMatrixItemWidget(QString module, QString title, LineChartView *chart = nullptr, QWidget *parent = nullptr);

    virtual void setParam(QVariant params) override;
    virtual void setData(QVariant data, bool sync_to_global = false) override;
    virtual QVariant getData() const override;
    virtual void refreshData(const SecKeyType &reg_section_key) override;
    virtual void setRegSectionKeyList(QList<SecKeyType> &&reg_section_key_list) override;
    virtual void addRegSectionKey(QString section, QString key) override;
    virtual void setRegValueType(QString reg_value_type) override;
    virtual void setOpType(QString op_type) override;
    virtual void setChmod(int mod) override;

    void setDoubleValidator(double min, double max);

protected:
    virtual void initValueWidget();
    virtual void retranslateUi() override;

private:
    QStringList getMatrixHorizontalHeaders();
    QStringList getMatrixVerticalHeaders();

private slots:
    void handleShowWindow();
    void handleWritePageData();

private:
    LineChartView *chart{nullptr};
    QPushButton *push_button;
    QStringList horizontal_headers;
    QStringList vertical_headers;
    CcmTable *color_correction_table{nullptr};
    RegisterChartTable *chart_reg_table{nullptr};
    FoldableRegisterTableWindow *foldable_register_window;
    bool is_chart_value_double;
    double validator_min;
    double validator_max;
};

#endif // CC_MATRIX_ITEM_WIDGET_H
