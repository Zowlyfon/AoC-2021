//
// Created by Zowlyfon on 14/12/2021.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

bool isOpeningBrace(char brace) {
    return brace == '(' || brace == '[' || brace == '{' || brace == '<';
}

char getClosingBrace(char brace) {
    switch(brace) {
        case '(':
            return ')';
        case '[':
            return ']';
        case '{':
            return '}';
        case '<':
            return '>';
        default:
            return '\0';
    }
}

int getScore(char brace) {
    switch(brace) {
        case ')':
            return 3;
        case ']':
            return 57;
        case '}':
            return 1197;
        case '>':
            return 25137;
        default:
            return 0;
    }
}

int getScore2(char brace) {
    switch(brace) {
        case ')':
            return 1;
        case ']':
            return 2;
        case '}':
            return 3;
        case '>':
            return 4;
        default:
            return 0;
    }
}

int incorrect = 0;

int main() {
    std::vector<std::string> navigationLines;
    std::vector<std::string> incompleteLines;
    std::ifstream navigationFile("input_10");
    std::string line;

    while (std::getline(navigationFile, line)) {
        navigationLines.push_back(line);
    }

    for (auto &nav : navigationLines) {
        std::vector<char> braces;
        bool lineIsCorrect = true;
        for (auto &brace : nav) {
            if (isOpeningBrace(brace)) {
                braces.push_back(brace);
            }
            else {
                if (brace != getClosingBrace(braces.back())) {
                    incorrect += getScore(brace);
                    lineIsCorrect = false;
                    break;
                }
                braces.pop_back();
            }
        }

        if (lineIsCorrect) {
            incompleteLines.push_back(nav);
        }
    }

    std::cout << "Incorrect score: " << incorrect <<std::endl;

    // part 2

    std::vector<unsigned long long int> autocompleteScores;

    for (auto &nav : incompleteLines) {
        std::vector<char> braces;
        unsigned long long int score = 0;

        for (auto &brace : nav) {
            if (isOpeningBrace(brace)) {
                braces.push_back(brace);
            }
            else {
                braces.pop_back();
            }
        }

        for (auto i = braces.rbegin(); i != braces.rend(); i++) {
            score *= 5;
            score += getScore2(getClosingBrace(*i));
        }

        autocompleteScores.push_back(score);


    }

    std::sort(autocompleteScores.begin(), autocompleteScores.end(),
              [](const auto &a, const auto &b){ return a > b; });

    int middleScorePos = (int)(autocompleteScores.size() / 2);
    unsigned long long int middleScore = autocompleteScores[middleScorePos];

    std::cout << "Middle Score: " << middleScore << std::endl;

    return 0;
}