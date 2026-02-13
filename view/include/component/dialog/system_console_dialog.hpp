#ifndef SYSTEM_CONSOLE_DIALOG_HPP
#define SYSTEM_CONSOLE_DIALOG_HPP

#include <QInputDialog>
#include <QLineEdit>
#include <QPushButton>

class SystemConsoleDialog : public QInputDialog
{
    Q_OBJECT
public:
    explicit SystemConsoleDialog(QWidget *parent = nullptr);
};

#endif // SYSTEM_CONSOLE_DIALOG_HPP
