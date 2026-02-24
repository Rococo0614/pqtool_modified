/****************************************************************************
** Meta object code from reading C++ file 'simple_line_chart_view.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/component/chart/simple_line_chart_view.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'simple_line_chart_view.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SimpleLineChartView_t {
    QByteArrayData data[13];
    char stringdata0[141];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SimpleLineChartView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SimpleLineChartView_t qt_meta_stringdata_SimpleLineChartView = {
    {
QT_MOC_LITERAL(0, 0, 19), // "SimpleLineChartView"
QT_MOC_LITERAL(1, 20, 16), // "sigPointYChanged"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 5), // "index"
QT_MOC_LITERAL(4, 44, 4), // "newY"
QT_MOC_LITERAL(5, 49, 15), // "sigCurveChanged"
QT_MOC_LITERAL(6, 65, 13), // "QList<QPoint>"
QT_MOC_LITERAL(7, 79, 6), // "points"
QT_MOC_LITERAL(8, 86, 17), // "sigUpdateMousePos"
QT_MOC_LITERAL(9, 104, 1), // "x"
QT_MOC_LITERAL(10, 106, 1), // "y"
QT_MOC_LITERAL(11, 108, 15), // "sigValueChanged"
QT_MOC_LITERAL(12, 124, 16) // "sigPointSelected"

    },
    "SimpleLineChartView\0sigPointYChanged\0"
    "\0index\0newY\0sigCurveChanged\0QList<QPoint>\0"
    "points\0sigUpdateMousePos\0x\0y\0"
    "sigValueChanged\0sigPointSelected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SimpleLineChartView[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   39,    2, 0x06 /* Public */,
       5,    1,   44,    2, 0x06 /* Public */,
       8,    2,   47,    2, 0x06 /* Public */,
      11,    0,   52,    2, 0x06 /* Public */,
      12,    1,   53,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QReal,    3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    9,   10,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void SimpleLineChartView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SimpleLineChartView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigPointYChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< qreal(*)>(_a[2]))); break;
        case 1: _t->sigCurveChanged((*reinterpret_cast< const QList<QPoint>(*)>(_a[1]))); break;
        case 2: _t->sigUpdateMousePos((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 3: _t->sigValueChanged(); break;
        case 4: _t->sigPointSelected((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QPoint> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SimpleLineChartView::*)(int , qreal );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SimpleLineChartView::sigPointYChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SimpleLineChartView::*)(const QList<QPoint> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SimpleLineChartView::sigCurveChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SimpleLineChartView::*)(double , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SimpleLineChartView::sigUpdateMousePos)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SimpleLineChartView::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SimpleLineChartView::sigValueChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (SimpleLineChartView::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SimpleLineChartView::sigPointSelected)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SimpleLineChartView::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_SimpleLineChartView.data,
    qt_meta_data_SimpleLineChartView,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SimpleLineChartView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SimpleLineChartView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SimpleLineChartView.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SimpleLineChartView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void SimpleLineChartView::sigPointYChanged(int _t1, qreal _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SimpleLineChartView::sigCurveChanged(const QList<QPoint> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SimpleLineChartView::sigUpdateMousePos(double _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SimpleLineChartView::sigValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void SimpleLineChartView::sigPointSelected(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
