#include "drawer_widget.hpp"
#include <QPropertyAnimation>
#include <QStyleOption>
#include <QPainter>

DrawerWidget::DrawerWidget(QWidget *firstWidget, QWidget *secondWidget, DRAWER_DIRECTION direction)
    : QWidget(nullptr), direction(direction)
{
    drawer_mode = DRAWER_MODE::SHOW;
    content_widget = firstWidget;
    extra_widget = secondWidget;
    initUI();
    initData();
}

void DrawerWidget::initUI()
{
    btn_container = new QWidget();
    btn_container->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    drawer_btn_left_up = new QPushButton();
    drawer_btn_left_up->setFlat(true);
    connect(drawer_btn_left_up, &QPushButton::clicked, this, &DrawerWidget::handleModeChangedLU);

    drawer_btn_right_down = new QPushButton();
    drawer_btn_right_down->setFlat(true);
    connect(drawer_btn_right_down, &QPushButton::clicked, this, &DrawerWidget::handleModeChangedRD);

    if (DRAWER_DIRECTION::VERTICAL_BOTTOM >= direction) {
        main_layout = new QVBoxLayout(this);
        btn_layout = new QHBoxLayout(btn_container);
        drawer_btn_left_up->setIcon(QIcon(":/icons/arrow_up.png"));
        drawer_btn_right_down->setIcon(QIcon(":/icons/arrow_down.png"));
    } else {
        main_layout = new QHBoxLayout(this);
        btn_layout = new QVBoxLayout(btn_container);
        drawer_btn_left_up->setIcon(QIcon(":/icons/arrow_left.png"));
        drawer_btn_right_down->setIcon(QIcon(":/icons/arrow_right.png"));
    }

    btn_layout->setMargin(0);
    btn_layout->setSpacing(1);
    btn_layout->addWidget(drawer_btn_left_up);
    btn_layout->addWidget(drawer_btn_right_down);


    main_layout->setSpacing(0);
    main_layout->setMargin(0);

    if (DRAWER_DIRECTION::VERTICAL_BOTTOM >= direction) {
        if (nullptr != content_widget) {
            main_layout->addWidget(btn_container, 0, Qt::AlignHCenter);
            main_layout->addWidget(content_widget);
            content_widget->layout()->setMargin(0);
            content_widget->layout()->setSpacing(0);
        }
    } else {
        if (extra_widget) {
            sub_layout = new QVBoxLayout(this);
            if (nullptr != extra_widget) {
                sub_layout->addWidget(extra_widget);
                extra_widget->layout()->setMargin(0);
                extra_widget->layout()->setSpacing(0);
                sub_layout->setSpacing(16);
            }
            if (nullptr != content_widget) {
                sub_layout->addWidget(content_widget);
                content_widget->layout()->setMargin(0);
                content_widget->layout()->setSpacing(0);
            }
            main_layout->addLayout(sub_layout);
        } else if (nullptr != content_widget) {
            main_layout->addWidget(content_widget);
            content_widget->layout()->setMargin(0);
            content_widget->layout()->setSpacing(0);
        }
        main_layout->addWidget(btn_container, 0, Qt::AlignVCenter);
    }
}

void DrawerWidget::initData()
{
    content_widget_show_width = content_widget->size().width();
    content_widget_show_height = content_widget->size().height();
    if (extra_widget) {
        extra_widget_show_width = content_widget->size().width();
        extra_widget_show_height = content_widget->size().height();
    } else {
        extra_widget_show_width = 0;
        extra_widget_show_height = 0;
    }

    content_widget_size = content_widget->size();
    if (extra_widget) {
        extra_widget_size = extra_widget->size();
    }
}

