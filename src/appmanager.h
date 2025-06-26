#ifndef APPMANAGER_H
#define APPMANAGER_H

#include <QObject>
#include <QStackedWidget>
#include <QJsonObject>
#include <QJsonArray>

class ApiClient;
class LoginWidget;
class RegisterWidget;
class ChatMainWidget;
class ForumMainWidget;

class AppManager : public QObject
{
    Q_OBJECT

public:
    explicit AppManager(QStackedWidget *stackedWidget, QObject *parent = nullptr);
    ~AppManager();

    // 界面切换
    void showLogin();
    void showRegister();
    void showChat();
    void showForum();

    // 用户状态
    bool isLoggedIn() const;
    QString getCurrentUsername() const;
    int getCurrentUserId() const;

signals:
    void loginStatusChanged(bool loggedIn);
    void userInfoChanged(const QString &username, int userId);

private slots:
    // API响应处理
    void onLoginSuccess(const QJsonObject &userData);
    void onLoginFailed(const QString &error);
    void onRegisterSuccess();
    void onRegisterFailed(const QString &error);
    void onLogoutSuccess();
    void onUserProfileReceived(const QJsonObject &profile);
    void onApiError(const QString &error);
    void onNetworkError(const QString &error);

    // 界面事件处理
    void onLoginRequested(const QString &username, const QString &password);
    void onRegisterRequested(const QString &username, const QString &password);
    void onLogoutRequested();
    void onShowRegisterRequested();
    void onShowLoginRequested();

private:
    QStackedWidget *m_stackedWidget;
    ApiClient *m_apiClient;
    
    // UI组件
    LoginWidget *m_loginWidget;
    RegisterWidget *m_registerWidget;
    ChatMainWidget *m_chatWidget;
    ForumMainWidget *m_forumWidget;
    
    // 用户状态
    bool m_isLoggedIn;
    QString m_currentUsername;
    int m_currentUserId;
    QString m_authToken;
    
    void setupUI();
    void connectSignals();
    void showError(const QString &title, const QString &message);
    void showInfo(const QString &title, const QString &message);
};

#endif // APPMANAGER_H
