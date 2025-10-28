#include <string>
#include <array>

using namespace std;

class Transaction {
    string sender;
    string receiver;
    int64_t amount;
    array<uint8_t, 16> transaction_id;
};
