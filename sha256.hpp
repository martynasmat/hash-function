// sha256_ossl.hpp
#pragma once
#include <array>
#include <vector>
#include <string>
#include <cstdint>
#include <openssl/sha.h>

inline std::array<uint8_t,32> sha256_bytes(const uint8_t* data, size_t len) {
    std::array<uint8_t,32> out{};
    SHA256(data, len, out.data());          // one-shot SHA-256
    return out;                              // 32 bytes
}

inline std::array<uint8_t,32> sha256_bytes(const std::vector<uint8_t>& v) {
    return sha256_bytes(v.data(), v.size());
}

inline std::array<uint8_t,32> sha256_string(const std::string& s) {
    return sha256_bytes(reinterpret_cast<const uint8_t*>(s.data()), s.size());
}

inline std::string to_hex_any(const uint8_t* p, size_t n) {
    static const char* H = "0123456789abcdef";
    std::string s; s.resize(n*2);
    for (size_t i=0;i<n;++i){ s[2*i]=H[p[i]>>4]; s[2*i+1]=H[p[i]&0xF]; }
    return s;
}
