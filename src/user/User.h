#pragma once
#include <cstdint>
#include <string>

using namespace std;

struct User {
    string name;
    string public_key;
    int64_t balance;
};