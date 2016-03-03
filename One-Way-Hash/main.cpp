#include <iostream>
#include <vector>
#include "numbers.cpp"
#include "rotate_functions.h"
#define DEBUG

using namespace std;

// Function prototypes
vector<vector<char>> chunk_message(vector<char> &entire_message);

int main() {
    string message = "hello, world!";

    // convert the string into a list of strings, representing each bit.
    vector<char> bit_string;
    const char* message_char_array = message.c_str();
    for (int i = 0; i < message.length(); i++) {
        for (int j = 0; j < 8; j++) {
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

    // Now append the length of the message to the bit string.
    int64_t length_of_message = message.length();
    for (int i = 0; i < 64; i++) {
        int bit = (length_of_message >> i) & 1;
        bit_string.push_back(bit + '0');
    }

#ifdef DEBUG
    cout << "Length of message: " << length_of_message << endl;
    cout << "Length of bit string: " << bit_string.size() << endl;
#endif

    vector<vector<char>> chunked_message = chunk_message(bit_string);

#ifdef DEBUG
    cout << "After chunking, there are " << chunked_message.size() << " chunks to process\n";
#endif

    for (vector<char> chunk : chunked_message) {
        // create a 64-entry message schedule array w[0..63] of 32-bit words
        char32_t schedule_array[64];
        int chunk_index = 0;
        for (int i = 0; i < 16; i++) {
            // copy chunk into first 16 words w[0..15] of the message schedule array
            char32_t word;

            for (int j = chunk_index; j < chunk_index + 32; j++) {
                if (chunk.at(j) == '1') {
                    word |= 1 << (j % 32);
                } else {
                    word &= ~(1 << (j % 32));
                }
            }

            schedule_array[i] = word;
            chunk_index += 32;
        }


#ifdef DEBUG
        cout << "After chunking, the schedule array should have 16 elements" << endl;
        for (int i = 0; i < 16; i++) {
            cout << schedule_array[i] << " ";
        }
#endif

        // Now extend the first 16 words into the remaining 48 words of the message schedule array
        for (int i = 16; i < 64; i++) {
            char32_t s0 = rotate_right_32(schedule_array[i - 15], 7) ^
                    rotate_right_32(schedule_array[i - 15], 18) ^
                    (schedule_array[i - 15] >> 3);

            char32_t s1 = rotate_right_32(schedule_array[i - 2], 17) ^
                          rotate_right_32(schedule_array[i - 2], 19) ^
                          (schedule_array[i - 2] >> 10);

            schedule_array[i] = schedule_array[i - 16] + s0 + schedule_array[i - 7] + s1;
        }
    }

    return 0;
}

vector<vector<char>> chunk_message(vector<char> &entire_message) {
    // Break the message into 512-bit chunks.
    vector<vector<char>> chunked_message;
    for (int i = 0; i < entire_message.size(); i += 512) {
        vector<char> chunk;
        for (int j = 0; j < 512; j++) {
            chunk.push_back(entire_message.at(i + j));
        }
        chunked_message.push_back(chunk);
    }

    return chunked_message;
}