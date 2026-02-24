/****************************************************************************
** Meta object code from reading C++ file 'focus_assistant_window.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/component/window/focus_assistant_window.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'focus_assistant_window.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FocusAssistantWindow_t {
    QByteArrayData data[17];
    char stringdata0[262];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FocusAssistantWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FocusAssistantWindow_t qt_meta_stringdata_FocusAssistantWindow = {
    {
QT_MOC_LITERAL(0, 0, 20), // "FocusAssistantWindow"
QT_MOC_LITERAL(1, 21, 23), // "slotSendCaptureImageCmd"
QT_MOC_LITERAL(2, 45, 0), // ""
QT_MOC_LITERAL(3, 46, 15), // "slotUpdateImage"
QT_MOC_LITERAL(4, 62, 3), // "cmd"
QT_MOC_LITERAL(5, 66, 19), // "slotSendGrabDataCmd"
QT_MOC_LITERAL(6, 86, 14), // "slotUpdateData"
QT_MOC_LITERAL(7, 101, 2), // "id"
QT_MOC_LITERAL(8, 104, 17), // "slotSetShowFvText"
QT_MOC_LITERAL(9, 122, 21), // "slotSetCurrentFvTable"
QT_MOC_LITERAL(10, 144, 5), // "index"
QT_MOC_LITERAL(11, 150, 18), // "slotSelectAllZones"
QT_MOC_LITERAL(12, 169, 20), // "slotDeselectAllZones"
QT_MOC_LITERAL(13, 190, 16), // "slotResetFvChart"
QT_MOC_LITERAL(14, 207, 17), // "handleImageStatus"
QT_MOC_LITERAL(15, 225, 17), // "handleUpdateTheme"
QT_MOC_LITERAL(16, 243, 18) // "dark_theme_enabled"

    },
    "FocusAssistantWindow\0slotSendCaptureImageCmd\0"
    "\0slotUpdateImage\0cmd\0slotSendGrabDataCmd\0"
    "slotUpdateData\0id\0slotSetShowFvText\0"
    "slotSetCurrentFvTable\0index\0"
    "slotSelectAllZones\0slotDeselectAllZones\0"
    "slotResetFvChart\0handleImageStatus\0"
    "handleUpdateTheme\0dark_theme_enabled"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FocusAssistantWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x08 /* Private */,
       3,    1,   70,    2, 0x08 /* Private */,
       5,    0,   73,    2, 0x08 /* Private */,
       6,    1,   74,    2, 0x08 /* Private */,
       8,    0,   77,    2, 0x08 /* Private */,
       9,    1,   78,    2, 0x08 /* Private */,
      11,    0,   81,    2, 0x08 /* Private */,
      12,    0,   82,    2, 0x08 /* Private */,
      13,    0,   83,    2, 0x08 /* Private */,
      14,    1,   84,    2, 0x08 /* Private */,
      15,    1,   87,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Bool,   16,

       0        // eod
};

void FocusAssistantWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FocusAssistantWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotSendCaptureImageCmd(); break;
        case 1: _t->slotUpdateImage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->slotSendGrabDataCmd(); break;
        case 3: _t->slotUpdateData((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->slotSetShowFvText(); break;
        case 5: _t->slotSetCurrentFvTable((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->slotSelectAllZones(); break;
        case 7: _t->slotDeselectAllZones(); break;
        case 8: _t->slotResetFvChart(); break;
        case 9: _t->handleImageStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->handleUpdateTheme((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FocusAssistantWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_FocusAssistantWindow.data,
    qt_meta_data_FocusAssistantWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *FocusAssistantWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FocusAssistantWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FocusAssistantWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int FocusAssistantWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
