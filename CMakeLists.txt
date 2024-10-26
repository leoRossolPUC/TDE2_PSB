cmake_minimum_required(VERSION 3.29)
project(TDE2_PSB C)

set(CMAKE_C_STANDARD 11)

add_executable(TDE2_PSB main.c
        mymemory.h
        mymemory.c)
