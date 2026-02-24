/****************************************************************************
** Meta object code from reading C++ file 'blc_histogram_widget.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/component/blc_histogram_widget.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'blc_histogram_widget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BlcHistogramWidget_t {
    QByteArrayData data[18];
    char stringdata0[223];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BlcHistogramWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BlcHistogramWidget_t qt_meta_stringdata_BlcHistogramWidget = {
    {
QT_MOC_LITERAL(0, 0, 18), // "BlcHistogramWidget"
QT_MOC_LITERAL(1, 19, 18), // "handleUpdatePoints"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 4), // "int*"
QT_MOC_LITERAL(4, 44, 7), // "rr_hist"
QT_MOC_LITERAL(5, 52, 7), // "gr_hist"
QT_MOC_LITERAL(6, 60, 7), // "gb_hist"
QT_MOC_LITERAL(7, 68, 7), // "bb_hist"
QT_MOC_LITERAL(8, 76, 13), // "r_offset_mean"
QT_MOC_LITERAL(9, 90, 14), // "gr_offset_mean"
QT_MOC_LITERAL(10, 105, 14), // "gb_offset_mean"
QT_MOC_LITERAL(11, 120, 13), // "b_offset_mean"
QT_MOC_LITERAL(12, 134, 15), // "r_offset_median"
QT_MOC_LITERAL(13, 150, 16), // "gr_offset_median"
QT_MOC_LITERAL(14, 167, 16), // "gb_offset_median"
QT_MOC_LITERAL(15, 184, 15), // "b_offset_median"
QT_MOC_LITERAL(16, 200, 17), // "handleSelectColor"
QT_MOC_LITERAL(17, 218, 4) // "text"

    },
    "BlcHistogramWidget\0handleUpdatePoints\0"
    "\0int*\0rr_hist\0gr_hist\0gb_hist\0bb_hist\0"
    "r_offset_mean\0gr_offset_mean\0"
    "gb_offset_mean\0b_offset_mean\0"
    "r_offset_median\0gr_offset_median\0"
    "gb_offset_median\0b_offset_median\0"
    "handleSelectColor\0text"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BlcHistogramWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,   12,   24,    2, 0x0a /* Public */,
      16,    1,   49,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3, 0x80000000 | 3, 0x80000000 | 3, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int, QMetaType::Int,    4,    5,    6,    7,    8,    9,   10,   11,   12,   13,   14,   15,
    QMetaType::Void, QMetaType::QString,   17,

       0        // eod
};

void BlcHistogramWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<BlcHistogramWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->handleUpdatePoints((*reinterpret_cast< int*(*)>(_a[1])),(*reinterpret_cast< int*(*)>(_a[2])),(*reinterpret_cast< int*(*)>(_a[3])),(*reinterpret_cast< int*(*)>(_a[4])),(*reinterpret_cast< int(*)>(_a[5])),(*reinterpret_cast< int(*)>(_a[6])),(*reinterpret_cast< int(*)>(_a[7])),(*reinterpret_cast< int(*)>(_a[8])),(*reinterpret_cast< int(*)>(_a[9])),(*reinterpret_cast< int(*)>(_a[10])),(*reinterpret_cast< int(*)>(_a[11])),(*reinterpret_cast< int(*)>(_a[12]))); break;
        case 1: _t->handleSelectColor((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject BlcHistogramWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_BlcHistogramWidget.data,
    qt_meta_data_BlcHistogramWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *BlcHistogramWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BlcHistogramWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BlcHistogramWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int BlcHistogramWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
