/****************************************************************************
** Meta object code from reading C++ file 'multi_image_viewer_widget.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/component/image/multi_image_viewer_widget.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'multi_image_viewer_widget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MultiImageViewerWidget_t {
    QByteArrayData data[31];
    char stringdata0[432];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MultiImageViewerWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MultiImageViewerWidget_t qt_meta_stringdata_MultiImageViewerWidget = {
    {
QT_MOC_LITERAL(0, 0, 22), // "MultiImageViewerWidget"
QT_MOC_LITERAL(1, 23, 11), // "sigSetImage"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 14), // "const QPixmap*"
QT_MOC_LITERAL(4, 51, 5), // "image"
QT_MOC_LITERAL(5, 57, 19), // "sigClearCaptureInfo"
QT_MOC_LITERAL(6, 77, 17), // "handleShowRawItem"
QT_MOC_LITERAL(7, 95, 13), // "RawImageItem*"
QT_MOC_LITERAL(8, 109, 8), // "raw_item"
QT_MOC_LITERAL(9, 118, 15), // "handleShowImage"
QT_MOC_LITERAL(10, 134, 7), // "QImage&"
QT_MOC_LITERAL(11, 142, 15), // "handleSaveImage"
QT_MOC_LITERAL(12, 158, 14), // "clearImageData"
QT_MOC_LITERAL(13, 173, 16), // "handleImgNavigat"
QT_MOC_LITERAL(14, 190, 3), // "dir"
QT_MOC_LITERAL(15, 194, 16), // "slotSelectFactor"
QT_MOC_LITERAL(16, 211, 4), // "text"
QT_MOC_LITERAL(17, 216, 22), // "handleChildWindowEvent"
QT_MOC_LITERAL(18, 239, 11), // "window_type"
QT_MOC_LITERAL(19, 251, 18), // "slotPositionChange"
QT_MOC_LITERAL(20, 270, 14), // "slotEnableGrid"
QT_MOC_LITERAL(21, 285, 5), // "state"
QT_MOC_LITERAL(22, 291, 20), // "handleRectToCropInfo"
QT_MOC_LITERAL(23, 312, 4), // "rect"
QT_MOC_LITERAL(24, 317, 6), // "factor"
QT_MOC_LITERAL(25, 324, 10), // "enableRect"
QT_MOC_LITERAL(26, 335, 11), // "disableRect"
QT_MOC_LITERAL(27, 347, 24), // "handleOpenColorHistogram"
QT_MOC_LITERAL(28, 372, 20), // "handleOpenLevelMeter"
QT_MOC_LITERAL(29, 393, 23), // "handleOpenRawStatistics"
QT_MOC_LITERAL(30, 417, 14) // "handleSetImage"

    },
    "MultiImageViewerWidget\0sigSetImage\0\0"
    "const QPixmap*\0image\0sigClearCaptureInfo\0"
    "handleShowRawItem\0RawImageItem*\0"
    "raw_item\0handleShowImage\0QImage&\0"
    "handleSaveImage\0clearImageData\0"
    "handleImgNavigat\0dir\0slotSelectFactor\0"
    "text\0handleChildWindowEvent\0window_type\0"
    "slotPositionChange\0slotEnableGrid\0"
    "state\0handleRectToCropInfo\0rect\0factor\0"
    "enableRect\0disableRect\0handleOpenColorHistogram\0"
    "handleOpenLevelMeter\0handleOpenRawStatistics\0"
    "handleSetImage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MultiImageViewerWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  104,    2, 0x06 /* Public */,
       5,    0,  107,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,  108,    2, 0x0a /* Public */,
       9,    1,  111,    2, 0x0a /* Public */,
      11,    0,  114,    2, 0x0a /* Public */,
      12,    0,  115,    2, 0x0a /* Public */,
      13,    1,  116,    2, 0x0a /* Public */,
      15,    1,  119,    2, 0x0a /* Public */,
      17,    1,  122,    2, 0x0a /* Public */,
      19,    1,  125,    2, 0x0a /* Public */,
      20,    1,  128,    2, 0x0a /* Public */,
      22,    2,  131,    2, 0x0a /* Public */,
      25,    0,  136,    2, 0x0a /* Public */,
      26,    0,  137,    2, 0x0a /* Public */,
      27,    0,  138,    2, 0x08 /* Private */,
      28,    0,  139,    2, 0x08 /* Private */,
      29,    0,  140,    2, 0x08 /* Private */,
      30,    1,  141,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, 0x80000000 | 10,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Void, QMetaType::Int,   21,
    QMetaType::Void, QMetaType::QRectF, QMetaType::Double,   23,   24,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void MultiImageViewerWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MultiImageViewerWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigSetImage((*reinterpret_cast< const QPixmap*(*)>(_a[1]))); break;
        case 1: _t->sigClearCaptureInfo(); break;
        case 2: _t->handleShowRawItem((*reinterpret_cast< RawImageItem*(*)>(_a[1]))); break;
        case 3: _t->handleShowImage((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 4: _t->handleSaveImage(); break;
        case 5: _t->clearImageData(); break;
        case 6: _t->handleImgNavigat((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->slotSelectFactor((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->handleChildWindowEvent((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->slotPositionChange((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->slotEnableGrid((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->handleRectToCropInfo((*reinterpret_cast< QRectF(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 12: _t->enableRect(); break;
        case 13: _t->disableRect(); break;
        case 14: _t->handleOpenColorHistogram(); break;
        case 15: _t->handleOpenLevelMeter(); break;
        case 16: _t->handleOpenRawStatistics(); break;
        case 17: _t->handleSetImage((*reinterpret_cast< const QPixmap*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
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
            using _t = void (MultiImageViewerWidget::*)(const QPixmap * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MultiImageViewerWidget::sigSetImage)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MultiImageViewerWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MultiImageViewerWidget::sigClearCaptureInfo)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MultiImageViewerWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_MultiImageViewerWidget.data,
    qt_meta_data_MultiImageViewerWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MultiImageViewerWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MultiImageViewerWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MultiImageViewerWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int MultiImageViewerWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}

// SIGNAL 0
void MultiImageViewerWidget::sigSetImage(const QPixmap * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MultiImageViewerWidget::sigClearCaptureInfo()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
