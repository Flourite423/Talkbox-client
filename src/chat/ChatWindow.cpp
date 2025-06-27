#include "ChatWindow.h"
#include "ui_ChatWindow.h"
#include <QJsonArray>
#include <QJsonObject>

ChatWindow::ChatWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatWindow)
    , m_httpClient(nullptr)
    , m_currentUserId(-1)
    , m_currentGroupId(-1)
    , m_isGroupChat(false)
{
    ui->setupUi(this);
    
    connect(ui->sendButton, &QPushButton::clicked, this, &ChatWindow::onSendClicked);
    connect(ui->refreshButton, &QPushButton::clicked, this, &ChatWindow::refreshMessages);
    connect(ui->messageLineEdit, &QLineEdit::returnPressed, this, &ChatWindow::onSendClicked);
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::setHttpClient(HttpClient *client)
{
    m_httpClient = client;
    if (m_httpClient) {
        connect(m_httpClient, &HttpClient::responseReceived, this, &ChatWindow::onHttpResponse);
    }
}

void ChatWindow::setCurrentUsername(const QString &username)
{
    m_currentUsername = username;
}

void ChatWindow::startPrivateChat(int userId, const QString &username)
{
    m_currentUserId = userId;
    m_currentGroupId = -1;
    m_isGroupChat = false;
    m_chatTitle = "私聊 - " + username;
    
    ui->chatTitleLabel->setText(m_chatTitle);
    ui->messagesTextEdit->clear();
    refreshMessages();
}

void ChatWindow::startGroupChat(int groupId, const QString &groupName)
{
    m_currentGroupId = groupId;
    m_currentUserId = -1;
    m_isGroupChat = true;
    m_chatTitle = "群聊 - " + groupName;
    
    ui->chatTitleLabel->setText(m_chatTitle);
    ui->messagesTextEdit->clear();
    refreshMessages();
}

void ChatWindow::refreshMessages()
{
    if (!m_httpClient) return;
    
    if (m_isGroupChat && m_currentGroupId != -1) {
        QJsonObject params;
        params["username"] = m_currentUsername;
        params["group_id"] = m_currentGroupId;
        m_httpClient->get("/api/get_group_messages", params);
    } else if (!m_isGroupChat) {
        QJsonObject params;
        params["username"] = m_currentUsername;
        m_httpClient->get("/api/get_messages", params);
    }
}

void ChatWindow::onSendClicked()
{
    QString message = ui->messageLineEdit->text().trimmed();
    if (message.isEmpty() || !m_httpClient) return;
    
    QJsonObject data;
    data["username"] = m_currentUsername;
    data["content"] = message;
    data["type"] = "text";
    
    if (m_isGroupChat && m_currentGroupId != -1) {
        data["group_id"] = m_currentGroupId;
    } else if (!m_isGroupChat && m_currentUserId != -1) {
        data["receiver_id"] = m_currentUserId;
    } else {
        return;
    }
    
    m_httpClient->post("/api/send_message", data);
    ui->messageLineEdit->clear();
}

void ChatWindow::onHttpResponse(const QJsonObject &response)
{
    if (response["status"].toString() == "success") {
        // 检查是否是获取消息的响应
        QJsonValue dataValue = response["data"];
        if (dataValue.isArray()) {
            // 显示消息列表
            ui->messagesTextEdit->clear();
            QJsonArray messages = dataValue.toArray();
            
            for (const auto &value : messages) {
                QJsonObject msg = value.toObject();
                int senderId = msg["sender_id"].toInt();
                QString content = msg["content"].toString();
                QString timestamp = msg["timestamp"].toString();
                
                // 过滤消息：私聊时只显示与当前用户相关的消息
                if (m_isGroupChat) {
                    int groupId = msg["group_id"].toInt();
                    if (groupId == m_currentGroupId) {
                        QString displayText = QString("[%1] 用户%2: %3")
                                            .arg(timestamp)
                                            .arg(senderId)
                                            .arg(content);
                        ui->messagesTextEdit->append(displayText);
                    }
                } else {
                    int receiverId = msg["receiver_id"].toInt();
                    if ((senderId == m_currentUserId) || (receiverId == m_currentUserId)) {
                        QString displayText = QString("[%1] 用户%2: %3")
                                            .arg(timestamp)
                                            .arg(senderId)
                                            .arg(content);
                        ui->messagesTextEdit->append(displayText);
                    }
                }
            }
        } else if (dataValue.isString()) {
            // 发送消息成功的响应
            if (dataValue.toString() == "消息发送成功") {
                refreshMessages();
            }
        }
    }
}
