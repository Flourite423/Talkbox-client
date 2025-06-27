#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include "../common/HttpClient.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class ChatWindow;
}
QT_END_NAMESPACE

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    ChatWindow(QWidget *parent = nullptr);
    ~ChatWindow();
    
    void setHttpClient(HttpClient *client);
    void setCurrentUsername(const QString &username);
    void startPrivateChat(int userId, const QString &username);
    void startGroupChat(int groupId, const QString &groupName);
    void refreshMessages();

private slots:
    void onSendClicked();
    void onHttpResponse(const QJsonObject &response);

private:
    Ui::ChatWindow *ui;
    HttpClient *m_httpClient;
    int m_currentUserId;  // 私聊时的用户ID
    int m_currentGroupId; // 群聊时的群组ID
    bool m_isGroupChat;   // 是否为群聊
    QString m_chatTitle;  // 聊天标题
    QString m_currentUsername; // 当前用户名
};

#endif // CHATWINDOW_H
