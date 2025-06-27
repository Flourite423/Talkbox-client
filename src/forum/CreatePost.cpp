#include "CreatePost.h"
#include "ui_CreatePost.h"
#include <QMessageBox>
#include <QTimer>
#include <QFileDialog>
#include <QFileInfo>
#include <QStandardPaths>

CreatePost::CreatePost(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreatePost)
    , m_httpClient(nullptr)
    , m_isCreatingPost(false)
{
    ui->setupUi(this);
    
    connect(ui->createButton, &QPushButton::clicked, this, &CreatePost::onCreateClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(ui->selectFileButton, &QPushButton::clicked, this, &CreatePost::onSelectFileClicked);
    connect(ui->clearFileButton, &QPushButton::clicked, this, &CreatePost::onClearFileClicked);
    
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
    
    if (!m_httpClient) {
        ui->statusLabel->setText("网络组件未初始化");
        ui->statusLabel->setStyleSheet("color: red;");
        return;
    }
    
    setLoading(true);
    m_isCreatingPost = true;  // 设置创建状态
    
    // 如果有文件需要上传，先上传文件再创建帖子
    if (!m_selectedFilePath.isEmpty()) {
        ui->statusLabel->setText("正在上传文件...");
        ui->statusLabel->setStyleSheet("color: blue;");
        uploadFileAndCreatePost();
    } else {
        // 直接创建帖子
        ui->statusLabel->setText("正在发布帖子...");
        ui->statusLabel->setStyleSheet("color: blue;");
        
        QString title = ui->titleLineEdit->text().trimmed();
        QString content = ui->contentTextEdit->toPlainText().trimmed();
        
        QJsonObject data;
        data["username"] = m_currentUsername;
        data["title"] = title;
        data["content"] = content;
        
        m_httpClient->post("/api/create_post", data);
    }
}

void CreatePost::uploadFileAndCreatePost()
{
    // 使用HttpClient上传文件
    m_httpClient->uploadFile("/api/upload_file", m_selectedFilePath);
}

void CreatePost::onHttpResponse(const QJsonObject &response)
{
    // 只处理创建帖子时的响应
    if (!m_isCreatingPost) {
        return;
    }
    
    if (response["status"].toString() == "success") {
        // 检查是否是文件上传成功的响应
        QJsonValue dataValue = response["data"];
        if (dataValue.isString() && dataValue.toString() == "文件上传成功") {
            // 文件上传成功，现在创建帖子
            ui->statusLabel->setText("文件上传成功，正在发布帖子...");
            ui->statusLabel->setStyleSheet("color: blue;");
            
            QString title = ui->titleLineEdit->text().trimmed();
            QString content = ui->contentTextEdit->toPlainText().trimmed();
            
            QJsonObject data;
            data["username"] = m_currentUsername;
            data["title"] = title;
            data["content"] = content;
            
            m_httpClient->post("/api/create_post", data);
            return;
        }
        
        // 检查是否是创建帖子成功的响应
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
    m_isCreatingPost = false;  // 重置状态
    setLoading(false);
    QString errorMsg = response["data"].toString();
    ui->statusLabel->setText("❌ 发布失败: " + errorMsg);
    ui->statusLabel->setStyleSheet("color: red;");
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
    ui->selectFileButton->setEnabled(!loading);
    ui->clearFileButton->setEnabled(!loading && !m_selectedFilePath.isEmpty());
    
    if (loading) {
        ui->createButton->setText("发布中...");
    } else {
        ui->createButton->setText("发布");
    }
}

void CreatePost::onSelectFileClicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "选择要上传的文件",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        "所有文件 (*);;图片文件 (*.png *.jpg *.jpeg *.gif *.bmp);;文档文件 (*.txt *.pdf *.doc *.docx)"
    );
    
    if (!fileName.isEmpty()) {
        QFileInfo fileInfo(fileName);
        
        // 检查文件大小（限制为10MB）
        if (fileInfo.size() > 10 * 1024 * 1024) {
            QMessageBox::warning(this, "文件过大", "文件大小不能超过10MB");
            return;
        }
        
        m_selectedFilePath = fileName;
        ui->filePathLineEdit->setText(fileInfo.fileName());
        ui->clearFileButton->setEnabled(true);
        
        // 在内容中添加文件引用
        QString currentContent = ui->contentTextEdit->toPlainText();
        if (!currentContent.isEmpty() && !currentContent.endsWith("\n")) {
            currentContent += "\n";
        }
        currentContent += QString("\n📎 附件: %1").arg(fileInfo.fileName());
        ui->contentTextEdit->setPlainText(currentContent);
    }
}

void CreatePost::onClearFileClicked()
{
    m_selectedFilePath.clear();
    ui->filePathLineEdit->clear();
    ui->clearFileButton->setEnabled(false);
    
    // 从内容中移除文件引用
    QString content = ui->contentTextEdit->toPlainText();
    QStringList lines = content.split('\n');
    QStringList filteredLines;
    
    for (const QString &line : lines) {
        if (!line.startsWith("📎 附件:")) {
            filteredLines.append(line);
        }
    }
    
    ui->contentTextEdit->setPlainText(filteredLines.join('\n').trimmed());
}

void CreatePost::setCurrentUsername(const QString &username)
{
    m_currentUsername = username;
}
