#include "i_observer.hpp"
#include "global_data.hpp"

IObserver::~IObserver()
{
    for (auto reg_section_key : this->reg_section_key_list) {
        GlobalData::getInstance()->unSubscribeObserver(reg_section_key, this);
    }
}

void IObserver::refreshData(const SecKeyType&)
{
}

QString IObserver::getOpType() const
{
    return this->op_type;
}

void IObserver::setOpType(QString op_type)
{
    if ((op_type != OP_TYPE_AUTO) && (op_type != OP_TYPE_MANUAL)) {
        return;
    }

    this->op_type = std::move(op_type);
}

QString IObserver::getRegValueType() const
{
    return this->reg_value_type;
}

void IObserver::setRegValueType(QString value_type)
{
    this->reg_value_type = std::move(value_type);
}

void IObserver::setRegSectionKeyList(QList<SecKeyType> &&reg_section_key_list)
{
    for (auto reg_section_key : this->reg_section_key_list) {
        GlobalData::getInstance()->unSubscribeObserver(reg_section_key, this);
    }

    this->reg_section_key_list = reg_section_key_list;

    for (auto reg_section_key : this->reg_section_key_list) {
        // refreshData(reg_section_key);

        GlobalData::getInstance()->subscribeObserver(reg_section_key, this);
    }
}

void IObserver::addRegSectionKey(QString section, QString key)
{
    if (section.isEmpty() || key.isEmpty()) {
        return;
    }

    SecKeyType reg_section_key = SecKeyType(section, key);
    this->reg_section_key_list.append(reg_section_key);

    refreshData(reg_section_key);
    GlobalData::getInstance()->subscribeObserver(reg_section_key, this);
}

QList<SecKeyType> IObserver::getSectionKeyList() const
{
    return reg_section_key_list;
}

void IObserver::setChmod(int mod)
{
    if (mod != MOD_RO && mod != MOD_RW)
        return;
    chmod = mod;
}

int IObserver::getChmod() const
{
    return chmod;
}

void IObserver::setDisplayIntBase(int base)
{

}
