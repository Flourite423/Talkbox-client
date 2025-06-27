#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QTimer>
#include <QApplication>
#include <QMap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_httpClient(new HttpClient(this))
    , m_userId(-1)
    , m_loginSuccessful(false)
{
    ui->setupUi(this);
    setupUI();
    
    // 先隐藏主窗口，等登录成功后再显示
    hide();
    
    if (!showLoginDialog()) {
        // 用户取消登录，应该退出应用程序
        QTimer::singleShot(0, qApp, &QApplication::quit);
        return;
    }
    
    // 检查登录是否成功
    if (m_loginSuccessful) {
        show();
    } else {
        QTimer::singleShot(0, qApp, &QApplication::quit);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI()
{
    // 创建各个组件
    m_contactList = new ContactList(this);
    m_chatWindow = new ChatWindow(this);
    m_groupManager = new GroupManager(this);
    m_forumWidget = new ForumWidget(this);
    m_postDetail = new PostDetail(this);
    m_createPost = new CreatePost(this);
    
    // 设置HTTP客户端
    m_contactList->setHttpClient(m_httpClient);
    m_chatWindow->setHttpClient(m_httpClient);
    m_groupManager->setHttpClient(m_httpClient);
    m_forumWidget->setHttpClient(m_httpClient);
    m_postDetail->setHttpClient(m_httpClient);
    m_createPost->setHttpClient(m_httpClient);
    
    // 设置布局 - 聊天标签页
    QVBoxLayout *contactLayout = new QVBoxLayout(ui->contactWidget);
    contactLayout->addWidget(m_contactList);
    
    QVBoxLayout *chatLayout = new QVBoxLayout(ui->chatWidget);
    chatLayout->addWidget(m_chatWindow);
    
    // 设置布局 - 群组标签页
    QVBoxLayout *groupManagerLayout = new QVBoxLayout(ui->groupManagerWidget);
    groupManagerLayout->addWidget(m_groupManager);
    
    // 群聊窗口复用聊天窗口组件，但需要另一个实例
    m_groupChatWindow = new ChatWindow(this);
    m_groupChatWindow->setHttpClient(m_httpClient);
    QVBoxLayout *groupChatLayout = new QVBoxLayout(ui->groupChatWidget);
    groupChatLayout->addWidget(m_groupChatWindow);
    
    // 设置布局 - 论坛标签页
    QVBoxLayout *forumListLayout = new QVBoxLayout(ui->forumListWidget);
    forumListLayout->addWidget(m_forumWidget);
    
    QVBoxLayout *forumDetailLayout = new QVBoxLayout(ui->forumDetailWidget);
    forumDetailLayout->addWidget(m_postDetail);
    
    // 连接信号槽
    connect(m_contactList, &ContactList::contactSelected, this, &MainWindow::onContactSelected);
    connect(m_contactList, &ContactList::contactsLoaded, this, &MainWindow::onContactsLoaded);
    connect(m_groupManager, &GroupManager::groupSelected, this, &MainWindow::onGroupSelected);
    connect(m_groupManager, &GroupManager::groupSelected, m_groupChatWindow, &ChatWindow::startGroupChat);
    connect(m_groupManager, &GroupManager::groupOperationCompleted, this, &MainWindow::onGroupOperationCompleted);
    connect(m_forumWidget, &ForumWidget::postSelected, this, &MainWindow::onPostSelected);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);
    connect(ui->actionLogout, &QAction::triggered, this, &MainWindow::onLogoutClicked);
    connect(m_createPost, &CreatePost::postCreated, m_forumWidget, &ForumWidget::refreshPosts);
    
    // 状态栏
    ui->statusbar->addWidget(ui->userLabel);
}

bool MainWindow::showLoginDialog()
{
    LoginDialog *loginDialog = new LoginDialog(this);
    
    // 使用局部变量来跟踪登录状态
    bool loginSucceeded = false;
    
    connect(loginDialog, &LoginDialog::loginSuccessful, 
            [this, &loginSucceeded](const QString &token, const QString &username, int userId) {
                onLoginSuccessful(token, username, userId);
                loginSucceeded = true;
            });
    
    int result = loginDialog->exec();
    
    loginDialog->deleteLater();
    
    return (result == QDialog::Accepted && loginSucceeded);
}

void MainWindow::onLoginSuccessful(const QString &token, const QString &username, int userId)
{
    m_token = token; // 保留为兼容性，但不再使用
    m_username = username;
    m_userId = userId;
    m_loginSuccessful = true;
    
    m_httpClient->setUsername(username);
    
    // 设置各组件的当前用户名
    m_contactList->setCurrentUsername(username);
    m_chatWindow->setCurrentUsername(username);
    m_groupChatWindow->setCurrentUsername(username);
    m_groupManager->setCurrentUsername(username);
    m_forumWidget->setCurrentUsername(username);
    m_postDetail->setCurrentUsername(username);
    m_createPost->setCurrentUsername(username);
    
    updateUserInfo();
    
    // 初始化数据
    m_contactList->refreshContacts();
    m_groupManager->refreshGroups();
    m_forumWidget->refreshPosts();
}

void MainWindow::updateUserInfo()
{
    ui->userLabel->setText(QString("已登录: %1 (ID: %2)").arg(m_username).arg(m_userId));
    setWindowTitle(QString("Talkbox - %1").arg(m_username));
}

void MainWindow::onLogoutClicked()
{
    if (m_httpClient && !m_username.isEmpty()) {
        QJsonObject data;
        data["username"] = m_username;
        m_httpClient->post("/api/logout", data);
    }
    
    m_token.clear();
    m_username.clear();
    m_userId = -1;
    m_loginSuccessful = false;
    m_httpClient->setUsername("");
    
    ui->userLabel->setText("未登录");
    setWindowTitle("Talkbox - 聊天软件");
    
    // 隐藏主窗口
    hide();
    
    if (!showLoginDialog()) {
        // 用户取消重新登录，退出应用程序
        QTimer::singleShot(0, qApp, &QApplication::quit);
    } else {
        // 登录成功，显示主窗口
        show();
    }
}

void MainWindow::onContactSelected(int userId, const QString &username)
{
    m_chatWindow->startPrivateChat(userId, username);
    ui->tabWidget->setCurrentIndex(0); // 切换到聊天标签页
}

void MainWindow::onGroupSelected(int groupId, const QString &groupName)
{
    Q_UNUSED(groupId)
    Q_UNUSED(groupName)
    ui->tabWidget->setCurrentIndex(1); // 切换到群组标签页
}

void MainWindow::onPostSelected(int postId, const QString &title, const QString &content, 
                                const QString &timestamp, int userId)
{
    m_postDetail->showPost(postId, title, content, timestamp, userId);
}

void MainWindow::onTabChanged(int index)
{
    // 当切换标签页时刷新相应的数据，但增加时间间隔控制
    QDateTime currentTime = QDateTime::currentDateTime();
    
    switch (index) {
    case 0: // 聊天
        if (m_lastContactRefreshTime.isNull() || 
            m_lastContactRefreshTime.secsTo(currentTime) >= REFRESH_INTERVAL_SECONDS) {
            m_contactList->refreshContacts();
            m_lastContactRefreshTime = currentTime;
        }
        break;
    case 1: // 群组
        if (m_lastGroupRefreshTime.isNull() || 
            m_lastGroupRefreshTime.secsTo(currentTime) >= REFRESH_INTERVAL_SECONDS) {
            m_groupManager->refreshGroups();
            m_lastGroupRefreshTime = currentTime;
        }
        break;
    case 2: // 论坛
        if (m_lastForumRefreshTime.isNull() || 
            m_lastForumRefreshTime.secsTo(currentTime) >= REFRESH_INTERVAL_SECONDS) {
            m_forumWidget->refreshPosts();
            m_lastForumRefreshTime = currentTime;
        }
        break;
    }
}

void MainWindow::updateUserMapping()
{
    // 为论坛组件设置当前用户的映射
    QMap<int, QString> userMapping;
    userMapping[m_userId] = m_username;
    
    m_forumWidget->setUserIdToNameMap(userMapping);
    m_postDetail->setUserIdToNameMap(userMapping);
}

void MainWindow::onContactsLoaded(const QMap<int, QString> &userMap)
{
    // 完整的用户映射，包括联系人和当前用户
    QMap<int, QString> completeMapping = userMap;
    completeMapping[m_userId] = m_username;  // 添加当前用户
    
    // 更新论坛组件的用户映射
    m_forumWidget->setUserIdToNameMap(completeMapping);
    m_postDetail->setUserIdToNameMap(completeMapping);
    
    // 更新聊天组件的用户映射
    m_chatWindow->setUserIdToNameMap(completeMapping);
    m_groupChatWindow->setUserIdToNameMap(completeMapping);
}

void MainWindow::onGroupOperationCompleted()
{
    // 群组操作（创建、加入、退出）完成后，刷新群组列表
    m_groupManager->refreshGroups();
    m_lastGroupRefreshTime = QDateTime::currentDateTime(); // 更新刷新时间
}
