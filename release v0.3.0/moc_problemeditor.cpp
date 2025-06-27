/****************************************************************************
** Meta object code from reading C++ file 'problemeditor.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../problemeditor.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'problemeditor.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN13ProblemEditorE_t {};
} // unnamed namespace

template <> constexpr inline auto ProblemEditor::qt_create_metaobjectdata<qt_meta_tag_ZN13ProblemEditorE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ProblemEditor",
        "ExitWin",
        "",
        "on_refreshbtn_clicked",
        "on_tlspinbox_valueChanged",
        "arg1",
        "on_mlspinbox_valueChanged",
        "on_probtypecombo_activated",
        "index",
        "on_importpdfbtn_clicked",
        "on_exportpdfbtn_clicked",
        "on_loadpdfbtn_clicked",
        "on_closepdfbtn_clicked",
        "on_srcaddbtn_clicked",
        "on_srcrembtn_clicked",
        "on_jutilswid_itemClicked",
        "QListWidgetItem*",
        "item",
        "on_jutilsaddbtn_clicked",
        "on_jutilsrembtn_clicked",
        "on_jutilsrenbtn_clicked",
        "on_phaserbtn_clicked",
        "on_phaserbtn_2_clicked",
        "on_cplsetaddbtn_clicked",
        "on_cplsetrembtn_clicked",
        "on_cplsettabwid_itemClicked",
        "QTableWidgetItem*",
        "on_TestDataBTN_clicked",
        "on_validationbtn_clicked",
        "on_validationbtn_2_clicked",
        "on_JudgeSettingBTN_clicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'ExitWin'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'on_refreshbtn_clicked'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_tlspinbox_valueChanged'
        QtMocHelpers::SlotData<void(int)>(4, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 5 },
        }}),
        // Slot 'on_mlspinbox_valueChanged'
        QtMocHelpers::SlotData<void(int)>(6, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 5 },
        }}),
        // Slot 'on_probtypecombo_activated'
        QtMocHelpers::SlotData<void(int)>(7, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 8 },
        }}),
        // Slot 'on_importpdfbtn_clicked'
        QtMocHelpers::SlotData<void()>(9, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_exportpdfbtn_clicked'
        QtMocHelpers::SlotData<void()>(10, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_loadpdfbtn_clicked'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_closepdfbtn_clicked'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_srcaddbtn_clicked'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_srcrembtn_clicked'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_jutilswid_itemClicked'
        QtMocHelpers::SlotData<void(QListWidgetItem *)>(15, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 16, 17 },
        }}),
        // Slot 'on_jutilsaddbtn_clicked'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_jutilsrembtn_clicked'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_jutilsrenbtn_clicked'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_phaserbtn_clicked'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_phaserbtn_2_clicked'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_cplsetaddbtn_clicked'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_cplsetrembtn_clicked'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_cplsettabwid_itemClicked'
        QtMocHelpers::SlotData<void(QTableWidgetItem *)>(25, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 26, 17 },
        }}),
        // Slot 'on_TestDataBTN_clicked'
        QtMocHelpers::SlotData<void()>(27, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_validationbtn_clicked'
        QtMocHelpers::SlotData<void()>(28, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_validationbtn_2_clicked'
        QtMocHelpers::SlotData<void()>(29, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_JudgeSettingBTN_clicked'
        QtMocHelpers::SlotData<void()>(30, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ProblemEditor, qt_meta_tag_ZN13ProblemEditorE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ProblemEditor::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13ProblemEditorE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13ProblemEditorE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13ProblemEditorE_t>.metaTypes,
    nullptr
} };

void ProblemEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ProblemEditor *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->ExitWin(); break;
        case 1: _t->on_refreshbtn_clicked(); break;
        case 2: _t->on_tlspinbox_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->on_mlspinbox_valueChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->on_probtypecombo_activated((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->on_importpdfbtn_clicked(); break;
        case 6: _t->on_exportpdfbtn_clicked(); break;
        case 7: _t->on_loadpdfbtn_clicked(); break;
        case 8: _t->on_closepdfbtn_clicked(); break;
        case 9: _t->on_srcaddbtn_clicked(); break;
        case 10: _t->on_srcrembtn_clicked(); break;
        case 11: _t->on_jutilswid_itemClicked((*reinterpret_cast< std::add_pointer_t<QListWidgetItem*>>(_a[1]))); break;
        case 12: _t->on_jutilsaddbtn_clicked(); break;
        case 13: _t->on_jutilsrembtn_clicked(); break;
        case 14: _t->on_jutilsrenbtn_clicked(); break;
        case 15: _t->on_phaserbtn_clicked(); break;
        case 16: _t->on_phaserbtn_2_clicked(); break;
        case 17: _t->on_cplsetaddbtn_clicked(); break;
        case 18: _t->on_cplsetrembtn_clicked(); break;
        case 19: _t->on_cplsettabwid_itemClicked((*reinterpret_cast< std::add_pointer_t<QTableWidgetItem*>>(_a[1]))); break;
        case 20: _t->on_TestDataBTN_clicked(); break;
        case 21: _t->on_validationbtn_clicked(); break;
        case 22: _t->on_validationbtn_2_clicked(); break;
        case 23: _t->on_JudgeSettingBTN_clicked(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ProblemEditor::*)()>(_a, &ProblemEditor::ExitWin, 0))
            return;
    }
}

const QMetaObject *ProblemEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ProblemEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13ProblemEditorE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int ProblemEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 24)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 24;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 24)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 24;
    }
    return _id;
}

// SIGNAL 0
void ProblemEditor::ExitWin()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
