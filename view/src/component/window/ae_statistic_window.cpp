#include <QTableWidget>
#include <QTableView>
#include <QPushButton>
#include <QLineEdit>
#include <QHeaderView>
#include <QLabel>
#include <QComboBox>
#include <QTabWidget>

#include "ae_statistic_window.hpp"
#include "hist_chart_view.hpp"

AEStatWindow::AEStatWindow(QWidget *parent):
    QMainWindow (parent)
{
    setMinimumHeight(550);
    setMinimumWidth(800);

    initUI();
    initMenu();
    initEvent();
}

void AEStatWindow::initUI()
{
    main_widget = new QWidget;
    main_layout = new QHBoxLayout(main_widget);
    main_layout->setStretch(0, 1);
    main_layout->setStretch(1, 3);

    initLeftArea();
    initRightArea();
    setCentralWidget(main_widget);
}

void AEStatWindow::initEvent()
{

}

void AEStatWindow::initMenu()
{

}

void AEStatWindow::initLeftArea()
{
    QWidget *left_widget = new QWidget;
    left_widget->setFixedWidth(300);

    QVBoxLayout *left_layout = new QVBoxLayout(left_widget);
    stat_table = new QTableWidget(10, 2);
    stat_table->setHorizontalHeaderLabels({"Item", "Value"});
    stat_table->verticalHeader()->setHidden(true);

    for (int i = 0; i < 10; i++)
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
    cell_edit->setStyleSheet("background-color:rgb(205, 205, 190)");
    cell_edit->setText("Global");
    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(0, 1));
    cell_edit->setStyleSheet("background-color:rgb(205, 205, 190)");

    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(1, 0));
    cell_edit->setText("R");
    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(2, 0));
    cell_edit->setText("Gr");
    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(3, 0));
    cell_edit->setText("Gb");
    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(4, 0));
    cell_edit->setText("B");

    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(5, 0));
    cell_edit->setText("Selected Zone");
    cell_edit->setStyleSheet("background-color:rgb(205, 205, 190)");
    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(5, 1));
    cell_edit->setStyleSheet("background-color:rgb(205, 205, 190)");

    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(6, 0));
    cell_edit->setText("R");
    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(7, 0));
    cell_edit->setText("Gr");
    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(8, 0));
    cell_edit->setText("Gb");
    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(9, 0));
    cell_edit->setText("B");

    QPushButton *export_btn = new QPushButton(tr("Export AE Statistics"));
    connect(export_btn, &QPushButton::clicked, this, &AEStatWindow::exportStatistic);

    left_layout->addWidget(stat_table);
    //left_layout->addWidget(export_btn);

    main_layout->addWidget(left_widget);
}

void AEStatWindow::initRightArea()
{
    QVBoxLayout *right_layout = new QVBoxLayout;
    QHBoxLayout *header = new QHBoxLayout;
    QLabel *pipe_label = new QLabel(tr("Pipe id:"));
    QComboBox *pipe_box = new QComboBox;
    pipe_box->addItems({"0", "1"});
    header->addWidget(pipe_label);
    header->addWidget(pipe_box);
    header->addStretch();

    QTabWidget *hist_tab = new QTabWidget;
    fe0_histogram = new HistChartView;
    fe0_histogram->initAEHist();
    fe0_histogram->setTitle(tr("FE0 Histogram 256"));
    fe1_histogram = new HistChartView;
    fe1_histogram->initAEHist();
    fe1_histogram->setTitle(tr("FE1 Histogram 256"));
    hist_tab->addTab(fe0_histogram, tr("FE0 Histogram 256"));
    hist_tab->addTab(fe1_histogram, tr("FE1 Histogram 256"));
    right_layout->addLayout(header);
    right_layout->addWidget(hist_tab);

    connect(pipe_box, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int idx) {
        pipe = idx;
        hist_tab->setCurrentIndex(idx);
        if (idx == 0) {
            this->updateStatistic(fe0_list);
        }
        else if (idx == 1) {
            this->updateStatistic(fe1_list);
        }
    });

    main_layout->addLayout(right_layout);
}

