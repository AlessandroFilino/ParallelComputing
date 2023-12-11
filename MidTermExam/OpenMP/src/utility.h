//
// Created by Alessandro Filino on 01/12/23.
//

#ifndef UTILITY_H
#define UTILITY_H

#include <string>

using namespace std;

/*
 * Definiamo i possibili caratteri da poter utilizzare all'interno di una password
 */
static char allowed_char [] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
                               'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
                               'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
                               'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F',
                               'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
                               'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
                               'W', 'X', 'Y', 'Z', '1', '2', '3', '4',
                               '5', '6', '7', '8', '9', '0', '.', '/'};
const int allowed_char_size = sizeof(allowed_char) / sizeof(allowed_char[0]);

template <size_t T>
bitset<T> shift_left(bitset<T> bit_text, unsigned int number_of_shift);
template <size_t N, size_t T>
bitset<T> make_permutation(const bitset<N>& bin_text, const int* permutation_table);
template <size_t T>
bitset<T> xor_function(const bitset<T>& a, const bitset<T>& b);
template <size_t T>
vector<bitset<T/2>> splitting_bitset(const bitset<T>& bin_text);
bitset<64> random_key_generator();
string random_text_generator();
bitset<64> string_to_binary(const string& string_text);
string binary_to_string(const bitset<64> binary_text);

void generate_all_possible_password(char password[], int password_length, long iteration);
#endif //UTILITY_H
