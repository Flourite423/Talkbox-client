#include "LoginDialog.h"
#include "ui_LoginDialog.h"
#include <QMessageBox>
#include <QJsonObject>
#include <QRegularExpression>
#include <QTimer>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
    , m_httpClient(new HttpClient(this))
    , m_isRegistering(false)
{
    ui->setupUi(this);
    setupConnections();
    
    // 初始设置为登录模式
    ui->confirmPasswordLabel->setVisible(false);
    ui->confirmPasswordEdit->setVisible(false);
    ui->modeLabel->setText("用户登录");
    ui->primaryButton->setText("登录");
    ui->secondaryButton->setText("切换到注册");
    ui->switchLabel->setText("<a href=\"#\">还没有账号？点击注册</a>");
    
    // 设置焦点
    ui->usernameEdit->setFocus();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::setupConnections()
{
    // 按钮连接
    connect(ui->primaryButton, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
    connect(ui->secondaryButton, &QPushButton::clicked, this, &LoginDialog::onSwitchToRegister);
    connect(ui->switchLabel, &QLabel::linkActivated, this, &LoginDialog::onSwitchToRegister);
    
    // HTTP客户端连接
    connect(m_httpClient, &HttpClient::responseReceived, this, &LoginDialog::onHttpResponse);
    connect(m_httpClient, &HttpClient::errorOccurred, this, &LoginDialog::onHttpError);
    
    // 回车键支持
    connect(ui->usernameEdit, &QLineEdit::returnPressed, this, &LoginDialog::onLoginClicked);
    connect(ui->passwordEdit, &QLineEdit::returnPressed, this, &LoginDialog::onLoginClicked);
    connect(ui->confirmPasswordEdit, &QLineEdit::returnPressed, this, &LoginDialog::onLoginClicked);
}

void LoginDialog::onLoginClicked()
{
    if (!validateInput()) {
        return;
    }
    
    QString username = ui->usernameEdit->text().trimmed();
    QString password = ui->passwordEdit->text();
    
    setLoading(true);
    ui->statusLabel->setText(m_isRegistering ? "正在注册..." : "正在登录...");
    
    QJsonObject data;
    data["username"] = username;
    data["password"] = password;
    
    QString endpoint = m_isRegistering ? "/api/register" : "/api/login";
    m_httpClient->post(endpoint, data);
}

void LoginDialog::onRegisterClicked()
{
    if (m_isRegistering) {
        onLoginClicked(); // 如果已经在注册模式，执行注册
    } else {
        onSwitchToRegister(); // 否则切换到注册模式
    }
}

void LoginDialog::onSwitchToRegister()
{
    if (m_isRegistering) {
        // 切换到登录模式
        m_isRegistering = false;
        ui->confirmPasswordLabel->setVisible(false);
        ui->confirmPasswordEdit->setVisible(false);
        ui->modeLabel->setText("用户登录");
        ui->primaryButton->setText("登录");
        ui->secondaryButton->setText("切换到注册");
        ui->switchLabel->setText("<a href=\"#\">还没有账号？点击注册</a>");
        setWindowTitle("Talkbox - 登录");
    } else {
        // 切换到注册模式
        m_isRegistering = true;
        ui->confirmPasswordLabel->setVisible(true);
        ui->confirmPasswordEdit->setVisible(true);
        ui->modeLabel->setText("用户注册");
        ui->primaryButton->setText("注册");
        ui->secondaryButton->setText("切换到登录");
        ui->switchLabel->setText("<a href=\"#\">已有账号？点击登录</a>");
        setWindowTitle("Talkbox - 注册");
    }
    
    // 重新连接按钮信号
    disconnect(ui->secondaryButton, &QPushButton::clicked, nullptr, nullptr);
    disconnect(ui->switchLabel, &QLabel::linkActivated, nullptr, nullptr);
    
    if (m_isRegistering) {
        connect(ui->secondaryButton, &QPushButton::clicked, this, &LoginDialog::onSwitchToLogin);
        connect(ui->switchLabel, &QLabel::linkActivated, this, &LoginDialog::onSwitchToLogin);
    } else {
        connect(ui->secondaryButton, &QPushButton::clicked, this, &LoginDialog::onSwitchToRegister);
        connect(ui->switchLabel, &QLabel::linkActivated, this, &LoginDialog::onSwitchToRegister);
    }
    
    resetForm();
}

void LoginDialog::onSwitchToLogin()
{
    onSwitchToRegister(); // 重用切换逻辑
}

void LoginDialog::onHttpResponse(const QJsonObject &response)
{
    setLoading(false);
    
    QString status = response["status"].toString();
    
    if (status == "success") {
        if (m_isRegistering) {
            ui->statusLabel->setText("注册成功！请登录");
            ui->statusLabel->setStyleSheet("color: green;");
            // 自动切换到登录模式
            QTimer::singleShot(1500, this, &LoginDialog::onSwitchToLogin);
        } else {
            // 登录成功
            QJsonObject data = response["data"].toObject();
            QString token = data["token"].toString();
            QString username = data["username"].toString();
            int userId = data["user_id"].toInt();
            
            ui->statusLabel->setText("登录成功！");
            ui->statusLabel->setStyleSheet("color: green;");
            
            emit loginSuccessful(token, username, userId);
            accept();
        }
    } else {
        QString errorMsg = response["data"].toString();
        ui->statusLabel->setText("错误: " + errorMsg);
        ui->statusLabel->setStyleSheet("color: red;");
    }
}

void LoginDialog::onHttpError(const QString &error)
{
    setLoading(false);
    ui->statusLabel->setText("网络错误: " + error);
    ui->statusLabel->setStyleSheet("color: red;");
}

void LoginDialog::resetForm()
{
    ui->usernameEdit->clear();
    ui->passwordEdit->clear();
    ui->confirmPasswordEdit->clear();
    ui->statusLabel->clear();
    ui->statusLabel->setStyleSheet("");
    ui->usernameEdit->setFocus();
}

void LoginDialog::setLoading(bool loading)
{
    ui->primaryButton->setEnabled(!loading);
    ui->secondaryButton->setEnabled(!loading);
    ui->usernameEdit->setEnabled(!loading);
    ui->passwordEdit->setEnabled(!loading);
    ui->confirmPasswordEdit->setEnabled(!loading);
}

bool LoginDialog::validateInput()
{
    QString username = ui->usernameEdit->text().trimmed();
    QString password = ui->passwordEdit->text();
    QString confirmPassword = ui->confirmPasswordEdit->text();
    
    // 清除之前的状态
    ui->statusLabel->setStyleSheet("");
    
    // 检查用户名
    if (username.isEmpty()) {
        ui->statusLabel->setText("请输入用户名");
        ui->statusLabel->setStyleSheet("color: orange;");
        ui->usernameEdit->setFocus();
        return false;
    }
    
    if (username.length() < 3) {
        ui->statusLabel->setText("用户名至少需要3个字符");
        ui->statusLabel->setStyleSheet("color: orange;");
        ui->usernameEdit->setFocus();
        return false;
    }
    
    // 检查用户名格式（只允许字母、数字、下划线）
    QRegularExpression usernameRegex("^[a-zA-Z0-9_]+$");
    if (!usernameRegex.match(username).hasMatch()) {
        ui->statusLabel->setText("用户名只能包含字母、数字和下划线");
        ui->statusLabel->setStyleSheet("color: orange;");
        ui->usernameEdit->setFocus();
        return false;
    }
    
    // 检查密码
    if (password.isEmpty()) {
        ui->statusLabel->setText("请输入密码");
        ui->statusLabel->setStyleSheet("color: orange;");
        ui->passwordEdit->setFocus();
        return false;
    }
    
    if (password.length() < 6) {
        ui->statusLabel->setText("密码至少需要6个字符");
        ui->statusLabel->setStyleSheet("color: orange;");
        ui->passwordEdit->setFocus();
        return false;
    }
    
    // 注册模式下检查确认密码
    if (m_isRegistering) {
        if (confirmPassword.isEmpty()) {
            ui->statusLabel->setText("请确认密码");
            ui->statusLabel->setStyleSheet("color: orange;");
            ui->confirmPasswordEdit->setFocus();
            return false;
        }
        
        if (password != confirmPassword) {
            ui->statusLabel->setText("两次输入的密码不一致");
            ui->statusLabel->setStyleSheet("color: orange;");
            ui->confirmPasswordEdit->setFocus();
            return false;
        }
    }
    
    return true;
}
