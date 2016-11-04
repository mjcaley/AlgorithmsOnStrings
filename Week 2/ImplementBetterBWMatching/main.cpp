#include <algorithm>
#include <cstdio>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using std::cin;
using std::istringstream;
using std::map;
using std::string;
using std::vector;

// Preprocess the Burrows-Wheeler Transform bwt of some text
// and compute as a result:
//   * starts - for each character C in bwt, starts[C] is the first position
//       of this character in the sorted array of
//       all characters of the text.
//   * occ_count_before - for each character C in bwt and each position P in bwt,
//       occ_count_before[C][P] is the number of occurrences of character C in bwt
//       from position 0 to position P inclusive.
void PreprocessBWT(const string& bwt,
                   map<char, int>& starts,
                   map<char, vector<int> >& occ_count_before) {
    
    for (int i { 0 }; i < bwt.length(); ++i)
    {
        auto counts_iter { occ_count_before.begin() };
        for (; counts_iter != occ_count_before.end(); ++counts_iter)
        {
            counts_iter->second[i + 1] = counts_iter->second[i];
        }
        
        counts_iter = occ_count_before.find(bwt[i]);
        if (counts_iter == occ_count_before.end())
        {
            occ_count_before[ bwt[i] ] = vector<int>( bwt.length() + 1, 0);
        }
        ++occ_count_before[bwt[i]][i + 1];
    }
    
    int num { 0 };
    for (auto iter { occ_count_before.begin() }; iter != occ_count_before.end(); ++iter)
    {
        starts[iter->first] = num;
        num += iter->second[iter->second.size() - 1];
    }
}

// Compute the number of occurrences of string pattern in the text
// given only Burrows-Wheeler Transform bwt of the text and additional
// information we get from the preprocessing stage - starts and occ_counts_before.
int CountOccurrences(const string& pattern,
                     const string& bwt,
                     const map<char, int>& starts,
                     const map<char, vector<int> >& occ_count_before) {
    
    int top { 0 };
    int bottom { static_cast<int>(bwt.length() - 1) };
    
    int pattern_range { static_cast<int>(pattern.length() - 1) };
    while (top <= bottom)
    {
        if (pattern_range >= 0)
        {
            auto symbol = pattern[pattern_range];
            --pattern_range;
            try
            {
                top = starts.at(symbol) + occ_count_before.at(symbol)[top];
                bottom = starts.at(symbol) + occ_count_before.at(symbol)[bottom + 1] - 1;
            }
            catch (std::out_of_range)
            {
                // symbol not in BWT
                break;
            }
        }
        else
        {
            return bottom - top + 1;
        }
    }
    
    return 0;
}


int main() {
    string bwt;
    cin >> bwt;
    int pattern_count;
    cin >> pattern_count;
    // Start of each character in the sorted list of characters of bwt,
    // see the description in the comment about function PreprocessBWT
    map<char, int> starts;
    // Occurrence counts for each character and each position in bwt,
    // see the description in the comment about function PreprocessBWT
    map<char, vector<int> > occ_count_before;
    // Preprocess the BWT once to get starts and occ_count_before.
    // For each pattern, we will then use these precomputed values and
    // spend only O(|pattern|) to find all occurrences of the pattern
    // in the text instead of O(|pattern| + |text|).
    PreprocessBWT(bwt, starts, occ_count_before);
    for (int pi = 0; pi < pattern_count; ++pi) {
        string pattern;
        cin >> pattern;
        int occ_count = CountOccurrences(pattern, bwt, starts, occ_count_before);
        printf("%d ", occ_count);
    }
    printf("\n");
    return 0;
}
