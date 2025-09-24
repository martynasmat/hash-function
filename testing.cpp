// g++ -O2 -std=c++17 testing_simple.cpp -o tests
// ./tests

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <array>
#include <chrono>
#include <cstdint>
#include "Hasher.hpp"

using namespace std;

static string to_hex(const array<uint8_t,8>& h){
    static const char* H="0123456789abcdef";
    string s; s.resize(16);
    for(int i=0;i<8;++i){ s[2*i]=H[h[i]>>4]; s[2*i+1]=H[h[i]&0xF]; }
    return s;
}

static string read_text(const string& path){
    ifstream in(path, ios::binary);
    ostringstream ss; ss<<in.rdbuf(); return ss.str();
}
static vector<string> read_lines(const string& path){
    ifstream in(path);
    vector<string> v; string line; while(getline(in,line)) v.push_back(line); return v;
}
static bool split_pair_first_space(const string& line, string& a, string& b){
    size_t p=line.find(' '); if(p==string::npos) return false;
    a=line.substr(0,p); b=line.substr(p+1); return true;
}

int main(){
    const string DIR = "test_files/"; // made by filegen.py

    // 1) išvedimo dydis ir deterministiskumas
    cout << "# isvedimo dydis ir deterministiskumas\n";
    vector<string> files1;
    for(int i=1;i<=10;++i) {
        files1.push_back(DIR+"test_1char_"+to_string(i)+".txt");
    }
    for(int i=1;i<=10;++i){
        files1.push_back(DIR+"test_1000chars_"+to_string(i)+".txt");
    }
    for(int i=1;i<=10;++i){
        files1.push_back(DIR+"test_1000chars_chardiff_"+to_string(i)+".txt");
    }

    for(size_t idx=0; idx<files1.size(); ++idx){
        string t = read_text(files1[idx]);
        vector<uint8_t> v(t.begin(), t.end());
        auto h1 = Hasher::hash(v);
        auto h2 = Hasher::hash(v);
        cout << files1[idx] << "  len=" << v.size()
             << "  hash_len=" << sizeof(h1)
             << "  hash=" << to_hex(h1) << " vienodi=" << boolalpha << (h1==h2) << "\n";
    }

    cout << "\n# efektyvumas\n";
    vector<string> lines = read_lines(DIR+"konstitucija.txt");
    vector<int> counts;
    for (int n = 1; n <= (int)lines.size(); n *= 2) counts.push_back(n);

    const int runs = 10;
    vector<double> avgs;
    avgs.reserve(counts.size());

    for (int n : counts) {
        string text; text.reserve(n * 32);
        for (int i = 0; i < n; ++i) { text += lines[i]; text.push_back('\n'); }

        vector<uint8_t> v(text.begin(), text.end());
        for (int w = 0; w < 3; ++w) (void)Hasher::hash(v);

        long long total_ns = 0;
        for (int r = 0; r < runs; ++r) {
            auto t0 = chrono::high_resolution_clock::now();
            auto h  = Hasher::hash(v);
            auto t1 = chrono::high_resolution_clock::now();
            total_ns += chrono::duration_cast<chrono::nanoseconds>(t1 - t0).count();
            (void)h;
        }
        double ms = (total_ns / 1e6) / runs;
        avgs.push_back(ms);
        cout << "  eiluciu_kiekis=" << n << "  avg_ms=" << fixed << setprecision(3) << ms << "\n";
    }

    // 3) collisions
    cout << "\n# collisions\n";
    for(int L : {10,100,500,1000}){
        string path = DIR + ("collision"+to_string(L)+".txt");
        ifstream in(path);
        size_t pairs=0, coll=0; string line,a,b;
        while(getline(in,line)){
            if(!split_pair_first_space(line,a,b)) continue;
            vector<uint8_t> v1(a.begin(),a.end()), v2(b.begin(),b.end());
            auto h1=Hasher::hash(v1), h2=Hasher::hash(v2);
            if(h1==h2) ++coll; ++pairs;
        }
        cout << "  len="<<L<<"  pairs="<<pairs<<"  collisions="<<coll<<"\n";
    }

    // 4) avalanche effect
    return 0;
}
