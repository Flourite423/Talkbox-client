#include "ForumWidget.h"
#include "ui_ForumWidget.h"
#include "CreatePost.h"
#include <QListWidgetItem>
#include <QJsonArray>
#include <QMap>

ForumWidget::ForumWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ForumWidget)
    , m_httpClient(nullptr)
{
    ui->setupUi(this);
    
    connect(ui->createPostButton, &QPushButton::clicked, this, &ForumWidget::onCreatePostClicked);
    connect(ui->refreshButton, &QPushButton::clicked, this, &ForumWidget::onRefreshClicked);
    connect(ui->postListWidget, &QListWidget::itemDoubleClicked, this, &ForumWidget::onPostClicked);
}

ForumWidget::~ForumWidget()
{
    delete ui;
}

void ForumWidget::setHttpClient(HttpClient *client)
{
    m_httpClient = client;
    if (m_httpClient) {
        connect(m_httpClient, &HttpClient::responseReceived, this, &ForumWidget::onHttpResponse);
    }
}

void ForumWidget::refreshPosts()
{
    if (m_httpClient) {
        m_httpClient->get("/api/get_posts");
    }
}

void ForumWidget::onCreatePostClicked()
{
    CreatePost *createDialog = new CreatePost(this);
    createDialog->setHttpClient(m_httpClient);
    createDialog->setCurrentUsername(m_currentUsername);
    
    // 连接帖子创建成功信号
    connect(createDialog, &CreatePost::postCreated, this, &ForumWidget::refreshPosts);
    
    // 连接对话框完成信号，确保正确清理
    connect(createDialog, &QDialog::finished, createDialog, &QDialog::deleteLater);
    
    // 使用非阻塞方式显示对话框
    createDialog->show();
    createDialog->raise();
    createDialog->activateWindow();
}

void ForumWidget::onRefreshClicked()
{
    refreshPosts();
}

void ForumWidget::onPostClicked()
{
    QListWidgetItem *item = ui->postListWidget->currentItem();
    if (item) {
        int postId = item->data(Qt::UserRole).toInt();
        if (postId > 0) { // 确保不是特殊项目（如空列表提示）
            QString title = item->data(Qt::UserRole + 1).toString();
            if (title.isEmpty()) {
                // 如果没有存储标题，从显示文本中提取
                title = item->text().split("\n")[0].replace("📝 ", "");
            }
            emit postSelected(postId, title);
        }
    }
}

void ForumWidget::onHttpResponse(const QJsonObject &response)
{
    if (response["status"].toString() == "success") {
        QJsonValue dataValue = response["data"];
        
        // 只处理帖子列表相关的响应，忽略其他响应（如创建帖子的响应）
        if (dataValue.isArray()) {
            QJsonArray dataArray = dataValue.toArray();
            
            // 检查是否是帖子列表响应
            if (dataArray.isEmpty() || 
                (!dataArray.isEmpty() && dataArray[0].toObject().contains("post_id") && 
                 dataArray[0].toObject().contains("title"))) {
            // 帖子列表响应
            ui->postListWidget->clear();
            QJsonArray posts = dataArray;
            
            if (posts.isEmpty()) {
                QListWidgetItem *emptyItem = new QListWidgetItem("暂无帖子，点击上方按钮发布第一个帖子吧！");
                emptyItem->setData(Qt::UserRole, -1); // 特殊标记
                emptyItem->setFlags(Qt::NoItemFlags); // 不可选择
                emptyItem->setTextAlignment(Qt::AlignCenter);
                ui->postListWidget->addItem(emptyItem);
                return;
            }
            
            for (const auto &value : posts) {
                QJsonObject post = value.toObject();
                int postId = post["post_id"].toInt();
                int userId = post["user_id"].toInt();
                QString title = post["title"].toString();
                QString content = post["content"].toString();
                QString timestamp = post["timestamp"].toString();
                
                // 限制内容显示长度
                QString shortContent = content.length() > 100 ? 
                                     content.left(100) + "..." : content;
                
                // 格式化时间显示
                QString formattedTime = formatTimestamp(timestamp);
                
                QString displayText = QString("📝 %1\n👤 作者: %2  🕒 %3\n💬 %4")
                                    .arg(title)
                                    .arg(getUserDisplayName(userId)) // 使用用户显示名称
                                    .arg(formattedTime)
                                    .arg(shortContent);
                
                QListWidgetItem *item = new QListWidgetItem(displayText);
                item->setData(Qt::UserRole, postId);
                item->setData(Qt::UserRole + 1, title); // 存储标题用于传递
                
                // 添加一些视觉分隔
                item->setSizeHint(QSize(0, 80));
                
                ui->postListWidget->addItem(item);
            }
            } // 结束帖子列表检查
        } else {
            // 忽略非数组响应（可能是其他组件的响应）
            return;
        }
    } else {
        // 显示错误信息
        ui->postListWidget->clear();
        QListWidgetItem *errorItem = new QListWidgetItem("加载帖子失败: " + response["data"].toString());
        errorItem->setFlags(Qt::NoItemFlags);
        errorItem->setTextAlignment(Qt::AlignCenter);
        ui->postListWidget->addItem(errorItem);
    }
}

QString ForumWidget::formatTimestamp(const QString &timestamp)
{
    // 简单的时间格式化，可以根据需要改进
    if (timestamp.contains("Jun 27")) {
        return "今天 " + timestamp.split(" ").last();
    } else if (timestamp.contains("Jun 26")) {
        return "昨天 " + timestamp.split(" ").last();
    }
    return timestamp;
}

void ForumWidget::setCurrentUsername(const QString &username)
{
    m_currentUsername = username;
}

void ForumWidget::setUserIdToNameMap(const QMap<int, QString> &userMap)
{
    m_userIdToNameMap = userMap;
}

QString ForumWidget::getUserDisplayName(int userId)
{
    // 如果在映射中找到用户名，返回用户名
    if (m_userIdToNameMap.contains(userId)) {
        return m_userIdToNameMap[userId];
    }
    
    // 否则返回 "用户{ID}" 格式
    return QString("用户%1").arg(userId);
}
