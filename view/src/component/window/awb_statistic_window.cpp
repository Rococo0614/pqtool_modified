#include <QTableWidget>
#include <QTableView>
#include <QPushButton>
#include <QLineEdit>
#include <QHeaderView>

#include "awb_statistic_window.hpp"

AWBStatWindow::AWBStatWindow(QWidget *parent):
    QMainWindow (parent)
{
    setMinimumHeight(550);
    setMinimumWidth(800);

    initUI();
    initMenu();
    initEvent();
}

void AWBStatWindow::initUI()
{
    main_widget = new QWidget;
    main_layout = new QHBoxLayout(main_widget);

    initLeftArea();
    initRightArea();
    setCentralWidget(main_widget);
}

void AWBStatWindow::initEvent()
{
    connect(show_statistics, &QCheckBox::clicked, this, &AWBStatWindow::slotSetShowStatistics);
    connect(show_planckian_curve, &QCheckBox::clicked, this, &AWBStatWindow::slotSetShowPlanckianCurve);
    connect(show_white_zone, &QCheckBox::clicked, this, &AWBStatWindow::slotSetShowWhiteZone);
}

void AWBStatWindow::initMenu()
{

}

void AWBStatWindow::initLeftArea()
{
    QWidget *left_widget = new QWidget;
    left_widget->setMaximumWidth(300);
    QVBoxLayout *left_layout = new QVBoxLayout(left_widget);
    stat_table = new QTableWidget(24, 2);
    stat_table->setHorizontalHeaderLabels({"Item", "Value"});
    stat_table->verticalHeader()->setHidden(true);

    for (int i = 0; i < 24; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            QLineEdit *edit = new QLineEdit;
            edit->setFrame(false);
            edit->setReadOnly(true);
            stat_table->setCellWidget(i, j, edit);
        }
        if ( i == 0 || i == 5 || i == 12 || i == 17) {
            stat_table->setSpan(i, 0, 1, 2);
        }
    }

    QLineEdit *cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(0, 1));
    cell_edit->setStyleSheet("background-color:rgb(205, 205, 190)");
    cell_edit->setText(" LE Global");

    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(1, 0));
    cell_edit->setText(" R");
    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(2, 0));
    cell_edit->setText(" G");
    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(3, 0));
    cell_edit->setText(" B");
    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(4, 0));
    cell_edit->setText(" CountAll");

    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(5, 1));
    cell_edit->setText(" LE Selected Zone");
    cell_edit->setStyleSheet("background-color:rgb(205, 205, 190)");

    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(6, 0));
    cell_edit->setText(" R");
    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(7, 0));
    cell_edit->setText(" G");
    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(8, 0));
    cell_edit->setText(" B");
    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(9, 0));
    cell_edit->setText(" CountAll");
    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(10, 0));
    cell_edit->setText(" G / R x 1024");
    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(11, 0));
    cell_edit->setText(" G / B x 1024");

//    for SE
    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(12, 1));
    cell_edit->setStyleSheet("background-color:rgb(205, 205, 190)");
    cell_edit->setText(" SE Global");

    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(13, 0));
    cell_edit->setText(" R");
    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(14, 0));
    cell_edit->setText(" G");
    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(15, 0));
    cell_edit->setText(" B");
    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(16, 0));
    cell_edit->setText(" CountAll");

    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(17, 1));
    cell_edit->setText(" SE Selected Zone");
    cell_edit->setStyleSheet("background-color:rgb(205, 205, 190)");

    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(18, 0));
    cell_edit->setText(" R");
    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(19, 0));
    cell_edit->setText(" G");
    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(20, 0));
    cell_edit->setText(" B");
    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(21, 0));
    cell_edit->setText(" CountAll");
    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(22, 0));
    cell_edit->setText(" G / R x 1024");
    cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(23, 0));
    cell_edit->setText(" G / B x 1024");

    QPushButton *export_btn = new QPushButton(tr("Export AWB Statistics"));
    connect(export_btn, &QPushButton::clicked, this, &AWBStatWindow::exportStatistic);

    left_layout->addWidget(stat_table, Qt::AlignTop);
    //left_layout->addWidget(export_btn, Qt::AlignBottom);

    main_layout->addWidget(left_widget);
}

