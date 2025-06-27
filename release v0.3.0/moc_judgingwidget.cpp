/****************************************************************************
** Meta object code from reading C++ file 'judgingwidget.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../judgingwidget.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'judgingwidget.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN13JudgingThreadE_t {};
} // unnamed namespace

template <> constexpr inline auto JudgingThread::qt_create_metaobjectdata<qt_meta_tag_ZN13JudgingThreadE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "JudgingThread",
        "CplResult",
        "",
        "participant",
        "problem",
        "TResult",
        "verdict",
        "log",
        "JudgeResult",
        "caseid",
        "score",
        "time",
        "mem",
        "judgeComplete",
        "startJud",
        "setAbort",
        "abort"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'CplResult'
        QtMocHelpers::SignalData<void(const QString &, const QString &, TResult, const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::QString, 4 }, { 0x80000000 | 5, 6 }, { QMetaType::QString, 7 },
        }}),
        // Signal 'JudgeResult'
        QtMocHelpers::SignalData<void(const QString &, const QString &, int, TResult, double, unsigned int, unsigned int, const QString &)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::QString, 4 }, { QMetaType::Int, 9 }, { 0x80000000 | 5, 6 },
            { QMetaType::Double, 10 }, { QMetaType::UInt, 11 }, { QMetaType::UInt, 12 }, { QMetaType::QString, 7 },
        }}),
        // Signal 'judgeComplete'
        QtMocHelpers::SignalData<void()>(13, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'startJud'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'setAbort'
        QtMocHelpers::SlotData<void(bool)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 16 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<JudgingThread, qt_meta_tag_ZN13JudgingThreadE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject JudgingThread::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13JudgingThreadE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13JudgingThreadE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13JudgingThreadE_t>.metaTypes,
    nullptr
} };

void JudgingThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<JudgingThread *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->CplResult((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<TResult>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4]))); break;
        case 1: _t->JudgeResult((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<TResult>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<uint>>(_a[6])),(*reinterpret_cast< std::add_pointer_t<uint>>(_a[7])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[8]))); break;
        case 2: _t->judgeComplete(); break;
        case 3: _t->startJud(); break;
        case 4: _t->setAbort((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (JudgingThread::*)(const QString & , const QString & , TResult , const QString & )>(_a, &JudgingThread::CplResult, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (JudgingThread::*)(const QString & , const QString & , int , TResult , double , unsigned int , unsigned int , const QString & )>(_a, &JudgingThread::JudgeResult, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (JudgingThread::*)()>(_a, &JudgingThread::judgeComplete, 2))
            return;
    }
}

const QMetaObject *JudgingThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *JudgingThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13JudgingThreadE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int JudgingThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void JudgingThread::CplResult(const QString & _t1, const QString & _t2, TResult _t3, const QString & _t4)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2, _t3, _t4);
}

// SIGNAL 1
void JudgingThread::JudgeResult(const QString & _t1, const QString & _t2, int _t3, TResult _t4, double _t5, unsigned int _t6, unsigned int _t7, const QString & _t8)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8);
}

// SIGNAL 2
void JudgingThread::judgeComplete()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
namespace {
struct qt_meta_tag_ZN13JudgingWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto JudgingWidget::qt_create_metaobjectdata<qt_meta_tag_ZN13JudgingWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "JudgingWidget",
        "startJudgeSig",
        "",
        "cplComplete",
        "participant",
        "problem",
        "TResult",
        "verdict",
        "log",
        "judgeComplete",
        "caseid",
        "score",
        "time",
        "mem",
        "allComplete",
        "clrResult",
        "showCplResult",
        "showJudResult",
        "on_StopBTN_clicked",
        "on_ContinueBTN_clicked",
        "on_ClearBTN_clicked",
        "on_judgeTWid_itemDoubleClicked",
        "QTreeWidgetItem*",
        "item",
        "column"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'startJudgeSig'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'cplComplete'
        QtMocHelpers::SignalData<void(const QString &, const QString &, TResult, const QString &)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 4 }, { QMetaType::QString, 5 }, { 0x80000000 | 6, 7 }, { QMetaType::QString, 8 },
        }}),
        // Signal 'judgeComplete'
        QtMocHelpers::SignalData<void(const QString &, const QString &, int, TResult, double, unsigned int, unsigned int, const QString &)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 4 }, { QMetaType::QString, 5 }, { QMetaType::Int, 10 }, { 0x80000000 | 6, 7 },
            { QMetaType::Double, 11 }, { QMetaType::UInt, 12 }, { QMetaType::UInt, 13 }, { QMetaType::QString, 8 },
        }}),
        // Signal 'allComplete'
        QtMocHelpers::SignalData<void()>(14, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'clrResult'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'showCplResult'
        QtMocHelpers::SlotData<void(const QString &, const QString &, TResult, const QString &)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 4 }, { QMetaType::QString, 5 }, { 0x80000000 | 6, 7 }, { QMetaType::QString, 8 },
        }}),
        // Slot 'showJudResult'
        QtMocHelpers::SlotData<void(const QString &, const QString &, int, TResult, double, unsigned int, unsigned int, const QString &)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 4 }, { QMetaType::QString, 5 }, { QMetaType::Int, 10 }, { 0x80000000 | 6, 7 },
            { QMetaType::Double, 11 }, { QMetaType::UInt, 12 }, { QMetaType::UInt, 13 }, { QMetaType::QString, 8 },
        }}),
        // Slot 'on_StopBTN_clicked'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_ContinueBTN_clicked'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_ClearBTN_clicked'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_judgeTWid_itemDoubleClicked'
        QtMocHelpers::SlotData<void(QTreeWidgetItem *, int)>(21, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 22, 23 }, { QMetaType::Int, 24 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<JudgingWidget, qt_meta_tag_ZN13JudgingWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject JudgingWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13JudgingWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13JudgingWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13JudgingWidgetE_t>.metaTypes,
    nullptr
} };

void JudgingWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<JudgingWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->startJudgeSig(); break;
        case 1: _t->cplComplete((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<TResult>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4]))); break;
        case 2: _t->judgeComplete((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<TResult>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<uint>>(_a[6])),(*reinterpret_cast< std::add_pointer_t<uint>>(_a[7])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[8]))); break;
        case 3: _t->allComplete(); break;
        case 4: _t->clrResult(); break;
        case 5: _t->showCplResult((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<TResult>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[4]))); break;
        case 6: _t->showJudResult((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<TResult>>(_a[4])),(*reinterpret_cast< std::add_pointer_t<double>>(_a[5])),(*reinterpret_cast< std::add_pointer_t<uint>>(_a[6])),(*reinterpret_cast< std::add_pointer_t<uint>>(_a[7])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[8]))); break;
        case 7: _t->on_StopBTN_clicked(); break;
        case 8: _t->on_ContinueBTN_clicked(); break;
        case 9: _t->on_ClearBTN_clicked(); break;
        case 10: _t->on_judgeTWid_itemDoubleClicked((*reinterpret_cast< std::add_pointer_t<QTreeWidgetItem*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (JudgingWidget::*)()>(_a, &JudgingWidget::startJudgeSig, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (JudgingWidget::*)(const QString & , const QString & , TResult , const QString & )>(_a, &JudgingWidget::cplComplete, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (JudgingWidget::*)(const QString & , const QString & , int , TResult , double , unsigned int , unsigned int , const QString & )>(_a, &JudgingWidget::judgeComplete, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (JudgingWidget::*)()>(_a, &JudgingWidget::allComplete, 3))
            return;
    }
}

const QMetaObject *JudgingWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *JudgingWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13JudgingWidgetE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int JudgingWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void JudgingWidget::startJudgeSig()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void JudgingWidget::cplComplete(const QString & _t1, const QString & _t2, TResult _t3, const QString & _t4)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2, _t3, _t4);
}

// SIGNAL 2
void JudgingWidget::judgeComplete(const QString & _t1, const QString & _t2, int _t3, TResult _t4, double _t5, unsigned int _t6, unsigned int _t7, const QString & _t8)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8);
}

// SIGNAL 3
void JudgingWidget::allComplete()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
