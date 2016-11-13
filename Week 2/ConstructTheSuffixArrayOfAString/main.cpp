#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <vector>
#include <utility>

using std::cin;
using std::cout;
using std::endl;
using std::make_pair;
using std::pair;
using std::string;
using std::vector;

template<typename Iterator>
struct NaiveSuffix
{
    NaiveSuffix(int num, Iterator index) : num(num), index(index) {};
    
    int num;
    Iterator index;
};

template<typename Iterator>
bool suffix_compare(const NaiveSuffix<Iterator>& lhs, const NaiveSuffix<Iterator>& rhs, const Iterator end)
{
    auto lhs_iter { lhs.index };
    auto rhs_iter { rhs.index };
    
    while (lhs_iter != end && rhs_iter != end)
    {
        if (*lhs_iter == *rhs_iter)
        {
            ++lhs_iter;
            ++rhs_iter;
        }
        else
        {
            return *lhs_iter < *rhs_iter;
        }
    }
    
    return false;
}


vector<int> naive(const string& text)
{
    using namespace std::placeholders;
    
    vector<int> result;
    
    vector<NaiveSuffix<string::const_iterator>> suffix_array;
    
    int num { 0 };
    for (auto iter { text.begin() }; iter != text.end(); ++iter)
    {
        suffix_array.emplace_back(num++, iter);
    }
    
    std::sort(suffix_array.begin(),
              suffix_array.end(),
              std::bind(suffix_compare<string::const_iterator>, _1, _2, text.end())
              );
    
    for (auto& suffix : suffix_array)
    {
        result.emplace_back(suffix.num);
    }
    
    return result;
}









static int NO_CHILD { -1 };

static std::map<char, int> EMPTY_CHILDREN
{
    { 'A', NO_CHILD },
    { 'C', NO_CHILD },
    { 'G', NO_CHILD },
    { 'T', NO_CHILD },
    { '$', NO_CHILD }
};

struct Suffix
{
    Suffix() {};
    Suffix(int b) : begin(b) {};
    Suffix(int b, int e) : begin(b), end(e) {};
    Suffix(const Suffix&) = default;
    Suffix(Suffix&) = default;
    
    int begin { -1 };
    int end { begin };
    bool leaf { false };
    int suffix;
    
    std::map<char, int> children { EMPTY_CHILDREN };
};
using SuffixTree = vector<Suffix>;


string print(int begin, int end, const string& text)
{
    string out;
    
    for (int index { begin }; index < end && index < text.size(); ++index)
    {
        out += text[index];
    }
    
    return out;
}

string print(const Suffix& node, const string& text)
{
    string out;
    
    out += print(node.begin, node.end, text);
    
    return out;
}

string print(const SuffixTree& tree, const string& text)
{
    string out;
    
    for (auto& node : tree)
    {
        out += print(node, text);
        out += '\n';
    }
    
    return out;
}

int get_child_index(SuffixTree& tree, const int node, const string& text, int index)
{
    int child { -1 };
    
    try
    {
        child = tree[node].children.at( text[index] );
    }
    catch (std::out_of_range)
    {
        return -1;
    }
    
    return child;
}

Suffix write_node(bool leaf, int begin, int end, int suffix = -1, std::map<char, int> children = EMPTY_CHILDREN)
{
    Suffix node;
    node.leaf = leaf;
    node.begin = begin;
    node.end = end;
    node.suffix = suffix;
    node.children = children;
    node.children.insert(EMPTY_CHILDREN.begin(), EMPTY_CHILDREN.end());
    
    return node;
}

void add_suffix(SuffixTree& tree, int node, const string& text, int begin, int end, int suffix);

