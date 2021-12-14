//
// Created by Zowlyfon on 14/12/2021.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

// (x, y + 1), (x + 1, y + 1), (x + 1, y), (x + 1, y - 1), (x, y - 1), (x - 1, y - 1), (x - 1, y), (x - 1, y + 1)

std::vector<std::pair<int, int>> getAdjacent(int x, int y) {
    std::vector<std::pair<int, int>> positions;
    positions.emplace_back(x, y + 1);
    positions.emplace_back(x + 1, y + 1);
    positions.emplace_back(x + 1, y);
    positions.emplace_back(x + 1, y - 1);
    positions.emplace_back(x, y - 1);
    positions.emplace_back(x - 1, y - 1);
    positions.emplace_back(x - 1, y);
    positions.emplace_back(x - 1, y + 1);

    std::vector<std::pair<int, int>> legalPositions;

    std::copy_if(positions.begin(), positions.end(), std::back_inserter(legalPositions),
                 [](auto pos){ return pos.first >= 0 && pos.first <= 9 && pos.second >= 0 && pos.second <= 9; });

    return legalPositions;
}

struct Octopus {
    explicit Octopus(int _energy) : energy(_energy) {}
    int energy;
    bool flashed = false;
};

int main() {
    std::vector<std::vector<Octopus>> octopi;
    std::ifstream octopiFile("input_11");
    std::string line;

    while (std::getline(octopiFile, line)) {
        std::vector<Octopus> octopiLine;
        for (auto &octopus : line) {
            std::string octopusString;
            octopusString.push_back(octopus);
            octopiLine.emplace_back(std::stoi(octopusString));
        }
        octopi.push_back(octopiLine);
    }

    auto octopi2(octopi);

    int totalFlashes = 0;

    for (int i = 0; i < 100; i++) {
        for (auto &row : octopi) {
            for (auto &octopus : row) {
                if (octopus.flashed) {
                    octopus.energy = 0;
                }
                octopus.flashed = false;
                octopus.energy++;
            }
        }
        int numFlashes = 1;
        while(numFlashes > 0) {
            numFlashes = 0;
            std::vector<std::pair<int, int>> adjacent;
            for (int y = 0; y < 10; y++) {
                for (int x = 0; x < 10; x++) {
                    if (octopi[y][x].energy > 9 && !octopi[y][x].flashed) {
                        numFlashes++;
                        auto newAdjacent = getAdjacent(x, y);
                        adjacent.insert(adjacent.end(), newAdjacent.begin(), newAdjacent.end());
                        octopi[y][x].flashed = true;
                    }
                }
            }
            for (auto &pos : adjacent) {
                octopi[pos.second][pos.first].energy++;
            }
            totalFlashes += numFlashes;
        }
    }

    std::cout << "Total Flashes: " << totalFlashes << std::endl;

    // part 2

    octopi = octopi2;
    int count = 0;
    bool allFlashed = false;

    while(!allFlashed) {
        allFlashed = true;
        for (auto &row : octopi) {
            for (auto &octopus : row) {
                if (octopus.flashed) {
                    octopus.energy = 0;
                }
                else {
                    allFlashed = false;
                }
                octopus.flashed = false;
                octopus.energy++;
            }
        }
        int numFlashes = 1;
        while(numFlashes > 0) {
            numFlashes = 0;
            std::vector<std::pair<int, int>> adjacent;
            for (int y = 0; y < 10; y++) {
                for (int x = 0; x < 10; x++) {
                    if (octopi[y][x].energy > 9 && !octopi[y][x].flashed) {
                        numFlashes++;
                        auto newAdjacent = getAdjacent(x, y);
                        adjacent.insert(adjacent.end(), newAdjacent.begin(), newAdjacent.end());
                        octopi[y][x].flashed = true;
                    }
                }
            }
            for (auto &pos : adjacent) {
                octopi[pos.second][pos.first].energy++;
            }
            totalFlashes += numFlashes;
        }

        count++;
    }

    std::cout << "First simultaneous flash: " << count - 1 << std::endl;
    return 0;

}