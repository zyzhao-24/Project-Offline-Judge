/****************************************************************************
** Meta object code from reading C++ file 'contesteditor.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../contesteditor.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'contesteditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN13ContestEditorE_t {};
} // unnamed namespace

template <> constexpr inline auto ContestEditor::qt_create_metaobjectdata<qt_meta_tag_ZN13ContestEditorE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ContestEditor",
        "on_refreshbtn_clicked",
        "",
        "on_savebtn_clicked",
        "on_returnbtn_clicked",
        "on_setnamebtn_clicked",
        "on_dstchkbox_checkStateChanged",
        "Qt::CheckState",
        "arg1",
        "on_dedchkbox_checkStateChanged",
        "on_settimebtn_clicked",
        "on_addbtn_clicked",
        "on_rembtn_clicked",
        "on_impbtn_clicked",
        "on_clearbtn_clicked",
        "on_expbtn_clicked",
        "on_setbtn_clicked",
        "on_addprobbtn_clicked",
        "on_renprobbtn_clicked",
        "on_remprobbtn_clicked",
        "on_problistwid_currentTextChanged",
        "currentText",
        "on_confprobbtn_clicked",
        "on_gradebtn_clicked",
        "on_stusavebtn_clicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'on_refreshbtn_clicked'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_savebtn_clicked'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_returnbtn_clicked'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_setnamebtn_clicked'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_dstchkbox_checkStateChanged'
        QtMocHelpers::SlotData<void(const Qt::CheckState &)>(6, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 7, 8 },
        }}),
        // Slot 'on_dedchkbox_checkStateChanged'
        QtMocHelpers::SlotData<void(const Qt::CheckState &)>(9, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 7, 8 },
        }}),
        // Slot 'on_settimebtn_clicked'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_addbtn_clicked'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_rembtn_clicked'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_impbtn_clicked'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_clearbtn_clicked'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_expbtn_clicked'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_setbtn_clicked'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_addprobbtn_clicked'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_renprobbtn_clicked'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_remprobbtn_clicked'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_problistwid_currentTextChanged'
        QtMocHelpers::SlotData<void(const QString &)>(20, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 21 },
        }}),
        // Slot 'on_confprobbtn_clicked'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_gradebtn_clicked'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_stusavebtn_clicked'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ContestEditor, qt_meta_tag_ZN13ContestEditorE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ContestEditor::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13ContestEditorE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13ContestEditorE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13ContestEditorE_t>.metaTypes,
    nullptr
} };

void ContestEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ContestEditor *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->on_refreshbtn_clicked(); break;
        case 1: _t->on_savebtn_clicked(); break;
        case 2: _t->on_returnbtn_clicked(); break;
        case 3: _t->on_setnamebtn_clicked(); break;
        case 4: _t->on_dstchkbox_checkStateChanged((*reinterpret_cast< std::add_pointer_t<Qt::CheckState>>(_a[1]))); break;
        case 5: _t->on_dedchkbox_checkStateChanged((*reinterpret_cast< std::add_pointer_t<Qt::CheckState>>(_a[1]))); break;
        case 6: _t->on_settimebtn_clicked(); break;
        case 7: _t->on_addbtn_clicked(); break;
        case 8: _t->on_rembtn_clicked(); break;
        case 9: _t->on_impbtn_clicked(); break;
        case 10: _t->on_clearbtn_clicked(); break;
        case 11: _t->on_expbtn_clicked(); break;
        case 12: _t->on_setbtn_clicked(); break;
        case 13: _t->on_addprobbtn_clicked(); break;
        case 14: _t->on_renprobbtn_clicked(); break;
        case 15: _t->on_remprobbtn_clicked(); break;
        case 16: _t->on_problistwid_currentTextChanged((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 17: _t->on_confprobbtn_clicked(); break;
        case 18: _t->on_gradebtn_clicked(); break;
        case 19: _t->on_stusavebtn_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject *ContestEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ContestEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13ContestEditorE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int ContestEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 20)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 20;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 20)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 20;
    }
    return _id;
}
QT_WARNING_POP
