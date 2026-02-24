/****************************************************************************
** Meta object code from reading C++ file 'main_window.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/main_window.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'main_window.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_IniReadWorker_t {
    QByteArrayData data[7];
    char stringdata0[64];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IniReadWorker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IniReadWorker_t qt_meta_stringdata_IniReadWorker = {
    {
QT_MOC_LITERAL(0, 0, 13), // "IniReadWorker"
QT_MOC_LITERAL(1, 14, 12), // "sigUpdateReg"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 7), // "section"
QT_MOC_LITERAL(4, 36, 3), // "key"
QT_MOC_LITERAL(5, 40, 9), // "value_str"
QT_MOC_LITERAL(6, 50, 13) // "sigFinishRead"

    },
    "IniReadWorker\0sigUpdateReg\0\0section\0"
    "key\0value_str\0sigFinishRead"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IniReadWorker[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   24,    2, 0x06 /* Public */,
       6,    0,   31,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    3,    4,    5,
    QMetaType::Void,

       0        // eod
};

void IniReadWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<IniReadWorker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigUpdateReg((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 1: _t->sigFinishRead(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (IniReadWorker::*)(QString , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IniReadWorker::sigUpdateReg)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (IniReadWorker::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IniReadWorker::sigFinishRead)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject IniReadWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QThread::staticMetaObject>(),
    qt_meta_stringdata_IniReadWorker.data,
    qt_meta_data_IniReadWorker,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *IniReadWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IniReadWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_IniReadWorker.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int IniReadWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void IniReadWorker::sigUpdateReg(QString _t1, QString _t2, QString _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void IniReadWorker::sigFinishRead()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[99];
    char stringdata0[1606];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 23), // "openConnectDeviceWindow"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 14), // "sigToggleTheme"
QT_MOC_LITERAL(4, 51, 18), // "dark_theme_enabled"
QT_MOC_LITERAL(5, 70, 12), // "sigStatusLog"
QT_MOC_LITERAL(6, 83, 7), // "message"
QT_MOC_LITERAL(7, 91, 9), // "LOG_LEVEL"
QT_MOC_LITERAL(8, 101, 9), // "log_level"
QT_MOC_LITERAL(9, 111, 11), // "dataUpdated"
QT_MOC_LITERAL(10, 123, 26), // "IPAddressOrUartPortChanged"
QT_MOC_LITERAL(11, 150, 17), // "handleUpdatePages"
QT_MOC_LITERAL(12, 168, 23), // "handleShowSystemConsole"
QT_MOC_LITERAL(13, 192, 18), // "handleDataModified"
QT_MOC_LITERAL(14, 211, 10), // "SecKeyType"
QT_MOC_LITERAL(15, 222, 12), // "writeAllData"
QT_MOC_LITERAL(16, 235, 8), // "openFile"
QT_MOC_LITERAL(17, 244, 8), // "filePath"
QT_MOC_LITERAL(18, 253, 21), // "showSuccessfulMessage"
QT_MOC_LITERAL(19, 275, 8), // "saveFile"
QT_MOC_LITERAL(20, 284, 15), // "saveFileJsonrpc"
QT_MOC_LITERAL(21, 300, 25), // "loadDataFromOldJsonFormat"
QT_MOC_LITERAL(22, 326, 9), // "file_path"
QT_MOC_LITERAL(23, 336, 25), // "loadDataFromNewJsonFormat"
QT_MOC_LITERAL(24, 362, 18), // "loadParamsToBuffer"
QT_MOC_LITERAL(25, 381, 2), // "id"
QT_MOC_LITERAL(26, 384, 11), // "QJsonArray&"
QT_MOC_LITERAL(27, 396, 5), // "value"
QT_MOC_LITERAL(28, 402, 20), // "enableAllPageButtons"
QT_MOC_LITERAL(29, 423, 6), // "module"
QT_MOC_LITERAL(30, 430, 3), // "cmd"
QT_MOC_LITERAL(31, 434, 11), // "BUTTON_MODE"
QT_MOC_LITERAL(32, 446, 4), // "mode"
QT_MOC_LITERAL(33, 451, 21), // "disableAllPageButtons"
QT_MOC_LITERAL(34, 473, 27), // "matchMenuItemofSelectedPage"
QT_MOC_LITERAL(35, 501, 5), // "index"
QT_MOC_LITERAL(36, 507, 16), // "handleSearchItem"
QT_MOC_LITERAL(37, 524, 4), // "item"
QT_MOC_LITERAL(38, 529, 19), // "getParamsFromBuffer"
QT_MOC_LITERAL(39, 549, 11), // "readAllData"
QT_MOC_LITERAL(40, 561, 14), // "readPageManual"
QT_MOC_LITERAL(41, 576, 15), // "writePageManual"
QT_MOC_LITERAL(42, 592, 12), // "selectModule"
QT_MOC_LITERAL(43, 605, 13), // "connectDevice"
QT_MOC_LITERAL(44, 619, 16), // "disconnectDevice"
QT_MOC_LITERAL(45, 636, 12), // "fetchRawData"
QT_MOC_LITERAL(46, 649, 17), // "showCaptureWindow"
QT_MOC_LITERAL(47, 667, 16), // "showCModelWindow"
QT_MOC_LITERAL(48, 684, 20), // "showCModelCtrlWindow"
QT_MOC_LITERAL(49, 705, 21), // "showCalibrationWindow"
QT_MOC_LITERAL(50, 727, 30), // "showCalibrationDistortioWindow"
QT_MOC_LITERAL(51, 758, 28), // "showCalibrationCaptureWindow"
QT_MOC_LITERAL(52, 787, 31), // "showImportCalibrationJsonWindow"
QT_MOC_LITERAL(53, 819, 25), // "showCalibrationPdafWindow"
QT_MOC_LITERAL(54, 845, 23), // "toggleShowRegisterPanel"
QT_MOC_LITERAL(55, 869, 18), // "handleIniUpdateReg"
QT_MOC_LITERAL(56, 888, 7), // "section"
QT_MOC_LITERAL(57, 896, 3), // "key"
QT_MOC_LITERAL(58, 900, 9), // "value_str"
QT_MOC_LITERAL(59, 910, 26), // "handleIniFinishReadAllData"
QT_MOC_LITERAL(60, 937, 17), // "showBinUtilWindow"
QT_MOC_LITERAL(61, 955, 17), // "showFishEyeWindow"
QT_MOC_LITERAL(62, 973, 17), // "showRawUtilWindow"
QT_MOC_LITERAL(63, 991, 17), // "showYuvUtilWindow"
QT_MOC_LITERAL(64, 1009, 22), // "showAutoAnalyserWindow"
QT_MOC_LITERAL(65, 1032, 24), // "showFocusAssistantWindow"
QT_MOC_LITERAL(66, 1057, 20), // "showBracketingWindow"
QT_MOC_LITERAL(67, 1078, 23), // "showContinuousRawWindow"
QT_MOC_LITERAL(68, 1102, 18), // "showAnalyzerWindow"
QT_MOC_LITERAL(69, 1121, 19), // "showI2cEditorWindow"
QT_MOC_LITERAL(70, 1141, 17), // "showRegToolWindow"
QT_MOC_LITERAL(71, 1159, 19), // "showSpiEditorWindow"
QT_MOC_LITERAL(72, 1179, 17), // "handleGetChipInfo"
QT_MOC_LITERAL(73, 1197, 17), // "handleToggleTheme"
QT_MOC_LITERAL(74, 1215, 24), // "handleCheckGainSepStatus"
QT_MOC_LITERAL(75, 1240, 23), // "showDrcToneViewerWindow"
QT_MOC_LITERAL(76, 1264, 17), // "updateEvShowState"
QT_MOC_LITERAL(77, 1282, 13), // "checked_state"
QT_MOC_LITERAL(78, 1296, 18), // "handleTimeOutEvent"
QT_MOC_LITERAL(79, 1315, 20), // "updateAllPageEvValue"
QT_MOC_LITERAL(80, 1336, 22), // "handleAutoApplyTimeout"
QT_MOC_LITERAL(81, 1359, 10), // "searchItem"
QT_MOC_LITERAL(82, 1370, 14), // "page_order_idx"
QT_MOC_LITERAL(83, 1385, 8), // "QString&"
QT_MOC_LITERAL(84, 1394, 5), // "title"
QT_MOC_LITERAL(85, 1400, 20), // "soltModuleListUpdate"
QT_MOC_LITERAL(86, 1421, 4), // "text"
QT_MOC_LITERAL(87, 1426, 16), // "updateModuleList"
QT_MOC_LITERAL(88, 1443, 7), // "modules"
QT_MOC_LITERAL(89, 1451, 16), // "onFetchCompleted"
QT_MOC_LITERAL(90, 1468, 12), // "fetchStarted"
QT_MOC_LITERAL(91, 1481, 10), // "undoAction"
QT_MOC_LITERAL(92, 1492, 10), // "redoAction"
QT_MOC_LITERAL(93, 1503, 12), // "loadCaliData"
QT_MOC_LITERAL(94, 1516, 15), // "selectedModules"
QT_MOC_LITERAL(95, 1532, 19), // "filterLscParameters"
QT_MOC_LITERAL(96, 1552, 13), // "originalValue"
QT_MOC_LITERAL(97, 1566, 19), // "filterCcmParameters"
QT_MOC_LITERAL(98, 1586, 19) // "filterAwbParameters"

    },
    "MainWindow\0openConnectDeviceWindow\0\0"
    "sigToggleTheme\0dark_theme_enabled\0"
    "sigStatusLog\0message\0LOG_LEVEL\0log_level\0"
    "dataUpdated\0IPAddressOrUartPortChanged\0"
    "handleUpdatePages\0handleShowSystemConsole\0"
    "handleDataModified\0SecKeyType\0"
    "writeAllData\0openFile\0filePath\0"
    "showSuccessfulMessage\0saveFile\0"
    "saveFileJsonrpc\0loadDataFromOldJsonFormat\0"
    "file_path\0loadDataFromNewJsonFormat\0"
    "loadParamsToBuffer\0id\0QJsonArray&\0"
    "value\0enableAllPageButtons\0module\0cmd\0"
    "BUTTON_MODE\0mode\0disableAllPageButtons\0"
    "matchMenuItemofSelectedPage\0index\0"
    "handleSearchItem\0item\0getParamsFromBuffer\0"
    "readAllData\0readPageManual\0writePageManual\0"
    "selectModule\0connectDevice\0disconnectDevice\0"
    "fetchRawData\0showCaptureWindow\0"
    "showCModelWindow\0showCModelCtrlWindow\0"
    "showCalibrationWindow\0"
    "showCalibrationDistortioWindow\0"
    "showCalibrationCaptureWindow\0"
    "showImportCalibrationJsonWindow\0"
    "showCalibrationPdafWindow\0"
    "toggleShowRegisterPanel\0handleIniUpdateReg\0"
    "section\0key\0value_str\0handleIniFinishReadAllData\0"
    "showBinUtilWindow\0showFishEyeWindow\0"
    "showRawUtilWindow\0showYuvUtilWindow\0"
    "showAutoAnalyserWindow\0showFocusAssistantWindow\0"
    "showBracketingWindow\0showContinuousRawWindow\0"
    "showAnalyzerWindow\0showI2cEditorWindow\0"
    "showRegToolWindow\0showSpiEditorWindow\0"
    "handleGetChipInfo\0handleToggleTheme\0"
    "handleCheckGainSepStatus\0"
    "showDrcToneViewerWindow\0updateEvShowState\0"
    "checked_state\0handleTimeOutEvent\0"
    "updateAllPageEvValue\0handleAutoApplyTimeout\0"
    "searchItem\0page_order_idx\0QString&\0"
    "title\0soltModuleListUpdate\0text\0"
    "updateModuleList\0modules\0onFetchCompleted\0"
    "fetchStarted\0undoAction\0redoAction\0"
    "loadCaliData\0selectedModules\0"
    "filterLscParameters\0originalValue\0"
    "filterCcmParameters\0filterAwbParameters"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      73,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  379,    2, 0x06 /* Public */,
       3,    1,  380,    2, 0x06 /* Public */,
       5,    2,  383,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,  388,    2, 0x0a /* Public */,
      10,    0,  389,    2, 0x0a /* Public */,
      11,    0,  390,    2, 0x0a /* Public */,
      12,    0,  391,    2, 0x0a /* Public */,
      13,    1,  392,    2, 0x0a /* Public */,
      15,    0,  395,    2, 0x0a /* Public */,
      16,    2,  396,    2, 0x0a /* Public */,
      16,    1,  401,    2, 0x2a /* Public | MethodCloned */,
      16,    0,  404,    2, 0x2a /* Public | MethodCloned */,
      19,    1,  405,    2, 0x0a /* Public */,
      19,    0,  408,    2, 0x2a /* Public | MethodCloned */,
      20,    1,  409,    2, 0x0a /* Public */,
      20,    0,  412,    2, 0x2a /* Public | MethodCloned */,
      21,    1,  413,    2, 0x0a /* Public */,
      23,    1,  416,    2, 0x0a /* Public */,
      24,    2,  419,    2, 0x0a /* Public */,
      28,    3,  424,    2, 0x0a /* Public */,
      33,    0,  431,    2, 0x0a /* Public */,
      34,    1,  432,    2, 0x0a /* Public */,
      36,    1,  435,    2, 0x0a /* Public */,
      38,    1,  438,    2, 0x0a /* Public */,
      39,    0,  441,    2, 0x08 /* Private */,
      40,    1,  442,    2, 0x08 /* Private */,
      41,    1,  445,    2, 0x08 /* Private */,
      42,    1,  448,    2, 0x08 /* Private */,
      43,    0,  451,    2, 0x08 /* Private */,
      44,    0,  452,    2, 0x08 /* Private */,
      45,    0,  453,    2, 0x08 /* Private */,
      46,    0,  454,    2, 0x08 /* Private */,
      47,    0,  455,    2, 0x08 /* Private */,
      48,    0,  456,    2, 0x08 /* Private */,
      49,    0,  457,    2, 0x08 /* Private */,
      50,    0,  458,    2, 0x08 /* Private */,
      51,    0,  459,    2, 0x08 /* Private */,
      52,    0,  460,    2, 0x08 /* Private */,
      53,    0,  461,    2, 0x08 /* Private */,
      54,    0,  462,    2, 0x08 /* Private */,
      55,    3,  463,    2, 0x08 /* Private */,
      59,    0,  470,    2, 0x08 /* Private */,
      60,    0,  471,    2, 0x08 /* Private */,
      61,    0,  472,    2, 0x08 /* Private */,
      62,    0,  473,    2, 0x08 /* Private */,
      63,    0,  474,    2, 0x08 /* Private */,
      64,    0,  475,    2, 0x08 /* Private */,
      65,    0,  476,    2, 0x08 /* Private */,
      66,    0,  477,    2, 0x08 /* Private */,
      67,    0,  478,    2, 0x08 /* Private */,
      68,    0,  479,    2, 0x08 /* Private */,
      69,    0,  480,    2, 0x08 /* Private */,
      70,    0,  481,    2, 0x08 /* Private */,
      71,    0,  482,    2, 0x08 /* Private */,
      72,    1,  483,    2, 0x08 /* Private */,
      73,    0,  486,    2, 0x08 /* Private */,
      74,    0,  487,    2, 0x08 /* Private */,
      75,    0,  488,    2, 0x08 /* Private */,
      76,    1,  489,    2, 0x08 /* Private */,
      78,    0,  492,    2, 0x08 /* Private */,
      79,    1,  493,    2, 0x08 /* Private */,
      80,    0,  496,    2, 0x08 /* Private */,
      81,    2,  497,    2, 0x08 /* Private */,
      85,    1,  502,    2, 0x08 /* Private */,
      87,    1,  505,    2, 0x08 /* Private */,
      89,    0,  508,    2, 0x08 /* Private */,
      90,    0,  509,    2, 0x08 /* Private */,
      91,    0,  510,    2, 0x08 /* Private */,
      92,    0,  511,    2, 0x08 /* Private */,
      93,    2,  512,    2, 0x08 /* Private */,
      95,    1,  517,    2, 0x08 /* Private */,
      97,    1,  520,    2, 0x08 /* Private */,
      98,    1,  523,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    4,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 7,    6,    8,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 14,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,   17,   18,
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void,
    QMetaType::Int, QMetaType::QString,   22,
    QMetaType::Int, QMetaType::QString,   22,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 26,   25,   27,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 31,   29,   30,   32,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   35,
    QMetaType::Void, QMetaType::QString,   37,
    QMetaType::QJsonArray, QMetaType::QString,   25,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   25,
    QMetaType::Void, QMetaType::QString,   25,
    QMetaType::Void, QMetaType::QString,   29,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   56,   57,   58,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   25,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   77,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   25,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 83,   82,   84,
    QMetaType::Void, QMetaType::QString,   86,
    QMetaType::Void, QMetaType::QStringList,   88,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Int, QMetaType::QString, QMetaType::QStringList,   17,   94,
    QMetaType::QJsonArray, QMetaType::QJsonArray,   96,
    QMetaType::QJsonArray, QMetaType::QJsonArray,   96,
    QMetaType::QJsonArray, QMetaType::QJsonArray,   96,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->openConnectDeviceWindow(); break;
        case 1: _t->sigToggleTheme((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->sigStatusLog((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< LOG_LEVEL(*)>(_a[2]))); break;
        case 3: _t->dataUpdated(); break;
        case 4: _t->IPAddressOrUartPortChanged(); break;
        case 5: _t->handleUpdatePages(); break;
        case 6: _t->handleShowSystemConsole(); break;
        case 7: _t->handleDataModified((*reinterpret_cast< SecKeyType(*)>(_a[1]))); break;
        case 8: _t->writeAllData(); break;
        case 9: _t->openFile((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 10: _t->openFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->openFile(); break;
        case 12: _t->saveFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: _t->saveFile(); break;
        case 14: _t->saveFileJsonrpc((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 15: _t->saveFileJsonrpc(); break;
        case 16: { int _r = _t->loadDataFromOldJsonFormat((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 17: { int _r = _t->loadDataFromNewJsonFormat((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 18: _t->loadParamsToBuffer((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QJsonArray(*)>(_a[2]))); break;
        case 19: _t->enableAllPageButtons((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< BUTTON_MODE(*)>(_a[3]))); break;
        case 20: _t->disableAllPageButtons(); break;
        case 21: _t->matchMenuItemofSelectedPage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: _t->handleSearchItem((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 23: { QJsonArray _r = _t->getParamsFromBuffer((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QJsonArray*>(_a[0]) = std::move(_r); }  break;
        case 24: _t->readAllData(); break;
        case 25: _t->readPageManual((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 26: _t->writePageManual((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 27: _t->selectModule((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 28: _t->connectDevice(); break;
        case 29: _t->disconnectDevice(); break;
        case 30: _t->fetchRawData(); break;
        case 31: _t->showCaptureWindow(); break;
        case 32: _t->showCModelWindow(); break;
        case 33: _t->showCModelCtrlWindow(); break;
        case 34: _t->showCalibrationWindow(); break;
        case 35: _t->showCalibrationDistortioWindow(); break;
        case 36: _t->showCalibrationCaptureWindow(); break;
        case 37: _t->showImportCalibrationJsonWindow(); break;
        case 38: _t->showCalibrationPdafWindow(); break;
        case 39: _t->toggleShowRegisterPanel(); break;
        case 40: _t->handleIniUpdateReg((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 41: _t->handleIniFinishReadAllData(); break;
        case 42: _t->showBinUtilWindow(); break;
        case 43: _t->showFishEyeWindow(); break;
        case 44: _t->showRawUtilWindow(); break;
        case 45: _t->showYuvUtilWindow(); break;
        case 46: _t->showAutoAnalyserWindow(); break;
        case 47: _t->showFocusAssistantWindow(); break;
        case 48: _t->showBracketingWindow(); break;
        case 49: _t->showContinuousRawWindow(); break;
        case 50: _t->showAnalyzerWindow(); break;
        case 51: _t->showI2cEditorWindow(); break;
        case 52: _t->showRegToolWindow(); break;
        case 53: _t->showSpiEditorWindow(); break;
        case 54: _t->handleGetChipInfo((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 55: _t->handleToggleTheme(); break;
        case 56: _t->handleCheckGainSepStatus(); break;
        case 57: _t->showDrcToneViewerWindow(); break;
        case 58: _t->updateEvShowState((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 59: _t->handleTimeOutEvent(); break;
        case 60: _t->updateAllPageEvValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 61: _t->handleAutoApplyTimeout(); break;
        case 62: _t->searchItem((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 63: _t->soltModuleListUpdate((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 64: _t->updateModuleList((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 65: _t->onFetchCompleted(); break;
        case 66: _t->fetchStarted(); break;
        case 67: _t->undoAction(); break;
        case 68: _t->redoAction(); break;
        case 69: { int _r = _t->loadCaliData((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QStringList(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 70: { QJsonArray _r = _t->filterLscParameters((*reinterpret_cast< const QJsonArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QJsonArray*>(_a[0]) = std::move(_r); }  break;
        case 71: { QJsonArray _r = _t->filterCcmParameters((*reinterpret_cast< const QJsonArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QJsonArray*>(_a[0]) = std::move(_r); }  break;
        case 72: { QJsonArray _r = _t->filterAwbParameters((*reinterpret_cast< const QJsonArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QJsonArray*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::openConnectDeviceWindow)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::sigToggleTheme)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(QString , LOG_LEVEL );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::sigStatusLog)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 73)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 73;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 73)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 73;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::openConnectDeviceWindow()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void MainWindow::sigToggleTheme(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MainWindow::sigStatusLog(QString _t1, LOG_LEVEL _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
