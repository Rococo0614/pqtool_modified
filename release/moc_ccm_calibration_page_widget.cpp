/****************************************************************************
** Meta object code from reading C++ file 'ccm_calibration_page_widget.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/pages/ccm_calibration_page_widget.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ccm_calibration_page_widget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CCMCalibrationPageWidget_t {
    QByteArrayData data[10];
    char stringdata0[151];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CCMCalibrationPageWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CCMCalibrationPageWidget_t qt_meta_stringdata_CCMCalibrationPageWidget = {
    {
QT_MOC_LITERAL(0, 0, 24), // "CCMCalibrationPageWidget"
QT_MOC_LITERAL(1, 25, 20), // "handleCv184xMlscMode"
QT_MOC_LITERAL(2, 46, 0), // ""
QT_MOC_LITERAL(3, 47, 4), // "flag"
QT_MOC_LITERAL(4, 52, 16), // "handleImportData"
QT_MOC_LITERAL(5, 69, 4), // "name"
QT_MOC_LITERAL(6, 74, 4), // "data"
QT_MOC_LITERAL(7, 79, 23), // "handleCustomizeIspGamma"
QT_MOC_LITERAL(8, 103, 27), // "handleCustomizeLabReference"
QT_MOC_LITERAL(9, 131, 19) // "handleCustomLabData"

    },
    "CCMCalibrationPageWidget\0handleCv184xMlscMode\0"
    "\0flag\0handleImportData\0name\0data\0"
    "handleCustomizeIspGamma\0"
    "handleCustomizeLabReference\0"
    "handleCustomLabData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CCMCalibrationPageWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x0a /* Public */,
       4,    2,   42,    2, 0x09 /* Protected */,
       7,    0,   47,    2, 0x09 /* Protected */,
       8,    0,   48,    2, 0x09 /* Protected */,
       9,    0,   49,    2, 0x09 /* Protected */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QJsonObject,    5,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CCMCalibrationPageWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CCMCalibrationPageWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->handleCv184xMlscMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->handleImportData((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QJsonObject(*)>(_a[2]))); break;
        case 2: _t->handleCustomizeIspGamma(); break;
        case 3: _t->handleCustomizeLabReference(); break;
        case 4: _t->handleCustomLabData(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CCMCalibrationPageWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<BaseCalibrationPageWidget::staticMetaObject>(),
    qt_meta_stringdata_CCMCalibrationPageWidget.data,
    qt_meta_data_CCMCalibrationPageWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CCMCalibrationPageWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CCMCalibrationPageWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CCMCalibrationPageWidget.stringdata0))
        return static_cast<void*>(this);
    return BaseCalibrationPageWidget::qt_metacast(_clname);
}

int CCMCalibrationPageWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseCalibrationPageWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
