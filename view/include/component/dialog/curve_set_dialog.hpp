#ifndef CURVE_SET_DIALOG_H
#define CURVE_SET_DIALOG_H

#include "utils.hpp"
#include "spinner_item_widget.hpp"
#include "double_spinner_item_widget.hpp"
#include <QDialog>
#include <QComboBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QKeyEvent>
#include <QGroupBox>

class CurveSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CurveSetDialog(QWidget *parent = nullptr);
    explicit CurveSetDialog(double min_x, double max_x, double min_y, double max_y, QWidget *parent = nullptr);
    void initUI();

private:
    void retranslateUi();

private slots:
    void handleApply();
    void handleOk();
    void slotLineParamUpdate();
    void slotGaussianParamUpdate();
    void slotGammaParamUpdate();

public:
    void pub();

signals:
    void sigLineParamsChanged(double, double);
    void sigGaussianParamsChanged(double, double, double);
    void sigGammaParamsChanged(double);

private slots:
    void slotFunctionChange();

private:
    QGridLayout *main_layout;

    QLabel *function_type_label;
    QComboBox *function_type_combox;

    SpinnerItemWidget *line_slope;
    SpinnerItemWidget *line_intercept;

    SpinnerItemWidget *gaussian_mean;
    DoubleSpinnerItemWidget *gaussian_std;
    SpinnerItemWidget *gaussian_gain;

    DoubleSpinnerItemWidget *gamma;

    QPushButton *ok_btn;
    QPushButton *apply_btn;
    double min_x{0};
    double max_x{15};
    double min_y{0};
    double max_y{255};
};

#endif // CURVE_SET_DIALOG_H
