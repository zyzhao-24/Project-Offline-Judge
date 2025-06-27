/****************************************************************************
** Meta object code from reading C++ file 'judgepanel.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../judgepanel.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'judgepanel.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN10JudgePanelE_t {};
} // unnamed namespace

template <> constexpr inline auto JudgePanel::qt_create_metaobjectdata<qt_meta_tag_ZN10JudgePanelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "JudgePanel",
        "ExitWin",
        "",
        "updateCplResult",
        "participant",
        "problem",
        "TResult",
        "verdict",
        "log",
        "updateJudgeResult",
        "caseid",
        "score",
        "time",
        "mem",
        "on_ExportBTN_clicked",
        "on_JudgeAllBTN_clicked",
        "on_JudgeBTN_clicked",
        "on_ImportBTN_clicked",
        "on_ScoreWID_cellDoubleClicked",
        "row",
        "column"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'ExitWin'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'updateCplResult'
        QtMocHelpers::SlotData<void(const QString &, const QString &, TResult, const QString &)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 4 }, { QMetaType::QString, 5 }, { 0x80000000 | 6, 7 }, { QMetaType::QString, 8 },
        }}),
        // Slot 'updateJudgeResult'
        QtMocHelpers::SlotData<void(const QString &, const QString &, int, TResult, double, unsigned int, unsigned int, const QString &)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 4 }, { QMetaType::QString, 5 }, { QMetaType::Int, 10 }, { 0x80000000 | 6, 7 },
            { QMetaType::Double, 11 }, { QMetaType::UInt, 12 }, { QMetaType::UInt, 13 }, { QMetaType::QString, 8 },
        }}),
        // Slot 'on_ExportBTN_clicked'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_JudgeAllBTN_clicked'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_JudgeBTN_clicked'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_ImportBTN_clicked'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_ScoreWID_cellDoubleClicked'
        QtMocHelpers::SlotData<void(int, int)>(18, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 19 }, { QMetaType::Int, 20 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<JudgePanel, qt_meta_tag_ZN10JudgePanelE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject JudgePanel::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10JudgePanelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10JudgePanelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10JudgePanelE_t>.metaTypes,
    nullptr
} };

void JudgePanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<JudgePanel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->ExitWin(); break;
        case 1: _t->updateCplResult((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<TResult>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4]))); break;
        case 2: _t->updateJudgeResult((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<TResult>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<uint>>(_a[6])),(*reinterpret_cast< std::add_pointer_t<uint>>(_a[7])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[8]))); break;
        case 3: _t->on_ExportBTN_clicked(); break;
        case 4: _t->on_JudgeAllBTN_clicked(); break;
        case 5: _t->on_JudgeBTN_clicked(); break;
        case 6: _t->on_ImportBTN_clicked(); break;
        case 7: _t->on_ScoreWID_cellDoubleClicked((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (JudgePanel::*)()>(_a, &JudgePanel::ExitWin, 0))
            return;
    }
}

const QMetaObject *JudgePanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *JudgePanel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10JudgePanelE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int JudgePanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void JudgePanel::ExitWin()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
