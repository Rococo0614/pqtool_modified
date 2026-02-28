#-------------------------------------------------
#
# Project created by QtCreator 2019-01-25T13:34:11
#
#-------------------------------------------------

QT       += core gui charts websockets quickwidgets quickcontrols2 gui-private
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = CviPQTool
TEMPLATE = app

TRANSLATIONS += languages/isp_tool.en.ts languages/isp_tool.zh_TW.ts languages/isp_tool.zh_CN.ts

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    main.cpp \
    $$files(view/src/*.cpp) \
    $$files(model/src/*.cpp) \
    $$files(model/src/dewarp/*.cpp) \
    $$files(view/src/component/*.cpp) \
    $$files(view/src/component/chart/*.cpp) \
    $$files(view/src/component/register/*.cpp) \
    $$files(view/src/component/register/register_item/*.cpp) \
    $$files(view/src/component/register/calibration_item/*.cpp) \
    $$files(view/src/component/image/*.cpp) \
    $$files(view/src/component/dialog/*.cpp) \
    $$files(view/src/component/window/*.cpp) \
    $$files(view/src/component/list/*.cpp) \
    $$files(view/src/pages/*.cpp) \
    $$files(test/src/*.cpp) \
    $$files(3rdparty/QCustomPlot/src/*.cpp)

HEADERS += \
    $$files(view/include/*.hpp) \
    $$files(model/include/*.hpp) \
    $$files(model/include/dewarp/*.h) \
    $$files(view/include/component/*.hpp) \
    $$files(view/include/component/chart/*.hpp) \
    $$files(view/include/component/register/*.hpp) \
    $$files(view/include/component/register/register_item/*.hpp) \
    $$files(view/include/component/register/calibration_item/*.hpp) \
    $$files(view/include/component/image/*.hpp) \
    $$files(view/include/component/dialog/*.hpp) \
    $$files(view/include/component/window/*.hpp) \
    $$files(view/include/component/list/*.hpp) \
    $$files(view/include/pages/*.hpp) \
    $$files(3rdparty/include/SimpleIni/*.h) \
    $$files(3rdparty/Calibration/*.h) \
    $$files(3rdparty/rgbirCalibration/*.h) \
    $$files(3rdparty/QCustomPlot/include/*.h)

INCLUDEPATH += view/include \
    view/include/component \
    view/include/component/chart \
    view/include/component/register \
    view/include/component/register/register_item \
    view/include/component/register/calibration_item \
    view/include/component/image \
    view/include/component/dialog \
    view/include/component/window \
    view/include/component/list \
    view/include/pages \
    model/include \
    model/include/dewarp \
    3rdparty/SimpleIni \
    3rdparty/Eigen \
    3rdparty/Calibration \
    3rdparty/rgbirCalibration \
    3rdparty/opencv/ \
    3rdparty/QCustomPlot/include

LIBS += -L$$PWD/libs/ -lcalibration -lrgbir -lcamcalib -lgenmesh -lsophldc -lopencv_world3416 

DISTFILES += \
    jsons/*.json \
    qml/*.qml

RESOURCES += \
    resource.qrc \
    qdarkstyle/dark/style.qrc

RC_FILE=icon.rc

CONFIG(debug, debug|release) {
    message("debug mode")
    build_mode = "debug"
} else {
    message("release mode")
    build_mode = "release"
}

## Disable automatic portable packaging during build to avoid "sharing conflict" when files are locked by other processes.
## To run packaging manually after a successful build, run: pack_portable.bat <repo_path> <qt_install_prefix> <branch> <build_mode>
# QMAKE_POST_LINK += $$PWD/pack_portable.bat $$PWD $$[QT_INSTALL_PREFIX] $$[BRANCH] $$build_mode

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

