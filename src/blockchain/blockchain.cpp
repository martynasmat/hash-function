#include "Blockchain.h"

using namespace std;

void Blockchain::genGenesisBlock(const vector<Transaction>& txs) {
    // Fill Genesis block prev_block_hash with 0s
    array<uint8_t,16> prev_hash{};
    prev_hash.fill(0);

    Block genesis(prev_hash, txs, difficulty);
    genesis.mine();

    head = new BlockNode(std::move(genesis));
    block_count = 1;
}
