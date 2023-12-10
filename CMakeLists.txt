cmake_minimum_required(VERSION 3.18)




#######################CLIENT#######################

project(r-type_client LANGUAGES CXX)
include(FetchContent)

FetchContent_Declare(asio-cmake
  GIT_REPOSITORY https://github.com/kingsamchen/asio-cmake.git
  GIT_TAG        origin/master
)

set(ASIO_CMAKE_ASIO_TAG asio-1-12-2)

FetchContent_MakeAvailable(asio-cmake)

find_package(SFML COMPONENTS system window graphics audio REQUIRED)
set(CMAKE_CXX_STANDARD 17)
if (LINUX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include_directories(client/include)

set(SRC_CLIENT client/main.cpp
              client/tcp/tcp.cpp
              client/udp/udp.cpp
              client/GameLoop.cpp
)

add_executable(r-type_client ${SRC_CLIENT})

target_link_libraries(r-type_client
    PRIVATE sfml-system
    PRIVATE sfml-window
    PRIVATE sfml-graphics
    PRIVATE sfml-audio
    PRIVATE asio
)

#######################SERVER#######################

project(r-type_server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(server/include)

set(SRC_SERVER server/main.cpp
                server/tcp/tcp.cpp
                server/udp/udp.cpp
                server/server.cpp
)

add_executable(r-type_server ${SRC_SERVER})

target_link_libraries(r-type_server
    PRIVATE asio
)


