#ifndef GROUPMANAGER_H
#define GROUPMANAGER_H

#include <QWidget>
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
    void refreshGroups();

signals:
    void groupSelected(int groupId, const QString &groupName);

private slots:
    void onCreateGroupClicked();
    void onJoinGroupClicked();
    void onLeaveGroupClicked();
    void onGroupClicked();
    void onHttpResponse(const QJsonObject &response);

private:
    Ui::GroupManager *ui;
    HttpClient *m_httpClient;
};

#endif // GROUPMANAGER_H