void AWBStatWindow::initRightArea()
{
    initRightItems();

    QVBoxLayout *right_layout = new QVBoxLayout;
    QHBoxLayout *header = new QHBoxLayout;
    header->addWidget(show_statistics);
    header->addWidget(show_planckian_curve);
    header->addWidget(show_white_zone);
    header->addWidget(calibration_status);
    header->addStretch();

    QTabWidget *curve_tab = new QTabWidget;

    chart_le = new QtCharts::QChart();
    chart_le->setTitle("AWB Statistics Chart LE");
    chart_view_le = new QtCharts::QChartView(chart_le);
    chart_view_le->setRenderHint(QPainter::Antialiasing);
    curve_tab->addTab(chart_view_le, tr("Bin LE"));

    chart_se = new QtCharts::QChart();
    chart_se->setTitle("AWB Statistics Chart SE");
    chart_view_se = new QtCharts::QChartView(chart_se);
    chart_view_se->setRenderHint(QPainter::Antialiasing);
    curve_tab->addTab(chart_view_se, tr("Bin SE"));

    addSeries(chart_le);
    tab_index = 0;
    curve_tab->setCurrentIndex(tab_index);

    right_layout->addLayout(header);
    right_layout->addWidget(curve_tab);

    main_layout->addLayout(right_layout);
    connect(curve_tab, &QTabWidget::currentChanged, this, &AWBStatWindow::slotCurrentTabChanged);
}

