# Repository Guidelines

## Project Overview

Talkbox Client is a Qt6 C++ desktop GUI application for real-time chat, group messaging, and BBS forum. It communicates with Talkbox Server via HTTP/JSON API. Built with CMake, it targets Linux platforms.

## Architecture & Data Flow

```
User Interaction (Qt Widgets)
    ↓
MainWindow (QStackedWidget, tab-based UI)
    ↓
Feature Modules
├── Auth (LoginDialog, RegisterDialog)
├── Chat (ChatWindow, ContactList, GroupManager)
├── Forum (ForumWidget, PostDetail, CreatePost)
└── Common (HttpClient)
    ↓
HttpClient (QNetworkAccessManager)
    ↓
Talkbox Server (HTTP/JSON API)
```

**Entry point**: `src/main.cpp` → creates `QApplication` → creates `MainWindow` → shows login dialog → on success, shows main UI.

**Request flow**: UI events → feature modules → `HttpClient::get/post()` → `QNetworkAccessManager` → HTTP to server → JSON response → signals back to UI.

## Key Directories

- `src/` — All source code
  - `src/auth/` — Login and registration dialogs
  - `src/chat/` — Chat windows, contact list, group management
  - `src/forum/` — Forum widget, post details, create post
  - `src/common/` — HTTP client wrapper
- `.github/workflows/` — CI configuration

## Development Commands

```bash
# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
make

# Run the application
./Talkbox-client

# Or use make target
make run

# Clean build artifacts
make clean-all
```

## Code Conventions & Common Patterns

### Qt Patterns
- Uses Qt's signal/slot mechanism for async communication
- Each UI component gets its own `HttpClient` instance (avoids signal conflicts)
- `.ui` files for Qt Designer form definitions
- `Q_OBJECT` macro in all QObject-derived classes

### Module Structure
Each feature module follows this pattern:
- Header (.h) declares class with Q_OBJECT macro
- Source (.cpp) implements slots and business logic
- UI file (.ui) defines layout (optional, for complex forms)
- Constructor takes parent QWidget* and HttpClient*

### HttpClient Usage
```cpp
// Set base URL and username
m_httpClient->setBaseUrl("http://localhost:8080");
m_httpClient->setUsername(username);

// Make requests
m_httpClient->post("/api/login", jsonObject);
m_httpClient->get("/api/get_messages", queryParams);

// Connect to signals
connect(m_httpClient, &HttpClient::responseReceived, this, &MyClass::onResponse);
connect(m_httpClient, &HttpClient::errorOccurred, this, &MyClass::onError);
```

### Naming Conventions
- Classes: PascalCase (`ChatWindow`, `ContactList`)
- Methods: camelCase (`onLoginSuccessful`, `setupUI`)
- Files: PascalCase matching class name (`ChatWindow.cpp`)
- Member variables: m_ prefix (`m_httpClient`, `m_token`)
- Slots: on + PascalCase (`onContactSelected`, `onTabChanged`)

### UI Structure
- MainWindow uses QStackedWidget for tab-based navigation
- Login dialog shown first, main UI hidden until successful auth
- Refresh throttling: 30-second interval for group/forum/contact tabs

## Important Files

### Entry & Core
- `src/main.cpp` — Application entry point
- `src/mainwindow.h/cpp/ui` — Main window, tab management, routing

### Auth Module
- `src/auth/LoginDialog.h/cpp/ui` — Login form
- `src/auth/RegisterDialog.h/cpp/ui` — Registration form

### Chat Module
- `src/chat/ChatWindow.h/cpp/ui` — Message display and input
- `src/chat/ContactList.h/cpp/ui` — User list with online status
- `src/chat/GroupManager.h/cpp/ui` — Group creation/join/leave

### Forum Module
- `src/forum/ForumWidget.h/cpp/ui` — Post list with pagination
- `src/forum/PostDetail.h/cpp/ui` — Single post view with replies
- `src/forum/CreatePost.h/cpp/ui` — New post form

### Common
- `src/common/HttpClient.h/cpp` — HTTP client wrapper around QNetworkAccessManager

### Build & Config
- `CMakeLists.txt` — CMake build configuration (Qt6, C++17)
- `.github/workflows/ci.yml` — GitHub Actions CI

### Documentation
- `README.md` — Project overview, quickstart, structure (Chinese)
- `API.md` — HTTP API documentation (Chinese)

## Runtime/Tooling Preferences

- **Language**: C++17
- **GUI Framework**: Qt6 (Core, Widgets, Network)
- **Build system**: CMake 3.16+
- **Platform**: Linux (tested)
- **No unit tests**: No test framework configured
- **No package manager**: System packages only (apt/pacman for qt6-base-dev)

## Testing & QA

### Test Framework
- No unit test framework configured
- No test files found in repository

### Manual Testing
- Run application and test UI flows manually
- Requires running Talkbox Server instance

### CI
- GitHub Actions runs on push/PR to main/master
- Builds and verifies binary exists
- No automated tests (no test step in CI)

### Adding Tests
To add unit tests, consider:
- Qt Test module (`Qt6::Test`)
- Google Test (requires integration with CMake)
- Place tests in `tests/` directory
