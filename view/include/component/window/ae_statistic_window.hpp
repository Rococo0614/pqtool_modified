#ifndef AE_STATISTIC_WINDOW_HPP
#define AE_STATISTIC_WINDOW_HPP

#include <QWidget>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QMap>
#include <QCloseEvent>

#include "hist_chart_view.hpp"

const QString KEY_ATTR_FE0_HIST = "FE0 Histogram";
const QString KEY_ATTR_FE0_GLOBAL_R = "FE0 Global R";
const QString KEY_ATTR_FE0_GLOBAL_GR = "FE0 Global Gr";
const QString KEY_ATTR_FE0_GLOBAL_GB = "FE0 Global Gb";
const QString KEY_ATTR_FE0_GLOBAL_B = "FE0 Global B";
const QString KEY_ATTR_FE0_ZONE_R = "FE0 Zone R";
const QString KEY_ATTR_FE0_ZONE_GR = "FE0 Zone Gr";
const QString KEY_ATTR_FE0_ZONE_GB = "FE0 Zone Gb";
const QString KEY_ATTR_FE0_ZONE_B = "FE0 Zone B";

const QString KEY_ATTR_FE1_HIST = "FE1 Histogram";
const QString KEY_ATTR_FE1_GLOBAL_R = "FE1 Global R";
const QString KEY_ATTR_FE1_GLOBAL_GR = "FE1 Global Gr";
const QString KEY_ATTR_FE1_GLOBAL_GB = "FE1 Global Gb";
const QString KEY_ATTR_FE1_GLOBAL_B = "FE1 Global B";
const QString KEY_ATTR_FE1_ZONE_R = "FE1 Zone R";
const QString KEY_ATTR_FE1_ZONE_GR = "FE1 Zone Gr";
const QString KEY_ATTR_FE1_ZONE_GB = "FE1 Zone Gb";
const QString KEY_ATTR_FE1_ZONE_B = "FE1 Zone B";
const QString KEY_ATTR_AE_WIN_CROP = "AE WinCrop";


class AEStatWindow: public QMainWindow
{
    Q_OBJECT

public:
    explicit AEStatWindow(QWidget *parent = nullptr);
    void handleUpdateTheme(bool dark_theme_enabled);

public slots:
    void updateStatistic(QVariant );
    void updateHist(QMap <QString, QVariantList> &hist_map);
    void updateStatistic(QMap<QString, int> &);
    void exportStatistic();

signals:
    void sigWinClose();

protected:
    virtual void closeEvent(QCloseEvent *event) override;

private:
    void initUI();
    void initEvent();
    void initMenu();
    void initLeftArea();
    void initRightArea();

private:
    QHBoxLayout *main_layout;
    QWidget *main_widget;

    QTableWidget *stat_table;
    HistChartView *fe0_histogram;
    HistChartView *fe1_histogram;
    int pipe{0};
    QVariantList fe0_list;
    QVariantList fe1_list;
};

#endif
