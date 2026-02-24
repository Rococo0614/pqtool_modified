/****************************************************************************
** Meta object code from reading C++ file 'register_table_window.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/component/window/register_table_window.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'register_table_window.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RegisterTableWindow_t {
    QByteArrayData data[17];
    char stringdata0[262];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RegisterTableWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RegisterTableWindow_t qt_meta_stringdata_RegisterTableWindow = {
    {
QT_MOC_LITERAL(0, 0, 19), // "RegisterTableWindow"
QT_MOC_LITERAL(1, 20, 13), // "sigSwithChart"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 13), // "sigExportFile"
QT_MOC_LITERAL(4, 49, 9), // "file_path"
QT_MOC_LITERAL(5, 59, 13), // "sigImportFile"
QT_MOC_LITERAL(6, 73, 16), // "sigHideSumColumn"
QT_MOC_LITERAL(7, 90, 5), // "state"
QT_MOC_LITERAL(8, 96, 16), // "handleImportFile"
QT_MOC_LITERAL(9, 113, 16), // "handleExportFile"
QT_MOC_LITERAL(10, 130, 19), // "handleSwitchContent"
QT_MOC_LITERAL(11, 150, 19), // "readCurrentPageData"
QT_MOC_LITERAL(12, 170, 20), // "writeCurrentPageData"
QT_MOC_LITERAL(13, 191, 11), // "switchChart"
QT_MOC_LITERAL(14, 203, 18), // "handleCCMHideEvent"
QT_MOC_LITERAL(15, 222, 22), // "openCurveSettingDialog"
QT_MOC_LITERAL(16, 245, 16) // "showCCMSumButton"

    },
    "RegisterTableWindow\0sigSwithChart\0\0"
    "sigExportFile\0file_path\0sigImportFile\0"
    "sigHideSumColumn\0state\0handleImportFile\0"
    "handleExportFile\0handleSwitchContent\0"
    "readCurrentPageData\0writeCurrentPageData\0"
    "switchChart\0handleCCMHideEvent\0"
    "openCurveSettingDialog\0showCCMSumButton"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RegisterTableWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x06 /* Public */,
       3,    1,   80,    2, 0x06 /* Public */,
       5,    1,   83,    2, 0x06 /* Public */,
       6,    1,   86,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   89,    2, 0x08 /* Private */,
       9,    0,   90,    2, 0x08 /* Private */,
      10,    0,   91,    2, 0x08 /* Private */,
      11,    0,   92,    2, 0x08 /* Private */,
      12,    0,   93,    2, 0x08 /* Private */,
      13,    0,   94,    2, 0x08 /* Private */,
      14,    0,   95,    2, 0x08 /* Private */,
      15,    0,   96,    2, 0x08 /* Private */,
      16,    1,   97,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::Bool,    7,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    7,

       0        // eod
};

void RegisterTableWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RegisterTableWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigSwithChart(); break;
        case 1: _t->sigExportFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->sigImportFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->sigHideSumColumn((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->handleImportFile(); break;
        case 5: _t->handleExportFile(); break;
        case 6: _t->handleSwitchContent(); break;
        case 7: _t->readCurrentPageData(); break;
        case 8: _t->writeCurrentPageData(); break;
        case 9: _t->switchChart(); break;
        case 10: _t->handleCCMHideEvent(); break;
        case 11: _t->openCurveSettingDialog(); break;
        case 12: _t->showCCMSumButton((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (RegisterTableWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RegisterTableWindow::sigSwithChart)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (RegisterTableWindow::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RegisterTableWindow::sigExportFile)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (RegisterTableWindow::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RegisterTableWindow::sigImportFile)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (RegisterTableWindow::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RegisterTableWindow::sigHideSumColumn)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject RegisterTableWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_RegisterTableWindow.data,
    qt_meta_data_RegisterTableWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RegisterTableWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RegisterTableWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RegisterTableWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int RegisterTableWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void RegisterTableWindow::sigSwithChart()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void RegisterTableWindow::sigExportFile(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void RegisterTableWindow::sigImportFile(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void RegisterTableWindow::sigHideSumColumn(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