void AWBStatWindow::initRightItems()
{
    show_statistics = new QCheckBox(tr("Show Statistics"));
    show_statistics->setChecked(true);
    show_planckian_curve = new QCheckBox(tr("Show Planckian Curve"));
    show_planckian_curve->setChecked(true);
    show_white_zone = new QCheckBox(tr("Show White Zone"));
    show_white_zone->setChecked(true);
    calibration_status = new QLabel(tr("Calibration Status: "));

    scatter_series_zone[0] = new QtCharts::QScatterSeries();
    scatter_series_zone[0]->setName(tr("Zone.Sta0"));
    scatter_series_zone[0]->setMarkerShape(QtCharts::QScatterSeries::MarkerShapeRectangle);
    scatter_series_zone[0]->setBorderColor(Qt::blue);
    scatter_series_zone[0]->setBrush(QBrush(Qt::blue));
    scatter_series_zone[0]->setMarkerSize(1);
    scatter_series_zone[1] = new QtCharts::QScatterSeries();
    scatter_series_zone[1]->setName(tr("Zone.Sta1"));
    scatter_series_zone[1]->setMarkerShape(QtCharts::QScatterSeries::MarkerShapeRectangle);
    scatter_series_zone[1]->setBorderColor(Qt::blue);
    scatter_series_zone[1]->setBrush(QBrush(Qt::blue));
    scatter_series_zone[1]->setMarkerSize(1);
    for (int i = 0; i < v_wnd; i++) {
        for (int j = 0; j < h_wnd; j++) {
            scatter_series_zone[0]->append(-1, -1);
            scatter_series_zone[1]->append(-1, -1);
        }
    }
    scatter_series_zone_black[0] = new QtCharts::QScatterSeries();
    scatter_series_zone_black[0]->setName(tr("Zone.StaNone0"));
    scatter_series_zone_black[0]->setMarkerShape(QtCharts::QScatterSeries::MarkerShapeRectangle);
    scatter_series_zone_black[0]->setBorderColor(Qt::black);
    scatter_series_zone_black[0]->setBrush(QBrush(Qt::black));
    scatter_series_zone_black[0]->setMarkerSize(1);
    scatter_series_zone_black[1] = new QtCharts::QScatterSeries();
    scatter_series_zone_black[1]->setName(tr("Zone.StaNone1"));
    scatter_series_zone_black[1]->setMarkerShape(QtCharts::QScatterSeries::MarkerShapeRectangle);
    scatter_series_zone_black[1]->setBorderColor(Qt::black);
    scatter_series_zone_black[1]->setBrush(QBrush(Qt::black));
    scatter_series_zone_black[1]->setMarkerSize(1);

    scatter_series_global[0] = new QtCharts::QScatterSeries();
    scatter_series_global[0]->setName(tr("Glbal.Sta0"));
    scatter_series_global[0]->setMarkerShape(QtCharts::QScatterSeries::MarkerShapeRectangle);
    scatter_series_global[0]->setBorderColor(Qt::black);
    scatter_series_global[0]->setBrush(QBrush(Qt::darkGreen));
    scatter_series_global[0]->setMarkerSize(15);
    scatter_series_global[0]->append(-1, -1);
    scatter_series_global[1] = new QtCharts::QScatterSeries();
    scatter_series_global[1]->setName(tr("Glbal.Sta1"));
    scatter_series_global[1]->setMarkerShape(QtCharts::QScatterSeries::MarkerShapeRectangle);
    scatter_series_global[1]->setBorderColor(Qt::black);
    scatter_series_global[1]->setBrush(QBrush(Qt::darkGreen));
    scatter_series_global[1]->setMarkerSize(15);
    scatter_series_global[1]->append(-1, -1);

    scatter_series_result = new QtCharts::QScatterSeries();
    scatter_series_result->setName(tr("Result"));
    scatter_series_result->setMarkerShape(QtCharts::QScatterSeries::MarkerShapeCircle);
    scatter_series_result->setBorderColor(Qt::black);
    scatter_series_result->setBrush(QBrush(QColor(160, 32, 240)));
    scatter_series_result->setMarkerSize(15);
    scatter_series_result->append(-1, -1);

    calib_point[0] = new QtCharts::QScatterSeries();
    calib_point[0]->setName(tr("Calib0"));
    calib_point[0]->setMarkerShape(QtCharts::QScatterSeries::MarkerShapeCircle);
    calib_point[0]->setBorderColor(Qt::white);
    calib_point[0]->setBrush(QBrush(QColor(240, 0, 0)));
    calib_point[0]->setMarkerSize(10);
    calib_point[0]->append(-1, -1);
    calib_point[1] = new QtCharts::QScatterSeries();
    calib_point[1]->setName(tr("Calib1"));
    calib_point[1]->setMarkerShape(QtCharts::QScatterSeries::MarkerShapeCircle);
    calib_point[1]->setBorderColor(Qt::white);
    calib_point[1]->setBrush(QBrush(QColor(240, 0, 0)));
    calib_point[1]->setMarkerSize(10);
    calib_point[1]->append(-1, -1);
    calib_point[2] = new QtCharts::QScatterSeries();
    calib_point[2]->setName(tr("Calib2"));
    calib_point[2]->setMarkerShape(QtCharts::QScatterSeries::MarkerShapeCircle);
    calib_point[2]->setBorderColor(Qt::white);
    calib_point[2]->setBrush(QBrush(QColor(240, 0, 0)));
    calib_point[2]->setMarkerSize(10);
    calib_point[2]->append(-1, -1);

    scatter_series_selected[0] = new QtCharts::QScatterSeries();
    scatter_series_selected[0]->setName(tr("Selected0"));
    scatter_series_selected[0]->setMarkerShape(QtCharts::QScatterSeries::MarkerShapeCircle);
    scatter_series_selected[0]->setBorderColor(Qt::black);
    scatter_series_selected[0]->setBrush(QBrush(Qt::magenta));
    scatter_series_selected[0]->setMarkerSize(15);
    scatter_series_selected[0]->append(-1, -1);
    scatter_series_selected[1] = new QtCharts::QScatterSeries();
    scatter_series_selected[1]->setName(tr("Selected1"));
    scatter_series_selected[1]->setMarkerShape(QtCharts::QScatterSeries::MarkerShapeCircle);
    scatter_series_selected[1]->setBorderColor(Qt::black);
    scatter_series_selected[1]->setBrush(QBrush(Qt::magenta));
    scatter_series_selected[1]->setMarkerSize(15);
    scatter_series_selected[1]->append(-1, -1);

    QPen penA, penB, penC;
    penA.setColor(Qt::green);
    penA.setWidth(3);
    penB.setColor(Qt::red);
    penB.setWidth(2);
    penC.setColor(Qt::darkRed);
    penC.setWidth(3);
    pen_dot_green.setColor(Qt::green);
    pen_dot_green.setStyle(Qt::PenStyle::DotLine);
    pen_dot_green.setWidth(2);
    pen_dot_red.setColor(Qt::red);
    pen_dot_red.setStyle(Qt::PenStyle::DotLine);
    pen_dot_red.setWidth(2);
    line_series_mid = new QtCharts::QLineSeries();
    line_series_mid->setName(tr("Plank.C"));
    line_series_mid->setPen(penC);

    line_series_top = new QtCharts::QLineSeries();
    line_series_top->setName(tr("White Zone"));
    line_series_top->setPen(penA);

    line_series_bot = new QtCharts::QLineSeries();
    line_series_bot->setName(tr("White Zone"));
    line_series_bot->setPen(penA);

    line_series_left_limit = new QtCharts::QLineSeries();
    line_series_left_limit->setName(tr("Curve Limit"));
    line_series_left_limit->setPen(penB);
    for (int i = 0; i < 3; i++) {
        line_series_left_limit->append(-1, -1);
    }

    line_series_right_limit = new QtCharts::QLineSeries();
    line_series_right_limit->setName(tr("Curve Limit"));
    line_series_right_limit->setPen(penB);
    for (int i = 0; i < 3; i++) {
        line_series_right_limit->append(-1, -1);
    }

    line_series_extra_LS[0] = new QtCharts::QLineSeries();
    line_series_extra_LS[0]->setName(tr("LS1(Dot Line)"));
    line_series_extra_LS[0]->setPen(pen_dot_green);
    line_series_extra_LS[1] = new QtCharts::QLineSeries();
    line_series_extra_LS[1]->setName(tr("LS2(Dot Line)"));
    line_series_extra_LS[1]->setPen(pen_dot_green);
    line_series_extra_LS[2] = new QtCharts::QLineSeries();
    line_series_extra_LS[2]->setName(tr("LS3(Dot Line)"));
    line_series_extra_LS[2]->setPen(pen_dot_green);
    line_series_extra_LS[3] = new QtCharts::QLineSeries();
    line_series_extra_LS[3]->setName(tr("LS4(Dot Line)"));
    line_series_extra_LS[3]->setPen(pen_dot_green);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 25; j++) {
            line_series_extra_LS[i]->append(-1, -1);
        }
    }

    line_region_r[0] = new QtCharts::QLineSeries();
    line_region_r[0]->setName(tr("R1(Dot Line)"));
    line_region_r[0]->setPen(pen_dot_green);
    line_region_r[1] = new QtCharts::QLineSeries();
    line_region_r[1]->setName(tr("R2(Dot Line)"));
    line_region_r[1]->setPen(pen_dot_green);
    line_region_r[2] = new QtCharts::QLineSeries();
    line_region_r[2]->setName(tr("R3(Dot Line)"));
    line_region_r[2]->setPen(pen_dot_green);

    axis_x = new QtCharts::QValueAxis();
    axis_y = new QtCharts::QValueAxis();
    axis_x->setLabelFormat("%.1f");
    axis_y->setLabelFormat("%.1f");
    axis_x->setTickCount(13);
    axis_y->setTickCount(13);
    axis_x->setRange(0, 1.2);
    axis_y->setRange(0, 1.2);
    axis_x->setTitleText("R/G");
    axis_y->setTitleText("B/G");

}

