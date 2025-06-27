# Talkbox Client

一个基于 Qt6 的现代化聊天客户端应用程序，提供即时通讯、群组管理和论坛功能。

## 目录

- [Talkbox Client](#talkbox-client)
  - [目录](#目录)
  - [技术栈](#技术栈)
  - [快速开始](#快速开始)
    - [环境要求](#环境要求)
    - [安装依赖](#安装依赖)
    - [编译运行](#编译运行)
  - [API 文档](#api-文档)
  - [项目结构](#项目结构)
    - [模块说明](#模块说明)
  - [许可证](#许可证)

## 技术栈

- **编程语言**: C++17
- **GUI 框架**: Qt6 (Core, Widgets, Network)
- **构建系统**: CMake 3.16+
- **网络通信**: HTTP/JSON
- **平台支持**: Linux

## 快速开始

### 环境要求

- Linux 操作系统 (已测试)
- GCC 或 Clang 编译器 (支持 C++17)
- Qt6 开发环境
- CMake 3.16 或更高版本

### 安装依赖

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install build-essential cmake qt6-base-dev qt6-tools-dev
```

**Arch Linux:**
```bash
sudo pacman -S base-devel cmake qt6-base qt6-tools
```

### 编译运行

1. **获取源码**
   ```bash
   git clone https://github.com/Flourite423/Talkbox-client.git
   cd Talkbox-client
   ```

2. **创建构建目录**
   ```bash
   mkdir build && cd build
   ```

3. **配置和编译**
   ```bash
   cmake ..
   make
   ```

4. **运行程序**
   ```bash
   ./Talkbox-client
   # 或者使用 make 目标
   make run
   ```

## API 文档

客户端通过 HTTP 协议与后端服务器通信，使用 JSON 格式进行数据交换。


详细的 API 接口文档请参考 [API.md](API.md)。

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
├── LICENSE              # 许可证文件
└── README.md           # 项目说明文档
```

### 模块说明

- **auth**: 用户认证相关功能，包括登录和注册界面
- **chat**: 聊天功能实现，包括聊天窗口、联系人管理和群组功能
- **forum**: 论坛系统，支持帖子的发布、浏览和管理
- **common**: 公共模块，包含 HTTP 客户端等共用组件

## 许可证

详细信息请查看 [LICENSE](LICENSE) 文件。

