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
    void showPost(int postId, const QString &title, const QString &content, 
                  const QString &timestamp, int userId);

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
    QString m_downloadingFileName;  // 正在下载的文件名
    QString m_downloadSavePath;     // 文件保存路径
    
    void loadReplies();
    void displayReplies(const QJsonArray &replies);
    void extractAndDisplayAttachments(const QString &content);
    QString getUserDisplayName(int userId);  // 获取用户显示名称
    void saveDownloadedFile(const QString &fileName, const QString &fileData);  // 保存下载的文件
};

#endif // POSTDETAIL_H
