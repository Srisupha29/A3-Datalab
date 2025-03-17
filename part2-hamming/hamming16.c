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
*          ↑        ↑        ↑        ↑
*        col 0    col 1    col 2    col 3
*/

/*
 * extendedHammingEncode:
 *  - data: lower 11 bits represent the 11-bit data.
 *  - Returns a 16-bit code word with:
 *      - Extended Parity (EP) at bit 0.
 *      - Hamming parity bits at positions 1, 2, 4, 8.
 *      - Data bits at positions 3, 5, 6, 7, 9, 10, 11, 12, 13, 14, 15.
 */
uint16_t extendedHammingEncode(uint16_t data) {
    uint16_t code = 0;

    // TODO: Insert the 11 data bits into the proper positions of the 15-bit Hamming code.
    // For example, assign bit0 of data to position 3, bit1 to position 5, etc.

    // TODO: Compute the parity bits for positions 1, 2, 4, and 8.
    // For each parity bit, XOR all bits in the code word covered by that parity position.

    // TODO: Insert the computed parity bits into the code word.

    // TODO: Compute the overall parity bit (bit 0) as the XOR of bits 1 through 15.
    // Then, set bit 0 accordingly.

    return code;
}

/*
 * extendedHammingDecode:
 *  - code: 16-bit Extended Hamming code word.
 *  - errorPos: pointer to an integer to store the bit position of a corrected error.
 *  - Returns the original 11-bit data (in the lower 11 bits).
 *
 * The function should:
 *   1. Recompute the parity bits to form a syndrome.
 *   2. Check the overall parity (bit 0) to confirm if an error occurred.
 *   3. If a single-bit error is detected, correct the error by flipping that bit.
 *   4. Extract and return the 11-bit data.
 */
uint16_t extendedHammingDecode(uint16_t code, int *errorPos) {
    uint16_t correctedCode = code;
    int syndrome = 0;

    // TODO: Recompute parity bits for positions 1, 2, 4, and 8.
    // Combine the parity check results to form a syndrome (a number from 0 to 15).

    // TODO: Check the overall parity bit (bit 0) to decide if an error occurred.
    // If syndrome != 0 and overall parity is incorrect, then a single-bit error occurred.

    // Correct the error by flipping the bit at the position indicated by syndrome.
    // Set *errorPos to the corrected bit position (1-indexed).
    // If no error, set *errorPos to 0.
    // If error detection indicates an uncorrectable error, set *errorPos to -1.

    // TODO: Extract the original 11-bit data from the corrected code word.
    uint16_t data = 0;

    return data;
}

/*
 * introduceError:
 *  - code: a 16-bit code word.
 *  - bitPos: bit position (1-indexed, 1 to 16) to flip.
 *  - Returns the code word with the specified bit flipped.
 */
uint16_t introduceError(uint16_t code, int bitPos) {
    return code ^ (1 << (bitPos - 1));
}