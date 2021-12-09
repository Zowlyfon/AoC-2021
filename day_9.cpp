//
// Created by Zowlyfon on 09/12/2021.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

int getLeft(const std::vector<std::vector<int>> &input, int x, int y) {
    if(x > 0) {
        return input[y][x - 1];
    }
    return 10;
}

int getRight(const std::vector<std::vector<int>> &input, int x, int y) {
    if(x < input[0].size() - 1){
        return input[y][x + 1];
    }
    return 10;
}

int getUp(const std::vector<std::vector<int>> &input, int x, int y) {
    if(y > 0) {
        return input[y - 1][x];
    }
    return 10;
}

int getDown(const std::vector<std::vector<int>> &input, int x, int y) {
    if(y < input.size() - 1) {
        return input[y + 1][x];
    }
    return 10;
}

int main() {
    std::vector<std::vector<int>> map;
    std::ifstream mapFile("input_9");
    std::string line;

    while(std::getline(mapFile, line)) {
        std::vector<int> newRow;
        for(auto &letter : line) {
            std::string sLetter;
            sLetter.push_back(letter);
            newRow.push_back(std::stoi(sLetter));
        }
        map.push_back(newRow);
    }

    std::vector<std::pair<int, int>> lowPoints;

    for(int j = 0; j < map.size(); j++) {
        for(int i = 0; i < map[0].size(); i++) {
            int left = getLeft(map, i, j);
            int right = getRight(map, i, j);
            int up = getUp(map, i, j);
            int down = getDown(map, i, j);
            int x = map[j][i];
            if(left > x && right > x && up > x && down > x) {
                lowPoints.emplace_back(i, j);
            }
        }
    }

    int totalRisk = 0;

    for(auto &lowPoint : lowPoints) {
        totalRisk += map[lowPoint.second][lowPoint.first] + 1;
    }

    std::cout << "Total Risk: " << totalRisk << std::endl;

    // part 2
    std::vector<std::vector<std::pair<int, int>>> basins;

    for(auto &lowPoint : lowPoints) {
        std::pair<int, int> x = lowPoint;

        bool done = false;

        for (auto &b : basins) {
            for (auto &p : b) {
                if (p == x) {
                    done = true;
                }
            }
        }

        if(!done) {
            std::vector<std::pair<int, int>> basin;
            bool complete = false;
            int stuckCount = 0;

            while(!complete) {
                if (getLeft(map, x.first, x.second) < 9 &&
                    std::find(basin.begin(), basin.end(), std::make_pair(x.first - 1, x.second)) == basin.end()) {
                    x.first--;
                    basin.push_back(x);
                    stuckCount = 0;
                }
                else if(getUp(map, x.first, x.second) < 9 &&
                        std::find(basin.begin(), basin.end(), std::make_pair(x.first, x.second - 1)) == basin.end()) {
                    x.second--;
                    basin.push_back(x);
                    stuckCount = 0;
                }
                else if(getRight(map, x.first, x.second) < 9 &&
                        std::find(basin.begin(), basin.end(), std::make_pair(x.first + 1, x.second)) == basin.end()) {
                    x.first++;
                    basin.push_back(x);
                    stuckCount = 0;
                }
                else if(getDown(map, x.first, x.second) < 9 &&
                        std::find(basin.begin(), basin.end(), std::make_pair(x.first, x.second + 1)) == basin.end()) {
                    x.second++;
                    basin.push_back(x);
                    stuckCount = 0;
                }
                else {
                    stuckCount++;

                    if(stuckCount >= basin.size() - 1) {
                        complete = true;
                    }
                    else {
                        x = basin.rbegin()[stuckCount];
                    }
                }
            }

            basins.push_back(basin);
        }
    }

    std::sort(basins.begin(), basins.end(), [](const auto &a, const auto &b){ return a.size() > b.size(); });

    int answer = (int)(basins[0].size() * basins[1].size() * basins[2].size());

    std::cout << "Answer: " << answer << std::endl;

    return 0;
}