cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(delaunay C)

set(CMAKE_SKIP_RULE_DEPENDENCY TRUE)

add_library(
    delaunay
    STATIC
    delaunay.c
    delaunay.h
    )
