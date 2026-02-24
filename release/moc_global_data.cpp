/****************************************************************************
** Meta object code from reading C++ file 'global_data.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../model/include/global_data.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'global_data.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GlobalData_t {
    QByteArrayData data[24];
    char stringdata0[331];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GlobalData_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GlobalData_t qt_meta_stringdata_GlobalData = {
    {
QT_MOC_LITERAL(0, 0, 10), // "GlobalData"
QT_MOC_LITERAL(1, 11, 13), // "sigDataLoaded"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 15), // "sigDataModified"
QT_MOC_LITERAL(4, 42, 10), // "SecKeyType"
QT_MOC_LITERAL(5, 53, 15), // "reg_section_key"
QT_MOC_LITERAL(6, 69, 12), // "sigStatusLog"
QT_MOC_LITERAL(7, 82, 7), // "message"
QT_MOC_LITERAL(8, 90, 9), // "LOG_LEVEL"
QT_MOC_LITERAL(9, 100, 9), // "log_level"
QT_MOC_LITERAL(10, 110, 21), // "sigCheckGainSepStatus"
QT_MOC_LITERAL(11, 132, 20), // "sigGetWDRFusionFrame"
QT_MOC_LITERAL(12, 153, 17), // "sigUpdateTemplate"
QT_MOC_LITERAL(13, 171, 14), // "file_full_name"
QT_MOC_LITERAL(14, 186, 17), // "sigModuleRWResult"
QT_MOC_LITERAL(15, 204, 6), // "module"
QT_MOC_LITERAL(16, 211, 3), // "cmd"
QT_MOC_LITERAL(17, 215, 6), // "result"
QT_MOC_LITERAL(18, 222, 15), // "sigReceiveGamma"
QT_MOC_LITERAL(19, 238, 17), // "handleDataChanged"
QT_MOC_LITERAL(20, 256, 20), // "handleReceiveWDRMode"
QT_MOC_LITERAL(21, 277, 2), // "id"
QT_MOC_LITERAL(22, 280, 24), // "handleDataChangedJsonrpc"
QT_MOC_LITERAL(23, 305, 25) // "handleReceiveGammaJsonrpc"

    },
    "GlobalData\0sigDataLoaded\0\0sigDataModified\0"
    "SecKeyType\0reg_section_key\0sigStatusLog\0"
    "message\0LOG_LEVEL\0log_level\0"
    "sigCheckGainSepStatus\0sigGetWDRFusionFrame\0"
    "sigUpdateTemplate\0file_full_name\0"
    "sigModuleRWResult\0module\0cmd\0result\0"
    "sigReceiveGamma\0handleDataChanged\0"
    "handleReceiveWDRMode\0id\0"
    "handleDataChangedJsonrpc\0"
    "handleReceiveGammaJsonrpc"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GlobalData[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x06 /* Public */,
       3,    1,   75,    2, 0x06 /* Public */,
       6,    2,   78,    2, 0x06 /* Public */,
      10,    0,   83,    2, 0x06 /* Public */,
      11,    0,   84,    2, 0x06 /* Public */,
      12,    1,   85,    2, 0x06 /* Public */,
      14,    3,   88,    2, 0x06 /* Public */,
      18,    0,   95,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      19,    1,   96,    2, 0x08 /* Private */,
      20,    1,   99,    2, 0x08 /* Private */,
      22,    1,  102,    2, 0x08 /* Private */,
      23,    1,  105,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 8,    7,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   15,   16,   17,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void, QMetaType::Int,   21,

       0        // eod
};

void GlobalData::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GlobalData *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigDataLoaded(); break;
        case 1: _t->sigDataModified((*reinterpret_cast< SecKeyType(*)>(_a[1]))); break;
        case 2: _t->sigStatusLog((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< LOG_LEVEL(*)>(_a[2]))); break;
        case 3: _t->sigCheckGainSepStatus(); break;
        case 4: _t->sigGetWDRFusionFrame(); break;
        case 5: _t->sigUpdateTemplate((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->sigModuleRWResult((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 7: _t->sigReceiveGamma(); break;
        case 8: _t->handleDataChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->handleReceiveWDRMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->handleDataChangedJsonrpc((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->handleReceiveGammaJsonrpc((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GlobalData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalData::sigDataLoaded)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (GlobalData::*)(SecKeyType );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalData::sigDataModified)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (GlobalData::*)(QString , LOG_LEVEL );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalData::sigStatusLog)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (GlobalData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalData::sigCheckGainSepStatus)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (GlobalData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalData::sigGetWDRFusionFrame)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (GlobalData::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalData::sigUpdateTemplate)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (GlobalData::*)(QString , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalData::sigModuleRWResult)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (GlobalData::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GlobalData::sigReceiveGamma)) {
                *result = 7;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject GlobalData::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_GlobalData.data,
    qt_meta_data_GlobalData,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GlobalData::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GlobalData::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GlobalData.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int GlobalData::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void GlobalData::sigDataLoaded()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void GlobalData::sigDataModified(SecKeyType _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void GlobalData::sigStatusLog(QString _t1, LOG_LEVEL _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void GlobalData::sigCheckGainSepStatus()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void GlobalData::sigGetWDRFusionFrame()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void GlobalData::sigUpdateTemplate(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void GlobalData::sigModuleRWResult(QString _t1, QString _t2, QString _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void GlobalData::sigReceiveGamma()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
