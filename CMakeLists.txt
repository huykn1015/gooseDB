cmake_minimum_required(VERSION 3.9.1)
project(gooseDB)
add_library(http_tcp_linux ./HTTP_/src/http_tcp_linux.cpp ./HTTP_/include/http_tcp_linux.hpp)
add_library(kvs_lib ./kvs/src/kvs.cc ./kvs/include/kvs.h)