cmake_minimum_required(VERSION 3.26)
project(blockchain)

set(CMAKE_CXX_STANDARD 17)

add_executable(blockchain
        src/main.cpp
        src/block/Block.cpp
        src/transaction/Transaction.cpp
        src/hasher/hashfunc.hpp
        src/transaction/Transaction.cpp
        src/blockchain/blockchain.h
)
