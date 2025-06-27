#include "ChatWindow.h"
#include "ui_ChatWindow.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QMap>
#include <QFileDialog>
#include <QMessageBox>
#include <QScrollBar>
#include <QDateTime>
#include <QTextCursor>
#include <QMouseEvent>
#include <QTextDocument>
#include <QAbstractTextDocumentLayout>
#include <QStandardPaths>
#include <QRegularExpression>
#include <QTextBrowser>
#include <QUrl>

ChatWindow::ChatWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatWindow)
    , m_httpClient(nullptr)
    , m_currentUserId(-1)
    , m_currentGroupId(-1)
    , m_isGroupChat(false)
    , m_refreshTimer(new QTimer(this))
{
    ui->setupUi(this);
    
    connect(ui->sendButton, &QPushButton::clicked, this, &ChatWindow::onSendClicked);
    connect(ui->fileButton, &QPushButton::clicked, this, &ChatWindow::onSendFileClicked);
    connect(ui->refreshButton, &QPushButton::clicked, this, &ChatWindow::refreshMessages);
    connect(ui->messageLineEdit, &QLineEdit::returnPressed, this, &ChatWindow::onSendClicked);
    
    // 安装事件过滤器以处理消息区域的点击
    ui->messagesTextEdit->installEventFilter(this);
    
    // 连接锚点点击信号
    connect(ui->messagesTextEdit, &QTextBrowser::anchorClicked, this, &ChatWindow::onAnchorClicked);
    
    // 设置自动刷新定时器（每5秒刷新一次）
    m_refreshTimer->setInterval(5000);
    connect(m_refreshTimer, &QTimer::timeout, this, &ChatWindow::onAutoRefresh);
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

void ChatWindow::setUserIdToNameMap(const QMap<int, QString> &userMap)
{
    m_userIdToNameMap = userMap;
}

QString ChatWindow::getUserDisplayName(int userId)
{
    // 如果在映射中找到用户名，返回用户名
    if (m_userIdToNameMap.contains(userId)) {
        return m_userIdToNameMap[userId];
    }
    
    // 否则返回 "用户{ID}" 格式
    return QString("用户%1").arg(userId);
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
    
    // 开始自动刷新
    m_refreshTimer->start();
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
    
    // 开始自动刷新
    m_refreshTimer->start();
}

void ChatWindow::refreshMessages()
{
    if (!m_httpClient) return;
    
    if (m_isGroupChat && m_currentGroupId != -1) {
        QJsonObject params;
        params["username"] = m_currentUsername;
        params["group_id"] = QString::number(m_currentGroupId);
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
        data["group_id"] = QString::number(m_currentGroupId);
    } else if (!m_isGroupChat && m_currentUserId != -1) {
        data["receiver_id"] = QString::number(m_currentUserId);
    } else {
        return;
    }
    
    m_httpClient->post("/api/send_message", data);
    ui->messageLineEdit->clear();
}

void ChatWindow::onSendFileClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "选择文件", "", "所有文件 (*.*)");
    if (filePath.isEmpty() || !m_httpClient) return;
    
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "错误", "无法打开文件");
        return;
    }
    
    QByteArray fileData = file.readAll();
    file.close();
    
    QFileInfo fileInfo(filePath);
    QString fileName = fileInfo.fileName();
    QString utf8Data = QString::fromUtf8(fileData);
    
    // 首先上传文件到服务器
    QJsonObject uploadData;
    uploadData["username"] = m_currentUsername;
    uploadData["filename"] = fileName;
    uploadData["data"] = utf8Data;
    
    // 存储文件名，用于上传成功后发送消息
    m_pendingFileName = fileName;
    
    m_httpClient->post("/api/upload_file", uploadData);
}

