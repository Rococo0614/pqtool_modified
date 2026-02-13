#include "calibration_iso_list_widget.hpp"

#include <QMenuBar>
#include <QLabel>
#include <QHeaderView>
#include <QCheckBox>
#include <QLineEdit>

CalibrationIsoListWidget::CalibrationIsoListWidget(QWidget *parent) : QWidget(parent)
{
    initUI();
    retranslateUi();
}

void CalibrationIsoListWidget::initUI()
{
    main_layout = new QVBoxLayout(this);

    iso_table_widget = new QTableWidget(0, 5);
    /*iso_table_widget->setColumnWidth(0, 15);
    iso_table_widget->setColumnWidth(1, 50);
    iso_table_widget->setColumnWidth(2, 50);
    iso_table_widget->setColumnWidth(3, 60);
    iso_table_widget->setColumnWidth(4, 15);*/
    iso_table_widget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    main_layout->addWidget(iso_table_widget);
    iso_table_widget->clearContents();
}

void CalibrationIsoListWidget::retranslateUi()
{
    iso_table_widget->setHorizontalHeaderLabels({tr("Del"), tr("Group"), tr("ISO"), tr("Frames"), tr("Sel")});
}

bool CalibrationIsoListWidget::addTableItem(IsoItem *item)
{
    if (item == nullptr) {
        return false;
    }

    int group = getNextGroup();
    if (group < 0) {
        return false;
    }

    int index = iso_item_list.size();
    if (index > MAX_ISO_ITEM_NUM) {
        return false;
    }

    item->setGroup(group);

    this->iso_item_list.append(item);

    iso_table_widget->setRowCount(iso_item_list.size());

    QPushButton *del_btn = new QPushButton(tr("X"));
    del_btn->setMaximumWidth(40);
    iso_table_widget->setCellWidget(index, 0, del_btn);
    connect(del_btn, &QPushButton::clicked, [=]() {
       int idx = iso_item_list.indexOf(item);
       iso_table_widget->removeRow(idx);
       iso_item_list.removeAt(idx);
    });

    QTableWidgetItem *group_item = new QTableWidgetItem(QString::number(item->getGroup()));
    iso_table_widget->setItem(index, 1, group_item);

    QLineEdit *iso_edit = new QLineEdit(QString::number(item->getIso()));
    iso_edit->setFrame(false);
    iso_edit->setValidator(new QIntValidator(0, 3276800));
    iso_table_widget->setCellWidget(index, 2, iso_edit);
    connect(iso_edit, &QLineEdit::textChanged, [=](QString text) {
        int iso = text.toInt();
        item->setIso(iso);
        emit item->displayStateChanged(item->getDisplay());
    });

    QLineEdit *frames_item = new QLineEdit(QString::number(item->getFrames()));
    frames_item->setFrame(false);
    frames_item->setDisabled(true);
    iso_table_widget->setCellWidget(index, 3, frames_item);

    QCheckBox *sel_checkbox = new QCheckBox;
    sel_checkbox->setCheckState(item->getDisplay());
    iso_table_widget->setCellWidget(index, 4, sel_checkbox);
    connect(sel_checkbox, &QCheckBox::stateChanged, [=](int state) {
       int idx = iso_item_list.indexOf(item);
       this->iso_item_list[idx]->setDisplay(static_cast<Qt::CheckState>(state));
       // set input and output image
       emit sigSetInputPixmap(this->iso_item_list[idx]->getInputPixmap());
       emit sigSetOutputPixmap(this->iso_item_list[idx]->getOutputPixmap());
    });

    connect(item, &IsoItem::displayStateChanged, [=] (Qt::CheckState state) {
        if (state == Qt::CheckState::Checked) {
            clearSameIsoItemDisplay(item);
        }
        sel_checkbox->setCheckState(state);
    });

    return true;
}

QList<IsoItem *> CalibrationIsoListWidget::getIsoList()
{
    return this->iso_item_list;
}

QList<IsoItem *> CalibrationIsoListWidget::getDisplayIsoList()
{
    QList<IsoItem *> list;
    for (int i = 0; i < iso_item_list.size(); i++) {
        if (iso_item_list[i]->getDisplay() == Qt::CheckState::Checked)
            list.append(iso_item_list[i]);
    }
    return list;
}

int CalibrationIsoListWidget::getNextGroup()
{
    int group = -1;
    bool find = false;

    for (int i = 0; i < MAX_ISO_ITEM_NUM; i++) {
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

void CalibrationIsoListWidget::clearOtherItemDisplay(IsoItem *item)
{
    for (auto iso_item : iso_item_list) {
        if (item == iso_item) continue;
        iso_item->setDisplay(Qt::CheckState::Unchecked);
    }
}

void CalibrationIsoListWidget::clearSameIsoItemDisplay(IsoItem *item)
{
    for (auto iso_item : iso_item_list) {
        if (item == iso_item) continue;
        if (iso_item->getIso() == item->getIso()) {
            iso_item->setDisplay(Qt::CheckState::Unchecked);
        }
    }
}
