#include "registerwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QSpacerItem>
#include <QMessageBox>

RegisterWidget::RegisterWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

RegisterWidget::~RegisterWidget()
{
}

void RegisterWidget::setupUI()
{
    // 设置窗口样式
    setStyleSheet(R"(
        QWidget {
            background-color: #f0f0f0;
            font-family: 'Microsoft YaHei', Arial, sans-serif;
        }

        QLabel {
            color: #333;
        }

        QPushButton {
            background-color: #007acc;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 8px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: #005999;
        }

        QPushButton:pressed {
            background-color: #004d7a;
        }

        QPushButton:disabled {
            background-color: #cccccc;
        }

        QLineEdit {
            border: 2px solid #ddd;
            border-radius: 6px;
            padding: 8px;
            font-size: 14px;
        }

        QLineEdit:focus {
            border-color: #007acc;
        }

        QFrame {
            background-color: white;
            border-radius: 10px;
            padding: 20px;
            margin: 20px;
        }
    )");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // 顶部间距
    mainLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    
    // 标题
    m_titleLabel = new QLabel("创建 Talkbox 账户", this);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #007acc; margin: 20px;");
    mainLayout->addWidget(m_titleLabel);
    
    // 注册表单框架
    QFrame *registerFrame = new QFrame(this);
    registerFrame->setFrameShape(QFrame::StyledPanel);
    mainLayout->addWidget(registerFrame);
    
    QVBoxLayout *formLayout = new QVBoxLayout(registerFrame);
    formLayout->setSpacing(12);
    
    // 用户名
    m_usernameLabel = new QLabel("用户名", this);
    m_usernameLabel->setStyleSheet("font-weight: bold;");
    formLayout->addWidget(m_usernameLabel);
    
    m_usernameEdit = new QLineEdit(this);
    m_usernameEdit->setPlaceholderText("请输入用户名（3-20个字符）");
    formLayout->addWidget(m_usernameEdit);
    
    // 密码
    m_passwordLabel = new QLabel("密码", this);
    m_passwordLabel->setStyleSheet("font-weight: bold;");
    formLayout->addWidget(m_passwordLabel);
    
    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setPlaceholderText("请输入密码（至少6位）");
    formLayout->addWidget(m_passwordEdit);
    
    // 确认密码
    m_confirmPasswordLabel = new QLabel("确认密码", this);
    m_confirmPasswordLabel->setStyleSheet("font-weight: bold;");
    formLayout->addWidget(m_confirmPasswordLabel);
    
    m_confirmPasswordEdit = new QLineEdit(this);
    m_confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    m_confirmPasswordEdit->setPlaceholderText("请再次输入密码");
    formLayout->addWidget(m_confirmPasswordEdit);
    
    // 注册按钮
    m_registerButton = new QPushButton("注册", this);
    m_registerButton->setMinimumHeight(40);
    formLayout->addWidget(m_registerButton);
    
    // 登录链接
    QHBoxLayout *linkLayout = new QHBoxLayout();
    m_haveAccountLabel = new QLabel("已有账户？", this);
    linkLayout->addWidget(m_haveAccountLabel);
    
    m_loginButton = new QPushButton("立即登录", this);
    m_loginButton->setStyleSheet(R"(
        QPushButton {
            background: none;
            color: #007acc;
            text-decoration: underline;
            border: none;
        }
        QPushButton:hover {
            color: #005999;
        }
    )");
    linkLayout->addWidget(m_loginButton);
    
    linkLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    formLayout->addLayout(linkLayout);
    
    // 底部间距
    mainLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    
    // 连接信号
    connect(m_registerButton, &QPushButton::clicked, this, &RegisterWidget::onRegisterButtonClicked);
    connect(m_loginButton, &QPushButton::clicked, this, &RegisterWidget::onLoginButtonClicked);
    
    // 回车键注册
    connect(m_usernameEdit, &QLineEdit::returnPressed, this, &RegisterWidget::onRegisterButtonClicked);
    connect(m_passwordEdit, &QLineEdit::returnPressed, this, &RegisterWidget::onRegisterButtonClicked);
    connect(m_confirmPasswordEdit, &QLineEdit::returnPressed, this, &RegisterWidget::onRegisterButtonClicked);
}

void RegisterWidget::clearForm()
{
    m_usernameEdit->clear();
    m_passwordEdit->clear();
    m_confirmPasswordEdit->clear();
    m_usernameEdit->setFocus();
}

void RegisterWidget::setRegisterEnabled(bool enabled)
{
    m_registerButton->setEnabled(enabled);
    m_usernameEdit->setEnabled(enabled);
    m_passwordEdit->setEnabled(enabled);
    m_confirmPasswordEdit->setEnabled(enabled);
}

bool RegisterWidget::validateForm()
{
    QString username = m_usernameEdit->text().trimmed();
    QString password = m_passwordEdit->text();
    QString confirmPassword = m_confirmPasswordEdit->text();
    
    if (username.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请输入用户名");
        m_usernameEdit->setFocus();
        return false;
    }
    
    if (username.length() < 3 || username.length() > 20) {
        QMessageBox::warning(this, "输入错误", "用户名长度应为3-20个字符");
        m_usernameEdit->setFocus();
        return false;
    }
    
    if (password.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请输入密码");
        m_passwordEdit->setFocus();
        return false;
    }
    
    if (password.length() < 6) {
        QMessageBox::warning(this, "输入错误", "密码长度至少为6位");
        m_passwordEdit->setFocus();
        return false;
    }
    
    if (password != confirmPassword) {
        QMessageBox::warning(this, "输入错误", "两次输入的密码不一致");
        m_confirmPasswordEdit->setFocus();
        return false;
    }
    
    return true;
}

void RegisterWidget::onRegisterButtonClicked()
{
    if (!validateForm()) {
        return;
    }
    
    QString username = m_usernameEdit->text().trimmed();
    QString password = m_passwordEdit->text();
    
    emit registerRequested(username, password);
}

void RegisterWidget::onLoginButtonClicked()
{
    emit showLoginRequested();
}
