#ifndef CONTROL_PANEL_WINDOW_HPP
#define CONTROL_PANEL_WINDOW_HPP

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QPixmap>
#include <QGroupBox>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QKeyEvent>

enum CONTROL_CMD {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    HOME,
    ZOOM_IN_H,
    ZOOM_OUT_H,
    ZOOM_IN_V,
    ZOOM_OUT_V,
    CONTROL_CMD_END
};

class CtrlPanelWindow: public QMainWindow
{
    Q_OBJECT

public:
    explicit CtrlPanelWindow(QWidget *parent = nullptr);
    void initMenu();
    void initUI();
    void initEvent();
    void refresh();

protected:
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void handleButtonPressed(CONTROL_CMD cmd);

signals:
    void sigKeyUp();
    void sigKeyDown();
    void sigKeyLeft();
    void sigKeyRight();
    void sigKeyHome();
    void sigKeyZoomInH();
    void sigKeyZoomOutH();
    void sigKeyZoomInV();
    void sigKeyZoomOutV();
};

#endif //CONTROL_PANEL_WINDOW_HPP
