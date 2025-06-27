#include "GroupManager.h"
#include "ui_GroupManager.h"
#include <QListWidgetItem>
#include <QJsonArray>
#include <QInputDialog>

GroupManager::GroupManager(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GroupManager)
    , m_httpClient(nullptr)
    , m_isLoadingAllGroups(false)
    , m_isRefreshing(false)
    , m_refreshTimer(new QTimer(this))
{
    ui->setupUi(this);
    
    // 设置防抖计时器
    m_refreshTimer->setSingleShot(true);
    m_refreshTimer->setInterval(500); // 500ms防抖
    connect(m_refreshTimer, &QTimer::timeout, this, &GroupManager::doRefreshGroups);
    
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
        // 使用防抖计时器，避免频繁刷新
        if (m_isRefreshing) {
            m_refreshTimer->start(); // 重新启动计时器
            return;
        }
        
        m_refreshTimer->start();
    }
}

void GroupManager::doRefreshGroups()
{
    if (m_httpClient && !m_currentUsername.isEmpty()) {
        // 防止重复刷新
        if (m_isRefreshing) {
            return;
        }
        
        // 标记开始刷新流程
        m_isRefreshing = true;
        m_isLoadingAllGroups = true;
        
        ui->statusLabel->setText("正在加载群组列表...");
        ui->statusLabel->setStyleSheet("color: blue;");
        
        // 第一步：获取所有群组列表
        m_httpClient->get("/api/get_groups");
    }
}

void GroupManager::onCreateGroupClicked()
{
    QString groupName = ui->groupNameEdit->text().trimmed();
    QString description = ui->groupDescEdit->text().trimmed();
    
    if (groupName.isEmpty()) {
        ui->statusLabel->setText("请输入群组名称");
        ui->statusLabel->setStyleSheet("color: orange;");
        return;
    }
    
    if (groupName.length() < 2) {
        ui->statusLabel->setText("群组名称至少需要2个字符");
        ui->statusLabel->setStyleSheet("color: orange;");
        return;
    }
    
    if (!m_httpClient) return;
    
    QJsonObject data;
    data["username"] = m_currentUsername;
    data["group_name"] = groupName;
    data["description"] = description.isEmpty() ? "无描述" : description;
    
    ui->statusLabel->setText("正在创建群组...");
    ui->statusLabel->setStyleSheet("color: blue;");
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
    
    // 检查是否已经是成员
    if (item->text().contains("[已加入]")) {
        ui->statusLabel->setText("您已经是该群组的成员");
        ui->statusLabel->setStyleSheet("color: orange;");
        return;
    }
    
    int groupId = item->data(Qt::UserRole).toInt();
    
    QJsonObject data;
    data["username"] = m_currentUsername;
    data["group_id"] = QString::number(groupId);
    
    ui->statusLabel->setText("正在加入群组...");
    m_httpClient->post("/api/join_group", data);
}

void GroupManager::onLeaveGroupClicked()
{
    QListWidgetItem *item = ui->groupListWidget->currentItem();
    if (!item || !m_httpClient) {
        ui->statusLabel->setText("请选择要退出的群组");
        return;
    }
    
    // 检查是否是成员
    if (!item->text().contains("[已加入]")) {
        ui->statusLabel->setText("您还不是该群组的成员");
        ui->statusLabel->setStyleSheet("color: orange;");
        return;
    }
    
    int groupId = item->data(Qt::UserRole).toInt();
    
    QJsonObject data;
    data["username"] = m_currentUsername;
    data["group_id"] = QString::number(groupId);
    
    ui->statusLabel->setText("正在退出群组...");
    m_httpClient->post("/api/leave_group", data);
}

