cmake_minimum_required(VERSION 3.18)

project(r-type_server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

set(SRC_SERVER server/main.cpp
                network/tcpServer/tcpServer.cpp
                ecs/system/system.cpp
                ecs/component/component.cpp
)

add_executable(r-type_server ${SRC_SERVER})

target_link_libraries(r-type_server
    PRIVATE asio
    PRIVATE sfml-system
    PRIVATE sfml-window
    PRIVATE sfml-graphics
)