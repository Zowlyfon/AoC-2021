//
// Created by Zowlyfon on 02/12/2021.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int main() {
    std::ifstream depthMeasurementsFile("input_1");
    std::vector<int> depthMeasurements;
    std::string line;
    while(std::getline(depthMeasurementsFile, line)) {
        int measurement = std::stoi(line);
        depthMeasurements.push_back(measurement);
    }

    int increments = 0;
    int lastMeasurement = depthMeasurements.front();

    for(int measurement : depthMeasurements) {
        if (measurement > lastMeasurement)
        {
            increments++;
        }
        lastMeasurement = measurement;
    }

    std::cout << increments << std::endl;

    // part 2

    int bracketSum = 0;
    increments = 0;

    for(int i = 0; i <= 2; i++) {
        bracketSum += depthMeasurements[i];
    }

    lastMeasurement = bracketSum;

    for(int i = 1; i < (depthMeasurements.size() - 2); i++)
    {
        bracketSum = 0;
        for (int j = 0; j <= 2; j++) {
            bracketSum += depthMeasurements[i + j];
        }
        if(bracketSum > lastMeasurement)
        {
            increments++;
        }
        lastMeasurement = bracketSum;
    }

    std::cout << increments << std::endl;
}