void AEStatWindow::updateStatistic(QVariant data)
{
    QVariantList data_list = data.toList();
    if (!data_list.isEmpty()) {
        int idx = 0;
        for (int i = 0; idx < data_list.size() && i < 10; i++) {
            if (i == 0 || i == 5)
                continue;
            QLineEdit *edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(i, 1));
            if (edit) {
                edit->setText(QString::number(data_list[idx++].toInt()));
            }
        }
    }
}

void AEStatWindow::updateHist(QMap <QString, QVariantList> &hist_map)
{
    fe0_histogram->setData(hist_map[KEY_ATTR_FE0_HIST]);
    fe1_histogram->setData(hist_map[KEY_ATTR_FE1_HIST]);
}

void AEStatWindow::updateStatistic(QMap<QString, int> & map)
{
    fe0_list.clear();
    fe1_list.clear();
    fe0_list.append(map[KEY_ATTR_FE0_GLOBAL_R]);
    fe0_list.append(map[KEY_ATTR_FE0_GLOBAL_GR]);
    fe0_list.append(map[KEY_ATTR_FE0_GLOBAL_GB]);
    fe0_list.append(map[KEY_ATTR_FE0_GLOBAL_B]);
    fe0_list.append(map[KEY_ATTR_FE0_ZONE_R]);
    fe0_list.append(map[KEY_ATTR_FE0_ZONE_GR]);
    fe0_list.append(map[KEY_ATTR_FE0_ZONE_GB]);
    fe0_list.append(map[KEY_ATTR_FE0_ZONE_B]);

    fe1_list.append(map[KEY_ATTR_FE1_GLOBAL_R]);
    fe1_list.append(map[KEY_ATTR_FE1_GLOBAL_GR]);
    fe1_list.append(map[KEY_ATTR_FE1_GLOBAL_GB]);
    fe1_list.append(map[KEY_ATTR_FE1_GLOBAL_B]);
    fe1_list.append(map[KEY_ATTR_FE1_ZONE_R]);
    fe1_list.append(map[KEY_ATTR_FE1_ZONE_GR]);
    fe1_list.append(map[KEY_ATTR_FE1_ZONE_GB]);
    fe1_list.append(map[KEY_ATTR_FE1_ZONE_B]);

    if (pipe == 0) {
        updateStatistic(fe0_list);
    } else {
        updateStatistic(fe1_list);
    }
}

void AEStatWindow::exportStatistic()
{

}

void AEStatWindow::closeEvent(QCloseEvent *event)
{
    emit sigWinClose();
    event->accept();
}

void AEStatWindow::handleUpdateTheme(bool dark_theme_enabled)
{
    QLineEdit *cell_edit;

    fe0_histogram->handleUpdateTheme(dark_theme_enabled);
    fe1_histogram->handleUpdateTheme(dark_theme_enabled);
    if (dark_theme_enabled) {
        cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(0, 0));
        cell_edit->setStyleSheet("background-color:rgb(50, 50, 65)");
        cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(0, 1));
        cell_edit->setStyleSheet("background-color:rgb(50, 50, 65)");
        cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(5, 0));
        cell_edit->setStyleSheet("background-color:rgb(50, 50, 65)");
        cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(5, 1));
        cell_edit->setStyleSheet("background-color:rgb(50, 50, 65)");
    } else {
        cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(0, 0));
        cell_edit->setStyleSheet("background-color:rgb(205, 205, 190)");
        cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(0, 1));
        cell_edit->setStyleSheet("background-color:rgb(205, 205, 190)");
        cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(5, 0));
        cell_edit->setStyleSheet("background-color:rgb(205, 205, 190)");
        cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(5, 1));
        cell_edit->setStyleSheet("background-color:rgb(205, 205, 190)");
    }
}
