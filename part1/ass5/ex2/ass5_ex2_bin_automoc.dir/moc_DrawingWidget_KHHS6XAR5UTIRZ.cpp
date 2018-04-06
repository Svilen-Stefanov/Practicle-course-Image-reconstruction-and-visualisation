/****************************************************************************
** Meta object code from reading C++ file 'DrawingWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/DrawingWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DrawingWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DrawingWidget_t {
    QByteArrayData data[12];
    char stringdata0[111];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DrawingWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DrawingWidget_t qt_meta_stringdata_DrawingWidget = {
    {
QT_MOC_LITERAL(0, 0, 13), // "DrawingWidget"
QT_MOC_LITERAL(1, 14, 12), // "colorReadout"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 5), // "color"
QT_MOC_LITERAL(4, 34, 8), // "setImage"
QT_MOC_LITERAL(5, 43, 8), // "newImage"
QT_MOC_LITERAL(6, 52, 7), // "setZoom"
QT_MOC_LITERAL(7, 60, 10), // "zoomFactor"
QT_MOC_LITERAL(8, 71, 11), // "setPenColor"
QT_MOC_LITERAL(9, 83, 8), // "newColor"
QT_MOC_LITERAL(10, 92, 10), // "setPenSize"
QT_MOC_LITERAL(11, 103, 7) // "penSize"

    },
    "DrawingWidget\0colorReadout\0\0color\0"
    "setImage\0newImage\0setZoom\0zoomFactor\0"
    "setPenColor\0newColor\0setPenSize\0penSize"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DrawingWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   42,    2, 0x0a /* Public */,
       6,    1,   45,    2, 0x0a /* Public */,
       8,    1,   48,    2, 0x0a /* Public */,
      10,    1,   51,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QColor,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QImage,    5,
    QMetaType::Void, QMetaType::Double,    7,
    QMetaType::Void, QMetaType::QColor,    9,
    QMetaType::Void, QMetaType::Int,   11,

       0        // eod
};

void DrawingWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DrawingWidget *_t = static_cast<DrawingWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->colorReadout((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 1: _t->setImage((*reinterpret_cast< const QImage(*)>(_a[1]))); break;
        case 2: _t->setZoom((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->setPenColor((*reinterpret_cast< const QColor(*)>(_a[1]))); break;
        case 4: _t->setPenSize((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (DrawingWidget::*_t)(const QColor & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DrawingWidget::colorReadout)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject DrawingWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_DrawingWidget.data,
      qt_meta_data_DrawingWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *DrawingWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DrawingWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_DrawingWidget.stringdata0))
        return static_cast<void*>(const_cast< DrawingWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int DrawingWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void DrawingWidget::colorReadout(const QColor & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
