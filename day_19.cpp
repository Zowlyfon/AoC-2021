//
// Created by zowlyfon on 20/12/2021.
//

#include <utility>
#include <vector>
#include <string>
#include <fstream>
#include <regex>
#include <iostream>
#include <eigen3/Eigen/Dense>
#include <map>
#include <set>
#include <cmath>

struct ScannerRelation {
    explicit ScannerRelation(Eigen::RowVector3i _beacon) : beacon(std::move(_beacon)), relations(std::vector<Eigen::RowVector3i>()) {}
    Eigen::RowVector3i beacon;
    std::vector<Eigen::RowVector3i> relations;
};

struct Beacon {
    explicit Beacon(Eigen::RowVector3i _pos) : pos(std::move(_pos)), distances(std::vector<std::pair<Eigen::RowVector3i, double>>()) {}
    Eigen::RowVector3i pos;
    std::vector<std::pair<Eigen::RowVector3i, double>> distances;
    std::vector<std::pair<int, std::vector<ScannerRelation>>> relations;
    std::vector<std::pair<int, Eigen::RowVector3i>> matchingBeacons;
};

struct Scanner {
    //explicit Scanner(std::vector<Beacon> _beacons) : beacons(std::move(_beacons)) {}
    std::vector<Beacon> beacons;
    //std::vector<std::pair<int, std::pair<Eigen::Matrix3i, Eigen::RowVector3i>>> transforms;
    std::map<int, std::pair<Eigen::Matrix3i, Eigen::RowVector3i>> transforms;
    //std::vector<std::pair<int, std::vector<std::pair<Eigen::RowVector3i, Eigen::RowVector3i>>>> matchingBeacons;
    std::map<int, std::vector<std::pair<Eigen::RowVector3i, Eigen::RowVector3i>>> matchingBeacons;
};

std::vector<Eigen::Matrix3i> getRotations() {
    std::vector<Eigen::Matrix3i> signs;
    std::vector<Eigen::Matrix3i> permutations;

    Eigen::Matrix3i m;

    m << 1, 0, 0,
         0, 1, 0,
         0, 0, 1;
    signs.push_back(m);

    m << -1, 0, 0,
         0, 1, 0,
         0, 0, 1;
    signs.push_back(m);

    m << 1, 0, 0,
         0, -1, 0,
         0, 0, 1;
    signs.push_back(m);

    m << 1, 0, 0,
         0, 1, 0,
         0, 0, -1;
    signs.push_back(m);

    m << -1, 0, 0,
         0, -1, 0,
         0, 0, 1;
    signs.push_back(m);

    m << 1, 0, 0,
         0, -1, 0,
         0, 0, -1;
    signs.push_back(m);

    m << -1, 0, 0,
         0, 1, 0,
         0, 0, -1;
    signs.push_back(m);

    m << -1, 0, 0,
         0, -1, 0,
         0, 0, -1;
    signs.push_back(m);

    m << 1, 0, 0,
         0, 1, 0,
         0, 0, 1;
    permutations.push_back(m);

    m << 1, 0, 0,
         0, 0, 1,
         0, 1, 0;
    permutations.push_back(m);

    m << 0, 1, 0,
         1, 0, 0,
         0, 0, 1;
    permutations.push_back(m);

    m << 0, 1, 0,
         0, 0, 1,
         1, 0, 0;
    permutations.push_back(m);

    m << 0, 0, 1,
         1, 0, 0,
         0, 1, 0;
    permutations.push_back(m);

    m << 0, 0, 1,
         0, 1, 0,
         1, 0, 0;
    permutations.push_back(m);

    std::vector<Eigen::Matrix3i> rotations;

    for (auto &sign : signs) {
        for (auto &perm : permutations) {
            auto rot = sign * perm;
            rotations.emplace_back(rot);
        }
    }

    return rotations;
}

