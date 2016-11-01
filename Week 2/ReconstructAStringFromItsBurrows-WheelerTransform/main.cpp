#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;


class Counter
{
public:
    int add_char(char c)
    {
        return ++counter[c];
    };
    
    int get_count(char c) const
    {
        try
        {
            return counter.at(c);
        }
        catch (std::out_of_range)
        {
            return 0;
        }
    };
    
    int count_up_to(char c, int occurance) const
    {
        int total { 0 };
        
        for (auto& v : counter)
        {
            if (c == v.first)
            {
                total += occurance;
                break;
            }
            total += v.second;
        }
        
        return --total;
    };
    
private:
    std::map<char, int> counter;
};

struct Character
{
    Character(char character, int occurance) :
        character(character), occurance(occurance) {};
    
    char character;
    int occurance;
};

int row_number(const Character& c, const Counter& counter)
{
    return counter.count_up_to(c.character, c.occurance);
}

string InverseBWT(const string& text) {
    Counter counter;
    
    vector<Character> last_row;
    for (auto c { text.begin() }; c != text.end(); ++c)
    {
        counter.add_char(*c);
        last_row.emplace_back( Character (*c, counter.get_count(*c)) );
    }
    
    vector<char> result;
    int index { 0 };
    for (int i { 0 }; i < last_row.size() - 1; ++i)
    {
        result.emplace_back(last_row[index].character);
        index = row_number(last_row[index], counter);
    }
    
    std::reverse(result.begin(), result.end());
    result.emplace_back('$');
    
    return string(result.begin(), result.end());
}

int main() {
    string bwt;
    cin >> bwt;
    cout << InverseBWT(bwt) << endl;
    return 0;
}
