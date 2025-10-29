#pragma once

#include <iostream>
#include <iomanip>
#include <cstdint>
#include <vector>
#include <array>
#include <algorithm>
#include <random>
#include <climits>
#include <cstring>

using namespace std;

struct Hasher {
    static inline uint64_t rotl64(uint64_t x, unsigned r) {
    #if defined(_MSC_VER)
        return _rotl64(x, r);
    #else
        return (x << r) | (x >> (64 - r));
    #endif
    }

    static inline void mix_round(uint64_t s[4]) {
        // simple ARX that mixes all lanes
        s[0] += s[1];  s[2] += s[3];
        s[1] = rotl64(s[1], 13) ^ s[0];
        s[3] = rotl64(s[3], 16) ^ s[2];
        s[0] = rotl64(s[0], 32) + s[3];
        s[2] = rotl64(s[2], 21) + s[1];
        s[1] = rotl64(s[1], 17) ^ s[2];
        s[3] = rotl64(s[3], 32) ^ s[0];
    }

    // unaligned little-endian load via memcpy (optimizes well)
    static inline uint64_t load64_le(const uint8_t* p) {
        uint64_t v; std::memcpy(&v, p, 8);
    #if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
        v = __builtin_bswap64(v);
    #endif
        return v;
    }

    static inline void absorb32(uint64_t s[4],
                                uint64_t m0, uint64_t m1,
                                uint64_t m2, uint64_t m3,
                                int rounds)
    {
        s[0] ^= m0; s[1] ^= m1; s[2] ^= m2; s[3] ^= m3;
        for (int r = 0; r < rounds; ++r) mix_round(s);
        // feed-forward on a different pairing
        s[0] ^= m2; s[1] ^= m3; s[2] ^= m0; s[3] ^= m1;
    }

    static std::array<uint8_t,16> hash(const std::vector<uint8_t>& in) {
        // 256-bit state (nothing-up-my-sleeve constants)
        uint64_t s[4] = {
            0x243F6A8885A308D3ULL, 0x13198A2E03707344ULL,
            0xA4093822299F31D0ULL, 0x082EFA98EC4E6C89ULL
        };

        constexpr int ROUNDS_PER_BLOCK = 2;   // tune: 2 is fast, 3 is stronger
        constexpr int FINAL_ROUNDS     = 8;

        // ---- main loop: 32-byte blocks ----
        const uint8_t* p = in.data();
        size_t n = in.size();
        size_t nb = n / 32;
        for (size_t b = 0; b < nb; ++b, p += 32) {
            uint64_t m0 = load64_le(p +  0);
            uint64_t m1 = load64_le(p +  8);
            uint64_t m2 = load64_le(p + 16);
            uint64_t m3 = load64_le(p + 24);
            absorb32(s, m0, m1, m2, m3, ROUNDS_PER_BLOCK);
        }

        // ---- padding: SHA-like with 64-bit length at end ----
        // Total bytes must end at ... 24 mod 32 (last 8 bytes hold length).
        uint8_t last1[32] = {0};
        uint8_t last2[32] = {0};
        size_t rem = n & 31; // n % 32

        // copy remainder
        if (rem) std::memcpy(last1, p, rem);
        // append 0x80
        last1[rem] = 0x80;

        // where should length go?
        // we want length at offset 24..31 of the final block
        if (rem <= 24) {
            // fits in one block
            uint64_t len = static_cast<uint64_t>(n);
            std::memcpy(last1 + 24, &len, 8);
            // absorb last1
            absorb32(s,
                load64_le(last1 +  0),
                load64_le(last1 +  8),
                load64_le(last1 + 16),
                load64_le(last1 + 24),
                ROUNDS_PER_BLOCK);
        } else {
            // need two blocks; length goes into second
            uint64_t len = static_cast<uint64_t>(n);
            // absorb last1 as-is
            absorb32(s,
                load64_le(last1 +  0),
                load64_le(last1 +  8),
                load64_le(last1 + 16),
                load64_le(last1 + 24),
                ROUNDS_PER_BLOCK);
            // second block = zeros ... len at tail
            std::memcpy(last2 + 24, &len, 8);
            absorb32(s,
                load64_le(last2 +  0),
                load64_le(last2 +  8),
                load64_le(last2 + 16),
                load64_le(last2 + 24),
                ROUNDS_PER_BLOCK);
        }

        // ---- finalization ----
        for (int r = 0; r < FINAL_ROUNDS; ++r) mix_round(s);
        uint64_t h0 = s[0] ^ s[2];
        uint64_t h1 = s[1] ^ s[3];

        std::array<uint8_t,16> out{};
        std::memcpy(out.data() + 0, &h0, 8);
        std::memcpy(out.data() + 8, &h1, 8);
        return out;
    }
};
