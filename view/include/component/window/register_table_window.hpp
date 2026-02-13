#ifndef REGISTER_TABLE_WINDOW_HPP
#define REGISTER_TABLE_WINDOW_HPP

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include "module_define.hpp"
#include "curve_set_dialog.hpp"
#include "table_curve.hpp"

enum REG_WINDOW_CONTENT {
    DEFAULT_CONTENT,
    CONTENT_MATRIX = DEFAULT_CONTENT,
    CONTENT_CURVE,
    CONTENT_HIST
};

class RegisterTableWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegisterTableWindow(QWidget *parent = nullptr);
    RegisterTableWindow(QString module, QWidget *parent = nullptr);
    RegisterTableWindow(int type, QWidget *parent = nullptr);
    RegisterTableWindow(int type, QString module, QWidget *parent = nullptr);
    ~RegisterTableWindow();
    void setWidget(QWidget *widget);
    void setDesp(QString desp);
    void setReadOnly(bool m);
    void setContentType(int t);
    void switchContent(int t);
    void setSwitchButton(bool enabled);
    void setImportButton(bool enabled);
    void setExportButton(bool enabled);
    void addIsoWidget(QWidget *widget);
    void setRange(double min_x, double max_x, double min_y, double max_y);

signals:
    void sigSwithChart();

protected:
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void initUI();
    void initMenu();
    void retranslateUi();
    void refresh();
    void connectEvent(QWidget *widget);
    void disconnectEvent(QWidget *widget);

signals:
    void sigExportFile(QString file_path);
    void sigImportFile(QString file_path);
    void sigHideSumColumn(bool state);

private slots:
    void handleImportFile();
    void handleExportFile();
    void handleSwitchContent();
    void readCurrentPageData();
    void writeCurrentPageData();
    void switchChart();
    void handleCCMHideEvent();
    void openCurveSettingDialog();

public slots:
    void showCCMSumButton(bool state);

private:
    QWidget *main_widget;
    QGridLayout *main_layout;
    QPushButton *import_btn{nullptr};
    QPushButton *export_btn{nullptr};
    QPushButton *read_btn{nullptr};
    QPushButton *write_btn{nullptr};
    QPushButton *chart_btn{nullptr};
    QPushButton *chart_setting_btn{nullptr};
    QLabel *desp_label;
    int content_type{CONTENT_MATRIX};
    QWidget *curve_widget{nullptr};
    QWidget *iso_table{nullptr};
    bool switch_enabled{false};
    QPushButton *switch_btn{nullptr};
    QPushButton *ccm_sum_btn{nullptr};
    QString module_name{""};
    bool sum_column_hide{false};
    CurveSetDialog *curve_set_dialog{nullptr};
    double min_x{0};
    double max_x{0};
    double min_y{0};
    double max_y{0};
public:
    SimpleTableCurve *table_curve{nullptr};
};

#endif // REGISTER_TABLE_WINDOW_HPP
