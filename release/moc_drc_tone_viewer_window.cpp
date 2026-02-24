/****************************************************************************
** Meta object code from reading C++ file 'drc_tone_viewer_window.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/component/window/drc_tone_viewer_window.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'drc_tone_viewer_window.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DrcToneViewerWindow_t {
    QByteArrayData data[7];
    char stringdata0[102];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DrcToneViewerWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DrcToneViewerWindow_t qt_meta_stringdata_DrcToneViewerWindow = {
    {
QT_MOC_LITERAL(0, 0, 19), // "DrcToneViewerWindow"
QT_MOC_LITERAL(1, 20, 30), // "slotSendGetDrcHistogramInfoCmd"
QT_MOC_LITERAL(2, 51, 0), // ""
QT_MOC_LITERAL(3, 52, 19), // "slotUpdateAllCharts"
QT_MOC_LITERAL(4, 72, 2), // "id"
QT_MOC_LITERAL(5, 75, 21), // "slotHandleSelectColor"
QT_MOC_LITERAL(6, 97, 4) // "text"

    },
    "DrcToneViewerWindow\0slotSendGetDrcHistogramInfoCmd\0"
    "\0slotUpdateAllCharts\0id\0slotHandleSelectColor\0"
    "text"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DrcToneViewerWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x08 /* Private */,
       3,    1,   30,    2, 0x08 /* Private */,
       5,    1,   33,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::QString,    6,

       0        // eod
};

void DrcToneViewerWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DrcToneViewerWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotSendGetDrcHistogramInfoCmd(); break;
        case 1: _t->slotUpdateAllCharts((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->slotHandleSelectColor((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DrcToneViewerWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_DrcToneViewerWindow.data,
    qt_meta_data_DrcToneViewerWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DrcToneViewerWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DrcToneViewerWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DrcToneViewerWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int DrcToneViewerWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
