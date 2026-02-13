#ifndef GAMMA_CURVE_ITEM_WIDGET_HPP
#define GAMMA_CURVE_ITEM_WIDGET_HPP

#include "base_item_widget.hpp"
#include "register_table_window.hpp"
#include "gamma_curve.hpp"

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QJsonObject>

enum GAMMA_CURVE_TYPE {
    MAP_CURVE_TYPE_T,
    GAMMA_CURVE_TYPE_T,
    GAMMA_CURVE_TYPE_LV,
    GAMMA_CURVE_TYPE_RGB
};

class GammaCurveItemWidget : public BaseItemWidget
{
    Q_OBJECT

public:
    explicit GammaCurveItemWidget(QString module, QString title, GAMMA_CURVE_TYPE type, QWidget *parent = nullptr);
    ~GammaCurveItemWidget();

    virtual void refreshData(const SecKeyType &reg_section_key) override;
    virtual void setParam(QVariant param) override;
    virtual void setData(QVariant data, bool sync_to_global = false) override;
    virtual QVariant getData() const override;
    virtual void setRegSectionKeyList(QList<SecKeyType> &&reg_section_key_list) override;
    virtual void setOpType(QString op_type) override;
    virtual void setChmod(int mod) override;

protected:
    virtual void initValueWidget();
    virtual void retranslateUi() override;

private slots:
    void handleShowWindow();
    void handleWritePageData();

private:
    QPushButton *push_button;
    RegisterTableWindow *register_window;
    GammaCurve *gamma_curve;
    QWidget *pageWidget;
    QStringList horizontal_headers;
    QStringList vertical_headers;
    GAMMA_CURVE_TYPE type;
};

#endif
