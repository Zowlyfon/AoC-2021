//
// Created by Zowlyfon on 08/12/2021.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <regex>
#include <sstream>

struct Signal {
    std::vector<std::string> inputString;
    std::vector<std::string> outputString;
    // <input, length>
    std::vector<std::pair<int, int>> inputs;
    std::vector<int> outputs;
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
    int g;
};
// a = 1, b = 2, c = 4, d = 8, e = 16, f = 32, g = 64

bool isEasyNumber(const std::string& number) {
    return number.length() == 2 || number.length() == 4 || number.length() == 3 || number.length() == 7;
}

int letterToNumber(char letter) {
    switch(letter) {
        case 'a':
            return 1;
        case 'b':
            return 2;
        case 'c':
            return 4;
        case 'd':
            return 8;
        case 'e':
            return 16;
        case 'f':
            return 32;
        case 'g':
            return 64;
        default:
            return 0;
    }
}

char signalToNumber(const Signal &signal, int number) {
    if (number == (signal.a + signal.b + signal.c + signal.e + signal.f + signal.g))
        return '0';
    if (number == (signal.c + signal.f))
        return '1';
    if (number == (signal.a + signal.c + signal.d + signal.e + signal.g))
        return '2';
    if (number == (signal.a + signal.c + signal.d + signal.f + signal.g))
        return '3';
    if (number == (signal.b + signal.c + signal.d + signal.f))
        return '4';
    if (number == (signal.a + signal.b + signal.d + signal.f + signal.g))
        return '5';
    if (number == (signal.a + signal.b + signal.d + signal.e + signal.f + signal.g))
        return '6';
    if (number == (signal.a + signal.c + signal.f))
        return '7';
    if (number == 127)
        return '8';
    if (number == (signal.a + signal.b + signal.c + signal.d + signal.f + signal.g))
        return '9';

    return '\0';
}
int main() {
    std::vector<Signal> signals;
    std::ifstream signalFile("input_8");
    std::string line;
    std::smatch match;
    std::string word;

    while(std::getline(signalFile, line)) {
        if(std::regex_match(line, match, std::regex(R"(([\w\s]+)\s\|\s([\w\s]+))"))) {
            std::stringstream inputs(match[1]);
            std::stringstream outputs(match[2]);
            Signal newSignal;
            while(std::getline(inputs, word, ' ')) {
                int input = 0;
                for (auto &letter : word) {
                    input += letterToNumber(letter);
                }
                newSignal.inputs.emplace_back(input, (int)word.length());
                newSignal.inputString.push_back(word);
            }
            while(std::getline(outputs, word, ' ')) {
                int output = 0;
                for (auto &letter : word) {
                    output += letterToNumber(letter);
                }
                newSignal.outputs.push_back(output);
                newSignal.outputString.push_back(word);
            }
            signals.push_back(newSignal);
        }
    }

    int easyNumberCount = 0;

    for(auto &signal : signals) {
        for (auto &output : signal.outputString) {
            if(isEasyNumber(output)) {
                easyNumberCount++;
            }
        }
    }

    std::cout << "Easy Numbers: " << easyNumberCount << std::endl;

    // part 2

    std::vector<int> outputNumbers;

    for (auto &signal : signals) {
        int one;
        int two;
        int four;
        int five;
        int six;
        int seven;
        int eight;

        for(auto &input : signal.inputs) {
            if (input.second == 2)
                one = input.first;
            if (input.second == 3)
                seven = input.first;
            if (input.second == 4)
                four = input.first;
            if (input.second == 7)
                eight = input.first;
        }

        signal.c = one;
        signal.f = one;

        signal.a = seven - one;

        signal.b = four - one;
        signal.d = four - one;

        signal.e = eight - (seven | four);
        signal.g = eight - (seven | four);

        for(auto &input : signal.inputs) {
            if (input.second == 6) {
                if((input.first & one) != one) {
                    six = input.first;
                }
            }
        }

        signal.c = (one & six) ^ one;
        signal.f = one - signal.c;


        for(auto &input : signal.inputs) {
            if(input.second == 5){
                if(input.first & signal.c && !(input.first & signal.f)) {
                    two = input.first;
                }
                if(input.first & signal.f && !(input.first & signal.c)){
                    five = input.first;
                }
            }
        }

        signal.d = signal.d & two;
        signal.b = signal.b - signal.d;

        signal.g = signal.g & five;
        signal.e = signal.e - signal.g;

        std::string outputString;
        int outputNumber;

        for(auto &output : signal.outputs) {
            outputString.push_back(signalToNumber(signal, output));
        }

        outputNumber = std::stoi(outputString);

        outputNumbers.push_back(outputNumber);
    }

    int total = 0;

    for(auto &number : outputNumbers) {
        total += number;
    }

    std::cout << "Total Sum: " << total << std::endl;


    return 0;
}