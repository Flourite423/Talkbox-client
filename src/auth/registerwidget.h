#ifndef REGISTERWIDGET_H
#define REGISTERWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QPushButton;
class QLabel;
QT_END_NAMESPACE

class RegisterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterWidget(QWidget *parent = nullptr);
    ~RegisterWidget();

    void clearForm();
    void setRegisterEnabled(bool enabled);

signals:
    void registerRequested(const QString &username, const QString &password);
    void showLoginRequested();

private slots:
    void onRegisterButtonClicked();
    void onLoginButtonClicked();

private:
    void setupUI();
    bool validateForm();
    
    QLineEdit *m_usernameEdit;
    QLineEdit *m_passwordEdit;
    QLineEdit *m_confirmPasswordEdit;
    QPushButton *m_registerButton;
    QPushButton *m_loginButton;
    QLabel *m_titleLabel;
    QLabel *m_usernameLabel;
    QLabel *m_passwordLabel;
    QLabel *m_confirmPasswordLabel;
    QLabel *m_haveAccountLabel;
};

#endif // REGISTERWIDGET_H
