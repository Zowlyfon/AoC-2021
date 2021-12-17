//
// Created by zowlyfon on 17/12/2021.
//

#include <vector>
#include <string>
#include <fstream>
#include <regex>
#include <iostream>

int simulateTrajectory(int xV, int yV, int x1, int x2, int y1, int y2) {
    int x = 0;
    int y = 0;
    int yMax = 0;
    while (y >= y1) {
        x += xV;
        y += yV;

        if (y > yMax) {
            yMax = y;
        }

        if (x >= x1 && x <= x2) {
            if (y >= y1 && y <= y2) {
                return yMax;
            }
        }

        if (xV > 0) {
            xV--;
        }
        else if (xV < 0){
            xV++;
        }
        yV--;
    }
    return -1000;
}

int main() {
    std::ifstream inputFile("input_17");

    std::string line;
    std::getline(inputFile, line);

    std::smatch match;

    int x1, x2, y1, y2;

    if (std::regex_match(line, match, std::regex(R"(target area: x=(-{0,1}\d+)..(-{0,1}\d+), y=(-{0,1}\d+)..(-{0,1}\d+))"))) {
        x1 = std::stoi(match[1]);
        x2 = std::stoi(match[2]);
        y1 = std::stoi(match[3]);
        y2 = std::stoi(match[4]);
    }

    int yMax = 0;
    std::vector<std::pair<int, int>> trajectories;

    for (int x = -1000; x <= 1000; x++) {
        for (int y = -1000; y <= 1000; y++) {
            int simMax = simulateTrajectory(x, y, x1, x2, y1, y2);
            if (simMax > yMax) {
                yMax = simMax;
            }
            if (simMax > -1000) {
                trajectories.emplace_back(x, y);
            }
        }
    }

    std::cout << "YMax: " << yMax << std::endl;

    std::cout << "Trajectories: " << trajectories.size() << std::endl;
    return 0;
}