//
// Created by Alessandro Filino on 02/12/23.
//

#include <iostream>
#include <gtest/gtest.h>
#include <string>
#include <omp.h>

#include "desTextEncryptor.h"
#include "desKeyGenerator.h"
#include "utility.h"

using namespace std;

/*
 *  Valutiamo quanto tempo è necessario per cifrare n password con l'algoritmo sequenziale
 *  In questo caso non è necessario l'utilizzo della google test lib
 *  123.237 s
 */
TEST(des, test_time_to_encrypt_one_milion_password) {
    bitset<64> des_key(random_key_generator()); //Alternativa se non vogliamo inserire manualmente una chiave
    cout << "Chiave selezionata: "  << des_key << endl;
    vector<bitset<48>> sub_keys = create_sub_keys(des_key);

    int password_length = 8;
    char password_generate[password_length + 1]; //+1 per il carattere di terminazione \0
    long number_of_possible_passwords = 10000000; //10000000
    // cout << "Total of possible password: " << number_of_possible_passwords << endl;

    //SETUP openMP
    int number_threads = 8;
    cout << "Inizio cifratura di "<< number_of_possible_passwords << " password ..." << endl;
    double start_time = omp_get_wtime();

    #pragma omp parallel default(none) shared(sub_keys, password_length, number_of_possible_passwords, password_generate, number_threads) num_threads(number_threads)
    {
        #pragma omp for schedule(static, number_threads)
        for (long i = 0; i < number_of_possible_passwords; i++) {
            int thread_id = omp_get_thread_num();
            long my_unique_i = i + thread_id * number_of_possible_passwords;

            generate_all_possible_password(password_generate, password_length, my_unique_i);
            password_generate[password_length] = '\0';
            bitset<64> cipher_password_generate = des_encrypt_text(password_generate, sub_keys);
        }
    }

    double end_time = omp_get_wtime();
    double total_time = end_time - start_time;
    cout << "Operazione Completata" << endl;
    cout << "Tempo richiesto: " << total_time << " s" << endl;
}
