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

int main() {
    // Generate users
    vector<User> users = generateUsers(USER_COUNT);

    // Generate transactions
    vector<Transaction> transactions = generateTransactions(
            users,
            TRANSACTION_COUNT
            );

    // genesis block
    Transaction tx1("test1", "test2", 50);
    Transaction tx2("test1", "test3", 20);
    Transaction tx3("test4", "test2", 5);
    vector<Transaction> genesis_block_txs = { tx1, tx2, tx3 };

    Transaction tx4("test6", "test4", 12);
    Transaction tx5("test3", "test6", 7);
    vector<Transaction> new_block_txs = { tx4, tx5 };

    Blockchain blockchain("000");
    blockchain.genGenesisBlock(genesis_block_txs);
    blockchain.genBlock(new_block_txs);

    blockchain.printChain();

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
    mt19937_64 rng(251029);
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