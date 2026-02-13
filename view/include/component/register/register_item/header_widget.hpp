#ifndef HEADER_WIDGET_HPP
#define HEADER_WIDGET_HPP

#include "base_item_widget.hpp"

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class HeaderWidget : public BaseItemWidget
{
    Q_OBJECT

public:
    explicit HeaderWidget(QString module, QString title, QWidget *parent = nullptr);
    virtual void handleUpdateTheme(bool dark_theme_enabled) override;
    virtual void updateHighlight(bool bHiglight);

protected:
    QLabel *header_label;
};

#endif // HEADER_WIDGET_HPP
