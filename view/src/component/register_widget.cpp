#include "register_widget.hpp"
#include <QLabel>

RegisterWidget::RegisterWidget(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void RegisterWidget::initUI()
{
    main_layout = new QHBoxLayout(this);

    register_checkbox = new RegisterCheckBox;
    main_layout->addWidget(register_checkbox);
}
