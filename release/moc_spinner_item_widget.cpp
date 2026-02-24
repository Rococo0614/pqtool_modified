/****************************************************************************
** Meta object code from reading C++ file 'spinner_item_widget.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/component/register/register_item/spinner_item_widget.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'spinner_item_widget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SpinnerItemWidget_t {
    QByteArrayData data[7];
    char stringdata0[96];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SpinnerItemWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SpinnerItemWidget_t qt_meta_stringdata_SpinnerItemWidget = {
    {
QT_MOC_LITERAL(0, 0, 17), // "SpinnerItemWidget"
QT_MOC_LITERAL(1, 18, 18), // "sigValueChangedCpt"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 5), // "value"
QT_MOC_LITERAL(4, 44, 12), // "valueChanged"
QT_MOC_LITERAL(5, 57, 19), // "spinnerValueChanged"
QT_MOC_LITERAL(6, 77, 18) // "sliderValueChanged"

    },
    "SpinnerItemWidget\0sigValueChangedCpt\0"
    "\0value\0valueChanged\0spinnerValueChanged\0"
    "sliderValueChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SpinnerItemWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   37,    2, 0x08 /* Private */,
       5,    1,   40,    2, 0x08 /* Private */,
       6,    1,   43,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QVariant,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,

       0        // eod
};

void SpinnerItemWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SpinnerItemWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigValueChangedCpt((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 1: _t->valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->spinnerValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->sliderValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SpinnerItemWidget::*)(QVariant );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SpinnerItemWidget::sigValueChangedCpt)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SpinnerItemWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<BaseItemWidget::staticMetaObject>(),
    qt_meta_stringdata_SpinnerItemWidget.data,
    qt_meta_data_SpinnerItemWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SpinnerItemWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SpinnerItemWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SpinnerItemWidget.stringdata0))
        return static_cast<void*>(this);
    return BaseItemWidget::qt_metacast(_clname);
}

int SpinnerItemWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseItemWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void SpinnerItemWidget::sigValueChangedCpt(QVariant _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
