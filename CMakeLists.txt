cmake_minimum_required(VERSION 3.16)

project(Talkbox-client VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 默认使用Unix Makefiles生成器
if(NOT CMAKE_GENERATOR)
    set(CMAKE_GENERATOR "Unix Makefiles")
endif()

# 设置构建类型
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)

qt_standard_project_setup()

# 源文件分组
set(MAIN_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
)

set(AUTH_SOURCES
    src/auth/LoginDialog.cpp
    src/auth/LoginDialog.h
    src/auth/LoginDialog.ui
    src/auth/RegisterDialog.cpp
    src/auth/RegisterDialog.h
    src/auth/RegisterDialog.ui
)

set(CHAT_SOURCES
    src/chat/ChatWindow.cpp
    src/chat/ChatWindow.h
    src/chat/ChatWindow.ui
    src/chat/ContactList.cpp
    src/chat/ContactList.h
    src/chat/ContactList.ui
    src/chat/GroupManager.cpp
    src/chat/GroupManager.h
    src/chat/GroupManager.ui
)

set(FORUM_SOURCES
    src/forum/ForumWidget.cpp
    src/forum/ForumWidget.h
    src/forum/ForumWidget.ui
    src/forum/PostDetail.cpp
    src/forum/PostDetail.h
    src/forum/PostDetail.ui
    src/forum/CreatePost.cpp
    src/forum/CreatePost.h
    src/forum/CreatePost.ui
)

set(COMMON_SOURCES
    src/common/HttpClient.cpp
    src/common/HttpClient.h
)

# 所有源文件
set(ALL_SOURCES
    ${MAIN_SOURCES}
    ${AUTH_SOURCES}
    ${CHAT_SOURCES}
    ${FORUM_SOURCES}
    ${COMMON_SOURCES}
)

qt_add_executable(Talkbox-client ${ALL_SOURCES})

qt_add_resources(Talkbox-client "resources"
    PREFIX "/"
)

target_link_libraries(Talkbox-client 
    PRIVATE 
    Qt6::Core 
    Qt6::Widgets 
    Qt6::Network
)

set_target_properties(Talkbox-client PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

# 添加一些有用的编译选项
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(Talkbox-client PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()

# 为IDE创建源码分组
source_group("Main" FILES ${MAIN_SOURCES})
source_group("Auth" FILES ${AUTH_SOURCES})
source_group("Chat" FILES ${CHAT_SOURCES})
source_group("Forum" FILES ${FORUM_SOURCES})
source_group("Common" FILES ${COMMON_SOURCES})

# 设置包含目录
target_include_directories(Talkbox-client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/auth
    ${CMAKE_CURRENT_SOURCE_DIR}/src/chat
    ${CMAKE_CURRENT_SOURCE_DIR}/src/forum
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common
)

# 添加自定义make目标
add_custom_target(run
    COMMAND ${CMAKE_BINARY_DIR}/Talkbox-client
    DEPENDS Talkbox-client
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running Talkbox-client"
)

add_custom_target(clean-all
    COMMAND ${CMAKE_BUILD_TOOL} clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/CMakeCache.txt
    COMMENT "Cleaning all build files"
)