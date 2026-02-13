#ifndef MODULE_TAB_WIDGET_H
#define MODULE_TAB_WIDGET_H

#include <QTabWidget>
#include "module_define.hpp"

class ModuleTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit ModuleTabWidget(QWidget *parent = nullptr);
    void setTabClosable(bool closable);

protected:
    virtual void paintEvent(QPaintEvent* e ) override;

private slots:
    void closeModuleTab(int tab_index);
};

#endif // MODULE_TAB_WIDGET_H
