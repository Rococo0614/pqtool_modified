#include "value_validator.hpp"

ValueIntValidator::State ValueIntValidator::validate(QString &s, int &p) const
{
    State state = QIntValidator::validate(s, p);
    //qDebug("validate state:%d ", state);
    return state;
}

void ValueIntValidator::fixup(QString &input) const
{
    if (input.toInt() < bottom())
    {
        input = QString("%1").arg(bottom());
    }

    if (input.toInt() > top())
    {
        input = QString("%1").arg(top());
    }
}
