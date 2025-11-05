#include "Blockchain.h"
#include "../user/User.h"
#include <unordered_set>
#include <algorithm>

using namespace std;

void Blockchain::mineNextBlock() {
    if (mempool.empty()) {
        return;
    }

    array<uint8_t,16> prev_block_hash{};
    if (head == nullptr) {
        // Genesis block
        prev_block_hash.fill(0);
    } else {
        prev_block_hash = head->block.getHash();
    }

    vector<Transaction> mempool_snapshot = mempool;
    unordered_map<string, int64_t> balances_snapshot = balances;

    // Create 5 candidate blocks
    vector<Candidate> candidates;
    candidates.reserve(5);

    for (size_t i = 0; i < 5 && !mempool_snapshot.empty(); ++i) {
        Candidate candidate = buildCandidate(mempool_snapshot, balances_snapshot);
        if (!candidate.transactions.empty()) {
            candidates.push_back(std::move(candidate));
        }
    }

    if (candidates.empty()) {
        rejected_txs.insert(rejected_txs.end(), mempool.begin(), mempool.end());
        mempool.clear();
        working_balances = balances;
        return;
    }

    uint64_t attempt_window_ms = 5000;

    // Attempt to mine candidate blocks
    while (true) {
        for (size_t idx = 0; idx < candidates.size(); ++idx) {
            Block block(prev_block_hash, candidates[idx].transactions, difficulty);
            if (!block.mine(attempt_window_ms)) {
                continue;
            }

            unordered_set<string> mined_ids;
            mined_ids.reserve(candidates[idx].transactions.size());
            for (const auto& tx : candidates[idx].transactions) {
                mined_ids.insert(Block::toHex(tx.getId()));
            }

            auto new_end = remove_if(mempool.begin(), mempool.end(), [&](const Transaction& tx) {
                return mined_ids.count(Block::toHex(tx.getId())) > 0;
            });
            mempool.erase(new_end, mempool.end());

            balances = candidates[idx].balances_after;
            working_balances = balances;

            HashPointer hp(prev_block_hash, head);
            auto* new_node = new BlockNode(std::move(block), hp);
            head = new_node;
            block_count++;
            return;
        }

        attempt_window_ms += 5000;
    }
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

Blockchain::Candidate Blockchain::buildCandidate(vector<Transaction>& mempool_snapshot,
                                                 unordered_map<string, int64_t> balances_snapshot) {
    Candidate candidate;
    candidate.transactions.reserve(100);

    std::random_device rd;
    std::mt19937_64 rng(rd());

    while (!mempool_snapshot.empty() && candidate.transactions.size() < 100) {
        std::uniform_int_distribution<size_t> dist(0, mempool_snapshot.size() - 1);
        size_t index = dist(rng);
        Transaction tx = mempool_snapshot[index];
        mempool_snapshot[index] = mempool_snapshot.back();
        mempool_snapshot.pop_back();

        if (rehashTransaction(tx) != tx.getId()) {
            continue;
        }

        if (balances_snapshot[tx.getSender()] < tx.getAmount()) {
            continue;
        }

        balances_snapshot[tx.getSender()] -= tx.getAmount();
        balances_snapshot[tx.getReceiver()] += tx.getAmount();
        candidate.transactions.push_back(tx);
    }

    candidate.balances_after = std::move(balances_snapshot);
    return candidate;
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
        } else {
            rejected_txs.push_back(candidate_tx);
        }
        mempool.pop_back();
    }

    return selected;
}
