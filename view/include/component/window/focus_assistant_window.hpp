#ifndef FOCUS_ASSISTANT_WINDOW_HPP
#define FOCUS_ASSISTANT_WINDOW_HPP

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QPixmap>
#include <QCheckBox>
#include <QTimer>
#include <QJsonArray>
#include <QJsonObject>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>

#include "image_viewer_widget.hpp"
#include "af_statistic_window.hpp"

class FocusAssistantWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FocusAssistantWindow(QWidget *parent = nullptr);
    void initUI();
    void initEvent();

protected:
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void closeEvent(QCloseEvent *event) override;
    void updateFvCurve();
    void cropImage(QImage &source, QPixmap &destination);
    void resetData();

private slots:
    void slotSendCaptureImageCmd();
    void slotUpdateImage(QString cmd);
    void slotSendGrabDataCmd();
    void slotUpdateData(int id);
    void slotSetShowFvText();
    void slotSetCurrentFvTable(int index);
    void slotSelectAllZones();
    void slotDeselectAllZones();
    void slotResetFvChart();
    void handleImageStatus(int id);

public slots:
    void handleUpdateTheme(bool dark_theme_enabled);

private:
    QWidget *main_widget;
    QTimer *timer;
    QHBoxLayout *main_layout;
    QPushButton *capture_btn;
    QCheckBox *grab_af_statistics;
    QLineEdit *interval_edit;
    QComboBox *fv_item_combobox;
    QCheckBox *show_zone_fv;
    QPushButton *select_all_zones_btn;
    QPushButton *deselect_all_zones_btn;
    QPushButton *reset_fv_chart_btn;
    ImageLabel *img_label;
    QScrollArea *scroll_area;
    QImage image;
    QPixmap crop_image;
    bool crop_enable = false;
    int crop_x = 0, crop_y = 0, crop_w = 3840, crop_h = 2160;
    int h_wnd = GRID_AF_COL, v_wnd = GRID_AF_ROW;
    int fv_v1_now_table[GRID_AF_ROW][GRID_AF_COL] = {};
    int fv_v1_max_table[GRID_AF_ROW][GRID_AF_COL] = {};
    int fv_h1_now_table[GRID_AF_ROW][GRID_AF_COL] = {};
    int fv_h1_max_table[GRID_AF_ROW][GRID_AF_COL] = {};
    int fv_h2_now_table[GRID_AF_ROW][GRID_AF_COL] = {};
    int fv_h2_max_table[GRID_AF_ROW][GRID_AF_COL] = {};
    bool fv_selected_table[GRID_AF_ROW][GRID_AF_COL] = {};
    int zone_num_selected = 0;
    int fv_v1_avg_selected = 0;
    int fv_v1_max_selected = 0;
    int fv_h1_avg_selected = 0;
    int fv_h1_max_selected = 0;
    int fv_h2_avg_selected = 0;
    int fv_h2_max_selected = 0;
    vector<int> fv_v1_recent;
    vector<int> fv_h1_recent;
    vector<int> fv_h2_recent;
    QtCharts::QValueAxis *axis_x;
    QtCharts::QValueAxis *axis_y;
    QtCharts::QLineSeries *line_max_series;
    QtCharts::QLineSeries *line_series;
    QtCharts::QScatterSeries *scatter_series;
    QtCharts::QChart *chart;
    QtCharts::QChartView *chart_view;
    QByteArray yuv_head_data;
    QByteArray img_yuv_data;
    int total_counts = 0;
    int receive_counts = 0;
};

#endif // FOCUS_ASSISTANT_WINDOW_HPP
