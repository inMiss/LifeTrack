/****************************************************************************
** Meta object code from reading C++ file 'activeTaskPage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../panel/mainPanel/activeTaskPage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'activeTaskPage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CActiveTaskPage_t {
    QByteArrayData data[12];
    char stringdata0[139];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CActiveTaskPage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CActiveTaskPage_t qt_meta_stringdata_CActiveTaskPage = {
    {
QT_MOC_LITERAL(0, 0, 15), // "CActiveTaskPage"
QT_MOC_LITERAL(1, 16, 16), // "sigTaskOperation"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 7), // "task_id"
QT_MOC_LITERAL(4, 42, 4), // "nRow"
QT_MOC_LITERAL(5, 47, 14), // "taskActiveType"
QT_MOC_LITERAL(6, 62, 12), // "taskOperType"
QT_MOC_LITERAL(7, 75, 18), // "slotsButtonClicked"
QT_MOC_LITERAL(8, 94, 3), // "row"
QT_MOC_LITERAL(9, 98, 10), // "operatorId"
QT_MOC_LITERAL(10, 109, 23), // "slotsFilterComBoxChange"
QT_MOC_LITERAL(11, 133, 5) // "index"

    },
    "CActiveTaskPage\0sigTaskOperation\0\0"
    "task_id\0nRow\0taskActiveType\0taskOperType\0"
    "slotsButtonClicked\0row\0operatorId\0"
    "slotsFilterComBoxChange\0index"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CActiveTaskPage[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    2,   38,    2, 0x08 /* Private */,
      10,    1,   43,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::QString, QMetaType::QString,    3,    4,    5,    6,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    8,    9,
    QMetaType::Void, QMetaType::Int,   11,

       0        // eod
};

void CActiveTaskPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<CActiveTaskPage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sigTaskOperation((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 1: _t->slotsButtonClicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: _t->slotsFilterComBoxChange((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CActiveTaskPage::*)(const QString & , const int , const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CActiveTaskPage::sigTaskOperation)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CActiveTaskPage::staticMetaObject = { {
    QMetaObject::SuperData::link<CCustomWidget::staticMetaObject>(),
    qt_meta_stringdata_CActiveTaskPage.data,
    qt_meta_data_CActiveTaskPage,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *CActiveTaskPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CActiveTaskPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CActiveTaskPage.stringdata0))
        return static_cast<void*>(this);
    return CCustomWidget::qt_metacast(_clname);
}

int CActiveTaskPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CCustomWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void CActiveTaskPage::sigTaskOperation(const QString & _t1, const int _t2, const QString & _t3, const QString & _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
