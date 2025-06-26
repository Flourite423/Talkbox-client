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
    void refreshPosts();

signals:
    void postSelected(int postId, const QString &title);

private slots:
    void onCreatePostClicked();
    void onRefreshClicked();
    void onPostClicked();
    void onHttpResponse(const QJsonObject &response);

private:
    Ui::ForumWidget *ui;
    HttpClient *m_httpClient;
    
    QString formatTimestamp(const QString &timestamp);
};

#endif // FORUMWIDGET_H