void AWBStatWindow::updateStatistic(QVariant data)
{
    QVariantList data_list = data.toList();
    if (!data_list.isEmpty()) {
        int idx = 0;
        for (int i = 0; idx < data_list.size() && i < 22; i++) {
            if (i == 0 || i == 5 || i == 10 || i == 11 || i == 12 || i == 17)
                continue;
            QLineEdit *edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(i, 1));
            if (edit) {
                edit->setText(QString::number(data_list[idx++].toInt()));
            }
        }
        QLineEdit *edit_g_divide_r0 = qobject_cast<QLineEdit *>(stat_table->cellWidget(10, 1));
        if (edit_g_divide_r0 && data_list[4].toInt() != 0) {
            edit_g_divide_r0->setText(QString::number(int(data_list[5].toFloat() / data_list[4].toFloat() * 1024)));
        } else if (edit_g_divide_r0) {
            edit_g_divide_r0->setText(QString::number(0));
        }
        QLineEdit *edit_g_divide_b0 = qobject_cast<QLineEdit *>(stat_table->cellWidget(11, 1));
        if (edit_g_divide_b0 && data_list[6].toInt() != 0) {
            edit_g_divide_b0->setText(QString::number(int(data_list[5].toFloat() / data_list[6].toFloat() * 1024)));
        } else if (edit_g_divide_b0) {
            edit_g_divide_b0->setText(QString::number(0));
        }
        QLineEdit *edit_g_divide_r1 = qobject_cast<QLineEdit *>(stat_table->cellWidget(22, 1));
        if (edit_g_divide_r1 && data_list[12].toInt() != 0) {
            edit_g_divide_r1->setText(QString::number(int(data_list[13].toFloat() / data_list[12].toFloat() * 1024)));
        } else if (edit_g_divide_r1) {
            edit_g_divide_r1->setText(QString::number(0));
        }
        QLineEdit *edit_g_divide_b1 = qobject_cast<QLineEdit *>(stat_table->cellWidget(23, 1));
        if (edit_g_divide_b1 && data_list[14].toInt() != 0) {
            edit_g_divide_b1->setText(QString::number(int(data_list[13].toFloat() / data_list[14].toFloat() * 1024)));
        } else if (edit_g_divide_b1) {
            edit_g_divide_b1->setText(QString::number(0));
        }
    }
}