void DrawerWidget::handleModeChangedLU() {
    if (nullptr == content_widget) {
        return;
    }

    switch (drawer_mode) {
    case DRAWER_MODE::SHOW:
        switch (direction) {
        case DRAWER_DIRECTION::VERTICAL_TOP:
            drawer_mode = DRAWER_MODE::HIDE;
            break;
        case DRAWER_DIRECTION::VERTICAL_BOTTOM:
            drawer_mode = DRAWER_MODE::MAXIMIZE;
            break;
        case DRAWER_DIRECTION::HORIZONTAL_LEFT:
            drawer_mode = DRAWER_MODE::HIDE;
            break;
        case DRAWER_DIRECTION::HORIZONTAL_RIGHT:
            drawer_mode = DRAWER_MODE::MAXIMIZE;
            break;
        default:
            break;
        }
        break;
    case DRAWER_MODE::HIDE:
        switch (direction) {
        case DRAWER_DIRECTION::VERTICAL_TOP:
            break;
        case DRAWER_DIRECTION::VERTICAL_BOTTOM:
            drawer_mode = DRAWER_MODE::SHOW;
            break;
        case DRAWER_DIRECTION::HORIZONTAL_LEFT:
            break;
        case DRAWER_DIRECTION::HORIZONTAL_RIGHT:
            drawer_mode = DRAWER_MODE::SHOW;
            break;
        default:
            break;
        }
        break;
    case DRAWER_MODE::MAXIMIZE:
        switch (direction) {
        case DRAWER_DIRECTION::VERTICAL_TOP:
            drawer_mode = DRAWER_MODE::SHOW;
            break;
        case DRAWER_DIRECTION::VERTICAL_BOTTOM:
            break;
        case DRAWER_DIRECTION::HORIZONTAL_LEFT:
            drawer_mode = DRAWER_MODE::SHOW;
            break;
        case DRAWER_DIRECTION::HORIZONTAL_RIGHT:
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    // update widget size
    updateWidgetSize();

    QPropertyAnimation *animation = new QPropertyAnimation(content_widget, "maximumSize");
    connect(animation, &QPropertyAnimation::finished, this, &DrawerWidget::handleAnimationFinished);
    animation->setDuration(400);
    animation->setStartValue(content_widget->size());
    animation->setEndValue(content_widget_size);
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    animation->start(QAbstractAnimation::DeleteWhenStopped);

    if (extra_widget != nullptr) {
        QPropertyAnimation *extraAnimation = new QPropertyAnimation(extra_widget, "maximumSize");
        connect(extraAnimation, &QPropertyAnimation::finished, this, &DrawerWidget::handleAnimationFinished);

        extraAnimation->setDuration(400);
        extraAnimation->setStartValue(extra_widget->size());
        extraAnimation->setEndValue(extra_widget_size);
        extraAnimation->setEasingCurve(QEasingCurve::InOutQuad);
        extraAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void DrawerWidget::handleModeChangedRD() {
    if (nullptr == content_widget) {
        return;
    }

    switch (drawer_mode) {
    case DRAWER_MODE::SHOW:
        switch (direction) {
        case DRAWER_DIRECTION::VERTICAL_TOP:
            drawer_mode = DRAWER_MODE::MAXIMIZE;
            break;
        case DRAWER_DIRECTION::VERTICAL_BOTTOM:
            drawer_mode = DRAWER_MODE::HIDE;
            break;
        case DRAWER_DIRECTION::HORIZONTAL_LEFT:
            drawer_mode = DRAWER_MODE::MAXIMIZE;
            break;
        case DRAWER_DIRECTION::HORIZONTAL_RIGHT:
            drawer_mode = DRAWER_MODE::HIDE;
            break;
        default:
            break;
        }
        break;
    case DRAWER_MODE::HIDE:
        switch (direction) {
        case DRAWER_DIRECTION::VERTICAL_TOP:
            drawer_mode = DRAWER_MODE::SHOW;
            break;
        case DRAWER_DIRECTION::VERTICAL_BOTTOM:
            break;
        case DRAWER_DIRECTION::HORIZONTAL_LEFT:
            drawer_mode = DRAWER_MODE::SHOW;
            break;
        case DRAWER_DIRECTION::HORIZONTAL_RIGHT:
            break;
        default:
            break;
        }
        break;
    case DRAWER_MODE::MAXIMIZE:
        switch (direction) {
        case DRAWER_DIRECTION::VERTICAL_TOP:
            break;
        case DRAWER_DIRECTION::VERTICAL_BOTTOM:
            drawer_mode = DRAWER_MODE::SHOW;
            break;
        case DRAWER_DIRECTION::HORIZONTAL_LEFT:
            break;
        case DRAWER_DIRECTION::HORIZONTAL_RIGHT:
            drawer_mode = DRAWER_MODE::SHOW;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    // update widget size
    updateWidgetSize();

    QPropertyAnimation *animation = new QPropertyAnimation(content_widget, "maximumSize");
    connect(animation, &QPropertyAnimation::finished, this, &DrawerWidget::handleAnimationFinished);
    animation->setDuration(400);
    animation->setStartValue(content_widget->size());
    animation->setEndValue(content_widget_size);
    animation->setEasingCurve(QEasingCurve::InOutQuad);
    animation->start(QAbstractAnimation::DeleteWhenStopped);

    if (extra_widget != nullptr) {
        QPropertyAnimation *extraAnimation = new QPropertyAnimation(extra_widget, "maximumSize");
        connect(extraAnimation, &QPropertyAnimation::finished, this, &DrawerWidget::handleAnimationFinished);

        extraAnimation->setDuration(400);
        extraAnimation->setStartValue(extra_widget->size());
        extraAnimation->setEndValue(extra_widget_size);
        extraAnimation->setEasingCurve(QEasingCurve::InOutQuad);
        extraAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void DrawerWidget::updateWidgetSize()
{
    bool is_vertical = (direction <= DRAWER_DIRECTION::VERTICAL_BOTTOM);
    int width = 0;
    int height = 0;

    int extra_width = 0;
    int extra_height = 0;

    switch(drawer_mode) {
    case DRAWER_MODE::HIDE:
        if (is_vertical) {
            width = extra_width = QWIDGETSIZE_MAX;
            height = extra_height = 0;
        } else {
            width = extra_width = 0;
            height = extra_height = QWIDGETSIZE_MAX;
        }
        break;
    case DRAWER_MODE::SHOW:
        if (is_vertical) {
            height = content_widget_show_height;
            extra_height = extra_widget_show_height;
            width = extra_width = QWIDGETSIZE_MAX;
        } else {
            width = content_widget_show_width;
            extra_width = extra_widget_show_width;
            height = extra_height = QWIDGETSIZE_MAX;
        }
        break;
    case DRAWER_MODE::MAXIMIZE:
        width = extra_width = QWIDGETSIZE_MAX;
        height = extra_height = QWIDGETSIZE_MAX;
        break;
    default:
        break;
    }
    // update
    content_widget_size.setWidth(width);
    content_widget_size.setHeight(height);
    extra_widget_size.setWidth(extra_width);
    extra_widget_size.setHeight(extra_height);

    if (width == 0) {
        width = drawer_btn_left_up->size().width();
    }

    if (height == 0) {
        height = drawer_btn_left_up->size().height();
    }
}

void DrawerWidget::updateBtnStatus()
{
    if (drawer_mode == DRAWER_MODE::SHOW) {
        drawer_btn_left_up->setHidden(false);
        drawer_btn_right_down->setHidden(false);
        return;
    }

    bool is_left_up_btn_disable =
        (drawer_mode == DRAWER_MODE::HIDE && (direction == DRAWER_DIRECTION::VERTICAL_TOP || direction == DRAWER_DIRECTION::HORIZONTAL_LEFT)) ||
        (drawer_mode == DRAWER_MODE::MAXIMIZE && (direction == DRAWER_DIRECTION::VERTICAL_BOTTOM || direction == DRAWER_DIRECTION::HORIZONTAL_RIGHT));

    bool is_right_down_btn_disable =
        (drawer_mode == DRAWER_MODE::MAXIMIZE && (direction == DRAWER_DIRECTION::VERTICAL_TOP || direction == DRAWER_DIRECTION::HORIZONTAL_LEFT)) ||
        (drawer_mode == DRAWER_MODE::HIDE && (direction == DRAWER_DIRECTION::VERTICAL_BOTTOM || direction == DRAWER_DIRECTION::HORIZONTAL_RIGHT));

    if (is_left_up_btn_disable) {
        drawer_btn_left_up->setHidden(true);
    }
    if (is_right_down_btn_disable) {
        drawer_btn_right_down->setHidden(true);
    }
}

void DrawerWidget::handleAnimationFinished()
{
    if (nullptr == content_widget) {
        return;
    }

    // set the size
    content_widget->setMaximumSize(content_widget_size.width(), content_widget_size.height());
    if (extra_widget) {
        extra_widget->setMaximumSize(extra_widget_size.width(), extra_widget_size.height());
    }

    // udpate Btn status
    updateBtnStatus();
}
