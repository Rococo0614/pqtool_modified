#ifndef DRC_PAGE_HPP
#define DRC_PAGE_HPP

#include "base_page_widget.hpp"

class DRCPageWidget : public BasePageWidget
{
    Q_OBJECT

public:
    explicit DRCPageWidget(QWidget *parent = nullptr);

protected:
    virtual void initLeftArea() override;
    virtual void initCenterArea() override;
    void initChart();

private:

};

#endif // DRC_PAGE_HPP