void AWBStatWindow::updateStatistic(QMap<QString, int> &map)
{
    QVariantList list;
    list.append(map[KEY_ATTR_LE_WB_GLOBAL_R]);
    list.append(map[KEY_ATTR_LE_WB_GLOBAL_G]);
    list.append(map[KEY_ATTR_LE_WB_GLOBAL_B]);
    list.append(map[KEY_ATTR_LE_WB_GLOBAL_ALL]);
    list.append(map[KEY_ATTR_LE_WB_ZONE_R]);
    list.append(map[KEY_ATTR_LE_WB_ZONE_G]);
    list.append(map[KEY_ATTR_LE_WB_ZONE_B]);
    list.append(map[KEY_ATTR_LE_WB_ZONE_ALL]);
    list.append(map[KEY_ATTR_SE_WB_GLOBAL_R]);
    list.append(map[KEY_ATTR_SE_WB_GLOBAL_G]);
    list.append(map[KEY_ATTR_SE_WB_GLOBAL_B]);
    list.append(map[KEY_ATTR_SE_WB_GLOBAL_ALL]);
    list.append(map[KEY_ATTR_SE_WB_ZONE_R]);
    list.append(map[KEY_ATTR_SE_WB_ZONE_G]);
    list.append(map[KEY_ATTR_SE_WB_ZONE_B]);
    list.append(map[KEY_ATTR_SE_WB_ZONE_ALL]);

    wdr_mode = map[KEY_ATTR_IS_WDR_MODE];

    updateStatistic(list);
}

void AWBStatWindow::slotSetShowStatistics() {
    if (show_statistics->isChecked()) {
        scatter_series_zone[0]->show();
        scatter_series_zone[1]->show();
        scatter_series_zone_black[0]->show();
        scatter_series_zone_black[1]->show();
        scatter_series_global[0]->show();
        scatter_series_global[1]->show();
        scatter_series_result->show();
        scatter_series_selected[0]->show();
        scatter_series_selected[1]->show();
    } else {
        scatter_series_zone[0]->hide();
        scatter_series_zone[1]->hide();
        scatter_series_zone_black[0]->hide();
        scatter_series_zone_black[1]->hide();
        scatter_series_global[0]->hide();
        scatter_series_global[1]->hide();
        scatter_series_result->hide();
        scatter_series_selected[0]->hide();
        scatter_series_selected[1]->hide();
    }
}

void AWBStatWindow::slotSetShowPlanckianCurve() {
    if (show_planckian_curve->isChecked()) {
        line_series_mid->show();
    } else {
        line_series_mid->hide();
    }
}

void AWBStatWindow::slotSetShowWhiteZone() {
    if (show_white_zone->isChecked()) {
        line_series_top->show();
        line_series_bot->show();
        line_series_left_limit->show();
        line_series_right_limit->show();
    } else {
        line_series_top->hide();
        line_series_bot->hide();
        line_series_left_limit->hide();
        line_series_right_limit->hide();
    }
}

