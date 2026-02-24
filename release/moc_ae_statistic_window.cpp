/****************************************************************************
** Meta object code from reading C++ file 'ae_statistic_window.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../view/include/component/window/ae_statistic_window.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ae_statistic_window.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AEStatWindow_t {
    QByteArrayData data[9];
    char stringdata0[125];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AEStatWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AEStatWindow_t qt_meta_stringdata_AEStatWindow = {
    {
QT_MOC_LITERAL(0, 0, 12), // "AEStatWindow"
QT_MOC_LITERAL(1, 13, 11), // "sigWinClose"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 15), // "updateStatistic"
QT_MOC_LITERAL(4, 42, 10), // "updateHist"
QT_MOC_LITERAL(5, 53, 27), // "QMap<QString,QVariantList>&"
QT_MOC_LITERAL(6, 81, 8), // "hist_map"
QT_MOC_LITERAL(7, 90, 18), // "QMap<QString,int>&"
QT_MOC_LITERAL(8, 109, 15) // "exportStatistic"

    },
    "AEStatWindow\0sigWinClose\0\0updateStatistic\0"
    "updateHist\0QMap<QString,QVariantList>&\0"
    "hist_map\0QMap<QString,int>&\0exportStatistic"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AEStatWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   40,    2, 0x0a /* Public */,
       4,    1,   43,    2, 0x0a /* Public */,
       3,    1,   46,    2, 0x0a /* Public */,
       8,    0,   49,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QVariant,    2,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 7,    2,
    QMetaType::Void,

       0        // eod
};

void AEStatWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AEStatWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigWinClose(); break;
        case 1: _t->updateStatistic((*reinterpret_cast< QVariant(*)>(_a[1]))); break;
        case 2: _t->updateHist((*reinterpret_cast< QMap<QString,QVariantList>(*)>(_a[1]))); break;
        case 3: _t->updateStatistic((*reinterpret_cast< QMap<QString,int>(*)>(_a[1]))); break;
        case 4: _t->exportStatistic(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (AEStatWindow::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AEStatWindow::sigWinClose)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject AEStatWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_AEStatWindow.data,
    qt_meta_data_AEStatWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *AEStatWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AEStatWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AEStatWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int AEStatWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void AEStatWindow::sigWinClose()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
