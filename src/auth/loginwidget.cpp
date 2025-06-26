#include "loginwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QSpacerItem>

LoginWidget::LoginWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

LoginWidget::~LoginWidget()
{
}

void LoginWidget::setupUI()
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
    m_titleLabel = new QLabel("登录 Talkbox", this);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #007acc; margin: 20px;");
    mainLayout->addWidget(m_titleLabel);
    
    // 登录表单框架
    QFrame *loginFrame = new QFrame(this);
    loginFrame->setFrameShape(QFrame::StyledPanel);
    mainLayout->addWidget(loginFrame);
    
    QVBoxLayout *formLayout = new QVBoxLayout(loginFrame);
    formLayout->setSpacing(12);
    
    // 用户名
    m_usernameLabel = new QLabel("用户名", this);
    m_usernameLabel->setStyleSheet("font-weight: bold;");
    formLayout->addWidget(m_usernameLabel);
    
    m_usernameEdit = new QLineEdit(this);
    m_usernameEdit->setPlaceholderText("请输入用户名");
    formLayout->addWidget(m_usernameEdit);
    
    // 密码
    m_passwordLabel = new QLabel("密码", this);
    m_passwordLabel->setStyleSheet("font-weight: bold;");
    formLayout->addWidget(m_passwordLabel);
    
    m_passwordEdit = new QLineEdit(this);
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setPlaceholderText("请输入密码");
    formLayout->addWidget(m_passwordEdit);
    
    // 登录按钮
    m_loginButton = new QPushButton("登录", this);
    m_loginButton->setMinimumHeight(40);
    formLayout->addWidget(m_loginButton);
    
    // 注册链接
    QHBoxLayout *linkLayout = new QHBoxLayout();
    m_haveAccountLabel = new QLabel("没有账户？", this);
    linkLayout->addWidget(m_haveAccountLabel);
    
    m_registerButton = new QPushButton("立即注册", this);
    m_registerButton->setStyleSheet(R"(
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
    linkLayout->addWidget(m_registerButton);
    
    linkLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    formLayout->addLayout(linkLayout);
    
    // 底部间距
    mainLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    
    // 连接信号
    connect(m_loginButton, &QPushButton::clicked, this, &LoginWidget::onLoginButtonClicked);
    connect(m_registerButton, &QPushButton::clicked, this, &LoginWidget::onRegisterButtonClicked);
    
    // 回车键登录
    connect(m_usernameEdit, &QLineEdit::returnPressed, this, &LoginWidget::onLoginButtonClicked);
    connect(m_passwordEdit, &QLineEdit::returnPressed, this, &LoginWidget::onLoginButtonClicked);
}

void LoginWidget::clearForm()
{
    m_usernameEdit->clear();
    m_passwordEdit->clear();
    m_usernameEdit->setFocus();
}

void LoginWidget::setLoginEnabled(bool enabled)
{
    m_loginButton->setEnabled(enabled);
    m_usernameEdit->setEnabled(enabled);
    m_passwordEdit->setEnabled(enabled);
}

void LoginWidget::onLoginButtonClicked()
{
    QString username = m_usernameEdit->text().trimmed();
    QString password = m_passwordEdit->text();
    
    if (username.isEmpty() || password.isEmpty()) {
        return;
    }
    
    emit loginRequested(username, password);
}

void LoginWidget::onRegisterButtonClicked()
{
    emit showRegisterRequested();
}
