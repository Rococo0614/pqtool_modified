#ifndef GAMMA_PAGE_WIDGET_H
#define GAMMA_PAGE_WIDGET_H

#include "base_page_widget.hpp"
#include "binary_item_widget.hpp"
#include "combobox_item_widget.hpp"
#include "spinner_item_widget.hpp"
#include "double_spinner_item_widget.hpp"
#include "file_dialog.hpp"
#include "gamma_chart_item_widget.hpp"


class GammaPageWidget : public BasePageWidget
{
    Q_OBJECT

public:
    explicit GammaPageWidget(QWidget *parent = nullptr);
    virtual void updateUI() override;
    void initEvent();
    void slotRefreshChart();

public slots:
    virtual void readData() override;

protected:
    void initLeftArea() override;
    void initCenterArea() override;
    void paintEvent(QPaintEvent *event) override;
    virtual void retranslateUi() override;
    virtual void paegUpdateTheme(bool dark_theme_enabled) override;
    void getGammaTableByType();
    virtual void readModuleFromNetworkJsonrpc() override;
    virtual QScrollArea *getPageScrollArea() override {return scroll_area;} 

private slots:
    void slotParamsChanged(QVariant);
    void slotControlPointsNumChanged(QVariant);
    void openLoadDialog();
    void openSaveDialog();
    void handleSelectFile(QString format, QString file_path, MODE mode);
    void handleAddCtrlPt();
    void handleSetEvent();
    void updateMousePos(double x, double y);
    void resetCurve();
    void slotReferSetSelect(QVariant);
    void slotReferSetSave();
    void slotReferSetApply();
    void slotXAdjustChanged(QVariant);
    void slotYAdjustChanged(QVariant);
    void slotAjustUpdate(QPointF p);
    void slotRefreshTable();
    void slotHandleSelectLine(const QString &text);

private:
    QList<QPointF> getDefaultPoints();
    QList<QPointF> getSRGBPoints();
    QList<QPointF> getUserDefinePoints();
    QList<QPointF> getAutoGammaPoints(int index);
    QList<QPointF> initPoints(double coefficient, double slope);
    void initGammaWidgets();
    void initChart();
    QLayout *initButtons();
    void setAjustButtons(int mode);
    QLayout *initCombobox();
    QtCharts::QLineSeries *createLineSeries(QString name, QColor color, QList<QPointF> points);

private:
    BinaryItemWidget *gamma_enable;
    ComboBoxItemWidget *gamma_mode;
    DoubleSpinnerItemWidget *coeff_spinner;
    DoubleSpinnerItemWidget *slope_spinner;
    SpinnerItemWidget *control_points_spinner;
    SpinnerItemWidget *UpdateInterval_spinner;
    GammaChartItemWidget *line_chart_widget;
    FileDialog *file_select_dialog;
    QPushButton *load_button;
    QPushButton *save_button;
    QPushButton *reset_button;
    UniversalMatrixItemWidget *temp_table;
    UniversalMatrixItemWidget *default_table;
    UniversalMatrixItemWidget *srgb_table;
    UniversalMatrixItemWidget *user_define_table;
    UniversalMatrixItemWidget *auto_gamma_table0;
    UniversalMatrixItemWidget *auto_gamma_table1;
    UniversalMatrixItemWidget *auto_gamma_table2;
    UniversalMatrixItemWidget *auto_gamma_table3;
    UniversalMatrixItemWidget *auto_gamma_table4;
    ComboBoxItemWidget *table_choice;
    SpinnerItemWidget *gamma_table_number;
    SpinnerItemWidget *auto_gamma_lv0;
    SpinnerItemWidget *auto_gamma_lv1;
    SpinnerItemWidget *auto_gamma_lv2;
    SpinnerItemWidget *auto_gamma_lv3;
    SpinnerItemWidget *auto_gamma_lv4;
    QComboBox *reference_set_combo;
    QPushButton *reference_save_button;
    QPushButton *reference_apply_button;
    SpinnerItemWidget *x_adjust_spinner;
    SpinnerItemWidget *y_adjust_spinner;
    QLabel *pos_label;
    QPushButton *add_ctrlpoint_btn;
    QPushButton *set_btn;
    int control_points_num{2};
    QMap<int, QVariantList> refer_set;
    int adjust_mode{0};
    int mode[3]{0, 0, 0};
    int choice[3]{0, 0, 0};
    QScrollArea *scroll_area;
    QComboBox *line_combobox;
    QtCharts::QLineSeries *series_sRGB;
    QtCharts::QLineSeries *series_bt709;
    QList<QString> line_list;
};

#endif // GAMMA_PAGE_WIDGET_H
