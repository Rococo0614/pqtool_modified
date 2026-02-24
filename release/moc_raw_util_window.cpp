/****************************************************************************
** Meta object code from reading C++ file 'raw_util_window.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/component/window/raw_util_window.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'raw_util_window.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RawUtilWindow_t {
    QByteArrayData data[15];
    char stringdata0[183];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RawUtilWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RawUtilWindow_t qt_meta_stringdata_RawUtilWindow = {
    {
QT_MOC_LITERAL(0, 0, 13), // "RawUtilWindow"
QT_MOC_LITERAL(1, 14, 17), // "handleOpenImgFile"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 8), // "slotSend"
QT_MOC_LITERAL(4, 42, 9), // "slotFlush"
QT_MOC_LITERAL(5, 52, 10), // "slotCancel"
QT_MOC_LITERAL(6, 63, 9), // "slotBatch"
QT_MOC_LITERAL(7, 73, 20), // "handleReceiveMessage"
QT_MOC_LITERAL(8, 94, 2), // "id"
QT_MOC_LITERAL(9, 97, 11), // "batch_conti"
QT_MOC_LITERAL(10, 109, 13), // "batch_dumpAll"
QT_MOC_LITERAL(11, 123, 20), // "batch_updateProgress"
QT_MOC_LITERAL(12, 144, 20), // "handleMultiYuvStatus"
QT_MOC_LITERAL(13, 165, 13), // "slotGotYuvImg"
QT_MOC_LITERAL(14, 179, 3) // "cmd"

    },
    "RawUtilWindow\0handleOpenImgFile\0\0"
    "slotSend\0slotFlush\0slotCancel\0slotBatch\0"
    "handleReceiveMessage\0id\0batch_conti\0"
    "batch_dumpAll\0batch_updateProgress\0"
    "handleMultiYuvStatus\0slotGotYuvImg\0"
    "cmd"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RawUtilWindow[] = {

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
       3,    0,   70,    2, 0x08 /* Private */,
       4,    0,   71,    2, 0x08 /* Private */,
       5,    0,   72,    2, 0x08 /* Private */,
       6,    0,   73,    2, 0x08 /* Private */,
       7,    1,   74,    2, 0x08 /* Private */,
       9,    0,   77,    2, 0x08 /* Private */,
      10,    0,   78,    2, 0x08 /* Private */,
      11,    0,   79,    2, 0x08 /* Private */,
      12,    1,   80,    2, 0x08 /* Private */,
      13,    1,   83,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::QString,   14,

       0        // eod
};

void RawUtilWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RawUtilWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->handleOpenImgFile(); break;
        case 1: _t->slotSend(); break;
        case 2: _t->slotFlush(); break;
        case 3: _t->slotCancel(); break;
        case 4: _t->slotBatch(); break;
        case 5: _t->handleReceiveMessage((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->batch_conti(); break;
        case 7: _t->batch_dumpAll(); break;
        case 8: _t->batch_updateProgress(); break;
        case 9: _t->handleMultiYuvStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->slotGotYuvImg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject RawUtilWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_RawUtilWindow.data,
    qt_meta_data_RawUtilWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RawUtilWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RawUtilWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RawUtilWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int RawUtilWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
