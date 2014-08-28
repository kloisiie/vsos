/****************************************************************************
** Meta object code from reading C++ file 'verify.h'
**
** Created: Thu Jan 2 08:20:02 2014
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "verify.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'verify.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LoginDialog[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,
      28,   12,   12,   12, 0x08,
      44,   12,   12,   12, 0x08,
      65,   60,   12,   12, 0x08,
      92,   60,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_LoginDialog[] = {
    "LoginDialog\0\0loginClicked()\0cancelClicked()\0"
    "changeClicked()\0pass\0enableLoginButton(QString)\0"
    "enableChangeButton(QString)\0"
};

const QMetaObject LoginDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_LoginDialog,
      qt_meta_data_LoginDialog, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LoginDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LoginDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LoginDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LoginDialog))
        return static_cast<void*>(const_cast< LoginDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int LoginDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: loginClicked(); break;
        case 1: cancelClicked(); break;
        case 2: changeClicked(); break;
        case 3: enableLoginButton((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: enableChangeButton((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
