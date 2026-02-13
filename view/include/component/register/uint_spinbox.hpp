#ifndef UINT_SPINBOX_H
#define UINT_SPINBOX_H

#include <QWidget>
#include <QSpinBox>

class UIntSpinBox: public QSpinBox
{
    Q_OBJECT

public:
    explicit UIntSpinBox(QWidget *parent=nullptr);
    void setOffset(uint offset);

protected:
    QValidator::State validate(QString &input, int &pos) const override;
    virtual int valueFromText(const QString &text) const override;
    virtual QString textFromValue(int val) const override;

private:
    inline unsigned int u(int i) const
    {
        return *reinterpret_cast<unsigned int*>(&i);
    }

    inline int i(unsigned int u) const
    {
        return *reinterpret_cast<int *>(&u);
    }

    uint offset {0};
};

#endif // UINT_SPINBOX_H
