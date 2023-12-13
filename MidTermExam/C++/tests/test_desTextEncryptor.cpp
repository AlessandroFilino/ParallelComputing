//
// Created by Alessandro Filino on 29/11/23.
//

#include <iostream>
#include <gtest/gtest.h>
#include <string>

#include "desTextEncryptor.h"
#include "desKeyGenerator.h"
#include "utility.h"

using namespace std;

/*
 * Proviamo la funzione des_encrypt_text
 * Questa prende come input un testo ascii e restituisce la sua versione cifrata
 * L'esepio è stato fatto seguendo i risultati in: http://styere.xyz/JS-DES.html
 */
TEST(desTextEncryptor, test_text_encryptor){
    bitset<64> test_key(string_to_binary("A4rT9v.w"));
    bitset<64> expected_text_encrypt("0110110011000111110011001101100001110111100000100010001100000101");
    string text = "abcabcab";
    vector<bitset<48>> sub_keys = create_sub_keys(test_key);

    bitset<64> chiper_text = des_encrypt_text(text, sub_keys);

    ASSERT_EQ(chiper_text, expected_text_encrypt);
}

/*
 * Testiamo se l'algoritmo continua a funzionare nel caso in cui la stringa da cifrare non è a 64 bit ma inferiore.
 * In questo caso la stringa è a 48 bit
 * Viene qundi fatto padding aggiungendo n 0 iniziali quanti necessari per arrivare a completare il bitset da 64 bit
 */
TEST(desTextEncryptor, test_text_encryptor_2){
    bitset<64> test_key(string_to_binary("A4rT9v.w"));
    bitset<64> expected_text_encrypt("0010111010100111111001111101110111111000001010111101100011001110");
    string text = "test12";
    vector<bitset<48>> sub_keys = create_sub_keys(test_key);

    bitset<64> chiper_text = des_encrypt_text(text, sub_keys);

    ASSERT_EQ(chiper_text, expected_text_encrypt);
}