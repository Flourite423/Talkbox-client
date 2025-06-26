#include "HttpClient.h"
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QTimer>
#include <QHash>

HttpClient::HttpClient(QObject *parent)
    : QObject(parent)
    , m_manager(new QNetworkAccessManager(this))
    , m_baseUrl("http://localhost:8080")
    , m_timeoutMs(10000) // 10秒超时
{
}

void HttpClient::setBaseUrl(const QString &url)
{
    m_baseUrl = url;
}

void HttpClient::setToken(const QString &token)
{
    m_token = token;
}

void HttpClient::setTimeout(int timeoutMs)
{
    m_timeoutMs = timeoutMs;
}

void HttpClient::get(const QString &endpoint, const QJsonObject &params)
{
    QUrl url(m_baseUrl + endpoint);
    
    if (!params.isEmpty()) {
        QUrlQuery query;
        for (auto it = params.begin(); it != params.end(); ++it) {
            query.addQueryItem(it.key(), it.value().toString());
        }
        url.setQuery(query);
    }
    
    QNetworkRequest request = createRequest(endpoint);
    request.setUrl(url);
    
    QNetworkReply *reply = m_manager->get(request);
    setupReply(reply);
}

void HttpClient::post(const QString &endpoint, const QJsonObject &data)
{
    QNetworkRequest request = createRequest(endpoint);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    QJsonDocument doc(data);
    QByteArray jsonData = doc.toJson();
    
    QNetworkReply *reply = m_manager->post(request, jsonData);
    setupReply(reply);
}

void HttpClient::put(const QString &endpoint, const QJsonObject &data)
{
    QNetworkRequest request = createRequest(endpoint);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    QJsonDocument doc(data);
    QByteArray jsonData = doc.toJson();
    
    QNetworkReply *reply = m_manager->put(request, jsonData);
    setupReply(reply);
}

void HttpClient::deleteRequest(const QString &endpoint)
{
    QNetworkRequest request = createRequest(endpoint);
    
    QNetworkReply *reply = m_manager->deleteResource(request);
    setupReply(reply);
}

void HttpClient::onReplyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;
    
    // 清理定时器
    cleanupReply(reply);
    
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        
        if (!doc.isNull() && doc.isObject()) {
            emit responseReceived(doc.object());
        } else {
            emit errorOccurred("服务器返回了无效的JSON数据");
        }
    } else {
        QString errorMessage;
        switch (reply->error()) {
        case QNetworkReply::ConnectionRefusedError:
            errorMessage = "连接被拒绝，请检查服务器是否启动";
            break;
        case QNetworkReply::TimeoutError:
            errorMessage = "连接超时，请检查网络连接";
            break;
        case QNetworkReply::HostNotFoundError:
            errorMessage = "找不到服务器，请检查服务器地址";
            break;
        case QNetworkReply::NetworkSessionFailedError:
            errorMessage = "网络会话失败";
            break;
        default:
            errorMessage = reply->errorString();
            break;
        }
        emit errorOccurred(errorMessage);
    }
    
    reply->deleteLater();
}

void HttpClient::onTimeout()
{
    QTimer *timer = qobject_cast<QTimer*>(sender());
    if (!timer) return;
    
    // 找到对应的reply
    QNetworkReply *reply = nullptr;
    for (auto it = m_timers.begin(); it != m_timers.end(); ++it) {
        if (it.value() == timer) {
            reply = it.key();
            break;
        }
    }
    
    if (reply) {
        cleanupReply(reply);
        reply->abort();
        emit errorOccurred("请求超时");
        reply->deleteLater();
    }
}

QNetworkRequest HttpClient::createRequest(const QString &endpoint)
{
    QNetworkRequest request;
    request.setUrl(QUrl(m_baseUrl + endpoint));
    
    // 设置常用请求头
    request.setRawHeader("User-Agent", "Talkbox-Client/1.0");
    request.setRawHeader("Accept", "application/json");
    
    if (!m_token.isEmpty()) {
        request.setRawHeader("Authorization", ("Bearer " + m_token).toUtf8());
    }
    
    return request;
}

void HttpClient::setupReply(QNetworkReply *reply)
{
    connect(reply, &QNetworkReply::finished, this, &HttpClient::onReplyFinished);
    
    // 设置超时定时器
    QTimer *timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, &HttpClient::onTimeout);
    timer->start(m_timeoutMs);
    
    m_timers[reply] = timer;
}

void HttpClient::cleanupReply(QNetworkReply *reply)
{
    auto it = m_timers.find(reply);
    if (it != m_timers.end()) {
        it.value()->stop();
        it.value()->deleteLater();
        m_timers.erase(it);
    }
}
