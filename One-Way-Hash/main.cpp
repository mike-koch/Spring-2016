#include <iostream>
#include <fstream>
#include <iomanip>
//#define DEBUG

#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

using namespace std;

unsigned int message_array[16];

//these are the values used to calculate SHA-256
unsigned int h[] = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

//these are values needed to calculate SHA-256
unsigned int k[] =
        {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
         0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
         0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
         0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
         0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
         0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
         0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
         0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

int main(int argc, char *argv[]) {
    ifstream input_stream;
    input_stream.open(argv[1]);

    uint64_t string_count = 0;

    string message;
    char next_character;

    while (input_stream.get(next_character)) {
        message += next_character;
        string_count++;
    }

    // add a 1 to the string
    // 80H = 10000000 binary
    message += 0x80;


    unsigned int w[64];
    for (int i = 0; i < 64; i++) {
        w[i] = 0;
    }

    for (int i = 0; i < 16; i++) {
        message_array[i] = 0;
    }

    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (((i * 4) + j) < message.length())
                message_array[i] |= ((unsigned char)(message.at((i * 4) + j)) << (32 - (8 * (j + 1))));
        }
    }


    //as long as the message length is less than 448 and a 1 has been appended to the string,
    //we can append the files length to the end of the array
    if (message.length() * 8 <= 448)
    {
        unsigned int temp1 = (uint)((string_count * 8) >> 32);
        unsigned int temp2 = (uint)(string_count * 8);
        message_array[14] = temp1;
        message_array[15] = temp2;
    }

    // create a 64-entry message schedule array w[0..63] of 32-bit words
    for (int i = 0; i < 16; i++) {
        w[i] = message_array[i];
    }


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
    unsigned int a = h[0];
    unsigned int b = h[1];
    unsigned int c = h[2];
    unsigned int d = h[3];
    unsigned int e = h[4];
    unsigned int f = h[5];
    unsigned int g = h[6];
    unsigned int hv = h[7];

    // Compression function main loop:
    for (int i = 0; i < 64; i++) {
        unsigned int s1 = (ROTRIGHT(e, 6) ^ ROTRIGHT(e, 11) ^ ROTRIGHT(e, 25));
        unsigned int ch = ((e & f) ^ ((~e) & g));
        unsigned int temp1 = hv + s1 + ch + k[i] + w[i];
        unsigned int s0 = (ROTRIGHT(a, 2) ^ ROTRIGHT(a, 13) ^ ROTRIGHT(a, 22));
        unsigned int maj = ((a & b) ^ (a & c) ^ (b & c));
        unsigned int temp2 = s0 + maj;

        hv = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
    }

    // Add the compressed chunk to the current hash value
    h[0] += a;
    h[1] += b;
    h[2] += c;
    h[3] += d;
    h[4] += e;
    h[5] += f;
    h[6] += g;
    h[7] += hv;

    input_stream.close();

    cout << setfill('0') << setw(8) << hex << h[0];
    cout << setfill('0') << setw(8) << hex << h[1];
    cout << setfill('0') << setw(8) << hex << h[2];
    cout << setfill('0') << setw(8) << hex << h[3];
    cout << setfill('0') << setw(8) << hex << h[4];
    cout << setfill('0') << setw(8) << hex << h[5];
    cout << setfill('0') << setw(8) << hex << h[6];
    cout << setfill('0') << setw(8) << hex << h[7];
    cout << endl;

    return 0;
}