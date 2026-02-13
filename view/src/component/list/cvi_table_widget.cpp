#include "cvi_table_widget.hpp"

#define MAX_WIN_WIDTH (400)
#define MAX_WIN_HEIGHT (400)


CviTableWidget::CviTableWidget(int rows, int columns, QWidget *parent) : QTableWidget(rows, columns, parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setDragDropMode(QAbstractItemView::InternalMove);
    setDefaultDropAction(Qt::IgnoreAction);
    setDropIndicatorShown(true);
    setDragEnabled(true);
}

void CviTableWidget::dropEvent(QDropEvent *event)
{
    int src_row, dst_row;
    src_row = currentRow();
    QModelIndex index = indexAt(event->pos());
    if (index.isValid()) {
        dst_row = index.row();
        qDebug("----dropEvent----%d,%d----",src_row,dst_row);
        if (dst_row > src_row) {
            insertRow(dst_row + 1);
            for (int i = 0; i < columnCount() - 1; i++) {
                setItem(dst_row + 1, i, takeItem(src_row, i));
            }
            QWidget *itemWidget = cellWidget(src_row, columnCount() - 1);
            setCellWidget(dst_row + 1, columnCount() - 1, itemWidget);
            removeRow(src_row);
            clearSelection();
            selectRow(dst_row);
            emit rowSwaped(src_row, dst_row);
        } else if (dst_row < src_row) {
            insertRow(dst_row + 1);
            for (int i = 0; i < columnCount() - 1; i++) {
                setItem(dst_row + 1, i, takeItem(dst_row, i));
                setItem(dst_row, i, takeItem(src_row + 1, i));
            }
            QWidget *itemWidget = cellWidget(src_row + 1, columnCount() - 1);
            setCellWidget(dst_row + 1, columnCount() - 1, itemWidget);
            removeRow(src_row + 1);
            clearSelection();
            selectRow(dst_row);
            emit rowSwaped(src_row, dst_row);
        }
    }
}
