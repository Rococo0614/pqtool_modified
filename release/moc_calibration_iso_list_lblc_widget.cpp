/****************************************************************************
** Meta object code from reading C++ file 'calibration_iso_list_lblc_widget.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/component/list/calibration_iso_list_lblc_widget.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'calibration_iso_list_lblc_widget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ISOItem_t {
    QByteArrayData data[5];
    char stringdata0[52];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ISOItem_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ISOItem_t qt_meta_stringdata_ISOItem = {
    {
QT_MOC_LITERAL(0, 0, 7), // "ISOItem"
QT_MOC_LITERAL(1, 8, 19), // "displayStateChanged"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 14), // "Qt::CheckState"
QT_MOC_LITERAL(4, 44, 7) // "display"

    },
    "ISOItem\0displayStateChanged\0\0"
    "Qt::CheckState\0display"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ISOItem[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void ISOItem::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ISOItem *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->displayStateChanged((*reinterpret_cast< Qt::CheckState(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ISOItem::*)(Qt::CheckState );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ISOItem::displayStateChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ISOItem::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ISOItem.data,
    qt_meta_data_ISOItem,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ISOItem::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ISOItem::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ISOItem.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ISOItem::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void ISOItem::displayStateChanged(Qt::CheckState _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_CalibrationISOLBlcListWidget_t {
    QByteArrayData data[6];
    char stringdata0[83];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CalibrationISOLBlcListWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CalibrationISOLBlcListWidget_t qt_meta_stringdata_CalibrationISOLBlcListWidget = {
    {
QT_MOC_LITERAL(0, 0, 28), // "CalibrationISOLBlcListWidget"
QT_MOC_LITERAL(1, 29, 17), // "sigSetInputPixmap"
QT_MOC_LITERAL(2, 47, 0), // ""
QT_MOC_LITERAL(3, 48, 8), // "QPixmap*"
QT_MOC_LITERAL(4, 57, 6), // "pixmap"
QT_MOC_LITERAL(5, 64, 18) // "sigSetOutputPixmap"

    },
    "CalibrationISOLBlcListWidget\0"
    "sigSetInputPixmap\0\0QPixmap*\0pixmap\0"
    "sigSetOutputPixmap"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CalibrationISOLBlcListWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,
       5,    1,   27,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void CalibrationISOLBlcListWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CalibrationISOLBlcListWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigSetInputPixmap((*reinterpret_cast< QPixmap*(*)>(_a[1]))); break;
        case 1: _t->sigSetOutputPixmap((*reinterpret_cast< QPixmap*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CalibrationISOLBlcListWidget::*)(QPixmap * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalibrationISOLBlcListWidget::sigSetInputPixmap)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CalibrationISOLBlcListWidget::*)(QPixmap * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CalibrationISOLBlcListWidget::sigSetOutputPixmap)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CalibrationISOLBlcListWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CalibrationISOLBlcListWidget.data,
    qt_meta_data_CalibrationISOLBlcListWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CalibrationISOLBlcListWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CalibrationISOLBlcListWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CalibrationISOLBlcListWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int CalibrationISOLBlcListWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void CalibrationISOLBlcListWidget::sigSetInputPixmap(QPixmap * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CalibrationISOLBlcListWidget::sigSetOutputPixmap(QPixmap * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
