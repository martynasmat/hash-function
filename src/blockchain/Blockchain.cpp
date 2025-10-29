#include "Blockchain.h"

using namespace std;

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
