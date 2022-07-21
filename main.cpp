#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>

int getFreq(std::unordered_map<std::string, int>& freq, const std::string curName,
            const std::unordered_multimap<std::string, std::string>& nameSynonyms1,
            const std::unordered_multimap<std::string, std::string>& nameSynonyms2,
            std::unordered_map<std::string, int>::iterator& iter)
{
    if(freq.find(curName) == freq.end()) return 0;

    int curFreq = freq[curName];

    iter = freq.find(curName);

    freq.erase(iter++);

    auto synonymRange = nameSynonyms1.equal_range(curName);

    for(auto synonym = synonymRange.first; synonym != synonymRange.second; ++synonym)
        curFreq += getFreq(freq, synonym->second, nameSynonyms1, nameSynonyms2, iter);

    synonymRange = nameSynonyms2.equal_range(curName);

    for(auto synonym = synonymRange.first; synonym != synonymRange.second; ++synonym)
        curFreq += getFreq(freq, synonym->second,nameSynonyms1, nameSynonyms2, iter);

    return curFreq;
}

std::unordered_map<std::string, int> makeList(std::unordered_map<std::string, int> curList,
                                              const std::unordered_multimap<std::string, std::string>& nameSynonyms1)
{
    std::unordered_map<std::string, int> newList;

    std::unordered_multimap<std::string, std::string> nameSynonyms2;

    for(const auto& iter : nameSynonyms1)
        nameSynonyms2.emplace(iter.second, iter.first);

    for(auto iter = curList.begin(); iter != curList.end(); )
    {
        const std::string curName = iter->first;
        const int freq = getFreq(curList, curName, nameSynonyms1, nameSynonyms2, iter);
        newList.emplace(curName, freq);
    }

    return newList;
}

int main()
{
    std::unordered_map<std::string, int> res = makeList({{"John", 15}, {"Jon", 12}, {"Chris", 13}, {"Kris", 4}, {"Christopher", 19}},
                                                       {{"Jon", "John"}, {"John", "Johnny"}, {"Chris", "Kris"}, {"Chris", "Christopher"}});
    for(auto i : res)
        std::cout << i.first << ' ' << i.second << '\n';
    return 0;
}
