#include "calibration_iso_list_lblc_widget.hpp"

#include <QMenuBar>
#include <QLabel>
#include <QHeaderView>
#include <QCheckBox>
#include <QLineEdit>

CalibrationISOLBlcListWidget::CalibrationISOLBlcListWidget(QWidget *parent) : QWidget(parent)
{
    initUI();
    retranslateUi();
}

void CalibrationISOLBlcListWidget::initUI()
{
    main_layout = new QVBoxLayout(this);

    iso_table_widget = new QTableWidget(0, 5);
    iso_table_widget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    main_layout->addWidget(iso_table_widget);
    iso_table_widget->clearContents();
}

void CalibrationISOLBlcListWidget::retranslateUi()
{
    iso_table_widget->setHorizontalHeaderLabels({tr("Del"), tr("Group"), tr("Name"), tr("ISO"), tr("Sel")});
}

bool CalibrationISOLBlcListWidget::addTableItem(ISOItem *item)
{
    if (item == nullptr) {
        return false;
    }

    int group = getNextGroup();
    if (group < 0) {
        return false;
    }

    int index = iso_item_list.size();
    if (index > MAX_LBLC_ISO_ITEM_NUM) {
        return false;
    }

    item->setGroup(group);

    this->iso_item_list.append(item);

    iso_table_widget->setRowCount(iso_item_list.size());

    QPushButton *del_btn = new QPushButton(tr("X"));
    del_btn->setMaximumWidth(30);
    iso_table_widget->setCellWidget(index, 0, del_btn);
    connect(del_btn, &QPushButton::clicked, [=]() {
       int idx = iso_item_list.indexOf(item);
       iso_table_widget->removeRow(idx);
       iso_item_list.removeAt(idx);
    });

    QTableWidgetItem *group_item = new QTableWidgetItem(QString::number(item->getGroup()));
    iso_table_widget->setItem(index, 1, group_item);

    QTableWidgetItem *name_item = new QTableWidgetItem(item->getName());
    iso_table_widget->setItem(index, 2, name_item);

    QLineEdit *iso_edit = new QLineEdit(QString::number(item->getISO()));
    iso_edit->setFrame(false);
    iso_edit->setValidator(new QIntValidator(0, 20000));
    iso_table_widget->setCellWidget(index, 3, iso_edit);
    connect(iso_edit, &QLineEdit::textChanged, [=](QString text) {
        int iso = text.toInt();
        item->setISO(iso);
        emit item->displayStateChanged(item->getDisplay());
    });

    QCheckBox *sel_checkbox = new QCheckBox;
    sel_checkbox->setCheckState(item->getDisplay());
    iso_table_widget->setCellWidget(index, 4, sel_checkbox);
    connect(sel_checkbox, &QCheckBox::stateChanged, [=](int state) {
       int idx = iso_item_list.indexOf(item);
       this->iso_item_list[idx]->setDisplay(static_cast<Qt::CheckState>(state));
    });

    connect(item, &ISOItem::displayStateChanged, [=] (Qt::CheckState state) {
        if (state == Qt::CheckState::Checked) {
            clearOtherItemDisplay(item);
        }
        sel_checkbox->setCheckState(state);
    });

    return true;
}

QList<ISOItem *> CalibrationISOLBlcListWidget::getISOList()
{
    return this->iso_item_list;
}

QList<ISOItem *> CalibrationISOLBlcListWidget::getDisplayISOList()
{
    QList<ISOItem *> list;
    for (int i = 0; i < iso_item_list.size(); i++) {
        if (iso_item_list[i]->getDisplay() == Qt::CheckState::Checked) {
            list.append(iso_item_list[i]);
        }
    }

    return list;
}

int CalibrationISOLBlcListWidget::getNextGroup()
{
    int group = -1;
    bool find = false;

    for (int i = 0; i < MAX_LBLC_ISO_ITEM_NUM; i++) {
        find = false;
        for (auto iso_item : iso_item_list) {
            if (iso_item->getGroup() == i) {
                find = true;
                break;
            }
        }

        if (find == false) {
            group = i;
            return group;
        }
    }

    return group;
}

void CalibrationISOLBlcListWidget::clearOtherItemDisplay(ISOItem *item)
{
    for (auto iso_item : iso_item_list) {
        if (item == iso_item) continue;
        if (item->getISO() == iso_item->getISO()) {
            iso_item->setDisplay(Qt::CheckState::Unchecked);
        }
    }
}
