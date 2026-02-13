#ifndef FISH_EYE_UTILITY_WINDOW_HPP
#define FISH_EYE_UTILITY_WINDOW_HPP

#include "grid_image_viewer_widget.hpp"
#include "ctrl_panel_window.hpp"
#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QPixmap>
#include <QGroupBox>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QEvent>

#include <QDesktopWidget>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <QPlainTextEdit>
#include <QGuiApplication>
#include <functional>
#include <QShortcut>

#include "dewarp_engine_helper.hpp"
#include "dwp_normal_imgviewer_widget.hpp"

class FishEyeWindow: public QMainWindow
{
    Q_OBJECT

public:
    explicit FishEyeWindow(QWidget *parent = nullptr);
    void initMenu();
    void initUI();
    void initEvent();
    void refresh();

protected:
    void changeEvent(QEvent *event) override;
    void retranslateUi();
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void showCtrlPanel();
    void handleOpenImage();
    void handleModeSelect(int m);
    void handleMountModeChange(int m);
    void slotUpdateRgn(int rgn_idx, QImage *image);
    void slotUpdatePanTilt(double delta_pan, double delta_tilt);
    void slotUpdateTheta(double delta_ThetaX, double delta_ThetaY, double delta_ThetaZ);
    void slotUpdateZoom(double delta_ZoomH, double delta_ZoomV);
    void slotResetAllRgns();

private:
    void createImagePanel();
    void changeMode(FISH_EYE_MODE m);
    void initModeMenu();
    void initMountMenu();
    void updatePanelImage();

private:
    FISH_EYE_MODE mode{MODE_02_1O4R};
    bool bInited{false};
    DewarpNormalImgViewerWidget *main_view_widget{nullptr};
    CtrlPanelWindow *ctrl_panel{nullptr};
    QAction *ctrl_panel_action{nullptr};
    QMenu *mode_menu;
    QMenu *mount_menu;
    shared_ptr<DewarpEngineHelper> dewarp_helper = DewarpEngineHelper::getInstance();
    int cur_idx;

};

#endif
