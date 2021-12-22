//
// Created by zowlyfon on 22/12/2021.
//

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

int getEnhancementPos(const std::vector<std::vector<int>> &inputImage, int x, int y, int oob) {
    std::vector<std::pair<int, int>> positions = {
            std::make_pair(x - 1, y - 1),
            std::make_pair(x, y - 1),
            std::make_pair(x + 1, y - 1),
            std::make_pair(x - 1, y),
            std::make_pair(x, y),
            std::make_pair(x + 1, y),
            std::make_pair(x - 1, y + 1),
            std::make_pair(x, y + 1),
            std::make_pair(x + 1, y + 1)
    };

    std::string posString;
    int xMax = inputImage.back().size();
    int yMax = inputImage.size();

    for (auto &pos : positions) {
        if ((pos.first >= 0 && pos.first < xMax) &&
                (pos.second >= 0 && pos.second < yMax)) {
            posString.push_back(std::to_string(inputImage[pos.second][pos.first])[0]);
        }
        else {
            posString.push_back(std::to_string(oob)[0]);
        }
    }

    return std::stoi(posString, nullptr, 2);
}

std::vector<std::vector<int>> processImage(const std::vector<std::vector<int>> &inputImage, const std::vector<int> &enhancement, int oob) {
    std::vector<std::vector<int>> processedImage;

    int ySize = inputImage.size();
    int xSize = inputImage.back().size();
    for (int y = -1; y <= ySize; y++) {
        processedImage.emplace_back(std::vector<int>());
        for (int x = -1; x <= xSize; x++) {
            int enhancementPos = getEnhancementPos(inputImage, x, y, oob);
            processedImage.back().push_back(enhancement[enhancementPos]);
        }
    }

    return processedImage;
}

int sumImage(const std::vector<std::vector<int>> &inputImage) {
    int sum = 0;
    for (auto &row : inputImage) {
        for (auto &value : row) {
            sum += value;
        }
    }
    return sum;
}

int main() {
    std::vector<int> enhancement;
    std::vector<std::vector<int>> inputImage;
    std::ifstream inputFile("input_20");
    std::string line;

    std::getline(inputFile, line);

    for (auto &input : line) {
        if (input == '.') {
            enhancement.push_back(0);
        }
        else if (input == '#') {
            enhancement.push_back(1);
        }
    }

    std::getline(inputFile, line);

    while(std::getline(inputFile, line)) {
        inputImage.emplace_back(std::vector<int>());
        for (auto &input : line) {
            if (input == '.') {
                inputImage.back().push_back(0);
            }
            else if (input == '#') {
                inputImage.back().push_back(1);
            }
        }
    }

    int oob = 0;

    for (int i = 0; i < 2; i++) {
        auto processedImage = processImage(inputImage, enhancement, oob);
        inputImage = processedImage;
        if (oob) {
            oob = enhancement[511];
        } else {
            oob = enhancement[0];
        }
    }

    int sum = sumImage(inputImage);

    std::cout << "Light Pixels: " << sum << std::endl;

    // part 2

    for (int i = 2; i < 50; i++) {
        auto processedImage = processImage(inputImage, enhancement, oob);
        inputImage = processedImage;
        if (oob) {
            oob = enhancement[511];
        } else {
            oob = enhancement[0];
        }
    }

    sum = sumImage(inputImage);

    std::cout << "Light Pixels: " << sum << std::endl;

    return 0;
}
