#include <vector>
#include <array>

#include "Transaction.h"
#include "Block.h"

using namespace std;

int main() {
    Transaction tx1("testas_publickey_0001", "testas_publickey_0002", 50);
    Transaction tx2("testas_publickey_0002", "testas_publickey_0001", 20);
    Transaction tx3("testas_publickey_0003", "testas_publickey_0004", 5);

    vector<Transaction> transactions;
    transactions.push_back(tx1);
    transactions.push_back(tx2);
    transactions.push_back(tx3);

    array<uint8_t,16> prev_hash{};
    prev_hash.fill(0);

    Block genesis_block(prev_hash, transactions, "000");
    genesis_block.mine();

    return 0;
}
