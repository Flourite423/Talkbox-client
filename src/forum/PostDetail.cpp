#include "PostDetail.h"
#include "ui_PostDetail.h"
#include <QJsonArray>
#include <QTimer>
#include <QFileDialog>
#include <QStandardPaths>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDebug>
#include <QMap>

PostDetail::PostDetail(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PostDetail)
    , m_httpClient(nullptr)
    , m_currentPostId(-1)
{
    ui->setupUi(this);
    
    connect(ui->replyButton, &QPushButton::clicked, this, &PostDetail::onReplyClicked);
    connect(ui->refreshRepliesButton, &QPushButton::clicked, this, &PostDetail::onRefreshRepliesClicked);
    connect(ui->replyLineEdit, &QLineEdit::returnPressed, this, &PostDetail::onReplyClicked);
    connect(ui->attachmentListWidget, &QListWidget::itemDoubleClicked, this, &PostDetail::onAttachmentClicked);
}

PostDetail::~PostDetail()
{
    delete ui;
}

void PostDetail::setHttpClient(HttpClient *client)
{
    m_httpClient = client;
    if (m_httpClient) {
        connect(m_httpClient, &HttpClient::responseReceived, this, &PostDetail::onHttpResponse);
    }
}

void PostDetail::showPost(int postId, const QString &title)
{
    m_currentPostId = postId;
    ui->titleLabel->setText("📝 " + title);
    ui->contentTextEdit->clear();
    ui->repliesTextEdit->clear();
    ui->replyLineEdit->clear();
    
    // 设置占位符文本
    ui->contentTextEdit->setPlaceholderText("正在加载帖子内容...");
    ui->repliesTextEdit->setPlaceholderText("正在加载回复...");
    
    // 先获取帖子详情，然后加载回复
    loadPostContent();
    loadReplies();
}

void PostDetail::loadPostContent()
{
    if (!m_httpClient || m_currentPostId == -1) return;
    
    // 获取帖子列表，然后找到对应的帖子
    m_httpClient->get("/api/get_posts");
}

void PostDetail::onReplyClicked()
{
    QString content = ui->replyLineEdit->text().trimmed();
    if (content.isEmpty()) {
        // 简单的提示
        ui->replyLineEdit->setPlaceholderText("请输入回复内容");
        return;
    }
    
    if (!m_httpClient || m_currentPostId == -1) return;
    
    // 禁用输入，显示状态
    ui->replyButton->setEnabled(false);
    ui->replyButton->setText("发送中...");
    ui->replyLineEdit->setEnabled(false);
    
    QJsonObject data;
    data["username"] = m_currentUsername;
    data["post_id"] = QString::number(m_currentPostId);
    data["content"] = content;
    
    m_httpClient->post("/api/reply_post", data);
}

void PostDetail::onRefreshRepliesClicked()
{
    ui->refreshRepliesButton->setText("刷新中...");
    ui->refreshRepliesButton->setEnabled(false);
    loadReplies();
}

void PostDetail::loadReplies()
{
    if (!m_httpClient) {
        ui->repliesTextEdit->setPlainText("HTTP客户端未初始化");
        return;
    }
    
    if (m_currentPostId == -1) {
        ui->repliesTextEdit->setPlainText("请先选择一个帖子查看回复");
        return;
    }
    
    // 添加调试信息
    qDebug() << "Loading replies for post ID:" << m_currentPostId;
    
    QJsonObject params;
    params["post_id"] = QString::number(m_currentPostId);
    
    m_httpClient->get("/api/get_post_replies", params);
}

