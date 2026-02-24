/****************************************************************************
** Meta object code from reading C++ file 'calibration_capture_window.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/component/window/calibration_capture_window.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'calibration_capture_window.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CalibrationCaptureWindow_t {
    QByteArrayData data[13];
    char stringdata0[169];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CalibrationCaptureWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CalibrationCaptureWindow_t qt_meta_stringdata_CalibrationCaptureWindow = {
    {
QT_MOC_LITERAL(0, 0, 24), // "CalibrationCaptureWindow"
QT_MOC_LITERAL(1, 25, 20), // "handleModuleRWResult"
QT_MOC_LITERAL(2, 46, 0), // ""
QT_MOC_LITERAL(3, 47, 6), // "module"
QT_MOC_LITERAL(4, 54, 3), // "cmd"
QT_MOC_LITERAL(5, 58, 6), // "result"
QT_MOC_LITERAL(6, 65, 15), // "handleRawStatus"
QT_MOC_LITERAL(7, 81, 2), // "id"
QT_MOC_LITERAL(8, 84, 17), // "handleRecvBinInfo"
QT_MOC_LITERAL(9, 102, 17), // "handleRecvBinData"
QT_MOC_LITERAL(10, 120, 13), // "handleRawData"
QT_MOC_LITERAL(11, 134, 16), // "OpenSaveFilePath"
QT_MOC_LITERAL(12, 151, 17) // "CheckSaveFilePath"

    },
    "CalibrationCaptureWindow\0handleModuleRWResult\0"
    "\0module\0cmd\0result\0handleRawStatus\0"
    "id\0handleRecvBinInfo\0handleRecvBinData\0"
    "handleRawData\0OpenSaveFilePath\0"
    "CheckSaveFilePath"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CalibrationCaptureWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    3,   49,    2, 0x08 /* Private */,
       6,    1,   56,    2, 0x08 /* Private */,
       8,    1,   59,    2, 0x08 /* Private */,
       9,    1,   62,    2, 0x08 /* Private */,
      10,    1,   65,    2, 0x08 /* Private */,
      11,    0,   68,    2, 0x08 /* Private */,
      12,    0,   69,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,    3,    4,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,
    QMetaType::Bool,

       0        // eod
};

void CalibrationCaptureWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CalibrationCaptureWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->handleModuleRWResult((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 1: _t->handleRawStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->handleRecvBinInfo((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->handleRecvBinData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->handleRawData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->OpenSaveFilePath(); break;
        case 6: { bool _r = _t->CheckSaveFilePath();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CalibrationCaptureWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CalibrationCaptureWindow.data,
    qt_meta_data_CalibrationCaptureWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CalibrationCaptureWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CalibrationCaptureWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CalibrationCaptureWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int CalibrationCaptureWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
