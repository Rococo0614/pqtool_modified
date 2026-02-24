/****************************************************************************
** Meta object code from reading C++ file 'bracketing_window.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/component/window/bracketing_window.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bracketing_window.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BracketingWindow_t {
    QByteArrayData data[18];
    char stringdata0[274];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BracketingWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BracketingWindow_t qt_meta_stringdata_BracketingWindow = {
    {
QT_MOC_LITERAL(0, 0, 16), // "BracketingWindow"
QT_MOC_LITERAL(1, 17, 16), // "sigCaptureRawEnd"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 10), // "sigDumpEnd"
QT_MOC_LITERAL(4, 46, 16), // "handleGetBracket"
QT_MOC_LITERAL(5, 63, 19), // "handleBracketStatus"
QT_MOC_LITERAL(6, 83, 2), // "id"
QT_MOC_LITERAL(7, 86, 17), // "handleCancelEvent"
QT_MOC_LITERAL(8, 104, 16), // "OpenSaveFilePath"
QT_MOC_LITERAL(9, 121, 17), // "handleBracketData"
QT_MOC_LITERAL(10, 139, 3), // "cmd"
QT_MOC_LITERAL(11, 143, 19), // "handleSingleYuvData"
QT_MOC_LITERAL(12, 163, 21), // "handleSingleRawStatus"
QT_MOC_LITERAL(13, 185, 19), // "handleSingleRawData"
QT_MOC_LITERAL(14, 205, 10), // "handledump"
QT_MOC_LITERAL(15, 216, 17), // "handleRecvBinInfo"
QT_MOC_LITERAL(16, 234, 17), // "handleRecvBinData"
QT_MOC_LITERAL(17, 252, 21) // "handleSingleYuvStatus"

    },
    "BracketingWindow\0sigCaptureRawEnd\0\0"
    "sigDumpEnd\0handleGetBracket\0"
    "handleBracketStatus\0id\0handleCancelEvent\0"
    "OpenSaveFilePath\0handleBracketData\0"
    "cmd\0handleSingleYuvData\0handleSingleRawStatus\0"
    "handleSingleRawData\0handledump\0"
    "handleRecvBinInfo\0handleRecvBinData\0"
    "handleSingleYuvStatus"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BracketingWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x06 /* Public */,
       3,    0,   85,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   86,    2, 0x08 /* Private */,
       5,    1,   87,    2, 0x08 /* Private */,
       7,    0,   90,    2, 0x08 /* Private */,
       8,    0,   91,    2, 0x08 /* Private */,
       9,    1,   92,    2, 0x08 /* Private */,
      11,    1,   95,    2, 0x08 /* Private */,
      12,    1,   98,    2, 0x08 /* Private */,
      13,    1,  101,    2, 0x08 /* Private */,
      14,    0,  104,    2, 0x08 /* Private */,
      15,    1,  105,    2, 0x08 /* Private */,
      16,    1,  108,    2, 0x08 /* Private */,
      17,    1,  111,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::Int,    6,

       0        // eod
};

void BracketingWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<BracketingWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigCaptureRawEnd(); break;
        case 1: _t->sigDumpEnd(); break;
        case 2: _t->handleGetBracket(); break;
        case 3: _t->handleBracketStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->handleCancelEvent(); break;
        case 5: _t->OpenSaveFilePath(); break;
        case 6: _t->handleBracketData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->handleSingleYuvData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->handleSingleRawStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->handleSingleRawData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->handledump(); break;
        case 11: _t->handleRecvBinInfo((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->handleRecvBinData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: _t->handleSingleYuvStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (BracketingWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BracketingWindow::sigCaptureRawEnd)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (BracketingWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BracketingWindow::sigDumpEnd)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject BracketingWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_BracketingWindow.data,
    qt_meta_data_BracketingWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *BracketingWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BracketingWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BracketingWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int BracketingWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void BracketingWindow::sigCaptureRawEnd()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void BracketingWindow::sigDumpEnd()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
