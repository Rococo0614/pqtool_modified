#ifndef LEVEL_METER_HISTOGRAM_WINDOW_H
#define LEVEL_METER_HISTOGRAM_WINDOW_H

#include "global_data.hpp"
#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QtCharts/QLineSeries>
#include <QtCharts/QAreaSeries>
#include <QComboBox>
#include <QtCharts>
#include <QGroupBox>
#include "qcustomplot.h"

class LevelMeterHistogramWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LevelMeterHistogramWindow(QWidget *parent = nullptr);
    ~LevelMeterHistogramWindow();
    void initUI();
    void changeEvent(QEvent *event);
    void setImage(QPixmap *image);
    void handleUpdatePointsInfo(const QPixmap *image, QRectF rect);
    void setStatusBarState(QString text);
    void closeWindow();

protected:
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void retranslateUi();
    QGroupBox *createControlShowGroupBox();
    QTableWidget *createStatisticTable(QWidget *table_widget);
    QGroupBox *createAxisControlBar(QWidget *table_widget);
    QWidget *createTableMenu();
    void updateTabItemContents();
    void cellEdited(QLineEdit *edit);
    void closeEvent(QCloseEvent *event);
    QCustomPlot *createChartView();

signals:
    void sigChildWindowEvent(int window_type);

private slots:
    void handleSelectShowColor(bool checked_state);

private:
    QCustomPlot *customPlot;
    QCPGraph *r_line;
    QCPGraph *g_line;
    QCPGraph *b_line;
    QCPGraph *y_line;

    QTableWidget *stat_table;
    QCheckBox *r_check_box;
    QCheckBox *g_check_box;
    QCheckBox *b_check_box;
    QCheckBox *y_check_box;
    QCheckBox *bar_r_check_box;
    QCheckBox *bar_g_check_box;
    QCheckBox *bar_b_check_box;
    QCheckBox *bar_y_check_box;
    QLabel *status_label;
    QLineEdit *max_range_edit;
    QLineEdit *min_range_edit;

    QList<double> statics_list;
};

#endif
