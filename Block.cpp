#include "Block.h"
#include <ctime>
#include <vector>
#include <iostream>

string Block::toHex(const std::array<uint8_t, 16>& bytes) {
    // Converts array<uint8_t, 16> to hex, returns string
    static const char* hexDigits = "0123456789abcdef";

    std::string out;
    out.reserve(bytes.size() * 2);

    for (uint8_t b : bytes) {
        out.push_back(hexDigits[(b >> 4) & 0x0F]);
        out.push_back(hexDigits[b & 0x0F]);
    }

    return out;
}

Block::Block(const std::array<uint8_t, 16>& prev_hash,
             const std::vector<Transaction>& txs,
             const std::string& diff_target)
        : prev_block_hash(prev_hash),
          timestamp(static_cast<uint64_t>(time(nullptr))),
          version("0.1"),
          nonce(0),
          difficulty_target(diff_target),
          transactions(txs)
{
    hashRoot();
}

void Block::hashRoot() {
    vector<uint8_t> buf;

    for (const auto& tx : transactions) {
        const array<uint8_t,16>& tx_id = tx.getId();
        buf.insert(buf.end(), tx_id.begin(), tx_id.end());
    }

    if (buf.empty()) {
        buf.push_back(0);
    }

    root_hash = hasher.hash(buf);
}
