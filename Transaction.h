#include <string>
#include <array>

using namespace std;

class Transaction {
private:
    string sender;
    string receiver;
    int64_t amount;
    array<uint8_t, 16> transaction_id;

public:
    Transaction(const string& sender,
                const string& receiver,
                int64_t amount);

    const string& getSender() const {
        return sender;
    }

    const string& getReceiver() const {
        return receiver;
    }

    int64_t getAmount() const {
        return amount;
    }

    const array<uint8_t,16>& getId() const {
        return transaction_id;
    }
};
