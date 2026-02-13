#ifndef UNIVERSAL_MATRIX_ITEM_WIDGET_HPP
#define UNIVERSAL_MATRIX_ITEM_WIDGET_HPP

#include "base_item_widget.hpp"
#include "universal_table.hpp"
#include "register_table_window.hpp"

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QJsonObject>

class UniversalMatrixItemWidget : public BaseItemWidget
{
    Q_OBJECT

public:
    explicit UniversalMatrixItemWidget(QString module, QString title, QWidget *parent = nullptr);
    explicit UniversalMatrixItemWidget(QString module, QString title, int, QWidget *parent = nullptr);
    ~UniversalMatrixItemWidget();

    virtual void refreshData(const SecKeyType &reg_section_key) override;
    virtual void setParam(QVariant param) override;
    virtual void setData(QVariant data, bool sync_to_global = false) override;
    virtual QVariant getData() const override;
    virtual void setRegSectionKeyList(QList<SecKeyType> &&reg_section_key_list) override;
    virtual void addRegSectionKey(QString section, QString key) override;
    virtual void setOpType(QString op_type) override;
    virtual void setChmod(int mod) override;
    QString getAutoType() {return auto_type;};
    void setTableIndex(int index) {table_index  = index;};
    QVariant getDataByIndex(int index);

public slots:
    void handleLoadManuData(QVariant);

protected:
    virtual void initValueWidget();
    virtual void retranslateUi() override;

protected slots:
    void handleShowWindow();
    void handleWritePageData();
    void handleTableValueChanged();

signals:
    void sigTableValueChanged(QVariant);

protected:
    QPushButton *push_button;
    RegisterTableWindow *register_window{nullptr};
    UniversalTable *univ_table;
    QStringList horizontal_headers;
    QStringList vertical_headers;
    QString module_name;
    QString title;
    QString auto_type;
    int table_index;
};

#endif // UNIVERSAL_MATRIX_ITEM_WIDGET_HPP
