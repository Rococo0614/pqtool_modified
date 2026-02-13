#ifndef TABLE_CURVE_HPP
#define TABLE_CURVE_HPP

#include <QWidget>
#include "global_data.hpp"
#include "simple_line_chart_view.hpp"
#include "universal_table.hpp"
#include "spinner_item_widget.hpp"
#include <QSplitter>

class SimpleTableCurve : public QWidget
{
    Q_OBJECT

public:
    explicit SimpleTableCurve(QString module, QString title, QWidget *parent = nullptr);
    virtual void initUI();
    void initChart();
    virtual void setParam(QVariant param);
    void updatePoints(QList<QPoint> points);
    void setAxis(double, double, double, double);
    void getAxis(double *, double *, double *, double *);
    void setUnivTable(UniversalTable *univ_table);
    void setAxisXLabelList(QStringList);
    void refreshChart();
    bool isMultRow() {return is_multRow;};

public slots:
    virtual void slotSwitchChart();
    void slotUpdateLineCurve(double line_slope, double line_intercept);
    void slotUpdateGaussianCurve(double gaussian_mean, double gaussian_std, double gaussian_gain);
    void slotUpdateGammaCurve(double gamma);

protected slots:
    void slotRefreshChart();
    void slotChartChange();
    void updateMousePos(double x, double y);
    void slotTableClicked();
    void slotPointSelected(int);

protected:
    virtual void initLeftArea();
    virtual void retranslateUi();
    virtual void initCenterArea();
    QList<QPoint> getDefaultPoints();

signals:
    void sigValueChanged();
    void sigWritePageData();

protected:
    QString title;
    QString module;
    double MIN_X{0};
    double MAX_X{256};
    double MIN_Y{0};
    double MAX_Y{255};
    QHBoxLayout *main_layout;
    QWidget *pageWidget;
    QLabel *pos_label;
    SimpleLineChartView *simple_line_char;
    QJsonObject param_obj;
    UniversalTable *univ_table{nullptr};
    QSplitter *splitter;
    bool is_multRow{false};
    int select_col{0};
    int row{1};
    int col{1};
};


class MethodTableCurve : public SimpleTableCurve
{
    Q_OBJECT

public:
    explicit MethodTableCurve(QString module, QString title, QWidget *parent = nullptr);
    virtual  void initUI() override;
    virtual void setParam(QVariant param) override;

public slots:
    void slotSwitchChart() override;

private slots:
    void resetCurve();
    void slotCurveParamChanged(QVariant);

private:
    virtual void retranslateUi() override;
    virtual void initCenterArea() override;
    void generate_curve();
    void generate_gaussian_curve(double sigma, int *nearestX);
    void generate_sqrt_curve(double alpha, double beta, int *delta);
    int findNearestX(double target, double stddev);
    double gaussian(double x, double stddev);

private:
    QPushButton *reset_button;
    QString curve_method;
    SpinnerItemWidget *param_adjust_spinner;
};

#endif
