#ifndef HEX_SPINBOX_H
#define HEX_SPINBOX_H

#include <QWidget>
#include <QSpinBox>

class HexSpinBox: public QSpinBox
{
    Q_OBJECT

public:
    explicit HexSpinBox(QWidget *parent=nullptr);
    unsigned int hexValue() const;
    void setHexValue(unsigned int v);

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
};

#endif // HEX_SPINBOX_H
