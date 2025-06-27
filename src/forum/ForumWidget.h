#ifndef FORUMWIDGET_H
#define FORUMWIDGET_H

#include <QWidget>
#include "../common/HttpClient.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class ForumWidget;
}
QT_END_NAMESPACE

class CreatePost;

class ForumWidget : public QWidget
{
    Q_OBJECT

public:
    ForumWidget(QWidget *parent = nullptr);
    ~ForumWidget();
    
    void setHttpClient(HttpClient *client);
    void setCurrentUsername(const QString &username);
    void refreshPosts();
    void setUserIdToNameMap(const QMap<int, QString> &userMap);

signals:
    void postSelected(int postId, const QString &title, const QString &content, 
                     const QString &timestamp, int userId);

private slots:
    void onCreatePostClicked();
    void onRefreshClicked();
    void onPostClicked();
    void onHttpResponse(const QJsonObject &response);

private:
    Ui::ForumWidget *ui;
    HttpClient *m_httpClient;
    QString m_currentUsername;
    QMap<int, QString> m_userIdToNameMap;  // 用户ID到用户名的映射
    
    QString formatTimestamp(const QString &timestamp);
    QString getUserDisplayName(int userId);  // 获取用户显示名称
};

#endif // FORUMWIDGET_H
