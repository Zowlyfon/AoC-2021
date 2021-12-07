//
// Created by zowlyfon on 07/12/2021.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

int main() {
    std::vector<int> crabSubmarines;
    std::ifstream submarinesFile("input_7");
    std::string line;
    std::string word;

    std::getline(submarinesFile, line);

    std::stringstream lineStream(line);

    while(std::getline(lineStream, word, ',')) {
        crabSubmarines.push_back(std::stoi(word));
    }

    int highestValue = 0;

    for(int crabSubmarine : crabSubmarines) {
        if(crabSubmarine > highestValue) {
            highestValue = crabSubmarine;
        }
    }

    int mostEfficientValue = 0;
    int fuelUsage = 0;
    int optimalPosition = 0;

    for(int crabSubmarine : crabSubmarines) {
            fuelUsage += crabSubmarine;
    }

    mostEfficientValue = fuelUsage;

    for(int i = 1; i <= highestValue; i++) {
        fuelUsage = 0;
        for(int crabSubmarine : crabSubmarines) {
            if (crabSubmarine <= i) {
                fuelUsage += i - crabSubmarine;
            }
            else {
                fuelUsage += crabSubmarine - i;
            }
        }
        if(fuelUsage < mostEfficientValue) {
            mostEfficientValue = fuelUsage;
            optimalPosition = i;
        }
    }

    std::cout << "Optimal Position: " << optimalPosition << std::endl;
    std::cout << "Fuel Usage: " << mostEfficientValue << std::endl;

    // part 2

    fuelUsage = 0;
    optimalPosition = 0;
    mostEfficientValue = 0;
    int diff = 0;

    for(int crabSubmarine : crabSubmarines) {
        diff = crabSubmarine;
        fuelUsage += (diff * (diff + 1)) / 2;
    }

    mostEfficientValue = fuelUsage;

    for(int i = 1; i <= highestValue; i++) {
        fuelUsage = 0;
        for(int crabSubmarine : crabSubmarines) {
            if (crabSubmarine <= i) {
                diff = i - crabSubmarine;
                fuelUsage += (diff * (diff + 1)) / 2;
            }
            else {
                diff = crabSubmarine - i;
                fuelUsage += (diff * (diff + 1)) / 2;
            }
        }
        if(fuelUsage < mostEfficientValue) {
            mostEfficientValue = fuelUsage;
            optimalPosition = i;
        }
    }

    std::cout << "Optimal Position: " << optimalPosition << std::endl;
    std::cout << "Fuel Usage: " << mostEfficientValue << std::endl;

    return 0;
}
