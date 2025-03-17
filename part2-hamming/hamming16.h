#ifndef HAMMING16_H
#define HAMMING16_H

#include <stdint.h>

uint16_t extendedHammingEncode(uint16_t data);
uint16_t extendedHammingDecode(uint16_t code, int *errorPos);
uint16_t introduceError(uint16_t code, int bitPos);

#endif // HAMMING16_H