/****************************************************************************
** Meta object code from reading C++ file 'gbuffersCustom.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src-nodes/genericNodes/gbuffersCustom/gbuffersCustom.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qplugin.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gbuffersCustom.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_GbuffersCustomWidget_t {
    QByteArrayData data[4];
    char stringdata0[49];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GbuffersCustomWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GbuffersCustomWidget_t qt_meta_stringdata_GbuffersCustomWidget = {
    {
QT_MOC_LITERAL(0, 0, 20), // "GbuffersCustomWidget"
QT_MOC_LITERAL(1, 21, 11), // "loadClicked"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 14) // "defaultClicked"

    },
    "GbuffersCustomWidget\0loadClicked\0\0"
    "defaultClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GbuffersCustomWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x0a /* Public */,
       3,    0,   25,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void GbuffersCustomWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GbuffersCustomWidget *_t = static_cast<GbuffersCustomWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->loadClicked(); break;
        case 1: _t->defaultClicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject GbuffersCustomWidget::staticMetaObject = {
    { &GenericCustomWidget::staticMetaObject, qt_meta_stringdata_GbuffersCustomWidget.data,
      qt_meta_data_GbuffersCustomWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *GbuffersCustomWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GbuffersCustomWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_GbuffersCustomWidget.stringdata0))
        return static_cast<void*>(const_cast< GbuffersCustomWidget*>(this));
    return GenericCustomWidget::qt_metacast(_clname);
}

int GbuffersCustomWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = GenericCustomWidget::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_GbuffersCustomHandle_t {
    QByteArrayData data[1];
    char stringdata0[21];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GbuffersCustomHandle_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GbuffersCustomHandle_t qt_meta_stringdata_GbuffersCustomHandle = {
    {
QT_MOC_LITERAL(0, 0, 20) // "GbuffersCustomHandle"

    },
    "GbuffersCustomHandle"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GbuffersCustomHandle[] = {

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

void GbuffersCustomHandle::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject GbuffersCustomHandle::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_GbuffersCustomHandle.data,
      qt_meta_data_GbuffersCustomHandle,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *GbuffersCustomHandle::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GbuffersCustomHandle::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_GbuffersCustomHandle.stringdata0))
        return static_cast<void*>(const_cast< GbuffersCustomHandle*>(this));
    if (!strcmp(_clname, "NodeHandleInterface"))
        return static_cast< NodeHandleInterface*>(const_cast< GbuffersCustomHandle*>(this));
    if (!strcmp(_clname, "Gratin.NodeHandle/1.0"))
        return static_cast< NodeHandleInterface*>(const_cast< GbuffersCustomHandle*>(this));
    return QObject::qt_metacast(_clname);
}

int GbuffersCustomHandle::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
    0x15, 0x00, 'G',  'r',  'a',  't',  'i',  'n', 
    '.',  'G',  'b',  'u',  'f',  'f',  'e',  'r', 
    's',  'C',  'u',  's',  't',  'o',  'm',  0x00,
    0x1b, 0x08, 0x00, 0x00, 0x09, 0x00, 'c',  'l', 
    'a',  's',  's',  'N',  'a',  'm',  'e',  0x00,
    0x14, 0x00, 'G',  'b',  'u',  'f',  'f',  'e', 
    'r',  's',  'C',  'u',  's',  't',  'o',  'm', 
    'H',  'a',  'n',  'd',  'l',  'e',  0x00, 0x00,
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
    0x15, 0x00, 'G',  'r',  'a',  't',  'i',  'n', 
    '.',  'G',  'b',  'u',  'f',  'f',  'e',  'r', 
    's',  'C',  'u',  's',  't',  'o',  'm',  0x00,
    0x15, 0x08, 0x00, 0x00, 0x08, 0x00, 'M',  'e', 
    't',  'a',  'D',  'a',  't',  'a',  0x00, 0x00,
    0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x9b, 0x0b, 0x00, 0x00,
    0x09, 0x00, 'c',  'l',  'a',  's',  's',  'N', 
    'a',  'm',  'e',  0x00, 0x14, 0x00, 'G',  'b', 
    'u',  'f',  'f',  'e',  'r',  's',  'C',  'u', 
    's',  't',  'o',  'm',  'H',  'a',  'n',  'd', 
    'l',  'e',  0x00, 0x00, '1',  0x00, 0x00, 0x00,
    0x05, 0x00, 'd',  'e',  'b',  'u',  'g',  0x00,
    ':',  0xa0, 0xa0, 0x00, 0x07, 0x00, 'v',  'e', 
    'r',  's',  'i',  'o',  'n',  0x00, 0x00, 0x00,
    0x0c, 0x00, 0x00, 0x00, '0',  0x00, 0x00, 0x00,
    'L',  0x00, 0x00, 0x00, 't',  0x00, 0x00, 0x00,
    0x80, 0x00, 0x00, 0x00
};
#endif // QT_NO_DEBUG

QT_MOC_EXPORT_PLUGIN(GbuffersCustomHandle, GbuffersCustomHandle)

QT_END_MOC_NAMESPACE