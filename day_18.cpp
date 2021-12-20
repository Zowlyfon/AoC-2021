//
// Created by zowlyfon on 18/12/2021.
//

#include <vector>
#include <string>
#include <fstream>
#include <regex>
#include <iostream>
#include <boost/variant.hpp>

struct SnailNumber {
    boost::variant<int, boost::recursive_wrapper<SnailNumber>> left;
    boost::variant<int, boost::recursive_wrapper<SnailNumber>> right;
};

class SnailVisitor {
public:
    int operator()(int i) {
        return i;
    }
    SnailNumber operator()(SnailNumber s) {
        return s;
    }
};

SnailNumber parseSnailNumber(const std::string& input) {
    SnailNumber snailNumber;
    auto it = input.begin();
    it = std::next(it);
    if (*it == '[') {
        int brackets = 1;
        auto it2 = std::next(it);
        while (brackets > 0) {
            if (*it2 == ']') {
                brackets--;
            }
            else if (*it2 == '[') {
                brackets++;
            }
            it2 = std::next(it2);
        }
        std::string leftInput(it, it2);
        snailNumber.left = parseSnailNumber(leftInput);
        it = std::next(it2, -1);
    }
    else {
        snailNumber.left = *it - '0';
    }

    it = std::next(it, 2);

    if (*it == '[') {
        int brackets = 1;
        auto it2 = std::next(it);
        while (brackets > 0) {
            if (*it2 == ']') {
                brackets--;
            }
            else if (*it2 == '[') {
                brackets++;
            }
            it2 = std::next(it2);
        }
        std::string rightInput(it, it2);
        snailNumber.right = parseSnailNumber(rightInput);
        it = it2;
    }
    else {
        snailNumber.right = *it - '0';
    }

    return snailNumber;
}

std::string snailToString(const SnailNumber& snailNumber) {
    std::string output;
    output.push_back('[');

    std::string leftString;

    switch(snailNumber.left.which()){
        case 1:
            leftString = snailToString(boost::get<SnailNumber>(snailNumber.left));
            output.insert(output.end(), leftString.begin(), leftString.end());
            break;
        case 0:
            leftString = std::to_string(boost::get<int>(snailNumber.left));
            output.insert(output.end(), leftString.begin(), leftString.end());
            break;
    }

    output.push_back(',');

    std::string rightString;

    switch(snailNumber.right.which()){
        case 1:
            rightString = snailToString(boost::get<SnailNumber>(snailNumber.right));
            output.insert(output.end(), rightString.begin(), rightString.end());
            break;
        case 0:
            rightString = std::to_string(boost::get<int>(snailNumber.right));
            output.insert(output.end(), rightString.begin(), rightString.end());
            break;
    }

    output.push_back(']');

    return output;
}

SnailNumber* getParent(SnailNumber &snailNumber, std::vector<bool> tree) {
    std::reverse(tree.begin(), tree.end());
    SnailNumber *returnNumber = &snailNumber;
    while (!tree.empty()) {
        if (tree.back()) {
            returnNumber = &boost::get<SnailNumber>(returnNumber->right);
        }
        else {
            returnNumber = &boost::get<SnailNumber>(returnNumber->left);
        }
        tree.pop_back();
    }
    return returnNumber;
}

void implodeRight(SnailNumber &snailNumber, int explode) {
    if (snailNumber.right.which()) {
        implodeRight(boost::get<SnailNumber>(snailNumber.right), explode);
    }
    else {
        snailNumber.right = boost::get<int>(snailNumber.right) + explode;
    }
}

void implodeLeft(SnailNumber &snailNumber, int explode) {
    if (snailNumber.left.which()) {
        implodeLeft(boost::get<SnailNumber>(snailNumber.left), explode);
    }
    else {
        snailNumber.left = boost::get<int>(snailNumber.left) + explode;
    }
}

void explodeLeft(SnailNumber &root, SnailNumber &snailNumber, int explode, std::vector<bool> tree) {
    bool hasParent = !tree.empty();
    if (hasParent) {
        bool parentRight = tree.back();
        tree.pop_back();
        auto parent = getParent(root, tree);
        if (parentRight) {
            if (parent->left.which()) {
                implodeRight(boost::get<SnailNumber>(parent->left), explode);
            }
            else {
                parent->left = boost::get<int>(parent->left) + explode;
            }
        }
        else {
            explodeLeft(root, *parent, explode, tree);
        }
    }
}

void explodeRight(SnailNumber &root, SnailNumber &snailNumber, int explode, std::vector<bool> tree) {
    bool hasParent = !tree.empty();
    if (hasParent) {
        bool parentRight = tree.back();
        tree.pop_back();
        auto parent = getParent(root, tree);
        if (parentRight) {
            explodeRight(root, *parent, explode, tree);
        } else {
            if (parent->right.which()) {
                implodeLeft(boost::get<SnailNumber>(parent->right), explode);
            }
            else {
                parent->right = boost::get<int>(parent->right) + explode;
            }

        }
    }
}

void explode(SnailNumber &root, SnailNumber &snailNumber, std::vector<bool> tree) {
    std::cout << "Explode: ";
    bool parentRight = tree.back();
    tree.pop_back();
    int leftValue = boost::get<int>(snailNumber.left);
    int rightValue = boost::get<int>(snailNumber.right);
    auto parent = getParent(root, tree);

    if (parentRight) {
        explodeRight(root, snailNumber, boost::get<int>(snailNumber.right), tree);

        if (parent->left.which()) {
            implodeRight(boost::get<SnailNumber>(parent->left), boost::get<int>(snailNumber.right));
        }
        else {
            parent->left = boost::get<int>(parent->left) + boost::get<int>(snailNumber.left);
        }
        parent->right = 0;
    }
    else {
        explodeLeft(root, snailNumber, boost::get<int>(snailNumber.left), tree);

        if (parent->right.which()) {
            implodeLeft(boost::get<SnailNumber>(parent->right), boost::get<int>(snailNumber.right));
        }
        else {
            parent->right = boost::get<int>(parent->right) + boost::get<int>(snailNumber.right);
        }
        parent->left = 0;
    }
}

