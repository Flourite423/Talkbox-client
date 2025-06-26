#include "forummainwidget.h"
#include "../apiclient.h"
#include <QStackedWidget>
#include <QListWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>
#include <QInputDialog>
#include <QSplitter>

ForumMainWidget::ForumMainWidget(ApiClient *apiClient, QWidget *parent)
    : QWidget(parent)
    , m_apiClient(apiClient)
    , m_currentUserId(-1)
    , m_currentPostId(-1)
{
    setupUI();
    
    // 连接API信号
    connect(m_apiClient, &ApiClient::postsReceived, this, &ForumMainWidget::onPostsReceived);
    connect(m_apiClient, &ApiClient::postDetailReceived, this, &ForumMainWidget::onPostDetailReceived);
    connect(m_apiClient, &ApiClient::repliesReceived, this, &ForumMainWidget::onRepliesReceived);
    connect(m_apiClient, &ApiClient::postCreated, this, &ForumMainWidget::onPostCreated);
    connect(m_apiClient, &ApiClient::replyCreated, this, &ForumMainWidget::onReplyCreated);
}

ForumMainWidget::~ForumMainWidget()
{
}

void ForumMainWidget::setupUI()
{
    setStyleSheet(R"(
        QWidget {
            background-color: #f5f5f5;
            font-family: 'Microsoft YaHei', Arial, sans-serif;
        }
        
        QListWidget {
            border: 1px solid #ddd;
            background-color: white;
            selection-background-color: #007acc;
        }
        
        QListWidget::item {
            padding: 12px;
            border-bottom: 1px solid #eee;
        }
        
        QListWidget::item:hover {
            background-color: #f0f8ff;
        }
        
        QTextEdit {
            border: 1px solid #ddd;
            background-color: white;
            padding: 10px;
        }
        
        QLineEdit {
            border: 1px solid #ddd;
            padding: 8px;
            border-radius: 4px;
        }
        
        QPushButton {
            background-color: #007acc;
            color: white;
            border: none;
            border-radius: 4px;
            padding: 8px 16px;
            font-weight: bold;
        }
        
        QPushButton:hover {
            background-color: #005999;
        }
        
        QPushButton:pressed {
            background-color: #004d7a;
        }
        
        QLabel {
            color: #333;
        }
    )");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // 创建堆叠窗口
    m_stackedWidget = new QStackedWidget();
    mainLayout->addWidget(m_stackedWidget);
    
    // 设置帖子列表页面
    m_postListPage = new QWidget();
    QVBoxLayout *listLayout = new QVBoxLayout(m_postListPage);
    
    // 顶部工具栏
    QHBoxLayout *toolbarLayout = new QHBoxLayout();
    
    QLabel *titleLabel = new QLabel("Talkbox 论坛");
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #007acc;");
    toolbarLayout->addWidget(titleLabel);
    
    toolbarLayout->addStretch();
    
    m_createPostButton = new QPushButton("发布新帖");
    m_chatButton = new QPushButton("聊天");
    m_logoutButton = new QPushButton("退出登录");
    m_logoutButton->setStyleSheet("QPushButton { background-color: #dc3545; } QPushButton:hover { background-color: #c82333; }");
    
    toolbarLayout->addWidget(m_createPostButton);
    toolbarLayout->addWidget(m_chatButton);
    toolbarLayout->addWidget(m_logoutButton);
    
    listLayout->addLayout(toolbarLayout);
    
    // 帖子列表
    m_postsList = new QListWidget();
    listLayout->addWidget(m_postsList);
    
    m_stackedWidget->addWidget(m_postListPage);
    
    // 设置帖子详情页面
    m_postDetailPage = new QWidget();
    QVBoxLayout *detailLayout = new QVBoxLayout(m_postDetailPage);
    
    // 顶部工具栏
    QHBoxLayout *topToolbar = new QHBoxLayout();
    m_backButton = new QPushButton("← 返回列表");
    topToolbar->addWidget(m_backButton);
    topToolbar->addStretch();
    detailLayout->addLayout(topToolbar);
    
    // 帖子信息
    m_postTitleLabel = new QLabel();
    m_postTitleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #333; margin: 10px 0;");
    detailLayout->addWidget(m_postTitleLabel);
    
    QHBoxLayout *postInfoLayout = new QHBoxLayout();
    m_postAuthorLabel = new QLabel();
    m_postTimeLabel = new QLabel();
    m_postAuthorLabel->setStyleSheet("color: #666;");
    m_postTimeLabel->setStyleSheet("color: #666;");
    postInfoLayout->addWidget(m_postAuthorLabel);
    postInfoLayout->addWidget(m_postTimeLabel);
    postInfoLayout->addStretch();
    detailLayout->addLayout(postInfoLayout);
    
    // 帖子内容
    m_postContentDisplay = new QTextEdit();
    m_postContentDisplay->setReadOnly(true);
    m_postContentDisplay->setMaximumHeight(200);
    detailLayout->addWidget(m_postContentDisplay);
    
    // 回复标题
    QLabel *repliesLabel = new QLabel("回复列表");
    repliesLabel->setStyleSheet("font-size: 16px; font-weight: bold; margin-top: 20px;");
    detailLayout->addWidget(repliesLabel);
    
    // 回复列表
    m_repliesList = new QListWidget();
    detailLayout->addWidget(m_repliesList);
    
    // 回复输入
    QHBoxLayout *replyLayout = new QHBoxLayout();
    m_replyInput = new QLineEdit();
    m_replyInput->setPlaceholderText("输入回复内容...");
    m_replyButton = new QPushButton("回复");
    
    replyLayout->addWidget(m_replyInput);
    replyLayout->addWidget(m_replyButton);
    detailLayout->addLayout(replyLayout);
    
    m_stackedWidget->addWidget(m_postDetailPage);
    
    // 连接信号
    connect(m_postsList, &QListWidget::itemDoubleClicked, this, &ForumMainWidget::onPostSelected);
    connect(m_createPostButton, &QPushButton::clicked, this, &ForumMainWidget::onCreatePostClicked);
    connect(m_chatButton, &QPushButton::clicked, this, &ForumMainWidget::onShowChatRequested);
    connect(m_logoutButton, &QPushButton::clicked, this, &ForumMainWidget::onLogoutRequested);
    connect(m_backButton, &QPushButton::clicked, this, &ForumMainWidget::onBackToListClicked);
    connect(m_replyButton, &QPushButton::clicked, this, &ForumMainWidget::onReplyPostClicked);
    connect(m_replyInput, &QLineEdit::returnPressed, this, &ForumMainWidget::onReplyPostClicked);
    
    // 显示帖子列表
    showPostList();
}

