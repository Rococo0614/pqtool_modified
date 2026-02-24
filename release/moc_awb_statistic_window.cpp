/****************************************************************************
** Meta object code from reading C++ file 'awb_statistic_window.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/component/window/awb_statistic_window.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'awb_statistic_window.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AWBStatWindow_t {
    QByteArrayData data[44];
    char stringdata0[581];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AWBStatWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AWBStatWindow_t qt_meta_stringdata_AWBStatWindow = {
    {
QT_MOC_LITERAL(0, 0, 13), // "AWBStatWindow"
QT_MOC_LITERAL(1, 14, 11), // "sigWinClose"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 15), // "updateStatistic"
QT_MOC_LITERAL(4, 43, 18), // "QMap<QString,int>&"
QT_MOC_LITERAL(5, 62, 15), // "exportStatistic"
QT_MOC_LITERAL(6, 78, 19), // "updateZoneSatPoints"
QT_MOC_LITERAL(7, 98, 6), // "float*"
QT_MOC_LITERAL(8, 105, 12), // "s_AWB_DBG_S*"
QT_MOC_LITERAL(9, 118, 7), // "pAwbDbg"
QT_MOC_LITERAL(10, 126, 20), // "updateGlobalSatPoint"
QT_MOC_LITERAL(11, 147, 2), // "x0"
QT_MOC_LITERAL(12, 150, 2), // "y0"
QT_MOC_LITERAL(13, 153, 2), // "x1"
QT_MOC_LITERAL(14, 156, 2), // "y1"
QT_MOC_LITERAL(15, 159, 17), // "updateResultPoint"
QT_MOC_LITERAL(16, 177, 1), // "x"
QT_MOC_LITERAL(17, 179, 1), // "y"
QT_MOC_LITERAL(18, 181, 24), // "updateWhiteZoneCurvesMid"
QT_MOC_LITERAL(19, 206, 3), // "num"
QT_MOC_LITERAL(20, 210, 6), // "tableX"
QT_MOC_LITERAL(21, 217, 6), // "tableY"
QT_MOC_LITERAL(22, 224, 24), // "updateWhiteZoneCurvesTop"
QT_MOC_LITERAL(23, 249, 24), // "updateWhiteZoneCurvesBot"
QT_MOC_LITERAL(24, 274, 20), // "updateCurveLeftLimit"
QT_MOC_LITERAL(25, 295, 21), // "updateCurveRightLimit"
QT_MOC_LITERAL(26, 317, 19), // "updateSelectedPoint"
QT_MOC_LITERAL(27, 337, 23), // "updateCalibrationStatus"
QT_MOC_LITERAL(28, 361, 6), // "status"
QT_MOC_LITERAL(29, 368, 22), // "updateExtraLightSource"
QT_MOC_LITERAL(30, 391, 7), // "enabled"
QT_MOC_LITERAL(31, 399, 4), // "int*"
QT_MOC_LITERAL(32, 404, 11), // "float[][25]"
QT_MOC_LITERAL(33, 416, 16), // "updateCalibPoint"
QT_MOC_LITERAL(34, 433, 9), // "int[3][3]"
QT_MOC_LITERAL(35, 443, 8), // "calib_pt"
QT_MOC_LITERAL(36, 452, 17), // "updateCurveRegion"
QT_MOC_LITERAL(37, 470, 6), // "int[4]"
QT_MOC_LITERAL(38, 477, 6), // "region"
QT_MOC_LITERAL(39, 484, 21), // "slotSetShowStatistics"
QT_MOC_LITERAL(40, 506, 25), // "slotSetShowPlanckianCurve"
QT_MOC_LITERAL(41, 532, 20), // "slotSetShowWhiteZone"
QT_MOC_LITERAL(42, 553, 21), // "slotCurrentTabChanged"
QT_MOC_LITERAL(43, 575, 5) // "index"

    },
    "AWBStatWindow\0sigWinClose\0\0updateStatistic\0"
    "QMap<QString,int>&\0exportStatistic\0"
    "updateZoneSatPoints\0float*\0s_AWB_DBG_S*\0"
    "pAwbDbg\0updateGlobalSatPoint\0x0\0y0\0"
    "x1\0y1\0updateResultPoint\0x\0y\0"
    "updateWhiteZoneCurvesMid\0num\0tableX\0"
    "tableY\0updateWhiteZoneCurvesTop\0"
    "updateWhiteZoneCurvesBot\0updateCurveLeftLimit\0"
    "updateCurveRightLimit\0updateSelectedPoint\0"
    "updateCalibrationStatus\0status\0"
    "updateExtraLightSource\0enabled\0int*\0"
    "float[][25]\0updateCalibPoint\0int[3][3]\0"
    "calib_pt\0updateCurveRegion\0int[4]\0"
    "region\0slotSetShowStatistics\0"
    "slotSetShowPlanckianCurve\0"
    "slotSetShowWhiteZone\0slotCurrentTabChanged\0"
    "index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AWBStatWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  119,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,  120,    2, 0x0a /* Public */,
       3,    1,  123,    2, 0x0a /* Public */,
       5,    0,  126,    2, 0x0a /* Public */,
       6,    5,  127,    2, 0x0a /* Public */,
      10,    4,  138,    2, 0x0a /* Public */,
      15,    2,  147,    2, 0x0a /* Public */,
      18,    3,  152,    2, 0x0a /* Public */,
      22,    3,  159,    2, 0x0a /* Public */,
      23,    3,  166,    2, 0x0a /* Public */,
      24,    2,  173,    2, 0x0a /* Public */,
      25,    2,  178,    2, 0x0a /* Public */,
      26,    4,  183,    2, 0x0a /* Public */,
      27,    1,  192,    2, 0x0a /* Public */,
      29,    4,  195,    2, 0x0a /* Public */,
      33,    1,  204,    2, 0x0a /* Public */,
      36,    1,  207,    2, 0x0a /* Public */,
      39,    0,  210,    2, 0x08 /* Private */,
      40,    0,  211,    2, 0x08 /* Private */,
      41,    0,  212,    2, 0x08 /* Private */,
      42,    1,  213,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QVariant,    2,
    QMetaType::Void, 0x80000000 | 4,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7, 0x80000000 | 7, 0x80000000 | 7, 0x80000000 | 7, 0x80000000 | 8,    2,    2,    2,    2,    9,
    QMetaType::Void, QMetaType::Float, QMetaType::Float, QMetaType::Float, QMetaType::Float,   11,   12,   13,   14,
    QMetaType::Void, QMetaType::Float, QMetaType::Float,   16,   17,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 7, 0x80000000 | 7,   19,   20,   21,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 7, 0x80000000 | 7,   19,   20,   21,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 7, 0x80000000 | 7,   19,   20,   21,
    QMetaType::Void, QMetaType::Float, QMetaType::Float,   16,   17,
    QMetaType::Void, QMetaType::Float, QMetaType::Float,   16,   17,
    QMetaType::Void, QMetaType::Float, QMetaType::Float, QMetaType::Float, QMetaType::Float,   11,   12,   13,   14,
    QMetaType::Void, QMetaType::Int,   28,
    QMetaType::Void, QMetaType::Bool, 0x80000000 | 31, 0x80000000 | 32, 0x80000000 | 32,   30,   28,   16,   17,
    QMetaType::Void, 0x80000000 | 34,   35,
    QMetaType::Void, 0x80000000 | 37,   38,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   43,

       0        // eod
};

void AWBStatWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AWBStatWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigWinClose(); break;
        case 1: _t->updateStatistic((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 2: _t->updateStatistic((*reinterpret_cast< QMap<QString,int>(*)>(_a[1]))); break;
        case 3: _t->exportStatistic(); break;
        case 4: _t->updateZoneSatPoints((*reinterpret_cast< float*(*)>(_a[1])),(*reinterpret_cast< float*(*)>(_a[2])),(*reinterpret_cast< float*(*)>(_a[3])),(*reinterpret_cast< float*(*)>(_a[4])),(*reinterpret_cast< s_AWB_DBG_S*(*)>(_a[5]))); break;
        case 5: _t->updateGlobalSatPoint((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3])),(*reinterpret_cast< float(*)>(_a[4]))); break;
        case 6: _t->updateResultPoint((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 7: _t->updateWhiteZoneCurvesMid((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float*(*)>(_a[2])),(*reinterpret_cast< float*(*)>(_a[3]))); break;
        case 8: _t->updateWhiteZoneCurvesTop((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float*(*)>(_a[2])),(*reinterpret_cast< float*(*)>(_a[3]))); break;
        case 9: _t->updateWhiteZoneCurvesBot((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float*(*)>(_a[2])),(*reinterpret_cast< float*(*)>(_a[3]))); break;
        case 10: _t->updateCurveLeftLimit((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 11: _t->updateCurveRightLimit((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 12: _t->updateSelectedPoint((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3])),(*reinterpret_cast< float(*)>(_a[4]))); break;
        case 13: _t->updateCalibrationStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->updateExtraLightSource((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int*(*)>(_a[2])),(*reinterpret_cast< float(*)[][25]>(_a[3])),(*reinterpret_cast< float(*)[][25]>(_a[4]))); break;
        case 15: _t->updateCalibPoint((*reinterpret_cast< int(*)[3][3]>(_a[1]))); break;
        case 16: _t->updateCurveRegion((*reinterpret_cast< int(*)[4]>(_a[1]))); break;
        case 17: _t->slotSetShowStatistics(); break;
        case 18: _t->slotSetShowPlanckianCurve(); break;
        case 19: _t->slotSetShowWhiteZone(); break;
        case 20: _t->slotCurrentTabChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (AWBStatWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AWBStatWindow::sigWinClose)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject AWBStatWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_AWBStatWindow.data,
    qt_meta_data_AWBStatWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *AWBStatWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AWBStatWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AWBStatWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int AWBStatWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 21)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 21;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 21)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 21;
    }
    return _id;
}

// SIGNAL 0
void AWBStatWindow::sigWinClose()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
