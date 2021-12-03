//
// Created by Zowlyfon on 03/12/2021.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int main() {
    std::ifstream diagnosticFile("input_3");
    std::vector<std::string> diagnosticNumbers;

    std::string line;

    while(std::getline(diagnosticFile, line)) {
        diagnosticNumbers.push_back(line);
    }

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

    return 0;
}