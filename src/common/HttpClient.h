#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTimer>

class HttpClient : public QObject
{
    Q_OBJECT

public:
    explicit HttpClient(QObject *parent = nullptr);
    
    void setBaseUrl(const QString &url);
    void setToken(const QString &token);
    void setTimeout(int timeoutMs);
    
    void get(const QString &endpoint, const QJsonObject &params = QJsonObject());
    void post(const QString &endpoint, const QJsonObject &data);
    void put(const QString &endpoint, const QJsonObject &data);
    void deleteRequest(const QString &endpoint);

signals:
    void responseReceived(const QJsonObject &response);
    void errorOccurred(const QString &error);

private slots:
    void onReplyFinished();
    void onTimeout();

private:
    QNetworkAccessManager *m_manager;
    QString m_baseUrl;
    QString m_token;
    int m_timeoutMs;
    QHash<QNetworkReply*, QTimer*> m_timers;
    
    QNetworkRequest createRequest(const QString &endpoint);
    void setupReply(QNetworkReply *reply);
    void cleanupReply(QNetworkReply *reply);
};

#endif // HTTPCLIENT_H
