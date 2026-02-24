/****************************************************************************
** Meta object code from reading C++ file 'raw_dump.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../model/include/raw_dump.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'raw_dump.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RawDump_t {
    QByteArrayData data[15];
    char stringdata0[198];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RawDump_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RawDump_t qt_meta_stringdata_RawDump = {
    {
QT_MOC_LITERAL(0, 0, 7), // "RawDump"
QT_MOC_LITERAL(1, 8, 10), // "DumpStatus"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 6), // "status"
QT_MOC_LITERAL(4, 27, 14), // "ConnectSignals"
QT_MOC_LITERAL(5, 42, 2), // "ip"
QT_MOC_LITERAL(6, 45, 19), // "StartRawDumpSignals"
QT_MOC_LITERAL(7, 65, 4), // "data"
QT_MOC_LITERAL(8, 70, 17), // "DisconnectSignals"
QT_MOC_LITERAL(9, 88, 12), // "ConnectSlots"
QT_MOC_LITERAL(10, 101, 17), // "StartRawDumpSlots"
QT_MOC_LITERAL(11, 119, 15), // "DisconnectSlots"
QT_MOC_LITERAL(12, 135, 26), // "handleConnectStatteChanged"
QT_MOC_LITERAL(13, 162, 28), // "QAbstractSocket::SocketState"
QT_MOC_LITERAL(14, 191, 6) // "stsate"

    },
    "RawDump\0DumpStatus\0\0status\0ConnectSignals\0"
    "ip\0StartRawDumpSignals\0data\0"
    "DisconnectSignals\0ConnectSlots\0"
    "StartRawDumpSlots\0DisconnectSlots\0"
    "handleConnectStatteChanged\0"
    "QAbstractSocket::SocketState\0stsate"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RawDump[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       4,    1,   57,    2, 0x06 /* Public */,
       6,    1,   60,    2, 0x06 /* Public */,
       8,    0,   63,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    1,   64,    2, 0x08 /* Private */,
      10,    1,   67,    2, 0x08 /* Private */,
      11,    0,   70,    2, 0x08 /* Private */,
      12,    1,   71,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QVariant,    7,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QVariant,    7,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,   14,

       0        // eod
};

void RawDump::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RawDump *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->DumpStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->ConnectSignals((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->StartRawDumpSignals((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 3: _t->DisconnectSignals(); break;
        case 4: _t->ConnectSlots((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->StartRawDumpSlots((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 6: _t->DisconnectSlots(); break;
        case 7: _t->handleConnectStatteChanged((*reinterpret_cast< QAbstractSocket::SocketState(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketState >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (RawDump::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RawDump::DumpStatus)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (RawDump::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RawDump::ConnectSignals)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (RawDump::*)(QVariant );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RawDump::StartRawDumpSignals)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (RawDump::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RawDump::DisconnectSignals)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject RawDump::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_RawDump.data,
    qt_meta_data_RawDump,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RawDump::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RawDump::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RawDump.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int RawDump::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void RawDump::DumpStatus(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RawDump::ConnectSignals(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void RawDump::StartRawDumpSignals(QVariant _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void RawDump::DisconnectSignals()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
