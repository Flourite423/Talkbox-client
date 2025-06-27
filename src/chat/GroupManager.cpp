#include "GroupManager.h"
#include "ui_GroupManager.h"
#include <QListWidgetItem>
#include <QJsonArray>
#include <QInputDialog>

GroupManager::GroupManager(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GroupManager)
    , m_httpClient(nullptr)
{
    ui->setupUi(this);
    
    connect(ui->createButton, &QPushButton::clicked, this, &GroupManager::onCreateGroupClicked);
    connect(ui->joinButton, &QPushButton::clicked, this, &GroupManager::onJoinGroupClicked);
    connect(ui->leaveButton, &QPushButton::clicked, this, &GroupManager::onLeaveGroupClicked);
    connect(ui->refreshButton, &QPushButton::clicked, this, &GroupManager::refreshGroups);
    connect(ui->groupListWidget, &QListWidget::itemDoubleClicked, this, &GroupManager::onGroupClicked);
}

GroupManager::~GroupManager()
{
    delete ui;
}

void GroupManager::setHttpClient(HttpClient *client)
{
    m_httpClient = client;
    if (m_httpClient) {
        connect(m_httpClient, &HttpClient::responseReceived, this, &GroupManager::onHttpResponse);
    }
}

void GroupManager::refreshGroups()
{
    if (m_httpClient && !m_currentUsername.isEmpty()) {
        QJsonObject params;
        params["username"] = m_currentUsername;
        m_httpClient->get("/api/get_groups", params);
    }
}

void GroupManager::onCreateGroupClicked()
{
    QString groupName = ui->groupNameEdit->text().trimmed();
    QString description = ui->groupDescEdit->text().trimmed();
    
    if (groupName.isEmpty()) {
        ui->statusLabel->setText("请输入群组名称");
        return;
    }
    
    if (!m_httpClient) return;
    
    QJsonObject data;
    data["username"] = m_currentUsername;
    data["group_name"] = groupName;
    data["description"] = description;
    
    m_httpClient->post("/api/create_group", data);
    
    ui->groupNameEdit->clear();
    ui->groupDescEdit->clear();
}

void GroupManager::onJoinGroupClicked()
{
    QListWidgetItem *item = ui->groupListWidget->currentItem();
    if (!item || !m_httpClient) {
        ui->statusLabel->setText("请选择要加入的群组");
        return;
    }
    
    int groupId = item->data(Qt::UserRole).toInt();
    
    QJsonObject data;
    data["username"] = m_currentUsername;
    data["group_id"] = groupId;
    
    m_httpClient->post("/api/join_group", data);
}

void GroupManager::onLeaveGroupClicked()
{
    QListWidgetItem *item = ui->groupListWidget->currentItem();
    if (!item || !m_httpClient) {
        ui->statusLabel->setText("请选择要退出的群组");
        return;
    }
    
    int groupId = item->data(Qt::UserRole).toInt();
    
    QJsonObject data;
    data["username"] = m_currentUsername;
    data["group_id"] = groupId;
    
    m_httpClient->post("/api/leave_group", data);
}

void GroupManager::onGroupClicked()
{
    QListWidgetItem *item = ui->groupListWidget->currentItem();
    if (item) {
        int groupId = item->data(Qt::UserRole).toInt();
        QString groupName = item->text().split(" - ")[0]; // 提取群组名称部分
        emit groupSelected(groupId, groupName);
    }
}

void GroupManager::onHttpResponse(const QJsonObject &response)
{
    if (response["status"].toString() == "success") {
        QJsonValue dataValue = response["data"];
        
        if (dataValue.isArray()) {
            // 群组列表响应
            ui->groupListWidget->clear();
            QJsonArray groups = dataValue.toArray();
            
            for (const auto &value : groups) {
                QJsonObject group = value.toObject();
                int groupId = group["group_id"].toInt();
                QString groupName = group["group_name"].toString();
                QString description = group["description"].toString();
                bool isMember = group["is_member"].toBool();
                
                QString displayText = QString("%1 - %2 %3")
                                    .arg(groupName)
                                    .arg(description)
                                    .arg(isMember ? "(已加入)" : "");
                
                QListWidgetItem *item = new QListWidgetItem(displayText);
                item->setData(Qt::UserRole, groupId);
                ui->groupListWidget->addItem(item);
            }
            ui->statusLabel->setText("群组列表已更新");
        } else if (dataValue.isString()) {
            // 操作结果响应
            ui->statusLabel->setText(dataValue.toString());
            if (dataValue.toString().contains("成功")) {
                refreshGroups(); // 操作成功后刷新列表
            }
        }
    } else {
        ui->statusLabel->setText("错误: " + response["data"].toString());
    }
}

void GroupManager::setCurrentUsername(const QString &username)
{
    m_currentUsername = username;
}
