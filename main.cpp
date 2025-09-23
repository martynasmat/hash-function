#include <iostream>
#include <iomanip>
#include <cstdint>
#include <vector>

using namespace std;

struct Cli {
    string fileName;
    string literalString;
};

struct Hasher {
    static vector<uint8_t> pad_input(const vector<uint8_t>& in) {
        vector<uint8_t> p = in;
        uint8_t len = p.size();
        uint8_t padZeros = 15 - (len % 16);
        p.insert(p.end(), padZeros, 0x00);
        p.push_back(static_cast<uint8_t>(len % 256));
        return p;
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

    vector<uint8_t> h = Hasher::pad_input(inputBytes);
    for (auto b : h) cout << hex << setw(2) << setfill('0') << (int)b;
    cout << "\n";
    return 0;
}
