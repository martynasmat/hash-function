#include <iostream>
#include <vector>
#include <cstdint>
#include <array>
#include <random>

#include "./transaction/Transaction.h"
#include "./blockchain/Blockchain.h"
#include "./user/User.h"

using namespace std;

uint64_t USER_COUNT = 1000;
uint64_t TRANSACTION_COUNT = 10000;

vector<User> generateUsers(size_t count);
vector<Transaction> generateTransactions(vector<User>& u, size_t count);
void printBlock(uint64_t index, bool with_txs, const Block* b);
void runCli(const Blockchain& chain);

int main() {
    // Generate users
    vector<User> users = generateUsers(USER_COUNT);

    // Generate transactions
    vector<Transaction> transactions = generateTransactions(
            users,
            TRANSACTION_COUNT
            );

    Blockchain blockchain("000");
    blockchain.loadIntoMempool(transactions);
    blockchain.setBalances(users);

    while (!blockchain.isMempoolEmpty()) {
        blockchain.mineNextBlock();
    }

    blockchain.printChain();

    cout << blockchain.getRejected().size() << " transactions rejected" << endl;

    runCli(blockchain);

    return 0;
}

vector<User> generateUsers(size_t count) {
    vector<User> users;

    mt19937_64 rng(251029);
    uniform_int_distribution<int64_t> gen(100, 1000000);

    for (size_t i = 0; i < count; i++) {
        User u;
        u.name = "user-" + to_string(i);
        u.public_key = "public_key_" + to_string(i);
        u.balance = gen(rng);
        users.push_back(u);
    }

    return users;
}

vector<Transaction> generateTransactions(vector<User>& u, size_t count) {
    vector<Transaction> txs;
    mt19937_64 rng(259);
    uniform_int_distribution<size_t> userDist(0, u.size() - 1);
    uniform_int_distribution<int64_t> amtDist(100, 100000);

    for (uint64_t i = 0; i < count; i++) {
        User snd = u[userDist(rng)];
        User rcv = u[userDist(rng)];

        while (snd.public_key == rcv.public_key) {
            rcv = u[userDist(rng)];
        };

        Transaction tx(
                snd.public_key,
               rcv.public_key,
               amtDist(rng)
                       );
        txs.push_back(tx);
    }

    return txs;
}

void runCli(const Blockchain& chain) {
    cout << "\nCommands:\n"
            "  count             -> show number of blocks\n"
            "  tip               -> show the latest block (with txs)\n"
            "  show N            -> show block N (0 = genesis)\n"
            "  showfull N          -> show block N with transactions\n"
            "  help              -> show this help\n"
            "  quit              -> exit\n\n";

    string line;
    while (true) {
        cout << "> ";
        if (!getline(cin, line)) break;

        if (line == "quit" || line == "exit") {
            break;
        } else if (line == "help") {
            cout << "count | tip | show N | showfull N | help | quit\n";
        } else if (line == "count") {
            cout << "Blocks: " << chain.getCount() << "\n";
        } else if (line == "tip") {
            if (chain.getCount() == 0) { cout << "Chain is empty.\n"; continue; }
            printBlock(chain.getCount() - 1, true, chain.getBlockByIndex(chain.getCount() - 1));
        } else if (line.rfind("showfull ", 0) == 0) {
            try {
                size_t idx = stoull(line.substr(9));
                printBlock(idx, true, chain.getBlockByIndex(idx));
            } catch (...) { cout << "Usage: showfull N\n"; }
        } else if (line.rfind("show ", 0) == 0) {
            try {
                size_t idx = stoull(line.substr(5));
                printBlock(idx, false, chain.getBlockByIndex(idx));
            } catch (...) { cout << "Usage: show N\n"; }
        } else if (line.empty()) {
            continue;
        } else {
            cout << "Unrecognized command. Type 'help'.\n";
        }
    }
}

void printBlock(uint64_t index, bool with_txs, const Block* b) {
    if (b == nullptr) {
        return;
    }

    std::cout << "Block " << index << "\n";
    std::cout << "  Hash:        " << Block::toHex(b->getHash()) << "\n";
    std::cout << "  Prev Hash:   " << Block::toHex(b->getPrevHash()) << "\n";
    std::cout << "  Version:     " << b->getVersion() << "\n";
    std::cout << "  Timestamp:   " << b->getTimestamp() << "\n";
    std::cout << "  Nonce:       " << b->getNonce() << "\n";
    std::cout << "  Tx count:    " << b->getTransactions().size() << "\n";

    if (with_txs) {
        std::cout << "  Transactions:\n";
        for (const auto& tx : b->getTransactions()) {
            std::cout << "    " << tx.getSender() << " -> " << tx.getReceiver()
                      << " : " << tx.getAmount()
                      << " | tx_id=" << Block::toHex(tx.getId()) << "\n";
        }
    }
}