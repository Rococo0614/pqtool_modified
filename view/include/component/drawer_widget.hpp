#ifndef DRAWER_WIDGET_HPP
#define DRAWER_WIDGET_HPP

#include <QWidget>
#include <QPushButton>
#include <QBoxLayout>
#include <QPropertyAnimation>

enum DRAWER_DIRECTION
{
    VERTICAL_TOP,
    VERTICAL_BOTTOM,
    HORIZONTAL_LEFT,
    HORIZONTAL_RIGHT,
};

enum DRAWER_MODE
{
    HIDE,
    SHOW,
    MAXIMIZE,
};

class DrawerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DrawerWidget(QWidget *firstWidget = nullptr, QWidget *secondWidget = nullptr,
                          DRAWER_DIRECTION direction = DRAWER_DIRECTION::VERTICAL_BOTTOM);
    void initUI();

    void initData();

private:
    void updateUI();
    void updateWidgetSize();
    void updateBtnStatus();

private slots:
    void handleModeChangedLU();
    void handleModeChangedRD();
    void handleAnimationFinished();

private:
    DRAWER_MODE drawer_mode;
    QSize content_widget_normal_size;
    QSize content_widget_max_size;
    QWidget *content_widget;
    QWidget *extra_widget;
    QBoxLayout *main_layout;
    QBoxLayout *sub_layout;
    QPushButton *drawer_btn_left_up;
    QPushButton *drawer_btn_right_down;
    QWidget *btn_container;
    QBoxLayout *btn_layout;
    DRAWER_DIRECTION direction;
    QPropertyAnimation *animation;
    QPropertyAnimation *extraAnimation;
    // data
    int content_widget_show_width;
    int content_widget_show_height;
    int extra_widget_show_width;
    int extra_widget_show_height;
    QSize content_widget_size;
    QSize extra_widget_size;
    QSize drawer_size;
};

#endif // DRAWER_WIDGET_HPP
