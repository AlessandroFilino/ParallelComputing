//
// Created by Alessandro Filino on 01/12/23.
//

#ifndef DESTEXTENCRYPTOR_H
#define DESTEXTENCRYPTOR_H

#include <bitset>
#include <string>
#include <vector>

std::bitset<64> des_encrypt_text(std::string plain_text, std::vector<std::bitset<48>> sub_keys);

#endif //DESTEXTENCRYPTOR_H
