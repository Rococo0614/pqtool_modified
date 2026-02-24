/****************************************************************************
** Meta object code from reading C++ file 'register_tool_window.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/component/window/register_tool_window.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'register_tool_window.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RegToolWindow_t {
    QByteArrayData data[22];
    char stringdata0[312];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RegToolWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RegToolWindow_t qt_meta_stringdata_RegToolWindow = {
    {
QT_MOC_LITERAL(0, 0, 13), // "RegToolWindow"
QT_MOC_LITERAL(1, 14, 10), // "importJson"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 15), // "onModuleChanged"
QT_MOC_LITERAL(4, 42, 10), // "moduleName"
QT_MOC_LITERAL(5, 53, 12), // "onRegChanged"
QT_MOC_LITERAL(6, 66, 7), // "regName"
QT_MOC_LITERAL(7, 74, 12), // "onGetClicked"
QT_MOC_LITERAL(8, 87, 12), // "onSetClicked"
QT_MOC_LITERAL(9, 100, 21), // "onModuleSearchChanged"
QT_MOC_LITERAL(10, 122, 10), // "searchText"
QT_MOC_LITERAL(11, 133, 18), // "onRegSearchChanged"
QT_MOC_LITERAL(12, 152, 26), // "onModuleCompletionSelected"
QT_MOC_LITERAL(13, 179, 4), // "text"
QT_MOC_LITERAL(14, 184, 23), // "onRegCompletionSelected"
QT_MOC_LITERAL(15, 208, 15), // "handleGetStatus"
QT_MOC_LITERAL(16, 224, 2), // "id"
QT_MOC_LITERAL(17, 227, 15), // "handleSetStatus"
QT_MOC_LITERAL(18, 243, 13), // "handleTimeOut"
QT_MOC_LITERAL(19, 257, 18), // "onValueTextChanged"
QT_MOC_LITERAL(20, 276, 17), // "onGetRadixChanged"
QT_MOC_LITERAL(21, 294, 17) // "onSetRadixChanged"

    },
    "RegToolWindow\0importJson\0\0onModuleChanged\0"
    "moduleName\0onRegChanged\0regName\0"
    "onGetClicked\0onSetClicked\0"
    "onModuleSearchChanged\0searchText\0"
    "onRegSearchChanged\0onModuleCompletionSelected\0"
    "text\0onRegCompletionSelected\0"
    "handleGetStatus\0id\0handleSetStatus\0"
    "handleTimeOut\0onValueTextChanged\0"
    "onGetRadixChanged\0onSetRadixChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RegToolWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x08 /* Private */,
       3,    1,   90,    2, 0x08 /* Private */,
       5,    1,   93,    2, 0x08 /* Private */,
       7,    0,   96,    2, 0x08 /* Private */,
       8,    0,   97,    2, 0x08 /* Private */,
       9,    1,   98,    2, 0x08 /* Private */,
      11,    1,  101,    2, 0x08 /* Private */,
      12,    1,  104,    2, 0x08 /* Private */,
      14,    1,  107,    2, 0x08 /* Private */,
      15,    1,  110,    2, 0x08 /* Private */,
      17,    1,  113,    2, 0x08 /* Private */,
      18,    0,  116,    2, 0x08 /* Private */,
      19,    1,  117,    2, 0x08 /* Private */,
      20,    0,  120,    2, 0x08 /* Private */,
      21,    0,  121,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void, QMetaType::Int,   16,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void RegToolWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RegToolWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->importJson(); break;
        case 1: _t->onModuleChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->onRegChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->onGetClicked(); break;
        case 4: _t->onSetClicked(); break;
        case 5: _t->onModuleSearchChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->onRegSearchChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->onModuleCompletionSelected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->onRegCompletionSelected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->handleGetStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->handleSetStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->handleTimeOut(); break;
        case 12: _t->onValueTextChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 13: _t->onGetRadixChanged(); break;
        case 14: _t->onSetRadixChanged(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject RegToolWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_RegToolWindow.data,
    qt_meta_data_RegToolWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RegToolWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RegToolWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RegToolWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int RegToolWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
