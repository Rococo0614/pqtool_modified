#include <QMenuBar>
#include <QLabel>
#include <QHeaderView>
#include <QCheckBox>

#include "global_data.hpp"
#include "yuv_image_list_widget.hpp"
#include "isp_info_dialog.hpp"

#define MAX_WIN_WIDTH (1000)

YuvImageListWidget::YuvImageListWidget(QWidget *parent)
{
    initUI();
    retranslateUi();
}

void YuvImageListWidget::setCheckState(Qt::CheckState state)
{
    for(int i = 0; i < yuv_image_list.size(); i++)
    {
        QCheckBox *cb = qobject_cast<QCheckBox *>(image_table_widget->cellWidget(i, 0));
        cb->setCheckState(state);
    }
}

void YuvImageListWidget::addTableItem(YuvImageListItem *item)
{
    if (!item) {
        return;
    }

    int index = yuv_image_list.size();
    this->yuv_image_list.append(item);

    image_table_widget->setRowCount(yuv_image_list.size());

    QCheckBox *sel_checkbox = new QCheckBox;
    image_table_widget->setCellWidget(index, 0, sel_checkbox);
    connect(sel_checkbox, &QCheckBox::stateChanged, [=](int state){
        int idx = yuv_image_list.indexOf(item);
        yuv_image_list[idx]->setCheckState(static_cast<Qt::CheckState>(state));
    });

    QTableWidgetItem *name_item = new QTableWidgetItem(item->getFileName());
    image_table_widget->setItem(index, 1, name_item);
    name_item->setFlags(name_item->flags() & (~Qt::ItemFlag::ItemIsEditable));

    QTableWidgetItem *size_item = new QTableWidgetItem(QString("%1x%2").arg(item->getWidth()).arg(item->getHeight()));
    image_table_widget->setItem(index, 2, size_item);
    size_item->setFlags(size_item->flags() & (~Qt::ItemFlag::ItemIsEditable));

    QTableWidgetItem *bitwidth_item = new QTableWidgetItem(bitwidth_list[item->getBitwidth().toInt()]);
    image_table_widget->setItem(index, 3, bitwidth_item);
    bitwidth_item->setFlags(bitwidth_item->flags() & (~Qt::ItemFlag::ItemIsEditable));

    QTableWidgetItem *pixel_item = new QTableWidgetItem(pixeltype_short_list[item->getPixelType().toInt()]);
    image_table_widget->setItem(index, 4, pixel_item);
    pixel_item->setFlags(pixel_item->flags() & (~Qt::ItemFlag::ItemIsEditable));

    QTableWidgetItem *video_item = new QTableWidgetItem(videotype_short_list[item->getVideoType().toInt()]);
    image_table_widget->setItem(index, 5, video_item);
    video_item->setFlags(video_item->flags() & (~Qt::ItemFlag::ItemIsEditable));

    QLineEdit *start_item = new QLineEdit;
    start_item->setFrame(false);
    start_item->setReadOnly(true);
    start_item->setText(QString::number(item->getFStart()));
    image_table_widget->setCellWidget(index, 6, start_item);
    connect(start_item, &QLineEdit::textChanged, [=](const QString &text) {
        int value = text.toInt();
        value = qBound(1, value, item->getFrameNum());
        int idx = yuv_image_list.indexOf(item);
        yuv_image_list[idx]->setFStart(value);
    });

    QLineEdit *end_item = new QLineEdit;
    end_item->setFrame(false);
    end_item->setReadOnly(true);
    end_item->setText(QString::number(item->getFEnd()));
    image_table_widget->setCellWidget(index, 7, end_item);
    connect(end_item, &QLineEdit::textChanged, [=](const QString &text) {
        int value = text.toInt();
        value = qBound(1, value, item->getFrameNum());
        int idx = yuv_image_list.indexOf(item);
        yuv_image_list[idx]->setFEnd(value);
    });

    QLineEdit *gop_item = new QLineEdit;
    gop_item->setFrame(false);
    gop_item->setReadOnly(true);
    gop_item->setText(QString::number(item->getFGop()));
    image_table_widget->setCellWidget(index, 8, gop_item);
    connect(gop_item, &QLineEdit::textChanged, [=](const QString &text) {
        int value = text.toInt();
        value = qBound(1, value, item->getFrameNum());
        int idx = yuv_image_list.indexOf(item);
        yuv_image_list[idx]->setFGop(value);
    });

    connect(item, &YuvImageListItem::displayStateChanged, [=] (Qt::CheckState state) {
        if (state == Qt::CheckState::Checked) {
            clearOtherItemDisplay(item);
        }
        sel_checkbox->setCheckState(state);
    });
}

void YuvImageListWidget::clearOtherItemDisplay(YuvImageListItem *item)
{
    for (auto img_item : yuv_image_list) {
        if (item == img_item) continue;
        img_item->setCheckState(Qt::CheckState::Unchecked);
    }
}

void YuvImageListWidget::printImageList()
{
//    for (int i=0; i<yuv_image_list.size(); i++) {
//        qDebug("bit: %d", yuv_image_list[i]->getBits().toInt());
//    }
}

void YuvImageListWidget::moveRow(int from_idx, int to_idx)
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

void YuvImageListWidget::moveup_sel()
{
    int row = image_table_widget->currentRow();
    //image_table_widget->rowCount();
    qDebug("row:%d", row);
    if(row == 0)
        return;

    moveRow(row, row - 1);
    yuv_image_list.move(row, row - 1);
    //printImageList();
}

void YuvImageListWidget::movedown_sel()
{
    int row = image_table_widget->currentRow();
    qDebug("row:%d", row);
    if (row == image_table_widget->rowCount() - 1)
        return;
    qDebug("row cnt:%d", image_table_widget->rowCount());
    moveRow(row, row + 2);
    yuv_image_list.move(row, row + 1);
    //printImageList();
}

void YuvImageListWidget::del_selected()
{
    int row = image_table_widget->currentRow();
    image_table_widget->removeRow(row);
    yuv_image_list.removeAt(row);
}

void YuvImageListWidget::del_all()
{
    while(image_table_widget->rowCount() != 0) {
        image_table_widget->removeRow(0);
    }
    yuv_image_list.clear();
}

QList<YuvImageListItem *> YuvImageListWidget::getYuvImageList()
{
    return this->yuv_image_list;
}

QList<YuvImageListItem *> YuvImageListWidget::getSelYuvImageList()
{
    QList<YuvImageListItem *> list;
    for (int i = 0; i < yuv_image_list.size(); i++) {
        if (yuv_image_list[i]->getCheckState() == Qt::CheckState::Checked)
        {
            list.append(yuv_image_list[i]);
        }
    }

    return list;
}

void YuvImageListWidget::initUI()
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

void YuvImageListWidget::retranslateUi()
{
    this->image_table_widget->setHorizontalHeaderLabels({tr("Sel"), tr("File"), tr("Size"), tr("BitWidth"),
                                                        tr("PixelType"), tr("VideoType"), tr("F.Start"), tr("F.End"), tr("F.Gop")});
}

QVariant YuvImageListItem::getBitwidth() const
{
    return bitwidth;
}

void YuvImageListItem::setBitwidth(const QVariant &value)
{
    bitwidth = value;
}

QVariant YuvImageListItem::getPixelType() const
{
    return pixelType;
}

void YuvImageListItem::setPixelType(const QVariant &value)
{
    pixelType = value;
}

QVariant YuvImageListItem::getVideoType() const
{
    return videoType;
}

void YuvImageListItem::setVideoType(const QVariant &value)
{
    videoType = value;
}
