/****************************************************************************
** Meta object code from reading C++ file 'base_calibration_page_widget.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/pages/base_calibration_page_widget.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'base_calibration_page_widget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BaseCalibrationPageWidget_t {
    QByteArrayData data[29];
    char stringdata0[401];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BaseCalibrationPageWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BaseCalibrationPageWidget_t qt_meta_stringdata_BaseCalibrationPageWidget = {
    {
QT_MOC_LITERAL(0, 0, 25), // "BaseCalibrationPageWidget"
QT_MOC_LITERAL(1, 26, 14), // "sigCalibration"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 12), // "sigImportRaw"
QT_MOC_LITERAL(4, 55, 18), // "sigImportSourceJpg"
QT_MOC_LITERAL(5, 74, 18), // "sigImportTargetJpg"
QT_MOC_LITERAL(6, 93, 19), // "sigShowInputRawItem"
QT_MOC_LITERAL(7, 113, 13), // "RawImageItem*"
QT_MOC_LITERAL(8, 127, 8), // "raw_item"
QT_MOC_LITERAL(9, 136, 21), // "sigShowOutputRawImage"
QT_MOC_LITERAL(10, 158, 7), // "QImage&"
QT_MOC_LITERAL(11, 166, 5), // "image"
QT_MOC_LITERAL(12, 172, 21), // "sigShowOutputBlcImage"
QT_MOC_LITERAL(13, 194, 19), // "sigChangeCaliStatus"
QT_MOC_LITERAL(14, 214, 18), // "CALIBRATION_STATUS"
QT_MOC_LITERAL(15, 233, 6), // "status"
QT_MOC_LITERAL(16, 240, 13), // "sigImportData"
QT_MOC_LITERAL(17, 254, 4), // "name"
QT_MOC_LITERAL(18, 259, 4), // "data"
QT_MOC_LITERAL(19, 264, 15), // "sigImportAwbRaw"
QT_MOC_LITERAL(20, 280, 13), // "sigCaliStatus"
QT_MOC_LITERAL(21, 294, 21), // "CALIBRATION_MODULE_ID"
QT_MOC_LITERAL(22, 316, 2), // "id"
QT_MOC_LITERAL(23, 319, 3), // "log"
QT_MOC_LITERAL(24, 323, 20), // "writeCalibrationData"
QT_MOC_LITERAL(25, 344, 11), // "showMessage"
QT_MOC_LITERAL(26, 356, 16), // "handleImportData"
QT_MOC_LITERAL(27, 373, 16), // "handleExportData"
QT_MOC_LITERAL(28, 390, 10) // "handleSave"

    },
    "BaseCalibrationPageWidget\0sigCalibration\0"
    "\0sigImportRaw\0sigImportSourceJpg\0"
    "sigImportTargetJpg\0sigShowInputRawItem\0"
    "RawImageItem*\0raw_item\0sigShowOutputRawImage\0"
    "QImage&\0image\0sigShowOutputBlcImage\0"
    "sigChangeCaliStatus\0CALIBRATION_STATUS\0"
    "status\0sigImportData\0name\0data\0"
    "sigImportAwbRaw\0sigCaliStatus\0"
    "CALIBRATION_MODULE_ID\0id\0log\0"
    "writeCalibrationData\0showMessage\0"
    "handleImportData\0handleExportData\0"
    "handleSave"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BaseCalibrationPageWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      11,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   94,    2, 0x06 /* Public */,
       3,    0,   95,    2, 0x06 /* Public */,
       4,    0,   96,    2, 0x06 /* Public */,
       5,    0,   97,    2, 0x06 /* Public */,
       6,    1,   98,    2, 0x06 /* Public */,
       9,    1,  101,    2, 0x06 /* Public */,
      12,    1,  104,    2, 0x06 /* Public */,
      13,    1,  107,    2, 0x06 /* Public */,
      16,    2,  110,    2, 0x06 /* Public */,
      19,    0,  115,    2, 0x06 /* Public */,
      20,    3,  116,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      24,    1,  123,    2, 0x0a /* Public */,
      24,    0,  126,    2, 0x2a /* Public | MethodCloned */,
      26,    0,  127,    2, 0x0a /* Public */,
      27,    0,  128,    2, 0x0a /* Public */,
      28,    0,  129,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, QMetaType::QString, QMetaType::QJsonObject,   17,   18,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 21, 0x80000000 | 14, QMetaType::QString,   22,   15,   23,

 // slots: parameters
    QMetaType::Int, QMetaType::Bool,   25,
    QMetaType::Int,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void BaseCalibrationPageWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<BaseCalibrationPageWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigCalibration(); break;
        case 1: _t->sigImportRaw(); break;
        case 2: _t->sigImportSourceJpg(); break;
        case 3: _t->sigImportTargetJpg(); break;
        case 4: _t->sigShowInputRawItem((*reinterpret_cast< RawImageItem*(*)>(_a[1]))); break;
        case 5: _t->sigShowOutputRawImage((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 6: _t->sigShowOutputBlcImage((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 7: _t->sigChangeCaliStatus((*reinterpret_cast< CALIBRATION_STATUS(*)>(_a[1]))); break;
        case 8: _t->sigImportData((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QJsonObject(*)>(_a[2]))); break;
        case 9: _t->sigImportAwbRaw(); break;
        case 10: _t->sigCaliStatus((*reinterpret_cast< CALIBRATION_MODULE_ID(*)>(_a[1])),(*reinterpret_cast< CALIBRATION_STATUS(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 11: { int _r = _t->writeCalibrationData((*reinterpret_cast< bool(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 12: { int _r = _t->writeCalibrationData();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 13: _t->handleImportData(); break;
        case 14: _t->handleExportData(); break;
        case 15: _t->handleSave(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< RawImageItem* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (BaseCalibrationPageWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BaseCalibrationPageWidget::sigCalibration)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (BaseCalibrationPageWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BaseCalibrationPageWidget::sigImportRaw)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (BaseCalibrationPageWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BaseCalibrationPageWidget::sigImportSourceJpg)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (BaseCalibrationPageWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BaseCalibrationPageWidget::sigImportTargetJpg)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (BaseCalibrationPageWidget::*)(RawImageItem * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BaseCalibrationPageWidget::sigShowInputRawItem)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (BaseCalibrationPageWidget::*)(QImage & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BaseCalibrationPageWidget::sigShowOutputRawImage)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (BaseCalibrationPageWidget::*)(QImage & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BaseCalibrationPageWidget::sigShowOutputBlcImage)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (BaseCalibrationPageWidget::*)(CALIBRATION_STATUS );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BaseCalibrationPageWidget::sigChangeCaliStatus)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (BaseCalibrationPageWidget::*)(QString , QJsonObject );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BaseCalibrationPageWidget::sigImportData)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (BaseCalibrationPageWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BaseCalibrationPageWidget::sigImportAwbRaw)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (BaseCalibrationPageWidget::*)(CALIBRATION_MODULE_ID , CALIBRATION_STATUS , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BaseCalibrationPageWidget::sigCaliStatus)) {
                *result = 10;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject BaseCalibrationPageWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<BasePageWidget::staticMetaObject>(),
    qt_meta_stringdata_BaseCalibrationPageWidget.data,
    qt_meta_data_BaseCalibrationPageWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *BaseCalibrationPageWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BaseCalibrationPageWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BaseCalibrationPageWidget.stringdata0))
        return static_cast<void*>(this);
    return BasePageWidget::qt_metacast(_clname);
}

int BaseCalibrationPageWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BasePageWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void BaseCalibrationPageWidget::sigCalibration()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void BaseCalibrationPageWidget::sigImportRaw()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void BaseCalibrationPageWidget::sigImportSourceJpg()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void BaseCalibrationPageWidget::sigImportTargetJpg()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void BaseCalibrationPageWidget::sigShowInputRawItem(RawImageItem * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void BaseCalibrationPageWidget::sigShowOutputRawImage(QImage & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void BaseCalibrationPageWidget::sigShowOutputBlcImage(QImage & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void BaseCalibrationPageWidget::sigChangeCaliStatus(CALIBRATION_STATUS _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void BaseCalibrationPageWidget::sigImportData(QString _t1, QJsonObject _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void BaseCalibrationPageWidget::sigImportAwbRaw()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}

// SIGNAL 10
void BaseCalibrationPageWidget::sigCaliStatus(CALIBRATION_MODULE_ID _t1, CALIBRATION_STATUS _t2, QString _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
