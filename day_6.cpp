//
// Created by zowlyfon on 06/12/2021.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

int main() {
    std::ifstream fishFile("input_6");

    std::string line;
    std::string word;

    std::getline(fishFile, line);

    std::stringstream lineStream(line);

    unsigned long int lanternFish[9] = {0};

    while(std::getline(lineStream, word, ',')) {
        int days = std::stoi(word);
        if (days <= 8) {
            lanternFish[days]++;
        }
    }

    unsigned long int tempFish[9] = {0};

    for(int i = 0; i < 80; i++) {
        for(int j = 8; j > 0; j--) {
            tempFish[j - 1] = lanternFish[j];
        }

        tempFish[6] += lanternFish[0];
        tempFish[8] += lanternFish[0];

        for(int j = 0; j <= 8; j++) {
            lanternFish[j] = tempFish[j];
            tempFish[j] = 0;
        }
    }

    unsigned long int numFish = 0;

    for(int i = 0; i <= 8; i++) {
        numFish += lanternFish[i];
    }

    std::cout << "Number of Fish Day 80: " << numFish << std::endl;

    // part 2

    for(int i = 80; i < 256; i++) {
        for(int j = 8; j > 0; j--) {
            tempFish[j - 1] = lanternFish[j];
        }

        tempFish[6] += lanternFish[0];
        tempFish[8] += lanternFish[0];

        for(int j = 0; j <= 8; j++) {
            lanternFish[j] = tempFish[j];
            tempFish[j] = 0;
        }
    }

    numFish = 0;

    for(int i = 0; i <= 8; i++) {
        numFish += lanternFish[i];
    }

    std::cout << "Number of Fish Day 256: " << numFish << std::endl;
    return 0;
}