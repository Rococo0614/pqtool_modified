#include "bnr_page_widget.hpp"
#include "module_define.hpp"
#include "global_data.hpp"
#include "network_client_helper.hpp"
#include "utils.hpp"

BNRPageWidget::BNRPageWidget(QWidget *parent) : BasePageWidget(MODULE_NAME_BNR, parent)
{
    initData();
    initUI();
    main_layout->setStretch(0, 8);
    main_layout->setStretch(1, 1);
}

void BNRPageWidget::initLeftArea()
{
    initGroupLeftArea();
}

void BNRPageWidget::initCenterArea()
{
}

void BNRPageWidget::slotEnableChecked(bool enabled)
{
    qDebug("slotEnableChecked %d", enabled);
}

void BNRPageWidget::slotFilterChecked(int value)
{
    qDebug("slotFilterChecked %d", value);
}

void BNRPageWidget::slotStrengthChanged(int value)
{
    qDebug("slotStrengthChanged %d", value);
}
