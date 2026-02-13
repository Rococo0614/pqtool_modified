#ifndef TEMPLATE_PAGE_HPP
#define TEMPLATE_PAGE_HPP

#include "base_page_widget.hpp"

class TemplatePageWidget : public BasePageWidget
{
    Q_OBJECT

public:
    explicit TemplatePageWidget(QString module_id, QWidget *parent = nullptr);

protected:
    void initLeftArea() override;
    void initCenterArea() override;
};

#endif // TEMPLATE_PAGE_HPP
