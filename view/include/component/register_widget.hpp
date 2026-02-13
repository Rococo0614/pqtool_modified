#ifndef REGISTER_WIDGET_HPP
#define REGISTER_WIDGET_HPP

#include "register_checkbox.hpp"
#include <QWidget>
#include <QHBoxLayout>

class RegisterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RegisterWidget(QWidget *parent = nullptr);
    void initUI();

private:
    QHBoxLayout *main_layout;
    RegisterCheckBox *register_checkbox;
};

#endif // REGISTER_WIDGET_HPP
