#include "module_list_widget.hpp"
#include "global_data.hpp"

const int WIDGET_MAX_WIDTH = 220;

ModuleListWidget::ModuleListWidget(QWidget *parent) : QWidget(parent)
{
    initUI();
}

ModuleListWidget::ModuleListWidget(QStringList list, QWidget *parent) : QWidget(parent),
    ready_list(list)
{
    initUI();
}

void ModuleListWidget::initUI()
{
    main_layout = new QVBoxLayout(this);
    tree_widget = new QTreeWidget();

    main_layout->addWidget(tree_widget);
    tree_widget->setHeaderHidden(true);
    tree_widget->setColumnCount(1);

    for (int i = 0; i < ready_list.size(); i++) {
        QString name = ready_list[i];
        QTreeWidgetItem *item = new QTreeWidgetItem({name});

        item->setData(0, Qt::UserRole, name);
        item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
        tree_widget->addTopLevelItem(item);
        item_list.append(item);
    }

    this->setMaximumWidth(WIDGET_MAX_WIDTH);

    connect(tree_widget, &QTreeWidget::itemClicked,
            this, &ModuleListWidget::fireSelectModuleSignal);
}

void ModuleListWidget::updateModuleListVisible()
{
    for (int i = 0; i < item_list.size(); i++) {
        QString name = item_list[i]->data(0, Qt::UserRole).toString();
        if (!ready_list.contains(name)) {
            if (GlobalData::getInstance()->getPageMode() == 0) {
                if (!item_list[i]->isHidden())
                    item_list[i]->setHidden(true);
            } else if (GlobalData::getInstance()->getPageMode() == 1){
                if (item_list[i]->isHidden())
                    item_list[i]->setHidden(false);
            }
        }
    }
}

void ModuleListWidget::fireSelectModuleSignal(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);
    QString name = item->data(0, Qt::UserRole).toString();
    emit selectModule(name);
}

void ModuleListWidget::updateModuleList(QList<QTreeWidgetItem *> item_list,  QStringList ready_list)
{
    tree_widget->clear();
    this->ready_list.clear();
    this->item_list.clear();
    this->ready_list = ready_list;
    this->item_list = item_list;

    tree_widget->addTopLevelItems(this->item_list);
    updateModuleListVisible();
}

void ModuleListWidget::selectedMenuItem(QString name)
{
    QTreeWidgetItem *item = NULL;
    int index = 0;

    for (; index < item_list.size(); index++) {
        QString module = item_list[index]->data(0, Qt::UserRole).toString();
        if (name == module) {
            item = item_list[index];
            break;
        }
    }
    if (index != item_list.size()) {
        tree_widget->setCurrentItem(item);
    }
}

void ModuleListWidget::setItemHide(QString name, bool isHidden)
{
    for (int i = 0; i < item_list.size(); i++) {
        QString module = item_list[i]->data(0, Qt::UserRole).toString();
        if (name == module) {
            item_list[i]->setHidden(isHidden);
            break;
        }
    }
}

bool ModuleListWidget::isItemHide(QString name)
{
    for (int i = 0; i < item_list.size(); i++) {
        QString id = item_list[i]->data(0, Qt::UserRole).toString();
        if (id == name) {
            return item_list[i]->isHidden();
        }
    }

    return false;
}

void ModuleListWidget::insertWidget(int index, QWidget *widget)
{
    if (widget == nullptr || main_layout == nullptr) {
        return;
    }
    main_layout->insertWidget(index, widget);
}
