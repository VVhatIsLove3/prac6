cmake_minimum_required(VERSION 3.10)
project(my_project)

set(CMAKE_C_STANDARD 11)

# Добавляем исполняемые файлы для каждой программы
find_package(Threads REQUIRED)
link_libraries(Threads::Threads)
add_executable(6_1 6-1.c)
add_executable(6_2 6-2.c)


if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/index)
    add_subdirectory(index)
endif()

if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/arind)
    add_subdirectory(arind)
endif()
