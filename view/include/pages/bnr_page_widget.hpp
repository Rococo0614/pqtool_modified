#ifndef BNR_PAGE_WIDGET_H
#define BNR_PAGE_WIDGET_H

#include "base_page_widget.hpp"

class BNRPageWidget : public BasePageWidget
{
    Q_OBJECT

public:
    explicit BNRPageWidget(QWidget *parent = nullptr);

protected:
    void initLeftArea() override;
    void initCenterArea() override;

private slots:
    void slotEnableChecked(bool checked);
    void slotFilterChecked(int value);
    void slotStrengthChanged(int value);
};

#endif // BNR_PAGE_WIDGET_H
