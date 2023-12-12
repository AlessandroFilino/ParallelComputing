//
// Created by Alessandro Filino on 23/11/23.
//

#include <iostream>
#include <string>
#include <bitset>
#include <cstring>
#include "utility.h"

using namespace std;

/*
 *  Metodo per eseguire gli shift circolari verso sinistra richiesti dall'algoritmo.
 *  text --> stringa binaria iniziale da shiftare
 *  number_of_shift --> numero di sift da eseguire (dipenderanno dal round).
 */
template <size_t T>
bitset<T> shift_left(bitset<T> bit_text, unsigned int number_of_shift){
    return (bit_text << number_of_shift) | (bit_text >> (T - number_of_shift));
}
// Istanziazione del template per i valori necessari
template bitset<64> shift_left(bitset<64> bit_text, unsigned int number_of_shift);
template bitset<28> shift_left(bitset<28> bit_text, unsigned int number_of_shift);

/*
 *  Template per effettuare le permutazioni
 *  N --> Dimensione in bit iniziale
 *  T --> Dimensione in bit finale
 *  bin_text --> Input binario su cui vogliamo eseguire una permutazione
 *  permutation_table --> Array di interi che seguirà il metodo per effettuare la permutazione
 */

template <size_t N, size_t T>
bitset<T> make_permutation(const bitset<N>& bin_text, const int* permutation_table){
    bitset<T> new_bin_text;
    for (int i = 0; i < T; i++){
        new_bin_text.set((T-i)-1, bin_text[N - (permutation_table[i] - 1) -1]);
    }
    return new_bin_text;
}

template bitset<64> make_permutation(const bitset<64>& key, const int* permutation_table);
template bitset<56> make_permutation(const bitset<64>& key, const int* permutation_table);
template bitset<48> make_permutation(const bitset<56>& key, const int* permutation_table);
template bitset<32> make_permutation(const bitset<32>& key, const int* permutation_table);
template bitset<48> make_permutation(const bitset<32>& key, const int* permutation_table);
//template bitset<6> make_permutation(const bitset<6>& key, const int* permutation_table); //Solo per testing

/*
 *  XOR Function
 *  Prendiamo due stringhe binare a e b ed eseguiamo l'operazione xor tra di esse
 */
template <size_t T>
bitset<T> xor_function(const bitset<T>& a, const bitset<T>& b){
    return a ^ b;
}
template bitset<64> xor_function(const bitset<64>& a, const bitset<64>& b);
template bitset<48> xor_function(const bitset<48>& a, const bitset<48>& b);
template bitset<32> xor_function(const bitset<32>& a, const bitset<32>& b);
/*
 * splitting_bitset
 * Questo template prende una stringa bitset e restuisce due sottoparti
 */
template <size_t T>
vector<bitset<T/2>> splitting_bitset(const bitset<T>& bin_text){
    vector<bitset<T/2>> split_bitsets;

    bitset<T/2> left(bin_text.to_ulong() >> T/2);
    split_bitsets.push_back(left);

    bitset<T/2> right(bin_text.to_ulong() & ((1ull << T/2) - 1));
    split_bitsets.push_back(right);

    return split_bitsets;
}
template vector<bitset<56/2>> splitting_bitset(const bitset<56>& bin_text);
template vector<bitset<64/2>> splitting_bitset(const bitset<64>& bin_text);

bitset<64> string_to_binary(const string& string_text){
    bitset<64> bin_text;

    for (char c : string_text) {
        int asciiValue = static_cast<int>(c);
        bin_text = (bin_text << 8) | bitset<64>(asciiValue);
    }

    return bin_text;
}

string binary_to_string(const bitset<64> binary_text){
    string string_text;

    for (int i = 56; i >= 0; i -= 8) {
        // Prendiamo i prossimi 8 bit del bitset
        bitset<8> byte((binary_text >> i).to_ullong());

        // Convertiamo gli 8 bit presi in char
        char asciiChar = static_cast<char>(byte.to_ulong());

        // Aggiungi il carattere alla stringa
        string_text.push_back(asciiChar);
    }

    return string_text;
}

/*
 * Generatore di chiave.
 * Genera una chiave a 64 bit ossia una stringa composta da 8 char
 */
bitset<64> random_key_generator(){
    bitset<64> random_key;

    srand(static_cast<unsigned int>(time(0))); //Resettimo il generatore di numeri casuali. Se commentassimo questa riga, otterremo sempre la stessa chiave

    for (int i = 0; i < 64; ++i) {
        // Genera un bit casuale (0 o 1)
        int randomBit = rand() % 2;
        random_key[i] = randomBit;
    }

    return random_key;
}

/*
 * Generatore di testo composto da 8 caratteri scelti tra quelli disponibili (definiti nell'array allowed_char)
 */
string random_text_generator(){
    string random_text;

    srand(static_cast<unsigned int>(time(0))); //Resettimo il generatore di numeri casuali. Se commentassimo questa riga, otterremo sempre la stessa password

    for (int i = 0; i < 8; ++i) {
        int randomPosition = rand() % (strlen(allowed_char) - 1);
        random_text += allowed_char[randomPosition];
    }

    return random_text;
}

/*
 * Data una lunghezza, la funzione è in grado di generare tutte le possibili password dato l'array allowed_char
 * Argomenti:
 * - password: --> Passowrd che genereremo via via
 * - password_length
 * - iteration --> Contatore da cui ripartire a generare
 */
void generate_all_possible_password(char password[], int password_length, long iteration){
    for (int j = 0; j < password_length ; j++){
        password[j] = allowed_char[iteration % allowed_char_size];
        iteration /= allowed_char_size;
    }
}

