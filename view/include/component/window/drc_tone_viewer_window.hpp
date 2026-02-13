#ifndef DRC_TONE_VIEWER_WINDOW_HPP
#define DRC_TONE_VIEWER_WINDOW_HPP

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>
#include "hist_item_widget.hpp"
#include "base_page_widget.hpp"

class DrcToneViewerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DrcToneViewerWindow(QWidget *parent = nullptr);
    void initUI();
    void initEvent();

private:
    void initTopArea();
    void initCenterArea();
    void initItems();
    void initCharts();
    void initChartsNew();
    void initChartsMars3();
    void updateHistCharts(int idx);

private slots:
    void slotSendGetDrcHistogramInfoCmd();
    void slotUpdateAllCharts(int id);
    void slotHandleSelectColor(const QString &text);

private:
    QWidget *main_widget;
    QVBoxLayout *main_layout;
    QPushButton *update_btn;
    QLineEdit *step_edit;
    QLineEdit *expRatio_edit;
    HistItemWidget *global_tone_curve_widget;
    HistItemWidget *dark_tone_curve_widget;
    HistItemWidget *bright_tone_curve_widget;
    QList<QString> color_list;
    QComboBox *color_combobox;
    QJsonObject params_obj;

    QtCharts::QChart *chart;
    QtCharts::QChart *h_chart;
    QtCharts::QValueAxis *axis_x;
    QtCharts::QValueAxis *axis_y;
    QtCharts::QLineSeries *g_series;
    QtCharts::QLineSeries *b_series;
    QtCharts::QLineSeries *d_series;
    QtCharts::QLineSeries *l_series;
    QtCharts::QAreaSeries *g_area_series;
    QtCharts::QAreaSeries *b_area_series;
    QtCharts::QAreaSeries *d_area_series;
    QtCharts::QAreaSeries *l_area_series;
    QtCharts::QLineSeries *drc_series;
    QtCharts::QLineSeries *dci_series;
    QtCharts::QLineSeries *rlsc_r_series;
    QtCharts::QLineSeries *rlsc_g_series;
    QtCharts::QLineSeries *rlsc_b_series;
    QtCharts::QLineSeries *ae_series;
    QtCharts::QAreaSeries *drc_area_series;
    QtCharts::QAreaSeries *dci_area_series;
    QtCharts::QAreaSeries *rlsc_r_area_series;
    QtCharts::QAreaSeries *rlsc_g_area_series;
    QtCharts::QAreaSeries *rlsc_b_area_series;
    QtCharts::QAreaSeries *ae_area_series;
    BasePageWidget *ae_info_page;
    UniversalMatrixItemWidget *ae_hist_matrix;
};

#endif // DRC_TONE_VIEWER_WINDOW_HPP
