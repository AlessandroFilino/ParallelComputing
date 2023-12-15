//
// Created by Alessandro Filino on 27/11/23.
//

#include <iostream>

#include "utility.h"

using namespace std;

/*
  *   Permutazione iniziale IP
  */
static int initial_permutation_table[64] = {
        58, 50, 42, 34, 26, 18, 10, 2,
        60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6,
        64, 56, 48, 40, 32, 24, 16, 8,
        57, 49, 41, 33, 25, 17,  9, 1,
        59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5,
        63, 55, 47, 39, 31, 23, 15, 7
};

/*
 * Expansion Table
 * Trasforma il blocco da 32 a 64 bit ripetendo alcuni tra i bit presenti
 */
static int expansion_permutation_table[48] = {
        32, 1, 2, 3, 4, 5, 4, 5,
        6, 7, 8, 9, 8, 9, 10, 11,
        12, 13, 12, 13, 14, 15, 16, 17,
        16, 17, 18, 19, 20, 21, 20, 21,
        22, 23, 24, 25, 24, 25, 26, 27,
        28, 29, 28, 29, 30, 31, 32, 1
};

/*
 * S - Box Table
 */
const int s_box_table[8][4][16] = {
        {
                14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
                0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
                4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
                15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13
        },
        {
                15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
                3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
                0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
                13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9
        },
        {
                10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
                13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
                13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
                1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12
        },
        {
                7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
                13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
                10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
                3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14
        },
        {
                2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
                14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
                4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
                11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3
        },
        {
                12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
                10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
                9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
                4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13
        },
        {
                4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
                13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
                1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
                6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12
        },
        {
                13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
                1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
                7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
                2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
        }
};

/*
 * D-Box Permutation Table
 */
const int straight_permutation_table[32] = {
        16,  7, 20, 21,
        29, 12, 28, 17,
        1, 15, 23, 26,
        5, 18, 31, 10,
        2,  8, 24, 14,
        32, 27,  3,  9,
        19, 13, 30,  6,
        22, 11,  4, 25
};

/*
 * Final Permutation Table
 */
const int final_permutation_table[64] = {
        40, 8, 48, 16, 56, 24, 64, 32,
        39, 7, 47, 15, 55, 23, 63, 31,
        38, 6, 46, 14, 54, 22, 62, 30,
        37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58, 26,
        33, 1, 41, 9, 49, 17, 57, 25
};

/*
 * des_encrypt_text
 * Cifriamo il testo in chiaro utilizzando i passi dell'algoritmo DES
 * plain_text --> Supponiamo che il testo sia passato già cifrato in binario
 */
bitset<64> des_encrypt_text(string plain_text, vector<bitset<48>> sub_keys){
    bitset<64> cipher_text;

    bitset<64> bin_plain_text (string_to_binary(plain_text));
    //bitset<64> bin_plain_text("1010101111001101111001101010101111001101000100110010010100110110");
    //Applicchiamo la permutazione con la tabella IP
    bin_plain_text = make_permutation<64,64>(bin_plain_text, initial_permutation_table);

    //Splitting del blocco a 64 bit del testo passato. Otterremo due sottoblocchi da 32 bit ciascuno
    bitset<32> left_block = (splitting_bitset(bin_plain_text)[0]);
    bitset<32> right_block = (splitting_bitset(bin_plain_text)[1]);

    for (int round = 0; round < 16; round++) {
        //Applichiamo la permutazione con la tabella di espansione (D-Box)
        bitset<48> right_expanded = make_permutation<32,48>(right_block, expansion_permutation_table);

        //Applichiamo la funzione XOR tra la sottochiave round-esima e il blocco destro permutato precedentemente
        bitset<48> result = xor_function(sub_keys[round], right_expanded);

        //Andiamo ad applicare la tabella S-Box ossia calcoliamo riga e colonna e riportiamo il risultato
        bitset<32> s_box_result;

        for (int i = 0; i < 8; i++) {
            bitset<6> block;

            for (int j = 0; j < 6; j++) {
                block.set(j, result[47 - (i * 6 + j)]);
            }

            int row = (block[0] << 1) + block[5];
            int col = (block[1] << 3) + (block[2] << 2) + (block[3] << 1) + block[4];

            bitset<4> s_box_value(s_box_table[i][row][col]);

            s_box_result |= (bitset<32>(s_box_value.to_ulong()) << (28 - i * 4));

        }
        s_box_result = make_permutation<32, 32>(s_box_result, straight_permutation_table);

        bitset<32> xor_result = xor_function(s_box_result, left_block);
        left_block = xor_result;

        if (round != 15){
            //Swapping
            left_block ^= right_block;
            right_block ^= left_block;
            left_block ^= right_block;
        }
    }

    bitset<64> combined_key((left_block.to_ullong() << 32) | right_block.to_ullong());

    cipher_text = make_permutation<64,64>(combined_key, final_permutation_table);

    return cipher_text;
}
