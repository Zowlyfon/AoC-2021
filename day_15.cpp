//
// Created by Zowlyfon on 16/12/2021.
//

#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <numeric>
#include <chrono>

std::vector<std::pair<int, int>> getGridNeighbours(std::pair<int, int> pos, int max) {
    std::vector<std::pair<int, int>> neighbours;
    if (pos.first + 1 < max) {
        neighbours.emplace_back(pos.first + 1, pos.second);
    }
    if (pos.second + 1 < max) {
        neighbours.emplace_back(pos.first, pos.second + 1);
    }
    if (pos.first - 1 >= 0) {
        neighbours.emplace_back(pos.first - 1, pos.second);
    }
    if (pos.second - 1 >= 0) {
        neighbours.emplace_back(pos.first, pos.second - 1);
    }
    return neighbours;
}

int heuristic(std::pair<int, int> a, std::pair<int, int> b) {
    return std::abs(a.first - b.first) + std::abs(a.second - b.second);
}

struct comp {
    bool operator()(const std::pair<std::pair<int, int>, int> a, std::pair<std::pair<int, int>, int> b) {
        return a.second > b.second;
    }
};

std::vector<std::pair<int, int>> aStarSearch(const std::vector<std::vector<int>> &map) {
    int max = (int)map.size();
    auto start = std::make_pair(0, 0);
    auto goal = std::make_pair(max - 1, max - 1);



    std::priority_queue<std::pair<std::pair<int, int>, int>, std::vector<std::pair<std::pair<int, int>, int>>, comp> frontier;
    frontier.emplace(start, 0);

    std::map<std::pair<int, int>, std::pair<int, int>> cameFrom;
    std::map<std::pair<int, int>, int> costSoFar;
    cameFrom[start] = start;
    costSoFar[start] = 0;

    while (!frontier.empty()) {
        auto current = frontier.top().first;
        frontier.pop();

        if (current == goal) {
            break;
        }

        for (auto &next : getGridNeighbours(current, max)) {
            int newCost = costSoFar[current] + map[current.second][current.first];
            if (!costSoFar.contains(next) || newCost < costSoFar[next]) {
                costSoFar[next] = newCost;
                int priority = newCost + heuristic(goal, next);
                frontier.emplace(next, priority);
                cameFrom[next] = current;
            }
        }
    }

    auto current = goal;
    std::vector<std::pair<int, int>> path;
    path.push_back(current);

    while (current != start) {
        current = cameFrom[current];
        path.push_back(current);
    }

    path.pop_back();

    return path;
}

int main() {
    const auto startT = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<int>> map;
    std::ifstream mapFile("input_15");
    std::string line;

    while (std::getline(mapFile, line)) {
        std::vector<int> mapLine;
        for (auto &number : line) {
            mapLine.push_back(std::stoi(std::string(1, number)));
        }
        map.push_back(mapLine);
    }

    auto path = aStarSearch(map);

    int totalRisk = std::accumulate(path.begin(), path.end(), 0,
                                    [&map](int risk, auto pos){ return risk += map[pos.second][pos.first]; });

    std::cout << "Total Risk: " << totalRisk << std::endl;

    const auto endT = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(endT - startT) << std::endl;

    // part 2;

    std::vector<std::vector<int>> newMap;

    for (auto &row : map) {
        std::vector<int> newRow;
        newRow.insert(newRow.end(), row.begin(), row.end());
        for (int i = 1; i < 5; i++) {
            for (auto &num : row) {
                if (num + i > 9) {
                    newRow.push_back(num + i - 9);
                }
                else {
                    newRow.push_back(num + i);
                }
            }
        }
        newMap.push_back(newRow);
    }

    map = newMap;

    for (int i = 1; i < 5; i++) {
        for (auto &row : map) {
            std::vector<int> newRow;
            for (auto &num : row) {
                if (num + i > 9) {
                    newRow.push_back(num + i - 9);
                }
                else {
                    newRow.push_back(num + i);
                }
            }
            newMap.push_back(newRow);
        }
    }

    map = newMap;

    path = aStarSearch(map);
    totalRisk = std::accumulate(path.begin(), path.end(), 0,
                                [&map](int risk, auto pos){ return risk += map[pos.second][pos.first]; });
    std::cout << "Total Risk 2: " << totalRisk << std::endl;

    const auto endT2 = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(endT2 - startT) << std::endl;

    return 0;
}