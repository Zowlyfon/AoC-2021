//
// Created by Zowlyfon on 03/12/2021.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

void countBits(int bit, int *onBits, int *offBits, std::vector<std::string> *data) {
    *onBits = 0;
    *offBits = 0;
    for(std::string element : *data) {
        if (element[bit] == '1') {
            (*onBits)++;
        }
        else {
            (*offBits)++;
        }
    }
}

int main() {
    std::ifstream diagnosticFile("input_3");
    std::vector<std::string> diagnosticNumbers;

    std::string line;

    while(std::getline(diagnosticFile, line)) {
        diagnosticNumbers.push_back(line);
    }

    std::cout << "DiagnosticNumbers Count: " << diagnosticNumbers.size() << std::endl;

    int bitCount[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int count = 0;

    for(std::string number : diagnosticNumbers) {
        for(int i = 0; i < 12; i++) {
            bitCount[i] += number[i] - '0';
        }
        count++;
    }

    std::string gammaBin;
    std::string epsilonBin;
    int threshold = count / 2;

    std::cout << "Threshold: " << threshold << std::endl;

    for(int i = 0; i < 12; i++) {
        if(bitCount[i] >= threshold) {
            gammaBin.push_back('1');
            epsilonBin.push_back('0');
        }
        else {
            gammaBin.push_back('0');
            epsilonBin.push_back('1');
        }
    }

    int gamma = std::stoi(gammaBin, nullptr, 2);
    int epsilon = std::stoi(epsilonBin, nullptr, 2);
    int result = gamma * epsilon;

    std::cout << "Gamma: " << gamma << std::endl;
    std::cout << "Epsilon: " << epsilon << std::endl;
    std::cout << "Result: " << result << std::endl;

    // part 2

    int onBitCount = 0;
    int offBitCount = 0;
    char compBit;

    std::vector<std::string> oxygenNumbers(diagnosticNumbers);
    std::vector<std::string> co2Numbers(diagnosticNumbers);

    std::vector<std::string> tempNumbers;

    int bit = 0;

    while(oxygenNumbers.size() > 1) {
        countBits(bit, &onBitCount, &offBitCount, &oxygenNumbers);

        if (onBitCount >= offBitCount) {
            compBit = '1';
        }
        else {
            compBit = '0';
        }

        std::copy_if(oxygenNumbers.begin(), oxygenNumbers.end(), std::back_inserter(tempNumbers),
                     [&bit, &compBit](std::string number){
            return number[bit] == compBit;
        });

        oxygenNumbers = tempNumbers;
        tempNumbers.clear();
        bit++;
    }

    bit = 0;

    while(co2Numbers.size() > 1) {
        countBits(bit, &onBitCount, &offBitCount, &co2Numbers);

        if (offBitCount <= onBitCount) {
            compBit = '0';
        }
        else {
            compBit = '1';
        }

        std::copy_if(co2Numbers.begin(), co2Numbers.end(), std::back_inserter(tempNumbers),
                     [&bit, &compBit](std::string number){
                         return number[bit] == compBit;
                     });

        co2Numbers = tempNumbers;
        tempNumbers.clear();
        bit++;
    }

    int oxygenNumber = std::stoi(oxygenNumbers.front(), nullptr, 2);
    int co2Number = std::stoi(co2Numbers.front(), nullptr, 2);

    std::cout << "Oxygen: " << oxygenNumber << std::endl;
    std::cout << "CO2: " << co2Number << std::endl;
    std::cout << "Life Support: " << oxygenNumber * co2Number << std::endl;

    return 0;
}