//
// Created by Alessandro Filino on 23/11/23.
//

#include <iostream>
#include <string>
#include <random>
#include "utility.h"

using namespace std;

/*
 * Parity Drop Table 1
 * Prima tabella che permuterà la nostra chiave generata (PC-1)
 * La chiave iniziale a 64 bit viene ridotta a 56.
 * Per fare questo, sono scartat i bit in che erano in una posizione multipla di 8 (8, 16, 24, ...).
 * Questi bit scartati sono detti bit di parità e sono utilizzati per confermare il successo dello scambio della chiave in una comunicazione
 */
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

/*
 *  Shift Table
 *  Tabella di riferimento che indica per ogni round, il numero di shift verso destra da eseguire partendo dal bit più a sinista.
 *  Ottenuti i 56 bit dall'operazione precedente, si divide la chiave in due parti di uguale dimensione (28 bit).
 *  Per ogni parte si esegue uno shift in base al numero di round che stiamo seguendo.
 */
static int shift_table[16] = {
        1, 1, 2, 2, 2, 2, 2, 2,
        1, 2, 2, 2, 2, 2, 2, 1
};

/*
 *  Key Compression Table
 *  Si applica un ultima permutazione alla chiave.
 *  Inoltre dai 56 bit precedenti, viene ridotta a 48 finali.
 */
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

/*
 *  Generiamo le 16 sottochiavi utili nell'algoritmo a partire da una chiave a 64 bit
 *  key --> sarà la chiave iniziale. Supponiamo che sia già in formato binario
 */

vector<bitset<48>> create_sub_keys(bitset<64> key){
    vector<bitset<48>> sub_keys;

    //Applichiamo la prima permutazione alla chiave --> La chiave passa dai 64 bit iniziali a 56
    bitset<56> resized_key = make_permutation<64,56>(key, parity_drop_table);

    //Split Right and Left della chiave
    bitset<28> left(splitting_bitset(resized_key)[0]);
    bitset<28> right(splitting_bitset(resized_key)[1]);

    //Shifting
    for (int i = 0; i < 16; i++) {
        left = shift_left(left, shift_table[i]);
        right = shift_left(right, shift_table[i]);

        //Combino il risultato tra le due parti
        bitset<56> combined_key((left.to_ullong() << 28) | right.to_ullong());

        //Applico la permutazione con key_compression_table
        bitset<48> roundKey = make_permutation<56,48>(combined_key, key_compression_table);

        sub_keys.push_back(roundKey);

    }
    return sub_keys;
}


