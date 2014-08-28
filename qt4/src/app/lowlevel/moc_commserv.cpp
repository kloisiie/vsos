/****************************************************************************
** Meta object code from reading C++ file 'commserv.h'
**
** Created: Sat Nov 30 14:24:31 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "commserv.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'commserv.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TitleBar[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x0a,
      22,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_TitleBar[] = {
    "TitleBar\0\0showSmall()\0showMaxRestore()\0"
};

const QMetaObject TitleBar::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_TitleBar,
      qt_meta_data_TitleBar, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TitleBar::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TitleBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TitleBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TitleBar))
        return static_cast<void*>(const_cast< TitleBar*>(this));
    return QWidget::qt_metacast(_clname);
}

int TitleBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: showSmall(); break;
        case 1: showMaxRestore(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
static const uint qt_meta_data_CommServDialog[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,
      34,   31,   15,   15, 0x08,
      68,   61,   15,   15, 0x08,
     100,   15,   15,   15, 0x08,
     114,   15,   15,   15, 0x08,
     129,   15,   15,   15, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CommServDialog[] = {
    "CommServDialog\0\0crossClicked()\0id\0"
    "enableWriteButton(QString)\0nameId\0"
    "enableRead_writeButton(QString)\0"
    "readClicked()\0writeClicked()\0"
    "read_writeClicked()\0"
};

const QMetaObject CommServDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CommServDialog,
      qt_meta_data_CommServDialog, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CommServDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CommServDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CommServDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CommServDialog))
        return static_cast<void*>(const_cast< CommServDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int CommServDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: crossClicked(); break;
        case 1: enableWriteButton((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: enableRead_writeButton((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: readClicked(); break;
        case 4: writeClicked(); break;
        case 5: read_writeClicked(); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
