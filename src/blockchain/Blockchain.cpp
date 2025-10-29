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

void Blockchain::genBlock(const std::vector<Transaction>& txs) {
    std::array<uint8_t,16> prev_hash = head->block.getHash();

    Block newBlock(prev_hash, txs, difficulty);
    newBlock.mine();

    HashPointer hp(prev_hash, head);

    auto* node = new BlockNode(std::move(newBlock), hp);

    head = node;
    block_count += 1;
}

void Blockchain::printChain() const {
    cout << "\n(!) BLOCKCHAIN\n";

    const BlockNode* cur = head;
    size_t index = 0;

    while (cur != nullptr) {
        const Block& block = cur->block;

        cout << "Block no. "
                  << (block_count - 1 - index) << ":\n";

        cout << "  Block Hash:   "
                  << Block::toHex(block.getHash()) << "\n";

        cout << "  Prev Block Hash:    "
                  << Block::toHex(block.getPrevHash()) << "\n";

        cout << "  Nonce:        "
                  << block.getNonce() << "\n";

        cout << "  Tx Count:     "
                  << block.getTransactions().size() << "\n";

        if (cur->prev_hp.prev_block_ptr == nullptr) {
            // Genesis block
            cout << "  HashPointer.prev_block_ptr:  nullptr\n";
            cout << "  HashPointer.prev_block_hash: "
                      << Block::toHex(cur->prev_hp.prev_block_hash)
                      << "\n\n";
        } else {
            cout << "  HashPointer.prev_block_ptr:  "
                      << cur->prev_hp.prev_block_ptr << "\n";

            cout << "  HashPointer.prev_block_hash: "
                      << Block::toHex(cur->prev_hp.prev_block_hash)
                      << "\n\n";
        }

        cur = cur->prev_hp.prev_block_ptr;
        index++;
    }
}
