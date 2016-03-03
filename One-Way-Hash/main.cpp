#include <iostream>
#include <vector>
#include "numbers.cpp"
#define DEBUG

using namespace std;

int main() {
    string message = "hello, world!";

    // convert the string into a list of strings, representing each bit.
    vector<char> bit_string;
    const char* message_char_array = message.c_str();
    for (int i = 0; i < message.length(); i++) {
        for (int j = 0; j < 7; j++) {
            if (message_char_array[i] & (1 << j)) {
                bit_string.push_back('1');
            } else {
                bit_string.push_back('0');
            }
        }
    }

    // append a '1' bit to the end
    bit_string.push_back('1');

#ifdef DEBUG
    cout << "Size of bit string, including bit at end: " << bit_string.size() << endl;
#endif

    int number_of_zeros_to_add = 448 - (bit_string.size() % 512);

    // If we are negative, that means the mod operator returned a value larger than 448. We need to loop around and come back.
    if (number_of_zeros_to_add < 0) {
        number_of_zeros_to_add = 512 - number_of_zeros_to_add;
    }

#ifdef DEBUG
    cout << "Number of zeros we need to add: " << number_of_zeros_to_add << endl;
#endif

    for (int i = 0; i < number_of_zeros_to_add; i++) {
        bit_string.push_back('0');
    }

    // initialize the 8 working variables, known as hnm1 in numbers.cpp

    //Initialize array of round constants.. known as k in numbers.cpp


    // append '0' bits such that the length of the message mod 512 = 448

    cout << "Hello, World!" << endl;
    return 0;
}