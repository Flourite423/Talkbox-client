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
    void showPost(int postId, const QString &title);

private slots:
    void onReplyClicked();
    void onRefreshRepliesClicked();
    void onHttpResponse(const QJsonObject &response);

private:
    Ui::PostDetail *ui;
    HttpClient *m_httpClient;
    int m_currentPostId;
    
    void loadPostContent();
    void loadReplies();
    void displayReplies(const QJsonArray &replies);
};

#endif // POSTDETAIL_H
