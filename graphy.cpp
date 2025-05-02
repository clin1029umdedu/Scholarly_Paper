#include <iostream>
#include <list>
#include <cmath>
#include <vector>
#include <algorithm>
#include <random>
#include <unordered_set>
#include <fstream>
#include <string>
#include <sstream>
#include <bitset>

using namespace std;

const uint32_t n = 10000;

unordered_set<uint32_t> intersection(
    const unordered_set<uint32_t>& a,
    const unordered_set<uint32_t>& b
) {
    unordered_set<uint32_t> result;
    for (uint32_t x : a) {
        if (b.count(x)) {
            result.insert(x);
        }
    }
    return result;
}

template <typename T>
string to_string(const unordered_set<T>& s) {
    ostringstream oss;
    oss << "{";
    bool first = true;
    for (const auto& elem : s) {
        if (!first) oss << ", ";
        oss << elem;
        first = false;
    }
    oss << "}";
    return oss.str();
}

vector<bitset<n>> _makeGraph(double p) {
    vector<bitset<n>> G(n);
    vector<uint32_t> connect(n);
    for (uint32_t i = 0; i < n; ++i)
        connect[i] = i;

    random_device rd;
    mt19937 rng(rd());
    shuffle(connect.begin(), connect.end(), rng);

    vector<uint32_t> connected;
    connected.push_back(connect[0]);

    for (uint32_t i = 1; i < n; ++i) {
        uniform_int_distribution<uint32_t> dist(0, i - 1);
        uint32_t rand_index = dist(rng);

        uint32_t u = connect[i];
        uint32_t v = connected[rand_index];

        G[u][v] = 1;
        G[v][u] = 1;

        connected.push_back(u);
    }

    uint64_t edgeMax = static_cast<uint64_t>(p * (n * (n - 1) / 2));
    uniform_int_distribution<int> dist(0, n - 1);

    for (uint64_t i = 0; i < (edgeMax - n); ++i) {
        int u = dist(rng);
        int v = dist(rng);
        if (u == v || G[u][v]) continue;

        // Find common neighbors with a bitwise AND
        bitset<n> common = G[u] & G[v];

        // Check if any two neighbors in `common` are connected (4-clique check)
        bool valid = true;
        for (int a = 0; a < n && valid; ++a) {
            if (!common[a]) continue;
            for (int b = a + 1; b < n; ++b) {
                if (common[b] && G[a][b]) {
                    valid = false;
                    break;
                }
            }
        }

        if (valid) {
            G[u][v] = 1;
            G[v][u] = 1;
        }
    }

    return G;
}

vector<unordered_set<uint32_t>> makeGraph(uint32_t n, double p) {
    vector<unordered_set<uint32_t>> G(n, unordered_set<uint32_t>());
    vector<uint32_t> connect(n);
    for (int i = 0; i < n; i++)
        connect[i] = i;
    random_device rd;
    mt19937 rng(rd());
    shuffle(connect.begin(), connect.end(), rng);
    vector<uint32_t> connected;
    connected.push_back(connect[0]);

    for (int i = 1; i < n; i ++) {
        uniform_int_distribution<int> dist(0, i);
        uint32_t rand_index = dist(rng);

        uint32_t u = connect[i];
        uint32_t v = connected[rand_index];

        G[u].insert(v);
        G[v].insert(u);

        connected.push_back(u);
    }

    uint64_t edgeMax = static_cast<uint64_t>(p * (n * (n - 1) / 2));
    uniform_int_distribution<int> dist(0, n - 1);
    for (int i = 0; i < (edgeMax - n); i++) {
        int u = dist(rng);
        int v = dist(rng);
        if (u == v || G[u].count(v)) continue;

        const auto& small = (G[u].size() < G[v].size()) ? G[u] : G[v];
        const auto& large = (G[u].size() < G[v].size()) ? G[v] : G[u];

        vector<uint32_t> common;
        for (uint32_t x : small) {
            if (large.count(x)) {
                common.push_back(x);
            }
        }

        bool flag = true;
        // Step 2: Iterate through all unordered pairs (a, b) from common
        for (uint32_t i = 0; i < common.size() && flag; ++i) {
            uint32_t a = common[i];
            for (uint32_t j = i + 1; j < common.size(); ++j) {
                uint32_t b = common[j];
                if (G[a].count(b)) {
                    flag = false;
                    break;
                }
            }
        }
        if (flag) {
            G[u].insert(v);
            G[v].insert(u);
        }
    }

    return G;
}



int main(int argc, char* argv[]) {
    // Program should accept 1 parameters p
    if (argc != 2) {
        cerr << "Wrong Parameter Count" << endl;
        return 1;
    }

    double p = stod(argv[1]);

    vector<bitset<n>> G = _makeGraph(p);

    return 0;
}