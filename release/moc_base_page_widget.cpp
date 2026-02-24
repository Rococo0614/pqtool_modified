/****************************************************************************
** Meta object code from reading C++ file 'base_page_widget.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/pages/base_page_widget.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'base_page_widget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BasePageWidget_t {
    QByteArrayData data[25];
    char stringdata0[314];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BasePageWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BasePageWidget_t qt_meta_stringdata_BasePageWidget = {
    {
QT_MOC_LITERAL(0, 0, 14), // "BasePageWidget"
QT_MOC_LITERAL(1, 15, 11), // "sigWriteAll"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 10), // "sigReadAll"
QT_MOC_LITERAL(4, 39, 13), // "sigEnvirValue"
QT_MOC_LITERAL(5, 53, 13), // "checked_state"
QT_MOC_LITERAL(6, 67, 12), // "sigStatusLog"
QT_MOC_LITERAL(7, 80, 7), // "message"
QT_MOC_LITERAL(8, 88, 9), // "LOG_LEVEL"
QT_MOC_LITERAL(9, 98, 9), // "log_level"
QT_MOC_LITERAL(10, 108, 8), // "readData"
QT_MOC_LITERAL(11, 117, 9), // "writeData"
QT_MOC_LITERAL(12, 127, 22), // "handleAutoWriteChanged"
QT_MOC_LITERAL(13, 150, 19), // "handleEvShowChanged"
QT_MOC_LITERAL(14, 170, 12), // "pageWriteAll"
QT_MOC_LITERAL(15, 183, 11), // "pageReadAll"
QT_MOC_LITERAL(16, 195, 14), // "receiveLdcSize"
QT_MOC_LITERAL(17, 210, 2), // "id"
QT_MOC_LITERAL(18, 213, 14), // "sendLdcBinData"
QT_MOC_LITERAL(19, 228, 15), // "paegUpdateTheme"
QT_MOC_LITERAL(20, 244, 18), // "dark_theme_enabled"
QT_MOC_LITERAL(21, 263, 19), // "handleOpTypeChanged"
QT_MOC_LITERAL(22, 283, 5), // "value"
QT_MOC_LITERAL(23, 289, 15), // "handleCopyApply"
QT_MOC_LITERAL(24, 305, 8) // "doRecord"

    },
    "BasePageWidget\0sigWriteAll\0\0sigReadAll\0"
    "sigEnvirValue\0checked_state\0sigStatusLog\0"
    "message\0LOG_LEVEL\0log_level\0readData\0"
    "writeData\0handleAutoWriteChanged\0"
    "handleEvShowChanged\0pageWriteAll\0"
    "pageReadAll\0receiveLdcSize\0id\0"
    "sendLdcBinData\0paegUpdateTheme\0"
    "dark_theme_enabled\0handleOpTypeChanged\0"
    "value\0handleCopyApply\0doRecord"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BasePageWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   99,    2, 0x06 /* Public */,
       3,    0,  100,    2, 0x06 /* Public */,
       4,    1,  101,    2, 0x06 /* Public */,
       6,    2,  104,    2, 0x06 /* Public */,
       6,    1,  109,    2, 0x26 /* Public | MethodCloned */,

 // slots: name, argc, parameters, tag, flags
      10,    0,  112,    2, 0x0a /* Public */,
      11,    0,  113,    2, 0x0a /* Public */,
      12,    1,  114,    2, 0x0a /* Public */,
      13,    1,  117,    2, 0x0a /* Public */,
      14,    0,  120,    2, 0x0a /* Public */,
      15,    0,  121,    2, 0x0a /* Public */,
      16,    1,  122,    2, 0x0a /* Public */,
      18,    1,  125,    2, 0x0a /* Public */,
      19,    1,  128,    2, 0x0a /* Public */,
      21,    1,  131,    2, 0x0a /* Public */,
      23,    0,  134,    2, 0x0a /* Public */,
      24,    0,  135,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 8,    7,    9,
    QMetaType::Void, QMetaType::QString,    7,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Bool,   20,
    QMetaType::Void, QMetaType::QVariant,   22,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void BasePageWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<BasePageWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigWriteAll(); break;
        case 1: _t->sigReadAll(); break;
        case 2: _t->sigEnvirValue((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->sigStatusLog((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< LOG_LEVEL(*)>(_a[2]))); break;
        case 4: _t->sigStatusLog((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->readData(); break;
        case 6: _t->writeData(); break;
        case 7: _t->handleAutoWriteChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->handleEvShowChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->pageWriteAll(); break;
        case 10: _t->pageReadAll(); break;
        case 11: _t->receiveLdcSize((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->sendLdcBinData((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->paegUpdateTheme((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->handleOpTypeChanged((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 15: _t->handleCopyApply(); break;
        case 16: _t->doRecord(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (BasePageWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BasePageWidget::sigWriteAll)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (BasePageWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BasePageWidget::sigReadAll)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (BasePageWidget::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BasePageWidget::sigEnvirValue)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (BasePageWidget::*)(QString , LOG_LEVEL );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BasePageWidget::sigStatusLog)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject BasePageWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_BasePageWidget.data,
    qt_meta_data_BasePageWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *BasePageWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BasePageWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BasePageWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int BasePageWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void BasePageWidget::sigWriteAll()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void BasePageWidget::sigReadAll()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void BasePageWidget::sigEnvirValue(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void BasePageWidget::sigStatusLog(QString _t1, LOG_LEVEL _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
