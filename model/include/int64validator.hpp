#ifndef VALUE_INT64_VALIDATOR_H
#define VALUE_INT64_VALIDATOR_H

#include <QObject>
#include <QIntValidator>

class ValueInt64Validator : public QIntValidator
{
    Q_OBJECT
public:
    explicit ValueInt64Validator(QObject * parent = nullptr);
    ValueInt64Validator(int64_t bottom, int64_t top, QObject *parent = nullptr):
        QIntValidator(bottom, top, parent), i64_top(top), i64_bottom(bottom)
    {

    }
    ~ValueInt64Validator() {}

    virtual State validate(QString &s, int &p) const override;
    virtual void fixup(QString &input) const override;
private:
    int64_t i64_top;
    int64_t i64_bottom;
};


#endif // VALUE_INT64_VALIDATOR_H
