cmake_minimum_required(VERSION 3.10)
project(VideoFiltersApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

find_package(Threads REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB SOURCES "src/*.cpp")

add_executable(VideoFiltersApp ${SOURCES})

target_link_libraries(VideoFiltersApp ${OpenCV_LIBS} Threads::Threads)
