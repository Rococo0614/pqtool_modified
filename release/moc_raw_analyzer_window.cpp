/****************************************************************************
** Meta object code from reading C++ file 'raw_analyzer_window.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/component/window/raw_analyzer_window.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'raw_analyzer_window.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RawAnalyzerWindow_t {
    QByteArrayData data[6];
    char stringdata0[92];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RawAnalyzerWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RawAnalyzerWindow_t qt_meta_stringdata_RawAnalyzerWindow = {
    {
QT_MOC_LITERAL(0, 0, 17), // "RawAnalyzerWindow"
QT_MOC_LITERAL(1, 18, 11), // "sigLabImgOk"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 19), // "sigcloseChildWindow"
QT_MOC_LITERAL(4, 51, 18), // "handleSaveDstImage"
QT_MOC_LITERAL(5, 70, 21) // "updateImageParameters"

    },
    "RawAnalyzerWindow\0sigLabImgOk\0\0"
    "sigcloseChildWindow\0handleSaveDstImage\0"
    "updateImageParameters"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RawAnalyzerWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,
       3,    0,   35,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   36,    2, 0x08 /* Private */,
       5,    0,   37,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void RawAnalyzerWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RawAnalyzerWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigLabImgOk(); break;
        case 1: _t->sigcloseChildWindow(); break;
        case 2: _t->handleSaveDstImage(); break;
        case 3: _t->updateImageParameters(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (RawAnalyzerWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RawAnalyzerWindow::sigLabImgOk)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (RawAnalyzerWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RawAnalyzerWindow::sigcloseChildWindow)) {
                *result = 1;
                return;
            }
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject RawAnalyzerWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_RawAnalyzerWindow.data,
    qt_meta_data_RawAnalyzerWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RawAnalyzerWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RawAnalyzerWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RawAnalyzerWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int RawAnalyzerWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void RawAnalyzerWindow::sigLabImgOk()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void RawAnalyzerWindow::sigcloseChildWindow()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
