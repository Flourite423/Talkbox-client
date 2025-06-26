#include "appmanager.h"
#include "apiclient.h"
#include "auth/loginwidget.h"
#include "auth/registerwidget.h"
#include "chat/chatmainwidget.h"
#include "forum/forummainwidget.h"
#include <QStackedWidget>
#include <QMessageBox>
#include <QJsonObject>

AppManager::AppManager(QStackedWidget *stackedWidget, QObject *parent)
    : QObject(parent)
    , m_stackedWidget(stackedWidget)
    , m_isLoggedIn(false)
    , m_currentUserId(-1)
{
    // 创建API客户端
    m_apiClient = new ApiClient(this);
    
    // 设置UI
    setupUI();
    
    // 连接信号
    connectSignals();
    
    // 显示登录界面
    showLogin();
}

AppManager::~AppManager()
{
}

void AppManager::setupUI()
{
    // 创建各个界面
    m_loginWidget = new LoginWidget();
    m_registerWidget = new RegisterWidget();
    m_chatWidget = new ChatMainWidget(m_apiClient);
    m_forumWidget = new ForumMainWidget(m_apiClient);
    
    // 添加到堆叠窗口
    m_stackedWidget->addWidget(m_loginWidget);
    m_stackedWidget->addWidget(m_registerWidget);
    m_stackedWidget->addWidget(m_chatWidget);
    m_stackedWidget->addWidget(m_forumWidget);
}

void AppManager::connectSignals()
{
    // API信号
    connect(m_apiClient, &ApiClient::loginSuccess, this, &AppManager::onLoginSuccess);
    connect(m_apiClient, &ApiClient::loginFailed, this, &AppManager::onLoginFailed);
    connect(m_apiClient, &ApiClient::registerSuccess, this, &AppManager::onRegisterSuccess);
    connect(m_apiClient, &ApiClient::registerFailed, this, &AppManager::onRegisterFailed);
    connect(m_apiClient, &ApiClient::logoutSuccess, this, &AppManager::onLogoutSuccess);
    connect(m_apiClient, &ApiClient::userProfileReceived, this, &AppManager::onUserProfileReceived);
    connect(m_apiClient, &ApiClient::apiError, this, &AppManager::onApiError);
    connect(m_apiClient, &ApiClient::networkError, this, &AppManager::onNetworkError);
    
    // 登录界面信号
    connect(m_loginWidget, &LoginWidget::loginRequested, this, &AppManager::onLoginRequested);
    connect(m_loginWidget, &LoginWidget::showRegisterRequested, this, &AppManager::onShowRegisterRequested);
    
    // 注册界面信号
    connect(m_registerWidget, &RegisterWidget::registerRequested, this, &AppManager::onRegisterRequested);
    connect(m_registerWidget, &RegisterWidget::showLoginRequested, this, &AppManager::onShowLoginRequested);
    
    // 聊天界面信号
    connect(m_chatWidget, &ChatMainWidget::showForumRequested, this, &AppManager::showForum);
    connect(m_chatWidget, &ChatMainWidget::logoutRequested, this, &AppManager::onLogoutRequested);
    
    // 论坛界面信号
    connect(m_forumWidget, &ForumMainWidget::showChatRequested, this, &AppManager::showChat);
    connect(m_forumWidget, &ForumMainWidget::logoutRequested, this, &AppManager::onLogoutRequested);
}

void AppManager::showLogin()
{
    m_loginWidget->clearForm();
    m_stackedWidget->setCurrentWidget(m_loginWidget);
}

void AppManager::showRegister()
{
    m_registerWidget->clearForm();
    m_stackedWidget->setCurrentWidget(m_registerWidget);
}

void AppManager::showChat()
{
    if (!m_isLoggedIn) {
        showLogin();
        return;
    }
    
    m_chatWidget->setCurrentUser(m_currentUsername, m_currentUserId);
    m_stackedWidget->setCurrentWidget(m_chatWidget);
}

void AppManager::showForum()
{
    if (!m_isLoggedIn) {
        showLogin();
        return;
    }
    
    m_forumWidget->setCurrentUser(m_currentUsername, m_currentUserId);
    m_stackedWidget->setCurrentWidget(m_forumWidget);
}

bool AppManager::isLoggedIn() const
{
    return m_isLoggedIn;
}

QString AppManager::getCurrentUsername() const
{
    return m_currentUsername;
}

int AppManager::getCurrentUserId() const
{
    return m_currentUserId;
}

void AppManager::onLoginSuccess(const QJsonObject &userData)
{
    m_isLoggedIn = true;
    m_currentUsername = userData["username"].toString();
    m_currentUserId = userData["user_id"].toInt();
    m_authToken = userData["token"].toString();
    
    // 启用界面
    m_loginWidget->setLoginEnabled(true);
    
    emit loginStatusChanged(true);
    emit userInfoChanged(m_currentUsername, m_currentUserId);
    
    showInfo("登录成功", QString("欢迎回来，%1！").arg(m_currentUsername));
    
    // 显示聊天界面
    showChat();
}

void AppManager::onLoginFailed(const QString &error)
{
    m_loginWidget->setLoginEnabled(true);
    showError("登录失败", error);
}

void AppManager::onRegisterSuccess()
{
    m_registerWidget->setRegisterEnabled(true);
    showInfo("注册成功", "账户创建成功，请登录！");
    showLogin();
}

void AppManager::onRegisterFailed(const QString &error)
{
    m_registerWidget->setRegisterEnabled(true);
    showError("注册失败", error);
}

void AppManager::onLogoutSuccess()
{
    m_isLoggedIn = false;
    m_currentUsername.clear();
    m_currentUserId = -1;
    m_authToken.clear();
    
    emit loginStatusChanged(false);
    emit userInfoChanged("", -1);
    
    showInfo("退出登录", "已成功退出登录");
    showLogin();
}

void AppManager::onUserProfileReceived(const QJsonObject &profile)
{
    // 更新用户信息
    m_currentUsername = profile["username"].toString();
    m_currentUserId = profile["user_id"].toInt();
    
    emit userInfoChanged(m_currentUsername, m_currentUserId);
}

void AppManager::onApiError(const QString &error)
{
    showError("API错误", error);
}

void AppManager::onNetworkError(const QString &error)
{
    showError("网络错误", error);
}

void AppManager::onLoginRequested(const QString &username, const QString &password)
{
    if (username.isEmpty() || password.isEmpty()) {
        showError("输入错误", "请输入用户名和密码");
        return;
    }
    
    m_loginWidget->setLoginEnabled(false);
    m_apiClient->loginUser(username, password);
}

void AppManager::onRegisterRequested(const QString &username, const QString &password)
{
    if (username.isEmpty() || password.isEmpty()) {
        showError("输入错误", "请输入完整信息");
        return;
    }
    
    m_registerWidget->setRegisterEnabled(false);
    m_apiClient->registerUser(username, password);
}

void AppManager::onLogoutRequested()
{
    if (m_isLoggedIn) {
        m_apiClient->logoutUser();
    }
}

void AppManager::onShowRegisterRequested()
{
    showRegister();
}

void AppManager::onShowLoginRequested()
{
    showLogin();
}

void AppManager::showError(const QString &title, const QString &message)
{
    QMessageBox::critical(m_stackedWidget, title, message);
}

void AppManager::showInfo(const QString &title, const QString &message)
{
    QMessageBox::information(m_stackedWidget, title, message);
}
