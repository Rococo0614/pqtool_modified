#include <QTableWidget>
#include <QTableView>
#include <QPushButton>
#include <QLineEdit>
#include <QHeaderView>
#include <QLabel>
#include <QComboBox>

#include "af_statistic_window.hpp"

AFStatWindow::AFStatWindow(QWidget *parent):
    QMainWindow (parent)
{
    setFixedSize(300, 550);

    initUI();
    initMenu();
    initEvent();
}

void AFStatWindow::initUI()
{
    main_widget = new QWidget;
    main_layout = new QHBoxLayout(main_widget);

    initLeftArea();
    initRightArea();
    setCentralWidget(main_widget);
}

void AFStatWindow::initEvent()
{

}

void AFStatWindow::initMenu()
{

}

void AFStatWindow::initLeftArea()
{
    QVBoxLayout *left_layout = new QVBoxLayout;
    QHBoxLayout *item_layout = new QHBoxLayout;
    QLabel *label = new QLabel(tr("Item"));
    QComboBox *item_box = new QComboBox;
    /* Hide H2 Statistics */
    // item_box->addItems({"V1", "H1", "H2", "HlCnt"});
    item_box->addItems({"V1", "H1", "HlCnt"});
    item_box->setCurrentIndex(0);
    connect(item_box, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int idx) {
        emit sigChangeFocusItem(idx);
    });
    item_layout->addWidget(label, Qt::AlignLeft);
    item_layout->addWidget(item_box, Qt::AlignRight);

    stat_table = new QTableWidget(4, 2);
    stat_table->setHorizontalHeaderLabels({"Item", "Value"});
    stat_table->verticalHeader()->setHidden(true);

    for (int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            QLineEdit *edit = new QLineEdit;
            edit->setFrame(false);
            edit->setReadOnly(true);
            stat_table->setCellWidget(i, j, edit);
        }
    }

    QLineEdit *cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(0, 0));
    cell_edit->setText("V1");
    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(1, 0));
    cell_edit->setText("H1");
    /* Hide H2 Statistics */
    // cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(2, 0));
    // cell_edit->setText("H2");
    // cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(3, 0));
    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(2, 0));
    cell_edit->setText("HlCnt");

    QPushButton *export_btn = new QPushButton(tr("Export AF Statistics"));
    connect(export_btn, &QPushButton::clicked, this, &AFStatWindow::exportStatistic);

    left_layout->addLayout(item_layout, Qt::AlignTop);
    left_layout->addWidget(stat_table, Qt::AlignTop);
    left_layout->addWidget(export_btn, Qt::AlignBottom);

    main_layout->addLayout(left_layout);
}

void AFStatWindow::initRightArea()
{
}

void AFStatWindow::updateStatistic(QVariant data)
{
    QVariantList data_list = data.toList();
    if (!data_list.isEmpty()) {
        int idx = 0;
        /* Hide H2 Statistics */
        // for (int i = 0; idx < data_list.size() && i < 4; i++) {
        for (int i = 0; idx < data_list.size() && i < 3; i++) {
            QLineEdit *edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(i, 1));
            if (edit) {
                edit->setText(QString::number(data_list[idx++].toInt()));
            }
        }
    }
}

void AFStatWindow::updateStatistic(QMap<QString, int> &map)
{
    QVariantList list;
    list.append(map[KEY_ATTR_AF_ZONE_V1]);
    list.append(map[KEY_ATTR_AF_ZONE_H1]);
    /* Hide H2 Statistics */
    // list.append(map[KEY_ATTR_AF_ZONE_H2]);
    list.append(map[KEY_ATTR_AF_ZONE_HLCNT]);

    updateStatistic(list);
}

void AFStatWindow::exportStatistic()
{

}

void AFStatWindow::closeEvent(QCloseEvent *event)
{
    emit sigWinClose();
    event->accept();
}
