cmake_minimum_required(VERSION 3.18)


project(r-type)

include(libs/depencies.cmake) # Install depencies

if (LINUX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
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
