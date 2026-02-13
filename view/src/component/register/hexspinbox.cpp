#include "hexspinbox.hpp"
#include "global_data.hpp"
#include "utils.hpp"


HexSpinBox::HexSpinBox(QWidget *parent): QSpinBox (parent)
{
    setPrefix("0x");
    setDisplayIntegerBase(16);
}

unsigned int HexSpinBox::hexValue() const
{
    return u(value());
}

void HexSpinBox::setHexValue(unsigned int v)
{
    setValue(i(v));
}

QValidator::State HexSpinBox::validate(QString &input, int &pos) const
{
    QString copy(input);
    if (copy.startsWith("0x"))
    {
        copy.remove(0, 2);
    }

    pos -= copy.size() - copy.trimmed().size();
    copy = copy.trimmed();

    if (copy.isEmpty())
    {
        return QValidator::Intermediate;
    }

    input = QString("0x") + copy.toUpper();
    bool ok;
    copy.toUInt(&ok, 16);
    if (!ok)
    {
        return QValidator::Invalid;
    }

    return QValidator::Acceptable;
}

int HexSpinBox::valueFromText(const QString &text) const
{
    return i(text.toUInt(nullptr, 16));
}

QString HexSpinBox::textFromValue(int val) const
{
    return QString::number(u(val), 16);
}
