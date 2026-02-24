/****************************************************************************
** Meta object code from reading C++ file 'table_curve.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/component/register/table_curve.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'table_curve.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SimpleTableCurve_t {
    QByteArrayData data[21];
    char stringdata0[292];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SimpleTableCurve_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SimpleTableCurve_t qt_meta_stringdata_SimpleTableCurve = {
    {
QT_MOC_LITERAL(0, 0, 16), // "SimpleTableCurve"
QT_MOC_LITERAL(1, 17, 15), // "sigValueChanged"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 16), // "sigWritePageData"
QT_MOC_LITERAL(4, 51, 15), // "slotSwitchChart"
QT_MOC_LITERAL(5, 67, 19), // "slotUpdateLineCurve"
QT_MOC_LITERAL(6, 87, 10), // "line_slope"
QT_MOC_LITERAL(7, 98, 14), // "line_intercept"
QT_MOC_LITERAL(8, 113, 23), // "slotUpdateGaussianCurve"
QT_MOC_LITERAL(9, 137, 13), // "gaussian_mean"
QT_MOC_LITERAL(10, 151, 12), // "gaussian_std"
QT_MOC_LITERAL(11, 164, 13), // "gaussian_gain"
QT_MOC_LITERAL(12, 178, 20), // "slotUpdateGammaCurve"
QT_MOC_LITERAL(13, 199, 5), // "gamma"
QT_MOC_LITERAL(14, 205, 16), // "slotRefreshChart"
QT_MOC_LITERAL(15, 222, 15), // "slotChartChange"
QT_MOC_LITERAL(16, 238, 14), // "updateMousePos"
QT_MOC_LITERAL(17, 253, 1), // "x"
QT_MOC_LITERAL(18, 255, 1), // "y"
QT_MOC_LITERAL(19, 257, 16), // "slotTableClicked"
QT_MOC_LITERAL(20, 274, 17) // "slotPointSelected"

    },
    "SimpleTableCurve\0sigValueChanged\0\0"
    "sigWritePageData\0slotSwitchChart\0"
    "slotUpdateLineCurve\0line_slope\0"
    "line_intercept\0slotUpdateGaussianCurve\0"
    "gaussian_mean\0gaussian_std\0gaussian_gain\0"
    "slotUpdateGammaCurve\0gamma\0slotRefreshChart\0"
    "slotChartChange\0updateMousePos\0x\0y\0"
    "slotTableClicked\0slotPointSelected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SimpleTableCurve[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x06 /* Public */,
       3,    0,   70,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   71,    2, 0x0a /* Public */,
       5,    2,   72,    2, 0x0a /* Public */,
       8,    3,   77,    2, 0x0a /* Public */,
      12,    1,   84,    2, 0x0a /* Public */,
      14,    0,   87,    2, 0x09 /* Protected */,
      15,    0,   88,    2, 0x09 /* Protected */,
      16,    2,   89,    2, 0x09 /* Protected */,
      19,    0,   94,    2, 0x09 /* Protected */,
      20,    1,   95,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    6,    7,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Double,    9,   10,   11,
    QMetaType::Void, QMetaType::Double,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,   17,   18,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void SimpleTableCurve::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SimpleTableCurve *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigValueChanged(); break;
        case 1: _t->sigWritePageData(); break;
        case 2: _t->slotSwitchChart(); break;
        case 3: _t->slotUpdateLineCurve((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 4: _t->slotUpdateGaussianCurve((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 5: _t->slotUpdateGammaCurve((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: _t->slotRefreshChart(); break;
        case 7: _t->slotChartChange(); break;
        case 8: _t->updateMousePos((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 9: _t->slotTableClicked(); break;
        case 10: _t->slotPointSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SimpleTableCurve::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SimpleTableCurve::sigValueChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SimpleTableCurve::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SimpleTableCurve::sigWritePageData)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SimpleTableCurve::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_SimpleTableCurve.data,
    qt_meta_data_SimpleTableCurve,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SimpleTableCurve::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SimpleTableCurve::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SimpleTableCurve.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SimpleTableCurve::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void SimpleTableCurve::sigValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SimpleTableCurve::sigWritePageData()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
struct qt_meta_stringdata_MethodTableCurve_t {
    QByteArrayData data[5];
    char stringdata0[67];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MethodTableCurve_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MethodTableCurve_t qt_meta_stringdata_MethodTableCurve = {
    {
QT_MOC_LITERAL(0, 0, 16), // "MethodTableCurve"
QT_MOC_LITERAL(1, 17, 15), // "slotSwitchChart"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 10), // "resetCurve"
QT_MOC_LITERAL(4, 45, 21) // "slotCurveParamChanged"

    },
    "MethodTableCurve\0slotSwitchChart\0\0"
    "resetCurve\0slotCurveParamChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MethodTableCurve[] = {

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
       1,    0,   29,    2, 0x0a /* Public */,
       3,    0,   30,    2, 0x08 /* Private */,
       4,    1,   31,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QVariant,    2,

       0        // eod
};

void MethodTableCurve::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MethodTableCurve *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotSwitchChart(); break;
        case 1: _t->resetCurve(); break;
        case 2: _t->slotCurveParamChanged((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MethodTableCurve::staticMetaObject = { {
    QMetaObject::SuperData::link<SimpleTableCurve::staticMetaObject>(),
    qt_meta_stringdata_MethodTableCurve.data,
    qt_meta_data_MethodTableCurve,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MethodTableCurve::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MethodTableCurve::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MethodTableCurve.stringdata0))
        return static_cast<void*>(this);
    return SimpleTableCurve::qt_metacast(_clname);
}

int MethodTableCurve::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SimpleTableCurve::qt_metacall(_c, _id, _a);
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
