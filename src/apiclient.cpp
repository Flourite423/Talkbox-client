#include "apiclient.h"
#include <QNetworkRequest>
#include <QJsonArray>
#include <QUrlQuery>
#include <QDebug>

ApiClient::ApiClient(QObject *parent)
    : QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_serverUrl("http://localhost:8080")
{
    connect(m_networkManager, &QNetworkAccessManager::finished,
            this, &ApiClient::onReplyFinished);
}

ApiClient::~ApiClient()
{
}

void ApiClient::setServerUrl(const QString &url)
{
    m_serverUrl = url;
}

void ApiClient::setAuthToken(const QString &token)
{
    m_authToken = token;
}

void ApiClient::registerUser(const QString &username, const QString &password)
{
    QJsonObject data;
    data["username"] = username;
    data["password"] = password;
    makeRequest("/api/register", "POST", data);
}

void ApiClient::loginUser(const QString &username, const QString &password)
{
    QJsonObject data;
    data["username"] = username;
    data["password"] = password;
    makeRequest("/api/login", "POST", data);
}

void ApiClient::logoutUser()
{
    makeRequest("/api/logout", "POST", QJsonObject(), true);
}

void ApiClient::getUserProfile()
{
    makeRequest("/api/user/profile", "GET", QJsonObject(), true);
}

void ApiClient::sendMessage(int receiverId, const QString &content, const QString &type)
{
    QJsonObject data;
    data["receiver_id"] = QString::number(receiverId);
    data["content"] = content;
    data["type"] = type;
    makeRequest("/api/send_message", "POST", data, true);
}

void ApiClient::sendGroupMessage(int groupId, const QString &content, const QString &type)
{
    QJsonObject data;
    data["group_id"] = QString::number(groupId);
    data["content"] = content;
    data["type"] = type;
    makeRequest("/api/send_message", "POST", data, true);
}

void ApiClient::getMessages()
{
    makeRequest("/api/get_messages", "GET", QJsonObject(), true);
}

void ApiClient::getGroupMessages(int groupId)
{
    QJsonObject data;
    data["group_id"] = QString::number(groupId);
    makeRequest("/api/get_group_messages", "GET", data, true);
}

void ApiClient::getContacts()
{
    makeRequest("/api/get_contacts", "GET", QJsonObject(), true);
}

void ApiClient::createGroup(const QString &groupName, const QString &description)
{
    QJsonObject data;
    data["group_name"] = groupName;
    data["description"] = description;
    makeRequest("/api/create_group", "POST", data, true);
}

void ApiClient::joinGroup(int groupId)
{
    QJsonObject data;
    data["group_id"] = QString::number(groupId);
    makeRequest("/api/join_group", "POST", data, true);
}

void ApiClient::leaveGroup(int groupId)
{
    QJsonObject data;
    data["group_id"] = QString::number(groupId);
    makeRequest("/api/leave_group", "POST", data, true);
}

void ApiClient::getGroups()
{
    makeRequest("/api/get_groups", "GET", QJsonObject(), true);
}

void ApiClient::createPost(const QString &title, const QString &content)
{
    QJsonObject data;
    data["title"] = title;
    data["content"] = content;
    makeRequest("/api/create_post", "POST", data, true);
}

void ApiClient::getPosts()
{
    makeRequest("/api/get_posts", "GET");
}

void ApiClient::getPostDetail(int postId)
{
    makeRequest(QString("/api/post/%1").arg(postId), "GET");
}

void ApiClient::replyPost(int postId, const QString &content)
{
    QJsonObject data;
    data["post_id"] = QString::number(postId);
    data["content"] = content;
    makeRequest("/api/reply_post", "POST", data, true);
}

void ApiClient::getPostReplies(int postId)
{
    QJsonObject data;
    data["post_id"] = QString::number(postId);
    makeRequest("/api/get_post_replies", "GET", data);
}

void ApiClient::uploadFile(const QString &filename, const QString &data)
{
    QJsonObject requestData;
    requestData["filename"] = filename;
    requestData["data"] = data;
    makeRequest("/api/upload_file", "POST", requestData, true);
}

