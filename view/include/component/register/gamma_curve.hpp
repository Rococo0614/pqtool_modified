#ifndef GAMMA_CURVE_HPP
#define GAMMA_CURVE_HPP

#include <QWidget>
#include "binary_item_widget.hpp"
#include "combobox_item_widget.hpp"
#include "spinner_item_widget.hpp"
#include "uint_spinner_item_widget.hpp"
#include "double_spinner_item_widget.hpp"
#include "universal_matrix_item_widget.hpp"
#include "file_dialog.hpp"
#include "gamma_chart_item_widget.hpp"
#include "group_widget.hpp"
#include "global_data.hpp"



class GammaCurve : public QWidget
{
    Q_OBJECT

public:
    explicit GammaCurve(QString module, QString title, QWidget *parent = nullptr);
    virtual void initUI();
    void initEvent();
    void slotRefreshChart();
    void setAxis(double x, double y);
    virtual void initChart();
    void setParamArr(QJsonArray arr);
    virtual void setParam(QVariant param);
    virtual void setData(QVariant data);
    virtual QVariant getData();
    void updatePoints(QList<QPointF> points, bool sync);
    virtual void onOpenWindow() {};

protected:
    virtual void initLeftArea();
    void initCenterArea();
    void paintEvent(QPaintEvent *event);
    virtual void retranslateUi();
    virtual QScrollArea *getPageScrollArea() {return scroll_area;}

protected slots:
    virtual void slotParamsChanged(QVariant);
    void slotControlPointsNumChanged(QVariant);
    void openLoadDialog();
    void openSaveDialog();
    void handleSelectFile(QString format, QString file_path, MODE mode);
    void updateMousePos(double x, double y);
    void resetCurve();
    void slotReferSetSelect(QVariant);
    void slotReferSetSave();
    void slotReferSetApply();
    virtual void slotChartChange();

private:
    QList<QPointF> getDefaultPoints();
    QLayout *initButtons();

signals:
    void sigValueChanged();
    void sigWritePageData();

protected:
    GroupItemWidget *adjust_group_widget;
    GammaChartItemWidget *line_chart_widget;
    FileDialog *file_select_dialog;
    QPushButton *load_button;
    QPushButton *save_button;
    QPushButton *reset_button;
    SpinnerItemWidget *control_points_spinner;
    QComboBox *reference_set_combo;
    QPushButton *reference_save_button;
    QPushButton *reference_apply_button;
    QLabel *pos_label;
    QScrollArea *scroll_area;
    QWidget *pageWidget;
    int control_points_num{2};
    QMap<int, QVariantList> refer_set;
    QString title;
    QString module;
    double MAX_X{256};
    double MAX_Y{255};
    QHBoxLayout *main_layout;
    QJsonArray related_params_json;
    QMap<int, ItemValueType> related_params_value;
    QList<BaseItemWidget *> related_params_widget;
};


class GammaCurveLV : public GammaCurve
{
    Q_OBJECT

public:
    explicit GammaCurveLV(QString module, QString title, QWidget *parent = nullptr);
    void initUI();
    void setData(QVariant data);
    QVariant getData();
    void setParam(QVariant param);

private:
    void initLeftArea();
    void updateRelatedWidget();

private slots:
    void slotChartChange();
    void slotRefreshChart();
    void slotRelatedChanged(QVariant);
    void slotCheckLvValue(QVariant);

private:
    ComboBoxItemWidget *table_choice;
    QList<BaseItemWidget *> gamma_table_list;
    QList<BaseItemWidget *> gamma_lv_list;
    BaseItemWidget *gamma_table_num;
    BaseItemWidget *dci_gamma;
    BaseItemWidget *dci_offset;
    BaseItemWidget *dci_contrast;
    SpinnerItemWidget *dci_gamma_spinner;
    SpinnerItemWidget *dci_offset_spinner;
    SpinnerItemWidget *dci_contrast_spinner;
};


class GammaCurveRGB : public GammaCurve
{
    Q_OBJECT

public:
    explicit GammaCurveRGB(QString module, QString title, QWidget *parent = nullptr);
    void initUI();

private:
    void initLeftArea();
    QList<QPointF> initPoints(double coefficient, double slope);
    void addLine2Chart();
    QtCharts::QLineSeries *createLineSeries(QString name, QColor color, QList<QPointF> points);

private slots:
    void slotParamsChanged(QVariant);

private:
    DoubleSpinnerItemWidget *coeff_spinner;
    DoubleSpinnerItemWidget *slope_spinner;
};

class MapCurve : public GammaCurve
{
    Q_OBJECT

public:
    explicit MapCurve(QString module, QString title, QWidget *parent = nullptr);
    void initUI();
    void setParam(QVariant param);
    void setData(QVariant data);
    QVariant getData();
    void initChart();
    void onOpenWindow();

private:
    void initCenterArea();
    QLayout *initCenterTop();
    void retranslateUi();
    void initEvent();
    int getLumaByX(int x);
    QtCharts::QLineSeries *createLineSeries(QString name, QColor color);

private slots:
    void slotSendGetRecFshdrMapcurveInfoCmd();
    void slotRefreshCurve(int id);

private:
    QPushButton *refresh_button;
    CubicCurveChartView *map_curve_chart;
    QtCharts::QLineSeries *series_l;
    QtCharts::QLineSeries *series_r;
    QtCharts::QLineSeries *series_end;
    int ev_fmt;
    int knee_by_ev;
    int max_value_by_ev;
};

#endif
