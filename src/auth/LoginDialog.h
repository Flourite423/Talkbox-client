#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QStackedWidget>
#include "../common/HttpClient.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class LoginDialog;
}
QT_END_NAMESPACE

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

signals:
    void loginSuccessful(const QString &token, const QString &username, int userId);

protected:
    void closeEvent(QCloseEvent *event) override;
    void reject() override;

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void onSwitchToRegister();
    void onSwitchToLogin();
    void onHttpResponse(const QJsonObject &response);
    void onHttpError(const QString &error);

private:
    Ui::LoginDialog *ui;
    HttpClient *m_httpClient;
    bool m_isRegistering;
    
    void setupConnections();
    void resetForm();
    void setLoading(bool loading);
    bool validateInput();
};

#endif // LOGINDIALOG_H
