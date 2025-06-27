#ifndef GROUPMANAGER_H
#define GROUPMANAGER_H

#include <QWidget>
#include <QJsonArray>
#include <QTimer>
#include "../common/HttpClient.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class GroupManager;
}
QT_END_NAMESPACE

class GroupManager : public QWidget
{
    Q_OBJECT

public:
    GroupManager(QWidget *parent = nullptr);
    ~GroupManager();
    
    void setHttpClient(HttpClient *client);
    void setCurrentUsername(const QString &username);
    void refreshGroups();
    void refreshAllGroups(); // 新增方法：获取所有群组

signals:
    void groupSelected(int groupId, const QString &groupName);
    void groupOperationCompleted(); // 群组操作完成信号（创建、加入、退出成功时发出）

private slots:
    void onCreateGroupClicked();
    void onJoinGroupClicked();
    void onLeaveGroupClicked();
    void onGroupClicked();
    void onHttpResponse(const QJsonObject &response);
    void doRefreshGroups(); // 实际执行刷新的方法

private:
    Ui::GroupManager *ui;
    HttpClient *m_httpClient;
    QString m_currentUsername;
    QJsonArray m_allGroups;        // 存储所有群组
    QJsonArray m_userGroups;       // 存储用户群组
    bool m_isLoadingAllGroups;     // 标记是否正在加载所有群组
    bool m_isRefreshing;           // 标记是否正在执行完整刷新流程
    QTimer *m_refreshTimer;        // 防抖计时器
    
    void displayGroupsWithMemberStatus(); // 合并显示群组列表
};

#endif // GROUPMANAGER_H
