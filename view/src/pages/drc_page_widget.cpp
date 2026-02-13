#include "drc_page_widget.hpp"
#include "global_data.hpp"

DRCPageWidget::DRCPageWidget(QWidget *parent) : BasePageWidget(MODULE_NAME_DRC, parent)
{
    initData();
    initUI();
    main_layout->setStretch(0, 8);
    main_layout->setStretch(1, 1);
}

void DRCPageWidget::initLeftArea()
{
    initGroupLeftArea();
}

void DRCPageWidget::initCenterArea()
{
    initChart();
}

void DRCPageWidget::initChart()
{
}
