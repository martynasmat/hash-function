#pragma once

#include "../block/Block.h"
#include <utility>
#include <vector>
#include <array>
#include <cstdint>
#include <string>
#include <iostream>

using namespace std;

struct BlockNode;

struct HashPointer {
    array<uint8_t,16> prev_block_hash;
    BlockNode* prev_block_ptr;

    HashPointer()
            : prev_block_hash{}, prev_block_ptr(nullptr)
    {
        prev_block_hash.fill(0);
    }

    HashPointer(const std::array<uint8_t,16>& h, BlockNode* p)
            : prev_block_hash(h), prev_block_ptr(p) {}
};

struct BlockNode {
    Block block;
    HashPointer prev_hp;

    BlockNode(Block  b, const HashPointer& hp)
            : block(std::move(b)), prev_hp(hp) {}

    // Genesis block
    BlockNode(Block  b)
            : block(std::move(b)), prev_hp() {}
};

class Blockchain {
private:
    BlockNode* head;
    string difficulty;
    size_t block_count;
    vector<Transaction> mempool;

public:
    explicit Blockchain(string  diff)
            : head(nullptr),
              difficulty(std::move(diff)),
              block_count(0) {}

    void genGenesisBlock(const std::vector<Transaction>& txs);

    void genBlock(const std::vector<Transaction>& txs);

    void printChain() const;

    size_t getCount() const { return block_count; }

    void loadIntoMempool(const std::vector<Transaction>& txs) {
        mempool = txs;
    }

    bool isMempoolEmpty() const {
        return mempool.empty();
    }
};