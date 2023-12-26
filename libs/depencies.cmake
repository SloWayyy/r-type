cmake_minimum_required(VERSION 3.18)

project(Asio-SFML-Example)

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