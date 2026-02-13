#include "system_console_dialog.hpp"

SystemConsoleDialog::SystemConsoleDialog(QWidget *parent) :
    QInputDialog(parent)
{
    setLabelText("Please input system params:");
    setTextValue("reg_level=2");
}
