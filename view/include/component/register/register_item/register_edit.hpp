#ifndef REGISTER_EDIT_H
#define REGISTER_EDIT_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QGroupBox>
#include <qvalidator.h>

class RegisterEdit : public QWidget
{
    Q_OBJECT
public:
    explicit RegisterEdit(QString title = "title", QWidget *parent = nullptr);

    void setData(QString data);
    QString getData();
    QString getTitle();
    void setTitle(QString title);
    void handleEditFinished();
    void setEditProperty(QString prefix = "", int base = 0, int length = 8, bool readonly = false);
    void getEditProperty(QString *prefix, int *base, int *length);

private:
    QLineEdit *value_edit;
    QLabel *title_label;
    QLabel *prefix_label;
    int base{0};
    int length{8};
};

#endif
