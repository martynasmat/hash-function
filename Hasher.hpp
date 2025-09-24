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
    // Rotate function
    // https://blog.regehr.org/archives/1063
    static inline uint8_t rotl(uint8_t n, unsigned int c)
    {
        const unsigned int mask = (CHAR_BIT*sizeof(n) - 1);
        c &= mask;
        return (n<<c) | (n>>( (-c)&mask ));
    }

    // Rotate left the product of XOR of members of arrays a & b
    // A 16 byte input gives an 8 byte output
    static void mix(uint8_t out[8], const uint8_t a[8], const uint8_t b[8]) {
        for (int i = 0; i < 8; i++) {
            out[i] = rotl(uint8_t(a[i] ^ b[(i + 4) % 8]), 3);
        }
    }

    // Pad input with zero-bytes and length of input at the end so that the total length is divisible by 16
    static vector<uint8_t> pad_input(const vector<uint8_t>& in) {
        vector<uint8_t> p = in;
        uint8_t len = p.size();
        uint8_t padZeros = 15 - (len % 16);
        p.insert(p.end(), padZeros, 0x00);
        p.push_back(static_cast<uint8_t>(len % 256));
        return p;
    }

    static vector<uint8_t> rearrange(const vector<uint8_t>& padded_input, const vector<uint8_t>& org_input) {
        // rearranges characters using the mt19937 generator
        // uses first, middle and last chars of input XOR length of input as seed
        vector<uint8_t> rearranged = padded_input;
        uint8_t seed = org_input[0] + org_input[org_input.size()/2] + org_input.back();
        seed ^= org_input.size();
        mt19937 random(seed);
        shuffle(rearranged.begin(), rearranged.end(), random);
        return rearranged;
    }

    static array<uint8_t,8> hash(const vector<uint8_t>& input) {
        static const uint8_t first[] = { 0x20,0x03,0x01,0x08,0x20,0x03,0x01,0x08 };

        vector<uint8_t> msg = pad_input(input);

        uint8_t acc[8];
        uint8_t next[8];
        memcpy(acc, first, sizeof(acc));

        // apply permutation to padded input before mixing
        msg = rearrange(msg, input);

        for (size_t i = 0; i < msg.size(); i += 8) {
            mix(next, acc, &msg[i]);
            memcpy(acc, next, sizeof(acc));
        }

        array<uint8_t,8> out{};
        memcpy(&out, acc, 8);
        return out;
    }
};