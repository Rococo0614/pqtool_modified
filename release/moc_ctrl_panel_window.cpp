/****************************************************************************
** Meta object code from reading C++ file 'ctrl_panel_window.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/component/window/ctrl_panel_window.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ctrl_panel_window.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CtrlPanelWindow_t {
    QByteArrayData data[14];
    char stringdata0[165];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CtrlPanelWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CtrlPanelWindow_t qt_meta_stringdata_CtrlPanelWindow = {
    {
QT_MOC_LITERAL(0, 0, 15), // "CtrlPanelWindow"
QT_MOC_LITERAL(1, 16, 8), // "sigKeyUp"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 10), // "sigKeyDown"
QT_MOC_LITERAL(4, 37, 10), // "sigKeyLeft"
QT_MOC_LITERAL(5, 48, 11), // "sigKeyRight"
QT_MOC_LITERAL(6, 60, 10), // "sigKeyHome"
QT_MOC_LITERAL(7, 71, 13), // "sigKeyZoomInH"
QT_MOC_LITERAL(8, 85, 14), // "sigKeyZoomOutH"
QT_MOC_LITERAL(9, 100, 13), // "sigKeyZoomInV"
QT_MOC_LITERAL(10, 114, 14), // "sigKeyZoomOutV"
QT_MOC_LITERAL(11, 129, 19), // "handleButtonPressed"
QT_MOC_LITERAL(12, 149, 11), // "CONTROL_CMD"
QT_MOC_LITERAL(13, 161, 3) // "cmd"

    },
    "CtrlPanelWindow\0sigKeyUp\0\0sigKeyDown\0"
    "sigKeyLeft\0sigKeyRight\0sigKeyHome\0"
    "sigKeyZoomInH\0sigKeyZoomOutH\0sigKeyZoomInV\0"
    "sigKeyZoomOutV\0handleButtonPressed\0"
    "CONTROL_CMD\0cmd"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CtrlPanelWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06 /* Public */,
       3,    0,   65,    2, 0x06 /* Public */,
       4,    0,   66,    2, 0x06 /* Public */,
       5,    0,   67,    2, 0x06 /* Public */,
       6,    0,   68,    2, 0x06 /* Public */,
       7,    0,   69,    2, 0x06 /* Public */,
       8,    0,   70,    2, 0x06 /* Public */,
       9,    0,   71,    2, 0x06 /* Public */,
      10,    0,   72,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    1,   73,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 12,   13,

       0        // eod
};

void CtrlPanelWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CtrlPanelWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigKeyUp(); break;
        case 1: _t->sigKeyDown(); break;
        case 2: _t->sigKeyLeft(); break;
        case 3: _t->sigKeyRight(); break;
        case 4: _t->sigKeyHome(); break;
        case 5: _t->sigKeyZoomInH(); break;
        case 6: _t->sigKeyZoomOutH(); break;
        case 7: _t->sigKeyZoomInV(); break;
        case 8: _t->sigKeyZoomOutV(); break;
        case 9: _t->handleButtonPressed((*reinterpret_cast< CONTROL_CMD(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CtrlPanelWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CtrlPanelWindow::sigKeyUp)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CtrlPanelWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CtrlPanelWindow::sigKeyDown)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CtrlPanelWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CtrlPanelWindow::sigKeyLeft)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (CtrlPanelWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CtrlPanelWindow::sigKeyRight)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (CtrlPanelWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CtrlPanelWindow::sigKeyHome)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (CtrlPanelWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CtrlPanelWindow::sigKeyZoomInH)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (CtrlPanelWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CtrlPanelWindow::sigKeyZoomOutH)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (CtrlPanelWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CtrlPanelWindow::sigKeyZoomInV)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (CtrlPanelWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CtrlPanelWindow::sigKeyZoomOutV)) {
                *result = 8;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CtrlPanelWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CtrlPanelWindow.data,
    qt_meta_data_CtrlPanelWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CtrlPanelWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CtrlPanelWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CtrlPanelWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int CtrlPanelWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void CtrlPanelWindow::sigKeyUp()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void CtrlPanelWindow::sigKeyDown()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void CtrlPanelWindow::sigKeyLeft()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void CtrlPanelWindow::sigKeyRight()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void CtrlPanelWindow::sigKeyHome()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void CtrlPanelWindow::sigKeyZoomInH()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void CtrlPanelWindow::sigKeyZoomOutH()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void CtrlPanelWindow::sigKeyZoomInV()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void CtrlPanelWindow::sigKeyZoomOutV()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
