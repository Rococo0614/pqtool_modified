#ifndef CURVE_USERDEFINE_HPP
#define CURVE_USERDEFINE_HPP

#include <memory>
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>

#include "i_observer.hpp"
#include "cubic_curve_chart_view.hpp"
#include "double_spinner_item_widget.hpp"
#include "matrix_item_widget.hpp"
#include "spinner_item_widget.hpp"
#include "hist_chart_view.hpp"
#include "hist_item_widget.hpp"

class CurveUserDefine : public QWidget, public IObserver
{
    Q_OBJECT
public:
    CurveUserDefine();
    virtual void refreshData(const SecKeyType &reg_section_key) override;
    QVariant getData();
    void setData(QVariant data);
    void setRange(int min, int max);
    void initUI();
    void initEvent();
    void handleUpdateTheme(bool dark_theme_enabled);

private:
    QVariant getValue(const SecKeyType &reg_section_key);
    QVariant setValue(const SecKeyType &reg_section_key, QVariant value);
    // get ui item value by value mapping
    QVariant getValueByValueMapping(QVariantList &&values) const;
    // get reg value by reg's idx through value mapping table (for mapping to multiple regs)
    QVariant getRegValueByValueMapping(QVariant value, SecKeyType &&reg_section_key) const;
    // get reg values by value mapping table (for value is array)
    QVariantList getRegValueListByValueMapping(QVariant value) const;
    QVariant validateValue(QVariant value);
    void initItemWidgets();
    void initLeftArea();
    void initCenterArea();
    void init_chart();
    QList<QPointF> initDefaultPoints();

signals:
    void sigValueChanged(QVariant value);
    void sigPageWriteData();

public slots:
    void handleImportTable(QString file_path);
    void handleExportTable(QString file_path);
    void slotControlPointsNumChanged(QVariant num);
    void handleAddCtrlPt();
    void handleSetEvent();
    void updateMousePos(double x, double y);
    void resetCurve();
    void slotReferSetSelect(QVariant);
    void slotReferSetSave();
    void slotReferSetApply();
    void slotUpdateWdrHistogram(QVariant );
    void slotUpdateExpRatio(QVariant);
    void handlePageWriteData();
    void handleValueChanged(QVariant);

private:
    QVariant value_min = {0};
    QVariant value_max = {4095};
    CubicCurveChartView *chart;
    QHBoxLayout *main_layout;

    int control_points_num{2};
    SpinnerItemWidget *control_points_spinner;
    QPushButton *reset_button;
    //MatrixItemWidget *edit_matrix_button;
    QComboBox *reference_set_combo;
    QPushButton *reference_save_button;
    QPushButton *reference_apply_button;
    SpinnerItemWidget *x_adjust_spinner;
    SpinnerItemWidget *y_adjust_spinner;
    QLabel *pos_label;
    QPushButton *add_ctrlpoint_btn;
    QPushButton *set_btn;
    QMap<int, QVariantList> refer_set;
    SpinnerItemWidget *exp_ratio;

//    HistItemWidget *wdr_hist_widget;
};

#endif // CURVE_USERDEFINE_HPP
