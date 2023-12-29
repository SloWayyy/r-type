cmake_minimum_required(VERSION 3.18)

project(r-type_client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

set(SRC_CLIENT client/main.cpp
              server/updateGame/updateGame.cpp
              network/tcpClient/tcpClient.cpp
              ecs/system/system.cpp
              ecs/component/component.cpp
)

add_executable(r-type_client ${SRC_CLIENT})

target_link_libraries(r-type_client
    PRIVATE sfml-system
    PRIVATE sfml-window
    PRIVATE sfml-graphics
    PRIVATE sfml-audio
    PRIVATE asio
)
