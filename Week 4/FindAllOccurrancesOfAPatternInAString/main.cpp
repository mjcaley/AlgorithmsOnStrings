#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::string;
using std::vector;

vector<int> compute_prefix_function(const string& pattern)
{
    vector<int> prefix(pattern.length());
    
    prefix[0] = 0;
    int border { 0 };
    
    for (int i { 1 }; i < pattern.length(); ++i)
    {
        while ( (border > 0) && (pattern[i] != pattern[border]) )
        {
            border = prefix[border - 1];
        }
        
        if (pattern[i] == pattern[border])
        {
            border = border + 1;
        }
        else
        {
            border = 0;
        }
        
        prefix[i] = border;
    }
    
    return prefix;
}

vector<int> find_all_occurances(const string& pattern, const string& text)
{
    auto S = pattern + '$' + text;
    auto s = compute_prefix_function(S);
    
    vector<int> result;
    
    for (int i { static_cast<int>(pattern.length() + 1) }; i < S.length(); ++i)
    {
        if (s[i] == pattern.length())
        {
            result.emplace_back(i - 2 * pattern.length());
        }
    }
    
    return result;
}

// Find all occurrences of the pattern in the text and return a
// vector with all positions in the text (starting from 0) where
// the pattern starts in the text.
vector<int> find_pattern(const string& pattern, const string& text) {
    vector<int> result;
    // Implement this function yourself
    
    auto prefix = compute_prefix_function(pattern);
    result = find_all_occurances(pattern, text);
    
    return result;
}

int main() {
    string pattern, text;
    cin >> pattern;
    cin >> text;
    vector<int> result = find_pattern(pattern, text);
    for (int i = 0; i < result.size(); ++i) {
        printf("%d ", result[i]);
    }
    printf("\n");
    return 0;
}
