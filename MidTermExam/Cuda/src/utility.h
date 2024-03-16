//
// Created by Alessandro Filino on 23/02/24.
//

#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <vector>

using namespace std;

__device__ const char d_allowed_char [] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
                               'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
                               'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
                               'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F',
                               'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
                               'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
                               'W', 'X', 'Y', 'Z', '1', '2', '3', '4',
                               '5', '6', '7', '8', '9', '0', '.', '/'};
__device__ long allowed_char_size = sizeof(d_allowed_char) / sizeof(d_allowed_char[0]);

char* string_to_binary(const char* string_text, char* bin_text) {
    int index = 0;
    for (int i = 0; string_text[i] != '\0'; i++) {
        char currentChar = string_text[i];
        for (int j = 7; j >= 0; --j) {
            bin_text[index++] = ((currentChar >> j) & 1) ? '1' : '0';
        }
    }
    bin_text[index] = '\0';

    return bin_text;
} 



char* make_permutation(const char* bin_text, const int* permutation_table, int permutation_table_size, char* new_bin_text){
    for(int i = 0; i < permutation_table_size; i++){
        new_bin_text[i] = bin_text[permutation_table[i] - 1];
    }
    new_bin_text[permutation_table_size + 1] = '\0';

    return new_bin_text;
}

char* shift_left(char* bin_text, unsigned int number_of_shift) {
    int length = strlen(bin_text);
    char temp;
    for (unsigned int i = 0; i < number_of_shift; i++) {
        temp = bin_text[0];
        for (int j = 0; j < length - 1; j++) {
            bin_text[j] = bin_text[j + 1];
        }
        bin_text[length - 1] = temp;
    }
    return bin_text;
}


#endif //UTILITY_H

#ifdef __CUDACC__
__global__ void cuda_make_permutation(const char *bin_text, const int *permutation_table, int permutation_table_size, char *new_bin_text);
__global__ void cuda_shift_left(char *bin_text, unsigned int number_of_shift);
#endif