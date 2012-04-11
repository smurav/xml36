/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Wed Apr 11 00:57:26 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/xml36/mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      58,   38,   11,   11, 0x08,
     110,  105,   11,   11, 0x08,
     153,  105,   11,   11, 0x08,
     197,   11,   11,   11, 0x08,
     225,   11,   11,   11, 0x08,
     251,   11,   11,   11, 0x08,
     297,  285,   11,   11, 0x08,
     343,  285,   11,   11, 0x08,
     402,   11,   11,   11, 0x08,
     428,   11,   11,   11, 0x08,
     465,  285,   11,   11, 0x08,
     518,  285,   11,   11, 0x08,
     571,   11,   11,   11, 0x08,
     600,  285,   11,   11, 0x08,
     652,   11,   11,   11, 0x08,
     684,  285,   11,   11, 0x08,
     736,  730,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0on_actionOpen_triggered()\0"
    "clicked_item,column\0"
    "OnNodeQTreeWidgetPressed(QTreeWidgetItem*,int)\0"
    "item\0on_xml_tree_itemExpanded(QTreeWidgetItem*)\0"
    "on_xml_tree_itemCollapsed(QTreeWidgetItem*)\0"
    "on_actionSaveAs_triggered()\0"
    "on_actionSave_triggered()\0"
    "on_actionAddAttribute_triggered()\0"
    "item,column\0on_xml_tree_itemClicked(QTreeWidgetItem*,int)\0"
    "on_attributes_list_itemDoubleClicked(QTreeWidgetItem*,int)\0"
    "on_actionEdit_triggered()\0"
    "on_actionRemoveAttribute_triggered()\0"
    "on_attributes_list_itemClicked(QTreeWidgetItem*,int)\0"
    "on_attributes_list_itemChanged(QTreeWidgetItem*,int)\0"
    "on_actionAddNode_triggered()\0"
    "on_xml_tree_itemDoubleClicked(QTreeWidgetItem*,int)\0"
    "on_actionRemoveNode_triggered()\0"
    "on_xml_tree_itemChanged(QTreeWidgetItem*,int)\0"
    "index\0on_xml_tree_clicked(QModelIndex)\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->on_actionOpen_triggered(); break;
        case 1: _t->OnNodeQTreeWidgetPressed((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->on_xml_tree_itemExpanded((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 3: _t->on_xml_tree_itemCollapsed((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 4: _t->on_actionSaveAs_triggered(); break;
        case 5: _t->on_actionSave_triggered(); break;
        case 6: _t->on_actionAddAttribute_triggered(); break;
        case 7: _t->on_xml_tree_itemClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 8: _t->on_attributes_list_itemDoubleClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: _t->on_actionEdit_triggered(); break;
        case 10: _t->on_actionRemoveAttribute_triggered(); break;
        case 11: _t->on_attributes_list_itemClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 12: _t->on_attributes_list_itemChanged((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 13: _t->on_actionAddNode_triggered(); break;
        case 14: _t->on_xml_tree_itemDoubleClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 15: _t->on_actionRemoveNode_triggered(); break;
        case 16: _t->on_xml_tree_itemChanged((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 17: _t->on_xml_tree_clicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
