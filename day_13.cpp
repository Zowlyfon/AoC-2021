//
// Created by Zowlyfon on 15/12/2021.
//

#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <string>
#include <regex>
#include <numeric>

enum foldDimension {
    X,
    Y
};

struct Instruction {
    Instruction(int _pos, foldDimension _dimension) : pos(_pos), dimension(_dimension) {}
    int pos;
    foldDimension dimension;
};

int main() {
    std::vector<std::pair<int, int>> dots;
    std::vector<Instruction> instructions;
    std::ifstream instructionFile("input_13");
    std::string line;
    std::smatch match;

    while (std::getline(instructionFile, line)) {
        if (std::regex_match(line, match, std::regex("(\\d+),(\\d+)"))) {
            int x = std::stoi(match[1]);
            int y = std::stoi(match[2]);
            dots.emplace_back(x, y);
        }
        else if (std::regex_match(line, match, std::regex("fold along ([xy])=(\\d+)"))) {
            foldDimension dimension = Y;
            if (match[1] == 'x') {
                dimension = X;
            }
            int pos = std::stoi(match[2]);
            instructions.emplace_back(pos, dimension);
        }
    }

    Instruction firstInstruction = instructions.front();
    std::vector<std::pair<int, int>> foldedDots;

    for (auto &pos : dots) {
        if (firstInstruction.dimension == X) {
            if (pos.first > firstInstruction.pos) {
                int dist = pos.first - firstInstruction.pos;
                int newX = firstInstruction.pos - dist;
                auto newPos = std::make_pair(newX, pos.second);
                if (std::find(foldedDots.begin(), foldedDots.end(), newPos) == foldedDots.end()) {
                    foldedDots.emplace_back(newPos);
                }
            }
            else {
                if (std::find(foldedDots.begin(), foldedDots.end(), pos) == foldedDots.end()) {
                    foldedDots.emplace_back(pos);
                }
            }
        }
        else {
            if (pos.second > firstInstruction.pos) {
                int dist = pos.second - firstInstruction.pos;
                int newY = firstInstruction.pos - dist;
                auto newPos = std::make_pair(pos.second, newY);
                if (std::find(foldedDots.begin(), foldedDots.end(), newPos) == foldedDots.end()) {
                    foldedDots.emplace_back(newPos);
                }
            }
            else {
                if (std::find(foldedDots.begin(), foldedDots.end(), pos) == foldedDots.end()) {
                    foldedDots.emplace_back(pos);
                }
            }
        }
    }

    std::cout << "Number of dots: " << foldedDots.size() << std::endl;

    foldedDots.clear();

    for (auto &instruction : instructions) {
        for (auto &pos : dots) {
            if (instruction.dimension == X) {
                if (pos.first > instruction.pos) {
                    int dist = pos.first - instruction.pos;
                    int newX = instruction.pos - dist;
                    auto newPos = std::make_pair(newX, pos.second);
                    if (std::find(foldedDots.begin(), foldedDots.end(), newPos) == foldedDots.end()) {
                        foldedDots.emplace_back(newPos);
                    }
                }
                else {
                    if (std::find(foldedDots.begin(), foldedDots.end(), pos) == foldedDots.end()) {
                        foldedDots.emplace_back(pos);
                    }
                }
            }
            else {
                if (pos.second > instruction.pos) {
                    int dist = pos.second - instruction.pos;
                    int newY = instruction.pos - dist;
                    auto newPos = std::make_pair(pos.first, newY);
                    if (std::find(foldedDots.begin(), foldedDots.end(), newPos) == foldedDots.end()) {
                        foldedDots.emplace_back(newPos);
                    }
                }
                else {
                    if (std::find(foldedDots.begin(), foldedDots.end(), pos) == foldedDots.end()) {
                        foldedDots.emplace_back(pos);
                    }
                }
            }
        }
        dots = foldedDots;
        foldedDots.clear();
    }

    int maxX = std::accumulate(dots.begin(), dots.end(), -1000,[](int max, auto pos){ return (pos.first > max) ? pos.first : max; });
    int maxY = std::accumulate(dots.begin(), dots.end(), -1000,[](int max, auto pos){ return (pos.second > max) ? pos.second : max; });
    int minX = std::accumulate(dots.begin(), dots.end(), 1000,[](int min, auto pos){ return (pos.first < min) ? pos.first : min; });
    int minY = std::accumulate(dots.begin(), dots.end(), 1000,[](int min, auto pos){ return (pos.second < min) ? pos.second : min; });

    std::cout << std::endl;

    for (int y = minY; y <= maxY; y++) {
        std::cout << "|";
        for (int x = minX; x <= maxX; x++) {
            auto pos = std::make_pair(x, y);
            if (std::find(dots.begin(), dots.end(), pos) == dots.end()) {
                std::cout << ".";
            }
            else {
                std::cout << "#";
            }
        }
        std::cout << std::endl;
    }

    return 0;
}