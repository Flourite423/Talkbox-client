# Talkbox-client

基于Qt6的现代化聊天客户端，支持私聊、群聊和论坛功能。

## 功能特性

### 🔐 用户认证
- **智能登录/注册界面**: 支持在同一界面切换登录和注册模式
- **输入验证**: 
  - 用户名：至少3字符，只允许字母、数字、下划线
  - 密码：至少6字符
  - 注册时需要确认密码
- **友好的错误提示**: 详细的验证错误信息和网络错误处理
- **自动切换**: 注册成功后自动切换到登录模式

### 💬 即时通讯
- **私聊**: 与其他用户进行一对一聊天
- **群聊**: 创建和加入群组，支持多人聊天
- **联系人管理**: 查看所有用户及其在线状态
- **消息历史**: 查看聊天记录

### 🏛️ BBS论坛
- **发布帖子**: 创建新的讨论主题
- **回复互动**: 对帖子进行回复
- **浏览论坛**: 查看所有帖子和回复

### 🌐 网络功能
- **HTTP客户端**: 与后端API进行通信
- **Token认证**: 安全的用户身份验证
- **超时处理**: 10秒连接超时，友好的错误提示
- **自动重连**: 网络错误时的智能处理

## 技术架构

- **框架**: Qt6 (Core, Widgets, Network)
- **构建系统**: CMake + Make
- **编程语言**: C++17
- **网络协议**: HTTP/JSON
- **UI设计**: Qt Designer (.ui文件)

## 项目结构

```
src/
├── main.cpp                 # 应用程序入口
├── mainwindow.h/cpp/ui      # 主窗口
├── auth/                    # 认证模块
│   ├── LoginDialog.*        # 登录/注册对话框
│   └── RegisterDialog.*     # (预留)
├── chat/                    # 聊天模块
│   ├── ContactList.*        # 联系人列表
│   ├── ChatWindow.*         # 聊天窗口
│   └── GroupManager.*       # 群组管理
├── forum/                   # 论坛模块
│   ├── ForumWidget.*        # 论坛主界面
│   ├── PostDetail.*         # 帖子详情
│   └── CreatePost.*         # 创建帖子
└── common/                  # 公共模块
    └── HttpClient.*         # HTTP客户端
```

## 编译和运行

### 前置要求
- Qt6 (Core, Widgets, Network)
- CMake 3.16+
- C++17编译器 (GCC/Clang)

### 编译
```bash
# 使用项目根目录的Makefile包装器
make build

# 或者手动使用CMake
cmake -G "Unix Makefiles" -B build -S .
cd build && make -j$(nproc)
```

### 运行
```bash
# 直接运行
make run

# 或者
cd build && ./Talkbox-client
```

### 清理
```bash
make clean
```

## 使用说明

### 首次使用
1. 启动应用程序会显示登录界面
2. 点击"切换到注册"进行用户注册
3. 输入用户名(3+字符)和密码(6+字符)
4. 注册成功后自动切换到登录模式
5. 使用注册的账号登录

### 主界面功能
- **聊天标签**: 左侧联系人列表，右侧聊天窗口
- **群组标签**: 左侧群组管理，右侧群聊窗口
- **论坛标签**: 左侧帖子列表，右侧帖子详情

### 服务器配置
默认连接到 `http://localhost:8080`，需要确保Talkbox后端服务正在运行。

## API对应关系

| 客户端功能 | 后端API端点 |
|-----------|------------|
| 用户注册 | `POST /api/register` |
| 用户登录 | `POST /api/login` |
| 用户登出 | `POST /api/logout` |
| 获取联系人 | `GET /api/get_contacts` |
| 发送消息 | `POST /api/send_message` |
| 获取消息 | `GET /api/get_messages` |
| 创建群组 | `POST /api/create_group` |
| 加入群组 | `POST /api/join_group` |
| 获取群组列表 | `GET /api/get_groups` |
| 创建帖子 | `POST /api/create_post` |
| 获取帖子 | `GET /api/get_posts` |
| 回复帖子 | `POST /api/reply_post` |

## 开发特性

- **模块化设计**: 各功能模块独立，便于维护和扩展
- **信号槽机制**: 组件间松耦合通信
- **错误处理**: 完善的网络和用户输入错误处理
- **输入验证**: 客户端输入验证，提升用户体验
- **IDE友好**: 源码分组，包含目录配置

## 后续开发计划

- [ ] 添加UI样式和主题
- [ ] 实现文件传输功能
- [ ] 添加实时消息推送
- [ ] 支持表情和多媒体消息
- [ ] 添加用户设置和偏好
- [ ] 实现消息加密
- [ ] 添加离线消息功能

## 许可证

本项目采用开源许可证，详见 LICENSE 文件。