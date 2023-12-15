//
// Created by Alessandro Filino on 02/12/23.
//

#include <iostream>
#include <gtest/gtest.h>
#include <string>
#include <sys/time.h>

#include "desTextEncryptor.h"
#include "desKeyGenerator.h"
#include "utility.h"

using namespace std;

/*
 *  Valutiamo quanto tempo è necessario per cifrare n password con l'algoritmo sequenziale
 *  In questo caso non è necessario l'utilizzo della google test lib
 *  773.759 s
 */

TEST(des, test_time_to_encrypt_one_milion_password) {
    bitset<64> des_key(random_key_generator()); //Alternativa se non vogliamo inserire manualmente una chiave
    cout << "Chiave selezionata: "  << des_key << endl;
    vector<bitset<48>> sub_keys = create_sub_keys(des_key);

    int password_length = 8;
    char password_generate[password_length + 1]; //+1 per il carattere di terminazione \0
    long number_of_possible_passwords = 10000000;
    // cout << "Total of possible password: " << number_of_possible_passwords << endl;

    cout << "Inizio cifratura di "<< number_of_possible_passwords << " ..." << endl;
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);
    for (int i = 0; i < number_of_possible_passwords; i++) { // 1000000000
        generate_all_possible_password(password_generate, password_length, i);
        password_generate[password_length] = '\0';
        bitset<64> chiper_password_generate = des_encrypt_text(password_generate, sub_keys);
        cout << "[" << i <<"] Cifro la password: " << password_generate << " ----> " << chiper_password_generate << endl;

    }
    gettimeofday(&end_time, NULL);
    double total_time = ((end_time.tv_sec  - start_time.tv_sec) * 1000000u + end_time.tv_usec - start_time.tv_usec) / 1.e6;
    cout << "Operazione Completata" << endl;
    cout << "Tempo richiesto: " << total_time << " s" << endl;

}