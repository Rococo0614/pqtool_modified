#include "main_window.hpp"
#include "connect_device_window.hpp"
#include "global_data.hpp"

#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setEffectEnabled(Qt::UI_AnimateCombo, false);

    MainWindow::getInstance()->show();
    QString langStr = GlobalData::getInstance()->getSettings(SETTINGS_SECTION, KEY_LANG, "0").toString();
    //MainWindow::LANG lang = static_cast<MainWindow::LANG>(langStr.toInt());
    //main_window.changeLanguage(lang);

    ConnectDeviceWindow connect_dev_window;
    connect_dev_window.show();

    QObject::connect(GlobalData::getInstance().get(), &GlobalData::sigDataLoaded,
        MainWindow::getInstance().get(), &MainWindow::dataUpdated);
    QObject::connect(GlobalData::getInstance().get(), &GlobalData::sigDataModified,
        MainWindow::getInstance().get(), &MainWindow::handleDataModified);
    QObject::connect(GlobalData::getInstance().get(), &GlobalData::sigUpdateTemplate,
        &connect_dev_window, &ConnectDeviceWindow::handleUpdateTemplate);
    QObject::connect(&connect_dev_window, &ConnectDeviceWindow::sigConnect,
        MainWindow::getInstance().get(), &MainWindow::handleUpdatePages);
    QObject::connect(&connect_dev_window, &ConnectDeviceWindow::sigIPAddressOrUartPortChanged,
        MainWindow::getInstance().get(), &MainWindow::IPAddressOrUartPortChanged);
    QObject::connect(MainWindow::getInstance().get(), &MainWindow::openConnectDeviceWindow,
        &connect_dev_window, &ConnectDeviceWindow::show);

    return app.exec();
}
