#ifndef FORUMMAINWIDGET_H
#define FORUMMAINWIDGET_H

#include <QWidget>
#include <QJsonArray>
#include <QJsonObject>

QT_BEGIN_NAMESPACE
class QListWidget;
class QTextEdit;
class QLineEdit;
class QPushButton;
class QSplitter;
class QVBoxLayout;
class QHBoxLayout;
class QLabel;
class QStackedWidget;
QT_END_NAMESPACE

class ApiClient;

class ForumMainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ForumMainWidget(ApiClient *apiClient, QWidget *parent = nullptr);
    ~ForumMainWidget();

    void setCurrentUser(const QString &username, int userId);

public slots:
    void refreshPosts();

private slots:
    void onPostsReceived(const QJsonArray &posts);
    void onPostDetailReceived(const QJsonObject &post);
    void onRepliesReceived(const QJsonArray &replies);
    void onPostCreated();
    void onReplyCreated();
    void onPostSelected();
    void onCreatePostClicked();
    void onReplyPostClicked();
    void onBackToListClicked();
    void onShowChatRequested();
    void onLogoutRequested();

signals:
    void showChatRequested();
    void logoutRequested();

private:
    void setupUI();
    void showPostList();
    void showPostDetail(int postId);
    void updatePostDetail(const QJsonObject &post);
    void updateRepliesList(const QJsonArray &replies);
    
    ApiClient *m_apiClient;
    QString m_currentUsername;
    int m_currentUserId;
    int m_currentPostId;
    
    // UI组件
    QStackedWidget *m_stackedWidget;
    
    // 帖子列表页面
    QWidget *m_postListPage;
    QListWidget *m_postsList;
    QPushButton *m_createPostButton;
    QPushButton *m_chatButton;
    QPushButton *m_logoutButton;
    
    // 帖子详情页面
    QWidget *m_postDetailPage;
    QLabel *m_postTitleLabel;
    QLabel *m_postAuthorLabel;
    QLabel *m_postTimeLabel;
    QTextEdit *m_postContentDisplay;
    QListWidget *m_repliesList;
    QLineEdit *m_replyInput;
    QPushButton *m_replyButton;
    QPushButton *m_backButton;
    
    // 数据
    QJsonArray m_posts;
    QJsonObject m_currentPost;
    QJsonArray m_currentReplies;
};

#endif // FORUMMAINWIDGET_H