void ChatWindow::onHttpResponse(const QJsonObject &response)
{
    if (response["status"].toString() == "success") {
        QJsonValue dataValue = response["data"];
        
        // 检查是否是文件上传成功的响应
        if (dataValue.isString() && dataValue.toString() == "文件上传成功" && !m_pendingFileName.isEmpty()) {
            // 文件上传成功，现在发送文件消息
            QJsonObject messageData;
            messageData["username"] = m_currentUsername;
            messageData["content"] = m_pendingFileName; // 文件名作为消息内容
            messageData["type"] = "file";
            
            if (m_isGroupChat && m_currentGroupId != -1) {
                messageData["group_id"] = QString::number(m_currentGroupId);
            } else if (!m_isGroupChat && m_currentUserId != -1) {
                messageData["receiver_id"] = QString::number(m_currentUserId);
            } else {
                m_pendingFileName.clear();
                return;
            }
            
            m_httpClient->post("/api/send_message", messageData);
            m_pendingFileName.clear(); // 清除待发送状态
            return;
        }
        
        // 检查是否是文件下载响应
        if (dataValue.isString() && !m_downloadingFileName.isEmpty()) {
            // 这是文件下载响应
            QString fileData = dataValue.toString();
            
            // 保存文件
            saveDownloadedFile(m_downloadingFileName, fileData);
            m_downloadingFileName.clear(); // 清除下载状态
            m_downloadSavePath.clear();    // 清除保存路径状态
            return;
        }
        
        // 检查是否是获取消息的响应
        if (dataValue.isArray()) {
            QJsonArray dataArray = dataValue.toArray();
            
            // 检查是否是消息列表响应（必须包含sender_id、content、timestamp字段）
            // 空数组也可能是有效的消息响应（表示没有消息）
            bool isMessageResponse = false;
            
            if (dataArray.isEmpty()) {
                // 空数组可能是消息响应，但需要确认当前是否在聊天模式
                isMessageResponse = (m_currentUserId != -1 || m_currentGroupId != -1);
            } else {
                // 非空数组，检查第一个元素是否包含消息字段
                QJsonObject firstItem = dataArray[0].toObject();
                bool hasBasicFields = firstItem.contains("sender_id") && 
                                    firstItem.contains("content") && 
                                    firstItem.contains("timestamp");
                
                if (hasBasicFields) {
                    // 进一步检查消息类型匹配
                    if (m_isGroupChat && m_currentGroupId != -1) {
                        // 群聊模式：检查是否包含group_id字段
                        isMessageResponse = firstItem.contains("group_id");
                    } else if (!m_isGroupChat && m_currentUserId != -1) {
                        // 私聊模式：检查是否包含receiver_id字段
                        isMessageResponse = firstItem.contains("receiver_id");
                    }
                }
            }
            
            if (isMessageResponse) {
                
                // ...existing code...
                ui->messagesTextEdit->clear();
                QJsonArray messages = dataArray;
                
                // 判断是否是当前用户发送的消息
                int currentUserId = getCurrentUserIdFromUsername();
                
                for (const auto &value : messages) {
                    QJsonObject msg = value.toObject();
                    int senderId = msg["sender_id"].toInt();
                    QString content = msg["content"].toString();
                    QString timestamp = msg["timestamp"].toString();
                    QString type = msg["type"].toString();
                    
                        
                    // ...existing code...
                    QString senderName = getUserDisplayName(senderId);
                    
                    // 判断是否是当前用户发送的消息
                    bool isMyMessage = (senderId == currentUserId);
                    
                    // 过滤消息：私聊时只显示与当前用户相关的消息
                    bool shouldDisplay = false;
                    if (m_isGroupChat) {
                        int groupId = msg["group_id"].toInt();
                        shouldDisplay = (groupId == m_currentGroupId);
                    } else {
                        int receiverId = msg["receiver_id"].toInt();
                        // 判断是否与当前私聊用户相关的消息
                        shouldDisplay = (senderId == m_currentUserId) || (receiverId == m_currentUserId);
                    }
                    
                    if (shouldDisplay) {
                        QString displayText;
                        QString timeStr = formatTimestamp(timestamp);
                        
                        if (isMyMessage) {
                            // 自己的消息
                            if (type == "file") {
                                displayText = QString("<div style='margin: 3px 0; color: #333;'>"
                                                    "<span style='font-size: 11px; color: #666;'>[%1]</span> "
                                                    "<span style='color: #007ACC; font-weight: bold;'>我:</span> "
                                                    "<a href='download://%2' style='color: #0066CC; text-decoration: underline;' title='点击下载文件'>📎 %2</a>"
                                                    "</div>")
                                            .arg(timeStr)
                                            .arg(content);
                            } else {
                                displayText = QString("<div style='margin: 3px 0; color: #333;'>"
                                                    "<span style='font-size: 11px; color: #666;'>[%1]</span> "
                                                    "<span style='color: #007ACC; font-weight: bold;'>我:</span> "
                                                    "<span style='color: #333;'>%2</span>"
                                                    "</div>")
                                            .arg(timeStr)
                                            .arg(content);
                            }
                        } else {
                            // 别人的消息
                            if (type == "file") {
                                displayText = QString("<div style='margin: 3px 0; color: #333;'>"
                                                    "<span style='font-size: 11px; color: #666;'>[%1]</span> "
                                                    "<span style='color: #D2691E; font-weight: bold;'>%2:</span> "
                                                    "<a href='download://%3' style='color: #0066CC; text-decoration: underline;' title='点击下载文件'>📎 %3</a>"
                                                    "</div>")
                                            .arg(timeStr)
                                            .arg(senderName)
                                            .arg(content);
                            } else {
                                displayText = QString("<div style='margin: 3px 0; color: #333;'>"
                                                    "<span style='font-size: 11px; color: #666;'>[%1]</span> "
                                                    "<span style='color: #D2691E; font-weight: bold;'>%2:</span> "
                                                    "<span style='color: #333;'>%3</span>"
                                                    "</div>")
                                            .arg(timeStr)
                                            .arg(senderName)
                                            .arg(content);
                            }
                        }
                        ui->messagesTextEdit->insertHtml(displayText);
                        ui->messagesTextEdit->insertPlainText("\n"); // 添加换行
                        ui->messagesTextEdit->moveCursor(QTextCursor::End);
                    }
                }
                scrollToBottom();
            } else {
                // 忽略其他数组响应（如联系人列表、群组列表、论坛帖子等）
            }
        } else if (dataValue.isString()) {
            // 发送消息成功的响应
            if (dataValue.toString() == "消息发送成功") {
                refreshMessages();
            }
        }
    } else {
        // 处理错误
        QString errorMsg = response["message"].toString();
        if (!errorMsg.isEmpty()) {
            QMessageBox::warning(this, "错误", errorMsg);
        }
        
        // 如果是文件下载失败，清除下载状态
        if (!m_downloadingFileName.isEmpty()) {
            m_downloadingFileName.clear();
            m_downloadSavePath.clear();
        }
    }
}

