//
// Created by Alessandro Filino on 23/11/23.
//

#include <iostream>
#include <sys/time.h>
#include <math.h>
#include <cstring>

#include "utility.h"
#include "desKeyGenerator.h"
#include "desTextEncryptor.h"

using namespace std;

/*
 *  Dictiornary Attack
 *  Supponiamo di conoscere la chiave usata in DES e di voler cercare in un set di password se presente quella indicata
 *  Questo tipo di attacco è molto noto.
 *
 *  Password: 2/W.caaa
 *  3964,3 s dopo 1273791 50008053
 */
int main() {
    //Setup
    string text_key("A4rT9v.w");
    bitset<64> des_key(string_to_binary(text_key));
    // bitset<64> des_key(random_key_generator()); //Alternativa se non vogliamo inserire manualmente una chiave
    cout << "Chiave selezionata: " << text_key << " ----> " << des_key << endl;
    string password = "2/W.caaa";
    //stirng password = random_text_generator();
    vector<bitset<48>> sub_keys = create_sub_keys(des_key);
    bitset<64> chiper_password = des_encrypt_text(password, sub_keys);
    cout << "Password: " << password << " cifrata con DES ----> " << chiper_password << endl;

    int password_length = 8;
    char password_generate[password_length + 1]; //+1 per il carattere di terminazione \0
    long number_of_possible_passwords = (long)pow((double)allowed_char_size,(double)password_length);
    cout << "Total of possible password: " << number_of_possible_passwords << endl;

    cout << "Inizio attacco brute force ..." << endl;

    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    for (int i = 0; i < number_of_possible_passwords; i++) {
        generate_all_possible_password(password_generate, password_length, i);
        password_generate[password_length] = '\0';
        bitset<64> chiper_password_generate = des_encrypt_text(password_generate, sub_keys);
        cout << "Test [" << i << "] con la password: " << chiper_password_generate << " " << password_generate<<  endl;

        if (chiper_password == chiper_password_generate){
            cout << "Password trovata!" << endl;
            gettimeofday(&end_time, NULL);
            double total_time = ((end_time.tv_sec  - start_time.tv_sec) * 1000000u + start_time.tv_usec - end_time.tv_usec) / 1.e6;
            cout << "Tempo richiesto: " << total_time << " s" << endl;
            cout << "Attacco brute force terminato" << endl;
            break;
        }
    }

    return 0;
}
