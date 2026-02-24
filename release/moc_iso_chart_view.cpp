/****************************************************************************
** Meta object code from reading C++ file 'iso_chart_view.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/component/chart/iso_chart_view.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'iso_chart_view.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ISOChartView_t {
    QByteArrayData data[7];
    char stringdata0[81];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ISOChartView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ISOChartView_t qt_meta_stringdata_ISOChartView = {
    {
QT_MOC_LITERAL(0, 0, 12), // "ISOChartView"
QT_MOC_LITERAL(1, 13, 15), // "sigPointsUpdate"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 14), // "QList<QPointF>"
QT_MOC_LITERAL(4, 45, 2), // "ps"
QT_MOC_LITERAL(5, 48, 15), // "sigValueChanged"
QT_MOC_LITERAL(6, 64, 16) // "sigPageWriteData"

    },
    "ISOChartView\0sigPointsUpdate\0\0"
    "QList<QPointF>\0ps\0sigValueChanged\0"
    "sigPageWriteData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ISOChartView[] = {

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
       1,    1,   29,    2, 0x06 /* Public */,
       5,    1,   32,    2, 0x06 /* Public */,
       6,    0,   35,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QVariant,    2,
    QMetaType::Void,

       0        // eod
};

void ISOChartView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ISOChartView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigPointsUpdate((*reinterpret_cast< QList<QPointF>(*)>(_a[1]))); break;
        case 1: _t->sigValueChanged((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 2: _t->sigPageWriteData(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QPointF> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ISOChartView::*)(QList<QPointF> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ISOChartView::sigPointsUpdate)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ISOChartView::*)(QVariant );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ISOChartView::sigValueChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ISOChartView::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ISOChartView::sigPageWriteData)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ISOChartView::staticMetaObject = { {
    QMetaObject::SuperData::link<LineChartView::staticMetaObject>(),
    qt_meta_stringdata_ISOChartView.data,
    qt_meta_data_ISOChartView,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ISOChartView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ISOChartView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ISOChartView.stringdata0))
        return static_cast<void*>(this);
    return LineChartView::qt_metacast(_clname);
}

int ISOChartView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = LineChartView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void ISOChartView::sigPointsUpdate(QList<QPointF> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ISOChartView::sigValueChanged(QVariant _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ISOChartView::sigPageWriteData()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
