/****************************************************************************
** Meta object code from reading C++ file 'curve_userdefine.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/component/register/curve_userdefine.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'curve_userdefine.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CurveUserDefine_t {
    QByteArrayData data[23];
    char stringdata0[330];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CurveUserDefine_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CurveUserDefine_t qt_meta_stringdata_CurveUserDefine = {
    {
QT_MOC_LITERAL(0, 0, 15), // "CurveUserDefine"
QT_MOC_LITERAL(1, 16, 15), // "sigValueChanged"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 5), // "value"
QT_MOC_LITERAL(4, 39, 16), // "sigPageWriteData"
QT_MOC_LITERAL(5, 56, 17), // "handleImportTable"
QT_MOC_LITERAL(6, 74, 9), // "file_path"
QT_MOC_LITERAL(7, 84, 17), // "handleExportTable"
QT_MOC_LITERAL(8, 102, 27), // "slotControlPointsNumChanged"
QT_MOC_LITERAL(9, 130, 3), // "num"
QT_MOC_LITERAL(10, 134, 15), // "handleAddCtrlPt"
QT_MOC_LITERAL(11, 150, 14), // "handleSetEvent"
QT_MOC_LITERAL(12, 165, 14), // "updateMousePos"
QT_MOC_LITERAL(13, 180, 1), // "x"
QT_MOC_LITERAL(14, 182, 1), // "y"
QT_MOC_LITERAL(15, 184, 10), // "resetCurve"
QT_MOC_LITERAL(16, 195, 18), // "slotReferSetSelect"
QT_MOC_LITERAL(17, 214, 16), // "slotReferSetSave"
QT_MOC_LITERAL(18, 231, 17), // "slotReferSetApply"
QT_MOC_LITERAL(19, 249, 22), // "slotUpdateWdrHistogram"
QT_MOC_LITERAL(20, 272, 18), // "slotUpdateExpRatio"
QT_MOC_LITERAL(21, 291, 19), // "handlePageWriteData"
QT_MOC_LITERAL(22, 311, 18) // "handleValueChanged"

    },
    "CurveUserDefine\0sigValueChanged\0\0value\0"
    "sigPageWriteData\0handleImportTable\0"
    "file_path\0handleExportTable\0"
    "slotControlPointsNumChanged\0num\0"
    "handleAddCtrlPt\0handleSetEvent\0"
    "updateMousePos\0x\0y\0resetCurve\0"
    "slotReferSetSelect\0slotReferSetSave\0"
    "slotReferSetApply\0slotUpdateWdrHistogram\0"
    "slotUpdateExpRatio\0handlePageWriteData\0"
    "handleValueChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CurveUserDefine[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   94,    2, 0x06 /* Public */,
       4,    0,   97,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   98,    2, 0x0a /* Public */,
       7,    1,  101,    2, 0x0a /* Public */,
       8,    1,  104,    2, 0x0a /* Public */,
      10,    0,  107,    2, 0x0a /* Public */,
      11,    0,  108,    2, 0x0a /* Public */,
      12,    2,  109,    2, 0x0a /* Public */,
      15,    0,  114,    2, 0x0a /* Public */,
      16,    1,  115,    2, 0x0a /* Public */,
      17,    0,  118,    2, 0x0a /* Public */,
      18,    0,  119,    2, 0x0a /* Public */,
      19,    1,  120,    2, 0x0a /* Public */,
      20,    1,  123,    2, 0x0a /* Public */,
      21,    0,  126,    2, 0x0a /* Public */,
      22,    1,  127,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QVariant,    3,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QVariant,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,   13,   14,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QVariant,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QVariant,    2,
    QMetaType::Void, QMetaType::QVariant,    2,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QVariant,    2,

       0        // eod
};

void CurveUserDefine::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CurveUserDefine *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigValueChanged((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 1: _t->sigPageWriteData(); break;
        case 2: _t->handleImportTable((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->handleExportTable((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->slotControlPointsNumChanged((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 5: _t->handleAddCtrlPt(); break;
        case 6: _t->handleSetEvent(); break;
        case 7: _t->updateMousePos((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 8: _t->resetCurve(); break;
        case 9: _t->slotReferSetSelect((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 10: _t->slotReferSetSave(); break;
        case 11: _t->slotReferSetApply(); break;
        case 12: _t->slotUpdateWdrHistogram((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 13: _t->slotUpdateExpRatio((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 14: _t->handlePageWriteData(); break;
        case 15: _t->handleValueChanged((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CurveUserDefine::*)(QVariant );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CurveUserDefine::sigValueChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CurveUserDefine::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CurveUserDefine::sigPageWriteData)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CurveUserDefine::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CurveUserDefine.data,
    qt_meta_data_CurveUserDefine,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CurveUserDefine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CurveUserDefine::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CurveUserDefine.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "IObserver"))
        return static_cast< IObserver*>(this);
    return QWidget::qt_metacast(_clname);
}

int CurveUserDefine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void CurveUserDefine::sigValueChanged(QVariant _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CurveUserDefine::sigPageWriteData()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
