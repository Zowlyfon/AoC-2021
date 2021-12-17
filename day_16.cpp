//
// Created by Zowlyfon on 16/12/2021.
//

#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <string>
#include <bitset>
#include <numeric>

struct Packet {
    Packet(int _version, int _type) : version(_version), type(_type) {}
    int version;
    int type;
    bool lengthType = false;
    unsigned long long int value = 0;
    std::vector<Packet> subPackets;
};

std::string::iterator parseLiteral(std::string::iterator it, Packet &packet);
std::string::iterator parseOperator(std::string::iterator it, Packet &packet);
unsigned long long processPacket(const Packet &packet);

Packet parsePacket(std::string::iterator it) {
    std::string versionS(it, std::next(it, 3));
    int version = std::stoi(versionS, nullptr, 2);
    it = std::next(it, 3);

    std::string typeS(it, std::next(it, 3));
    int type = std::stoi(typeS, nullptr, 2);
    it = std::next(it, 3);

    Packet packet(version, type);

    if (type == 4) {
        it = parseLiteral(it, packet);
    } else {
        it = parseOperator(it, packet);
    }
    return packet;
}

std::string::iterator parseSubPacket(std::string::iterator it, Packet &packet) {
    std::string versionS(it, std::next(it, 3));
    int version = std::stoi(versionS, nullptr, 2);
    it = std::next(it, 3);

    std::string typeS(it, std::next(it, 3));
    int type = std::stoi(typeS, nullptr, 2);
    it = std::next(it, 3);

    Packet subPacket(version, type);

    if (type == 4) {
        it = parseLiteral(it, subPacket);
    } else {
        it = parseOperator(it, subPacket);
    }

    packet.subPackets.push_back(subPacket);
    return it;
}

std::string::iterator parseLiteral(std::string::iterator it, Packet &packet) {
    std::string literal;
    bool lastNibble = false;
    while (!lastNibble) {
        if (*it == '0') {
            lastNibble = true;
        }
        it = std::next(it);
        literal.insert(literal.end(), it, std::next(it, 4));
        it = std::next(it, 4);
    }
    packet.value = std::stoull(literal, nullptr, 2);
    return it;
}

std::string::iterator parseOperator(std::string::iterator it, Packet &packet) {
    if (*it == '0') {
        it = std::next(it);
        int numBits = std::stoi(std::string(it, std::next(it, 15)), nullptr, 2);
        packet.value = numBits;
        it = std::next(it, 15);
        auto it2 = parseSubPacket(it, packet);
        while (std::distance(it, it2) < packet.value) {
            auto distance = std::distance(it, it2);
            it2 = parseSubPacket(it2, packet);
        }
        it = it2;
        return it;
    }
    else {
        packet.lengthType = true;
        it = std::next(it);
        int numPackets = std::stoi(std::string(it, std::next(it, 11)), nullptr, 2);
        packet.value = numPackets;
        it = std::next(it, 11);
        for (int i = 0; i < numPackets; i++) {
            it = parseSubPacket(it, packet);
        }
        return it;
    }
}

unsigned long long processPacket(const Packet &packet) {
    /*
    if (packet.type == 0) {
        return std::accumulate(packet.subPackets.begin(), packet.subPackets.end(), 0,
                               [](unsigned long long acc, const Packet &subPacket){
                                    auto result = acc += processPacket(subPacket);
                                   return result;
                               });
    }
    else if (packet.type == 1) {
        return std::accumulate(packet.subPackets.begin(), packet.subPackets.end(), 1,
                               [](unsigned long long acc, const Packet &subPacket){
                                    auto result = acc *= processPacket(subPacket);
                                    return result;
                               });
    }
    else if (packet.type == 2) {
        return std::accumulate(std::next(packet.subPackets.begin()), packet.subPackets.end(),
                               processPacket(packet.subPackets[0]),
                               [](unsigned long long min, const Packet &subPacket){
                                    auto b = processPacket(subPacket);
                                    return min > b ? b : min;
                               });
    }
    else if (packet.type == 3) {
        return std::accumulate(packet.subPackets.begin(), packet.subPackets.end(),0,
                               [](unsigned long long max, const Packet &subPacket){
                                   auto b = processPacket(subPacket);
                                   return max < b ? b : max;
                               });
                               */
    if (packet.type == 0) {
        unsigned long long int acc = 0;
        for (auto &subPacket : packet.subPackets) {
            acc += processPacket(subPacket);
        }
        return acc;
    }
    else if (packet.type == 1) {
        unsigned long long int acc = 1;
        for (auto &subPacket : packet.subPackets) {
            acc *= processPacket(subPacket);
        }
        return acc;
    }
    else if (packet.type == 2) {
        unsigned long long int min = processPacket(packet.subPackets[0]);
        for (auto &subPacket : packet.subPackets) {
            auto a = processPacket(subPacket);
            if (a < min) {
                min = a;
            }
        }
        return min;
    }
    else if (packet.type == 3) {
        unsigned long long int max = processPacket(packet.subPackets[0]);
        for (auto &subPacket : packet.subPackets) {
            auto a = processPacket(subPacket);
            if (a > max) {
                max = a;
            }
        }
        return max;
    }
    else if (packet.type == 4) {
        return packet.value;
    }
    else if (packet.type == 5) {
        auto a = processPacket(packet.subPackets[0]);
        auto b = processPacket(packet.subPackets[1]);
        if (a > b) {
            return 1;
        }
        return 0;
    }
    else if (packet.type == 6) {
        if (processPacket(packet.subPackets[0]) < processPacket(packet.subPackets[1])) {
            return 1;
        }
        return 0;
    }
    else if (packet.type == 7) {
        if (processPacket(packet.subPackets[0]) == processPacket(packet.subPackets[1])) {
            return 1;
        }
        return 0;
    }
    else {
        return 0;
    }
}

int calculateVersionSum(const Packet &packet, int versionSum) {
    versionSum += packet.version;

    if (packet.subPackets.empty()) {
        return versionSum;
    }

    for (auto &subPacket : packet.subPackets) {
        versionSum = calculateVersionSum(subPacket, versionSum);
    }

    return versionSum;
}

int main() {
    std::string data;
    std::ifstream dataFile("input_16");
    std::string line;

    std::getline(dataFile, line);

    for (auto &hex : line) {
        unsigned long long int hexI = std::stoi(std::string(1, hex), nullptr, 16);
        std::bitset<4> bit {hexI};
        std::string bitS = bit.to_string();
        data.insert(data.end(), bitS.begin(), bitS.end());
    }

    auto it = data.begin();
    int versionSum = 0;

    Packet packet = parsePacket(it);

    versionSum = calculateVersionSum(packet, versionSum);

    std::cout << "Version Sum: " << versionSum << std::endl;

    unsigned long long int result = processPacket(packet);

    std::cout << "Result: " << result << std::endl;

    return 0;
}