#include "ini_helper.hpp"
#include "utils.hpp"

#include <QDir>

shared_ptr<IniHelper> IniHelper::m_instance = nullptr;

IniHelper::IniHelper()
{
    ini_path = QDir::currentPath() + PATH_SEPARATOR + "cmodel" + PATH_SEPARATOR + "release" + PATH_SEPARATOR + DEFAULT_INI_FILE_NAME;
    ini = new CSimpleIniA(true, false, true);
    SI_Error rc = ini->LoadFile(ini_path.toStdString().c_str());
    if (rc < 0) {
        qDebug("Load ini file failed");
    }
}

shared_ptr<IniHelper> IniHelper::getInstance()
{
    if (nullptr == IniHelper::m_instance) {
        IniHelper::m_instance = make_shared<IniHelper>();
    }

    return IniHelper::m_instance;
}

bool IniHelper::reload()
{
    return (ini->LoadFile(ini_path.toStdString().c_str()) < 0) ? false : true;
}

QList<std::tuple<QString, QString, QString>> IniHelper::readAllRegs()
{
    QList<std::tuple<QString, QString, QString>> result_list;
    CSimpleIniA::TNamesDepend sections;
    ini->GetAllSections(sections);
    for (auto section : sections) {
        CSimpleIniA::TNamesDepend keys;
        ini->GetAllKeys(section.pItem, keys);

        for (auto key : keys) {
             const char * pszValue = ini->GetValue(section.pItem, key.pItem, "");
             qDebug("read reg [%s] [%s]:%s", qUtf8Printable(section.pItem), qUtf8Printable(key.pItem), pszValue);

             std::tuple<QString, QString, QString> tuple = make_tuple(QString(section.pItem), QString(key.pItem),
                                                                      QString(pszValue).remove(QRegExp("[ \n\t]")));
             result_list.append(tuple);
        }
    }

    return result_list;
}

void IniHelper::writeReg(QString section, QString key, QString value)
{
    if (section.isEmpty() || key.isEmpty() || value.isEmpty()) {
        return;
    }

    SI_Error rc = ini->SetValue(section.toStdString().c_str(), key.toStdString().c_str(), value.toStdString().c_str());
    if (rc < 0) {
        qDebug("Ini setValue failed");
    }

    rc = ini->SaveFile(ini_path.toStdString().c_str());
    if (rc < 0) {
        qDebug("Ini saveFile failed");
    }
}

QString IniHelper::readReg(QString section, QString key, QString default_value)
{
    if (section.isEmpty() || key.isEmpty()) {
        return default_value;
    }

    const char * value = ini->GetValue(section.toStdString().c_str(), key.toStdString().c_str(), default_value.toStdString().c_str());
    return QString(value).remove(QRegExp("[ \n\t]"));
}
