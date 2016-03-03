//
// Derived from http://stackoverflow.com/questions/776508/best-practices-for-circular-shift-rotate-operations-in-c.
// Licensed under the MIT License
//

#include <assert.h>
#include <limits.h>
#include "rotate_functions.h"

char32_t rotate_left_32(char32_t n, unsigned int c) {
    const unsigned int mask = (CHAR_BIT*sizeof(n)-1);

    assert ( (c<=mask) &&"rotate by type width or more");
    c &= mask;  // avoid undef behaviour with NDEBUG.  0 overhead for most types / compilers
    return (n<<c) | (n>>( (-c)&mask ));
}

char32_t rotate_right_32(char32_t n, unsigned int c) {
    const unsigned int mask = (CHAR_BIT*sizeof(n)-1);

    assert ( (c<=mask) &&"rotate by type width or more");
    c &= mask;  // avoid undef behaviour with NDEBUG.  0 overhead for most types / compilers
    return (n>>c) | (n<<( (-c)&mask ));
}

