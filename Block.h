#include <array>
#include <string>
#include <cstdint>
#include "Transaction.h"

using namespace std;

class Block {
private:
    array<uint8_t,16> prev_block_hash;
    uint64_t timestamp;
    string version;
    uint64_t nonce;
    string difficulty_target;
    array<uint8_t, 16> root_hash;

    vector<Transaction> transactions;

public:
    Block(const std::array<uint8_t,16>& prev_hash,
          const std::vector<Transaction>& txs,
          const std::string& diff_target);

    void mine();

    const array<uint8_t,16>& getHash() const {
        return block_hash;
    }

    const array<uint8_t,16>& getPrevHash() const {
        return prev_block_hash;
    }

    const array<uint8_t,16>& getRootHash() const {
        return root_hash;
    }

    uint64_t getNonce() const {
        return nonce;
    }

    uint64_t getTimestamp() const {
        return timestamp;
    }

    const string& getVersion() const {
        return version;
    }

    const vector<Transaction>& getTransactions() const {
        return transactions;
    }
};
