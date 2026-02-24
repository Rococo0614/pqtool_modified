/****************************************************************************
** Meta object code from reading C++ file 'connect_device_window.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/component/window/connect_device_window.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'connect_device_window.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ConnectDeviceWindow_t {
    QByteArrayData data[12];
    char stringdata0[210];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ConnectDeviceWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ConnectDeviceWindow_t qt_meta_stringdata_ConnectDeviceWindow = {
    {
QT_MOC_LITERAL(0, 0, 19), // "ConnectDeviceWindow"
QT_MOC_LITERAL(1, 20, 10), // "sigConnect"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 29), // "sigIPAddressOrUartPortChanged"
QT_MOC_LITERAL(4, 62, 18), // "sigGetTemplateFile"
QT_MOC_LITERAL(5, 81, 19), // "handleConnectDevice"
QT_MOC_LITERAL(6, 101, 23), // "handleChangeConnectType"
QT_MOC_LITERAL(7, 125, 12), // "connect_type"
QT_MOC_LITERAL(8, 138, 19), // "CvipqToolSocConfirm"
QT_MOC_LITERAL(9, 158, 4), // "text"
QT_MOC_LITERAL(10, 163, 21), // "handleGetTemplateFile"
QT_MOC_LITERAL(11, 185, 24) // "handleSelectAutoJsonFile"

    },
    "ConnectDeviceWindow\0sigConnect\0\0"
    "sigIPAddressOrUartPortChanged\0"
    "sigGetTemplateFile\0handleConnectDevice\0"
    "handleChangeConnectType\0connect_type\0"
    "CvipqToolSocConfirm\0text\0handleGetTemplateFile\0"
    "handleSelectAutoJsonFile"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ConnectDeviceWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,
       3,    0,   55,    2, 0x06 /* Public */,
       4,    0,   56,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   57,    2, 0x08 /* Private */,
       6,    1,   58,    2, 0x08 /* Private */,
       8,    1,   61,    2, 0x08 /* Private */,
      10,    0,   64,    2, 0x08 /* Private */,
      11,    0,   65,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ConnectDeviceWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ConnectDeviceWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigConnect(); break;
        case 1: _t->sigIPAddressOrUartPortChanged(); break;
        case 2: _t->sigGetTemplateFile(); break;
        case 3: _t->handleConnectDevice(); break;
        case 4: _t->handleChangeConnectType((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->CvipqToolSocConfirm((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->handleGetTemplateFile(); break;
        case 7: _t->handleSelectAutoJsonFile(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ConnectDeviceWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ConnectDeviceWindow::sigConnect)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ConnectDeviceWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ConnectDeviceWindow::sigIPAddressOrUartPortChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ConnectDeviceWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ConnectDeviceWindow::sigGetTemplateFile)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ConnectDeviceWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_ConnectDeviceWindow.data,
    qt_meta_data_ConnectDeviceWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ConnectDeviceWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ConnectDeviceWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ConnectDeviceWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int ConnectDeviceWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void ConnectDeviceWindow::sigConnect()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ConnectDeviceWindow::sigIPAddressOrUartPortChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void ConnectDeviceWindow::sigGetTemplateFile()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
