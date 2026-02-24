/****************************************************************************
** Meta object code from reading C++ file 'gamma_page_widget.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/pages/gamma_page_widget.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gamma_page_widget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GammaPageWidget_t {
    QByteArrayData data[28];
    char stringdata0[360];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GammaPageWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GammaPageWidget_t qt_meta_stringdata_GammaPageWidget = {
    {
QT_MOC_LITERAL(0, 0, 15), // "GammaPageWidget"
QT_MOC_LITERAL(1, 16, 8), // "readData"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 17), // "slotParamsChanged"
QT_MOC_LITERAL(4, 44, 27), // "slotControlPointsNumChanged"
QT_MOC_LITERAL(5, 72, 14), // "openLoadDialog"
QT_MOC_LITERAL(6, 87, 14), // "openSaveDialog"
QT_MOC_LITERAL(7, 102, 16), // "handleSelectFile"
QT_MOC_LITERAL(8, 119, 6), // "format"
QT_MOC_LITERAL(9, 126, 9), // "file_path"
QT_MOC_LITERAL(10, 136, 4), // "MODE"
QT_MOC_LITERAL(11, 141, 4), // "mode"
QT_MOC_LITERAL(12, 146, 15), // "handleAddCtrlPt"
QT_MOC_LITERAL(13, 162, 14), // "handleSetEvent"
QT_MOC_LITERAL(14, 177, 14), // "updateMousePos"
QT_MOC_LITERAL(15, 192, 1), // "x"
QT_MOC_LITERAL(16, 194, 1), // "y"
QT_MOC_LITERAL(17, 196, 10), // "resetCurve"
QT_MOC_LITERAL(18, 207, 18), // "slotReferSetSelect"
QT_MOC_LITERAL(19, 226, 16), // "slotReferSetSave"
QT_MOC_LITERAL(20, 243, 17), // "slotReferSetApply"
QT_MOC_LITERAL(21, 261, 18), // "slotXAdjustChanged"
QT_MOC_LITERAL(22, 280, 18), // "slotYAdjustChanged"
QT_MOC_LITERAL(23, 299, 15), // "slotAjustUpdate"
QT_MOC_LITERAL(24, 315, 1), // "p"
QT_MOC_LITERAL(25, 317, 16), // "slotRefreshTable"
QT_MOC_LITERAL(26, 334, 20), // "slotHandleSelectLine"
QT_MOC_LITERAL(27, 355, 4) // "text"

    },
    "GammaPageWidget\0readData\0\0slotParamsChanged\0"
    "slotControlPointsNumChanged\0openLoadDialog\0"
    "openSaveDialog\0handleSelectFile\0format\0"
    "file_path\0MODE\0mode\0handleAddCtrlPt\0"
    "handleSetEvent\0updateMousePos\0x\0y\0"
    "resetCurve\0slotReferSetSelect\0"
    "slotReferSetSave\0slotReferSetApply\0"
    "slotXAdjustChanged\0slotYAdjustChanged\0"
    "slotAjustUpdate\0p\0slotRefreshTable\0"
    "slotHandleSelectLine\0text"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GammaPageWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  104,    2, 0x0a /* Public */,
       3,    1,  105,    2, 0x08 /* Private */,
       4,    1,  108,    2, 0x08 /* Private */,
       5,    0,  111,    2, 0x08 /* Private */,
       6,    0,  112,    2, 0x08 /* Private */,
       7,    3,  113,    2, 0x08 /* Private */,
      12,    0,  120,    2, 0x08 /* Private */,
      13,    0,  121,    2, 0x08 /* Private */,
      14,    2,  122,    2, 0x08 /* Private */,
      17,    0,  127,    2, 0x08 /* Private */,
      18,    1,  128,    2, 0x08 /* Private */,
      19,    0,  131,    2, 0x08 /* Private */,
      20,    0,  132,    2, 0x08 /* Private */,
      21,    1,  133,    2, 0x08 /* Private */,
      22,    1,  136,    2, 0x08 /* Private */,
      23,    1,  139,    2, 0x08 /* Private */,
      25,    0,  142,    2, 0x08 /* Private */,
      26,    1,  143,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QVariant,    2,
    QMetaType::Void, QMetaType::QVariant,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 10,    8,    9,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,   15,   16,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QVariant,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QVariant,    2,
    QMetaType::Void, QMetaType::QVariant,    2,
    QMetaType::Void, QMetaType::QPointF,   24,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   27,

       0        // eod
};

void GammaPageWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GammaPageWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->readData(); break;
        case 1: _t->slotParamsChanged((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 2: _t->slotControlPointsNumChanged((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 3: _t->openLoadDialog(); break;
        case 4: _t->openSaveDialog(); break;
        case 5: _t->handleSelectFile((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< MODE(*)>(_a[3]))); break;
        case 6: _t->handleAddCtrlPt(); break;
        case 7: _t->handleSetEvent(); break;
        case 8: _t->updateMousePos((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 9: _t->resetCurve(); break;
        case 10: _t->slotReferSetSelect((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 11: _t->slotReferSetSave(); break;
        case 12: _t->slotReferSetApply(); break;
        case 13: _t->slotXAdjustChanged((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 14: _t->slotYAdjustChanged((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 15: _t->slotAjustUpdate((*reinterpret_cast< QPointF(*)>(_a[1]))); break;
        case 16: _t->slotRefreshTable(); break;
        case 17: _t->slotHandleSelectLine((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject GammaPageWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<BasePageWidget::staticMetaObject>(),
    qt_meta_stringdata_GammaPageWidget.data,
    qt_meta_data_GammaPageWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GammaPageWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GammaPageWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GammaPageWidget.stringdata0))
        return static_cast<void*>(this);
    return BasePageWidget::qt_metacast(_clname);
}

int GammaPageWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BasePageWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 18;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
