#ifndef INI_HELPER_HPP
#define INI_HELPER_HPP

#include "SimpleIni.h"

#include <memory>
#include <QObject>

using namespace std;

const QString DEFAULT_INI_FILE_NAME = "isp.ini";

class IniHelper : public QObject
{
    Q_OBJECT

public:
    explicit IniHelper();
    static shared_ptr<IniHelper> getInstance();
    bool reload();
    QList<std::tuple<QString, QString, QString>> readAllRegs();
    void writeReg(QString section, QString key, QString value);
    QString readReg(QString section, QString key, QString default_value = "");

private:
    static shared_ptr<IniHelper> m_instance;
    QString ini_path;
    CSimpleIniA* ini;
};

#endif // INI_HELPER_HPP