void PostDetail::onHttpResponse(const QJsonObject &response)
{
    // 恢复按钮状态
    ui->replyButton->setEnabled(true);
    ui->replyButton->setText("回复");
    ui->replyLineEdit->setEnabled(true);
    ui->refreshRepliesButton->setEnabled(true);
    ui->refreshRepliesButton->setText("刷新回复");
    
    if (response["status"].toString() == "success") {
        QJsonValue dataValue = response["data"];
        
        if (dataValue.isArray()) {
            QJsonArray dataArray = dataValue.toArray();
            
            // 检查是否是帖子列表（包含post_id字段）还是回复列表（包含reply_id字段）
            if (!dataArray.isEmpty()) {
                QJsonObject firstItem = dataArray[0].toObject();
                
                if (firstItem.contains("post_id") && firstItem.contains("title")) {
                    // 这是帖子列表，查找当前帖子的内容
                    for (const auto &value : dataArray) {
                        QJsonObject post = value.toObject();
                        if (post["post_id"].toInt() == m_currentPostId) {
                            QString content = post["content"].toString();
                            QString timestamp = post["timestamp"].toString();
                            int userId = post["user_id"].toInt();
                            
                            QString userDisplayName = getUserDisplayName(userId);
                            
                            QString formattedContent = QString("👤 作者: %1\n🕒 发布时间: %2\n\n📄 内容:\n%3")
                                                     .arg(userDisplayName)
                                                     .arg(timestamp)
                                                     .arg(content);
                            
                            ui->contentTextEdit->setPlainText(formattedContent);
                            extractAndDisplayAttachments(content); // 提取并显示附件
                            break;
                        }
                    }
                } else if (firstItem.contains("reply_id")) {
                    // 这是回复列表
                    displayReplies(dataArray);
                }
            } else {
                // 空数组
                if (ui->repliesTextEdit->toPlainText().isEmpty()) {
                    ui->repliesTextEdit->setPlainText("暂无回复，成为第一个回复的人吧！");
                }
            }
        } else if (dataValue.isString()) {
            // 回复成功响应
            QString message = dataValue.toString();
            if (message == "回帖成功") {
                ui->replyLineEdit->clear();
                ui->replyLineEdit->setPlaceholderText("回复成功！正在刷新...");
                // 延迟刷新回复列表
                QTimer::singleShot(500, this, &PostDetail::loadReplies);
            }
        }
    } else {
        QString error = response["data"].toString();
        ui->repliesTextEdit->setPlainText("加载失败: " + error);
    }
}

void PostDetail::displayReplies(const QJsonArray &replies)
{
    ui->repliesTextEdit->clear();
    
    if (replies.isEmpty()) {
        ui->repliesTextEdit->setPlainText("暂无回复，成为第一个回复的人吧！");
        return;
    }
    
    QString repliesText = QString("💬 共有 %1 条回复:\n\n").arg(replies.size());
    
    for (int i = 0; i < replies.size(); ++i) {
        QJsonObject reply = replies[i].toObject();
        int userId = reply["user_id"].toInt();
        QString content = reply["content"].toString();
        QString timestamp = reply["timestamp"].toString();
        
        QString userDisplayName = getUserDisplayName(userId);
        
        QString replyText = QString("🏷️ #%1  👤 %2  🕒 %3\n💭 %4\n")
                          .arg(i + 1)
                          .arg(userDisplayName)
                          .arg(timestamp)
                          .arg(content);
        
        repliesText += replyText;
        
        if (i < replies.size() - 1) {
            repliesText += "─────────────────────────\n";
        }
    }
    
    ui->repliesTextEdit->setPlainText(repliesText);
}

void PostDetail::extractAndDisplayAttachments(const QString &content)
{
    ui->attachmentListWidget->clear();
    QStringList lines = content.split('\n');
    QStringList attachments;
    
    // 提取附件信息
    for (const QString &line : lines) {
        if (line.trimmed().startsWith("📎 附件:")) {
            QString filename = line.split("📎 附件:").last().trimmed();
            if (!filename.isEmpty()) {
                attachments.append(filename);
            }
        }
    }
    
    // 显示附件列表
    if (attachments.isEmpty()) {
        ui->attachmentLabel->hide();
        ui->attachmentListWidget->hide();
    } else {
        ui->attachmentLabel->show();
        ui->attachmentListWidget->show();
        
        for (const QString &filename : attachments) {
            QListWidgetItem *item = new QListWidgetItem("📎 " + filename + " (双击下载)");
            item->setData(Qt::UserRole, filename);
            ui->attachmentListWidget->addItem(item);
        }
    }
}

void PostDetail::onAttachmentClicked()
{
    QListWidgetItem *item = ui->attachmentListWidget->currentItem();
    if (item && m_httpClient) {
        QString filename = item->data(Qt::UserRole).toString();
        
        // 提示用户选择保存位置
        QString savePath = QFileDialog::getSaveFileName(
            this,
            "保存文件",
            QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/" + filename,
            "所有文件 (*)"
        );
        
        if (!savePath.isEmpty()) {
            item->setText("📎 " + filename + " (下载中...)");
            m_httpClient->downloadFile("/api/download_file", filename);
        }
    }
}

void PostDetail::setCurrentUsername(const QString &username)
{
    m_currentUsername = username;
}

void PostDetail::setUserIdToNameMap(const QMap<int, QString> &userMap)
{
    m_userIdToNameMap = userMap;
}

QString PostDetail::getUserDisplayName(int userId)
{
    // 如果在映射中找到用户名，返回用户名
    if (m_userIdToNameMap.contains(userId)) {
        return m_userIdToNameMap[userId];
    }
    
    // 否则返回 "用户{ID}" 格式
    return QString("用户%1").arg(userId);
}
