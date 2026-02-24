/****************************************************************************
** Meta object code from reading C++ file 'clut_online_item_widget.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/component/register/register_item/clut_online_item_widget.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'clut_online_item_widget.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GradientWidget_t {
    QByteArrayData data[1];
    char stringdata0[15];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GradientWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GradientWidget_t qt_meta_stringdata_GradientWidget = {
    {
QT_MOC_LITERAL(0, 0, 14) // "GradientWidget"

    },
    "GradientWidget"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GradientWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void GradientWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject GradientWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_GradientWidget.data,
    qt_meta_data_GradientWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GradientWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GradientWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GradientWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int GradientWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_CLUTCurveWidget_t {
    QByteArrayData data[11];
    char stringdata0[100];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CLUTCurveWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CLUTCurveWidget_t qt_meta_stringdata_CLUTCurveWidget = {
    {
QT_MOC_LITERAL(0, 0, 15), // "CLUTCurveWidget"
QT_MOC_LITERAL(1, 16, 13), // "pointYChanged"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 5), // "index"
QT_MOC_LITERAL(4, 37, 4), // "newY"
QT_MOC_LITERAL(5, 42, 12), // "curveChanged"
QT_MOC_LITERAL(6, 55, 15), // "QVector<QPoint>"
QT_MOC_LITERAL(7, 71, 6), // "points"
QT_MOC_LITERAL(8, 78, 17), // "sigUpdateMousePos"
QT_MOC_LITERAL(9, 96, 1), // "x"
QT_MOC_LITERAL(10, 98, 1) // "y"

    },
    "CLUTCurveWidget\0pointYChanged\0\0index\0"
    "newY\0curveChanged\0QVector<QPoint>\0"
    "points\0sigUpdateMousePos\0x\0y"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CLUTCurveWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   29,    2, 0x06 /* Public */,
       5,    1,   34,    2, 0x06 /* Public */,
       8,    2,   37,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QReal,    3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    9,   10,

       0        // eod
};

void CLUTCurveWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CLUTCurveWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->pointYChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< qreal(*)>(_a[2]))); break;
        case 1: _t->curveChanged((*reinterpret_cast< const QVector<QPoint>(*)>(_a[1]))); break;
        case 2: _t->sigUpdateMousePos((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<QPoint> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CLUTCurveWidget::*)(int , qreal );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CLUTCurveWidget::pointYChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CLUTCurveWidget::*)(const QVector<QPoint> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CLUTCurveWidget::curveChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CLUTCurveWidget::*)(double , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CLUTCurveWidget::sigUpdateMousePos)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CLUTCurveWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<GradientWidget::staticMetaObject>(),
    qt_meta_stringdata_CLUTCurveWidget.data,
    qt_meta_data_CLUTCurveWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CLUTCurveWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CLUTCurveWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLUTCurveWidget.stringdata0))
        return static_cast<void*>(this);
    return GradientWidget::qt_metacast(_clname);
}

int CLUTCurveWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = GradientWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void CLUTCurveWidget::pointYChanged(int _t1, qreal _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CLUTCurveWidget::curveChanged(const QVector<QPoint> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CLUTCurveWidget::sigUpdateMousePos(double _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
struct qt_meta_stringdata_CLUTOnlineItemWidget_t {
    QByteArrayData data[22];
    char stringdata0[262];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CLUTOnlineItemWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CLUTOnlineItemWidget_t qt_meta_stringdata_CLUTOnlineItemWidget = {
    {
QT_MOC_LITERAL(0, 0, 20), // "CLUTOnlineItemWidget"
QT_MOC_LITERAL(1, 21, 17), // "sigUpdateMousePos"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 1), // "x"
QT_MOC_LITERAL(4, 42, 1), // "y"
QT_MOC_LITERAL(5, 44, 19), // "sigPressedScatterPt"
QT_MOC_LITERAL(6, 64, 1), // "p"
QT_MOC_LITERAL(7, 66, 15), // "sigValueChanged"
QT_MOC_LITERAL(8, 82, 14), // "updateMousePos"
QT_MOC_LITERAL(9, 97, 14), // "openLoadDialog"
QT_MOC_LITERAL(10, 112, 14), // "openSaveDialog"
QT_MOC_LITERAL(11, 127, 16), // "handleResetCurve"
QT_MOC_LITERAL(12, 144, 16), // "slotRefreshTable"
QT_MOC_LITERAL(13, 161, 15), // "QVector<QPoint>"
QT_MOC_LITERAL(14, 177, 6), // "points"
QT_MOC_LITERAL(15, 184, 16), // "handleSelectFile"
QT_MOC_LITERAL(16, 201, 6), // "format"
QT_MOC_LITERAL(17, 208, 9), // "file_path"
QT_MOC_LITERAL(18, 218, 4), // "MODE"
QT_MOC_LITERAL(19, 223, 4), // "mode"
QT_MOC_LITERAL(20, 228, 17), // "hueSpinnerChanged"
QT_MOC_LITERAL(21, 246, 15) // "slotUpdateCurve"

    },
    "CLUTOnlineItemWidget\0sigUpdateMousePos\0"
    "\0x\0y\0sigPressedScatterPt\0p\0sigValueChanged\0"
    "updateMousePos\0openLoadDialog\0"
    "openSaveDialog\0handleResetCurve\0"
    "slotRefreshTable\0QVector<QPoint>\0"
    "points\0handleSelectFile\0format\0file_path\0"
    "MODE\0mode\0hueSpinnerChanged\0slotUpdateCurve"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CLUTOnlineItemWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   69,    2, 0x06 /* Public */,
       5,    1,   74,    2, 0x06 /* Public */,
       7,    0,   77,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    2,   78,    2, 0x08 /* Private */,
       9,    0,   83,    2, 0x08 /* Private */,
      10,    0,   84,    2, 0x08 /* Private */,
      11,    0,   85,    2, 0x08 /* Private */,
      12,    1,   86,    2, 0x08 /* Private */,
      15,    3,   89,    2, 0x08 /* Private */,
      20,    1,   96,    2, 0x08 /* Private */,
      21,    1,   99,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    3,    4,
    QMetaType::Void, QMetaType::QPoint,    6,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 18,   16,   17,   19,
    QMetaType::Void, QMetaType::QVariant,    2,
    QMetaType::Void, QMetaType::QVariant,    2,

       0        // eod
};

void CLUTOnlineItemWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CLUTOnlineItemWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigUpdateMousePos((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 1: _t->sigPressedScatterPt((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 2: _t->sigValueChanged(); break;
        case 3: _t->updateMousePos((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 4: _t->openLoadDialog(); break;
        case 5: _t->openSaveDialog(); break;
        case 6: _t->handleResetCurve(); break;
        case 7: _t->slotRefreshTable((*reinterpret_cast< const QVector<QPoint>(*)>(_a[1]))); break;
        case 8: _t->handleSelectFile((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< MODE(*)>(_a[3]))); break;
        case 9: _t->hueSpinnerChanged((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 10: _t->slotUpdateCurve((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<QPoint> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CLUTOnlineItemWidget::*)(double , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CLUTOnlineItemWidget::sigUpdateMousePos)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CLUTOnlineItemWidget::*)(QPoint );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CLUTOnlineItemWidget::sigPressedScatterPt)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CLUTOnlineItemWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CLUTOnlineItemWidget::sigValueChanged)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CLUTOnlineItemWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<BaseItemWidget::staticMetaObject>(),
    qt_meta_stringdata_CLUTOnlineItemWidget.data,
    qt_meta_data_CLUTOnlineItemWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CLUTOnlineItemWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CLUTOnlineItemWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLUTOnlineItemWidget.stringdata0))
        return static_cast<void*>(this);
    return BaseItemWidget::qt_metacast(_clname);
}

int CLUTOnlineItemWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BaseItemWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void CLUTOnlineItemWidget::sigUpdateMousePos(double _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CLUTOnlineItemWidget::sigPressedScatterPt(QPoint _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CLUTOnlineItemWidget::sigValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
