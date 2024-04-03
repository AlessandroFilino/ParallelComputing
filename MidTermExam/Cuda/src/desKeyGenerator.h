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

int** create_sub_keys(int* key) {
    int** sub_keys = (int**)malloc(16 * sizeof(int*));

    int* resized_key = (int*)malloc(56 * sizeof(int));
    make_permutation(key, parity_drop_table, 56, resized_key);

    int* left = (int*)malloc(28 * sizeof(int));
    int* right = (int*)malloc(28 * sizeof(int));

    for (int i = 0; i < 28; i++) {
        left[i] = resized_key[i];
        right[i] = resized_key[i + 28];
    }

    for (int i = 0; i < 16; i++) {
        left = shift_left(left, shift_table[i], 28);
        right = shift_left(right, shift_table[i], 28);


        int* combined_key = (int*)malloc(56 * sizeof(int));
        for(int j = 0; j < 28; j++){
            combined_key[j] = left[j];
            combined_key[j + 28] = right[j];
        }

        int* roundKey = (int*)malloc(48 * sizeof(int));
        make_permutation(combined_key, key_compression_table, 48, roundKey);

        sub_keys[i] = roundKey;
        free(combined_key);
    }
    free(resized_key);
    free(left);
    free(right);

    return sub_keys;
}

#endif //DESKEYGENERATOR_H

