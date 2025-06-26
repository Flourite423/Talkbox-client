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
    void refreshContacts();

signals:
    void contactSelected(int userId, const QString &username);

private slots:
    void onContactClicked(QListWidgetItem *item);
    void onHttpResponse(const QJsonObject &response);

private:
    Ui::ContactList *ui;
    HttpClient *m_httpClient;
};

#endif // CONTACTLIST_H
