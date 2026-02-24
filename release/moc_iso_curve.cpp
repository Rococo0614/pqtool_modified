/****************************************************************************
** Meta object code from reading C++ file 'iso_curve.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/component/register/iso_curve.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'iso_curve.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_IsoCurve_t {
    QByteArrayData data[21];
    char stringdata0[262];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IsoCurve_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IsoCurve_t qt_meta_stringdata_IsoCurve = {
    {
QT_MOC_LITERAL(0, 0, 8), // "IsoCurve"
QT_MOC_LITERAL(1, 9, 15), // "sigValueChanged"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 5), // "value"
QT_MOC_LITERAL(4, 32, 16), // "sigPageWriteData"
QT_MOC_LITERAL(5, 49, 14), // "sigUpdateChart"
QT_MOC_LITERAL(6, 64, 17), // "handleImportTable"
QT_MOC_LITERAL(7, 82, 9), // "file_path"
QT_MOC_LITERAL(8, 92, 17), // "handleExportTable"
QT_MOC_LITERAL(9, 110, 10), // "cellEdited"
QT_MOC_LITERAL(10, 121, 10), // "QLineEdit*"
QT_MOC_LITERAL(11, 132, 4), // "edit"
QT_MOC_LITERAL(12, 137, 3), // "idx"
QT_MOC_LITERAL(13, 141, 17), // "handleUpdateChart"
QT_MOC_LITERAL(14, 159, 16), // "handleIsoChanged"
QT_MOC_LITERAL(15, 176, 9), // "iso_index"
QT_MOC_LITERAL(16, 186, 18), // "handlePointsUpdate"
QT_MOC_LITERAL(17, 205, 14), // "QList<QPointF>"
QT_MOC_LITERAL(18, 220, 2), // "ps"
QT_MOC_LITERAL(19, 223, 19), // "handlePageWriteData"
QT_MOC_LITERAL(20, 243, 18) // "handleValueChanged"

    },
    "IsoCurve\0sigValueChanged\0\0value\0"
    "sigPageWriteData\0sigUpdateChart\0"
    "handleImportTable\0file_path\0"
    "handleExportTable\0cellEdited\0QLineEdit*\0"
    "edit\0idx\0handleUpdateChart\0handleIsoChanged\0"
    "iso_index\0handlePointsUpdate\0"
    "QList<QPointF>\0ps\0handlePageWriteData\0"
    "handleValueChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IsoCurve[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       4,    0,   72,    2, 0x06 /* Public */,
       5,    0,   73,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   74,    2, 0x0a /* Public */,
       8,    1,   77,    2, 0x0a /* Public */,
       9,    2,   80,    2, 0x0a /* Public */,
      13,    0,   85,    2, 0x0a /* Public */,
      14,    1,   86,    2, 0x0a /* Public */,
      16,    1,   89,    2, 0x0a /* Public */,
      19,    0,   92,    2, 0x0a /* Public */,
      20,    1,   93,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QVariant,    3,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, 0x80000000 | 10, QMetaType::Int,   11,   12,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   15,
    QMetaType::Void, 0x80000000 | 17,   18,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QVariant,    2,

       0        // eod
};

void IsoCurve::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<IsoCurve *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigValueChanged((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 1: _t->sigPageWriteData(); break;
        case 2: _t->sigUpdateChart(); break;
        case 3: _t->handleImportTable((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->handleExportTable((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->cellEdited((*reinterpret_cast< QLineEdit*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->handleUpdateChart(); break;
        case 7: _t->handleIsoChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->handlePointsUpdate((*reinterpret_cast< QList<QPointF>(*)>(_a[1]))); break;
        case 9: _t->handlePageWriteData(); break;
        case 10: _t->handleValueChanged((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QLineEdit* >(); break;
            }
            break;
        case 8:
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
            using _t = void (IsoCurve::*)(QVariant );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IsoCurve::sigValueChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (IsoCurve::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IsoCurve::sigPageWriteData)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (IsoCurve::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IsoCurve::sigUpdateChart)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject IsoCurve::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_IsoCurve.data,
    qt_meta_data_IsoCurve,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *IsoCurve::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IsoCurve::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_IsoCurve.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "IObserver"))
        return static_cast< IObserver*>(this);
    return QWidget::qt_metacast(_clname);
}

int IsoCurve::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void IsoCurve::sigValueChanged(QVariant _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void IsoCurve::sigPageWriteData()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void IsoCurve::sigUpdateChart()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
