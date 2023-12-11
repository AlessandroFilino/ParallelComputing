//
// Created by Alessandro Filino on 23/11/23.
//

#include <iostream>
#include <gtest/gtest.h>

#include "desKeyGenerator.h"
#include "utility.h"

using namespace std;

/*
 * Testiamo la funzione create_sub_keys(string key)
 * Data una chiave in binario, la funzione restituirà le 16 subkeys utilizzate nei passi successivi dell' algoritmo.
 * Per testare il metodo, utiliziamo:
 *  -   Una chiave nota
 *  -   16 Subkeys che derivano dalla chiave nota
 *  Verifichiamo quindi che il risultato del nostro metodo produca le stesse chiavi
 */
TEST(desKeyGenerator, test_generate_sub_keys){
    vector<bitset<48>> expected_subkeys = {
            bitset<48>("000110010100110011010000011100101101111010001100"),
            bitset<48>("010001010110100001011000000110101011110011001110"),
            bitset<48>("000001101110110110100100101011001111010110110101"),
            bitset<48>("110110100010110100000011001010110110111011100011"),
            bitset<48>("011010011010011000101001111111101100100100010011"),
            bitset<48>("110000011001010010001110100001110100011101011110"),
            bitset<48>("011100001000101011010010110111011011001111000000"),
            bitset<48>("001101001111100000100010111100001100011001101101"),
            bitset<48>("100001001011101101000100011100111101110011001100"),
            bitset<48>("000000100111011001010111000010001011010110111111"),
            bitset<48>("011011010101010101100000101011110111110010100101"),
            bitset<48>("110000101100000111101001011010100100101111110011"),
            bitset<48>("100110011100001100010011100101111100100100011111"),
            bitset<48>("001001010001101110001011110001110001011111010000"),
            bitset<48>("001100110011000011000101110110011010001101101101"),
            bitset<48>("000110000001110001011101011101011100011001101101")
     };
    bitset<64> test_key("1010101010111011000010010001100000100111001101101100110011011101");
    vector<bitset<48>> sub_keys = create_sub_keys(test_key);

    ASSERT_EQ(sub_keys, expected_subkeys);
}