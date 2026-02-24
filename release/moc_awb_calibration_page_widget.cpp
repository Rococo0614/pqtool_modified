/****************************************************************************
** Meta object code from reading C++ file 'awb_calibration_page_widget.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/pages/awb_calibration_page_widget.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'awb_calibration_page_widget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AWBCalibrationPageWidget_t {
    QByteArrayData data[14];
    char stringdata0[180];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AWBCalibrationPageWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AWBCalibrationPageWidget_t qt_meta_stringdata_AWBCalibrationPageWidget = {
    {
QT_MOC_LITERAL(0, 0, 24), // "AWBCalibrationPageWidget"
QT_MOC_LITERAL(1, 25, 16), // "handleImportData"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 4), // "name"
QT_MOC_LITERAL(4, 48, 4), // "data"
QT_MOC_LITERAL(5, 53, 14), // "handleDelPoint"
QT_MOC_LITERAL(6, 68, 3), // "idx"
QT_MOC_LITERAL(7, 72, 27), // "handleReceiveAWBCalibration"
QT_MOC_LITERAL(8, 100, 2), // "id"
QT_MOC_LITERAL(9, 103, 11), // "loadBlcData"
QT_MOC_LITERAL(10, 115, 4), // "path"
QT_MOC_LITERAL(11, 120, 11), // "selectItems"
QT_MOC_LITERAL(12, 132, 24), // "QuickCalibrationLoadData"
QT_MOC_LITERAL(13, 157, 22) // "handleQuickCalibration"

    },
    "AWBCalibrationPageWidget\0handleImportData\0"
    "\0name\0data\0handleDelPoint\0idx\0"
    "handleReceiveAWBCalibration\0id\0"
    "loadBlcData\0path\0selectItems\0"
    "QuickCalibrationLoadData\0"
    "handleQuickCalibration"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AWBCalibrationPageWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   49,    2, 0x09 /* Protected */,
       5,    1,   54,    2, 0x09 /* Protected */,
       7,    1,   57,    2, 0x09 /* Protected */,
       9,    1,   60,    2, 0x09 /* Protected */,
      11,    0,   63,    2, 0x09 /* Protected */,
      12,    1,   64,    2, 0x09 /* Protected */,
      13,    0,   67,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QJsonObject,    3,    4,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Int, QMetaType::QString,   10,
    QMetaType::Bool,
    QMetaType::Int, QMetaType::QString,   10,
    QMetaType::Int,

       0        // eod
};

void AWBCalibrationPageWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AWBCalibrationPageWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->handleImportData((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QJsonObject(*)>(_a[2]))); break;
        case 1: _t->handleDelPoint((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->handleReceiveAWBCalibration((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: { int _r = _t->loadBlcData((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 4: { bool _r = _t->selectItems();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 5: { int _r = _t->QuickCalibrationLoadData((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 6: { int _r = _t->handleQuickCalibration();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject AWBCalibrationPageWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<BaseCalibrationPageWidget::staticMetaObject>(),
    qt_meta_stringdata_AWBCalibrationPageWidget.data,
    qt_meta_data_AWBCalibrationPageWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *AWBCalibrationPageWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AWBCalibrationPageWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AWBCalibrationPageWidget.stringdata0))
        return static_cast<void*>(this);
    return BaseCalibrationPageWidget::qt_metacast(_clname);
}

int AWBCalibrationPageWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseCalibrationPageWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
