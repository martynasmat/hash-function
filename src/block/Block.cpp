#include "Block.h"
#include <ctime>
#include <utility>
#include <vector>
#include <iostream>

using namespace std;

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

Block::Block(const array<uint8_t, 16>& prev_hash,
             const vector<Transaction>& txs,
             string  diff_target)
        : prev_block_hash(prev_hash),
          timestamp(static_cast<uint64_t>(time(nullptr))),
          version("0.2"),
          nonce(0),
          difficulty_target(std::move(diff_target)),
          transactions(txs)
{
    hashRoot();
    block_hash.fill(0);
}

void Block::hashRoot() {
    vector<array<uint8_t,16>> leaves;
    leaves.reserve(transactions.size());

    for (const auto& tx : transactions) {
        leaves.push_back(tx.getId());
    }

    // No transactions
    if (leaves.empty()) {
        vector<uint8_t> zero_buf{0};
        root_hash = Hasher::hash(zero_buf);
        return;
    }

    vector<uint8_t> buffer;

    while (leaves.size() > 1) {
        // Duplicate last if odd number
        if (leaves.size() % 2 != 0) {
            leaves.push_back(leaves.back());
        }

        vector<array<uint8_t,16>> leaves_next;

        // Hash each pair in level
        for (size_t i = 0; i < leaves.size(); i += 2) {
            buffer.clear();
            buffer.insert(buffer.end(), leaves[i].begin(), leaves[i].end());
            buffer.insert(buffer.end(), leaves[i + 1].begin(), leaves[i + 1].end());

            leaves_next.push_back(Hasher::hash(buffer));
        }

        leaves = std::move(leaves_next);
    }

    root_hash = leaves[0];
}


array<uint8_t,16> Block::hashHeader()  {
    string data;

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

    return Hasher::hash(buffer);
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
    cout << "Merkle Root:    " << toHex(root_hash) << "\n";
    cout << "Block Hash:   " << toHex(block_hash) << "\n";
    cout << "Tx count:     " << transactions.size() << "\n\n";

    cout << "Transactions:\n";
    for (const auto& tx : transactions) {
        cout << tx.getSender() << " -> "
             << tx.getReceiver() << " amt: " << tx.getAmount()
             << " coins / id=" << Block::toHex(tx.getId()) << "\n";
    }

    cout << "\n\n";
}
