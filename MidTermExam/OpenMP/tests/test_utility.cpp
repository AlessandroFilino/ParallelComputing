//
// Created by Alessandro Filino on 24/11/23.
//
#include <iostream>
#include <gtest/gtest.h>
#include <algorithm>

#include "utility.h"

using namespace std;

/*
 *  KeyGenerator
 *  Proviamo a generare una chiave a 64 bit (ossia 8 char) generica. Questa sarà la chiave che utilizzeremo nel nostro algoritmo
 */
TEST(utility, generate_random_key){
    bitset<64> random_key = random_key_generator();

    ASSERT_EQ(random_key_generator().size(), 64);
}

/*
 *  xor_function
 *  Testiano la funzione xor
 *  Definiamo due stringhe binarie e verifichiamo che il risultato sia quello atteso
 */
TEST(utilty, test_xor_function){
    bitset<64> a("1100110010");
    bitset<64> b("1010101010");
    bitset<64> expected_result ("0110011000");
    bitset<64> result = xor_function(a, b);

    ASSERT_EQ(result, expected_result);
}

/*
 * Usiamo una chiave statica così da conoscerne la definizione binaria e ascii a priori per i test. In ascii questa corrisponde a A4rT9v.w
 */
static bitset<64> key("0100000100110100011100100101010000111001011101100010111001110111");

/*
 *  text_to_bin
 *  Testiamo la funzione che trasforma una qualsiasi stringa di testo in binario
 */
TEST(utility, test_text_to_bin){
    bitset<64> bin_static_key = string_to_binary("A4rT9v.w");
    ASSERT_EQ(bin_static_key, key);
}

/*
 *  bin_to_test
 *  Testiamo la funzione che trasforma una qualsiasi stringa binaria in testo
 */
TEST(utility, test_bin_to_text){
    string text_static_key = binary_to_string(key);

    ASSERT_EQ(text_static_key, "A4rT9v.w");
}

/*
 *  Testiamo la funzione di shift per una stirnga generica.
 *  Data la stringa di partenza "A4rT9v.w" che in binario sappiamo corrispondere a 0100000100110100011100100101010000111001011101100010111001110111,
 *  effettuiamo uno shift verso sinistra.
 *
 */
TEST(desKeyGenerator, test_shift_left_one_position){
    bitset<64> expected_result("1000001001101000111001001010100001110010111011000101110011101110");

    bitset<64> shifted_test_text = shift_left(key, 1);
    ASSERT_EQ(shifted_test_text, expected_result);
}

/*
 *  Facciamo lo stesto test ma effettuando due shift verso sinistra.
 *  In questo caso, considerando la stringa iniziale "A4rT9v.w", ci aspettiamo un risultato del tipo: "rT9v.wA4"
 */
TEST(desKeyGenerator, test_shift_left_two_position){
    bitset<64> expected_result("0000010011010001110010010101000011100101110110001011100111011101");
    bitset<64> shifted_test_text = shift_left(key, 2);
    ASSERT_EQ(shifted_test_text, expected_result);
}

/*
 *  make_permutation
 *  Prendiamo una semplice successione di 1 e 0 di 64 bit.
 *  Applichiamo quindi una semplce permutazioen che scambi l'elemento i contenente il valore 1 con l'elemento i+1 contenente il valore 0.
 *  Quello che dovremmo otenere è una stringa di 64 bit dove l'ordine degli 1 e degli 0 sarà invertito rispetto a quella di partenza
 *
 *  N.B test_make_permutation_simple è una prima prova fatta con una piccola sequenza di bit di cui si poteva già conoscere il risultato.
 *  È stata commentata per non aumentare il numero dei tipi di template per make_permutation
 /*
TEST(utility, test_make_permutation_simple){
    static int test_permutation_table[6] {
      1, 3, 2,
      5, 4, 6
    };
    bitset<6> bin_iniziale("101101");
    bitset<6> expected_result("110011");
    bitset<6> bin_permutato = make_permutation<6, 6>(bin_iniziale, test_permutation_table);
    ASSERT_EQ(bin_permutato, expected_result);
}*/
TEST(utility, test_make_permutation){
    static int swap_table[64] = {
            2, 1, 4, 3, 6, 5, 8, 7,
            10, 9, 12, 11, 14, 13, 16, 15,
            18, 17, 20, 19, 22, 21, 24, 23,
            26, 25, 28, 27, 30, 29, 32, 31,
            34, 33, 36, 35, 38, 37, 40, 39,
            42, 41, 44, 43, 46, 45, 48, 47,
            50, 49, 52, 51, 54, 53, 56, 55,
            58, 57, 60, 59, 62, 61, 64, 63
    };

    bitset<64> bin_string("0101010101010101010101010101010101010101010101010101010101010101");
    bitset<64> expected_result("1010101010101010101010101010101010101010101010101010101010101010");
    bitset<64> result = make_permutation <64, 64>(bin_string, swap_table);

    ASSERT_EQ(result, expected_result);
}

/*
 *  Eseguiamo lo stesso test fatto in precedenza.
 *  In questo caso partiamo da una chiave a 64 bit e proviamo a ridurla a 56 per poi applicarci una permutazione di tale grandezza
 */
TEST(utility, test_make_permutation_with_table_smaller_size){
    static int swap_table[56] = {
            2, 1, 4, 3, 6, 5, 8, 7,
            10, 9, 12, 11, 14, 13, 16, 15,
            18, 17, 20, 19, 22, 21, 24, 23,
            26, 25, 28, 27, 30, 29, 32, 31,
            34, 33, 36, 35, 38, 37, 40, 39,
            42, 41, 44, 43, 46, 45, 48, 47,
            50, 49, 52, 51, 54, 53, 56, 55
    };

    bitset<64> bin_string("0101010101010101010101010101010101010101010101010101010101010101");
    bitset<56> expected_result("10101010101010101010101010101010101010101010101010101010");
    bitset<56> result = make_permutation<64, 56>(bin_string, swap_table);

    ASSERT_EQ(result, expected_result);
}

/*
 * Testiamo il generatore di tutte le password
 * In particolare verifichiamo che gli elementi contenuti siano tutti diversi
 */
TEST(utility,test_generate_all_possible_password_error){
    int password_length = 8;
    char password[password_length + 1] ;
    long number_of_possible_passwords = (long)pow((double)allowed_char_size, (double)password_length);

    vector<string> passwords;
    passwords.push_back("aaaaaaaa"); //Aggiungo un elemento che sarà poi generato
    for (int i = 0; i < number_of_possible_passwords; ++i) {
        generate_all_possible_password(password, password_length, i);
        password[password_length] = '\0';
        passwords.push_back(password);
    }

    //sort(passwords.begin(), passwords.end()); //Necessario per un test migliore
    bool unique = std::unique(passwords.begin(), passwords.end()) == passwords.end();

    ASSERT_FALSE(unique);
}

TEST(utility,test_generate_all_possible_password){
    int password_length = 8;
    char password[password_length + 1] ;
    long number_of_possible_passwords = (long)pow((double)allowed_char_size, (double)password_length);

    vector<string> passwords;
    for (int i = 0; i < number_of_possible_passwords; ++i) {
        generate_all_possible_password(password, password_length, i);
        password[password_length] = '\0';
        passwords.push_back(password);
    }

    //sort(passwords.begin(), passwords.end()); //Necessario per un test migliore
    bool unique = std::unique(passwords.begin(), passwords.end()) == passwords.end();

    ASSERT_TRUE(unique);
}