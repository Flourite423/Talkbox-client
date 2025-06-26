#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_httpClient(new HttpClient(this))
    , m_userId(-1)
{
    ui->setupUi(this);
    setupUI();
    showLoginDialog();
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
    ChatWindow *groupChatWindow = new ChatWindow(this);
    groupChatWindow->setHttpClient(m_httpClient);
    QVBoxLayout *groupChatLayout = new QVBoxLayout(ui->groupChatWidget);
    groupChatLayout->addWidget(groupChatWindow);
    
    // 设置布局 - 论坛标签页
    QVBoxLayout *forumListLayout = new QVBoxLayout(ui->forumListWidget);
    forumListLayout->addWidget(m_forumWidget);
    
    QVBoxLayout *forumDetailLayout = new QVBoxLayout(ui->forumDetailWidget);
    forumDetailLayout->addWidget(m_postDetail);
    
    // 连接信号槽
    connect(m_contactList, &ContactList::contactSelected, this, &MainWindow::onContactSelected);
    connect(m_groupManager, &GroupManager::groupSelected, this, &MainWindow::onGroupSelected);
    connect(m_groupManager, &GroupManager::groupSelected, groupChatWindow, &ChatWindow::startGroupChat);
    connect(m_forumWidget, &ForumWidget::postSelected, this, &MainWindow::onPostSelected);
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);
    connect(ui->actionLogout, &QAction::triggered, this, &MainWindow::onLogoutClicked);
    
    // 连接创建帖子功能
    connect(m_forumWidget, &ForumWidget::postSelected, m_postDetail, &PostDetail::showPost);
    connect(m_createPost, &CreatePost::postCreated, m_forumWidget, &ForumWidget::refreshPosts);
    
    // 状态栏
    ui->statusbar->addWidget(ui->userLabel);
}

void MainWindow::showLoginDialog()
{
    LoginDialog *loginDialog = new LoginDialog(this);
    connect(loginDialog, &LoginDialog::loginSuccessful, this, &MainWindow::onLoginSuccessful);
    
    if (loginDialog->exec() != QDialog::Accepted) {
        // 用户取消登录，退出应用
        close();
    }
    
    loginDialog->deleteLater();
}

void MainWindow::onLoginSuccessful(const QString &token, const QString &username, int userId)
{
    m_token = token;
    m_username = username;
    m_userId = userId;
    
    m_httpClient->setToken(token);
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
    if (m_httpClient && !m_token.isEmpty()) {
        m_httpClient->post("/api/logout", QJsonObject());
    }
    
    m_token.clear();
    m_username.clear();
    m_userId = -1;
    m_httpClient->setToken("");
    
    ui->userLabel->setText("未登录");
    setWindowTitle("Talkbox - 聊天软件");
    
    showLoginDialog();
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

void MainWindow::onPostSelected(int postId, const QString &title)
{
    m_postDetail->showPost(postId, title);
}

void MainWindow::onTabChanged(int index)
{
    // 当切换标签页时刷新相应的数据
    switch (index) {
    case 0: // 聊天
        m_contactList->refreshContacts();
        break;
    case 1: // 群组
        m_groupManager->refreshGroups();
        break;
    case 2: // 论坛
        m_forumWidget->refreshPosts();
        break;
    }
}
