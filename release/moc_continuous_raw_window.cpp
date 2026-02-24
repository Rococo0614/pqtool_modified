/****************************************************************************
** Meta object code from reading C++ file 'continuous_raw_window.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/component/window/continuous_raw_window.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'continuous_raw_window.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ContinuousRawWindow_t {
    QByteArrayData data[18];
    char stringdata0[236];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ContinuousRawWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ContinuousRawWindow_t qt_meta_stringdata_ContinuousRawWindow = {
    {
QT_MOC_LITERAL(0, 0, 19), // "ContinuousRawWindow"
QT_MOC_LITERAL(1, 20, 12), // "sigGetYuvEnd"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 16), // "OpenSaveFilePath"
QT_MOC_LITERAL(4, 51, 12), // "GetSingleYuv"
QT_MOC_LITERAL(5, 64, 16), // "ReceiveSingleYuv"
QT_MOC_LITERAL(6, 81, 3), // "cmd"
QT_MOC_LITERAL(7, 85, 18), // "CancelGetSingleYuv"
QT_MOC_LITERAL(8, 104, 10), // "DumpStatus"
QT_MOC_LITERAL(9, 115, 6), // "status"
QT_MOC_LITERAL(10, 122, 13), // "HandelCapture"
QT_MOC_LITERAL(11, 136, 9), // "StartDump"
QT_MOC_LITERAL(12, 146, 7), // "RawDump"
QT_MOC_LITERAL(13, 154, 10), // "CancelDump"
QT_MOC_LITERAL(14, 165, 23), // "handleSingleImageStatus"
QT_MOC_LITERAL(15, 189, 2), // "id"
QT_MOC_LITERAL(16, 192, 19), // "GetRawDumpMaxFrames"
QT_MOC_LITERAL(17, 212, 23) // "ReceiveRawDumpMaxFrames"

    },
    "ContinuousRawWindow\0sigGetYuvEnd\0\0"
    "OpenSaveFilePath\0GetSingleYuv\0"
    "ReceiveSingleYuv\0cmd\0CancelGetSingleYuv\0"
    "DumpStatus\0status\0HandelCapture\0"
    "StartDump\0RawDump\0CancelDump\0"
    "handleSingleImageStatus\0id\0"
    "GetRawDumpMaxFrames\0ReceiveRawDumpMaxFrames"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ContinuousRawWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   80,    2, 0x08 /* Private */,
       4,    0,   81,    2, 0x08 /* Private */,
       5,    1,   82,    2, 0x08 /* Private */,
       7,    0,   85,    2, 0x08 /* Private */,
       8,    1,   86,    2, 0x08 /* Private */,
      10,    0,   89,    2, 0x08 /* Private */,
      11,    0,   90,    2, 0x08 /* Private */,
      12,    0,   91,    2, 0x08 /* Private */,
      13,    0,   92,    2, 0x08 /* Private */,
      14,    1,   93,    2, 0x08 /* Private */,
      16,    0,   96,    2, 0x08 /* Private */,
      17,    1,   97,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   15,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   15,

       0        // eod
};

void ContinuousRawWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ContinuousRawWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigGetYuvEnd(); break;
        case 1: _t->OpenSaveFilePath(); break;
        case 2: _t->GetSingleYuv(); break;
        case 3: _t->ReceiveSingleYuv((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->CancelGetSingleYuv(); break;
        case 5: _t->DumpStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->HandelCapture(); break;
        case 7: _t->StartDump(); break;
        case 8: _t->RawDump(); break;
        case 9: _t->CancelDump(); break;
        case 10: _t->handleSingleImageStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->GetRawDumpMaxFrames(); break;
        case 12: _t->ReceiveRawDumpMaxFrames((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ContinuousRawWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ContinuousRawWindow::sigGetYuvEnd)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ContinuousRawWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_ContinuousRawWindow.data,
    qt_meta_data_ContinuousRawWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ContinuousRawWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ContinuousRawWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ContinuousRawWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int ContinuousRawWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void ContinuousRawWindow::sigGetYuvEnd()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
