/****************************************************************************
** Meta object code from reading C++ file 'forummainwidget.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/forum/forummainwidget.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'forummainwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.1. It"
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
struct qt_meta_tag_ZN15ForumMainWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto ForumMainWidget::qt_create_metaobjectdata<qt_meta_tag_ZN15ForumMainWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ForumMainWidget",
        "showChatRequested",
        "",
        "logoutRequested",
        "refreshPosts",
        "onPostsReceived",
        "posts",
        "onPostDetailReceived",
        "post",
        "onRepliesReceived",
        "replies",
        "onPostCreated",
        "onReplyCreated",
        "onPostSelected",
        "onCreatePostClicked",
        "onReplyPostClicked",
        "onBackToListClicked",
        "onShowChatRequested",
        "onLogoutRequested"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'showChatRequested'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'logoutRequested'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'refreshPosts'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onPostsReceived'
        QtMocHelpers::SlotData<void(const QJsonArray &)>(5, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QJsonArray, 6 },
        }}),
        // Slot 'onPostDetailReceived'
        QtMocHelpers::SlotData<void(const QJsonObject &)>(7, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QJsonObject, 8 },
        }}),
        // Slot 'onRepliesReceived'
        QtMocHelpers::SlotData<void(const QJsonArray &)>(9, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QJsonArray, 10 },
        }}),
        // Slot 'onPostCreated'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onReplyCreated'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onPostSelected'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onCreatePostClicked'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onReplyPostClicked'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onBackToListClicked'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onShowChatRequested'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onLogoutRequested'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ForumMainWidget, qt_meta_tag_ZN15ForumMainWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ForumMainWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15ForumMainWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15ForumMainWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN15ForumMainWidgetE_t>.metaTypes,
    nullptr
} };

void ForumMainWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ForumMainWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->showChatRequested(); break;
        case 1: _t->logoutRequested(); break;
        case 2: _t->refreshPosts(); break;
        case 3: _t->onPostsReceived((*reinterpret_cast< std::add_pointer_t<QJsonArray>>(_a[1]))); break;
        case 4: _t->onPostDetailReceived((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 5: _t->onRepliesReceived((*reinterpret_cast< std::add_pointer_t<QJsonArray>>(_a[1]))); break;
        case 6: _t->onPostCreated(); break;
        case 7: _t->onReplyCreated(); break;
        case 8: _t->onPostSelected(); break;
        case 9: _t->onCreatePostClicked(); break;
        case 10: _t->onReplyPostClicked(); break;
        case 11: _t->onBackToListClicked(); break;
        case 12: _t->onShowChatRequested(); break;
        case 13: _t->onLogoutRequested(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ForumMainWidget::*)()>(_a, &ForumMainWidget::showChatRequested, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ForumMainWidget::*)()>(_a, &ForumMainWidget::logoutRequested, 1))
            return;
    }
}

const QMetaObject *ForumMainWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ForumMainWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN15ForumMainWidgetE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ForumMainWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void ForumMainWidget::showChatRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ForumMainWidget::logoutRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
