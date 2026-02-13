#include "ctrl_panel_window.hpp"
#include <QPushButton>


#define FIXED_WINDOW_WIDTH  (200)
#define FIXED_WINDOW_HEIGHT (200)

CtrlPanelWindow::CtrlPanelWindow(QWidget *parent) :QMainWindow (parent)
{
    setWindowTitle(tr("Dewarp Control Panel"));
    setWindowFlags(Qt::WindowCloseButtonHint);
    setFixedSize(QSize(FIXED_WINDOW_WIDTH, FIXED_WINDOW_HEIGHT));

    initMenu();
    initUI();
    initEvent();
}

void CtrlPanelWindow::initMenu()
{
}

void CtrlPanelWindow::initUI()
{
    QWidget *widget = new QWidget;
    QGridLayout *grid = new QGridLayout;

    QPushButton *left = new QPushButton("Left");
    connect(left, &QPushButton::clicked, this, std::bind(&CtrlPanelWindow::handleButtonPressed, this, CONTROL_CMD::LEFT));
    QPushButton *right = new QPushButton("Right");
    connect(right, &QPushButton::clicked, this, std::bind(&CtrlPanelWindow::handleButtonPressed, this, CONTROL_CMD::RIGHT));
    QPushButton *up = new QPushButton("Up");
    connect(up, &QPushButton::clicked, this, std::bind(&CtrlPanelWindow::handleButtonPressed, this, CONTROL_CMD::UP));
    QPushButton *down = new QPushButton("Down");
    connect(down, &QPushButton::clicked, this, std::bind(&CtrlPanelWindow::handleButtonPressed, this, CONTROL_CMD::DOWN));
    QPushButton *home = new QPushButton("Home");
    connect(home, &QPushButton::clicked, this, std::bind(&CtrlPanelWindow::handleButtonPressed, this, CONTROL_CMD::HOME));
    QPushButton *zoom_in_h = new QPushButton(" H+ ");
    connect(zoom_in_h, &QPushButton::clicked, this, std::bind(&CtrlPanelWindow::handleButtonPressed, this, CONTROL_CMD::ZOOM_IN_H));
    QPushButton *zoom_out_h = new QPushButton(" H- ");
    connect(zoom_out_h, &QPushButton::clicked, this, std::bind(&CtrlPanelWindow::handleButtonPressed, this, CONTROL_CMD::ZOOM_OUT_H));

    QPushButton *zoom_in_v = new QPushButton(" V+ ");
    connect(zoom_in_v, &QPushButton::clicked, this, std::bind(&CtrlPanelWindow::handleButtonPressed, this, CONTROL_CMD::ZOOM_IN_V));
    QPushButton *zoom_out_v = new QPushButton(" V- ");
    connect(zoom_out_v, &QPushButton::clicked, this, std::bind(&CtrlPanelWindow::handleButtonPressed, this, CONTROL_CMD::ZOOM_OUT_V));

    grid->addWidget(up, 0, 1);
    grid->addWidget(left, 1, 0);
    grid->addWidget(home, 1, 1);
    grid->addWidget(right, 1, 2);
    grid->addWidget(down, 2, 1);
    grid->addWidget(zoom_in_h, 3, 0);
    grid->addWidget(zoom_out_h, 3, 2);
    grid->addWidget(zoom_in_v, 4, 0);
    grid->addWidget(zoom_out_v, 4, 2);

    widget->setLayout(grid);
    setCentralWidget(widget);
}

void CtrlPanelWindow::initEvent()
{

}

void CtrlPanelWindow::refresh()
{

}

void CtrlPanelWindow::handleButtonPressed(CONTROL_CMD cmd)
{
    switch(cmd) {
        case LEFT:
            emit sigKeyLeft();
        break;
        case RIGHT:
            emit sigKeyRight();
            break;
        case UP:
            emit sigKeyUp();
            break;
        case DOWN:
            emit sigKeyDown();
            break;
        case HOME:
            emit sigKeyHome();
            break;
        case ZOOM_IN_H:
            emit sigKeyZoomInH();
            break;
        case ZOOM_OUT_H:
            emit sigKeyZoomOutH();
            break;
    }
}

void CtrlPanelWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        this->close();
    }
}
