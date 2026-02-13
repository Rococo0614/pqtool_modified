#ifndef REGESTER_LIST_WIDGET_H
#define REGESTER_LIST_WIDGET_H

#include "base_page_widget.hpp"

#include <QWidget>
#include <QVector>
#include <QString>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHash>
#include <QLineEdit>
#include <QCheckBox>
#include <QList>

class RegisterCheckBox;

class RegisterPageWidget : public BasePageWidget
{
    Q_OBJECT

public:
    explicit RegisterPageWidget(QWidget *parent = nullptr);

protected:
    virtual void initLeftArea() override;
    void initCenterArea() override;
    virtual void retranslateUi() override;

private:
    void updateCheckbox(int value);

private slots:
    void itemSelectionChanged();

private:
    QTableWidget *register_table_widget;
    QLabel *desp_label;
    RegisterCheckBox *checkbox_edit;
};

#endif // REGESTER_LIST_WIDGET_H
