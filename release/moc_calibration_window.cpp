/****************************************************************************
** Meta object code from reading C++ file 'calibration_window.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/component/window/calibration_window.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'calibration_window.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CalibrationWindow_t {
    QByteArrayData data[16];
    char stringdata0[250];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CalibrationWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CalibrationWindow_t qt_meta_stringdata_CalibrationWindow = {
    {
QT_MOC_LITERAL(0, 0, 17), // "CalibrationWindow"
QT_MOC_LITERAL(1, 18, 14), // "sigCloseWindow"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 17), // "handleOpenRawFile"
QT_MOC_LITERAL(4, 52, 15), // "handleLoadParam"
QT_MOC_LITERAL(5, 68, 15), // "handleSaveParam"
QT_MOC_LITERAL(6, 84, 21), // "handleUpdateRawConfig"
QT_MOC_LITERAL(7, 106, 15), // "handleImportRaw"
QT_MOC_LITERAL(8, 122, 26), // "BaseCalibrationPageWidget*"
QT_MOC_LITERAL(9, 149, 11), // "cali_widget"
QT_MOC_LITERAL(10, 161, 20), // "handleImportMultiRaw"
QT_MOC_LITERAL(11, 182, 18), // "handleUpdateStatus"
QT_MOC_LITERAL(12, 201, 18), // "CALIBRATION_STATUS"
QT_MOC_LITERAL(13, 220, 6), // "status"
QT_MOC_LITERAL(14, 227, 16), // "handleUpdateInfo"
QT_MOC_LITERAL(15, 244, 5) // "index"

    },
    "CalibrationWindow\0sigCloseWindow\0\0"
    "handleOpenRawFile\0handleLoadParam\0"
    "handleSaveParam\0handleUpdateRawConfig\0"
    "handleImportRaw\0BaseCalibrationPageWidget*\0"
    "cali_widget\0handleImportMultiRaw\0"
    "handleUpdateStatus\0CALIBRATION_STATUS\0"
    "status\0handleUpdateInfo\0index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CalibrationWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   60,    2, 0x08 /* Private */,
       4,    0,   61,    2, 0x08 /* Private */,
       5,    0,   62,    2, 0x08 /* Private */,
       6,    0,   63,    2, 0x08 /* Private */,
       7,    1,   64,    2, 0x08 /* Private */,
      10,    1,   67,    2, 0x08 /* Private */,
      11,    1,   70,    2, 0x08 /* Private */,
      14,    1,   73,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void, QMetaType::Int,   15,

       0        // eod
};

void CalibrationWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CalibrationWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigCloseWindow(); break;
        case 1: _t->handleOpenRawFile(); break;
        case 2: _t->handleLoadParam(); break;
        case 3: _t->handleSaveParam(); break;
        case 4: _t->handleUpdateRawConfig(); break;
        case 5: _t->handleImportRaw((*reinterpret_cast< BaseCalibrationPageWidget*(*)>(_a[1]))); break;
        case 6: _t->handleImportMultiRaw((*reinterpret_cast< BaseCalibrationPageWidget*(*)>(_a[1]))); break;
        case 7: _t->handleUpdateStatus((*reinterpret_cast< CALIBRATION_STATUS(*)>(_a[1]))); break;
        case 8: _t->handleUpdateInfo((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< BaseCalibrationPageWidget* >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< BaseCalibrationPageWidget* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CalibrationWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalibrationWindow::sigCloseWindow)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CalibrationWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CalibrationWindow.data,
    qt_meta_data_CalibrationWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CalibrationWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CalibrationWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CalibrationWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int CalibrationWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void CalibrationWindow::sigCloseWindow()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
