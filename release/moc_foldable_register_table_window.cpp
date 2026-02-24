/****************************************************************************
** Meta object code from reading C++ file 'foldable_register_table_window.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/component/window/foldable_register_table_window.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'foldable_register_table_window.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FoldableRegisterTableWindow_t {
    QByteArrayData data[10];
    char stringdata0[140];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FoldableRegisterTableWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FoldableRegisterTableWindow_t qt_meta_stringdata_FoldableRegisterTableWindow = {
    {
QT_MOC_LITERAL(0, 0, 27), // "FoldableRegisterTableWindow"
QT_MOC_LITERAL(1, 28, 9), // "sigToggle"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 10), // "fold_state"
QT_MOC_LITERAL(4, 50, 13), // "sigExportFile"
QT_MOC_LITERAL(5, 64, 9), // "file_path"
QT_MOC_LITERAL(6, 74, 13), // "sigImportFile"
QT_MOC_LITERAL(7, 88, 17), // "handleToggleEvent"
QT_MOC_LITERAL(8, 106, 16), // "handleImportFile"
QT_MOC_LITERAL(9, 123, 16) // "handleExportFile"

    },
    "FoldableRegisterTableWindow\0sigToggle\0"
    "\0fold_state\0sigExportFile\0file_path\0"
    "sigImportFile\0handleToggleEvent\0"
    "handleImportFile\0handleExportFile"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FoldableRegisterTableWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       4,    1,   47,    2, 0x06 /* Public */,
       6,    1,   50,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   53,    2, 0x08 /* Private */,
       8,    0,   54,    2, 0x08 /* Private */,
       9,    0,   55,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void FoldableRegisterTableWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FoldableRegisterTableWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigToggle((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->sigExportFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->sigImportFile((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->handleToggleEvent(); break;
        case 4: _t->handleImportFile(); break;
        case 5: _t->handleExportFile(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (FoldableRegisterTableWindow::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FoldableRegisterTableWindow::sigToggle)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (FoldableRegisterTableWindow::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FoldableRegisterTableWindow::sigExportFile)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (FoldableRegisterTableWindow::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FoldableRegisterTableWindow::sigImportFile)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FoldableRegisterTableWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_FoldableRegisterTableWindow.data,
    qt_meta_data_FoldableRegisterTableWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *FoldableRegisterTableWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FoldableRegisterTableWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FoldableRegisterTableWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int FoldableRegisterTableWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void FoldableRegisterTableWindow::sigToggle(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FoldableRegisterTableWindow::sigExportFile(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void FoldableRegisterTableWindow::sigImportFile(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
