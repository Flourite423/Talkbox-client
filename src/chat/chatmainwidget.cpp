#include "chatmainwidget.h"
#include "../apiclient.h"
#include <QTabWidget>
#include <QListWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>
#include <QInputDialog>
#include <QDateTime>

ChatMainWidget::ChatMainWidget(ApiClient *apiClient, QWidget *parent)
    : QWidget(parent)
    , m_apiClient(apiClient)
    , m_currentUserId(-1)
    , m_currentContactId(-1)
    , m_currentGroupId(-1)
    , m_isGroupChat(false)
{
    setupUI();
    
    // 连接API信号
    connect(m_apiClient, &ApiClient::contactsReceived, this, &ChatMainWidget::onContactsReceived);
    connect(m_apiClient, &ApiClient::groupsReceived, this, &ChatMainWidget::onGroupsReceived);
    connect(m_apiClient, &ApiClient::messagesReceived, this, &ChatMainWidget::onMessagesReceived);
    connect(m_apiClient, &ApiClient::groupMessagesReceived, this, &ChatMainWidget::onGroupMessagesReceived);
    connect(m_apiClient, &ApiClient::messageReceived, this, &ChatMainWidget::onMessageSent);
    connect(m_apiClient, &ApiClient::groupCreated, this, [this](const QJsonObject &) {
        refreshGroups();
    });
    connect(m_apiClient, &ApiClient::groupJoined, this, [this]() {
        refreshGroups();
    });
}

ChatMainWidget::~ChatMainWidget()
{
}

