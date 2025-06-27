#include "ContactList.h"
#include "ui_ContactList.h"
#include <QListWidgetItem>
#include <QJsonArray>
#include <QMap>

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

void ContactList::setCurrentUsername(const QString &username)
{
    m_currentUsername = username;
}

void ContactList::refreshContacts()
{
    if (m_httpClient && !m_currentUsername.isEmpty()) {
        QJsonObject params;
        params["username"] = m_currentUsername;
        m_httpClient->get("/api/get_contacts", params);
    }
}

void ContactList::onContactClicked(QListWidgetItem *item)
{
    if (item) {
        int userId = item->data(Qt::UserRole).toInt();
        QString username = item->text(); // 直接使用显示文本作为用户名
        emit contactSelected(userId, username);
    }
}

void ContactList::onHttpResponse(const QJsonObject &response)
{
    if (response["status"].toString() == "success") {
        QJsonValue dataValue = response["data"];
        
        // 只处理联系人列表相关的响应
        if (dataValue.isArray()) {
            QJsonArray dataArray = dataValue.toArray();
            
            // 检查是否是联系人列表响应（包含user_id、username字段）
            if (!dataArray.isEmpty()) {
                QJsonObject firstItem = dataArray[0].toObject();
                if (firstItem.contains("user_id") && firstItem.contains("username")) {
                    // 这是联系人列表响应
                    ui->contactListWidget->clear();
                    m_userMapping.clear();
                    
                    for (const auto &value : dataArray) {
                        QJsonObject contact = value.toObject();
                        int userId = contact["user_id"].toInt();
                        QString username = contact["username"].toString();
                        // 移除在线状态显示
                        
                        // 建立用户映射
                        m_userMapping[userId] = username;
                        
                        QString displayText = username; // 只显示用户名
                        QListWidgetItem *item = new QListWidgetItem(displayText);
                        item->setData(Qt::UserRole, userId);
                        ui->contactListWidget->addItem(item);
                    }
                    
                    // 发出联系人加载完成信号
                    emit contactsLoaded(m_userMapping);
                }
            } else {
                // 空数组也可能是联系人列表（没有联系人）
                ui->contactListWidget->clear();
                m_userMapping.clear();
                emit contactsLoaded(m_userMapping);
            }
        }
        // 忽略非数组响应（可能是其他组件的响应）
    }
}

QMap<int, QString> ContactList::getUserMapping() const
{
    return m_userMapping;
}