void GroupManager::onGroupClicked()
{
    QListWidgetItem *item = ui->groupListWidget->currentItem();
    if (item) {
        // 只有已加入的群组才能进入聊天
        if (!item->text().contains("[已加入]")) {
            ui->statusLabel->setText("请先加入该群组才能开始聊天");
            ui->statusLabel->setStyleSheet("color: orange;");
            return;
        }
        
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
            QJsonArray groups = dataValue.toArray();
            
            // 验证是否是群组列表响应
            bool isGroupResponse = false;
            if (groups.isEmpty()) {
                // 空数组在刷新状态下认为是群组响应
                isGroupResponse = (m_isLoadingAllGroups || m_isRefreshing);
            } else {
                // 非空数组，检查第一个元素是否包含群组字段
                QJsonObject firstItem = groups[0].toObject();
                isGroupResponse = firstItem.contains("group_id") && firstItem.contains("group_name");
            }
            
            if (isGroupResponse) {
                if (m_isLoadingAllGroups) {
                    // 第一步：收到所有群组列表
                    m_allGroups = groups;
                    m_isLoadingAllGroups = false;
                    
                    ui->statusLabel->setText("正在获取成员信息...");
                    ui->statusLabel->setStyleSheet("color: blue;");
                    
                    // 第二步：获取用户的群组成员信息
                    QJsonObject params;
                    params["username"] = m_currentUsername;
                    m_httpClient->get("/api/get_groups", params);
                } else {
                    // 第二步：收到用户群组信息，合并显示
                    m_userGroups = groups;
                    displayGroupsWithMemberStatus();
                    
                    // 完成刷新流程
                    m_isRefreshing = false;
                }
            }
            // 忽略非群组的数组响应（如消息列表、联系人列表等）
        } else if (dataValue.isString()) {
            // 操作结果响应
            QString message = dataValue.toString();
            ui->statusLabel->setStyleSheet("");
            
            if (message.contains("成功")) {
                ui->statusLabel->setText("成功: " + message);
                ui->statusLabel->setStyleSheet("color: green;");
                // 操作成功后不自动刷新，发出信号通知主窗口
                emit groupOperationCompleted();
            } else {
                ui->statusLabel->setText("失败: " + message);
                ui->statusLabel->setStyleSheet("color: red;");
            }
        }
    } else {
        // 重置所有加载状态
        m_isLoadingAllGroups = false;
        m_isRefreshing = false;
        ui->statusLabel->setText("错误: " + response["data"].toString());
        ui->statusLabel->setStyleSheet("color: red;");
    }
}

void GroupManager::setCurrentUsername(const QString &username)
{
    m_currentUsername = username;
}

void GroupManager::refreshAllGroups()
{
    if (m_httpClient) {
        // 先获取所有群组列表（不带用户名参数）
        m_httpClient->get("/api/get_groups");
    }
}

void GroupManager::displayGroupsWithMemberStatus()
{
    ui->groupListWidget->clear();
    
    // 创建用户群组的映射，用于快速查找成员状态
    QMap<int, bool> userGroupMap;
    for (const auto &value : m_userGroups) {
        QJsonObject userGroup = value.toObject();
        int groupId = userGroup["group_id"].toInt();
        bool isMember = userGroup.contains("is_member") ? userGroup["is_member"].toBool() : true;
        userGroupMap[groupId] = isMember;
    }
    
    // 显示所有群组，标记成员状态
    for (const auto &value : m_allGroups) {
        QJsonObject group = value.toObject();
        int groupId = group["group_id"].toInt();
        QString groupName = group["group_name"].toString();
        QString description = group["description"].toString();
        
        // 检查用户是否是该群组的成员
        bool isMember = userGroupMap.contains(groupId) && userGroupMap[groupId];
        QString memberStatus = isMember ? "[已加入]" : "[未加入]";
        
        QString displayText = QString("%1 - %2 %3")
                            .arg(groupName)
                            .arg(description.isEmpty() ? "无描述" : description)
                            .arg(memberStatus);
        
        QListWidgetItem *item = new QListWidgetItem(displayText);
        item->setData(Qt::UserRole, groupId);
        // 存储成员状态用于后续判断
        item->setData(Qt::UserRole + 1, isMember);
        ui->groupListWidget->addItem(item);
    }
    
    ui->statusLabel->setText("群组列表已更新");
    ui->statusLabel->setStyleSheet("color: green;");
}
