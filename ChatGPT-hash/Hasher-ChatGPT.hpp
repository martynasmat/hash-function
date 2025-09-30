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
        return (x << r) | (x >> (64 - r));
    }

    static void mix_round(uint64_t s[4]) {
        s[0] += s[1]; s[2] += s[3];
        s[1] = rotl64(s[1], 13) ^ s[0];
        s[3] = rotl64(s[3], 16) ^ s[2];
        s[0] = rotl64(s[0], 32) + s[3];
        s[2] = rotl64(s[2], 21) + s[1];
        s[1] = rotl64(s[1], 17);
        s[3] = rotl64(s[3], 32);
    }

    // Load little-endian regardless of host endianness
    static inline uint64_t load64_le(const uint8_t* p) {
        uint64_t v = 0;
        for (int i = 0; i < 8; ++i) v |= uint64_t(p[i]) << (8 * i);
        return v;
    }

    static inline void absorb(uint64_t s[4], uint64_t m0, uint64_t m1, int rounds) {
        s[0] ^= m0; s[1] ^= m1;
        for (int r = 0; r < rounds; ++r) mix_round(s);
        s[2] ^= m0; s[3] ^= m1;
    }

    static std::array<uint8_t,16> hash(const std::vector<uint8_t>& in) {
        // 256-bit state
        uint64_t s[4] = {
            0x243F6A8885A308D3ULL, 0x13198A2E03707344ULL,
            0xA4093822299F31D0ULL, 0x082EFA98EC4E6C89ULL
        };

        const int ROUNDS_PER_BLOCK = 4;

        // Process full 16-byte blocks
        size_t i = 0;
        for (; i + 16 <= in.size(); i += 16) {
            uint64_t m0 = load64_le(&in[i]);
            uint64_t m1 = load64_le(&in[i + 8]);
            absorb(s, m0, m1, ROUNDS_PER_BLOCK);
        }

        // ------ Proper padding (SHA-like) ------
        // Build tail = remaining bytes + 0x80 + zeros until len mod 16 == 8, then 64-bit length
        std::vector<uint8_t> tail;
        tail.reserve(32); // at most two blocks
        tail.insert(tail.end(), in.begin() + i, in.end());
        tail.push_back(0x80);
        while ((tail.size() % 16) != 8) tail.push_back(0x00);

        uint64_t len = static_cast<uint64_t>(in.size());
        for (int k = 0; k < 8; ++k) tail.push_back(uint8_t((len >> (8 * k)) & 0xFF));
        // --------------------------------------

        // Absorb padded tail blocks
        for (size_t j = 0; j < tail.size(); j += 16) {
            uint64_t m0 = load64_le(&tail[j]);
            uint64_t m1 = load64_le(&tail[j + 8]);
            absorb(s, m0, m1, ROUNDS_PER_BLOCK);
        }

        // Finalization
        for (int r = 0; r < 8; ++r) mix_round(s);

        uint64_t h0 = s[0] ^ s[2];
        uint64_t h1 = s[1] ^ s[3];

        std::array<uint8_t,16> out{};
        for (int k = 0; k < 8; ++k) out[k]      = uint8_t((h0 >> (8 * k)) & 0xFF);
        for (int k = 0; k < 8; ++k) out[8 + k]  = uint8_t((h1 >> (8 * k)) & 0xFF);
        return out;
    }
};
