/****************************************************************************
** Meta object code from reading C++ file 'apiclient.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/apiclient.h"
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'apiclient.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN9ApiClientE_t {};
} // unnamed namespace

template <> constexpr inline auto ApiClient::qt_create_metaobjectdata<qt_meta_tag_ZN9ApiClientE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "ApiClient",
        "registerSuccess",
        "",
        "registerFailed",
        "error",
        "loginSuccess",
        "userData",
        "loginFailed",
        "logoutSuccess",
        "userProfileReceived",
        "profile",
        "messageReceived",
        "message",
        "messagesReceived",
        "messages",
        "groupMessagesReceived",
        "contactsReceived",
        "contacts",
        "groupCreated",
        "group",
        "groupJoined",
        "groupLeft",
        "groupsReceived",
        "groups",
        "postCreated",
        "postsReceived",
        "posts",
        "postDetailReceived",
        "post",
        "replyCreated",
        "repliesReceived",
        "replies",
        "fileUploaded",
        "fileDownloaded",
        "filename",
        "data",
        "apiError",
        "networkError",
        "onReplyFinished"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'registerSuccess'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'registerFailed'
        QtMocHelpers::SignalData<void(const QString &)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 4 },
        }}),
        // Signal 'loginSuccess'
        QtMocHelpers::SignalData<void(const QJsonObject &)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QJsonObject, 6 },
        }}),
        // Signal 'loginFailed'
        QtMocHelpers::SignalData<void(const QString &)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 4 },
        }}),
        // Signal 'logoutSuccess'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'userProfileReceived'
        QtMocHelpers::SignalData<void(const QJsonObject &)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QJsonObject, 10 },
        }}),
        // Signal 'messageReceived'
        QtMocHelpers::SignalData<void(const QJsonObject &)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QJsonObject, 12 },
        }}),
        // Signal 'messagesReceived'
        QtMocHelpers::SignalData<void(const QJsonArray &)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QJsonArray, 14 },
        }}),
        // Signal 'groupMessagesReceived'
        QtMocHelpers::SignalData<void(const QJsonArray &)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QJsonArray, 14 },
        }}),
        // Signal 'contactsReceived'
        QtMocHelpers::SignalData<void(const QJsonArray &)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QJsonArray, 17 },
        }}),
        // Signal 'groupCreated'
        QtMocHelpers::SignalData<void(const QJsonObject &)>(18, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QJsonObject, 19 },
        }}),
        // Signal 'groupJoined'
        QtMocHelpers::SignalData<void()>(20, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'groupLeft'
        QtMocHelpers::SignalData<void()>(21, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'groupsReceived'
        QtMocHelpers::SignalData<void(const QJsonArray &)>(22, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QJsonArray, 23 },
        }}),
        // Signal 'postCreated'
        QtMocHelpers::SignalData<void()>(24, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'postsReceived'
        QtMocHelpers::SignalData<void(const QJsonArray &)>(25, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QJsonArray, 26 },
        }}),
        // Signal 'postDetailReceived'
        QtMocHelpers::SignalData<void(const QJsonObject &)>(27, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QJsonObject, 28 },
        }}),
        // Signal 'replyCreated'
        QtMocHelpers::SignalData<void()>(29, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'repliesReceived'
        QtMocHelpers::SignalData<void(const QJsonArray &)>(30, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QJsonArray, 31 },
        }}),
        // Signal 'fileUploaded'
        QtMocHelpers::SignalData<void()>(32, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'fileDownloaded'
        QtMocHelpers::SignalData<void(const QString &, const QString &)>(33, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 34 }, { QMetaType::QString, 35 },
        }}),
        // Signal 'apiError'
        QtMocHelpers::SignalData<void(const QString &)>(36, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 4 },
        }}),
        // Signal 'networkError'
        QtMocHelpers::SignalData<void(const QString &)>(37, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 4 },
        }}),
        // Slot 'onReplyFinished'
        QtMocHelpers::SlotData<void()>(38, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<ApiClient, qt_meta_tag_ZN9ApiClientE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject ApiClient::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9ApiClientE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9ApiClientE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN9ApiClientE_t>.metaTypes,
    nullptr
} };

void ApiClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ApiClient *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->registerSuccess(); break;
        case 1: _t->registerFailed((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->loginSuccess((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 3: _t->loginFailed((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->logoutSuccess(); break;
        case 5: _t->userProfileReceived((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 6: _t->messageReceived((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 7: _t->messagesReceived((*reinterpret_cast< std::add_pointer_t<QJsonArray>>(_a[1]))); break;
        case 8: _t->groupMessagesReceived((*reinterpret_cast< std::add_pointer_t<QJsonArray>>(_a[1]))); break;
        case 9: _t->contactsReceived((*reinterpret_cast< std::add_pointer_t<QJsonArray>>(_a[1]))); break;
        case 10: _t->groupCreated((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 11: _t->groupJoined(); break;
        case 12: _t->groupLeft(); break;
        case 13: _t->groupsReceived((*reinterpret_cast< std::add_pointer_t<QJsonArray>>(_a[1]))); break;
        case 14: _t->postCreated(); break;
        case 15: _t->postsReceived((*reinterpret_cast< std::add_pointer_t<QJsonArray>>(_a[1]))); break;
        case 16: _t->postDetailReceived((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1]))); break;
        case 17: _t->replyCreated(); break;
        case 18: _t->repliesReceived((*reinterpret_cast< std::add_pointer_t<QJsonArray>>(_a[1]))); break;
        case 19: _t->fileUploaded(); break;
        case 20: _t->fileDownloaded((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2]))); break;
        case 21: _t->apiError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 22: _t->networkError((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 23: _t->onReplyFinished(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (ApiClient::*)()>(_a, &ApiClient::registerSuccess, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (ApiClient::*)(const QString & )>(_a, &ApiClient::registerFailed, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (ApiClient::*)(const QJsonObject & )>(_a, &ApiClient::loginSuccess, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (ApiClient::*)(const QString & )>(_a, &ApiClient::loginFailed, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (ApiClient::*)()>(_a, &ApiClient::logoutSuccess, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (ApiClient::*)(const QJsonObject & )>(_a, &ApiClient::userProfileReceived, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (ApiClient::*)(const QJsonObject & )>(_a, &ApiClient::messageReceived, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (ApiClient::*)(const QJsonArray & )>(_a, &ApiClient::messagesReceived, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (ApiClient::*)(const QJsonArray & )>(_a, &ApiClient::groupMessagesReceived, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (ApiClient::*)(const QJsonArray & )>(_a, &ApiClient::contactsReceived, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (ApiClient::*)(const QJsonObject & )>(_a, &ApiClient::groupCreated, 10))
            return;
        if (QtMocHelpers::indexOfMethod<void (ApiClient::*)()>(_a, &ApiClient::groupJoined, 11))
            return;
        if (QtMocHelpers::indexOfMethod<void (ApiClient::*)()>(_a, &ApiClient::groupLeft, 12))
            return;
        if (QtMocHelpers::indexOfMethod<void (ApiClient::*)(const QJsonArray & )>(_a, &ApiClient::groupsReceived, 13))
            return;
        if (QtMocHelpers::indexOfMethod<void (ApiClient::*)()>(_a, &ApiClient::postCreated, 14))
            return;
        if (QtMocHelpers::indexOfMethod<void (ApiClient::*)(const QJsonArray & )>(_a, &ApiClient::postsReceived, 15))
            return;
        if (QtMocHelpers::indexOfMethod<void (ApiClient::*)(const QJsonObject & )>(_a, &ApiClient::postDetailReceived, 16))
            return;
        if (QtMocHelpers::indexOfMethod<void (ApiClient::*)()>(_a, &ApiClient::replyCreated, 17))
            return;
        if (QtMocHelpers::indexOfMethod<void (ApiClient::*)(const QJsonArray & )>(_a, &ApiClient::repliesReceived, 18))
            return;
        if (QtMocHelpers::indexOfMethod<void (ApiClient::*)()>(_a, &ApiClient::fileUploaded, 19))
            return;
        if (QtMocHelpers::indexOfMethod<void (ApiClient::*)(const QString & , const QString & )>(_a, &ApiClient::fileDownloaded, 20))
            return;
        if (QtMocHelpers::indexOfMethod<void (ApiClient::*)(const QString & )>(_a, &ApiClient::apiError, 21))
            return;
        if (QtMocHelpers::indexOfMethod<void (ApiClient::*)(const QString & )>(_a, &ApiClient::networkError, 22))
            return;
    }
}

const QMetaObject *ApiClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ApiClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9ApiClientE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ApiClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void ApiClient::registerSuccess()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void ApiClient::registerFailed(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void ApiClient::loginSuccess(const QJsonObject & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void ApiClient::loginFailed(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void ApiClient::logoutSuccess()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void ApiClient::userProfileReceived(const QJsonObject & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}

// SIGNAL 6
void ApiClient::messageReceived(const QJsonObject & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1);
}

// SIGNAL 7
void ApiClient::messagesReceived(const QJsonArray & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1);
}

// SIGNAL 8
void ApiClient::groupMessagesReceived(const QJsonArray & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1);
}

// SIGNAL 9
void ApiClient::contactsReceived(const QJsonArray & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 9, nullptr, _t1);
}

// SIGNAL 10
void ApiClient::groupCreated(const QJsonObject & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 10, nullptr, _t1);
}

// SIGNAL 11
void ApiClient::groupJoined()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void ApiClient::groupLeft()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}

// SIGNAL 13
void ApiClient::groupsReceived(const QJsonArray & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 13, nullptr, _t1);
}

// SIGNAL 14
void ApiClient::postCreated()
{
    QMetaObject::activate(this, &staticMetaObject, 14, nullptr);
}

// SIGNAL 15
void ApiClient::postsReceived(const QJsonArray & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 15, nullptr, _t1);
}

// SIGNAL 16
void ApiClient::postDetailReceived(const QJsonObject & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 16, nullptr, _t1);
}

// SIGNAL 17
void ApiClient::replyCreated()
{
    QMetaObject::activate(this, &staticMetaObject, 17, nullptr);
}

// SIGNAL 18
void ApiClient::repliesReceived(const QJsonArray & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 18, nullptr, _t1);
}

// SIGNAL 19
void ApiClient::fileUploaded()
{
    QMetaObject::activate(this, &staticMetaObject, 19, nullptr);
}

// SIGNAL 20
void ApiClient::fileDownloaded(const QString & _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 20, nullptr, _t1, _t2);
}

// SIGNAL 21
void ApiClient::apiError(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 21, nullptr, _t1);
}

// SIGNAL 22
void ApiClient::networkError(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 22, nullptr, _t1);
}
QT_WARNING_POP
