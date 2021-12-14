//
// Created by zowlyfon on 05/12/2021.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <regex>
#include <sstream>

struct bingoNumber {
    int value = 0;
    bool marked = false;
};

struct bingoBoard {
    bingoNumber bingoNumbers[5][5];
    bool bingo = false;
};

struct bingoBoardWin {
    bingoBoard board;
    int call = 0;
};

int main() {
    std::vector<int> bingoCalls;
    std::vector<bingoBoard> bingoBoards;
    std::ifstream bingoFile("input_4");
    std::string line;
    std::string word;

    std::getline(bingoFile, line);

    std::stringstream lineStream(line);

    while(std::getline(lineStream, word, ',')) {
        bingoCalls.push_back(std::stoi(word));
    }

    int row = 0;

    std::smatch match;

    while(std::getline(bingoFile, line)) {
        if(std::regex_match(line, match, std::regex(R"(\s*(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+))"))) {
            if(row == 0) {
                bingoBoard newBingoBoard;
                bingoBoards.push_back(newBingoBoard);
            }
            for (int i = 1; i < 6; i++) {
                bingoBoards.back().bingoNumbers[row][i - 1].value = std::stoi(match[i]);
            }

            row++;

            if(row >= 5) {
                row = 0;
            }
        }
    }

    std::vector<bingoBoardWin> bingoBoardWins;
    int index = 0;

    for(int bingoCall : bingoCalls) {
        index = 0;
        for (bingoBoard &board : bingoBoards)
        {
            for(int i = 0; i < 5; i++) {
                for(int j = 0; j < 5; j++) {
                    if (board.bingoNumbers[i][j].value == bingoCall) {
                        board.bingoNumbers[i][j].marked = true;
                    }
                }
            }

            for(int i = 0; i < 5; i++) {
                if(!board.bingo &&
                    board.bingoNumbers[i][0].marked &&
                    board.bingoNumbers[i][1].marked &&
                    board.bingoNumbers[i][2].marked &&
                    board.bingoNumbers[i][3].marked &&
                    board.bingoNumbers[i][4].marked) {
                    board.bingo = true;
                    bingoBoardWin thisWin;
                    thisWin.board = board;
                    thisWin.call = bingoCall;
                    bingoBoardWins.push_back(thisWin);
                    //bingoBoards.erase(std::next(bingoBoards.begin(), index));
                    //index--;
                    break;
                }
                else if(!board.bingo &&
                    board.bingoNumbers[0][i].marked &&
                    board.bingoNumbers[1][i].marked &&
                    board.bingoNumbers[2][i].marked &&
                    board.bingoNumbers[3][i].marked &&
                    board.bingoNumbers[4][i].marked) {
                    board.bingo = true;
                    bingoBoardWin thisWin;
                    thisWin.board = board;
                    thisWin.call = bingoCall;
                    bingoBoardWins.push_back(thisWin);
                    //bingoBoards.erase(std::next(bingoBoards.begin(), index));
                    //index--;
                    break;
                }
            }
            index++;
        }
    }

    // calculate score

    int winScore = 0;
    int loseScore = 0;

    bingoBoardWin winningBoard = bingoBoardWins.front();
    bingoBoardWin losingBoard = bingoBoardWins.back();

    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            if (!winningBoard.board.bingoNumbers[i][j].marked) {
                winScore += winningBoard.board.bingoNumbers[i][j].value;
            }

            if (!losingBoard.board.bingoNumbers[i][j].marked) {
                loseScore += losingBoard.board.bingoNumbers[i][j].value;
            }
        }
    }

    winScore *= winningBoard.call;
    loseScore *= losingBoard.call;

    std::cout << "Win Score: " << winScore << std::endl;
    std::cout << "Lose Score: " << loseScore << std::endl;

    return 0;
}