void AWBStatWindow::updateZoneSatPoints(float *tableX0, float *tableY0, float *tableX1, float *tableY1, s_AWB_DBG_S *pAwbDbg)
{
    int main_ver = pAwbDbg->u16AlgoVer >> 8;
    int i, k;
    int winSize = pAwbDbg->u16WinWnum * pAwbDbg->u16WinHnum;
    float RG_ratio, BG_ratio;

    if (main_ver > 4) {
        scatter_series_zone[0]->clear();
        scatter_series_zone[1]->clear();
        scatter_series_zone_black[0]->clear();
        scatter_series_zone_black[1]->clear();
        for (k = 0; k < pAwbDbg->u16MaxFrameNum; k++) {
            for (i = 0; i < winSize; i++) {
                if (pAwbDbg->u16P_G[k][i]) {
                    RG_ratio = pAwbDbg->u16P_R[k][i] / (float)pAwbDbg->u16P_G[k][i];
                    BG_ratio = pAwbDbg->u16P_B[k][i] / (float)pAwbDbg->u16P_G[k][i];
                    if (pAwbDbg->u16P_type[k][i] == AWB_DBG_PT_OUT_CURVE) {
                        scatter_series_zone_black[k]->append(RG_ratio, BG_ratio);
                    } else if (pAwbDbg->u16P_type[k][i] == AWB_DBG_PT_IN_CURVE) {
                        scatter_series_zone[k]->append(RG_ratio, BG_ratio);
                    }
                }
            }
        }
    } else {
        for (int i = 0; i < v_wnd * h_wnd; i++) {
            scatter_series_zone[0]->replace(i, tableX0[i], tableY0[i]);
            scatter_series_zone[1]->replace(i, tableX1[i], tableY1[i]);
        }
    }
}

void AWBStatWindow::updateGlobalSatPoint(float x0, float y0, float x1, float y1)
{
    scatter_series_global[0]->replace(0, x0, y0);
    if (wdr_mode)
        scatter_series_global[1]->replace(0, x1, y1);
    else
        scatter_series_global[1]->replace(0, -1, -1);
}

void AWBStatWindow::updateResultPoint(float x, float y)
{
    scatter_series_result->replace(0, x, y);
}

void AWBStatWindow::updateWhiteZoneCurvesMid(int num, float *tableX, float *tableY)
{
    line_series_mid->clear();
    for (int i = 0; i < num; i++) {
        line_series_mid->append(tableX[i], tableY[i]);
    }
}

void AWBStatWindow::updateWhiteZoneCurvesTop(int num, float *tableX, float *tableY)
{
    line_series_top->clear();
    for (int i = 0; i < num; i++) {
        line_series_top->append(tableX[i], tableY[i]);
    }
}

void AWBStatWindow::updateWhiteZoneCurvesBot(int num, float *tableX, float *tableY)
{
    line_series_bot->clear();
    for (int i = 0; i < num; i++) {
        line_series_bot->append(tableX[i], tableY[i]);
    }
}

void AWBStatWindow::updateCurveLeftLimit(float x, float y)
{
    line_series_left_limit->clear();
    line_series_left_limit->append(0, y);
    line_series_left_limit->append(x, y);
    line_series_left_limit->append(x, 0);
}

void AWBStatWindow::updateCurveRightLimit(float x, float y)
{
    line_series_right_limit->clear();
    line_series_right_limit->append(1.2, y);
    line_series_right_limit->append(x, y);
    line_series_right_limit->append(x, 1.2);
}

void AWBStatWindow::updateSelectedPoint(float x0, float y0, float x1, float y1)
{
    scatter_series_selected[0]->replace(0, x0, y0);

    if (wdr_mode)
        scatter_series_selected[1]->replace(0, x1, y1);
    else
        scatter_series_selected[1]->replace(0, -1, -1);
}

void AWBStatWindow::updateCalibrationStatus(int status)
{
    if (status == 255) {
        calibration_status->setText("Calibration Status: Default");
    } else {
        calibration_status->setText("Calibration Status: Updated");
    }
}

