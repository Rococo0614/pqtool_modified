#include "fisheye_utility_window.hpp"
#include "utils.hpp"
#include "dewarp_engine_helper.hpp"
#include <QFileDialog>

#define MIN_WINDOW_WIDTH  (1280)
#define MIN_WINDOW_HEIGHT (720)
#define DELTA (5)
#define ZOOM_DELTA (15)

FishEyeWindow::FishEyeWindow(QWidget *parent):
    QMainWindow (parent)
{
    setWindowTitle(tr("Fish Eye Tool"));
    setMinimumSize(MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
    dewarp_helper->setMode(mode);

    initMenu();
    initUI();
    initEvent();
    retranslateUi();
}

void FishEyeWindow::showCtrlPanel()
{
    if (ctrl_panel == nullptr) {
        ctrl_panel = new CtrlPanelWindow();
        ctrl_panel->setWindowFlag(Qt::WindowStaysOnTopHint);
        connect(ctrl_panel, &CtrlPanelWindow::sigKeyUp, this, std::bind(&FishEyeWindow::slotUpdatePanTilt, this, 0, DELTA));
        connect(ctrl_panel, &CtrlPanelWindow::sigKeyDown, this, std::bind(&FishEyeWindow::slotUpdatePanTilt, this, 0, -DELTA));
        connect(ctrl_panel, &CtrlPanelWindow::sigKeyLeft, this, std::bind(&FishEyeWindow::slotUpdatePanTilt, this, -DELTA, 0 ));
        connect(ctrl_panel, &CtrlPanelWindow::sigKeyRight, this, std::bind(&FishEyeWindow::slotUpdatePanTilt, this, DELTA, 0));
        connect(ctrl_panel, &CtrlPanelWindow::sigKeyHome, this, &FishEyeWindow::slotResetAllRgns);
        connect(ctrl_panel, &CtrlPanelWindow::sigKeyZoomInH, this, std::bind(&FishEyeWindow::slotUpdateZoom, this, -ZOOM_DELTA, 0));
        connect(ctrl_panel, &CtrlPanelWindow::sigKeyZoomOutH, this, std::bind(&FishEyeWindow::slotUpdateZoom, this, ZOOM_DELTA, 0));
        connect(ctrl_panel, &CtrlPanelWindow::sigKeyZoomInV, this, std::bind(&FishEyeWindow::slotUpdateZoom, this, 0, -ZOOM_DELTA));
        connect(ctrl_panel, &CtrlPanelWindow::sigKeyZoomOutV, this, std::bind(&FishEyeWindow::slotUpdateZoom, this, 0, ZOOM_DELTA));
    }

    ctrl_panel->show();
    ctrl_panel->activateWindow();
}

void FishEyeWindow::handleOpenImage()
{
    QString img_src_path = QFileDialog::getOpenFileName(this, tr("Open File"), "", "*.ppm");

    if (img_src_path.isNull() || img_src_path.isEmpty())
    {
        return;
    }

    if (!main_view_widget) {
        createImagePanel();
    }

    dewarp_helper->setSrcFilePath(img_src_path);
    dewarp_helper->loadFishEyeImage();
}

void FishEyeWindow::initMenu()
{
    ctrl_panel_action = new QAction(this);
    ctrl_panel_action->setText("&Control");
    connect(ctrl_panel_action, &QAction::triggered, this, &FishEyeWindow::showCtrlPanel);
    menuBar()->addAction(ctrl_panel_action);

    QAction *open_img_action = new QAction(this);
    open_img_action->setText("&Open");
    connect(open_img_action, &QAction::triggered, this, &FishEyeWindow::handleOpenImage);
    menuBar()->addAction(open_img_action);

    initModeMenu();
    initMountMenu();
}

void FishEyeWindow::initModeMenu()
{
    mode_menu = new QMenu;
    menuBar()->addMenu(mode_menu);

    QAction *test = new QAction("MODE_PANORAMA_360");
    test->setCheckable(true);
    connect(test, &QAction::triggered, this, bind(&FishEyeWindow::handleModeSelect, this, MODE_PANORAMA_360));
    mode_menu->addAction(test);

    test = new QAction("MODE_PANORAMA_180");
    test->setCheckable(true);
    connect(test, &QAction::triggered, this, bind(&FishEyeWindow::handleModeSelect, this, MODE_PANORAMA_180));
    mode_menu->addAction(test);

    test = new QAction("MODE_01_1O");
    test->setCheckable(true);
    connect(test, &QAction::triggered, this, bind(&FishEyeWindow::handleModeSelect, this, MODE_01_1O));
    mode_menu->addAction(test);

    test = new QAction("MODE_02_1O4R");
    test->setCheckable(true);
    test->setChecked(true);
    connect(test, &QAction::triggered, this, bind(&FishEyeWindow::handleModeSelect, this, MODE_02_1O4R));
    mode_menu->addAction(test);

    test = new QAction("MODE_03_4R");
    test->setCheckable(true);
    connect(test, &QAction::triggered, this, bind(&FishEyeWindow::handleModeSelect, this, MODE_03_4R));
    mode_menu->addAction(test);

    test = new QAction("MODE_04_1P2R");
    test->setCheckable(true);
    connect(test, &QAction::triggered, this, bind(&FishEyeWindow::handleModeSelect, this, MODE_04_1P2R));
    mode_menu->addAction(test);

    test = new QAction("MODE_05_1P2R");
    test->setCheckable(true);
    connect(test, &QAction::triggered, this, bind(&FishEyeWindow::handleModeSelect, this, MODE_05_1P2R));
    mode_menu->addAction(test);

    test = new QAction("MODE_06_1P");
    test->setCheckable(true);
    connect(test, &QAction::triggered, this, bind(&FishEyeWindow::handleModeSelect, this, MODE_06_1P));
    mode_menu->addAction(test);

    test = new QAction("MODE_07_2P");
    test->setCheckable(true);
    connect(test, &QAction::triggered, this, bind(&FishEyeWindow::handleModeSelect, this, MODE_07_2P));
    mode_menu->addAction(test);
}

void FishEyeWindow::initMountMenu()
{
    mount_menu = new QMenu;
    menuBar()->addMenu(mount_menu);

    QAction *action = new QAction(tr("Ceiling"));
    action->setCheckable(true);
    action->setChecked(true);
    connect(action, &QAction::triggered, this, bind(&FishEyeWindow::handleMountModeChange, this, MOUNTMODE::CEILING));
    mount_menu->addAction(action);

    action = new QAction(tr("Floor"));
    action->setCheckable(true);
    connect(action, &QAction::triggered, this, bind(&FishEyeWindow::handleMountModeChange, this, MOUNTMODE::FLOOR));
    mount_menu->addAction(action);

    action = new QAction(tr("Wall"));
    action->setCheckable(true);
    connect(action, &QAction::triggered, this, bind(&FishEyeWindow::handleMountModeChange, this, MOUNTMODE::WALL));
    mount_menu->addAction(action);
}

void FishEyeWindow::initUI()
{
//    createImagePanel();
}

void FishEyeWindow::initEvent()
{
    connect(dewarp_helper.get(), &DewarpEngineHelper::sigDataUpdated, this, &FishEyeWindow::slotUpdateRgn);
}

void FishEyeWindow::refresh()
{

}

void FishEyeWindow::createImagePanel()
{
    if (main_view_widget) {
        delete main_view_widget;
        main_view_widget = nullptr;
    }

    main_view_widget = new DewarpNormalImgViewerWidget;
    main_view_widget->initMode(mode);

    setCentralWidget(main_view_widget);
}

void FishEyeWindow::changeEvent(QEvent *event)
{
    switch(event->type()) {
    case QEvent::LanguageChange:
        retranslateUi();
        break;
    default:
        break;
    }
    QMainWindow::changeEvent(event);
}

void FishEyeWindow::retranslateUi()
{
    setWindowTitle(tr("Fish Eye Tool"));

    ctrl_panel_action->setText(tr("&Control"));
    ctrl_panel_action->setStatusTip(tr("Control"));
    mode_menu->setTitle(tr("Usage Mode"));
    mount_menu->setTitle(tr("Mount Mode"));
}

void FishEyeWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        this->close();
    }
}

