cmake_minimum_required(VERSION 3.18)


project(r-type)

include(libs/depencies.cmake) # Install depencies


if (LINUX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()
if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "NORMAL MODE SET")
  set(CMAKE_BUILD_TYPE "is empty")
elseif (CMAKE_BUILD_TYPE STREQUAL "debug")
  message("CMAKE_BUILD_TYPE is ${CMAKE_BUILD_TYPE}")
  message(STATUS "DEBUG MODE SET")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
elseif(CMAKE_BUILD_TYPE STREQUAL "normal")
  message(STATUS "NORMAL MODE SET")
else()
  message(FATAL_ERROR "Use:\nNORMAL MODE: cmake -B build -DCMAKE_BUILD_TYPE=normal\nDEBUG MODE: cmake -B build -DCMAKE_BUILD_TYPE=debug")
  return()
endif()
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include(client/client.cmake) # Client

include(server/server.cmake) # Server


#######################COPY LIBS#######################
if (WIN32)
    add_custom_command(TARGET r-type_client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_BINARY_DIR}/_deps/sfml-build/lib
        ${CMAKE_CURRENT_BINARY_DIR}/)
endif()
