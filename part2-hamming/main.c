#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>  // for strtol and atoi
#include "hamming16.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <data in hex> [errorBit]\n", argv[0]);
        return 1;
    }
    
    // Parse the input data from command line (in hex or decimal)
    char *endptr;
    uint16_t data = (uint16_t)strtol(argv[1], &endptr, 0);
    // Ensure data fits in 11 bits (0-2047)
    if (data > 0x7FF) {
        fprintf(stderr, "Error: Data must be an 11-bit value (0 to 0x7FF).\n");
        return 1;
    }
    
    printf("Original data (11-bit): 0x%03X\n", data);
    
    // Encode the data using Extended Hamming (16,11)
    uint16_t encoded = extendedHammingEncode(data);
    printf("Encoded 16-bit code: 0x%04X\n", encoded);
    
    uint16_t codeForDecoding = encoded;
    
    // Optional: Introduce an error if an error bit position is provided.
    if (argc >= 3) {
        int errorBit = atoi(argv[2]);
        if (errorBit < 1 || errorBit > 16) {
            fprintf(stderr, "Error: Error bit must be in the range 1 to 16.\n");
            return 1;
        }
        codeForDecoding = introduceError(encoded, errorBit);
        printf("Encoded code with error at bit %d: 0x%04X\n", errorBit, codeForDecoding);
    }
    
    // Decode the code and correct a single-bit error if present.
    int correctedBit;
    uint16_t decoded = extendedHammingDecode(codeForDecoding, &correctedBit);
    
    if (correctedBit < 0) {
        printf("Error detection failed: multiple or uncorrectable errors detected.\n");
    } else if (correctedBit == 0) {
        printf("No error detected during decoding.\n");
    } else {
        printf("Error corrected at bit position: %d\n", correctedBit);
    }
    
    printf("Decoded data (11-bit): 0x%03X\n", decoded);
    
    if (decoded == data) {
        printf("Success: Decoded data matches the original data.\n");
    } else {
        printf("Failure: Decoded data does not match the original data.\n");
    }
    
    return 0;
}