#include "CreatePost.h"
#include "ui_CreatePost.h"
#include <QMessageBox>
#include <QTimer>

CreatePost::CreatePost(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreatePost)
    , m_httpClient(nullptr)
    , m_isCreatingPost(false)
{
    ui->setupUi(this);
    
    connect(ui->createButton, &QPushButton::clicked, this, &CreatePost::onCreateClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    
    // 设置窗口属性
    setModal(true);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    
    // 设置初始焦点
    ui->titleLineEdit->setFocus();
    
    // 连接快捷键
    ui->titleLineEdit->setMaxLength(200);
    ui->contentTextEdit->setMaximumHeight(200);
    
    // 设置提示文本
    ui->titleLineEdit->setPlaceholderText("请输入帖子标题（不超过200字符）");
    ui->contentTextEdit->setPlaceholderText("请输入帖子内容，支持多行文本...");
}

CreatePost::~CreatePost()
{
    // 断开所有连接，避免悬空指针
    if (m_httpClient) {
        disconnect(m_httpClient, nullptr, this, nullptr);
    }
    delete ui;
}

void CreatePost::setHttpClient(HttpClient *client)
{
    // 如果之前已经连接了，先断开
    if (m_httpClient) {
        disconnect(m_httpClient, nullptr, this, nullptr);
    }
    
    m_httpClient = client;
    if (m_httpClient) {
        // 使用 Qt::UniqueConnection 避免重复连接
        connect(m_httpClient, &HttpClient::responseReceived, this, &CreatePost::onHttpResponse, Qt::UniqueConnection);
        connect(m_httpClient, &HttpClient::errorOccurred, this, &CreatePost::onHttpError, Qt::UniqueConnection);
    }
}

void CreatePost::onCreateClicked()
{
    if (!validateInput()) {
        return;
    }
    
    QString title = ui->titleLineEdit->text().trimmed();
    QString content = ui->contentTextEdit->toPlainText().trimmed();
    
    if (!m_httpClient) {
        ui->statusLabel->setText("网络组件未初始化");
        ui->statusLabel->setStyleSheet("color: red;");
        return;
    }
    
    setLoading(true);
    m_isCreatingPost = true;  // 设置创建状态
    ui->statusLabel->setText("正在发布帖子...");
    ui->statusLabel->setStyleSheet("color: blue;");
    
    QJsonObject data;
    data["title"] = title;
    data["content"] = content;
    
    m_httpClient->post("/api/create_post", data);
}

void CreatePost::onHttpResponse(const QJsonObject &response)
{
    // 只处理创建帖子时的响应
    if (!m_isCreatingPost) {
        return;
    }
    
    m_isCreatingPost = false;  // 重置状态
    setLoading(false);
    
    // 检查响应是否是创建帖子的响应（通过检查返回的数据结构）
    if (response.contains("status")) {
        if (response["status"].toString() == "success") {
            // 检查是否是创建帖子成功的响应
            QJsonValue dataValue = response["data"];
            if (dataValue.isString() && dataValue.toString().contains("发帖成功")) {
                ui->statusLabel->setText("✅ 发布成功！");
                ui->statusLabel->setStyleSheet("color: green;");
                
                emit postCreated();
                
                // 延迟关闭对话框，让用户看到成功信息
                QTimer::singleShot(1000, this, [this]() {
                    accept();
                });
                return;
            }
        }
        
        // 处理错误响应
        QString errorMsg = response["data"].toString();
        ui->statusLabel->setText("❌ 发布失败: " + errorMsg);
        ui->statusLabel->setStyleSheet("color: red;");
    }
}

void CreatePost::onHttpError(const QString &error)
{
    // 只处理创建帖子时的错误
    if (!m_isCreatingPost) {
        return;
    }
    
    m_isCreatingPost = false;  // 重置状态
    setLoading(false);
    ui->statusLabel->setText("🌐 网络错误: " + error);
    ui->statusLabel->setStyleSheet("color: red;");
}

bool CreatePost::validateInput()
{
    QString title = ui->titleLineEdit->text().trimmed();
    QString content = ui->contentTextEdit->toPlainText().trimmed();
    
    ui->statusLabel->setStyleSheet("");
    
    if (title.isEmpty()) {
        ui->statusLabel->setText("⚠️ 请输入帖子标题");
        ui->statusLabel->setStyleSheet("color: orange;");
        ui->titleLineEdit->setFocus();
        return false;
    }
    
    if (title.length() < 3) {
        ui->statusLabel->setText("⚠️ 标题至少需要3个字符");
        ui->statusLabel->setStyleSheet("color: orange;");
        ui->titleLineEdit->setFocus();
        return false;
    }
    
    if (content.isEmpty()) {
        ui->statusLabel->setText("⚠️ 请输入帖子内容");
        ui->statusLabel->setStyleSheet("color: orange;");
        ui->contentTextEdit->setFocus();
        return false;
    }
    
    if (content.length() < 10) {
        ui->statusLabel->setText("⚠️ 内容至少需要10个字符");
        ui->statusLabel->setStyleSheet("color: orange;");
        ui->contentTextEdit->setFocus();
        return false;
    }
    
    if (title.length() > 200) {
        ui->statusLabel->setText("⚠️ 标题不能超过200个字符");
        ui->statusLabel->setStyleSheet("color: orange;");
        ui->titleLineEdit->setFocus();
        return false;
    }
    
    if (content.length() > 5000) {
        ui->statusLabel->setText("⚠️ 内容不能超过5000个字符");
        ui->statusLabel->setStyleSheet("color: orange;");
        ui->contentTextEdit->setFocus();
        return false;
    }
    
    return true;
}

void CreatePost::setLoading(bool loading)
{
    ui->createButton->setEnabled(!loading);
    ui->cancelButton->setEnabled(!loading);
    ui->titleLineEdit->setEnabled(!loading);
    ui->contentTextEdit->setEnabled(!loading);
    
    if (loading) {
        ui->createButton->setText("发布中...");
    } else {
        ui->createButton->setText("发布");
    }
}
