//
// Created by Zowlyfon on 14/12/2021.
//

#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <string>
#include <algorithm>
#include <regex>

enum caveType {
    LARGE,
    SMALL,
    START,
    END,
};

struct Cave {
    Cave(std::string _name, caveType _type) : name(std::move(_name)), type(_type) {}
    std::string name;
    caveType type;
    std::vector<std::string> connections;
};

Cave makeCave(const std::string& caveName) {
    std::smatch match;
    caveType type = LARGE;
    if (caveName == "start") {
        type = START;
    }
    else if (caveName == "end") {
        type = END;
    }
    else if (std::regex_match(caveName, match, std::regex("[a-z]+"))) {
        type = SMALL;
    }

    Cave newCave(caveName, type);

    return newCave;
}

Cave findCaveByName(std::vector<Cave> caves, const std::string& caveName) {
    auto cave = std::find_if(caves.begin(), caves.end(), [&caveName](auto &cave){ return cave.name == caveName; });
    return *cave;
}

std::vector<std::string> getSmallCaves(const std::vector<Cave>& caves, const std::vector<std::string>& route) {
    std::vector<std::string> smallCaves;
    for (auto &caveName : route) {
        Cave cave = findCaveByName(caves, caveName);
        if (cave.type == SMALL) {
            smallCaves.push_back(caveName);
        }
    }

    return smallCaves;
}

int main() {
    std::vector<Cave> caves;
    std::ifstream caveFile("input_12");
    std::string line;
    std::smatch match;

    while (std::getline(caveFile, line)) {
        if (std::regex_match(line, match, std::regex("(\\w+)-(\\w+)"))) {
            std::string cave1Name = match[1];
            std::string cave2Name = match[2];
            if (std::find_if(caves.begin(), caves.end(), [&cave1Name](auto &cave){ return cave.name == cave1Name; }) == caves.end()) {
                caves.push_back(makeCave(cave1Name));
            }
            if (std::find_if(caves.begin(), caves.end(), [&cave2Name](auto &cave){ return cave.name == cave2Name; }) == caves.end()) {
                caves.push_back(makeCave(cave2Name));
            }

            auto cave1 = std::find_if(caves.begin(), caves.end(), [&cave1Name](auto &cave){ return cave.name == cave1Name; });
            auto cave2 = std::find_if(caves.begin(), caves.end(), [&cave2Name](auto &cave){ return cave.name == cave2Name; });

            cave1->connections.push_back(cave2->name);
            cave2->connections.push_back(cave1->name);
        }
    }

    Cave startCave = findCaveByName(caves, "start");
    Cave endCave = findCaveByName(caves, "end");

    std::vector<std::vector<std::string>> routes;

    for (auto &con : startCave.connections) {
        std::vector<std::string> route;
        route.push_back(con);
        routes.push_back(route);
    }

    bool allRoutesComplete = false;

    std::vector<std::vector<std::string>> newRoutes;

    while (!allRoutesComplete) {
        allRoutesComplete = true;
        for (auto &route : routes) {
            auto cave = findCaveByName(caves, route.back());
            if (cave.type == END) {
                newRoutes.push_back(route);
            } else {
                for (auto &con : cave.connections) {
                    auto conCave = findCaveByName(caves, con);
                    if (conCave.type != START) {
                        if (conCave.type == LARGE || conCave.type == END) {
                            std::vector<std::string> newRoute;
                            newRoute = route;
                            newRoute.push_back(conCave.name);
                            newRoutes.push_back(newRoute);
                            allRoutesComplete = false;
                        }
                        else {
                            if (std::find(route.begin(), route.end(), con) == route.end()) {
                                std::vector<std::string> newRoute;
                                newRoute = route;
                                newRoute.push_back(conCave.name);
                                newRoutes.push_back(newRoute);
                                allRoutesComplete = false;
                            }
                        }
                    }
                }
            }
        }

        routes = newRoutes;
        newRoutes.clear();
    }

    std::cout << "Routes: " << routes.size() << std::endl;

    // part 2

    allRoutesComplete = false;
    newRoutes.clear();
    routes.clear();

    for (auto &con : startCave.connections) {
        std::vector<std::string> route;
        route.push_back(con);
        routes.push_back(route);
    }

    while (!allRoutesComplete) {
        allRoutesComplete = true;
        std::cout << " : " << routes.size() << std::endl;
        for (auto &route : routes) {
            auto cave = findCaveByName(caves, route.back());
            if (cave.type == END) {
                newRoutes.push_back(route);
            } else {
                for (auto &con : cave.connections) {
                    bool shouldAdd = false;
                    auto conCave = findCaveByName(caves, con);

                    if (conCave.type != START) {
                        if (conCave.type == LARGE || conCave.type == END) {
                            shouldAdd = true;
                        }
                        else {
                            if (std::find(route.begin(), route.end(), con) == route.end()) {
                                shouldAdd = true;
                            }
                            else {
                                int duplicates = 0;
                                auto smallCaves = getSmallCaves(caves, route);
                                for (auto &smallCave : smallCaves) {
                                    auto num = std::count(route.begin(), route.end(), smallCave);
                                    if (num > 1) {
                                        duplicates++;
                                    }
                                }
                                if (duplicates <= 0) {
                                    shouldAdd = true;
                                }
                            }
                        }
                    }
                    if (shouldAdd) {
                        std::vector<std::string> newRoute;
                        newRoute = route;
                        newRoute.push_back(conCave.name);
                        newRoutes.push_back(newRoute);
                        allRoutesComplete = false;
                    }
                }
            }
        }

        routes = newRoutes;
        newRoutes.clear();
    }

    std::cout << "Routes: " << routes.size() << std::endl;

    return 0;
}