//
// Created by Alessandro Filino on 01/12/23.
//

#ifndef DESTEXTENCRYPTOR_H
#define DESTEXTENCRYPTOR_H

#include <bitset>

using namespace std;

bitset<64> des_encrypt_text(string plain_text, vector<bitset<48>> sub_keys);

#endif //DESTEXTENCRYPTOR_H
