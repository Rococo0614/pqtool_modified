/****************************************************************************
** Meta object code from reading C++ file 'vo_gamma_page_widget.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/pages/vo_gamma_page_widget.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'vo_gamma_page_widget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VOGammaPageWidget_t {
    QByteArrayData data[19];
    char stringdata0[240];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VOGammaPageWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VOGammaPageWidget_t qt_meta_stringdata_VOGammaPageWidget = {
    {
QT_MOC_LITERAL(0, 0, 17), // "VOGammaPageWidget"
QT_MOC_LITERAL(1, 18, 17), // "slotParamsChanged"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 27), // "slotControlPointsNumChanged"
QT_MOC_LITERAL(4, 65, 14), // "openLoadDialog"
QT_MOC_LITERAL(5, 80, 14), // "openSaveDialog"
QT_MOC_LITERAL(6, 95, 16), // "handleSelectFile"
QT_MOC_LITERAL(7, 112, 6), // "format"
QT_MOC_LITERAL(8, 119, 9), // "file_path"
QT_MOC_LITERAL(9, 129, 4), // "MODE"
QT_MOC_LITERAL(10, 134, 4), // "mode"
QT_MOC_LITERAL(11, 139, 14), // "updateMousePos"
QT_MOC_LITERAL(12, 154, 1), // "x"
QT_MOC_LITERAL(13, 156, 1), // "y"
QT_MOC_LITERAL(14, 158, 10), // "resetCurve"
QT_MOC_LITERAL(15, 169, 18), // "slotReferSetSelect"
QT_MOC_LITERAL(16, 188, 16), // "slotReferSetSave"
QT_MOC_LITERAL(17, 205, 17), // "slotReferSetApply"
QT_MOC_LITERAL(18, 223, 16) // "slotRefreshTable"

    },
    "VOGammaPageWidget\0slotParamsChanged\0"
    "\0slotControlPointsNumChanged\0"
    "openLoadDialog\0openSaveDialog\0"
    "handleSelectFile\0format\0file_path\0"
    "MODE\0mode\0updateMousePos\0x\0y\0resetCurve\0"
    "slotReferSetSelect\0slotReferSetSave\0"
    "slotReferSetApply\0slotRefreshTable"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VOGammaPageWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x08 /* Private */,
       3,    1,   72,    2, 0x08 /* Private */,
       4,    0,   75,    2, 0x08 /* Private */,
       5,    0,   76,    2, 0x08 /* Private */,
       6,    3,   77,    2, 0x08 /* Private */,
      11,    2,   84,    2, 0x08 /* Private */,
      14,    0,   89,    2, 0x08 /* Private */,
      15,    1,   90,    2, 0x08 /* Private */,
      16,    0,   93,    2, 0x08 /* Private */,
      17,    0,   94,    2, 0x08 /* Private */,
      18,    0,   95,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QVariant,    2,
    QMetaType::Void, QMetaType::QVariant,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 9,    7,    8,   10,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,   12,   13,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QVariant,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void VOGammaPageWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VOGammaPageWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotParamsChanged((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 1: _t->slotControlPointsNumChanged((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 2: _t->openLoadDialog(); break;
        case 3: _t->openSaveDialog(); break;
        case 4: _t->handleSelectFile((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< MODE(*)>(_a[3]))); break;
        case 5: _t->updateMousePos((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 6: _t->resetCurve(); break;
        case 7: _t->slotReferSetSelect((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 8: _t->slotReferSetSave(); break;
        case 9: _t->slotReferSetApply(); break;
        case 10: _t->slotRefreshTable(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject VOGammaPageWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<BasePageWidget::staticMetaObject>(),
    qt_meta_stringdata_VOGammaPageWidget.data,
    qt_meta_data_VOGammaPageWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *VOGammaPageWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VOGammaPageWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VOGammaPageWidget.stringdata0))
        return static_cast<void*>(this);
    return BasePageWidget::qt_metacast(_clname);
}

int VOGammaPageWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BasePageWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
