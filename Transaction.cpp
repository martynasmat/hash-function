#include "Transaction.h"
#include <vector>
#include <cstdint>
#include <string>

using namespace std;

Transaction::Transaction(const string& sender,
                         const string& receiver,
                         int64_t amount)
        : sender(sender),
          receiver(receiver),
          amount(amount)
{
    string data;
    data += this->sender;
    data += this->receiver;
    data += to_string(this->amount);

    vector<uint8_t> buffer;
    for (char c : data) {
        buffer.push_back(static_cast<uint8_t>(c));
    }
    transaction_id = hasher.hash(buffer);
}