std::vector<int> findTransformPath(int goal, const std::map<int, std::set<int>> &graph) {
    std::vector<int> path;
    std::set<int> visited;

    visited.insert(0);
    path.push_back(0);

    while (!visited.empty()) {
        int next = path.back();
        if (graph.at(next).contains(goal)) {
            path.push_back(goal);
            visited.clear();
            break;
        }
        else {
            for (auto it = graph.at(next).begin(); it != graph.at(next).end(); it = std::next(it)) {
                if (!visited.contains(*it)) {
                    path.push_back(*it);
                    visited.insert(*it);
                    break;
                }
                else if (std::next(it) == graph.at(next).end()) {
                    path.pop_back();
                }
            }
        }
    }

    return path;
}

int main() {
    std::vector<Eigen::Matrix3i> rotations = getRotations();

    std::vector<Scanner> scanners;
    std::ifstream scannerFile("input_19");
    std::string line;
    std::smatch match;

    while (std::getline(scannerFile, line)) {
        if (std::regex_match(line, match, std::regex(R"(---\sscanner\s\d+\s---)"))) {
            scanners.emplace_back();
        }
        if (std::regex_match(line, match, std::regex(R"((-{0,1}\d+),(-{0,1}\d+),(-{0,1}\d+))"))) {
            int x = std::stoi(match[1]);
            int y = std::stoi(match[2]);
            int z = std::stoi(match[3]);
            auto vec = Eigen::RowVector3i(x, y, z);
            scanners.back().beacons.emplace_back(vec);
        }
    }

    for (auto &scanner : scanners) {
        for (auto &beacon : scanner.beacons) {
            for (auto &b2 : scanner.beacons) {
                if (&beacon != &b2) {
                    auto vec = beacon.pos - b2.pos;
                    double mag = vec.norm();
                    beacon.distances.emplace_back(b2.pos, mag);
                }
            }
        }
    }

    for (auto &s1 : scanners) {
        int count = 0;
        for (auto &s2 : scanners) {
            if (&s1 != &s2) {
                for (auto &b1 : s1.beacons) {
                    b1.relations.emplace_back(count, std::vector<ScannerRelation>());
                    for (auto &b2 : s2.beacons) {
                        b1.relations.back().second.emplace_back(b2.pos);
                        for (auto &d1 : b1.distances) {
                            for (auto &d2 : b2.distances) {
                                if (d1.second == d2.second && d1.second + d2.second > 0) {
                                    b1.relations.back().second.back().relations.emplace_back(d2.first);
                                }
                            }
                        }
                    }
                }
            }
            count++;
        }
    }

    for (auto &scanner : scanners) {
        for (auto &beacon : scanner.beacons) {
            for (auto &scannerRel : beacon.relations) {
                for (auto &rel : scannerRel.second) {
                    if (rel.relations.size() >= 11) {
                        beacon.matchingBeacons.emplace_back(scannerRel.first, rel.beacon);
                        scanner.matchingBeacons[scannerRel.first].emplace_back(beacon.pos, rel.beacon);
                    }
                }
            }
        }
    }

    for (auto &scanner : scanners) {
        for (auto &scannerMatch : scanner.matchingBeacons) {
            auto a1 = scannerMatch.second[0].first;
            auto a2 = scannerMatch.second[0].second;
            auto b1 = scannerMatch.second[1].first;
            auto b2 = scannerMatch.second[1].second;
            auto ab1 = a1 - b1;
            std::cout << "ab1 " << ab1 << std::endl;
            auto ab2 = a2 - b2;
            std::cout << "ab2" << ab2 << std::endl;
            for (auto &rot : rotations) {
                Eigen::RowVector3i transformed = ab2 * rot;
                if (transformed == ab1) {
                    scanner.transforms[scannerMatch.first] = std::make_pair(rot, Eigen::RowVector3i());
                    break;
                }
            }

            auto transform = std::find_if(scanner.transforms.begin(), scanner.transforms.end(), [&scannerMatch](auto &t){
                return t.first == scannerMatch.first;
            });

            auto rot = transform->second.first;

            Eigen::RowVector3i B = a2 * rot;
            Eigen::RowVector3i diff = a1 - B;

            transform->second.second = diff;


        }
    }


    int count = 0;
    for (auto &scanner : scanners) {
        for (auto &matchingScanner : scanner.matchingBeacons) {
            auto trans = scanner.transforms[matchingScanner.first];
            std::cout << trans.first << std::endl;
            std::cout << std::endl;
            std::cout << trans.second << std::endl;
            std::cout << std::endl;

            for (auto &matchingBeacon : matchingScanner.second) {
                std::cout << "Scanner: " << count << " Beacon: " << matchingBeacon.first
                << " Scanner: " << matchingScanner.first << " Beacon: " << matchingBeacon.second << std::endl;
                std::cout << "Rotation Applied: " << matchingBeacon.second * trans.first << std::endl;
                std::cout << "Transform Applied: " << matchingBeacon.second * trans.first + trans.second << std::endl;
                std::cout << std::endl;
            }
        }
        count++;
    }

    //std::cout << (scanners[1].matchingBeacons[4][0].second * scanners[1].transforms[4].first + scanners[1].transforms[4].second) *
    //scanners[0].transforms[1].first + scanners[0].transforms[1].second << std::endl;

    std::set<std::set<int>> pairs;

    count = 0;

    for (auto &scanner : scanners) {
        for (auto &scannerMatch : scanner.matchingBeacons) {
            std::set<int> pair;
            pair.insert(count);
            pair.insert(scannerMatch.first);
            pairs.insert(pair);
        }
        count++;
    }

    std::map<int, std::set<int>> nodes;

    for (auto &pair : pairs) {
        auto first = pair.begin();
        auto second = std::next(pair.begin());

        nodes[*first].insert(*second);
        nodes[*second].insert(*first);
    }

    std::map<int, std::vector<int>> paths;

    paths[0] = { 0 };

    for (int i = 1; i < scanners.size(); i++) {
        paths[i] = findTransformPath(i, nodes);
    }

    std::vector<Eigen::RowVector3i> scannerPositions;
    std::vector<Eigen::RowVector3i> transScannerPositions;

    for (auto &scanner : scanners) {
        scannerPositions.emplace_back(0, 0, 0);
    }

    for (int i = 0; i < scannerPositions.size(); i++) {
        auto scannerPos = scannerPositions[i];
        auto currentPath = paths[i];
        int previousTransform = i;
        while (currentPath.size() > 1) {
            currentPath.pop_back();
            int nextTransform = currentPath.back();
            auto trans = scanners[nextTransform].transforms[previousTransform];
            scannerPos = scannerPos * trans.first + trans.second;
            previousTransform = nextTransform;
        }
        transScannerPositions.push_back(scannerPos);
    }

    std::vector<Eigen::RowVector3i> beacons;

    for (int i = 0; i < scanners.size(); i++) {
        auto scannerBeacons = scanners[i].beacons;
        std::vector<Eigen::RowVector3i> transformBeacons;
        for (auto &beacon : scannerBeacons) {
            transformBeacons.push_back(beacon.pos);
        }

        auto currentPath = paths[i];
        int previousTransform = i;
        while (currentPath.size() > 1) {
            currentPath.pop_back();
            int nextTransform = currentPath.back();
            auto trans = scanners[nextTransform].transforms[previousTransform];
            auto transformBeaconsTemp = transformBeacons;
            transformBeacons.clear();
            for (auto &beacon : transformBeaconsTemp) {
                transformBeacons.emplace_back(beacon * trans.first + trans.second);
            }
            previousTransform = nextTransform;
        }

        for (auto &beacon : transformBeacons) {
            if (std::find(beacons.begin(), beacons.end(), beacon) == beacons.end()) {
                beacons.push_back(beacon);
            }
        }
    }

    for (auto &beacon : beacons) {
        std::cout << beacon << std::endl;
    }

    std::cout << "Number of Beacons: " << beacons.size() << std::endl;

    // part 2

    double maxManDist = 0;
    Eigen::RowVector3i maxb1;
    Eigen::RowVector3i maxb2;

    for (auto &b1 : transScannerPositions) {
        for (auto &b2 : transScannerPositions) {
            auto diff = b1 - b2;
            int manDist = diff.cwiseAbs().sum();
            if (manDist > maxManDist) {
                maxManDist = manDist;
                maxb1 = b1;
                maxb2 = b2;
            }
        }
    }

    std::cout << "Max Manhatten Distance: " << maxManDist << std::endl;
    std::cout << "Max b1" << maxb1 << std::endl;
    std::cout << "Max b2" << maxb2 << std::endl;

    return 0;
}