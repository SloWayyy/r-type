cmake_minimum_required(VERSION 3.18)


#######################CLIENT#######################

project(r-type_client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include_directories(client/include)

set(SRC_CLIENT client/main.cpp
)

add_executable(r-type_client ${SRC_CLIENT})

target_link_libraries(r-type_client 
    ${CONAN_LIBS}
)

#######################SERVER#######################

project(r-type_server LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(server/include)

set(SRC_SERVER server/main.cpp
)

add_executable(r-type_server ${SRC_SERVER})

target_link_libraries(r-type_server 
    ${CONAN_LIBS}
)


