#ifndef POSTDETAIL_H
#define POSTDETAIL_H

#include <QWidget>
#include "../common/HttpClient.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class PostDetail;
}
QT_END_NAMESPACE

class PostDetail : public QWidget
{
    Q_OBJECT

public:
    PostDetail(QWidget *parent = nullptr);
    ~PostDetail();
    
    void setHttpClient(HttpClient *client);
    void setCurrentUsername(const QString &username);
    void setUserIdToNameMap(const QMap<int, QString> &userMap);
    void showPost(int postId, const QString &title);

private slots:
    void onReplyClicked();
    void onRefreshRepliesClicked();
    void onHttpResponse(const QJsonObject &response);
    void onAttachmentClicked();

private:
    Ui::PostDetail *ui;
    HttpClient *m_httpClient;
    int m_currentPostId;
    QString m_currentUsername;
    QMap<int, QString> m_userIdToNameMap;  // 用户ID到用户名的映射
    
    void loadPostContent();
    void loadReplies();
    void displayReplies(const QJsonArray &replies);
    void extractAndDisplayAttachments(const QString &content);
    QString getUserDisplayName(int userId);  // 获取用户显示名称
};

#endif // POSTDETAIL_H
