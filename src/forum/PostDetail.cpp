#include "PostDetail.h"
#include "ui_PostDetail.h"
#include <QJsonArray>
#include <QTimer>

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
    data["post_id"] = m_currentPostId;
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
    if (!m_httpClient || m_currentPostId == -1) return;
    
    QJsonObject data;
    data["post_id"] = m_currentPostId;
    
    m_httpClient->post("/api/get_post_replies", data);
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
                            
                            QString formattedContent = QString("👤 作者: 用户%1\n🕒 发布时间: %2\n\n📄 内容:\n%3")
                                                     .arg(userId)
                                                     .arg(timestamp)
                                                     .arg(content);
                            
                            ui->contentTextEdit->setPlainText(formattedContent);
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
        
        QString replyText = QString("🏷️ #%1  👤 用户%2  🕒 %3\n💭 %4\n")
                          .arg(i + 1)
                          .arg(userId)
                          .arg(timestamp)
                          .arg(content);
        
        repliesText += replyText;
        
        if (i < replies.size() - 1) {
            repliesText += "─────────────────────────\n";
        }
    }
    
    ui->repliesTextEdit->setPlainText(repliesText);
}
