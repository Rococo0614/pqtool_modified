#ifndef REGISTER_EDIT_WIDGET_HPP
#define REGISTER_EDIT_WIDGET_HPP

#include <QWidget>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QLineEdit>
#include "cv_line_edit.hpp"

class RegisterEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RegisterEditWidget(QWidget *parent = nullptr);
    void initUI();

private:
    QHBoxLayout *main_layout;
    QList<QCheckBox *> checkbox_list;
    CvLineEdit *addr_line;
    CvLineEdit *value_line;

private slots:
    void checkboxChecked();
    void slotValueChanged(QString text);
    void addrChanged(QString text);
    void readReg();
    void writeReg();
    void slotUpdateReg(QString cmd);
};

#endif // REGISTER_EDIT_WIDGET_HPP
