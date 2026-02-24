/****************************************************************************
** Meta object code from reading C++ file 'curve_set_dialog.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/component/dialog/curve_set_dialog.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'curve_set_dialog.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CurveSetDialog_t {
    QByteArrayData data[11];
    char stringdata0[189];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CurveSetDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CurveSetDialog_t qt_meta_stringdata_CurveSetDialog = {
    {
QT_MOC_LITERAL(0, 0, 14), // "CurveSetDialog"
QT_MOC_LITERAL(1, 15, 20), // "sigLineParamsChanged"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 24), // "sigGaussianParamsChanged"
QT_MOC_LITERAL(4, 62, 21), // "sigGammaParamsChanged"
QT_MOC_LITERAL(5, 84, 11), // "handleApply"
QT_MOC_LITERAL(6, 96, 8), // "handleOk"
QT_MOC_LITERAL(7, 105, 19), // "slotLineParamUpdate"
QT_MOC_LITERAL(8, 125, 23), // "slotGaussianParamUpdate"
QT_MOC_LITERAL(9, 149, 20), // "slotGammaParamUpdate"
QT_MOC_LITERAL(10, 170, 18) // "slotFunctionChange"

    },
    "CurveSetDialog\0sigLineParamsChanged\0"
    "\0sigGaussianParamsChanged\0"
    "sigGammaParamsChanged\0handleApply\0"
    "handleOk\0slotLineParamUpdate\0"
    "slotGaussianParamUpdate\0slotGammaParamUpdate\0"
    "slotFunctionChange"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CurveSetDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   59,    2, 0x06 /* Public */,
       3,    3,   64,    2, 0x06 /* Public */,
       4,    1,   71,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   74,    2, 0x08 /* Private */,
       6,    0,   75,    2, 0x08 /* Private */,
       7,    0,   76,    2, 0x08 /* Private */,
       8,    0,   77,    2, 0x08 /* Private */,
       9,    0,   78,    2, 0x08 /* Private */,
      10,    0,   79,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    2,    2,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Double,    2,    2,    2,
    QMetaType::Void, QMetaType::Double,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CurveSetDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CurveSetDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigLineParamsChanged((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 1: _t->sigGaussianParamsChanged((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 2: _t->sigGammaParamsChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->handleApply(); break;
        case 4: _t->handleOk(); break;
        case 5: _t->slotLineParamUpdate(); break;
        case 6: _t->slotGaussianParamUpdate(); break;
        case 7: _t->slotGammaParamUpdate(); break;
        case 8: _t->slotFunctionChange(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CurveSetDialog::*)(double , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CurveSetDialog::sigLineParamsChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CurveSetDialog::*)(double , double , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CurveSetDialog::sigGaussianParamsChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CurveSetDialog::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CurveSetDialog::sigGammaParamsChanged)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CurveSetDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_CurveSetDialog.data,
    qt_meta_data_CurveSetDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CurveSetDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CurveSetDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CurveSetDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int CurveSetDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void CurveSetDialog::sigLineParamsChanged(double _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CurveSetDialog::sigGaussianParamsChanged(double _t1, double _t2, double _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CurveSetDialog::sigGammaParamsChanged(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