void branch(SuffixTree& tree, int node, const string& text, int begin, int end, int suffix)
{
    //    std::cout << "branch " << begin << ' ' << end << std::endl;
    
    auto node_index { tree[node].begin };
    auto node_end { tree[node].end  };
    
    for (; node_index < node_end; ++node_index, ++begin)
    {
        if (text[begin] != text[ node_index ])
        {
            break;
        }
    }
    
    if (node_index == node_end)
    {
        auto child_node = get_child_index(tree, node, text, begin);
        if (child_node == -1)
        {
            tree.emplace_back( write_node(true, begin, end, suffix) );
            tree[node].children[ text[begin] ] = tree.size() - 1;
        }
        else
        {
            branch(tree, child_node, text, begin, end, suffix);
        }
    }
    else
    {
        // Split current node
        
        // Make a new node and inherit the current children
        tree.emplace_back( write_node(true, node_index, tree[node].end, tree[node].suffix, tree[node].children) );
        // Make sure the old node points to the new child and isn't a leaf anymore
        tree[node] = write_node(false, tree[node].begin, node_index, -1, {{ text[node_index], tree.size() - 1 }});
        tree[node].children[ text[node_index] ] = tree.size() - 1;
        
        // Create new node
        tree.emplace_back( write_node(true, begin, end, suffix) );
        tree[node].children[ text[begin] ] = tree.size() - 1;
    }
}

void add_suffix(SuffixTree& tree, int node, const string& text, int begin, int end, int suffix)
{
    //    std::cout << "add " << begin << ' ' << end << std::endl;
    
    auto child_index = get_child_index(tree, node, text, begin);
    if (child_index == -1)
    {
        // no children
        tree.emplace_back( write_node(true, begin, end, suffix) );
        tree[node].children[text[begin]] = tree.size() - 1;
    }
    else
    {
        branch(tree, child_index, text, begin, end, suffix);
    }
}

SuffixTree construct_tree(const string& text)
{
    SuffixTree tree;
    tree.emplace_back(text.size());
    
    int root { 0 };
    
    auto iter = text.end() - 1;
    auto begin = text.begin();
    auto end = text.end();
    for (; iter >= begin; --iter)
    {
        add_suffix(tree, root, text, std::distance(text.begin(), iter), std::distance(text.begin(), text.end()), std::distance(text.begin(), iter));
    }
    
    return tree;
}


vector<int> dfs(SuffixTree& tree, int node)
{
    vector<int> suffix;
    
    for (auto& child : tree[node].children)
    {
        if (child.second != -1)
        {
            auto child_suffix = dfs(tree, child.second);
            suffix.insert(suffix.end(), child_suffix.begin(), child_suffix.end());
        }
    }
    
    if (tree[node].leaf && tree[node].suffix != -1)
    {
        suffix.emplace_back(tree[node].suffix);
    }
    
    return suffix;
}

std::ostream& operator<<(std::ostream& out, const std::vector<int>& in)
{
    if (in.empty()) { return out; }
    
    out << in.front();
    for (auto iter { in.begin() + 1 }; iter != in.end(); ++iter)
    {
        out << ", " << *iter;
    }
    
    return out;
}


template<typename Engine>
std::string random_data(Engine& gen, int length)
{
    std::string ret;
    
    std::uniform_int_distribution<char> uniform_dist(0, 3);
    vector<char> letters { 'A', 'C', 'G', 'T' };
    
    for (int i { 0 }; i < length; ++i)
    {
        ret += letters[uniform_dist(gen)];
    }
    
    ret += '$';
    
    return ret;
}


void rand_test()
{
    int test_num { 0 };
    std::default_random_engine gen(1);
    
    while (true)
    {
        auto rand_string = random_data(gen, 99999);
        
        std::cout << "Test " << test_num++ << " for " << rand_string << std::endl;
        
        auto n_result = naive(rand_string);
        
        auto tree = construct_tree(rand_string);
        auto f_result = dfs(tree, 0);
        
        if (n_result != f_result)
        {
            std::cout << "Failure on " << rand_string << std::endl;
            std::cout << "Naive " << n_result << std::endl;
            std::cout << "Fast " << f_result << std::endl;
            break;
        }
    }
}

// Build suffix array of the string text and
// return a vector result of the same length as the text
// such that the value result[i] is the index (0-based)
// in text where the i-th lexicographically smallest
// suffix of text starts.
vector<int> BuildSuffixArray(const string& text) {
    auto tree = construct_tree(text);
    auto result = dfs(tree, 0);
    
    rand_test();
    
    return result;
}

int main() {
    string text;
    cin >> text;
    vector<int> suffix_array = BuildSuffixArray(text);
    for (int i = 0; i < suffix_array.size(); ++i) {
        cout << suffix_array[i] << ' ';
    }
    cout << endl;
    return 0;
}
