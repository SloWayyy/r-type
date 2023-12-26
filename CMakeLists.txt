cmake_minimum_required(VERSION 3.18)




#######################CLIENT#######################

project(r-type_client LANGUAGES CXX)
include(FetchContent)

FetchContent_Declare(asio-cmake
  GIT_REPOSITORY https://github.com/kingsamchen/asio-cmake.git
  GIT_TAG        origin/master
)

FetchContent_Declare(SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG        2.5.x
)

set(ASIO_CMAKE_ASIO_TAG asio-1-12-2)
set(SFML_CMAKE_SFML_TAG 2.5.x)

FetchContent_MakeAvailable(asio-cmake)
FetchContent_MakeAvailable(SFML)

set(CMAKE_CXX_STANDARD 17)
if (LINUX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include_directories(client/include)

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

#######################SERVER#######################

project(r-type_server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(server/include)

set(SRC_SERVER server/main.cpp
                server/updateGame/updateGame.cpp
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


#######################COPY LIBS#######################
if (WIN32)
    add_custom_command(TARGET r-type_client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_BINARY_DIR}/_deps/sfml-build/lib
        ${CMAKE_CURRENT_BINARY_DIR}/)
endif()