void ForumMainWidget::setCurrentUser(const QString &username, int userId)
{
    m_currentUsername = username;
    m_currentUserId = userId;
    
    // 刷新帖子列表
    refreshPosts();
}

void ForumMainWidget::refreshPosts()
{
    m_apiClient->getPosts();
}

void ForumMainWidget::onPostsReceived(const QJsonArray &posts)
{
    m_posts = posts;
    m_postsList->clear();
    
    for (const QJsonValue &value : posts) {
        QJsonObject post = value.toObject();
        QString title = post["title"].toString();
        QString username = post["username"].toString();
        QString timestamp = post["timestamp"].toString();
        int postId = post["post_id"].toInt();
        
        QString displayText = QString("%1\n作者: %2 | 时间: %3")
                             .arg(title, username, timestamp);
        
        QListWidgetItem *item = new QListWidgetItem(displayText);
        item->setData(Qt::UserRole, postId);
        m_postsList->addItem(item);
    }
}

void ForumMainWidget::onPostDetailReceived(const QJsonObject &post)
{
    m_currentPost = post;
    updatePostDetail(post);
    
    // 获取回复
    int postId = post["post_id"].toInt();
    m_apiClient->getPostReplies(postId);
}

void ForumMainWidget::onRepliesReceived(const QJsonArray &replies)
{
    m_currentReplies = replies;
    updateRepliesList(replies);
}

void ForumMainWidget::onPostCreated()
{
    QMessageBox::information(this, "成功", "帖子发布成功！");
    refreshPosts();
}

void ForumMainWidget::onReplyCreated()
{
    QMessageBox::information(this, "成功", "回复发布成功！");
    m_replyInput->clear();
    
    // 刷新回复列表
    if (m_currentPostId > 0) {
        m_apiClient->getPostReplies(m_currentPostId);
    }
}

void ForumMainWidget::onPostSelected()
{
    QListWidgetItem *item = m_postsList->currentItem();
    if (!item) return;
    
    int postId = item->data(Qt::UserRole).toInt();
    showPostDetail(postId);
}

void ForumMainWidget::onCreatePostClicked()
{
    bool ok;
    QString title = QInputDialog::getText(this, "发布新帖", 
                                        "帖子标题:", QLineEdit::Normal, "", &ok);
    if (ok && !title.isEmpty()) {
        QString content = QInputDialog::getMultiLineText(this, "发布新帖", 
                                                       "帖子内容:", "", &ok);
        if (ok && !content.isEmpty()) {
            m_apiClient->createPost(title, content);
        }
    }
}

void ForumMainWidget::onReplyPostClicked()
{
    QString content = m_replyInput->text().trimmed();
    if (content.isEmpty() || m_currentPostId <= 0) return;
    
    m_apiClient->replyPost(m_currentPostId, content);
}

void ForumMainWidget::onBackToListClicked()
{
    showPostList();
}

void ForumMainWidget::onShowChatRequested()
{
    emit showChatRequested();
}

void ForumMainWidget::onLogoutRequested()
{
    emit logoutRequested();
}

void ForumMainWidget::showPostList()
{
    m_stackedWidget->setCurrentWidget(m_postListPage);
    m_currentPostId = -1;
}

void ForumMainWidget::showPostDetail(int postId)
{
    m_currentPostId = postId;
    m_apiClient->getPostDetail(postId);
    m_stackedWidget->setCurrentWidget(m_postDetailPage);
}

void ForumMainWidget::updatePostDetail(const QJsonObject &post)
{
    QString title = post["title"].toString();
    QString username = post["username"].toString();
    QString timestamp = post["timestamp"].toString();
    QString content = post["content"].toString();
    
    m_postTitleLabel->setText(title);
    m_postAuthorLabel->setText(QString("作者: %1").arg(username));
    m_postTimeLabel->setText(QString("发布时间: %1").arg(timestamp));
    m_postContentDisplay->setPlainText(content);
}

void ForumMainWidget::updateRepliesList(const QJsonArray &replies)
{
    m_repliesList->clear();
    
    for (const QJsonValue &value : replies) {
        QJsonObject reply = value.toObject();
        QString username = reply["username"].toString();
        QString content = reply["content"].toString();
        QString timestamp = reply["timestamp"].toString();
        
        QString displayText = QString("%1\n%2\n回复时间: %3")
                             .arg(username, content, timestamp);
        
        QListWidgetItem *item = new QListWidgetItem(displayText);
        m_repliesList->addItem(item);
    }
}
