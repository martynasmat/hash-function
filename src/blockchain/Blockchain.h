#pragma once

#include "../block/Block.h"
#include "../user/User.h"
#include <utility>
#include <vector>
#include <array>
#include <cstdint>
#include <string>
#include <iostream>
#include <unordered_map>

using namespace std;

struct BlockNode;

struct HashPointer {
    array<uint8_t,16> prev_block_hash;
    BlockNode* prev_block_ptr;

    HashPointer(const std::array<uint8_t,16>& h, BlockNode* p)
            : prev_block_hash(h), prev_block_ptr(p) {}
};

struct BlockNode {
    Block block;
    HashPointer prev_hp;

    BlockNode(Block  b, const HashPointer& hp)
            : block(std::move(b)), prev_hp(hp) {}
};

class Blockchain {
private:
    BlockNode* head;
    string difficulty;
    size_t block_count;
    vector<Transaction> mempool;
    vector<Transaction> rejected_txs;
    unordered_map<string, int64_t> balances;
    unordered_map<string, int64_t> working_balances;

public:
    explicit Blockchain(string  diff)
            : head(nullptr),
              difficulty(std::move(diff)),
              block_count(0) {}

    void printChain() const;

    size_t getCount() const { return block_count; }

    void loadIntoMempool(const std::vector<Transaction>& txs) {
        mempool = txs;
    }

    bool isMempoolEmpty() const {
        return mempool.empty();
    }

    void mineNextBlock();
    const Block* getBlockByIndex(uint64_t index) const;
    void setBalances(const vector<User>& users);

private:
    static array<uint8_t, 16> rehashTransaction(const Transaction& tx);
    vector<Transaction> getValidTxs(int64_t count);
    bool isOverspend(const Transaction& tx);
};
