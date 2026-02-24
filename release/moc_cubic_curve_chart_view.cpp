/****************************************************************************
** Meta object code from reading C++ file 'cubic_curve_chart_view.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/component/chart/cubic_curve_chart_view.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cubic_curve_chart_view.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CubicCurveChartView_t {
    QByteArrayData data[12];
    char stringdata0[141];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CubicCurveChartView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CubicCurveChartView_t qt_meta_stringdata_CubicCurveChartView = {
    {
QT_MOC_LITERAL(0, 0, 19), // "CubicCurveChartView"
QT_MOC_LITERAL(1, 20, 20), // "sigFocusPointUpdated"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 1), // "x"
QT_MOC_LITERAL(4, 44, 1), // "y"
QT_MOC_LITERAL(5, 46, 12), // "sigMouseMove"
QT_MOC_LITERAL(6, 59, 19), // "sigPressedScatterPt"
QT_MOC_LITERAL(7, 79, 1), // "p"
QT_MOC_LITERAL(8, 81, 21), // "sigUpdateWdrHistogram"
QT_MOC_LITERAL(9, 103, 4), // "data"
QT_MOC_LITERAL(10, 108, 15), // "sigValueChanged"
QT_MOC_LITERAL(11, 124, 16) // "sigPageWriteData"

    },
    "CubicCurveChartView\0sigFocusPointUpdated\0"
    "\0x\0y\0sigMouseMove\0sigPressedScatterPt\0"
    "p\0sigUpdateWdrHistogram\0data\0"
    "sigValueChanged\0sigPageWriteData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CubicCurveChartView[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   44,    2, 0x06 /* Public */,
       5,    2,   49,    2, 0x06 /* Public */,
       6,    1,   54,    2, 0x06 /* Public */,
       8,    1,   57,    2, 0x06 /* Public */,
      10,    1,   60,    2, 0x06 /* Public */,
      11,    0,   63,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QReal, QMetaType::QReal,    3,    4,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    3,    4,
    QMetaType::Void, QMetaType::QPointF,    7,
    QMetaType::Void, QMetaType::QVariant,    9,
    QMetaType::Void, QMetaType::QVariant,    2,
    QMetaType::Void,

       0        // eod
};

void CubicCurveChartView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CubicCurveChartView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigFocusPointUpdated((*reinterpret_cast< qreal(*)>(_a[1])),(*reinterpret_cast< qreal(*)>(_a[2]))); break;
        case 1: _t->sigMouseMove((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 2: _t->sigPressedScatterPt((*reinterpret_cast< QPointF(*)>(_a[1]))); break;
        case 3: _t->sigUpdateWdrHistogram((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 4: _t->sigValueChanged((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 5: _t->sigPageWriteData(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CubicCurveChartView::*)(qreal , qreal );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CubicCurveChartView::sigFocusPointUpdated)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CubicCurveChartView::*)(double , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CubicCurveChartView::sigMouseMove)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CubicCurveChartView::*)(QPointF );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CubicCurveChartView::sigPressedScatterPt)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (CubicCurveChartView::*)(QVariant );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CubicCurveChartView::sigUpdateWdrHistogram)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (CubicCurveChartView::*)(QVariant );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CubicCurveChartView::sigValueChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (CubicCurveChartView::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CubicCurveChartView::sigPageWriteData)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CubicCurveChartView::staticMetaObject = { {
    QMetaObject::SuperData::link<QtCharts::QChartView::staticMetaObject>(),
    qt_meta_stringdata_CubicCurveChartView.data,
    qt_meta_data_CubicCurveChartView,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CubicCurveChartView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CubicCurveChartView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CubicCurveChartView.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "IObserver"))
        return static_cast< IObserver*>(this);
    return QtCharts::QChartView::qt_metacast(_clname);
}

int CubicCurveChartView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QtCharts::QChartView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void CubicCurveChartView::sigFocusPointUpdated(qreal _t1, qreal _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CubicCurveChartView::sigMouseMove(double _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CubicCurveChartView::sigPressedScatterPt(QPointF _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void CubicCurveChartView::sigUpdateWdrHistogram(QVariant _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void CubicCurveChartView::sigValueChanged(QVariant _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void CubicCurveChartView::sigPageWriteData()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
