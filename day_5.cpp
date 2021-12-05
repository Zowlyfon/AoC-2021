//
// Created by zowlyfon on 05/12/2021.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <regex>

struct ventVector {
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
};

int main() {
    std::ifstream vectorFiles("input_5");
    std::string line;
    std::smatch match;
    std::vector<ventVector> ventVectors;
    int map[1000][1000];

    while(std::getline(vectorFiles, line)) {
        if(std::regex_match(line, match, std::regex(R"((\d+),(\d+)\s->\s(\d+),(\d+))"))) {
            ventVector newVector;
            newVector.x1 = std::stoi(match[1]);
            newVector.y1 = std::stoi(match[2]);
            newVector.x2 = std::stoi(match[3]);
            newVector.y2 = std::stoi(match[4]);
            ventVectors.push_back(newVector);
        }
    }

    for(int i = 0; i < 1000; i++) {
        for(int j = 0; j < 1000; j++) {
            map[i][j] = 0;
        }
    }

    for(ventVector vent : ventVectors) {
        if(vent.x1 == vent.x2) {
            if(vent.y1 <= vent.y2) {
                for(int i = vent.y1; i <= vent.y2; i++) {
                    map[vent.x1][i]++;
                }
            } else {
                for(int i = vent.y1; i >= vent.y2; i--) {
                    map[vent.x1][i]++;
                }
            }
        }
        else if(vent.y1 == vent.y2) {
            if(vent.x1 <= vent.x2) {
                for(int i = vent.x1; i <= vent.x2; i++) {
                    map[i][vent.y1]++;
                }
            } else {
                for(int i = vent.x1; i >= vent.x2; i--) {
                    map[i][vent.y1]++;
                }
            }
        }
        else if(vent.x1 <= vent.x2) {
            if(vent.y1 <= vent.y2) {
                if((vent.x2 - vent.x1) == (vent.y2 - vent.y1)) {
                    for(int i = 0; i <= vent.x2 - vent.x1; i++) {
                        map[vent.x1 + i][vent.y1 + i]++;
                    }
                }
            }
            else {
                if((vent.x2 - vent.x1) == (vent.y1 - vent.y2)) {
                    for(int i = 0; i <= vent.x2 - vent.x1; i++) {
                        map[vent.x1 + i][vent.y1 - i]++;
                    }
                }
            }
        }
        else {
            if(vent.y1 <= vent.y2) {
                if((vent.x1 - vent.x2) == (vent.y2 - vent.y1)) {
                    for(int i = 0; i <= vent.x1 - vent.x2; i++) {
                        map[vent.x1 - i][vent.y1 + i]++;
                    }
                }
            }
            else {
                if((vent.x1 - vent.x2) == (vent.y1 - vent.y2)) {
                    for(int i = 0; i <= vent.x1 - vent.x2; i++) {
                        map[vent.x1 - i][vent.y1 - i]++;
                    }
                }
            }
        }
    }

    int dangerCount = 0;

    for(int i = 0; i < 1000; i++) {
        for(int j = 0; j < 1000; j++) {
            if (map[i][j] >= 2) {
                dangerCount++;
            }
        }
    }

    std::cout << "Danger Count: " << dangerCount << std::endl;

    return 0;
}