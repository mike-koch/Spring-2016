#include <iostream>
#include <map>

// Function prototypes
void processCollatzSequence(int startingNumber, int endingNumber);
int getStoppingTime(int value);
void outputResults(std::map<int, int> &map1);

int main() {
    int startingNumber;
    int endingNumber;
    std::cout << "Enter your starting number: ";
    std::cin >> startingNumber;
    std::cout << std::endl << "Enter your ending number: ";
    std::cin >> endingNumber;
    std::cout << std::endl;
    processCollatzSequence(startingNumber, endingNumber);
    return 0;
}

void processCollatzSequence(int startingNumber, int endingNumber) {
    std::map<int, int> frequencyMap;
    for (int currentNumber = startingNumber; currentNumber <= endingNumber; currentNumber++) {
        int stoppingTime = getStoppingTime(currentNumber);
        frequencyMap[stoppingTime]++;
    }

    outputResults(frequencyMap);
}

int getStoppingTime(int value) {
    unsigned long long stoppingTime = 0;
    while (value != 1) {
        if (value % 2 == 0) {
            value /= 2;
        } else {
            value = (3 * value) + 1;
        }
        stoppingTime++;
    }
    return stoppingTime;
}

void outputResults(std::map<int, int> &frequencyMap) {
    std::cout << " Stopping Time | Frequency " << std::endl;
    std::cout << "---------------+-----------" << std::endl;
    for (auto const &element : frequencyMap) {
        std::cout << " ";
        std::printf("%13d", element.first);
        std::cout << " | ";
        std::printf("%9d", element.second);
        std::cout << std::endl;
    }
}