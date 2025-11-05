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
    unordered_map<string, int64_t> balances;

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
    array<uint8_t, 16> rehashTransaction(const Transaction& tx);

private:
    vector<Transaction> getRandomTxs(int64_t count) {
        vector<Transaction> selected;
        if (mempool.empty()) return selected;

        mt19937_64 rng(251029);
        uniform_int_distribution<uint64_t> dist(0, mempool.size() - 1);

        for (size_t i = 0; i < count && !mempool.empty(); i++) {
            uint64_t index = dist(rng);
            selected.push_back(mempool[index]);
            mempool[index] = mempool.back();
            mempool.pop_back();
        }

        return selected;
    }
};
