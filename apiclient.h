#ifndef APICLIENT_H
#define APICLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>

class ApiClient : public QObject
{
    Q_OBJECT

public:
    explicit ApiClient(QObject *parent = nullptr);
    ~ApiClient();

    // 设置服务器URL
    void setServerUrl(const QString &url);
    
    // 设置认证token
    void setAuthToken(const QString &token);
    
    // 用户管理接口
    void registerUser(const QString &username, const QString &password);
    void loginUser(const QString &username, const QString &password);
    void logoutUser();
    void getUserProfile();
    
    // 消息接口
    void sendMessage(int receiverId, const QString &content, const QString &type = "text");
    void sendGroupMessage(int groupId, const QString &content, const QString &type = "text");
    void getMessages();
    void getGroupMessages(int groupId);
    void getContacts();
    
    // 群组接口
    void createGroup(const QString &groupName, const QString &description);
    void joinGroup(int groupId);
    void leaveGroup(int groupId);
    void getGroups();
    
    // 论坛接口
    void createPost(const QString &title, const QString &content);
    void getPosts();
    void getPostDetail(int postId);
    void replyPost(int postId, const QString &content);
    void getPostReplies(int postId);
    
    // 文件接口
    void uploadFile(const QString &filename, const QString &data);
    void downloadFile(const QString &filename);

signals:
    // 用户管理信号
    void registerSuccess();
    void registerFailed(const QString &error);
    void loginSuccess(const QJsonObject &userData);
    void loginFailed(const QString &error);
    void logoutSuccess();
    void userProfileReceived(const QJsonObject &profile);
    
    // 消息信号
    void messageReceived(const QJsonObject &message);
    void messagesReceived(const QJsonArray &messages);
    void groupMessagesReceived(const QJsonArray &messages);
    void contactsReceived(const QJsonArray &contacts);
    
    // 群组信号
    void groupCreated(const QJsonObject &group);
    void groupJoined();
    void groupLeft();
    void groupsReceived(const QJsonArray &groups);
    
    // 论坛信号
    void postCreated();
    void postsReceived(const QJsonArray &posts);
    void postDetailReceived(const QJsonObject &post);
    void replyCreated();
    void repliesReceived(const QJsonArray &replies);
    
    // 文件信号
    void fileUploaded();
    void fileDownloaded(const QString &filename, const QString &data);
    
    // 错误信号
    void apiError(const QString &error);
    void networkError(const QString &error);

private slots:
    void onReplyFinished();

private:
    QNetworkAccessManager *m_networkManager;
    QString m_serverUrl;
    QString m_authToken;
    
    void makeRequest(const QString &endpoint, const QString &method, 
                    const QJsonObject &data = QJsonObject(), 
                    bool requiresAuth = false);
    QNetworkRequest createRequest(const QString &endpoint, bool requiresAuth = false);
    void processResponse(QNetworkReply *reply);
};

#endif // APICLIENT_H