void FishEyeWindow::updatePanelImage()
{
    QString path = dewarp_helper->getSrcFilePath();
    if (!path.isNull() && !path.isEmpty()) {
        dewarp_helper->loadFishEyeImage();
    }
}

void FishEyeWindow::handleMountModeChange(int m)
{
    for (int i = 0; i < mount_menu->actions().count(); i++) {
        QAction *action = mount_menu->actions()[i];
        action->setChecked((i+1) == m);
    }

    if (m >= MOUNTMODE::CEILING && m < MOUNTMODE::OTHER) {
        dewarp_helper->setMountMode(static_cast<MOUNTMODE>(m));

        updatePanelImage();
    }
}

void FishEyeWindow::handleModeSelect(int m)
{
    for (int i = 0; i < mode_menu->actions().count(); i++) {
        QAction *action = mode_menu->actions()[i];
        action->setChecked((i+1) == m);
    }

    changeMode(static_cast<FISH_EYE_MODE>(m));
    updatePanelImage();
}

void FishEyeWindow::changeMode(FISH_EYE_MODE m)
{
    if (m < MODE_PANORAMA_360 || m > MODE_07_2P || mode == m)
    {
        return;
    }

    mode = m;
    dewarp_helper->setMode(m);

    createImagePanel();
}

void FishEyeWindow::slotUpdateRgn(int rgn_idx, QImage *image)
{
    if (mode == FISH_EYE_MODE::MODE_01_1O) {
        main_view_widget->updateRgnView(SECTOR_1O, image);
    } else if (mode == FISH_EYE_MODE::MODE_02_1O4R) {
        main_view_widget->updateRgnView(rgn_idx, image);
    } else if (mode == FISH_EYE_MODE::MODE_03_4R) {
        main_view_widget->updateRgnView(rgn_idx, image);
    } else if (mode == MODE_04_1P2R || mode == MODE_05_1P2R) {
        if (rgn_idx == 0)
            main_view_widget->updateRgnView(SECTOR_PANORAMA_0, image);
        else {
            main_view_widget->updateRgnView(rgn_idx, image);
        }
    } else if (mode == MODE_06_1P || mode == MODE_07_2P) {
        main_view_widget->updateRgnView(rgn_idx + SECTOR_PANORAMA_0, image);
    } else if (mode == MODE_PANORAMA_180 || mode == MODE_PANORAMA_360) {
        main_view_widget->updateRgnView(SECTOR_PANORAMA_0, image);
    }
}

void FishEyeWindow::slotUpdatePanTilt(double delta_pan, double delta_tilt)
{
    cur_idx = main_view_widget->getActiveRegion();
    if (cur_idx < 0 || cur_idx > 4)
        return;

    dewarp_helper->updatePanTilt(cur_idx, delta_pan, delta_tilt);
}

void FishEyeWindow::slotUpdateTheta(double delta_ThetaX, double delta_ThetaY, double delta_ThetaZ)
{
    cur_idx = main_view_widget->getActiveRegion();
    if (cur_idx < 0 || cur_idx > 4)
        return;

    dewarp_helper->updateTheta(cur_idx, delta_ThetaX, delta_ThetaY, delta_ThetaZ);
}

void FishEyeWindow::slotUpdateZoom(double delta_ZoomH, double delta_ZoomV)
{
    cur_idx = main_view_widget->getActiveRegion();
    if (cur_idx < 0 || cur_idx > 4)
        return;

    dewarp_helper->updateZoom(cur_idx, delta_ZoomH, delta_ZoomV);
}

void FishEyeWindow::slotResetAllRgns()
{
    updatePanelImage();
}
