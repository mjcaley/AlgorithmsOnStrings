#include <string>
#include <iostream>
#include <vector>
#include <map>

using std::map;
using std::vector;
using std::string;

typedef map<char, int> edges;
typedef vector<edges> trie;

trie build_trie(const vector<string> & patterns) {
    trie t(1);
    
    for (auto& pattern : patterns)
    {
        
        int current_node = 0;
        for (auto c = pattern.begin(); c != pattern.end(); ++c)
        {
            
            auto found_node = t[current_node].find(*c);
            
            if (found_node != t[current_node].end())
            {
                current_node = found_node->second;
            }
            else
            {
                t.emplace_back();
                t[current_node][*c] = t.size() - 1;
                current_node = t.size() - 1;
            }
        }
    }
    
    return t;
}

int main() {
    size_t n;
    std::cin >> n;
    vector<string> patterns;
    for (size_t i = 0; i < n; i++) {
        string s;
        std::cin >> s;
        patterns.push_back(s);
    }
    
    trie t = build_trie(patterns);
    for (size_t i = 0; i < t.size(); ++i) {
        for (const auto & j : t[i]) {
            std::cout << i << "->" << j.second << ":" << j.first << "\n";
        }
    }
    
    return 0;
}
