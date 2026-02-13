#ifndef CALIBRATION_WINDOW_H
#define CALIBRATION_WINDOW_H

#include "calibration_utils.hpp"
#include "calibration_image_list_widget.hpp"
#include "base_calibration_page_widget.hpp"
#include "raw_image_format_dialog.hpp"
#include "module_tab_widget.hpp"
#include "clut_calibration_page_widget.hpp"

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QAction>


class CalibrationWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CalibrationWindow(QWidget *parent = nullptr);
    ~CalibrationWindow();

protected:
    void keyReleaseEvent(QKeyEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    void initUI();
    void initEvent();
    void initMenu();
    void retranslateUi();

private slots:
    void handleOpenRawFile();
    void handleLoadParam();
    void handleSaveParam();
    void handleUpdateRawConfig();
    void handleImportRaw(BaseCalibrationPageWidget *cali_widget);
    void handleImportMultiRaw(BaseCalibrationPageWidget *cali_widget);
    void handleUpdateStatus(CALIBRATION_STATUS status);
    void handleUpdateInfo(int index);
signals:
    void sigCloseWindow();

private:
    QWidget *main_widget;
    QGridLayout *main_layout;
    CalibrationImageListWidget *image_list_widget;
    ModuleTabWidget *calibration_module_tab_widget;
    QLabel *status_label;
    QAction *open_file_action;
    QAction *load_param_action;
    QAction *save_param_action;
    QLineEdit *raw_config_line_edit;
    RawConfigItem raw_config_item;
};

#endif // CALIBRATION_WINDOW_H
