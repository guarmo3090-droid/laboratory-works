cmake_minimum_required(VERSION 3.10)
project(VideoFiltersApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# пакет OpenCV
find_package(OpenCV REQUIRED)

# turn on директорію заголовних файлів
include_directories(${PROJECT_SOURCE_DIR}/include)

# find all .cpp files in src
file(GLOB SOURCES "src/*.cpp")

# Створюємо виконуваний файл
add_executable(VideoFiltersApp ${SOURCES})

# Лінкуємо бібліотеки OpenCV
target_link_libraries(VideoFiltersApp ${OpenCV_LIBS})
