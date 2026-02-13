#ifndef VO_GAMMA_PAGE_WIDGET_H
#define VO_GAMMA_PAGE_WIDGET_H

#include "base_page_widget.hpp"
#include "binary_item_widget.hpp"
#include "combobox_item_widget.hpp"
#include "spinner_item_widget.hpp"
#include "double_spinner_item_widget.hpp"
#include "file_dialog.hpp"
#include "gamma_chart_item_widget.hpp"


class VOGammaPageWidget : public BasePageWidget
{
    Q_OBJECT

public:
    explicit VOGammaPageWidget(QWidget *parent = nullptr);
    virtual void updateUI() override;
    void initEvent();
    void slotRefreshChart();

protected:
    void initLeftArea() override;
    void initCenterArea() override;
    void paintEvent(QPaintEvent *event) override;
    virtual void retranslateUi() override;
    virtual void paegUpdateTheme(bool dark_theme_enabled) override;
    virtual QScrollArea *getPageScrollArea() override {return scroll_area;}

private slots:
    void slotParamsChanged(QVariant);
    void slotControlPointsNumChanged(QVariant);
    void openLoadDialog();
    void openSaveDialog();
    void handleSelectFile(QString format, QString file_path, MODE mode);
    void updateMousePos(double x, double y);
    void resetCurve();
    void slotReferSetSelect(QVariant);
    void slotReferSetSave();
    void slotReferSetApply();
    void slotRefreshTable();

private:
    QList<QPointF> getVOGammaPoints();
    QList<QPointF> initPoints(double coefficient, double slope);
    void initGammaWidgets();
    void initChart();
    QLayout *initButtons();
    void setAjustButtons(int mode);

private:
    BinaryItemWidget *gamma_enable;
    BinaryItemWidget *osd_enable;
    DoubleSpinnerItemWidget *coeff_spinner;
    DoubleSpinnerItemWidget *slope_spinner;
    SpinnerItemWidget *control_points_spinner;
    GammaChartItemWidget *line_chart_widget;
    FileDialog *file_select_dialog;
    QPushButton *load_button;
    QPushButton *save_button;
    QPushButton *reset_button;
    UniversalMatrixItemWidget *vo_gamma_table;
    QComboBox *reference_set_combo;
    QPushButton *reference_save_button;
    QPushButton *reference_apply_button;
    QLabel *pos_label;
    int control_points_num{2};
    QMap<int, QVariantList> refer_set;
    QScrollArea *scroll_area;
};

#endif // VO_GAMMA_PAGE_WIDGET_H
