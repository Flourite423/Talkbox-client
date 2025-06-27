#ifndef CONTACTLIST_H
#define CONTACTLIST_H

#include <QWidget>
#include <QListWidget>
#include "../common/HttpClient.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class ContactList;
}
QT_END_NAMESPACE

class ContactList : public QWidget
{
    Q_OBJECT

public:
    ContactList(QWidget *parent = nullptr);
    ~ContactList();
    
    void setHttpClient(HttpClient *client);
    void setCurrentUsername(const QString &username);
    void refreshContacts();
    QMap<int, QString> getUserMapping() const;

signals:
    void contactSelected(int userId, const QString &username);
    void contactsLoaded(const QMap<int, QString> &userMap);

private slots:
    void onContactClicked(QListWidgetItem *item);
    void onHttpResponse(const QJsonObject &response);

private:
    Ui::ContactList *ui;
    HttpClient *m_httpClient;
    QString m_currentUsername;
    QMap<int, QString> m_userMapping;  // 存储用户ID到用户名的映射
};

#endif // CONTACTLIST_H
