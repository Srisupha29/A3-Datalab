#include "hamming16.h"

/*
* Data structure
*
* +--------+--------+--------+--------+
* | ( 0)EP | ( 1)P  | ( 2)P  | ( 3)D  |  <-- row 0
* +--------+--------+--------+--------+
* | ( 4)P  | ( 5)D  | ( 6)D  | ( 7)D  |  <-- row 1
* +--------+--------+--------+--------+
* | ( 8)P  | ( 9)D  | (10)D  | (11)D  |  <-- row 2
* +--------+--------+--------+--------+
* | (12)D  | (13)D  | (14)D  | (15)D  |  <-- row 3
* +--------+--------+--------+--------+
* ↑        ↑        ↑        ↑
* col 0    col 1    col 2    col 3
*/

/*
 * extendedHammingEncode:
 * - data: lower 11 bits represent the 11-bit data.
 * - Returns a 16-bit code word with:
 * - Extended Parity (EP) at bit 0.
 * - Hamming parity bits at positions 1, 2, 4, 8.
 * - Data bits at positions 3, 5, 6, 7, 9, 10, 11, 12, 13, 14, 15.
 */
uint16_t extendedHammingEncode(uint16_t data) {
    uint16_t code = 0;
    uint16_t p1, p2, p4, p8;
    uint16_t ep = 0;
    int i;

    code |= ((data >> 0) & 1) << 3;
    code |= ((data >> 1) & 1) << 5;
    code |= ((data >> 2) & 1) << 6;
    code |= ((data >> 3) & 1) << 7;
    code |= ((data >> 4) & 1) << 9;
    code |= ((data >> 5) & 1) << 10;
    code |= ((data >> 6) & 1) << 11;
    code |= ((data >> 7) & 1) << 12;
    code |= ((data >> 8) & 1) << 13;
    code |= ((data >> 9) & 1) << 14;
    code |= ((data >> 10) & 1) << 15;

    p1 = ((code >> 3) ^ (code >> 5) ^ (code >> 7) ^ (code >> 9) ^ (code >> 11) ^ (code >> 13) ^ (code >> 15)) & 1;
    p2 = ((code >> 3) ^ (code >> 6) ^ (code >> 7) ^ (code >> 10) ^ (code >> 11) ^ (code >> 14) ^ (code >> 15)) & 1;
    p4 = ((code >> 5) ^ (code >> 6) ^ (code >> 7) ^ (code >> 12) ^ (code >> 13) ^ (code >> 14) ^ (code >> 15)) & 1;
    p8 = ((code >> 9) ^ (code >> 10) ^ (code >> 11) ^ (code >> 12) ^ (code >> 13) ^ (code >> 14) ^ (code >> 15)) & 1;

    code |= (p1 << 1);
    code |= (p2 << 2);
    code |= (p4 << 4);
    code |= (p8 << 8);

    for (i = 1; i <= 15; i++) {
        ep ^= (code >> i) & 1;
    }
    code |= (ep & 1);

    return code;
}

/*
 * extendedHammingDecode:
 * - code: 16-bit Extended Hamming code word.
 * - errorPos: pointer to an integer to store the bit position of a corrected error.
 * - Returns the original 11-bit data (in the lower 11 bits).
 */
uint16_t extendedHammingDecode(uint16_t code, int *errorPos) {
    uint16_t correctedCode = code;
    int s1, s2, s4, s8;
    int syndrome = 0;
    int actual_ep = 0;
    int i;
    uint16_t data = 0;

    s1 = ((code >> 1) ^ (code >> 3) ^ (code >> 5) ^ (code >> 7) ^ (code >> 9) ^ (code >> 11) ^ (code >> 13) ^ (code >> 15)) & 1;
    s2 = ((code >> 2) ^ (code >> 3) ^ (code >> 6) ^ (code >> 7) ^ (code >> 10) ^ (code >> 11) ^ (code >> 14) ^ (code >> 15)) & 1;
    s4 = ((code >> 4) ^ (code >> 5) ^ (code >> 6) ^ (code >> 7) ^ (code >> 12) ^ (code >> 13) ^ (code >> 14) ^ (code >> 15)) & 1;
    s8 = ((code >> 8) ^ (code >> 9) ^ (code >> 10) ^ (code >> 11) ^ (code >> 12) ^ (code >> 13) ^ (code >> 14) ^ (code >> 15)) & 1;

    syndrome = s1 | (s2 << 1) | (s4 << 2) | (s8 << 3);

    for (i = 0; i <= 15; i++) {
        actual_ep ^= (code >> i) & 1;
    }

    if (syndrome == 0) {
        if (actual_ep == 0) {
            *errorPos = 0; 
        } else {
            correctedCode ^= (1 << 0);
            *errorPos = 1; 
        }
    } else {
        if (actual_ep == 1) {
            correctedCode ^= (1 << syndrome);
            *errorPos = syndrome + 1; 
        } else {
            *errorPos = -1;
        }
    }

    data |= ((correctedCode >> 3)  & 1) << 0;
    data |= ((correctedCode >> 5)  & 1) << 1;
    data |= ((correctedCode >> 6)  & 1) << 2;
    data |= ((correctedCode >> 7)  & 1) << 3;
    data |= ((correctedCode >> 9)  & 1) << 4;
    data |= ((correctedCode >> 10) & 1) << 5;
    data |= ((correctedCode >> 11) & 1) << 6;
    data |= ((correctedCode >> 12) & 1) << 7;
    data |= ((correctedCode >> 13) & 1) << 8;
    data |= ((correctedCode >> 14) & 1) << 9;
    data |= ((correctedCode >> 15) & 1) << 10;

    return data;
}

/*
 * introduceError:
 * - code: a 16-bit code word.
 * - bitPos: bit position (1-indexed, 1 to 16) to flip.
 * - Returns the code word with the specified bit flipped.
 */
uint16_t introduceError(uint16_t code, int bitPos) {
    return code ^ (1 << (bitPos - 1));
}