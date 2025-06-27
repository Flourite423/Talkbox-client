#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QMap>
#include "common/HttpClient.h"
#include "auth/LoginDialog.h"
#include "chat/ContactList.h"
#include "chat/ChatWindow.h"
#include "chat/GroupManager.h"
#include "forum/ForumWidget.h"
#include "forum/PostDetail.h"
#include "forum/CreatePost.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLoginSuccessful(const QString &token, const QString &username, int userId);
    void onLogoutClicked();
    void onContactSelected(int userId, const QString &username);
    void onGroupSelected(int groupId, const QString &groupName);
    void onPostSelected(int postId, const QString &title);
    void onTabChanged(int index);
    void updateUserMapping();
    void onContactsLoaded(const QMap<int, QString> &userMap);

private:
    Ui::MainWindow *ui;
    HttpClient *m_httpClient;
    
    // 用户信息
    QString m_token;
    QString m_username;
    int m_userId;
    bool m_loginSuccessful;
    
    // UI组件
    ContactList *m_contactList;
    ChatWindow *m_chatWindow;
    ChatWindow *m_groupChatWindow;
    GroupManager *m_groupManager;
    ForumWidget *m_forumWidget;
    PostDetail *m_postDetail;
    CreatePost *m_createPost;
    
    void setupUI();
    bool showLoginDialog();
    void updateUserInfo();
};

#endif // MAINWINDOW_H
