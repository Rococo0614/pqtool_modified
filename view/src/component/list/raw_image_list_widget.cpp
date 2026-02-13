#include <QMenuBar>
#include <QLabel>
#include <QHeaderView>
#include <QCheckBox>

#include "global_data.hpp"
#include "raw_image_list_widget.hpp"
#include "isp_info_dialog.hpp"

#define MAX_WIN_WIDTH (1000)

RawImageListWidget::RawImageListWidget(QWidget *parent)
{
    initUI();
    retranslateUi();
}

void RawImageListWidget::setCheckState(Qt::CheckState state)
{
    for(int i = 0; i < raw_image_list.size(); i++)
    {
        QCheckBox *cb = qobject_cast<QCheckBox *>(image_table_widget->cellWidget(i, 0));
        cb->setCheckState(state);
    }
}

void RawImageListWidget::addTableItem(RawImageListItem *item)
{
    if (!item) {
        return;
    }

    int index = raw_image_list.size();
    this->raw_image_list.append(item);

    image_table_widget->setRowCount(raw_image_list.size());

    QCheckBox *sel_checkbox = new QCheckBox;
    image_table_widget->setCellWidget(index, 0, sel_checkbox);
    connect(sel_checkbox, &QCheckBox::stateChanged, [=](int state){
        int idx = raw_image_list.indexOf(item);
        raw_image_list[idx]->setCheckState(static_cast<Qt::CheckState>(state));
    });

    QTableWidgetItem *name_item = new QTableWidgetItem(item->getFileName());
    image_table_widget->setItem(index, 1, name_item);
    name_item->setFlags(name_item->flags() & (~Qt::ItemFlag::ItemIsEditable));

    QTableWidgetItem *mode_item = new QTableWidgetItem(mode_strlist[item->getMode().toInt()]);
    image_table_widget->setItem(index, 2, mode_item);
    mode_item->setFlags(mode_item->flags() & (~Qt::ItemFlag::ItemIsEditable));

    QTableWidgetItem *size_item = new QTableWidgetItem(QString("%1x%2 (%3)")
                                                       .arg(item->getWidth())
                                                       .arg(item->getHeight())
                                                       .arg(bits_list[item->getBits().toInt()]));
    image_table_widget->setItem(index, 3, size_item);
    size_item->setFlags(size_item->flags() & (~Qt::ItemFlag::ItemIsEditable));
    QTableWidgetItem *bayer_item = new QTableWidgetItem(bayer_strlist[item->getBayer().toInt()]);
    image_table_widget->setItem(index, 4, bayer_item);
    bayer_item->setFlags(bayer_item->flags() & (~Qt::ItemFlag::ItemIsEditable));

    QLineEdit *start_item = new QLineEdit;
    start_item->setFrame(false);
    start_item->setText(QString::number(item->getFStart()));
    image_table_widget->setCellWidget(index, 5, start_item);
    connect(start_item, &QLineEdit::editingFinished, [=]{
        int value = start_item->text().toInt();
        int idx = raw_image_list.indexOf(item);
        value = qBound(1, value, raw_image_list[idx]->getFEnd());
        raw_image_list[idx]->setFStart(value);
        start_item->setText(QString::number(value));
    });

    QLineEdit *end_item = new QLineEdit;
    end_item->setFrame(false);
    end_item->setText(QString::number(item->getFEnd()));
    image_table_widget->setCellWidget(index, 6, end_item);
    connect(end_item, &QLineEdit::editingFinished, [=]() {
        int value = end_item->text().toInt();
        int idx = raw_image_list.indexOf(item);
        value = qBound(raw_image_list[idx]->getFStart(), value, item->getFrameNum());
        raw_image_list[idx]->setFEnd(value);
        end_item->setText(QString::number(value));
    });

    QLineEdit *gop_item = new QLineEdit;
    gop_item->setFrame(false);
    gop_item->setText(QString::number(item->getFGop()));
    image_table_widget->setCellWidget(index, 7, gop_item);
    connect(gop_item, &QLineEdit::textChanged, [=](const QString &text) {
        int value = text.toInt();
        value = qBound(1, value, item->getFrameNum());
        int idx = raw_image_list.indexOf(item);
        raw_image_list[idx]->setFGop(value);
    });

    QPushButton *edit_btn = new QPushButton(tr("view"));
    edit_btn->setMaximumWidth(50);
    image_table_widget->setCellWidget(index, 8, edit_btn);
    connect(edit_btn, &QPushButton::clicked, [=]{
        IspInfoDialog dialog(nullptr, item->getFrameNum(), item->getRoiMode());
        QList<RawIspInfo> list = item->getRawIspInfo();
        dialog.loadIspInfo(item->getFrameNum(), list);
        dialog.updateIspInfoDisp();
        QList<int> blackLvlList = item->getBlackLvl();
        dialog.loadBlackLvlInfo(blackLvlList);
        dialog.updateBlackLvlInfoDisp();
        if (item->getRoiMode()) {
            dialog.loadRoiInfo(item->getRoiInfo());
            dialog.updateRoiInfo();
        }
        if (QDialog::Accepted == dialog.exec())
        {
            item->setRawIspInfo(dialog.getIspInfo());
            item->setBlackLvl(dialog.getBlackLvl());
            if (item->getRoiMode()) {
                RoiInfo roiInfoNew = dialog.getRoiInfo();
                RoiInfo roiInfoOld = item->getRoiInfo();
                if ((roiInfoNew.cropX != roiInfoOld.cropX) || (roiInfoNew.cropY != roiInfoOld.cropY)
                  || (roiInfoNew.cropWidth != roiInfoOld.cropWidth) || (roiInfoNew.cropHeight != roiInfoOld.cropHeight)) {
                    roiInfoNew.cropChange = true;
                } else {
                    roiInfoNew.cropChange = false;
                }
                item->setRoiInfo(roiInfoNew);
            }
        }
    });

    connect(item, &RawImageListItem::displayStateChanged, [=] (Qt::CheckState state) {
        if (state == Qt::CheckState::Checked) {
            clearOtherItemDisplay(item);
        }
        sel_checkbox->setCheckState(state);
    });
}

