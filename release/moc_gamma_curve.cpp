/****************************************************************************
** Meta object code from reading C++ file 'gamma_curve.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/component/register/gamma_curve.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gamma_curve.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GammaCurve_t {
    QByteArrayData data[21];
    char stringdata0[265];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GammaCurve_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GammaCurve_t qt_meta_stringdata_GammaCurve = {
    {
QT_MOC_LITERAL(0, 0, 10), // "GammaCurve"
QT_MOC_LITERAL(1, 11, 15), // "sigValueChanged"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 16), // "sigWritePageData"
QT_MOC_LITERAL(4, 45, 17), // "slotParamsChanged"
QT_MOC_LITERAL(5, 63, 27), // "slotControlPointsNumChanged"
QT_MOC_LITERAL(6, 91, 14), // "openLoadDialog"
QT_MOC_LITERAL(7, 106, 14), // "openSaveDialog"
QT_MOC_LITERAL(8, 121, 16), // "handleSelectFile"
QT_MOC_LITERAL(9, 138, 6), // "format"
QT_MOC_LITERAL(10, 145, 9), // "file_path"
QT_MOC_LITERAL(11, 155, 4), // "MODE"
QT_MOC_LITERAL(12, 160, 4), // "mode"
QT_MOC_LITERAL(13, 165, 14), // "updateMousePos"
QT_MOC_LITERAL(14, 180, 1), // "x"
QT_MOC_LITERAL(15, 182, 1), // "y"
QT_MOC_LITERAL(16, 184, 10), // "resetCurve"
QT_MOC_LITERAL(17, 195, 18), // "slotReferSetSelect"
QT_MOC_LITERAL(18, 214, 16), // "slotReferSetSave"
QT_MOC_LITERAL(19, 231, 17), // "slotReferSetApply"
QT_MOC_LITERAL(20, 249, 15) // "slotChartChange"

    },
    "GammaCurve\0sigValueChanged\0\0"
    "sigWritePageData\0slotParamsChanged\0"
    "slotControlPointsNumChanged\0openLoadDialog\0"
    "openSaveDialog\0handleSelectFile\0format\0"
    "file_path\0MODE\0mode\0updateMousePos\0x\0"
    "y\0resetCurve\0slotReferSetSelect\0"
    "slotReferSetSave\0slotReferSetApply\0"
    "slotChartChange"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GammaCurve[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x06 /* Public */,
       3,    0,   80,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   81,    2, 0x09 /* Protected */,
       5,    1,   84,    2, 0x09 /* Protected */,
       6,    0,   87,    2, 0x09 /* Protected */,
       7,    0,   88,    2, 0x09 /* Protected */,
       8,    3,   89,    2, 0x09 /* Protected */,
      13,    2,   96,    2, 0x09 /* Protected */,
      16,    0,  101,    2, 0x09 /* Protected */,
      17,    1,  102,    2, 0x09 /* Protected */,
      18,    0,  105,    2, 0x09 /* Protected */,
      19,    0,  106,    2, 0x09 /* Protected */,
      20,    0,  107,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QVariant,    2,
    QMetaType::Void, QMetaType::QVariant,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, 0x80000000 | 11,    9,   10,   12,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,   14,   15,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QVariant,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void GammaCurve::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GammaCurve *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigValueChanged(); break;
        case 1: _t->sigWritePageData(); break;
        case 2: _t->slotParamsChanged((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 3: _t->slotControlPointsNumChanged((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 4: _t->openLoadDialog(); break;
        case 5: _t->openSaveDialog(); break;
        case 6: _t->handleSelectFile((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< MODE(*)>(_a[3]))); break;
        case 7: _t->updateMousePos((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 8: _t->resetCurve(); break;
        case 9: _t->slotReferSetSelect((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 10: _t->slotReferSetSave(); break;
        case 11: _t->slotReferSetApply(); break;
        case 12: _t->slotChartChange(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GammaCurve::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GammaCurve::sigValueChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (GammaCurve::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GammaCurve::sigWritePageData)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject GammaCurve::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_GammaCurve.data,
    qt_meta_data_GammaCurve,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GammaCurve::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GammaCurve::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GammaCurve.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int GammaCurve::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void GammaCurve::sigValueChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void GammaCurve::sigWritePageData()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
struct qt_meta_stringdata_GammaCurveLV_t {
    QByteArrayData data[6];
    char stringdata0[83];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GammaCurveLV_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GammaCurveLV_t qt_meta_stringdata_GammaCurveLV = {
    {
QT_MOC_LITERAL(0, 0, 12), // "GammaCurveLV"
QT_MOC_LITERAL(1, 13, 15), // "slotChartChange"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 16), // "slotRefreshChart"
QT_MOC_LITERAL(4, 47, 18), // "slotRelatedChanged"
QT_MOC_LITERAL(5, 66, 16) // "slotCheckLvValue"

    },
    "GammaCurveLV\0slotChartChange\0\0"
    "slotRefreshChart\0slotRelatedChanged\0"
    "slotCheckLvValue"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GammaCurveLV[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x08 /* Private */,
       3,    0,   35,    2, 0x08 /* Private */,
       4,    1,   36,    2, 0x08 /* Private */,
       5,    1,   39,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QVariant,    2,
    QMetaType::Void, QMetaType::QVariant,    2,

       0        // eod
};

void GammaCurveLV::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GammaCurveLV *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotChartChange(); break;
        case 1: _t->slotRefreshChart(); break;
        case 2: _t->slotRelatedChanged((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 3: _t->slotCheckLvValue((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject GammaCurveLV::staticMetaObject = { {
    QMetaObject::SuperData::link<GammaCurve::staticMetaObject>(),
    qt_meta_stringdata_GammaCurveLV.data,
    qt_meta_data_GammaCurveLV,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GammaCurveLV::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GammaCurveLV::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GammaCurveLV.stringdata0))
        return static_cast<void*>(this);
    return GammaCurve::qt_metacast(_clname);
}

int GammaCurveLV::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = GammaCurve::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
struct qt_meta_stringdata_GammaCurveRGB_t {
    QByteArrayData data[3];
    char stringdata0[33];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GammaCurveRGB_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GammaCurveRGB_t qt_meta_stringdata_GammaCurveRGB = {
    {
QT_MOC_LITERAL(0, 0, 13), // "GammaCurveRGB"
QT_MOC_LITERAL(1, 14, 17), // "slotParamsChanged"
QT_MOC_LITERAL(2, 32, 0) // ""

    },
    "GammaCurveRGB\0slotParamsChanged\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GammaCurveRGB[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QVariant,    2,

       0        // eod
};

void GammaCurveRGB::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GammaCurveRGB *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotParamsChanged((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject GammaCurveRGB::staticMetaObject = { {
    QMetaObject::SuperData::link<GammaCurve::staticMetaObject>(),
    qt_meta_stringdata_GammaCurveRGB.data,
    qt_meta_data_GammaCurveRGB,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GammaCurveRGB::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GammaCurveRGB::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GammaCurveRGB.stringdata0))
        return static_cast<void*>(this);
    return GammaCurve::qt_metacast(_clname);
}

int GammaCurveRGB::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = GammaCurve::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_MapCurve_t {
    QByteArrayData data[5];
    char stringdata0[65];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MapCurve_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MapCurve_t qt_meta_stringdata_MapCurve = {
    {
QT_MOC_LITERAL(0, 0, 8), // "MapCurve"
QT_MOC_LITERAL(1, 9, 34), // "slotSendGetRecFshdrMapcurveIn..."
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 16), // "slotRefreshCurve"
QT_MOC_LITERAL(4, 62, 2) // "id"

    },
    "MapCurve\0slotSendGetRecFshdrMapcurveInfoCmd\0"
    "\0slotRefreshCurve\0id"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MapCurve[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x08 /* Private */,
       3,    1,   25,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,

       0        // eod
};

void MapCurve::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MapCurve *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->slotSendGetRecFshdrMapcurveInfoCmd(); break;
        case 1: _t->slotRefreshCurve((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MapCurve::staticMetaObject = { {
    QMetaObject::SuperData::link<GammaCurve::staticMetaObject>(),
    qt_meta_stringdata_MapCurve.data,
    qt_meta_data_MapCurve,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MapCurve::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MapCurve::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MapCurve.stringdata0))
        return static_cast<void*>(this);
    return GammaCurve::qt_metacast(_clname);
}

int MapCurve::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = GammaCurve::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
