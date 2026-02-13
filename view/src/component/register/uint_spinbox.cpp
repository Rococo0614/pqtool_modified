#include "uint_spinbox.hpp"
#include "global_data.hpp"
#include "utils.hpp"


UIntSpinBox::UIntSpinBox(QWidget *parent): QSpinBox (parent)
{
}

void UIntSpinBox::setOffset(uint offset)
{
    this->offset = offset;
}

QValidator::State UIntSpinBox::validate(QString &input, int &pos) const
{
    QString copy(input);

    pos -= copy.size() - copy.trimmed().size();
    copy = copy.trimmed();

    if (copy.isEmpty())
    {
        return QValidator::Intermediate;
    }

    input = copy;
    bool ok;
    copy.toUInt(&ok, displayIntegerBase());
    if (!ok)
    {
        return QValidator::Invalid;
    }

    return QValidator::Acceptable;
}

int UIntSpinBox::valueFromText(const QString &text) const
{
    int v = i(text.toUInt(nullptr, displayIntegerBase()) - offset);
    return v;
}

QString UIntSpinBox::textFromValue(int val) const
{
    return QString::number(u(offset + val), displayIntegerBase()).toUpper();
}