void ChatMainWidget::setupUI()
{
    setStyleSheet(R"(
        QWidget {
            background-color: #f5f5f5;
            font-family: 'Microsoft YaHei', Arial, sans-serif;
        }
        
        QTabWidget::pane {
            border: 1px solid #ddd;
            background-color: white;
        }
        
        QTabBar::tab {
            background-color: #e0e0e0;
            padding: 8px 16px;
            margin-right: 2px;
        }
        
        QTabBar::tab:selected {
            background-color: white;
            border-bottom: 2px solid #007acc;
        }
        
        QListWidget {
            border: 1px solid #ddd;
            background-color: white;
            selection-background-color: #007acc;
        }
        
        QListWidget::item {
            padding: 8px;
            border-bottom: 1px solid #eee;
        }
        
        QListWidget::item:hover {
            background-color: #f0f8ff;
        }
        
        QTextEdit {
            border: 1px solid #ddd;
            background-color: white;
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
    )");

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    
    // 创建分割器
    m_splitter = new QSplitter(Qt::Horizontal, this);
    mainLayout->addWidget(m_splitter);
    
    // 左侧面板
    QWidget *leftPanel = new QWidget();
    leftPanel->setFixedWidth(250);
    leftPanel->setStyleSheet("background-color: white; border-right: 1px solid #ddd;");
    
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    
    // 标签页
    m_tabWidget = new QTabWidget();
    leftLayout->addWidget(m_tabWidget);
    
    // 联系人标签页
    QWidget *contactsTab = new QWidget();
    QVBoxLayout *contactsLayout = new QVBoxLayout(contactsTab);
    
    m_contactsList = new QListWidget();
    contactsLayout->addWidget(m_contactsList);
    
    m_tabWidget->addTab(contactsTab, "联系人");
    
    // 群组标签页
    QWidget *groupsTab = new QWidget();
    QVBoxLayout *groupsLayout = new QVBoxLayout(groupsTab);
    
    m_groupsList = new QListWidget();
    groupsLayout->addWidget(m_groupsList);
    
    QHBoxLayout *groupButtonLayout = new QHBoxLayout();
    m_createGroupButton = new QPushButton("创建群组");
    m_joinGroupButton = new QPushButton("加入群组");
    groupButtonLayout->addWidget(m_createGroupButton);
    groupButtonLayout->addWidget(m_joinGroupButton);
    groupsLayout->addLayout(groupButtonLayout);
    
    m_tabWidget->addTab(groupsTab, "群组");
    
    // 底部按钮
    QVBoxLayout *bottomButtonLayout = new QVBoxLayout();
    m_forumButton = new QPushButton("论坛");
    m_logoutButton = new QPushButton("退出登录");
    m_logoutButton->setStyleSheet("QPushButton { background-color: #dc3545; } QPushButton:hover { background-color: #c82333; }");
    
    bottomButtonLayout->addWidget(m_forumButton);
    bottomButtonLayout->addWidget(m_logoutButton);
    leftLayout->addLayout(bottomButtonLayout);
    
    m_splitter->addWidget(leftPanel);
    
    // 右侧聊天区域
    m_chatArea = new QWidget();
    QVBoxLayout *chatLayout = new QVBoxLayout(m_chatArea);
    
    // 聊天标题
    m_chatTitleLabel = new QLabel("选择联系人或群组开始聊天");
    m_chatTitleLabel->setStyleSheet("font-size: 16px; font-weight: bold; padding: 10px; background-color: #007acc; color: white;");
    chatLayout->addWidget(m_chatTitleLabel);
    
    // 消息显示区域
    m_messagesDisplay = new QTextEdit();
    m_messagesDisplay->setReadOnly(true);
    chatLayout->addWidget(m_messagesDisplay);
    
    // 消息输入区域
    QHBoxLayout *inputLayout = new QHBoxLayout();
    m_messageInput = new QLineEdit();
    m_messageInput->setPlaceholderText("输入消息...");
    m_sendButton = new QPushButton("发送");
    
    inputLayout->addWidget(m_messageInput);
    inputLayout->addWidget(m_sendButton);
    chatLayout->addLayout(inputLayout);
    
    m_splitter->addWidget(m_chatArea);
    
    // 设置分割器比例
    m_splitter->setSizes({250, 600});
    
    // 连接信号
    connect(m_contactsList, &QListWidget::itemClicked, this, &ChatMainWidget::onContactSelected);
    connect(m_groupsList, &QListWidget::itemClicked, this, &ChatMainWidget::onGroupSelected);
    connect(m_sendButton, &QPushButton::clicked, this, &ChatMainWidget::onSendMessageClicked);
    connect(m_messageInput, &QLineEdit::returnPressed, this, &ChatMainWidget::onSendMessageClicked);
    connect(m_createGroupButton, &QPushButton::clicked, this, &ChatMainWidget::onCreateGroupClicked);
    connect(m_joinGroupButton, &QPushButton::clicked, this, &ChatMainWidget::onJoinGroupClicked);
    connect(m_forumButton, &QPushButton::clicked, this, &ChatMainWidget::onShowForumRequested);
    connect(m_logoutButton, &QPushButton::clicked, this, &ChatMainWidget::onLogoutRequested);
}

void ChatMainWidget::setCurrentUser(const QString &username, int userId)
{
    m_currentUsername = username;
    m_currentUserId = userId;
    
    // 刷新数据
    refreshContacts();
    refreshGroups();
}

void ChatMainWidget::refreshContacts()
{
    m_apiClient->getContacts();
}

void ChatMainWidget::refreshGroups()
{
    m_apiClient->getGroups();
}

void ChatMainWidget::refreshMessages()
{
    if (m_isGroupChat && m_currentGroupId > 0) {
        m_apiClient->getGroupMessages(m_currentGroupId);
    } else {
        m_apiClient->getMessages();
    }
}

void ChatMainWidget::onContactsReceived(const QJsonArray &contacts)
{
    m_contacts = contacts;
    m_contactsList->clear();
    
    for (const QJsonValue &value : contacts) {
        QJsonObject contact = value.toObject();
        QString username = contact["username"].toString();
        int userId = contact["user_id"].toInt();
        
        QString displayText = QString("%1 (ID: %2)").arg(username).arg(userId);
        QListWidgetItem *item = new QListWidgetItem(displayText);
        item->setData(Qt::UserRole, userId);
        m_contactsList->addItem(item);
    }
}

void ChatMainWidget::onGroupsReceived(const QJsonArray &groups)
{
    m_groups = groups;
    m_groupsList->clear();
    
    for (const QJsonValue &value : groups) {
        QJsonObject group = value.toObject();
        QString groupName = group["group_name"].toString();
        int groupId = group["group_id"].toInt();
        
        QString displayText = QString("%1 (ID: %2)").arg(groupName).arg(groupId);
        QListWidgetItem *item = new QListWidgetItem(displayText);
        item->setData(Qt::UserRole, groupId);
        m_groupsList->addItem(item);
    }
}

void ChatMainWidget::onMessagesReceived(const QJsonArray &messages)
{
    if (!m_isGroupChat) {
        m_currentMessages = messages;
        updateMessageDisplay();
    }
}

void ChatMainWidget::onGroupMessagesReceived(const QJsonArray &messages)
{
    if (m_isGroupChat) {
        m_currentMessages = messages;
        updateMessageDisplay();
    }
}

void ChatMainWidget::onMessageSent()
{
    m_messageInput->clear();
    refreshMessages();
}

void ChatMainWidget::onContactSelected()
{
    QListWidgetItem *item = m_contactsList->currentItem();
    if (!item) return;
    
    m_currentContactId = item->data(Qt::UserRole).toInt();
    m_currentGroupId = -1;
    m_isGroupChat = false;
    
    m_chatTitleLabel->setText(QString("与 %1 的私聊").arg(item->text().split(" (ID:")[0]));
    refreshMessages();
}

void ChatMainWidget::onGroupSelected()
{
    QListWidgetItem *item = m_groupsList->currentItem();
    if (!item) return;
    
    m_currentGroupId = item->data(Qt::UserRole).toInt();
    m_currentContactId = -1;
    m_isGroupChat = true;
    
    m_chatTitleLabel->setText(QString("群聊: %1").arg(item->text().split(" (ID:")[0]));
    refreshMessages();
}

void ChatMainWidget::onSendMessageClicked()
{
    QString message = m_messageInput->text().trimmed();
    if (message.isEmpty()) return;
    
    if (m_isGroupChat && m_currentGroupId > 0) {
        m_apiClient->sendGroupMessage(m_currentGroupId, message);
    } else if (!m_isGroupChat && m_currentContactId > 0) {
        m_apiClient->sendMessage(m_currentContactId, message);
    }
}

void ChatMainWidget::onCreateGroupClicked()
{
    bool ok;
    QString groupName = QInputDialog::getText(this, "创建群组", 
                                            "群组名称:", QLineEdit::Normal, "", &ok);
    if (ok && !groupName.isEmpty()) {
        QString description = QInputDialog::getText(this, "创建群组", 
                                                  "群组描述:", QLineEdit::Normal, "", &ok);
        if (ok) {
            m_apiClient->createGroup(groupName, description);
        }
    }
}

void ChatMainWidget::onJoinGroupClicked()
{
    bool ok;
    int groupId = QInputDialog::getInt(this, "加入群组", 
                                     "请输入群组ID:", 1, 1, 99999, 1, &ok);
    if (ok) {
        m_apiClient->joinGroup(groupId);
    }
}

void ChatMainWidget::onShowForumRequested()
{
    emit showForumRequested();
}

void ChatMainWidget::onLogoutRequested()
{
    emit logoutRequested();
}

void ChatMainWidget::updateMessageDisplay()
{
    m_messagesDisplay->clear();
    
    for (const QJsonValue &value : m_currentMessages) {
        addMessageToDisplay(value.toObject());
    }
}

void ChatMainWidget::addMessageToDisplay(const QJsonObject &message)
{
    QString senderUsername = message["sender_username"].toString();
    QString content = message["content"].toString();
    QString timestamp = message["timestamp"].toString();
    int senderId = message["sender_id"].toInt();
    
    // 格式化时间
    QString displayTime = timestamp;
    
    // 判断是否是自己发送的消息
    bool isOwnMessage = (senderId == m_currentUserId);
    
    QString messageHtml;
    if (isOwnMessage) {
        messageHtml = QString(
            "<div style='text-align: right; margin: 5px;'>"
            "<span style='color: #666; font-size: 12px;'>%1 %2</span><br>"
            "<span style='background-color: #007acc; color: white; padding: 8px; border-radius: 8px; display: inline-block;'>%3</span>"
            "</div>"
        ).arg(senderUsername, displayTime, content);
    } else {
        messageHtml = QString(
            "<div style='text-align: left; margin: 5px;'>"
            "<span style='color: #666; font-size: 12px;'>%1 %2</span><br>"
            "<span style='background-color: #e0e0e0; color: black; padding: 8px; border-radius: 8px; display: inline-block;'>%3</span>"
            "</div>"
        ).arg(senderUsername, displayTime, content);
    }
    
    m_messagesDisplay->append(messageHtml);
}
