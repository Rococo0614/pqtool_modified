#include "calibration_image_list_widget.hpp"

#include <QMenuBar>
#include <QLabel>
#include <QHeaderView>
#include <QCheckBox>

const int MAX_WIN_WIDTH = 450;

CalibrationImageListWidget::CalibrationImageListWidget(QWidget *parent) : QWidget(parent)
{
    initUI();
    retranslateUi();
}

void CalibrationImageListWidget::initUI()
{
    this->setMaximumWidth(MAX_WIN_WIDTH);
    main_layout = new QVBoxLayout(this);

    image_table_widget = new QTableWidget(0, 5);
    /*image_table_widget->setColumnWidth(0, 20);
    image_table_widget->setColumnWidth(1, 100);
    image_table_widget->setColumnWidth(2, 60);
    image_table_widget->setColumnWidth(3, 20);*/
    image_table_widget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    main_layout->addWidget(image_table_widget);
    image_table_widget->clearContents();
    pv_window = new CalibrationPVWindow();
}

void CalibrationImageListWidget::retranslateUi()
{
    image_table_widget->setHorizontalHeaderLabels({tr("Del"), tr("Name"), tr("Scene"), tr("PV"), tr("Sel")});
}

void CalibrationImageListWidget::addTableItem(RawImageItem *item)
{
    if (!item) {
        return;
    }

    int index = raw_image_list.size();
    this->raw_image_list.append(item);

    image_table_widget->setRowCount(raw_image_list.size());

    QPushButton *del_btn = new QPushButton(tr("X"));
    del_btn->setFixedSize(30, 30);
    image_table_widget->setCellWidget(index, 0, del_btn);
    connect(del_btn, &QPushButton::clicked, [=]() {
       int idx = raw_image_list.indexOf(item);
       image_table_widget->removeRow(idx);
       raw_image_list.removeAt(idx);
    });

    QTableWidgetItem *name_item = new QTableWidgetItem(item->getFileName());
    image_table_widget->setItem(index, 1, name_item);

    QTableWidgetItem *scene_item = new QTableWidgetItem(getSceneName(item->getScene()));
    image_table_widget->setItem(index, 2, scene_item);

    QPushButton *pv_btn = new QPushButton;
    pv_btn->setFixedSize(30, 30);
    image_table_widget->setCellWidget(index, 3, pv_btn);
    connect(pv_btn, &QPushButton::clicked, [=]() {
       showPVWindow(item);
    });

    QCheckBox *sel_checkbox = new QCheckBox;
    image_table_widget->setCellWidget(index, 4, sel_checkbox);
    connect(sel_checkbox, &QCheckBox::stateChanged, [=](int state) {
       int idx = raw_image_list.indexOf(item);
       this->raw_image_list[idx]->setCheckState(static_cast<Qt::CheckState>(state));
       if (state == Qt::CheckState::Checked) {
            emit sigRawImageItemChecked(idx);
       }
    });

    connect(item, &RawImageItem::displayStateChanged, [=] (Qt::CheckState state) {
        if (state == Qt::CheckState::Checked) {
            clearOtherItemDisplay(item);
        }
        sel_checkbox->setCheckState(state);
    });
}

QList<RawImageItem *> CalibrationImageListWidget::getRawImageList()
{
    return this->raw_image_list;
}


void CalibrationImageListWidget::clearOtherItemDisplay(RawImageItem *item)
{
    for (auto img_item : raw_image_list) {
        if (item == img_item) continue;
        img_item->setCheckState(Qt::CheckState::Unchecked);
    }
}

void CalibrationImageListWidget::showPVWindow(RawImageItem *item)
{
    if (pv_window == nullptr) {
        pv_window = new CalibrationPVWindow();
    }
    pv_window->show();
    pv_window->activateWindow();
    pv_window->OpenImage(item);
}

