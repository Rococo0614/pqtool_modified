#include "calibration_ct_list_widget.hpp"

#include <QMenuBar>
#include <QLabel>
#include <QHeaderView>
#include <QCheckBox>
#include <QLineEdit>

const int DEFAULT_CT = 1000;

CalibrationCTListWidget::CalibrationCTListWidget(QWidget *parent) : QWidget(parent)
{
    initUI();
    retranslateUi();
}

void CalibrationCTListWidget::initUI()
{
    main_layout = new QVBoxLayout(this);

    ct_table_widget = new QTableWidget(0, 5);
    //ct_table_widget->setColumnWidth(0, 20);
    //ct_table_widget->setColumnWidth(1, 55);
    //ct_table_widget->setColumnWidth(2, 50);
    //ct_table_widget->setColumnWidth(3, 50);
    //ct_table_widget->setColumnWidth(4, 30);
    ct_table_widget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    main_layout->addWidget(ct_table_widget);
    ct_table_widget->clearContents();
}

void CalibrationCTListWidget::retranslateUi()
{
    ct_table_widget->setHorizontalHeaderLabels({tr("Del"), tr("Group"), tr("Name"), tr("CT"), tr("Sel")});
}

bool CalibrationCTListWidget::addTableItem(CTItem *item)
{
    if (item == nullptr) {
        return false;
    }

    int group = getNextGroup();
    if (group < 0) {
        return false;
    }

    int index = ct_item_list.size();
    if (index > MAX_CT_ITEM_NUM) {
        return false;
    }

    item->setGroup(group);

    this->ct_item_list.append(item);

    ct_table_widget->setRowCount(ct_item_list.size());

    QPushButton *del_btn = new QPushButton(tr("X"));
    del_btn->setMaximumWidth(30);
    ct_table_widget->setCellWidget(index, 0, del_btn);
    connect(del_btn, &QPushButton::clicked, [=]() {
       int idx = ct_item_list.indexOf(item);
       ct_table_widget->removeRow(idx);
       ct_item_list.removeAt(idx);
    });

    QTableWidgetItem *group_item = new QTableWidgetItem(QString::number(item->getGroup()));
    ct_table_widget->setItem(index, 1, group_item);

    QTableWidgetItem *name_item = new QTableWidgetItem(item->getName());
    ct_table_widget->setItem(index, 2, name_item);

    //QTableWidgetItem *ct_item = new QTableWidgetItem(QString::number(item->getCT()));
    //ct_table_widget->setItem(index, 3, ct_item);

    QLineEdit *ct_edit = new QLineEdit(QString::number(item->getCT()));
    ct_edit->setFrame(false);
    ct_edit->setValidator(new QIntValidator(0, 20000));
    ct_table_widget->setCellWidget(index, 3, ct_edit);
    connect(ct_edit, &QLineEdit::textChanged, [=](QString text) {
        int ct = text.toInt();
        item->setCT(ct);
        emit item->displayStateChanged(item->getDisplay());
    });

    QCheckBox *sel_checkbox = new QCheckBox;
    sel_checkbox->setCheckState(item->getDisplay());
    ct_table_widget->setCellWidget(index, 4, sel_checkbox);
    connect(sel_checkbox, &QCheckBox::stateChanged, [=](int state) {
       int idx = ct_item_list.indexOf(item);
       this->ct_item_list[idx]->setDisplay(static_cast<Qt::CheckState>(state));
       // set input and output image
       emit sigSetInputPixmap(this->ct_item_list[idx]->getInputPixmap());
       emit sigSetOutputPixmap(this->ct_item_list[idx]->getOutputPixmap());
    });

    connect(item, &CTItem::displayStateChanged, [=] (Qt::CheckState state) {
        if (state == Qt::CheckState::Checked) {
            clearOtherItemDisplay(item);
        }
        sel_checkbox->setCheckState(state);
    });

    return true;
}

QList<CTItem *> CalibrationCTListWidget::getCTList()
{
    return this->ct_item_list;
}

QList<CTItem *> CalibrationCTListWidget::getDisplayCTList()
{
    QList<CTItem *> list;
    for (int i = 0; i < ct_item_list.size(); i++) {
        if (ct_item_list[i]->getDisplay() == Qt::CheckState::Checked) {
            list.append(ct_item_list[i]);
        }
    }

    return list;
}

int CalibrationCTListWidget::getNextGroup()
{
    int group = -1;
    bool find = false;

    for (int i = 0; i < MAX_CT_ITEM_NUM; i++) {
        find = false;
        for (auto ct_item : ct_item_list) {
            if (ct_item->getGroup() == i) {
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

void CalibrationCTListWidget::clearOtherItemDisplay(CTItem *item)
{
    for (auto ct_item : ct_item_list) {
        if (item == ct_item) continue;
        if (item->getCT() == ct_item->getCT()) {
            ct_item->setDisplay(Qt::CheckState::Unchecked);
        }
    }
}
