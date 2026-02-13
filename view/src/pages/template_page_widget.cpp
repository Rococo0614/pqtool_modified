#include "template_page_widget.hpp"

TemplatePageWidget::TemplatePageWidget(QString module_id, QWidget *parent) : BasePageWidget(module_id, parent)
{
    initData();
    initUI();
    main_layout->setStretch(0, 8);
    main_layout->setStretch(1, 1);
}

void TemplatePageWidget::initLeftArea()
{
    initGroupLeftArea();
}

void TemplatePageWidget::initCenterArea()
{
}
