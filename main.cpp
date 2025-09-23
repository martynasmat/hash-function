#include <iostream>
#include <iomanip>
#include <cstdint>
#include <vector>
#include <array>

using namespace std;

struct Cli {
    string fileName;
    string literalString;
};

struct Hasher {
    // Rotate functions
    // https://blog.regehr.org/archives/1063
    static inline uint8_t rotl(uint8_t n, unsigned int c)
    {
        const unsigned int mask = (CHAR_BIT*sizeof(n) - 1);
        c &= mask;
        return (n<<c) | (n>>( (-c)&mask ));
    }

    static inline uint8_t rotr(uint8_t n, unsigned int c)
    {
        const unsigned int mask = (CHAR_BIT*sizeof(n) - 1);
        c &= mask;
        return (n>>c) | (n<<( (-c)&mask ));
    }

    static vector<uint8_t> pad_input(const vector<uint8_t>& in) {
        vector<uint8_t> p = in;
        uint8_t len = p.size();
        uint8_t padZeros = 15 - (len % 16);
        p.insert(p.end(), padZeros, 0x00);
        p.push_back(static_cast<uint8_t>(len % 256));
        return p;
    }

    static array<uint8_t,8> hash(const vector<uint8_t>& input) {
        static const uint8_t first[] = { 0x20,0x03,0x01,0x08,0x20,0x03,0x01,0x08 };

        vector<uint8_t> msg = pad_input(input);

        uint8_t acc[8];
        memcpy(acc, first, sizeof(acc));

        // hash 8 bytes at a time until whole input hashed
        for (size_t i = 0; i < msg.size(); i += 8) {
            uint8_t next[8];
            // TODO: operation(acc, next, msg[i])
            memcpy(acc, next, sizeof(acc));
        }

        array<uint8_t,8> out{};
        memcpy(&out, acc, 8);
        return out;
    }
};

bool parse_cli_args(int argc, char** argv, Cli& cli) {
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--file") {
            if (i + 1 >= argc) {
                cerr << "(!) Error: --file requires a path\n";
                return false;
            }
            cli.fileName = argv[++i];
        } else if (arg == "--str") {
            if (i + 1 >= argc) {
                cerr << "(!) Error: --str requires a value\n";
                return false;
            }
            cli.literalString = argv[++i];
        } else {
            cerr << "(!) Error: unknown option " << arg << "\n";
            return false;
        }
    }
    if (cli.fileName.empty() && cli.literalString.empty()) {
        cerr << "(!) Error: provide --file or --str\n";
        return false;
    }
    if (!cli.fileName.empty() && !cli.literalString.empty()) {
        cerr << "(!) Note: both --file and --str given; using --file and ignoring --str\n";
    }
    return true;
}

int main(int argc, char** argv) {
    Cli cli;
    vector<uint8_t> inputBytes;

    if (!parse_cli_args(argc, argv, cli)) {
        return 1;
    }

    if (!cli.fileName.empty()) {
    } else {
        inputBytes.assign(cli.literalString.begin(), cli.literalString.end());
    }

    array<uint8_t, 8> h = Hasher::hash(inputBytes);
    for (auto b : h) cout << hex << setw(2) << setfill('0') << (int)b;
    cout << "\n";
    return 0;
}
