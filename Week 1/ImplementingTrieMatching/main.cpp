#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

int const NA      =   -1;

struct Node
{
    int A { NA };
    int C { NA };
    int G { NA };
    int T { NA };
    
    int get(const char symbol) const
    {
        switch (symbol)
        {
            case 'A':
                return A;
                break;
            case 'C':
                return C;
                break;
            case 'G':
                return G;
                break;
            case 'T':
                return T;
                break;
            default:
                return NA;
        }
    }
    
    void set(const char symbol, const int destination)
    {
        switch (symbol)
        {
            case 'A':
                A = destination;
                break;
            case 'C':
                C = destination;
                break;
            case 'G':
                G = destination;
                break;
            case 'T':
                T = destination;
                break;
        }
    }
    
    bool is_leaf() const
    {
        return (A == NA &&
                C == NA &&
                G == NA &&
                T == NA);
    }
};

using trie = vector<Node>;

trie build_trie(const vector<string> & patterns) {
    trie t(1);
    
    for (auto& pattern : patterns)
    {
        int current_node = 0;
        for (auto c = pattern.begin(); c != pattern.end(); ++c)
        {
            if ( t[current_node].get(*c) != NA )
            {
                current_node = t[current_node].get(*c);
            }
            else
            {
                t.emplace_back();
                int dest_node = static_cast<int>(t.size() - 1);
                t[current_node].set(*c, dest_node);
                current_node = dest_node;
            }
        }
    }
    
    return t;
}

bool prefix_trie_matching(const std::string& text, const trie& t)
{
    auto symbol { text.begin() };
    int v { 0 }; // root of trie
    
    while (true)
    {
        if (t[v].is_leaf())
        {
            return true;
        }
        else if (t[v].get(*symbol) != NA)
        {
            v = t[v].get(*symbol);
            ++symbol;
        }
        else
        {
            return false;
        }
    }
}

vector<int> trie_matching(const std::string& text, const trie& t)
{
    vector<int> result;
    
    for (int i { 0 }; i < text.length(); ++i)
    {
        std::string substring(text.begin()+i, text.end());
        auto pattern_match = prefix_trie_matching(substring, t);
        if (pattern_match)
        {
            result.emplace_back(i);
        }
    }
    
    return result;
}

vector <int> solve (const string& text, int n, const vector <string>& patterns)
{
    vector <int> result;
    
    auto trie = build_trie(patterns);
    result = trie_matching(text, trie);
    
    return result;
}

int main (void)
{
    string t;
    cin >> t;
    
    int n;
    cin >> n;
    
    vector <string> patterns (n);
    for (int i = 0; i < n; i++)
    {
        cin >> patterns[i];
    }
    
    vector <int> ans;
    ans = solve (t, n, patterns);
    
    for (int i = 0; i < (int) ans.size (); i++)
    {
        cout << ans[i];
        if (i + 1 < (int) ans.size ())
        {
            cout << " ";
        }
        else
        {
            cout << endl;
        }
    }
    
    return 0;
}
