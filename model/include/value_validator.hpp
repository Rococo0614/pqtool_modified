#ifndef VALUE_VALIDATOR_H
#define VALUE_VALIDATOR_H

#include <QObject>
#include <QIntValidator>

class ValueIntValidator : public QIntValidator
{
    Q_OBJECT
public:
    explicit ValueIntValidator(QObject * parent = nullptr);
    ValueIntValidator(int bottom, int top, QObject *parent = nullptr):
        QIntValidator(bottom, top, parent)
    {

    }
    ~ValueIntValidator() {}

    virtual State validate(QString &s, int &p) const override;
    virtual void fixup(QString &input) const override;
};


#endif // VALUE_VALIDATOR_H
