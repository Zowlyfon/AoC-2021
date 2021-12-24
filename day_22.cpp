//
// Created by zowlyfon on 22/12/2021.
//

#include <utility>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <regex>
#include <chrono>

struct Instruction {
    bool status;
    std::pair<int, int> x;
    std::pair<int, int> y;
    std::pair<int, int> z;
};

struct Box {
    Box(std::pair<int, int> _x, std::pair<int, int> _y, std::pair<int, int> _z, bool _sign) :
    x(std::move(_x)), y(std::move(_y)), z(std::move(_z)), sign(_sign) {}
    bool sign;
    std::pair<int, int> x;
    std::pair<int, int> y;
    std::pair<int, int> z;
};

bool instructionWithinBounds(const Instruction &instruction) {
    if (instruction.x.first > 50 || instruction.x.first < -50)
        return false;
    if (instruction.x.second > 50 || instruction.x.second < -50)
        return false;
    if (instruction.y.first > 50 || instruction.y.first < -50)
        return false;
    if (instruction.y.second > 50 || instruction.y.second < -50)
        return false;
    if (instruction.y.first > 50 || instruction.y.first < -50)
        return false;
    if (instruction.y.second > 50 || instruction.y.second < -50)
        return false;

    return true;
}

int main() {
    std::map<int, std::map<int, std::map<int, bool>>> grid;

    std::vector<Instruction> instructions;
    std::ifstream instructionsFile("input_22");
    std::string line;
    std::smatch match;

    while(std::getline(instructionsFile, line)) {
        if(std::regex_match(line, match, std::regex(R"((on|off)\sx=(-{0,1}\d+)..(-{0,1}\d+),y=(-{0,1}\d+)..(-{0,1}\d+),z=(-{0,1}\d+)..(-{0,1}\d+))"))) {
            bool status = false;
            std::pair<int, int> x;
            std::pair<int, int> y;
            std::pair<int, int> z;

            if (match[1] == "on") {
                status = true;
            }

            x.first = std::stoi(match[2]);
            x.second = std::stoi(match[3]);

            y.first = std::stoi(match[4]);
            y.second = std::stoi(match[5]);

            z.first = std::stoi(match[6]);
            z.second = std::stoi(match[7]);

            Instruction instruction;
            instruction.status = status;
            instruction.x = x;
            instruction.y = y;
            instruction.z = z;

            instructions.push_back(instruction);
        }
    }

    for (auto &instruction : instructions) {
        if (instructionWithinBounds(instruction)) {
            for (int x = instruction.x.first; x <= instruction.x.second; x++) {
                for (int y = instruction.y.first; y <= instruction.y.second; y++) {
                    for (int z = instruction.z.first; z <= instruction.z.second; z++) {
                        grid[x][y][z] = instruction.status;
                    }
                }
            }
        }
    }

    unsigned long long int cubesTurnedOn = 0;

    for (auto &x : grid) {
        for (auto &y : x.second) {
            for (auto &z : y.second) {
                if (z.second) {
                    cubesTurnedOn++;
                }
            }
        }
    }

    std::cout << "Cubes Turned On: " << cubesTurnedOn << std::endl;

    // part 2

    std::vector<Box> boxes;

    unsigned long long int volume = 0;

    for (auto &i : instructions) {
        Box box(i.x, i.y, i.z, i.status);
        std::vector<Box> newBoxes;
        if (i.status) {
            newBoxes.push_back(box);
        }
        for (auto &b : boxes) {
            std::pair<int, int> ix = std::make_pair(std::max(b.x.first, i.x.first), std::min(b.x.second, i.x.second));
            std::pair<int, int> iy = std::make_pair(std::max(b.y.first, i.y.first), std::min(b.y.second, i.y.second));
            std::pair<int, int> iz = std::make_pair(std::max(b.z.first, i.z.first), std::min(b.z.second, i.z.second));
            bool sign = !b.sign;

            if (ix.first <= ix.second && iy.first <= iy.second && iz.first <= iz.second) {
                Box iBox(ix, iy, iz, sign);
                newBoxes.push_back(iBox);
            }
        }
        boxes.insert(boxes.end(), newBoxes.begin(), newBoxes.end());
    }

    for (auto &b : boxes) {
        long long int xLen = ((b.x.second - b.x.first) + 1);
        long long int yLen = ((b.y.second - b.y.first) + 1);
        long long int zLen = ((b.z.second - b.z.first) + 1);
        long long int boxVolume = xLen * yLen * zLen;
        if (boxVolume < 0) {
            std::cout << "NEGATIVE" << std::endl;
        }
        if (b.sign) {
            volume += boxVolume;
        }
        else {
            volume -= boxVolume;
        }
    }

    std::cout << "Lights on: " << volume << std::endl;

     return 0;
}
