cmake_minimum_required(VERSION 3.20)
project(src)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_link_options(-lncurses)

add_executable(src
        cpoint.h
        main.cpp
        screen.cpp
        screen.h
        snake.cpp
        snake.h
        stack.h
        winsys.cpp
        winsys.h
        )

