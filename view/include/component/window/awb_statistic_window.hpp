#ifndef AWB_STATISTIC_WINDOW_HPP
#define AWB_STATISTIC_WINDOW_HPP

#include <QWidget>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QMap>
#include <QCloseEvent>
#include <QCheckBox>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include "hist_chart_view.hpp"
#include "image_label.hpp"

const QString KEY_ATTR_LE_WB_GLOBAL_R = "WB Global R";
const QString KEY_ATTR_LE_WB_GLOBAL_G = "WB Global G";
const QString KEY_ATTR_LE_WB_GLOBAL_B = "WB Global B";
const QString KEY_ATTR_LE_WB_GLOBAL_ALL = "WB Global CountAll";
const QString KEY_ATTR_LE_WB_ZONE_R = "WB Zone R";
const QString KEY_ATTR_LE_WB_ZONE_G = "WB Zone G";
const QString KEY_ATTR_LE_WB_ZONE_B = "WB Zone B";
const QString KEY_ATTR_LE_WB_ZONE_ALL = "WB Zone CountAll";
const QString KEY_ATTR_SE_WB_GLOBAL_R = "SE WB Global R";
const QString KEY_ATTR_SE_WB_GLOBAL_G = "SE WB Global G";
const QString KEY_ATTR_SE_WB_GLOBAL_B = "SE WB Global B";
const QString KEY_ATTR_SE_WB_GLOBAL_ALL = "SE WB Global CountAll";
const QString KEY_ATTR_SE_WB_ZONE_R = "SE WB Zone R";
const QString KEY_ATTR_SE_WB_ZONE_G = "SE WB Zone G";
const QString KEY_ATTR_SE_WB_ZONE_B = "SE WB Zone B";
const QString KEY_ATTR_SE_WB_ZONE_ALL = "SE WB Zone CountAll";
const QString KEY_ATTR_IS_WDR_MODE = "isWdrmode";

const QString KEY_AWB_CURVE_R = "AWB Curve R";
const QString KEY_AWB_CURVE_B = "AWB Curve B";
const QString KEY_AWB_CURVE_B_TOP = "AWB Curve B Top";
const QString KEY_AWB_CURVE_B_BOT = "AWB Curve B Bot";
const QString KEY_AWB_BALANCE_R = "AWB Balance R";
const QString KEY_AWB_BALANCE_B = "AWB Balance B";
const QString KEY_AWB_CURVE_LEFT_LIMIT = "AWB Curve Left Limit";
const QString KEY_AWB_CURVE_RIGHT_LIMIT = "AWB Curve Right Limit";
const QString KEY_AWB_EXTRA_LIGHT_ENABLED = "ExtraLightEn";
const QString KEY_AWB_LIGHTINFO0_WHITE_RGAIN = "LightInfo[0].WhiteRgain";
const QString KEY_AWB_LIGHTINFO0_WHITE_BGAIN = "LightInfo[0].WhiteBgain";
const QString KEY_AWB_LIGHTINFO0_LIGHT_STATUS = "LightInfo[0].LightStatus";
const QString KEY_AWB_LIGHTINFO0_RADIUS = "LightInfo[0].Radius";
const QString KEY_AWB_LIGHTINFO1_WHITE_RGAIN = "LightInfo[1].WhiteRgain";
const QString KEY_AWB_LIGHTINFO1_WHITE_BGAIN = "LightInfo[1].WhiteBgain";
const QString KEY_AWB_LIGHTINFO1_LIGHT_STATUS = "LightInfo[1].LightStatus";
const QString KEY_AWB_LIGHTINFO1_RADIUS = "LightInfo[1].Radius";
const QString KEY_AWB_LIGHTINFO2_WHITE_RGAIN = "LightInfo[2].WhiteRgain";
const QString KEY_AWB_LIGHTINFO2_WHITE_BGAIN = "LightInfo[2].WhiteBgain";
const QString KEY_AWB_LIGHTINFO2_LIGHT_STATUS = "LightInfo[2].LightStatus";
const QString KEY_AWB_LIGHTINFO2_RADIUS = "LightInfo[2].Radius";
const QString KEY_AWB_LIGHTINFO3_WHITE_RGAIN = "LightInfo[3].WhiteRgain";
const QString KEY_AWB_LIGHTINFO3_WHITE_BGAIN = "LightInfo[3].WhiteBgain";
const QString KEY_AWB_LIGHTINFO3_LIGHT_STATUS = "LightInfo[3].LightStatus";
const QString KEY_AWB_LIGHTINFO3_RADIUS = "LightInfo[3].Radius";
const QString KEY_AWB_CALIBRATION_STATUS = "AWB Calibration Status";
const QString KEY_AWB_CROP_INFO = "AWB WinCrop";
const QString KEY_AWB_CURVE_REGION = "AWB CurveRegion";
const QString KEY_AWB_CALIB_INFO = "AWB Calib PT";
const QString KEY_AWB_CROP_STATUS = "AWB WinCrop Status";

const QString KEY_AWB_DEBUG_BIN = "AWB Debug Bin";

#define AWB_GAIN_BASE 1024
#define AWB_GAIN_SMALL_BASE 256
#define ONE_BASE 1000
typedef unsigned char           CVI_U8;
typedef unsigned short          CVI_U16;
typedef unsigned int            CVI_U32;
typedef unsigned int            CVI_HANDLE;

typedef signed char             CVI_S8;
typedef char                    CVI_CHAR;
typedef short                   CVI_S16;
typedef int                     CVI_S32;

