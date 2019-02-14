#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <cstring>
#include <algorithm>
using namespace std;
struct statisticsName
{
    int count;
    string word;
};
typedef struct statisticsName statistics;
bool Compare(const statistics& a, const statistics& b)
{
    if (((statistics)a).count < ((statistics)b).count) return false;
    else return true;
}

map <string, int> MatchWordsAndFrequency (const string path)
{
    const int max_number_of_same_marks = 5;
    char mark[] = "1234567890»«.,!?:;\"'-_\n ()*=+[]—%$№";
    ifstream file(path);
    if(!file.is_open())
    {
        cerr << "Can`t open file" << endl;
        exit(0);
    }
    string tmp;
    map <string, int> word;
    while(getline(file, tmp, ' '))
    {
        for (int i = 0; i < (int)strlen(mark) * max_number_of_same_marks; i++)
        {
            int pos = tmp.find(mark[i%strlen(mark)]);
            if (pos != -1) tmp.erase(pos);
        }
        transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
        if(tmp.size() != 0)
        {
            auto it2 = word.find(tmp);
            if (it2 != word.end()) (it2 -> second) ++;
            else word.insert(pair<string,int>(tmp, 1));
        }
    }
    file.close();
    return word;
}

vector <statistics> ReturnStats(const string path)
{
    map <string, int> matched;
    matched = MatchWordsAndFrequency(path);
    vector <statistics> stats;
    statistics tmp;
    for (auto it = matched.begin(); it != matched.end(); it ++)
    {
        tmp.word = it -> first;
        tmp.count = it -> second;
        stats.push_back(tmp);
    }
    sort(stats.begin(), stats.end(), Compare);
    return stats;
}
int main()
{
    vector<statistics> stats = ReturnStats("/home/ivan/Count_words_in_text/text");
    int number;
    cout << "Enter the number of words with high frequency you want to see: ";
    cin >> number;
    for (auto it = stats.begin(); it != stats.begin() + number; it++) cout << it->count << " " << it->word << endl;
}
