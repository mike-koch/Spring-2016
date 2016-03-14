#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include "numbers.cpp"
//#define DEBUG

#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

using namespace std;

// Function prototypes
vector<vector<char>> chunk_message(vector<char> &entire_message);

string gulp(std::istream &in)
{
    string ret;
    char buffer[4096];
    while (in.read(buffer, sizeof(buffer)))
        ret.append(buffer, sizeof(buffer));
    ret.append(buffer, in.gcount());
    return ret;
}

int main(int argc, char *argv[]) {
    ifstream input_stream;
    input_stream.open(argv[1]);


    string message = gulp(input_stream);

    unsigned int h0 = 0x6a09e667;
    unsigned int h1 = 0xbb67ae85;
    unsigned int h2 = 0x3c6ef372;
    unsigned int h3 = 0xa54ff53a;
    unsigned int h4 = 0x510e527f;
    unsigned int h5 = 0x9b05688c;
    unsigned int h6 = 0x1f83d9ab;
    unsigned int h7 = 0x5be0cd19;

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
        int chunk_index = 0;
        for (int i = 0; i < 16; i++) {
            // copy chunk into first 16 words w[0..15] of the message schedule array
            unsigned int word;

            for (int j = chunk_index; j < chunk_index + 32; j++) {
                if (chunk.at(j) == '1') {
                    word |= 1 << (j % 32);
                } else {
                    word &= ~(1 << (j % 32));
                }
            }

            w[i] = word;
            chunk_index += 32;
        }


#ifdef DEBUG
        cout << "After chunking, the schedule array should have 16 elements" << endl;
        for (int i = 0; i < 16; i++) {
            cout << w[i] << " ";
        }
#endif

        // Now extend the first 16 words into the remaining 48 words of the message schedule array
        for (int i = 16; i < 64; i++) {
            unsigned int s0 = ROTRIGHT(w[i - 15], 7) ^
                              ROTRIGHT(w[i - 15], 18) ^
                              (w[i - 15] >> 3);
            unsigned int s1 = ROTRIGHT(w[i - 2], 17) ^
                              ROTRIGHT(w[i - 2], 19) ^
                              (w[i - 2] >> 10);

            w[i] = w[i - 16] + s0 + w[i - 7] + s1;
        }

        // Initialize working variables to current hash value
        unsigned int a = h0;
        unsigned int b = h1;
        unsigned int c = h2;
        unsigned int d = h3;
        unsigned int e = h4;
        unsigned int f = h5;
        unsigned int g = h6;
        unsigned int h = h7;

        // Compression function main loop:
        for (int i = 0; i < 64; i++) {
            char32_t s1 = ROTRIGHT(e, 6) ^ ROTRIGHT(e, 11) ^ ROTRIGHT(e, 25);
            char32_t ch = ((e) & (f)) ^ (~(e) & (g));
            char32_t temp1 = h + s1 + ch + k[i] + w[i];
            char32_t s0 = ROTRIGHT(a, 2) ^ ROTRIGHT(a, 13) ^ ROTRIGHT(a, 22);
            char32_t maj = ((a) & (b)) ^ ((a) & (c)) ^ ((b) & (c));
            char32_t temp2 = s0 + maj;

            h = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }

        // Add the compressed chunk to the current hash value
        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        h4 += e;
        h5 += f;
        h6 += g;
        h7 += h;
    }

    cout << hex << h0;
    cout << hex << h1;
    cout << hex << h2;
    cout << hex << h3;
    cout << hex << h4;
    cout << hex << h5;
    cout << hex << h6;
    cout << hex << h7;

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