void ApiClient::downloadFile(const QString &filename)
{
    QString endpoint = QString("/api/download_file?filename=%1").arg(filename);
    makeRequest(endpoint, "GET");
}

void ApiClient::makeRequest(const QString &endpoint, const QString &method, 
                          const QJsonObject &data, bool requiresAuth)
{
    QNetworkRequest request = createRequest(endpoint, requiresAuth);
    QNetworkReply *reply = nullptr;
    
    if (method == "GET") {
        if (!data.isEmpty()) {
            // 对于GET请求，将数据作为查询参数或请求体
            QJsonDocument doc(data);
            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
            reply = m_networkManager->get(request);
        } else {
            reply = m_networkManager->get(request);
        }
    } else if (method == "POST") {
        QJsonDocument doc(data);
        QByteArray requestData = doc.toJson();
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        reply = m_networkManager->post(request, requestData);
    }
    
    if (reply) {
        // 存储请求信息以便在响应中使用
        reply->setProperty("endpoint", endpoint);
        reply->setProperty("method", method);
    }
}

QNetworkRequest ApiClient::createRequest(const QString &endpoint, bool requiresAuth)
{
    QNetworkRequest request;
    request.setUrl(QUrl(m_serverUrl + endpoint));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    if (requiresAuth && !m_authToken.isEmpty()) {
        request.setRawHeader("Authorization", QString("Bearer %1").arg(m_authToken).toUtf8());
    }
    
    return request;
}

void ApiClient::onReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;
    
    reply->deleteLater();
    processResponse(reply);
}

void ApiClient::processResponse(QNetworkReply *reply)
{
    QString endpoint = reply->property("endpoint").toString();
    QString method = reply->property("method").toString();
    
    if (reply->error() != QNetworkReply::NoError) {
        emit networkError(reply->errorString());
        return;
    }
    
    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject response = doc.object();
    
    QString status = response["status"].toString();
    QJsonValue dataValue = response["data"];
    
    if (status == "error") {
        emit apiError(dataValue.toString());
        return;
    }
    
    // 根据endpoint分发响应
    if (endpoint == "/api/register") {
        emit registerSuccess();
    } else if (endpoint == "/api/login") {
        QJsonObject userData = dataValue.toObject();
        QString token = userData["token"].toString();
        if (!token.isEmpty()) {
            setAuthToken(token);
        }
        emit loginSuccess(userData);
    } else if (endpoint == "/api/logout") {
        setAuthToken("");
        emit logoutSuccess();
    } else if (endpoint == "/api/user/profile") {
        emit userProfileReceived(dataValue.toObject());
    } else if (endpoint == "/api/send_message") {
        emit messageReceived(dataValue.toObject());
    } else if (endpoint == "/api/get_messages") {
        emit messagesReceived(dataValue.toArray());
    } else if (endpoint == "/api/get_group_messages") {
        emit groupMessagesReceived(dataValue.toArray());
    } else if (endpoint == "/api/get_contacts") {
        emit contactsReceived(dataValue.toArray());
    } else if (endpoint == "/api/create_group") {
        emit groupCreated(dataValue.toObject());
    } else if (endpoint == "/api/join_group") {
        emit groupJoined();
    } else if (endpoint == "/api/leave_group") {
        emit groupLeft();
    } else if (endpoint == "/api/get_groups") {
        emit groupsReceived(dataValue.toArray());
    } else if (endpoint == "/api/create_post") {
        emit postCreated();
    } else if (endpoint == "/api/get_posts") {
        emit postsReceived(dataValue.toArray());
    } else if (endpoint.startsWith("/api/post/")) {
        emit postDetailReceived(dataValue.toObject());
    } else if (endpoint == "/api/reply_post") {
        emit replyCreated();
    } else if (endpoint == "/api/get_post_replies") {
        emit repliesReceived(dataValue.toArray());
    } else if (endpoint == "/api/upload_file") {
        emit fileUploaded();
    } else if (endpoint.startsWith("/api/download_file")) {
        // 对于文件下载，需要特殊处理
        emit fileDownloaded("", dataValue.toString());
    }
}
