/****************************************************************************
** Meta object code from reading C++ file 'line_chart_view.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/component/chart/line_chart_view.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'line_chart_view.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LineChartView_t {
    QByteArrayData data[8];
    char stringdata0[72];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LineChartView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LineChartView_t qt_meta_stringdata_LineChartView = {
    {
QT_MOC_LITERAL(0, 0, 13), // "LineChartView"
QT_MOC_LITERAL(1, 14, 20), // "sigFocusPointUpdated"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 1), // "x"
QT_MOC_LITERAL(4, 38, 1), // "y"
QT_MOC_LITERAL(5, 40, 14), // "sigSelectCurve"
QT_MOC_LITERAL(6, 55, 3), // "idx"
QT_MOC_LITERAL(7, 59, 12) // "sigMouseMove"

    },
    "LineChartView\0sigFocusPointUpdated\0\0"
    "x\0y\0sigSelectCurve\0idx\0sigMouseMove"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LineChartView[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   29,    2, 0x06 /* Public */,
       5,    1,   34,    2, 0x06 /* Public */,
       7,    2,   37,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QReal, QMetaType::QReal,    3,    4,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    3,    4,

       0        // eod
};

void LineChartView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LineChartView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigFocusPointUpdated((*reinterpret_cast< qreal(*)>(_a[1])),(*reinterpret_cast< qreal(*)>(_a[2]))); break;
        case 1: _t->sigSelectCurve((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->sigMouseMove((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (LineChartView::*)(qreal , qreal );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LineChartView::sigFocusPointUpdated)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (LineChartView::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LineChartView::sigSelectCurve)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (LineChartView::*)(double , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LineChartView::sigMouseMove)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject LineChartView::staticMetaObject = { {
    QMetaObject::SuperData::link<QtCharts::QChartView::staticMetaObject>(),
    qt_meta_stringdata_LineChartView.data,
    qt_meta_data_LineChartView,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LineChartView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LineChartView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LineChartView.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "IObserver"))
        return static_cast< IObserver*>(this);
    return QtCharts::QChartView::qt_metacast(_clname);
}

int LineChartView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QtCharts::QChartView::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void LineChartView::sigFocusPointUpdated(qreal _t1, qreal _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void LineChartView::sigSelectCurve(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void LineChartView::sigMouseMove(double _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
