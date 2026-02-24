/****************************************************************************
** Meta object code from reading C++ file 'fisheye_utility_window.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/component/window/fisheye_utility_window.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fisheye_utility_window.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FishEyeWindow_t {
    QByteArrayData data[22];
    char stringdata0[272];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FishEyeWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FishEyeWindow_t qt_meta_stringdata_FishEyeWindow = {
    {
QT_MOC_LITERAL(0, 0, 13), // "FishEyeWindow"
QT_MOC_LITERAL(1, 14, 13), // "showCtrlPanel"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 15), // "handleOpenImage"
QT_MOC_LITERAL(4, 45, 16), // "handleModeSelect"
QT_MOC_LITERAL(5, 62, 1), // "m"
QT_MOC_LITERAL(6, 64, 21), // "handleMountModeChange"
QT_MOC_LITERAL(7, 86, 13), // "slotUpdateRgn"
QT_MOC_LITERAL(8, 100, 7), // "rgn_idx"
QT_MOC_LITERAL(9, 108, 7), // "QImage*"
QT_MOC_LITERAL(10, 116, 5), // "image"
QT_MOC_LITERAL(11, 122, 17), // "slotUpdatePanTilt"
QT_MOC_LITERAL(12, 140, 9), // "delta_pan"
QT_MOC_LITERAL(13, 150, 10), // "delta_tilt"
QT_MOC_LITERAL(14, 161, 15), // "slotUpdateTheta"
QT_MOC_LITERAL(15, 177, 12), // "delta_ThetaX"
QT_MOC_LITERAL(16, 190, 12), // "delta_ThetaY"
QT_MOC_LITERAL(17, 203, 12), // "delta_ThetaZ"
QT_MOC_LITERAL(18, 216, 14), // "slotUpdateZoom"
QT_MOC_LITERAL(19, 231, 11), // "delta_ZoomH"
QT_MOC_LITERAL(20, 243, 11), // "delta_ZoomV"
QT_MOC_LITERAL(21, 255, 16) // "slotResetAllRgns"

    },
    "FishEyeWindow\0showCtrlPanel\0\0"
    "handleOpenImage\0handleModeSelect\0m\0"
    "handleMountModeChange\0slotUpdateRgn\0"
    "rgn_idx\0QImage*\0image\0slotUpdatePanTilt\0"
    "delta_pan\0delta_tilt\0slotUpdateTheta\0"
    "delta_ThetaX\0delta_ThetaY\0delta_ThetaZ\0"
    "slotUpdateZoom\0delta_ZoomH\0delta_ZoomV\0"
    "slotResetAllRgns"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FishEyeWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x08 /* Private */,
       3,    0,   60,    2, 0x08 /* Private */,
       4,    1,   61,    2, 0x08 /* Private */,
       6,    1,   64,    2, 0x08 /* Private */,
       7,    2,   67,    2, 0x08 /* Private */,
      11,    2,   72,    2, 0x08 /* Private */,
      14,    3,   77,    2, 0x08 /* Private */,
      18,    2,   84,    2, 0x08 /* Private */,
      21,    0,   89,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 9,    8,   10,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,   12,   13,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Double,   15,   16,   17,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,   19,   20,
    QMetaType::Void,

       0        // eod
};

void FishEyeWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FishEyeWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->showCtrlPanel(); break;
        case 1: _t->handleOpenImage(); break;
        case 2: _t->handleModeSelect((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->handleMountModeChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->slotUpdateRgn((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QImage*(*)>(_a[2]))); break;
        case 5: _t->slotUpdatePanTilt((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 6: _t->slotUpdateTheta((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 7: _t->slotUpdateZoom((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 8: _t->slotResetAllRgns(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FishEyeWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_FishEyeWindow.data,
    qt_meta_data_FishEyeWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *FishEyeWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FishEyeWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FishEyeWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int FishEyeWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
