# Talkbox-client

一个基于 Qt6 的现代化聊天客户端应用程序，提供即时通讯、群组管理和论坛功能。

## 功能特性

- **用户认证**: 支持用户注册、登录功能
- **即时通讯**: 实时聊天消息收发
- **联系人管理**: 添加、管理联系人列表
- **群组聊天**: 创建和管理群组，支持多人聊天
- **论坛系统**: 发布帖子、查看帖子详情
- **现代化界面**: 基于 Qt6 的美观用户界面

## 系统要求

- **操作系统**: Linux (已测试)
- **编译器**: GCC 或 Clang (支持 C++17)
- **Qt 版本**: Qt6 (Core, Widgets, Network 模块)
- **CMake 版本**: 3.16 或更高

## 构建说明

1. **克隆仓库**
   ```bash
   git clone <repository-url>
   cd Talkbox-client
   ```

2. **创建构建目录**
   ```bash
   mkdir build
   cd build
   ```

3. **配置和编译**
   ```bash
   cmake ..
   make
   ```

4. **运行程序**
   ```bash
   make run
   # 或者直接执行
   ./Talkbox-client
   ```

## 项目结构

```
Talkbox-client/
├── src/                    # 源代码目录
│   ├── main.cpp           # 程序入口
│   ├── mainwindow.*       # 主窗口
│   ├── auth/              # 认证模块
│   │   ├── LoginDialog.*  # 登录对话框
│   │   └── RegisterDialog.* # 注册对话框
│   ├── chat/              # 聊天模块
│   │   ├── ChatWindow.*   # 聊天窗口
│   │   ├── ContactList.*  # 联系人列表
│   │   └── GroupManager.* # 群组管理
│   ├── forum/             # 论坛模块
│   │   ├── ForumWidget.*  # 论坛主界面
│   │   ├── PostDetail.*   # 帖子详情
│   │   └── CreatePost.*   # 创建帖子
│   └── common/            # 公共模块
│       └── HttpClient.*   # HTTP 客户端
├── build/                 # 构建输出目录
├── CMakeLists.txt        # CMake 配置文件
├── API.md               # API 接口文档
└── README.md           # 项目说明文档
```

## API 接口

客户端通过 HTTP 协议与后端服务器通信。详细的 API 接口文档请参考 [API.md](API.md)。

### 主要接口

- **用户管理**: 注册、登录、获取用户信息
- **聊天功能**: 发送消息、获取聊天记录
- **群组管理**: 创建群组、加入群组、群组消息
- **论坛功能**: 发布帖子、获取帖子列表、帖子详情

## 使用说明

1. **启动应用**: 运行 `./Talkbox-client`
2. **用户注册**: 首次使用需要注册新账号
3. **登录**: 使用注册的账号密码登录
4. **开始聊天**: 添加联系人，开始一对一或群组聊天
5. **论坛交流**: 浏览论坛，发布和回复帖子



## 许可证

本项目采用 [LICENSE](LICENSE) 许可证。

## 联系方式

如有问题或建议，请通过以下方式联系：

- 提交 Issue
- 发送邮件至项目维护者

## 版本历史

- **v1.0.0**: 初始版本
  - 基本的用户认证功能
  - 聊天和群组功能
  - 论坛系统
  - Qt6 用户界面