void split(SnailNumber &snailNumber, bool splitRight) {
    std::cout << "Split:   ";
    SnailNumber newNumber;
    int splitNumber;
    if (splitRight) {
        splitNumber = boost::get<int>(snailNumber.right);
    } else {
        splitNumber = boost::get<int>(snailNumber.left);
    }

    int newLeft = splitNumber / 2;
    int newRight = (splitNumber / 2) + (splitNumber % 2);
    newNumber.left = newLeft;
    newNumber.right = newRight;

    if (splitRight) {
        snailNumber.right = newNumber;
    }
    else {
        snailNumber.left = newNumber;
    }
}


int traverseExplode(SnailNumber &root, SnailNumber &snailNumber, int depth = 0, std::vector<bool> tree = std::vector<bool>(),
        std::pair<bool, bool> routeTaken = std::make_pair(false, false)) {
    if (routeTaken.first) {
        tree.push_back(routeTaken.second);
    }

    int leftRet = 0;
    int rightRet = 0;
    if (depth == 4) {
        explode(root, snailNumber, tree);
        return 1;
    }
    if (snailNumber.left.which()) {
        leftRet = traverseExplode(root, boost::get<SnailNumber>(snailNumber.left), depth + 1, tree,
                           std::make_pair(true, false));
    }
    if (snailNumber.right.which() && !leftRet) {
        rightRet = traverseExplode(root, boost::get<SnailNumber>(snailNumber.right), depth + 1, tree,
                            std::make_pair(true, true));
    }

    return leftRet + rightRet;
}

int traverseSplit(SnailNumber &root, SnailNumber &snailNumber, int depth = 0, std::vector<bool> tree = std::vector<bool>(),
             std::pair<bool, bool> routeTaken = std::make_pair(false, false)) {
    if (routeTaken.first) {
        tree.push_back(routeTaken.second);
    }

    int leftRet = 0;
    int rightRet = 0;
    if (snailNumber.left.which()) {
        leftRet = traverseSplit(root, boost::get<SnailNumber>(snailNumber.left), depth + 1, tree,
                           std::make_pair(true, false));
    }
    else {
        int left = boost::get<int>(snailNumber.left);
        if (left > 9) {
            split(snailNumber, false);
            return 1;
        }
    }
    if (snailNumber.right.which() && !leftRet) {
        rightRet = traverseSplit(root, boost::get<SnailNumber>(snailNumber.right), depth + 1, tree,
                            std::make_pair(true, true));
    }
    else if (!leftRet){
        int right = boost::get<int>(snailNumber.right);
        if (right > 9) {
            split(snailNumber, true);
            return 1;
        }
    }

    return leftRet + rightRet;
}

unsigned long long int magnitude(SnailNumber &snailNumber) {
    unsigned long long int left, right;
    if (snailNumber.left.which()) {
        left = magnitude(boost::get<SnailNumber>(snailNumber.left));
    }
    else {
        left = boost::get<int>(snailNumber.left);
    }

    if (snailNumber.right.which()) {
        right = magnitude(boost::get<SnailNumber>(snailNumber.right));
    }
    else {
        right = boost::get<int>(snailNumber.right);
    }

    return (left * 3) + (right * 2);
}

int main() {
    std::ifstream snailNumberFile("input_18");
    std::string line;
    std::vector<SnailNumber> snailNumbers;

    while(std::getline(snailNumberFile, line)) {
        SnailNumber snailNumber = parseSnailNumber(line);
        snailNumbers.push_back(snailNumber);
    }

    auto sum = snailNumbers.front();

    for (auto it = std::next(snailNumbers.begin()); it != snailNumbers.end(); it = std::next(it)) {
        SnailNumber newSum;
        newSum.left = sum;
        newSum.right = *it;
        sum = newSum;
        int ready = 1;
        std::cout << snailToString(sum) << std::endl;

        while (ready) {
            while (ready) {
                ready = traverseExplode(sum, sum);
                if (ready)
                    std::cout << snailToString(sum) << std::endl;
            }
            ready = traverseSplit(sum, sum);
            if (ready)
                std::cout << snailToString(sum) << std::endl;
        }

        std::cout << "Complete" << std::endl;
    }

    unsigned long long int mag = magnitude(sum);

    std::cout << "Magnitude: " << mag << std::endl;

    // part 2;

    unsigned long long int maxMag = 0;
    for (auto &sn1 : snailNumbers) {
        for (auto &sn2 : snailNumbers) {
            if (&sn1 != &sn2) {
                std::cout << "Number 1: " << snailToString(sn1) << std::endl;
                std::cout << "Number 2: " << snailToString(sn2) << std::endl;
                SnailNumber snailNumber;
                snailNumber.left = sn1;
                snailNumber.right = sn2;

                int ready = 1;

                while (ready) {
                    while (ready) {
                        ready = traverseExplode(snailNumber, snailNumber);
                    }
                    ready = traverseSplit(snailNumber, snailNumber);
                }

                auto sumMag = magnitude(snailNumber);

                std::cout << "Magnitude: " << sumMag << std::endl;

                if (sumMag > maxMag)
                    maxMag = sumMag;
            }

        }
    }

    std::cout << "Max Magnitude: " << maxMag << std::endl;

    return 0;
}