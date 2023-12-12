//
// Created by Alessandro Filino on 01/12/23.
//

#ifndef DESKEYGENERATOR_H
#define DESKEYGENERATOR_H

#include <bitset>
#include <vector>

std::vector<std::bitset<48>> create_sub_keys(std::bitset<64> key);


#endif //DESKEYGENERATOR_H