void AWBStatWindow::updateExtraLightSource(bool enabled, int *status, float x[][25], float y[][25])
{
    if (enabled == false) {
        for (int i = 0; i < 4; i++) {
            line_series_extra_LS[i]->hide();
        }
        return;
    }

    for (int i = 0; i < 4; i++) {
        if((status[i] > 0) && (status[i] < 3)) {
            if(status[i] == 1) {
                line_series_extra_LS[i]->setPen(pen_dot_green);
            } else if(status[i] == 2) {
                line_series_extra_LS[i]->setPen(pen_dot_red);
            }

            for (int j = 0; j < 25; j ++) {
                line_series_extra_LS[i]->replace(j, x[i][j], y[i][j]);
            }

            line_series_extra_LS[i]->show();
        } else {
            line_series_extra_LS[i]->hide();
        }
    }
}

void AWBStatWindow::updateCalibPoint(int calib_pt[3][3])
{
    int i;

    for (i = 0; i < 3; i++) {
        if (calib_pt[i][0] && calib_pt[i][1]) {
            calib_point[i]->replace(0, AWB_GAIN_BASE/(float)calib_pt[i][0], AWB_GAIN_BASE/(float)calib_pt[i][1]);
        }
    }
}
void AWBStatWindow::updateCurveRegion(int region[4])
{
    int i;

    for (i = 0; i < 3; i++) { // only 3 lines now
        line_region_r[i]->clear();
        if (region[i]) {
            line_region_r[i]->append(AWB_GAIN_BASE/(float)region[i], 0);
            line_region_r[i]->append(AWB_GAIN_BASE/(float)region[i], 1.2);
        }
    }
}

void AWBStatWindow::exportStatistic()
{

}

void AWBStatWindow::closeEvent(QCloseEvent *event)
{
    emit sigWinClose();
    event->accept();
}

void AWBStatWindow::handleUpdateTheme(bool dark_theme_enabled)
{
    QLineEdit *cell_edit;

    if (dark_theme_enabled) {
        chart_le->setBackgroundBrush(QColor(25, 35, 45));
        chart_se->setBackgroundBrush(QColor(25, 35, 45));
        axis_x->setLabelsColor(QColor(224, 225, 226));
        axis_y->setLabelsColor(QColor(224, 225, 226));
        cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(0, 1));
        cell_edit->setStyleSheet("background-color:rgb(50, 50, 65)");
        cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(5, 1));
        cell_edit->setStyleSheet("background-color:rgb(50, 50, 65)");
        cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(12, 1));
        cell_edit->setStyleSheet("background-color:rgb(50, 50, 65)");
        cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(17, 1));
        cell_edit->setStyleSheet("background-color:rgb(50, 50, 65)");
    } else {
        chart_le->setBackgroundBrush(QColor(255, 255, 255));
        chart_se->setBackgroundBrush(QColor(255, 255, 255));
        axis_x->setLabelsColor(QColor(0, 0, 0));
        axis_y->setLabelsColor(QColor(0, 0, 0));
        cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(0, 1));
        cell_edit->setStyleSheet("background-color:rgb(205, 205, 190)");
        cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(5, 1));
        cell_edit->setStyleSheet("background-color:rgb(205, 205, 190)");
        cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(12, 1));
        cell_edit->setStyleSheet("background-color:rgb(205, 205, 190)");
        cell_edit = qobject_cast<QLineEdit *>(stat_table->cellWidget(17, 1));
        cell_edit->setStyleSheet("background-color:rgb(205, 205, 190)");
    }
}

