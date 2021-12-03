//
// Created by Zowlyfon on 03/12/2021.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>

struct instruction {
    int command;
    int value;
};

int main() {
    std::ifstream instructionsFile("input_2");
    std::string line;

    std::vector<instruction> instructions;

    std::smatch match;

    while(std::getline(instructionsFile, line)) {
        if (regex_match(line, match, std::regex("(forward)\\s([\\d+])"))) {
            instruction newInstruction{};
            newInstruction.command = 0;
            newInstruction.value = std::stoi(match[2]);
            instructions.push_back(newInstruction);
        }
        else if (regex_match(line, match, std::regex("(up)\\s([\\d+])"))) {
            instruction newInstruction{};
            newInstruction.command = 1;
            newInstruction.value = std::stoi(match[2]);
            instructions.push_back(newInstruction);
        }
        else if (regex_match(line, match, std::regex("(down)\\s([\\d+])"))) {
            instruction newInstruction{};
            newInstruction.command = 2;
            newInstruction.value = std::stoi(match[2]);
            instructions.push_back(newInstruction);
        }
    }

    int depth = 0;
    int horizontal = 0;

    for(instruction currentInstruction : instructions) {
        switch(currentInstruction.command) {
            case 0:
                horizontal += currentInstruction.value;
                break;
            case 1:
                depth -= currentInstruction.value;
                break;
            case 2:
                depth += currentInstruction.value;
                break;
        }
    }

    std::cout << "Depth: " << depth << std::endl;
    std::cout << "Horizontal: " << horizontal << std::endl;
    std::cout << "Horizontal * Depth: " << horizontal * depth << std::endl;

    // part 2

    depth = 0;
    horizontal = 0;
    int aim = 0;

    for(instruction currentInstruction : instructions) {
        switch(currentInstruction.command) {
            case 0:
                horizontal += currentInstruction.value;
                depth += currentInstruction.value * aim;
                break;
            case 1:
                aim -= currentInstruction.value;
                break;
            case 2:
                aim += currentInstruction.value;
                break;
        }
    }

    std::cout << "Depth: " << depth << std::endl;
    std::cout << "Horizontal: " << horizontal << std::endl;
    std::cout << "Horizontal * Depth: " << horizontal * depth << std::endl;

    return 0;
}