void RawImageListWidget::printImageList()
{
    for (int i=0; i<raw_image_list.size(); i++) {
        qDebug("bit: %d", raw_image_list[i]->getBits().toInt());
    }
}

void RawImageListWidget::moveRow(int from_idx, int to_idx)
{
    if (image_table_widget == nullptr)
        return;
    int from = from_idx;
    int to = to_idx;
    if(from == to || from < 0 || to < 0
            || from > image_table_widget->rowCount()
            || to > image_table_widget->rowCount())
        return;

    if (to < from)
        from++;
    image_table_widget->setFocus();
    image_table_widget->insertRow(to);
    int col_cnt = image_table_widget->columnCount();

    image_table_widget->setCellWidget(to, 0, image_table_widget->cellWidget(from, 0));
    for(int i=1; i<5; i++) {
        image_table_widget->setItem(to, i, image_table_widget->takeItem(from, i));
    }
    for(int i=5; i < col_cnt; i++) {
        image_table_widget->setCellWidget(to, i, image_table_widget->cellWidget(from, i));
    }

    if (from < to) {
        to--;
    }
    image_table_widget->removeRow(from);
    image_table_widget->selectRow(to);
}

void RawImageListWidget::moveup_sel()
{
    int row = image_table_widget->currentRow();
    //image_table_widget->rowCount();
    qDebug("row:%d", row);
    if(row == 0)
        return;

    moveRow(row, row - 1);
    raw_image_list.move(row, row - 1);
    //printImageList();
}

void RawImageListWidget::movedown_sel()
{
    int row = image_table_widget->currentRow();
    qDebug("row:%d", row);
    if (row == image_table_widget->rowCount() - 1)
        return;
    qDebug("row cnt:%d", image_table_widget->rowCount());
    moveRow(row, row + 2);
    raw_image_list.move(row, row + 1);
    //printImageList();
}

void RawImageListWidget::del_selected()
{
    int row = image_table_widget->currentRow();
    image_table_widget->removeRow(row);
    raw_image_list.removeAt(row);
}

void RawImageListWidget::del_all()
{
    while(image_table_widget->rowCount() != 0) {
        image_table_widget->removeRow(0);
    }
    raw_image_list.clear();
}

QList<RawImageListItem *> RawImageListWidget::getRawImageList()
{
    return this->raw_image_list;
}

QList<RawImageListItem *> RawImageListWidget::getSelRawImageList()
{
    QList<RawImageListItem *> list;
    for (int i = 0; i < raw_image_list.size(); i++) {
        if (raw_image_list[i]->getCheckState() == Qt::CheckState::Checked)
        {
            list.append(raw_image_list[i]);
        }
    }

    return list;
}

void RawImageListWidget::initUI()
{
   this->setMaximumWidth(MAX_WIN_WIDTH);
   main_layout = new QVBoxLayout(this);

   image_table_widget = new QTableWidget(0, 9);
   image_table_widget->setColumnWidth(0, 60);
   image_table_widget->setColumnWidth(1, 400);
   image_table_widget->setColumnWidth(2, 60);
   image_table_widget->setColumnWidth(3, 100);
   image_table_widget->setColumnWidth(4, 60);
   image_table_widget->setColumnWidth(5, 60);
   image_table_widget->setColumnWidth(6, 60);
   image_table_widget->setColumnWidth(7, 60);
   image_table_widget->setColumnWidth(8, 80);
   main_layout->addWidget(image_table_widget);
   image_table_widget->clearContents();
   image_table_widget->verticalHeader()->hide();
   image_table_widget->horizontalHeader()->setStretchLastSection(true);
   //image_table_widget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void RawImageListWidget::retranslateUi()
{
     this->image_table_widget->setHorizontalHeaderLabels({tr("Sel"), tr("File"), tr("Mode"), tr("Size"), tr("Bayer"),
                                                         tr("F.Start"), tr("F.End"), tr("F.Gop"), tr("Info")});
}

void RawImageListWidget::clearOtherItemDisplay(RawImageListItem *item)
{
    for (auto img_item : raw_image_list) {
        if (item == img_item) continue;
        img_item->setCheckState(Qt::CheckState::Unchecked);
    }
}
