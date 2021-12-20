//
// Created by Zowlyfon on 14/12/2021.
//

#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <string>
#include <algorithm>
#include <execution>
#include <chrono>
#include <regex>
#include <map>


enum caveType {
    LARGE,
    SMALL,
    START,
    END,
};

struct Cave {
    std::string name;
    caveType type;
    std::vector<std::string> connections;
    explicit Cave(std::string _name = "", caveType _type = LARGE) :
            name(std::move(_name)), type(_type), connections(std::vector<std::string>()) {}
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

std::vector<std::string> getSmallCaves(const std::map<std::string, Cave>& caves, const std::vector<std::string>& route) {
    std::vector<std::string> smallCaves;
    smallCaves.reserve(route.size());
    std::copy_if(route.begin(), route.end(), std::back_inserter(smallCaves),
                 [&caves](auto &caveName){
        return caves.at(caveName).type == SMALL;
    });

    return smallCaves;
}

int main() {
    const auto startT = std::chrono::high_resolution_clock::now();
    std::map<std::string, Cave> caves;
    std::ifstream caveFile("input_12");
    std::string line;
    std::smatch match;

    while (std::getline(caveFile, line)) {
        if (std::regex_match(line, match, std::regex("(\\w+)-(\\w+)"))) {
            std::string cave1Name = match[1];
            std::string cave2Name = match[2];

            /*
            if (std::find_if(caves.begin(), caves.end(), [&cave1Name](auto &cave){ return cave.name == cave1Name; }) == caves.end()) {
                caves.push_back(makeCave(cave1Name));
            }
            if (std::find_if(caves.begin(), caves.end(), [&cave2Name](auto &cave){ return cave.name == cave2Name; }) == caves.end()) {
                caves.push_back(makeCave(cave2Name));
            }
             */

            if (!caves.contains(cave1Name)) {
                caves[cave1Name] = makeCave(cave1Name);
            }
            if (!caves.contains(cave2Name)) {
                caves[cave2Name] = makeCave(cave2Name);
            }

            caves.at(cave1Name).connections.push_back(cave2Name);
            caves.at(cave2Name).connections.push_back(cave1Name);
        }
    }

    Cave startCave = caves["start"];

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
            auto cave = caves.at(route.back());
            if (cave.type == END) {
                newRoutes.push_back(route);
            } else {
                for (auto &con : cave.connections) {
                    auto conCave = caves.at(con);
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

    const auto endT = std::chrono::high_resolution_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(endT - startT).count() << std::endl;

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
            auto cave = caves.at(route.back());
            if (cave.type == END) {
                newRoutes.push_back(route);
            } else {
                for (auto &con : cave.connections) {
                    bool shouldAdd = false;
                    auto conCave = caves.at(con);

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

    auto endT2 = std::chrono::high_resolution_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(endT2 - startT).count() << std::endl;

    return 0;
}