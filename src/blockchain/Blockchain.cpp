#include "Blockchain.h"
#include "../user/User.h"

using namespace std;

void Blockchain::mineNextBlock() {
    vector<Transaction> txs = getValidTxs(100);
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
    balances = std::move(working_balances);

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

void Blockchain::setBalances(const vector<User>& users) {
    balances.clear();
    balances.reserve(users.size());

    for (const auto& usr : users) {
        balances[usr.public_key] = usr.balance;
    }
}

array<uint8_t, 16> Blockchain::rehashTransaction(const Transaction& tx) {
    string data;
    data.clear();

    data += tx.getSender();
    data += tx.getReceiver();
    data += to_string(tx.getAmount());

    vector<uint8_t> buffer;
    for (char c : data) {
        buffer.push_back(static_cast<uint8_t>(c));
    }

    array<uint8_t, 16> transaction_id = Hasher::hash(buffer);
    return transaction_id;
}

bool Blockchain::isOverspend(const Transaction& tx) {
    return working_balances[tx.getSender()] < tx.getAmount();
}

vector<Transaction> Blockchain::getValidTxs(int64_t count) {
    // Get 100 valid transactions from mempool, discard invalid
    vector<Transaction> selected;
    working_balances = balances;
    if (mempool.empty()) return selected;

    while (selected.size() < count && !mempool.empty()) {
        Transaction candidate_tx = mempool.back();
        if (rehashTransaction(candidate_tx) == candidate_tx.getId()
            && !isOverspend(candidate_tx)) {
            working_balances[candidate_tx.getSender()] -= candidate_tx.getAmount();
            working_balances[candidate_tx.getReceiver()] += candidate_tx.getAmount();
            selected.push_back(candidate_tx);
        }
        mempool.pop_back();
    }

    return selected;
}