void ChatWindow::onAutoRefresh()
{
    // 自动刷新消息
    refreshMessages();
}

QString ChatWindow::formatTimestamp(const QString &timestamp)
{
    // 尝试解析时间戳格式
    QDateTime dateTime = QDateTime::fromString(timestamp, Qt::ISODate);
    if (!dateTime.isValid()) {
        // 如果ISO格式失败，尝试其他格式
        dateTime = QDateTime::fromString(timestamp, "yyyy-MM-dd hh:mm:ss");
    }
    
    if (dateTime.isValid()) {
        QDateTime now = QDateTime::currentDateTime();
        QDate today = now.date();
        QDate msgDate = dateTime.date();
        
        if (msgDate == today) {
            // 今天的消息只显示时间
            return dateTime.toString("hh:mm");
        } else if (msgDate == today.addDays(-1)) {
            // 昨天的消息
            return "昨天 " + dateTime.toString("hh:mm");
        } else {
            // 更早的消息显示日期和时间
            return dateTime.toString("MM-dd hh:mm");
        }
    }
    
    // 如果解析失败，返回原始字符串
    return timestamp;
}

void ChatWindow::scrollToBottom()
{
    QScrollBar *scrollBar = ui->messagesTextEdit->verticalScrollBar();
    scrollBar->setValue(scrollBar->maximum());
}

int ChatWindow::getCurrentUserIdFromUsername()
{
    // 在用户映射中查找当前用户名对应的用户ID
    for (auto it = m_userIdToNameMap.begin(); it != m_userIdToNameMap.end(); ++it) {
        if (it.value() == m_currentUsername) {
            return it.key();
        }
    }
    
    // 如果没找到，返回-1
    return -1;
}

bool ChatWindow::eventFilter(QObject *obj, QEvent *event)
{
    // 现在主要使用锚点点击处理文件下载，保留事件过滤器作为备用
    Q_UNUSED(obj)
    Q_UNUSED(event)
    return QWidget::eventFilter(obj, event);
}

void ChatWindow::onMessageTextEditClicked()
{
    // 这个函数现在不需要了，但保留以防编译错误
}

void ChatWindow::onAnchorClicked(const QUrl &url)
{
    QString scheme = url.scheme();
    QString fileName;
    
    // 检查是否是下载链接
    if (scheme == "download") {
        // 从URL中提取文件名
        QString urlString = url.toString();
        if (urlString.startsWith("download://")) {
            fileName = urlString.mid(11); // 移除 "download://" 前缀
        } else {
            fileName = url.path();
            if (fileName.startsWith("/")) {
                fileName = fileName.mid(1);
            }
        }
        
        if (!fileName.isEmpty()) {
            downloadFile(fileName);
        }
    }
}

void ChatWindow::downloadFile(const QString &fileName)
{
    if (!m_httpClient) {
        QMessageBox::warning(this, "错误", "HTTP客户端未初始化");
        return;
    }
    
    // 先弹出保存对话框让用户选择保存位置（论坛模式）
    QString saveDir = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    if (saveDir.isEmpty()) {
        saveDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    }
    
    QString savePath = QFileDialog::getSaveFileName(this, 
                                                   "保存文件", 
                                                   saveDir + "/" + fileName, 
                                                   "所有文件 (*.*)");
    
    if (savePath.isEmpty()) {
        return; // 用户取消了保存
    }
    
    // 记录下载状态
    m_downloadingFileName = fileName;
    m_downloadSavePath = savePath;
    
    // 使用HttpClient的downloadFile方法（与论坛保持一致）
    m_httpClient->downloadFile("/api/download_file", fileName);
}

void ChatWindow::saveDownloadedFile(const QString &/*fileName*/, const QString &fileData)
{
    // 使用预先选择的保存路径（论坛模式）
    if (m_downloadSavePath.isEmpty()) {
        return;
    }
    
    // 将utf8格式的文件数据转换为字节数组并保存文件
    QByteArray fileDataBytes = fileData.toUtf8();
    
    QFile file(m_downloadSavePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(fileDataBytes);
        file.close();
        QMessageBox::information(this, "成功", "文件已保存到: " + m_downloadSavePath);
    } else {
        QMessageBox::warning(this, "错误", "无法保存文件: " + file.errorString());
    }
}
