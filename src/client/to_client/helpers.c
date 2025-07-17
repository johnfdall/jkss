#include <stdio.h>
#include <stdint.h>

// Function to print the binary representation of a uint64_t
void print_binary(uint64_t number) {
        for (int i = 63; i >= 0; i--) { // Start from the most significant bit
                printf("%d", (int)(number >> i) & 1);
                if (i % 8 == 0 && i != 0) { // Add a space every 8 bits for readability
                        printf(" ");
                }
        }
        printf("\n");
}
