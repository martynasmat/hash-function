cmake_minimum_required(VERSION 3.26)
project(blockchain)

set(CMAKE_CXX_STANDARD 17)

add_executable(blockchain
        main.cpp
        Block.cpp
        Transaction.cpp
        hashfunc.hpp
        Transaction.cpp
)
