#ifndef REGISTER_LINEEDIT_H
#define REGISTER_LINEEDIT_H

#include <QLineEdit>
#include "i_observer.hpp"
#include "base_item_widget.hpp"

class RegisterLineEdit : public BaseItemWidget
{
    Q_OBJECT
public:
    explicit RegisterLineEdit(QWidget *parent = nullptr);

    virtual void setData(QVariant data, bool sync_to_global = false) override;
    void setBitmask(QString bitmask);
    QString getBitmask();
    virtual void setChmod(int mod) override;

signals:
    void sigFocusIn();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void initValueWidget();

private:
    QLineEdit *value_lineedit;
    QString bitmask;
};

#endif // REGISTER_LINEEDIT_H
