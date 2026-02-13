#ifndef UNIVERSAL_CURVE_ITEM_WIDGET_HPP
#define UNIVERSAL_CURVE_ITEM_WIDGET_HPP

#include "base_item_widget.hpp"
#include "universal_table.hpp"
#include "register_table_window.hpp"
#include "universal_curve.hpp"

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QJsonObject>

class UniversalCurveItemWidget : public BaseItemWidget
{
    Q_OBJECT

public:
    explicit UniversalCurveItemWidget(QString module, QString title, QWidget *parent = nullptr);
    ~UniversalCurveItemWidget();

    virtual void refreshData(const SecKeyType &reg_section_key) override;
    virtual void setParam(QVariant param) override;
    virtual void setDataJsonrpc(QString path, QVariant data, bool sync_to_global = false) override;
    virtual QVariant getDataJsonrpc(QString path) const override;
    virtual void setRegSectionKeyList(QList<SecKeyType> &&reg_section_key_list) override;
    virtual void addRegSectionKey(QString section, QString key) override;
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
    UniversalTable *univ_table;
    UniversalCurve *univ_curve;
    QStringList horizontal_headers;
    QStringList vertical_headers;
    QString module_name;
};

#endif // UNIVERSAL_CURVE_ITEM_WIDGET_HPP
