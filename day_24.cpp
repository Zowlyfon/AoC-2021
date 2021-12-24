//
// Created by zowlyfon on 24/12/2021.
//

#include <utility>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <regex>
#include <chrono>

enum opcode {
    INP,
    ADD,
    MUL,
    DIV,
    MOD,
    EQL
};

struct Instruction {
    opcode op;
    char a;
    long long int b;
    bool bIsVar;
    char bVar;
};

class ALU {
private:
    std::map<char, long long int> registers;
    std::vector<int> modelNumber;
    int modelNumberPointer = 13;
    std::vector<Instruction> instructions;

    int getImp() {
        int imp = modelNumber.back();
        modelNumber.pop_back();
        return imp;
    }

    int processInstruction(Instruction i) {
        long long b = i.b;
        if (i.bIsVar) {
            b = registers[i.bVar];
        }
        switch(i.op) {
            case INP:
                registers[i.a] = getImp();
                break;
            case ADD:
                registers[i.a] += b;
                break;
            case MUL:
                registers[i.a] *= b;
                break;
            case DIV:
                if (b != 0) {
                    registers[i.a] /= b;
                } else {
                    return -1;
                }
                break;
            case MOD:
                if (b > 0 && i.a >= 0) {
                    registers[i.a] %= b;
                }
                break;
            case EQL:
                if (registers[i.a] == b) {
                    registers[i.a] = 1;
                } else {
                    registers[i.a] = 0;
                }
                break;
            default:
                break;
        }
        return 0;
    }
public:
    ALU(std::vector<Instruction> _instructions, std::vector<int> _modelNumber) :
    instructions(std::move(_instructions)), modelNumber(std::move(_modelNumber)) {}

    long long int run() {
        int ret = 0;
        while (!instructions.empty()) {
            ret = processInstruction(instructions.back());
            if (ret != 0) {
                instructions.clear();
                return -1;
            }
            instructions.pop_back();
        }
        return registers.at('z');
    }
};

int incrementModelNumber(std::vector<int> &mn) {
    int pointer = 0;
    while (pointer < 14) {
        if (mn[pointer] < 9) {
            mn[pointer]++;
            break;
        }
        else {
            if (pointer == 13) {
                return -1;
            }
            mn[pointer] = 1;
        }
        pointer++;
    }
    return 0;
}

std::string modelNumberToString(std::vector<int> modelNumber) {
    std::string mnString;
    while (!modelNumber.empty()) {
        mnString.push_back(modelNumber.back());
        modelNumber.pop_back();
    }
    return mnString;
}

int main() {
    std::vector<Instruction> instructions;
    std::ifstream programFile("input_24");
    std::string line;
    std::smatch match;

    while(std::getline(programFile, line)) {
        if (std::regex_match(line, match, std::regex(R"((inp|add|mul|div|mod|eql)\s(\w)\s*(-{0,1}\d+|\w)*)"))) {
            char a = match[2].str()[0];
            long long int b = 0;
            char bVar = '\0';
            bool bIsVar = false;
            opcode op = INP;

            if (match[1] == "inp") {
                op = INP;
            }
            else {
                std::string bStr = match[3].str();
                std::smatch bMatch;
                if (std::regex_match(bStr, bMatch, std::regex(R"(-{0,1}\d+)"))) {
                    b = std::stoll(match[3]);
                }
                else {
                    bIsVar = true;
                    bVar = match[3].str()[0];
                }

                if (match[1] == "add") {
                    op = ADD;
                }
                else if (match[1] == "mul") {
                    op = MUL;
                }
                else if (match[1] == "div") {
                    op = DIV;
                }
                else if (match[1] == "mod") {
                    op = MOD;
                }
                else if (match[1] == "eql") {
                    op = EQL;
                }
            }
            Instruction newInstruction{op, a, b, bIsVar, bVar};
            instructions.push_back(newInstruction);
        }
        else {
            std::cout << "BAD INSTRUCTION" << std::endl;
            std::cout << line << std::endl;
        }
    }

    std::reverse(instructions.begin(), instructions.end());

    std::vector<int> modelNumber = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

    std::vector<int> largestMn = modelNumber;

    int mnRet = 0;
    while (mnRet == 0) {
        ALU alu(instructions, modelNumber);
        long long int ret = alu.run();
        if (ret == 0) {
            largestMn = modelNumber;
            std::cout << modelNumberToString(largestMn);
        }
        mnRet = incrementModelNumber(modelNumber);
    }


    return 0;
}