#define AWB_DBG_VER	(11)
#define AWB_SENSOR_NUM (2)
#define AWB_DBG_W	(64)
#define AWB_DBG_H	(32)
#define AWB_SHIFT_REGION_NUM	(4)
#define AWB_DEBG_RESERVE_SIZE	(32)
#define AWB_CALIB_PTS_NUM (3)

typedef struct _AWB_DBG_S {
    //Don't modify ==>
    CVI_U32 u32Date;
    CVI_U16 u16AlgoVer;
    CVI_U16 u16DbgVer;
    CVI_U16 u16MaxFrameNum;
    CVI_U16 u16SensorId;
    CVI_U32 u32BinSize;
    CVI_U16 u16WinWnum;
    CVI_U16 u16WinHnum;
    CVI_U16 u16WinOffX;
    CVI_U16 u16WinOffY;
    CVI_U16 u16WinWsize;
    CVI_U16 u16WinHsize;
    CVI_U16 u16P_R[2][AWB_DBG_W*AWB_DBG_H];//keep Max Size
    CVI_U16 u16P_G[2][AWB_DBG_W*AWB_DBG_H];//keep Max Size
    CVI_U16 u16P_B[2][AWB_DBG_W*AWB_DBG_H];//keep Max Size
    CVI_U16 u16P_type[2][AWB_DBG_W*AWB_DBG_H];//keep Max Size
    CVI_U16 u16GrayCnt;
    CVI_U16 u16BalanceR;
    CVI_U16 u16BalanceB;
    CVI_U16 u16FinalR;
    CVI_U16 u16FinalB;
    CVI_U16 u16CurrentR;
    CVI_U16 u16CurrentB;
    CVI_U16 u16IdealR;
    CVI_U16 u16IdealB;
    CVI_U16 CalibRgain[AWB_CALIB_PTS_NUM];
    CVI_U16 CalibBgain[AWB_CALIB_PTS_NUM];
    CVI_U16 CalibTemp[AWB_CALIB_PTS_NUM];
    CVI_U16 u16CurveR[256];//4096/16
    CVI_U16 u16CurveB[256];
    CVI_U16 u16CurveB_Top[256];
    CVI_U16 u16CurveB_Bot[256];
    CVI_S16 s16LvX100;
    CVI_U32 u32ISONum;
}s_AWB_DBG_S;

typedef enum _AWB_DBG_PT {
    AWB_DBG_PT_NO_USE = 0,
    AWB_DBG_PT_OUT_CURVE,
    AWB_DBG_PT_IN_CURVE,
} AWB_DBG_PT;


class AWBStatWindow: public QMainWindow
{
    Q_OBJECT

public:
    explicit AWBStatWindow(QWidget *parent = nullptr);
    void handleUpdateTheme(bool dark_theme_enabled);
    int h_wnd = GRID_AWB_COL, v_wnd = GRID_AWB_ROW;

public slots:
    void updateStatistic(QVariant);
    void updateStatistic(QMap<QString, int> &);
    void exportStatistic();
    void updateZoneSatPoints(float *, float *, float *, float *, s_AWB_DBG_S *pAwbDbg);
    void updateGlobalSatPoint(float x0, float y0, float x1, float y1);
    void updateResultPoint(float x, float y);
    void updateWhiteZoneCurvesMid(int num, float *tableX, float *tableY);
    void updateWhiteZoneCurvesTop(int num, float *tableX, float *tableY);
    void updateWhiteZoneCurvesBot(int num, float *tableX, float *tableY);
    void updateCurveLeftLimit(float x, float y);
    void updateCurveRightLimit(float x, float y);
    void updateSelectedPoint(float x0, float y0, float x1, float y1);
    void updateCalibrationStatus(int status);
    void updateExtraLightSource(bool enabled, int *status, float x[][25], float y[][25]);
    void updateCalibPoint(int calib_pt[3][3]);
    void updateCurveRegion(int region[4]);
private slots:
    void slotSetShowStatistics();
    void slotSetShowPlanckianCurve();
    void slotSetShowWhiteZone();
    void slotCurrentTabChanged(int index);

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
    void initRightItems();
    void addSeries(QtCharts::QChart *chart);
    void removeSeries(QtCharts::QChart *chart);

private:
    QHBoxLayout *main_layout;
    QWidget *main_widget;
    QPen pen_dot_green, pen_dot_red;

    QTableWidget *stat_table;
    QLabel *calibration_status;
    QtCharts::QValueAxis *axis_x;
    QtCharts::QValueAxis *axis_y;
    QtCharts::QScatterSeries *scatter_series_zone[2];
    QtCharts::QScatterSeries *scatter_series_zone_black[2];
    QtCharts::QScatterSeries *scatter_series_global[2];
    QtCharts::QScatterSeries *scatter_series_result;
    QtCharts::QScatterSeries *scatter_series_selected[2];
    QtCharts::QLineSeries *line_series_mid;
    QtCharts::QLineSeries *line_series_top;
    QtCharts::QLineSeries *line_series_bot;
    QtCharts::QLineSeries *line_series_left_limit;
    QtCharts::QLineSeries *line_series_right_limit;
    QtCharts::QLineSeries *line_series_extra_LS[4];
    QtCharts::QLineSeries *line_region_r[3];
    QtCharts::QScatterSeries *calib_point[3];
    QtCharts::QChart *chart_le;
    QtCharts::QChart *chart_se;
    QtCharts::QChartView *chart_view_le;
    QtCharts::QChartView *chart_view_se;
    QCheckBox *show_statistics;
    QCheckBox *show_planckian_curve;
    QCheckBox *show_white_zone;
    int tab_index{0};
    int wdr_mode;
};

#endif //AF_STATISTIC_WINDOW_HPP
