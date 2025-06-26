/****************************************************************************
** Meta object code from reading C++ file 'chatmainwidget.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/chat/chatmainwidget.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chatmainwidget.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN14ChatMainWidgetE_t {};
} // unnamed namespace

template <> constexpr inline auto ChatMainWidget::qt_create_metaobjectdata<qt_meta_tag_ZN14ChatMainWidgetE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ChatMainWidget",
        "showForumRequested",
        "",
        "logoutRequested",
        "refreshContacts",
        "refreshGroups",
        "refreshMessages",
        "onContactsReceived",
        "contacts",
        "onGroupsReceived",
        "groups",
        "onMessagesReceived",
        "messages",
        "onGroupMessagesReceived",
        "onMessageSent",
        "onContactSelected",
        "onGroupSelected",
        "onSendMessageClicked",
        "onCreateGroupClicked",
        "onJoinGroupClicked",
        "onShowForumRequested",
        "onLogoutRequested"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'showForumRequested'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'logoutRequested'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'refreshContacts'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'refreshGroups'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'refreshMessages'
        QtMocHelpers::SlotData<void()>(6, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onContactsReceived'
        QtMocHelpers::SlotData<void(const QJsonArray &)>(7, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QJsonArray, 8 },
        }}),
        // Slot 'onGroupsReceived'
        QtMocHelpers::SlotData<void(const QJsonArray &)>(9, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QJsonArray, 10 },
        }}),
        // Slot 'onMessagesReceived'
        QtMocHelpers::SlotData<void(const QJsonArray &)>(11, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QJsonArray, 12 },
        }}),
        // Slot 'onGroupMessagesReceived'
        QtMocHelpers::SlotData<void(const QJsonArray &)>(13, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QJsonArray, 12 },
        }}),
        // Slot 'onMessageSent'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onContactSelected'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onGroupSelected'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onSendMessageClicked'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onCreateGroupClicked'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onJoinGroupClicked'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onShowForumRequested'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onLogoutRequested'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ChatMainWidget, qt_meta_tag_ZN14ChatMainWidgetE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ChatMainWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14ChatMainWidgetE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14ChatMainWidgetE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN14ChatMainWidgetE_t>.metaTypes,
    nullptr
} };

void ChatMainWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ChatMainWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->showForumRequested(); break;
        case 1: _t->logoutRequested(); break;
        case 2: _t->refreshContacts(); break;
        case 3: _t->refreshGroups(); break;
        case 4: _t->refreshMessages(); break;
        case 5: _t->onContactsReceived((*reinterpret_cast< std::add_pointer_t<QJsonArray>>(_a[1]))); break;
        case 6: _t->onGroupsReceived((*reinterpret_cast< std::add_pointer_t<QJsonArray>>(_a[1]))); break;
        case 7: _t->onMessagesReceived((*reinterpret_cast< std::add_pointer_t<QJsonArray>>(_a[1]))); break;
        case 8: _t->onGroupMessagesReceived((*reinterpret_cast< std::add_pointer_t<QJsonArray>>(_a[1]))); break;
        case 9: _t->onMessageSent(); break;
        case 10: _t->onContactSelected(); break;
        case 11: _t->onGroupSelected(); break;
        case 12: _t->onSendMessageClicked(); break;
        case 13: _t->onCreateGroupClicked(); break;
        case 14: _t->onJoinGroupClicked(); break;
        case 15: _t->onShowForumRequested(); break;
        case 16: _t->onLogoutRequested(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ChatMainWidget::*)()>(_a, &ChatMainWidget::showForumRequested, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ChatMainWidget::*)()>(_a, &ChatMainWidget::logoutRequested, 1))
            return;
    }
}

const QMetaObject *ChatMainWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChatMainWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN14ChatMainWidgetE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ChatMainWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void ChatMainWidget::showForumRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ChatMainWidget::logoutRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
