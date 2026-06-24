#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QMap>
#include <QTimer>
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
    void setUserIdToNameMap(const QMap<int, QString> &userMap);
    void startPrivateChat(int userId, const QString &username);
    void startGroupChat(int groupId, const QString &groupName);
    void refreshMessages();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void onSendClicked();
    void onSendFileClicked();
    void onHttpResponse(const QJsonObject &response);
    void onAutoRefresh();
    void onMessageTextEditClicked();
    void onAnchorClicked(const QUrl &url);

private:
    Ui::ChatWindow *ui;
    HttpClient *m_httpClient;
    int m_currentUserId;  // 私聊时的用户ID
    int m_currentGroupId; // 群聊时的群组ID
    bool m_isGroupChat;   // 是否为群聊
    QString m_chatTitle;  // 聊天标题
    QString m_currentUsername; // 当前用户名
    QMap<int, QString> m_userIdToNameMap;  // 用户ID到用户名的映射
    QTimer *m_refreshTimer; // 自动刷新定时器
    QMap<QString, QString> m_fileDataMap; // 存储文件名到文件数据的映射
    QString m_downloadingFileName; // 正在下载的文件名
    QString m_downloadSavePath;    // 文件保存路径
    QString m_pendingFileName; // 等待发送消息的文件名
    
    // 无限滚动加载相关
    bool m_isLoadingMore;  // 是否正在加载更多消息
    int m_oldestMessageId; // 当前最早的消息ID，用于加载更多
    bool m_hasMoreMessages; // 是否还有更多消息可加载
    static const int MESSAGE_LOAD_LIMIT = 50; // 每次加载的消息数量
    
    QString getUserDisplayName(int userId);  // 获取用户显示名称
    QString formatTimestamp(const QString &timestamp); // 格式化时间戳
    void scrollToBottom(); // 滚动到底部
    void scrollToTop(); // 滚动到顶部
    int getCurrentUserIdFromUsername(); // 根据当前用户名获取用户ID
    void downloadFile(const QString &fileName); // 下载文件
    void saveDownloadedFile(const QString &fileName, const QString &fileData); // 保存下载的文件
    void loadMoreMessages(); // 加载更多消息（向上滚动时）
    void onScrollValueChanged(int value); // 滚动条值变化处理
};

#endif // CHATWINDOW_H
