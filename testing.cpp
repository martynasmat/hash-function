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

string to_hex(const array<uint8_t,8>& h){
    static const char* H="0123456789abcdef";
    string s; s.resize(16);
    for(int i=0;i<8;++i){ s[2*i]=H[h[i]>>4]; s[2*i+1]=H[h[i]&0xF]; }
    return s;
}

double hamming_distance_bits(const std::array<uint8_t,8>& a, const std::array<uint8_t,8>& b) {
    int d = 0;
    for (int i = 0; i < 8; i++) {
        auto x = uint8_t(a[i] ^ b[i]);
        for (int bit = 0; bit < 8; ++bit) d += (x >> bit) & 1; // popcount(x)
    }
    return 100.0 * double(d) / 64.0;
}

double hamming_distance_hex(const std::array<uint8_t,8>& a, const std::array<uint8_t,8>& b) {
    std::string ha = to_hex(a), hb = to_hex(b);
    int d = 0;
    for (int i = 0; i < 16; ++i) if (ha[i] != hb[i]) ++d;
    return 100.0 * double(d) / 16.0;
}

string read_text(const string& path){
    ifstream in(path, ios::binary);
    ostringstream ss; ss<<in.rdbuf(); return ss.str();
}
vector<string> read_lines(const string& path){
    ifstream in(path);
    vector<string> v; string line; while(getline(in,line)) v.push_back(line); return v;
}
bool split_pair_first_space(const string& line, string& a, string& b){
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

    // 3) kolizijos
    cout << "\n# kolizijos\n";
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
    cout << "\n# lavinos efektas\n";
    double sum_bits = 0, sum_hex = 0, min_bits=INT_MAX, max_bits=INT_MIN, min_hex=INT_MAX, max_hex=INT_MIN;
    int pairs=0;
    ifstream in(DIR+"avalanche.txt");
    string line,a,b;
    while(getline(in,line)){
        if(!split_pair_first_space(line,a,b)) continue;
        pairs++;
        vector<uint8_t> v1(a.begin(),a.end()), v2(b.begin(),b.end());
        auto h1=Hasher::hash(v1), h2=Hasher::hash(v2);
        double dist = hamming_distance_bits(h1, h2);

        if(dist == 0) cout << a << "COLL" << b << "\n\n\n\n";

        sum_bits += dist;
        if(dist < min_bits) min_bits = dist;
        if(dist > max_bits) max_bits = dist;

        dist = hamming_distance_hex(h1, h2);
        sum_hex += dist;
        if(dist < min_hex) min_hex = dist;
        if(dist > max_hex) max_hex = dist;
    }
    cout << "  hamming distance / bits: avg_percentage="<<sum_bits / double(pairs)<<"%  min="<<min_bits<<"%  max="<<max_bits<<"%\n";
    cout << "  hamming distance / hex: avg_percentage="<<sum_hex / double(pairs)<<"%  min="<<min_hex<<"%  max="<<max_hex<<"%\n";


    // 5) salt
    cout << "\n# negriztamumas\n";
    vector<uint8_t> input{'i','n','p','u','t'};
    vector<uint8_t> salt1{'i','n','p','u','t', '1','2','3'};
    vector<uint8_t> salt2{'i','n','p','u','t', '4','5','6'};
    cout << "  input=" << to_hex(Hasher::hash(input)) << endl;
    cout << "  input + salt1="<< to_hex(Hasher::hash(salt1)) << endl;
    cout << "  input + salt2="<< to_hex(Hasher::hash(salt2)) << endl;
    return 0;
}
