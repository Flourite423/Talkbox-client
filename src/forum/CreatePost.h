#ifndef CREATEPOST_H
#define CREATEPOST_H

#include <QDialog>
#include "../common/HttpClient.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class CreatePost;
}
QT_END_NAMESPACE

class CreatePost : public QDialog
{
    Q_OBJECT

public:
    CreatePost(QWidget *parent = nullptr);
    ~CreatePost();
    
    void setHttpClient(HttpClient *client);
    void setCurrentUsername(const QString &username);

signals:
    void postCreated();

private slots:
    void onCreateClicked();
    void onHttpResponse(const QJsonObject &response);
    void onHttpError(const QString &error);
    void onSelectFileClicked();
    void onClearFileClicked();

private:
    Ui::CreatePost *ui;
    HttpClient *m_httpClient;
    bool m_isCreatingPost;  // 标记是否正在创建帖子
    QString m_selectedFilePath;  // 选中的文件路径
    QString m_currentUsername;  // 当前用户名
    
    bool validateInput();
    void setLoading(bool loading);
    void uploadFileAndCreatePost();
};

#endif // CREATEPOST_H
