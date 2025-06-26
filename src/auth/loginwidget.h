#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QPushButton;
class QLabel;
QT_END_NAMESPACE

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();

    void clearForm();
    void setLoginEnabled(bool enabled);

signals:
    void loginRequested(const QString &username, const QString &password);
    void showRegisterRequested();

private slots:
    void onLoginButtonClicked();
    void onRegisterButtonClicked();

private:
    void setupUI();
    
    QLineEdit *m_usernameEdit;
    QLineEdit *m_passwordEdit;
    QPushButton *m_loginButton;
    QPushButton *m_registerButton;
    QLabel *m_titleLabel;
    QLabel *m_usernameLabel;
    QLabel *m_passwordLabel;
    QLabel *m_haveAccountLabel;
};

#endif // LOGINWIDGET_H
