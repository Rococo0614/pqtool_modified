/****************************************************************************
** Meta object code from reading C++ file 'lsc_calibration_page_widget.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/pages/lsc_calibration_page_widget.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'lsc_calibration_page_widget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LSCCalibrationPageWidget_t {
    QByteArrayData data[8];
    char stringdata0[103];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LSCCalibrationPageWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LSCCalibrationPageWidget_t qt_meta_stringdata_LSCCalibrationPageWidget = {
    {
QT_MOC_LITERAL(0, 0, 24), // "LSCCalibrationPageWidget"
QT_MOC_LITERAL(1, 25, 27), // "sigCv184xMlscChromaLumaFlag"
QT_MOC_LITERAL(2, 53, 0), // ""
QT_MOC_LITERAL(3, 54, 4), // "flag"
QT_MOC_LITERAL(4, 59, 16), // "handleImportData"
QT_MOC_LITERAL(5, 76, 4), // "name"
QT_MOC_LITERAL(6, 81, 4), // "data"
QT_MOC_LITERAL(7, 86, 16) // "handleLscSetting"

    },
    "LSCCalibrationPageWidget\0"
    "sigCv184xMlscChromaLumaFlag\0\0flag\0"
    "handleImportData\0name\0data\0handleLscSetting"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LSCCalibrationPageWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    2,   32,    2, 0x09 /* Protected */,
       7,    0,   37,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Int, QMetaType::Bool,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QJsonObject,    5,    6,
    QMetaType::Void,

       0        // eod
};

void LSCCalibrationPageWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LSCCalibrationPageWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { int _r = _t->sigCv184xMlscChromaLumaFlag((*reinterpret_cast< bool(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 1: _t->handleImportData((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QJsonObject(*)>(_a[2]))); break;
        case 2: _t->handleLscSetting(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = int (LSCCalibrationPageWidget::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LSCCalibrationPageWidget::sigCv184xMlscChromaLumaFlag)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject LSCCalibrationPageWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<BaseCalibrationPageWidget::staticMetaObject>(),
    qt_meta_stringdata_LSCCalibrationPageWidget.data,
    qt_meta_data_LSCCalibrationPageWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LSCCalibrationPageWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LSCCalibrationPageWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LSCCalibrationPageWidget.stringdata0))
        return static_cast<void*>(this);
    return BaseCalibrationPageWidget::qt_metacast(_clname);
}

int LSCCalibrationPageWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseCalibrationPageWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
int LSCCalibrationPageWidget::sigCv184xMlscChromaLumaFlag(bool _t1)
{
    int _t0{};
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t0))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
    return _t0;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
