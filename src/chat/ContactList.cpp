#include "ContactList.h"
#include "ui_ContactList.h"
#include <QListWidgetItem>
#include <QJsonArray>

ContactList::ContactList(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ContactList)
    , m_httpClient(nullptr)
{
    ui->setupUi(this);
    
    connect(ui->contactListWidget, &QListWidget::itemClicked, this, &ContactList::onContactClicked);
    connect(ui->refreshButton, &QPushButton::clicked, this, &ContactList::refreshContacts);
}

ContactList::~ContactList()
{
    delete ui;
}

void ContactList::setHttpClient(HttpClient *client)
{
    m_httpClient = client;
    if (m_httpClient) {
        connect(m_httpClient, &HttpClient::responseReceived, this, &ContactList::onHttpResponse);
    }
}

void ContactList::refreshContacts()
{
    if (m_httpClient) {
        m_httpClient->get("/api/get_contacts");
    }
}

void ContactList::onContactClicked(QListWidgetItem *item)
{
    if (item) {
        int userId = item->data(Qt::UserRole).toInt();
        QString username = item->text().split(" - ")[0]; // 提取用户名部分
        emit contactSelected(userId, username);
    }
}

void ContactList::onHttpResponse(const QJsonObject &response)
{
    if (response["status"].toString() == "success") {
        ui->contactListWidget->clear();
        
        QJsonArray contacts = response["data"].toArray();
        for (const auto &value : contacts) {
            QJsonObject contact = value.toObject();
            int userId = contact["user_id"].toInt();
            QString username = contact["username"].toString();
            bool online = contact["online"].toBool();
            
            QString displayText = username + " - " + (online ? "在线" : "离线");
            QListWidgetItem *item = new QListWidgetItem(displayText);
            item->setData(Qt::UserRole, userId);
            ui->contactListWidget->addItem(item);
        }
    }
}
