//
// Created by Alessandro Filino on 23/02/24.
//

#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <vector>

using namespace std;

const char allowed_char [] = {  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
                                'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
                                'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
                                'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F',
                                'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
                                'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
                                'W', 'X', 'Y', 'Z', '1', '2', '3', '4',
                                '5', '6', '7', '8', '9', '0', '.', '/'  };
long allowed_char_size = sizeof(allowed_char) / sizeof(allowed_char[0]);

int* string_to_binary(const char* string_text, int password_length, int* bin_text) {
    
    int current_index = 0;
    for (int i = 0; i < password_length; i++) {
        char currentChar = string_text[i];
        for (int j = 7; j >= 0; --j) {
            bin_text[current_index++] = (currentChar >> j) & 1;
        }
    }

    return bin_text;
} 

int* make_permutation(int* bin_text, const int* permutation_table, int permutation_table_size, int* new_bin_text){
    
    for(int i = 0; i < permutation_table_size; i++){
        new_bin_text[i] = bin_text[permutation_table[i] - 1];
    }

    return new_bin_text;
}

int* shift_left(int* bin_text, unsigned int number_of_shift, unsigned int length) {
    
    int temp;
    for (unsigned int i = 0; i < number_of_shift; i++) {
        temp = bin_text[0];
        for (unsigned int j = 0; j < length - 1; j++) {
            bin_text[j] = bin_text[j + 1];
        }
        bin_text[length - 1] = temp;
    }
    
    return bin_text;
}

#endif //UTILITY_H