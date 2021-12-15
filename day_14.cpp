//
// Created by Zowlyfon on 15/12/2021.
//

#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <string>
#include <regex>
#include <map>

int main() {
    std::map<std::string, char> rules;
    std::string polymer;
    std::ifstream polymerFile("input_14");
    std::string line;
    std::smatch match;

    std::getline(polymerFile, line);
    polymer = line;

    while (std::getline(polymerFile, line))  {
        if (std::regex_match(line, match, std::regex("([A-Z]{2}) -> ([A-Z])"))) {
            std::string key = match[1];
            char value = match[2].str().front();
            rules.emplace(key, value);
        }
    }

    std::map<std::string, unsigned long long int> pairs;
    std::map<std::string, unsigned long long int> newPairs;
    std::map<std::string, std::vector<std::string>> pairRules;

    for (auto &rule : rules) {
        pairs[rule.first] = 0;
        newPairs[rule.first] = 0;
    }

    for (auto &rule : rules) {
        std::string result;
        result.push_back(rule.first[0]);
        result.push_back(rule.second);
        result.push_back(rule.first[1]);

        std::string pair1;
        pair1.push_back(result[0]);
        pair1.push_back(result[1]);

        std::string pair2;
        pair2.push_back(result[1]);
        pair2.push_back(result[2]);

        pairRules[rule.first].push_back(pair1);
        pairRules[rule.first].push_back(pair2);
    }

    for (int i = 0; i < polymer.size() - 1; i++) {
        std::string pair;
        pair.push_back(polymer[i]);
        pair.push_back(polymer[i + 1]);
        pairs[pair]++;
    }


    newPairs = pairs;

    for (int i = 0; i < 10; i++) {
        for (auto &pair : pairs) {
            auto followingPairs = pairRules[pair.first];
            newPairs[pair.first] -= pair.second;
            for (auto &fPair : followingPairs) {
                newPairs[fPair] += pair.second;
            }
        }
        pairs = newPairs;
    }

    std::map<char, unsigned long long int> letters;

    for (auto &pair : pairs) {
        for (auto &letter : pair.first) {
            if (!letters.contains(letter)) {
                letters[letter] = pair.second;
            }
            else {
                letters[letter] += pair.second;
            }
        }
    }

    for (auto &letter : letters) {
        letter.second /= 2;
    }

    letters[polymer.back()]++;

    std::vector<std::pair<char, unsigned long long int>> lettersV;
    lettersV.reserve(letters.size());

    for (auto &letter : letters) {
        lettersV.emplace_back(letter);
    }

    std::sort(lettersV.begin(), lettersV.end(), [](auto a, auto b){ return a.second > b.second; });

    unsigned long long int max = lettersV.front().second;
    unsigned long long int min = lettersV.back().second;

    std::cout << "Difference: " << max - min << std::endl;

    // part 2

    for (int i = 10; i < 40; i++) {
        for (auto &pair : pairs) {
            auto followingPairs = pairRules[pair.first];
            newPairs[pair.first] -= pair.second;
            for (auto &fPair : followingPairs) {
                newPairs[fPair] += pair.second;
            }
        }
        pairs = newPairs;
    }

    letters.clear();

    for (auto &pair : pairs) {
        for (auto &letter : pair.first) {
            if (!letters.contains(letter)) {
                letters[letter] = pair.second;
            }
            else {
                letters[letter] += pair.second;
            }
        }
    }

    for (auto &letter : letters) {
        letter.second /= 2;
    }

    letters[polymer.back()]++;

    lettersV.clear();
    lettersV.reserve(letters.size());

    for (auto &letter : letters) {
        lettersV.emplace_back(letter);
    }

    std::sort(lettersV.begin(), lettersV.end(), [](auto a, auto b){ return a.second > b.second; });

    max = lettersV.front().second;
    min = lettersV.back().second;

    std::cout << "Difference: " << max - min << std::endl;

    return 0;
}