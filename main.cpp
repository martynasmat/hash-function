#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <array>
#include <random>
#include "Hasher.hpp"

using namespace std;

struct Cli {
    string fileName;
    string literalString;
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

static string read_file(const string& path){
    ifstream in(path, ios::binary);
    ostringstream ss;
    ss<<in.rdbuf();
    return ss.str();
}

int main(int argc, char** argv) {
    Cli cli;
    vector<uint8_t> inputBytes;

    if (!parse_cli_args(argc, argv, cli)) {
        return 1;
    }

    if (!cli.fileName.empty()) {
        string data = read_file(cli.fileName);
        inputBytes.assign(data.begin(), data.end());
    } else {
        inputBytes.assign(cli.literalString.begin(), cli.literalString.end());
    }

    array<uint8_t, 8> h = Hasher::hash(inputBytes);
    for (auto b : h) cout << hex << setw(2) << setfill('0') << (int)b;
    cout << "\n";
    return 0;
}
