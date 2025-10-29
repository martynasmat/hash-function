#include <array>
#include <string>

using namespace std;

class Block {
private:
    array<uint8_t,16> prev_block_hash;
    uint64_t timestamp;
    string version;
    uint64_t nonce;
    string difficulty_target;
    array<uint8_t, 16> root_hash;
};
