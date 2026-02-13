#include "header_widget.hpp"
#include "global_data.hpp"

HeaderWidget::HeaderWidget(QString module, QString title, QWidget *parent) : BaseItemWidget(module, title, parent)
{
    layout->removeWidget(icon_widget);
    layout->removeWidget(title_label);
    layout->removeWidget(value_widget);

    header_label = new QLabel(title);
    header_label->setMargin(5);
    header_label->setStyleSheet("background-color: #e0e0e0;");
    layout->addWidget(header_label);

    QObject::disconnect(GlobalData::getInstance().get(), &GlobalData::sigDataLoaded,
                        this, &HeaderWidget::dataLoaded);
}

void HeaderWidget::handleUpdateTheme(bool dark_theme_enabled)
{
    if (dark_theme_enabled) {
        header_label->setStyleSheet("background-color: #19232D;");
    } else {
        header_label->setStyleSheet("background-color: #e0e0e0;");
    }
}

void HeaderWidget::updateHighlight(bool bHiglight)
{
    if (bHiglight) {
        header_label->setStyleSheet("background-color: #a5cdff;");
    } else {
        header_label->setStyleSheet("background-color: #ffffff;");
    }
}