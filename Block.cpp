#include "Block.h"
#include <ctime>
#include <vector>
#include <iostream>

using namepace std;

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
    block_hash.fill(0);
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


array<uint8_t,16> Block::hashHeader() const {
    string data = "";

    data += toHex(prev_block_hash);
    data += std::to_string(timestamp);
    data += version;
    data += std::to_string(nonce);
    data += difficulty_target;
    data += toHex(root_hash);

    vector<uint8_t> buffer;
    buffer.reserve(data.size());
    for (char c : data) {
        buffer.push_back(static_cast<uint8_t>(c));
    }

    return hasher.hash(buffer);
}

void Block::mine() {
    nonce = 0;

    while (true) {
        array<uint8_t,16> hash_value = hashHeader();
        string hash_value_hex = toHex(hash_value);

        if (hash_value_hex.rfind(difficulty_target, 0) == 0) {
            block_hash = hash_value;
            break;
        }

        nonce++;
    }

    cout << "(+) BLOCK MINED\n";
    cout << "Version:      " << version << "\n";
    cout << "Timestamp:    " << timestamp << "\n";
    cout << "Nonce:        " << nonce << "\n";
    cout << "Difficulty:   " << difficulty_target << "\n";
    cout << "Prev Block Hash:    " << toHex(prev_block_hash) << "\n";
    cout << "Transactions Hash:    " << toHex(root_hash) << "\n";
    cout << "Block Hash:   " << toHex(block_hash) << "\n";
    cout << "Tx count:     " << transactions.size() << "\n";
    cout << "=======================\n";
}
