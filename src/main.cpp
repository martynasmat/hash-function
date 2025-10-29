#include <iostream>
#include <vector>
#include <cstdint>
#include <array>

#include "./transaction/Transaction.h"
#include "./blockchain/Blockchain.h"

using namespace std;

int main() {
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
