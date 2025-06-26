#ifndef CHATMAINWIDGET_H
#define CHATMAINWIDGET_H

#include <QWidget>
#include <QJsonArray>
#include <QJsonObject>

QT_BEGIN_NAMESPACE
class QTabWidget;
class QListWidget;
class QTextEdit;
class QLineEdit;
class QPushButton;
class QSplitter;
class QVBoxLayout;
class QHBoxLayout;
class QLabel;
QT_END_NAMESPACE

class ApiClient;

class ChatMainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatMainWidget(ApiClient *apiClient, QWidget *parent = nullptr);
    ~ChatMainWidget();

    void setCurrentUser(const QString &username, int userId);

public slots:
    void refreshContacts();
    void refreshGroups();
    void refreshMessages();

private slots:
    void onContactsReceived(const QJsonArray &contacts);
    void onGroupsReceived(const QJsonArray &groups);
    void onMessagesReceived(const QJsonArray &messages);
    void onGroupMessagesReceived(const QJsonArray &messages);
    void onMessageSent();
    void onContactSelected();
    void onGroupSelected();
    void onSendMessageClicked();
    void onCreateGroupClicked();
    void onJoinGroupClicked();
    void onShowForumRequested();
    void onLogoutRequested();

signals:
    void showForumRequested();
    void logoutRequested();

private:
    void setupUI();
    void updateMessageDisplay();
    void addMessageToDisplay(const QJsonObject &message);
    
    ApiClient *m_apiClient;
    QString m_currentUsername;
    int m_currentUserId;
    int m_currentContactId;
    int m_currentGroupId;
    bool m_isGroupChat;
    
    // UI组件
    QTabWidget *m_tabWidget;
    QSplitter *m_splitter;
    
    // 左侧面板
    QListWidget *m_contactsList;
    QListWidget *m_groupsList;
    QPushButton *m_createGroupButton;
    QPushButton *m_joinGroupButton;
    QPushButton *m_forumButton;
    QPushButton *m_logoutButton;
    
    // 右侧聊天区域
    QWidget *m_chatArea;
    QTextEdit *m_messagesDisplay;
    QLineEdit *m_messageInput;
    QPushButton *m_sendButton;
    QLabel *m_chatTitleLabel;
    
    // 数据
    QJsonArray m_contacts;
    QJsonArray m_groups;
    QJsonArray m_currentMessages;
};

#endif // CHATMAINWIDGET_H
