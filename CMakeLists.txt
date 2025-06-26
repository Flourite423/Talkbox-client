cmake_minimum_required(VERSION 3.19)
project(Talkbox-client LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network)

qt_standard_project_setup()

qt_add_executable(Talkbox-client
    WIN32 MACOSX_BUNDLE
    # 主要文件
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
    src/apiclient.cpp
    src/apiclient.h
    src/appmanager.cpp
    src/appmanager.h
    
    # 认证模块
    src/auth/loginwidget.cpp
    src/auth/loginwidget.h
    src/auth/login.ui
    src/auth/registerwidget.cpp
    src/auth/registerwidget.h
    src/auth/register.ui
    
    # 聊天模块
    src/chat/chatmainwidget.cpp
    src/chat/chatmainwidget.h
    src/chat/chat_main.ui
    src/chat/create_group_dialog.ui
    
    # 论坛模块
    src/forum/forummainwidget.cpp
    src/forum/forummainwidget.h
    src/forum/forum_main.ui
    src/forum/new_post_dialog.ui
    src/forum/post_detail.ui
)

target_link_libraries(Talkbox-client
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Network
)

include(GNUInstallDirs)

install(TARGETS Talkbox-client
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Talkbox-client
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
