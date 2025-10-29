#include <array>
#include <string>
#include <cstdint>
#include "Transaction.h"
#include <vector>
#include <hashfunc.hpp>

using namespace std;

class Block {
private:
    array<uint8_t,16> prev_block_hash;
    uint64_t timestamp;
    string version;
    uint64_t nonce;
    string difficulty_target;
    array<uint8_t, 16> root_hash;

    array<uint8_t, 16> block_hash;

    vector<Transaction> transactions;

    Hasher hasher;

public:
    Block(const array<uint8_t, 16>& prev_hash,
          const vector<Transaction>& txs,
          const string& diff_target);

    void mine();

    static string toHex(const array<uint8_t, 16>& bytes);

    const array<uint8_t, 16>& getHash() const {
        return block_hash;
    }

    const array<uint8_t, 16>& getPrevHash() const {
        return prev_block_hash;
    }

    const array<uint8_t, 16>& getRootHash() const {
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

private:
    array<uint8_t,16> hashHeader();
    array<uint8_t,16> hashRoot();
};
