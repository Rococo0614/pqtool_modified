#ifndef I_OBSERVER_H
#define I_OBSERVER_H

#include <QObject>

using SecKeyType = QPair<QString, QString>;

enum class HORIZONTAL_HEADER_TYPE
{
    ISO,
    LV,
};

class IObserver
{
public:
    virtual ~IObserver() = 0;
    virtual void refreshData(const SecKeyType &reg_section_key);
    virtual QString getOpType() const;
    virtual void setOpType(QString op_type);
    virtual void setRegValueType(QString reg_value_type);
    QString getRegValueType() const;
    virtual void setRegSectionKeyList(QList<SecKeyType> &&reg_section_key_list);
    virtual void addRegSectionKey(QString section, QString key);
    QList<SecKeyType> getSectionKeyList() const;
    virtual void setChmod(int mod);
    virtual int getChmod() const;
    virtual void setDisplayIntBase(int base);

protected:
    QList<SecKeyType> reg_section_key_list;
    QString op_type;
    QString reg_value_type;
    int chmod{0};
};

#endif // I_OBSERVER_H
