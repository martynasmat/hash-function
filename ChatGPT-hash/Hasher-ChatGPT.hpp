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

    static array<uint8_t, 16> hash(const vector<uint8_t>& in) {
        // 256-bit state with distinct odd constants
        uint64_t s[4] = {
            0x243F6A8885A308D3ULL, 0x13198A2E03707344ULL,
            0xA4093822299F31D0ULL, 0x082EFA98EC4E6C89ULL
        };

        // Process 32-byte blocks if possible; otherwise 16 then tail
        size_t i = 0;
        auto rd64 = [&](const uint8_t* p){ uint64_t v; memcpy(&v, p, 8); return v; };

        // Main loop: 16-byte chunks for simplicity
        for (; i + 16 <= in.size(); i += 16) {
            uint64_t m0 = rd64(&in[i]);
            uint64_t m1 = rd64(&in[i+8]);
            s[0] ^= m0; s[1] ^= m1;
            for (int r = 0; r < 4; ++r) mix_round(s); // a few rounds
            s[2] ^= m0; s[3] ^= m1;                   // feed-forward on other lanes
        }

        // Tail
        uint8_t last[16] = {0};
        size_t rem = in.size() - i;
        memcpy(last, &in[i], rem);

        // Append 64-bit length (in bytes) into the last 8 bytes
        uint64_t len = static_cast<uint64_t>(in.size());
        memcpy(last + 8, &len, 8);

        uint64_t t0 = rd64(last);
        uint64_t t1 = rd64(last + 8);
        s[0] ^= t0; s[1] ^= t1;
        for (int r = 0; r < 6; ++r) mix_round(s); // stronger finalization
        s[2] ^= t0; s[3] ^= t1;

        // Final fold to 128-bit (or keep 256-bit if you prefer)
        uint64_t h0 = s[0] ^ s[2];
        uint64_t h1 = s[1] ^ s[3];

        array<uint8_t,16> out{};
        memcpy(out.data()+0, &h0, 8);
        memcpy(out.data()+8, &h1, 8);
        return out;
    }
};