void AWBStatWindow::addSeries(QtCharts::QChart *chart)
{
    chart->addSeries(line_series_mid);
    chart->addSeries(line_series_top);
    chart->addSeries(line_series_bot);
    chart->addSeries(scatter_series_result);
    chart->addSeries(line_series_left_limit);
    chart->addSeries(line_series_right_limit);
    chart->addSeries(line_series_extra_LS[0]);
    chart->addSeries(line_series_extra_LS[1]);
    chart->addSeries(line_series_extra_LS[2]);
    chart->addSeries(line_series_extra_LS[3]);
    chart->addSeries(calib_point[0]);
    chart->addSeries(calib_point[1]);
    chart->addSeries(calib_point[2]);
    chart->addSeries(line_region_r[0]);
    chart->addSeries(line_region_r[1]);
    chart->addSeries(line_region_r[2]);

    chart->addAxis(axis_x, Qt::AlignBottom);
    chart->addAxis(axis_y, Qt::AlignLeft);

    scatter_series_result->attachAxis(axis_x);
    scatter_series_result->attachAxis(axis_y);
    line_series_mid->attachAxis(axis_x);
    line_series_mid->attachAxis(axis_y);
    line_series_top->attachAxis(axis_x);
    line_series_top->attachAxis(axis_y);
    line_series_bot->attachAxis(axis_x);
    line_series_bot->attachAxis(axis_y);
    line_series_left_limit->attachAxis(axis_x);
    line_series_left_limit->attachAxis(axis_y);
    line_series_right_limit->attachAxis(axis_x);
    line_series_right_limit->attachAxis(axis_y);
    line_series_extra_LS[0]->attachAxis(axis_x);
    line_series_extra_LS[0]->attachAxis(axis_y);
    line_series_extra_LS[1]->attachAxis(axis_x);
    line_series_extra_LS[1]->attachAxis(axis_y);
    line_series_extra_LS[2]->attachAxis(axis_x);
    line_series_extra_LS[2]->attachAxis(axis_y);
    line_series_extra_LS[3]->attachAxis(axis_x);
    line_series_extra_LS[3]->attachAxis(axis_y);
    calib_point[0]->attachAxis(axis_x);
    calib_point[0]->attachAxis(axis_y);
    calib_point[1]->attachAxis(axis_x);
    calib_point[1]->attachAxis(axis_y);
    calib_point[2]->attachAxis(axis_x);
    calib_point[2]->attachAxis(axis_y);
    line_region_r[0]->attachAxis(axis_x);
    line_region_r[0]->attachAxis(axis_y);
    line_region_r[1]->attachAxis(axis_x);
    line_region_r[1]->attachAxis(axis_y);
    line_region_r[2]->attachAxis(axis_x);
    line_region_r[2]->attachAxis(axis_y);

    if (tab_index == 0) {
        chart->addSeries(scatter_series_zone[0]);
        chart->addSeries(scatter_series_global[0]);
        chart->addSeries(scatter_series_selected[0]);
        chart->addSeries(scatter_series_zone_black[0]);

        scatter_series_zone[0]->attachAxis(axis_x);
        scatter_series_zone[0]->attachAxis(axis_y);
        scatter_series_global[0]->attachAxis(axis_x);
        scatter_series_global[0]->attachAxis(axis_y);
        scatter_series_selected[0]->attachAxis(axis_x);
        scatter_series_selected[0]->attachAxis(axis_y);
        scatter_series_zone_black[0]->attachAxis(axis_x);
        scatter_series_zone_black[0]->attachAxis(axis_y);
    } else if (tab_index == 1) {
        chart->addSeries(scatter_series_zone[1]);
        chart->addSeries(scatter_series_global[1]);
        chart->addSeries(scatter_series_selected[1]);
        chart->addSeries(scatter_series_zone_black[1]);

        scatter_series_zone[1]->attachAxis(axis_x);
        scatter_series_zone[1]->attachAxis(axis_y);
        scatter_series_global[1]->attachAxis(axis_x);
        scatter_series_global[1]->attachAxis(axis_y);
        scatter_series_selected[1]->attachAxis(axis_x);
        scatter_series_selected[1]->attachAxis(axis_y);
        scatter_series_zone_black[1]->attachAxis(axis_x);
        scatter_series_zone_black[1]->attachAxis(axis_y);
    }

}

void AWBStatWindow::removeSeries(QtCharts::QChart *chart)
{
    foreach (auto ax, chart->axes()) {
        chart->removeAxis(ax);
    }
    for (auto *s : chart->series()) {
        chart->removeSeries(s);
    }
}

void AWBStatWindow::slotCurrentTabChanged(int index)
{
    if (index != tab_index) {
        if (tab_index == 0) {
            tab_index = index;
            removeSeries(chart_le);
            addSeries(chart_se);
        } else if (tab_index == 1) {
            tab_index = index;
            removeSeries(chart_se);
            addSeries(chart_le);
        }
    }
}
