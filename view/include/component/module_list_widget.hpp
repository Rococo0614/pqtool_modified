#ifndef MODULE_LIST_WIDGET_H
#define MODULE_LIST_WIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include "module_define.hpp"
#include <QVBoxLayout>

class QTreeWidgetItem;

class ModuleListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ModuleListWidget(QWidget *parent = nullptr);
    ModuleListWidget(QStringList list, QWidget *parent = nullptr);
    void initUI();
    void updateModuleListVisible();
    void updateModuleList(QList<QTreeWidgetItem *> item_list,  QStringList ready_list);
    void selectedMenuItem(QString name);
    void setItemHide(QString name, bool hidden);
    bool isItemHide(QString name);
    void insertWidget(int index, QWidget *widget);

private:
    QVBoxLayout *main_layout;
    QTreeWidget *tree_widget;
    QStringList ready_list;
    QList<QTreeWidgetItem *> item_list;

signals:
    void selectModule(QString name);

private slots:
    void fireSelectModuleSignal(QTreeWidgetItem *item, int column);
};

#endif // MODULE_LIST_WIDGET_H
