/****************************************************************************
** Meta object code from reading C++ file 'calibration_distortion_window.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/component/window/calibration_distortion_window.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'calibration_distortion_window.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CalibrationDistortionWindow_t {
    QByteArrayData data[18];
    char stringdata0[222];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CalibrationDistortionWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CalibrationDistortionWindow_t qt_meta_stringdata_CalibrationDistortionWindow = {
    {
QT_MOC_LITERAL(0, 0, 27), // "CalibrationDistortionWindow"
QT_MOC_LITERAL(1, 28, 20), // "handleMultiYuvStatus"
QT_MOC_LITERAL(2, 49, 0), // ""
QT_MOC_LITERAL(3, 50, 2), // "id"
QT_MOC_LITERAL(4, 53, 13), // "slotGotYuvImg"
QT_MOC_LITERAL(5, 67, 3), // "cmd"
QT_MOC_LITERAL(6, 71, 17), // "slotUpdateEyeType"
QT_MOC_LITERAL(7, 89, 16), // "openSaveRootPath"
QT_MOC_LITERAL(8, 106, 19), // "openConvertRootPath"
QT_MOC_LITERAL(9, 126, 17), // "updateOpencvFlag2"
QT_MOC_LITERAL(10, 144, 5), // "index"
QT_MOC_LITERAL(11, 150, 19), // "handleImgListUpdate"
QT_MOC_LITERAL(12, 170, 11), // "SIGNAL_TYPE"
QT_MOC_LITERAL(13, 182, 4), // "type"
QT_MOC_LITERAL(14, 187, 9), // "updateRMS"
QT_MOC_LITERAL(15, 197, 5), // "total"
QT_MOC_LITERAL(16, 203, 14), // "QList<double>*"
QT_MOC_LITERAL(17, 218, 3) // "rms"

    },
    "CalibrationDistortionWindow\0"
    "handleMultiYuvStatus\0\0id\0slotGotYuvImg\0"
    "cmd\0slotUpdateEyeType\0openSaveRootPath\0"
    "openConvertRootPath\0updateOpencvFlag2\0"
    "index\0handleImgListUpdate\0SIGNAL_TYPE\0"
    "type\0updateRMS\0total\0QList<double>*\0"
    "rms"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CalibrationDistortionWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x08 /* Private */,
       4,    1,   57,    2, 0x08 /* Private */,
       6,    0,   60,    2, 0x08 /* Private */,
       7,    0,   61,    2, 0x08 /* Private */,
       8,    0,   62,    2, 0x08 /* Private */,
       9,    1,   63,    2, 0x08 /* Private */,
      11,    2,   66,    2, 0x08 /* Private */,
      14,    3,   71,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, 0x80000000 | 12, QMetaType::Int,   13,    3,
    QMetaType::Void, QMetaType::Double, 0x80000000 | 16, QMetaType::Int,   15,   17,   13,

       0        // eod
};

void CalibrationDistortionWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CalibrationDistortionWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->handleMultiYuvStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->slotGotYuvImg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->slotUpdateEyeType(); break;
        case 3: _t->openSaveRootPath(); break;
        case 4: _t->openConvertRootPath(); break;
        case 5: _t->updateOpencvFlag2((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->handleImgListUpdate((*reinterpret_cast< SIGNAL_TYPE(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->updateRMS((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< QList<double>*(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CalibrationDistortionWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<BaseCalibrationPageWidget::staticMetaObject>(),
    qt_meta_stringdata_CalibrationDistortionWindow.data,
    qt_meta_data_CalibrationDistortionWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CalibrationDistortionWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CalibrationDistortionWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CalibrationDistortionWindow.stringdata0))
        return static_cast<void*>(this);
    return BaseCalibrationPageWidget::qt_metacast(_clname);
}

int CalibrationDistortionWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseCalibrationPageWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
