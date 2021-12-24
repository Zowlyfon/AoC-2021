//
// Created by zowlyfon on 22/12/2021.
//

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <regex>
#include <chrono>

class DeterministicDie {
private:
    int nextRollValue = 1;
    int numberOfRolls = 0;
public:
    DeterministicDie() {}
    int roll() {
        int result = nextRollValue;
        if (nextRollValue == 100) {
            nextRollValue = 1;
        }
        else {
            nextRollValue++;
        }
        numberOfRolls++;
        return result;
    }
    int numRolls() {
        return numberOfRolls;
    }
};

struct Player {
    Player(int _position, int _score) : position(_position), score(_score), states(std::map<int, std::map<unsigned long long int, unsigned long long int>>()) {}
    int position;
    unsigned long long int score;
    std::map<int, std::map<unsigned long long int, unsigned long long int>> states;;
};

int main() {
    DeterministicDie die;
    std::vector<Player> players;

    players.emplace_back(9, 0);
    players.emplace_back(0, 0);

    bool gameWon = false;

    while(!gameWon) {
        for (auto &player : players) {
            int roll = die.roll() + die.roll() + die.roll();
            int position = (player.position + roll) % 10;
            player.position = position;
            player.score += position + 1;

            if (player.score >= 1000) {
                gameWon = true;
                break;
            }
        }
    }

    std::sort(players.begin(), players.end(), [](auto a, auto b){ return a.score > b.score; });
    int numRolls = die.numRolls();

    std::cout << "High Score * Rolls: " << players.back().score * die.numRolls() << std::endl;

    // part 2
    const auto startT = std::chrono::high_resolution_clock::now();
    // 3: 1, 4: 3, 5: 6, 6: 7, 7: 6, 8: 3, 9: 1
    std::vector<std::pair<int, int>> rolls {
        std::make_pair(3, 1),
        std::make_pair(4, 3),
        std::make_pair(5, 6),
        std::make_pair(6, 7),
        std::make_pair(7, 6),
        std::make_pair(8, 3),
        std::make_pair(9, 1)
    };

    std::vector<unsigned long long int> universesWon = {0, 0};

    // p1 pos, p1 score, p2 pos, p2 score, count;
    std::map<int, std::map<int, std::map<int, std::map<int, unsigned long long int>>>> states;
    states[9][0][0][0] = 1;

    while (!states.empty()) {
        std::map<int, std::map<int, std::map<int, std::map<int, unsigned long long int>>>> newStates;
        for (auto &p1Pos : states) {
            for (auto &p1Score : p1Pos.second) {
                for (auto &p2Pos : p1Score.second) {
                    for (auto &p2Score : p2Pos.second) {
                        for (auto &r1 : rolls) {
                            int newP1Pos = (p1Pos.first + r1.first) % 10;
                            int newP1Score = p1Score.first + newP1Pos + 1;
                            if (newP1Score >= 21) {
                                universesWon[0] += p2Score.second * r1.second;
                            }
                            else {
                                for (auto &r2 : rolls) {
                                    int newP2Pos = (p2Pos.first + r2.first) % 10;
                                    int newP2Score = p2Score.first + newP2Pos + 1;
                                    if (newP2Score >= 21) {
                                        universesWon[1] += p2Score.second * r1.second * r2.second;
                                    }
                                    else {
                                        newStates[newP1Pos][newP1Score][newP2Pos][newP2Score] += p2Score.second * r1.second * r2.second;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        states = newStates;
    }
    const auto endT = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(endT - startT).count() << std::endl;

    std::cout << "Player 1 Universes Won: " << universesWon[0] << std::endl;
    std::cout << "Player 2 Universes Won: " << universesWon[1] << std::endl;

    return 0;
}
