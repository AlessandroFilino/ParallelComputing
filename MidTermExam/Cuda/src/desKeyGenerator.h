//
// Created by Alessandro Filino on 23/02/24.
//

#ifndef DESKEYGENERATOR_H
#define DESKEYGENERATOR_H

#include "utility.h"

static int parity_drop_table[56] = {
        57, 49, 41, 33, 25, 17, 9,
        1, 58, 50, 42, 34, 26, 18,
        10, 2, 59, 51, 43, 35, 27,
        19, 11, 3, 60, 52, 44, 36,
        63, 55, 47, 39, 31, 23, 15,
        7, 62, 54, 46, 38, 30, 22,
        14, 6, 61, 53, 45, 37, 29,
        21, 13, 5, 28, 20, 12, 4
};

static int shift_table[16] = {
        1, 1, 2, 2, 2, 2, 2, 2,
        1, 2, 2, 2, 2, 2, 2, 1
};

static int key_compression_table[48] = {
        14, 17, 11, 24, 1, 5,
        3, 28, 15, 6, 21, 10,
        23, 19, 12, 4, 26, 8,
        16, 7, 27, 20, 13, 2,
        41, 52, 31, 37, 47, 55,
        30, 40, 51, 45, 33, 48,
        44, 49, 39, 56, 34, 53,
        46, 42, 50, 36, 29, 32
};

char** create_sub_keys(char* key) {
    char** sub_keys = (char**)malloc(16 * sizeof(char*)); // Memoria per 16 puntatori

    // Applichiamo la prima permutazione alla chiave --> La chiave passa dai 64 bit iniziali a 56
    char* resized_key = (char*)malloc(56 * sizeof(char));
    for(int i = 0; i < 56; i++) {
        resized_key[i] = key[parity_drop_table[i] - 1];
    }

    // Split Right and Left della chiave
    char* left = (char*)malloc(28 * sizeof(char));
    char* right = (char*)malloc(28 * sizeof(char));

    for (int i = 0; i < 28; i++) {
        left[i] = resized_key[i];
        right[i] = resized_key[i + 28];
    }

    for (int i = 0; i < 16; i++) {
        left = shift_left(left, shift_table[i]);
        right = shift_left(right, shift_table[i]);

        char* combined_key = (char*)malloc(56 * sizeof(char));
        memcpy(combined_key, left, 28);
        memcpy(combined_key + 28, right, 28);

        char* roundKey = (char*)malloc(48 * sizeof(char));

        for(int j = 0; j < 48; j++) {
            roundKey[j] = combined_key[key_compression_table[j] - 1];
        }
        sub_keys[i] = roundKey;
        free(combined_key);
    }
    free(resized_key);
    free(left);
    free(right);

    return sub_keys;
}



#endif //DESKEYGENERATOR_H

