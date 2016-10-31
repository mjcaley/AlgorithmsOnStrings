#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;


class BWTransform
{
public:
    BWTransform(const string& text)
    {
        matrix = vector<vector<char>>(text.size(), vector<char>(text.size()));
        construct_matrix(text);
        sort_matrix();
    }
    
    vector<vector<char>>& get_matrix() { return matrix; };
    
private:
    vector<vector<char>> matrix;
    
    void construct_matrix(const string& text)
    {
        matrix[0].assign(text.begin(), text.end()); // populate first element
        
        for (int c { 1 }; c < text.size(); ++c)
        {
            auto& prev = matrix[c - 1];
            auto& next = matrix[c];
            next = prev;
            std::rotate(next.rbegin(), next.rbegin() + 1, next.rend());
        }
    }
    
    void sort_matrix()
    {
        std::sort(matrix.begin(), matrix.end());
    }
};


string BWT(const string& text) {
    string result = "";
    
    BWTransform bwt { text };
    
    auto& matrix = bwt.get_matrix();
    for (auto& row : matrix)
    {
        result += row.back();
    }
    
    return result;
}

int main() {
    string text;
    cin >> text;
    cout << BWT(text) << endl;
    return 0;
}
