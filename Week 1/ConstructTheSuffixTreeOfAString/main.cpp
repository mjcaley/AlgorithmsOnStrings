#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::make_shared;
using std::string;
using std::shared_ptr;
using std::vector;


struct Suffix
{
    Suffix(int b) : begin(b) {};
    Suffix(int b, int e) : begin(b), end(e) {};
    
    int begin;
    int end { begin };
    
    vector<int> children;
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

int get_child_index(const SuffixTree& tree, const int node, const string& text, int index)
{
    for (auto tree_index : tree[node].children)
    {
        auto& child = tree[tree_index];
        
        if (text[child.begin] == text[index])
        {
            return tree_index;
        }
    }
    
    return -1;
}

void add_suffix(SuffixTree& tree, int node, const string& text, int begin, int end);

void branch(SuffixTree& tree, int node, const string& text, int begin, int end)
{
    if (tree[node].begin == begin && tree[node].end == end)  // might not need this
    {
        return;
    }
    
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
            add_suffix(tree, node, text, begin, end);
        }
        else
        {
            branch(tree, child_node, text, begin, end);
        }
    }
    else
    {
        // Split current node
        tree.emplace_back(node_index, tree[node].end);
        std::swap(tree[node].children, tree[tree.size() - 1].children); // swap children
        tree[node].children.emplace_back(tree.size() - 1);
        tree[node].end = node_index;
        
        // Create new node
        add_suffix(tree, node, text, begin, end);
    }
}

void add_suffix(SuffixTree& tree, int node, const string& text, int begin, int end)
{
    auto child_index = get_child_index(tree, node, text, begin);
    if (child_index == -1)
    {
        // no children
        tree.emplace_back(begin, end);
        tree[node].children.emplace_back(tree.size() - 1);
    }
    else
    {
        branch(tree, child_index, text, begin, end);
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
        add_suffix(tree, root, text, std::distance(text.begin(), iter), std::distance(text.begin(), text.end()));
    }
    
    return tree;
}

// Build a suffix tree of the string text and return a vector
// with all of the labels of its edges (the corresponding
// substrings of the text) in any order.
vector<string> ComputeSuffixTreeEdges(const string& text) {
    vector<string> result;
    
    auto tree = construct_tree(text);
    
    for (auto& node : tree)
    {
        if (node.begin == node.end)
        {
            continue;
        }
        
        result.emplace_back(print(node, text));
    }
    
    return result;
}

int main() {
    string text;
    cin >> text;
    vector<string> edges = ComputeSuffixTreeEdges(text);
    for (int i = 0; i < edges.size(); ++i) {
        cout << edges[i] << endl;
    }
    return 0;
}
