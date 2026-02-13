#ifndef REGISTER_CHECKBOX_H
#define REGISTER_CHECKBOX_H

#include <QWidget>
#include "i_observer.hpp"

class QLabel;
class QCheckBox;

class RegisterCheckBox : public QWidget, public IObserver
{
    Q_OBJECT
public:
    explicit RegisterCheckBox(QWidget *parent = nullptr);

    virtual void refreshData(const SecKeyType &reg_section_key) override;
    void setAddressText(QString address);
    void setBitmask(QString bitmask);
    SecKeyType getSectionKey();
    void checkboxChecked();

protected:
    void changeEvent(QEvent *event) override;

private:
    void initUI();
    void updateCheckbox(unsigned int value);
    void retranslateUI();

private:
    QList<QCheckBox *> checkbox_list;
    QLabel *selected_address_label;
    QLabel *selected_bitmask_label;
    QLabel *selected_value_label;
    QLabel *addr;
    QLabel *bitmask;
    QLabel *value;
};

#endif // REGISTER_CHECKBOX_H
