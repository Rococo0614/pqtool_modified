#ifndef AUTO_ANALYSER_WINDOW_HPP
#define AUTO_ANALYSER_WINDOW_HPP

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QPixmap>
#include <QCheckBox>
#include <QThread>
#include <QTimer>
#include <QMap>
#include <QJsonArray>
#include <QJsonObject>

#include "image_viewer_widget.hpp"
#include "ae_statistic_window.hpp"
#include "af_statistic_window.hpp"
#include "awb_statistic_window.hpp"
#include "image_label.hpp"
#include "network_client_helper.hpp"

enum class IMG_TYPE
{
  IMG_YUV,
  IMG_RAW,
};

class AutoAnalyserWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AutoAnalyserWindow(QWidget *parent = nullptr);
    void initMenu();
    void initUI();
    void initEvent();
    void refresh();

protected:
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void closeEvent(QCloseEvent *event) override;
    void changeState(AutoAnaState state);
    void saveStatisticInfo(QJsonObject &items);
    void updateStatisticInfo(int row, int col);
    void setAEStatInfo();
    void setAFStatInfo();
    void setAWBStatInfo();
    void setImgSourceType(IMG_TYPE img_type);
    void cropImage(QImage &source, QPixmap &destination);
    void Get3AStatisticsInfo();
    void GetAWBBin();
    void sendCaptureRawCmd();
    void handleCaptureRawImg();

private slots:
    void slotUpdateImage(QString cmd);
    void slotUpdateData(int id);
    void slotCheckAEInfo();
    void slotCheckAFInfo();
    void slotCheckAWBInfo();
    void slotCloseInfoWindow();
    void slotSendCapCmd();
    void slotUpdateInfoOnSel(int row, int col);
    void slotChangeFocusItem(int idx);
    void handleImageStatus(int id);
    void HandleAWBBinStatus(int id);
    void HandleAWBBinData(QString cmd);
    void handleSingleRawStatus(int id);
    void slotUpdateRawData(QString cmd);

public slots:
    void handleUpdateTheme(bool dark_theme_enabled);

private:
    QWidget *main_widget;
    QVBoxLayout *main_layout;
    ImageLabel *img_label;
    QScrollArea *scroll_area;

    double factor{1.0};
    int mode{STATE_NONE};
    QLabel *status_label;
    QLabel *img_source_label;
    QLabel *check_state_label;
    QCheckBox *cap_checkbox;

    AEStatWindow *ae_window{nullptr};
    AFStatWindow *af_window{nullptr};
    AWBStatWindow *awb_window{nullptr};
    QMainWindow *cur_active_win{nullptr};

    int src_idx;
    QTimer *timer;
    int row_sel {-1};
    int col_sel {-1};
    QJsonObject all_data;
    QImage image;
    QPixmap crop_image;
    QByteArray receive_raw_data;
    QList<QByteArray> wdr_raw_data_list;

    int ae_win_crop[5] = {};
    // AWB Statistics Window
    bool updated = false;
    int wb_global_r[2] = {};
    int wb_global_g[2] = {};
    int wb_global_b[2] = {};
    int wb_global_count[2] = {};
    int wb_zone_r_table[2][GRID_AWB_ROW][GRID_AWB_COL] = {};
    int wb_zone_g_table[2][GRID_AWB_ROW][GRID_AWB_COL] = {};
    int wb_zone_b_table[2][GRID_AWB_ROW][GRID_AWB_COL] = {};
    int wb_zone_count_table[2][GRID_AWB_ROW][GRID_AWB_COL] = {};
    int wb_crop_state = 0;
    float table_rg[2][GRID_AWB_ROW * GRID_AWB_COL] = {};
    float table_bg[2][GRID_AWB_ROW * GRID_AWB_COL] = {};
    int num_white_zone_mid = 0;
    int num_white_zone_top = 0;
    int num_white_zone_bot = 0;
    float white_zone_mid_x[256] = {};
    float white_zone_mid_y[256] = {};
    float white_zone_top_x[256] = {};
    float white_zone_top_y[256] = {};
    float white_zone_bot_x[256] = {};
    float white_zone_bot_y[256] = {};
    int blance_r = 0;
    int blance_b = 0;
    int curve_left_limit = 0;
    int curve_right_limit = 0;
    bool extra_light_enabled = 0;
    int lightInfo_white_rgain[4] = {};
    int lightInfo_white_bgain[4] = {};
    int lightInfo_light_status[4] = {};
    int lightInfo_radius[4] = {};
    float light_souce_x[4][25] = {};
    float light_souce_y[4][25] = {};
    int calibration_status = 0;
    int wb_win_crop[6] = {};
    int wb_curve_region[4] = {};
    int wb_calib_pt[3][3]; //3 point, Rgain,Bgain,TempK
    s_AWB_DBG_S sAwb_dbg = {};
    int awbGridRow;
    int awbGridCol;
    bool use_yuv_show = true;
    bool get_raw_finish = true;
    int timeout_times = 0;
    int raw_size = 0;
    int wdr_mode = 0;
};

#endif // AUTO_ANALYSER_WINDOW_HPP
