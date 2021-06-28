cmake_minimum_required(VERSION 3.14)
project(Project C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "~/Fall 2019/coms327/Project")

add_executable(
        info info.c
        config.c headers/config.h
        errors.c headers/errors.h
        helpers.c headers/helpers.h
)

add_executable(
        change change.c
        moves.c headers/moves.h
        config.c headers/config.h
        errors.c headers/errors.h
        helpers.c headers/helpers.h
)

add_executable(
        rankmoves rankmoves.c
        rank.c headers/rank.h
        moves.c  headers/moves.h
        config.c headers/config.h
        errors.c headers/errors.h
        helpers.c headers/helpers.h
)

add_executable(
        edit edit.c
        moves.c  headers/moves.h
        config.c headers/config.h
        errors.c headers/errors.h
        helpers.c headers/helpers.h
        render.c headers/render.h)