#include "module_tab_widget.hpp"

#include <QPainter>

ModuleTabWidget::ModuleTabWidget(QWidget *parent) : QTabWidget(parent)
{
    this->setTabsClosable(true);
    this->setMovable(true);

    connect(this, &QTabWidget::tabCloseRequested, this, &ModuleTabWidget::closeModuleTab);
}

void ModuleTabWidget::paintEvent(QPaintEvent* e)
{
    QTabWidget::paintEvent(e);
    QPainter painter(this);
    // If don't have any tabs
    if (!count())
    {
        painter.drawText(rect(), Qt::AlignCenter, "Don't have any tabs");
    }
}

void ModuleTabWidget::setTabClosable(bool closable)
{
    this->setTabsClosable(closable);
}

void ModuleTabWidget::closeModuleTab(int tab_index)
{
    this->removeTab(tab_index);
}
