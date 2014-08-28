/****************************************************************************
** Meta object code from reading C++ file 'logwindow.h'
**
** Created: Sat Nov 30 14:24:35 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "logwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'logwindow.h' doesn't include <QObject>."
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
static const uint qt_meta_data_Client[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,    8,    7,    7, 0x05,

 // slots: signature, parameters, type, tag, flags
      35,    7,    7,    7, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Client[] = {
    "Client\0\0str\0print_log(const char*)\0"
    "continueLog()\0"
};

const QMetaObject Client::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_Client,
      qt_meta_data_Client, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Client::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Client::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Client::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Client))
        return static_cast<void*>(const_cast< Client*>(this));
    return QThread::qt_metacast(_clname);
}

int Client::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: print_log((*reinterpret_cast< const char*(*)>(_a[1]))); break;
        case 1: continueLog(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void Client::print_log(const char * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_logWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      25,   10,   10,   10, 0x0a,
      49,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_logWindow[] = {
    "logWindow\0\0log_success()\0"
    "appendText(const char*)\0crossClicked()\0"
};

const QMetaObject logWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_logWindow,
      qt_meta_data_logWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &logWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *logWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *logWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_logWindow))
        return static_cast<void*>(const_cast< logWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int logWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: log_success(); break;
        case 1: appendText((*reinterpret_cast< const char*(*)>(_a[1]))); break;
        case 2: crossClicked(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void logWindow::log_success()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
