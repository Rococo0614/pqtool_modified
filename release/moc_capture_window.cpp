/****************************************************************************
** Meta object code from reading C++ file 'capture_window.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/component/window/capture_window.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'capture_window.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CaptureWindow_t {
    QByteArrayData data[36];
    char stringdata0[489];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CaptureWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CaptureWindow_t qt_meta_stringdata_CaptureWindow = {
    {
QT_MOC_LITERAL(0, 0, 13), // "CaptureWindow"
QT_MOC_LITERAL(1, 14, 16), // "sigCaptureRawEnd"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 16), // "sigCaptureYuvEnd"
QT_MOC_LITERAL(4, 49, 14), // "fetchCompleted"
QT_MOC_LITERAL(5, 64, 10), // "fetchStart"
QT_MOC_LITERAL(6, 75, 12), // "sigStatusLog"
QT_MOC_LITERAL(7, 88, 7), // "message"
QT_MOC_LITERAL(8, 96, 9), // "LOG_LEVEL"
QT_MOC_LITERAL(9, 106, 9), // "log_level"
QT_MOC_LITERAL(10, 116, 13), // "handleDumpAll"
QT_MOC_LITERAL(11, 130, 12), // "DUMPALL_MODE"
QT_MOC_LITERAL(12, 143, 4), // "mode"
QT_MOC_LITERAL(13, 148, 7), // "dirPath"
QT_MOC_LITERAL(14, 156, 8), // "fileName"
QT_MOC_LITERAL(15, 165, 21), // "handleReadAllFinished"
QT_MOC_LITERAL(16, 187, 19), // "handleCaptureYuvImg"
QT_MOC_LITERAL(17, 207, 20), // "handleMultiYuvStatus"
QT_MOC_LITERAL(18, 228, 2), // "id"
QT_MOC_LITERAL(19, 231, 19), // "handleCaptureRawImg"
QT_MOC_LITERAL(20, 251, 20), // "handleMultiRawStatus"
QT_MOC_LITERAL(21, 272, 17), // "handleRecvBinInfo"
QT_MOC_LITERAL(22, 290, 17), // "handleRecvBinData"
QT_MOC_LITERAL(23, 308, 3), // "cmd"
QT_MOC_LITERAL(24, 312, 17), // "handleCancelEvent"
QT_MOC_LITERAL(25, 330, 18), // "slotCheckRawParams"
QT_MOC_LITERAL(26, 349, 4), // "text"
QT_MOC_LITERAL(27, 354, 16), // "OpenSaveFilePath"
QT_MOC_LITERAL(28, 371, 17), // "CheckSaveFilePath"
QT_MOC_LITERAL(29, 389, 13), // "slotGotYuvImg"
QT_MOC_LITERAL(30, 403, 13), // "slotGotRawImg"
QT_MOC_LITERAL(31, 417, 10), // "cellEdited"
QT_MOC_LITERAL(32, 428, 3), // "idx"
QT_MOC_LITERAL(33, 432, 16), // "startTimingEvent"
QT_MOC_LITERAL(34, 449, 15), // "stopTimingEvent"
QT_MOC_LITERAL(35, 465, 23) // "startTimingCaptureEvent"

    },
    "CaptureWindow\0sigCaptureRawEnd\0\0"
    "sigCaptureYuvEnd\0fetchCompleted\0"
    "fetchStart\0sigStatusLog\0message\0"
    "LOG_LEVEL\0log_level\0handleDumpAll\0"
    "DUMPALL_MODE\0mode\0dirPath\0fileName\0"
    "handleReadAllFinished\0handleCaptureYuvImg\0"
    "handleMultiYuvStatus\0id\0handleCaptureRawImg\0"
    "handleMultiRawStatus\0handleRecvBinInfo\0"
    "handleRecvBinData\0cmd\0handleCancelEvent\0"
    "slotCheckRawParams\0text\0OpenSaveFilePath\0"
    "CheckSaveFilePath\0slotGotYuvImg\0"
    "slotGotRawImg\0cellEdited\0idx\0"
    "startTimingEvent\0stopTimingEvent\0"
    "startTimingCaptureEvent"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CaptureWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      26,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  144,    2, 0x06 /* Public */,
       3,    0,  145,    2, 0x06 /* Public */,
       4,    0,  146,    2, 0x06 /* Public */,
       5,    0,  147,    2, 0x06 /* Public */,
       6,    2,  148,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    3,  153,    2, 0x0a /* Public */,
      10,    2,  160,    2, 0x2a /* Public | MethodCloned */,
      10,    1,  165,    2, 0x2a /* Public | MethodCloned */,
      10,    0,  168,    2, 0x2a /* Public | MethodCloned */,
      15,    0,  169,    2, 0x0a /* Public */,
      16,    0,  170,    2, 0x08 /* Private */,
      17,    1,  171,    2, 0x08 /* Private */,
      19,    0,  174,    2, 0x08 /* Private */,
      20,    1,  175,    2, 0x08 /* Private */,
      21,    1,  178,    2, 0x08 /* Private */,
      22,    1,  181,    2, 0x08 /* Private */,
      24,    0,  184,    2, 0x08 /* Private */,
      25,    1,  185,    2, 0x08 /* Private */,
      27,    0,  188,    2, 0x08 /* Private */,
      28,    0,  189,    2, 0x08 /* Private */,
      29,    1,  190,    2, 0x08 /* Private */,
      30,    1,  193,    2, 0x08 /* Private */,
      31,    1,  196,    2, 0x08 /* Private */,
      33,    0,  199,    2, 0x08 /* Private */,
      34,    0,  200,    2, 0x08 /* Private */,
      35,    0,  201,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 8,    7,    9,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 11, QMetaType::QString, QMetaType::QString,   12,   13,   14,
    QMetaType::Void, 0x80000000 | 11, QMetaType::QString,   12,   13,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void, QMetaType::Int,   18,
    QMetaType::Void, QMetaType::QString,   23,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   26,
    QMetaType::Void,
    QMetaType::Bool,
    QMetaType::Void, QMetaType::QString,   23,
    QMetaType::Void, QMetaType::QString,   23,
    QMetaType::Void, QMetaType::Int,   32,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CaptureWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CaptureWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigCaptureRawEnd(); break;
        case 1: _t->sigCaptureYuvEnd(); break;
        case 2: _t->fetchCompleted(); break;
        case 3: _t->fetchStart(); break;
        case 4: _t->sigStatusLog((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< LOG_LEVEL(*)>(_a[2]))); break;
        case 5: _t->handleDumpAll((*reinterpret_cast< DUMPALL_MODE(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 6: _t->handleDumpAll((*reinterpret_cast< DUMPALL_MODE(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 7: _t->handleDumpAll((*reinterpret_cast< DUMPALL_MODE(*)>(_a[1]))); break;
        case 8: _t->handleDumpAll(); break;
        case 9: _t->handleReadAllFinished(); break;
        case 10: _t->handleCaptureYuvImg(); break;
        case 11: _t->handleMultiYuvStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->handleCaptureRawImg(); break;
        case 13: _t->handleMultiRawStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->handleRecvBinInfo((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->handleRecvBinData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 16: _t->handleCancelEvent(); break;
        case 17: _t->slotCheckRawParams((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 18: _t->OpenSaveFilePath(); break;
        case 19: { bool _r = _t->CheckSaveFilePath();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 20: _t->slotGotYuvImg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 21: _t->slotGotRawImg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 22: _t->cellEdited((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: _t->startTimingEvent(); break;
        case 24: _t->stopTimingEvent(); break;
        case 25: _t->startTimingCaptureEvent(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CaptureWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CaptureWindow::sigCaptureRawEnd)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CaptureWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CaptureWindow::sigCaptureYuvEnd)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (CaptureWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CaptureWindow::fetchCompleted)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (CaptureWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CaptureWindow::fetchStart)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (CaptureWindow::*)(QString , LOG_LEVEL );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CaptureWindow::sigStatusLog)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CaptureWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CaptureWindow.data,
    qt_meta_data_CaptureWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CaptureWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CaptureWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CaptureWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int CaptureWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 26)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 26;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 26)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 26;
    }
    return _id;
}

// SIGNAL 0
void CaptureWindow::sigCaptureRawEnd()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void CaptureWindow::sigCaptureYuvEnd()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void CaptureWindow::fetchCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void CaptureWindow::fetchStart()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void CaptureWindow::sigStatusLog(QString _t1, LOG_LEVEL _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
