#include "int64validator.hpp"

ValueInt64Validator::State ValueInt64Validator::validate(QString &s, int &pos) const
{
    QString copy(s);

    pos -= copy.size() - copy.trimmed().size();
    copy = copy.trimmed();

    if (copy.isEmpty())
    {
        return QValidator::Intermediate;
    }

    s = copy;

    bool ok;
    copy.toLongLong(&ok, 10);
    if (!ok)
    {
        return QValidator::Invalid;
    }

    return QValidator::Acceptable;
}

void ValueInt64Validator::fixup(QString &input) const
{
    if (input.toLongLong() < i64_bottom)
    {
        input = QString("%1").arg(i64_bottom);
    }

    if (input.toLongLong() > i64_top)
    {
        input = QString("%1").arg(i64_top);
    }
}
