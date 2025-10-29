#include "Blockchain.h"

using namespace std;

void Blockchain::mineNextBlock() {
    vector<Transaction> txs = getRandomTxs(100);
    if (txs.empty()) {
        return;
    }

    array<uint8_t,16> prev_block_hash{};
    if (head == nullptr) {
        // Genesis block
        prev_block_hash.fill(0);
    } else {
        prev_block_hash = head->block.getHash();
    }

    Block new_block(prev_block_hash, txs, difficulty);
    new_block.mine();

    HashPointer hp(prev_block_hash, head);
    auto* new_node = new BlockNode(std::move(new_block), hp);
    head = new_node;
    block_count += 1;
}

const Block* Blockchain::getBlockByIndex(uint64_t index) const {
    if (index >= block_count) return nullptr;

    size_t stepsFromHead = (block_count - 1) - index;
    const BlockNode* cur = head;
    for (size_t i = 0; i < stepsFromHead && cur; ++i) {
        cur = cur->prev_hp.prev_block_ptr;
    }
    return cur ? &cur->block : nullptr;
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
