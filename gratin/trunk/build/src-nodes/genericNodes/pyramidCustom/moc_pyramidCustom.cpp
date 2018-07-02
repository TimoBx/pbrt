/****************************************************************************
** Meta object code from reading C++ file 'pyramidCustom.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src-nodes/genericNodes/pyramidCustom/pyramidCustom.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qplugin.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pyramidCustom.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PyramidCustomWidget_t {
    QByteArrayData data[3];
    char stringdata0[33];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PyramidCustomWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PyramidCustomWidget_t qt_meta_stringdata_PyramidCustomWidget = {
    {
QT_MOC_LITERAL(0, 0, 19), // "PyramidCustomWidget"
QT_MOC_LITERAL(1, 20, 11), // "modeChanged"
QT_MOC_LITERAL(2, 32, 0) // ""

    },
    "PyramidCustomWidget\0modeChanged\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PyramidCustomWidget[] = {

 // content:
       7,       // revision
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
    QMetaType::Void, QMetaType::Int,    2,

       0        // eod
};

void PyramidCustomWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PyramidCustomWidget *_t = static_cast<PyramidCustomWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->modeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject PyramidCustomWidget::staticMetaObject = {
    { &GenericCustomWidget::staticMetaObject, qt_meta_stringdata_PyramidCustomWidget.data,
      qt_meta_data_PyramidCustomWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *PyramidCustomWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PyramidCustomWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_PyramidCustomWidget.stringdata0))
        return static_cast<void*>(const_cast< PyramidCustomWidget*>(this));
    return GenericCustomWidget::qt_metacast(_clname);
}

int PyramidCustomWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = GenericCustomWidget::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_PyramidCustomHandle_t {
    QByteArrayData data[1];
    char stringdata0[20];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PyramidCustomHandle_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PyramidCustomHandle_t qt_meta_stringdata_PyramidCustomHandle = {
    {
QT_MOC_LITERAL(0, 0, 19) // "PyramidCustomHandle"

    },
    "PyramidCustomHandle"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PyramidCustomHandle[] = {

 // content:
       7,       // revision
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

void PyramidCustomHandle::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject PyramidCustomHandle::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_PyramidCustomHandle.data,
      qt_meta_data_PyramidCustomHandle,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *PyramidCustomHandle::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PyramidCustomHandle::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_PyramidCustomHandle.stringdata0))
        return static_cast<void*>(const_cast< PyramidCustomHandle*>(this));
    if (!strcmp(_clname, "NodeHandleInterface"))
        return static_cast< NodeHandleInterface*>(const_cast< PyramidCustomHandle*>(this));
    if (!strcmp(_clname, "Gratin.NodeHandle/1.0"))
        return static_cast< NodeHandleInterface*>(const_cast< PyramidCustomHandle*>(this));
    return QObject::qt_metacast(_clname);
}

int PyramidCustomHandle::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}

QT_PLUGIN_METADATA_SECTION const uint qt_section_alignment_dummy = 42;

#ifdef QT_NO_DEBUG

QT_PLUGIN_METADATA_SECTION
static const unsigned char qt_pluginMetaData[] = {
    'Q', 'T', 'M', 'E', 'T', 'A', 'D', 'A', 'T', 'A', ' ', ' ',
    'q',  'b',  'j',  's',  0x01, 0x00, 0x00, 0x00,
    0xa4, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00,
    0x90, 0x00, 0x00, 0x00, 0x1b, 0x03, 0x00, 0x00,
    0x03, 0x00, 'I',  'I',  'D',  0x00, 0x00, 0x00,
    0x14, 0x00, 'G',  'r',  'a',  't',  'i',  'n', 
    '.',  'P',  'y',  'r',  'a',  'm',  'i',  'd', 
    'C',  'u',  's',  't',  'o',  'm',  0x00, 0x00,
    0x1b, 0x08, 0x00, 0x00, 0x09, 0x00, 'c',  'l', 
    'a',  's',  's',  'N',  'a',  'm',  'e',  0x00,
    0x13, 0x00, 'P',  'y',  'r',  'a',  'm',  'i', 
    'd',  'C',  'u',  's',  't',  'o',  'm',  'H', 
    'a',  'n',  'd',  'l',  'e',  0x00, 0x00, 0x00,
    ':',  0xa0, 0xa0, 0x00, 0x07, 0x00, 'v',  'e', 
    'r',  's',  'i',  'o',  'n',  0x00, 0x00, 0x00,
    0x11, 0x00, 0x00, 0x00, 0x05, 0x00, 'd',  'e', 
    'b',  'u',  'g',  0x00, 0x95, 0x10, 0x00, 0x00,
    0x08, 0x00, 'M',  'e',  't',  'a',  'D',  'a', 
    't',  'a',  0x00, 0x00, 0x0c, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0c, 0x00, 0x00, 0x00, 't',  0x00, 0x00, 0x00,
    '0',  0x00, 0x00, 0x00, 'h',  0x00, 0x00, 0x00,
    'X',  0x00, 0x00, 0x00
};

#else // QT_NO_DEBUG

QT_PLUGIN_METADATA_SECTION
static const unsigned char qt_pluginMetaData[] = {
    'Q', 'T', 'M', 'E', 'T', 'A', 'D', 'A', 'T', 'A', ' ', ' ',
    'q',  'b',  'j',  's',  0x01, 0x00, 0x00, 0x00,
    0xa4, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x00, 0x00,
    0x90, 0x00, 0x00, 0x00, 0x1b, 0x03, 0x00, 0x00,
    0x03, 0x00, 'I',  'I',  'D',  0x00, 0x00, 0x00,
    0x14, 0x00, 'G',  'r',  'a',  't',  'i',  'n', 
    '.',  'P',  'y',  'r',  'a',  'm',  'i',  'd', 
    'C',  'u',  's',  't',  'o',  'm',  0x00, 0x00,
    0x15, 0x08, 0x00, 0x00, 0x08, 0x00, 'M',  'e', 
    't',  'a',  'D',  'a',  't',  'a',  0x00, 0x00,
    0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x9b, 0x0b, 0x00, 0x00,
    0x09, 0x00, 'c',  'l',  'a',  's',  's',  'N', 
    'a',  'm',  'e',  0x00, 0x13, 0x00, 'P',  'y', 
    'r',  'a',  'm',  'i',  'd',  'C',  'u',  's', 
    't',  'o',  'm',  'H',  'a',  'n',  'd',  'l', 
    'e',  0x00, 0x00, 0x00, '1',  0x00, 0x00, 0x00,
    0x05, 0x00, 'd',  'e',  'b',  'u',  'g',  0x00,
    ':',  0xa0, 0xa0, 0x00, 0x07, 0x00, 'v',  'e', 
    'r',  's',  'i',  'o',  'n',  0x00, 0x00, 0x00,
    0x0c, 0x00, 0x00, 0x00, '0',  0x00, 0x00, 0x00,
    'L',  0x00, 0x00, 0x00, 't',  0x00, 0x00, 0x00,
    0x80, 0x00, 0x00, 0x00
};
#endif // QT_NO_DEBUG

QT_MOC_EXPORT_PLUGIN(PyramidCustomHandle, PyramidCustomHandle)

QT_END_MOC_NAMESPACE
