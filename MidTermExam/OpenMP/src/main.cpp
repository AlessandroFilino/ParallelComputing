//
// Created by Alessandro Filino on 01/12/23.
//

#include <iostream>
#include <omp.h>
#include <unistd.h>

#include "utility.h"
#include "desKeyGenerator.h"
#include "desTextEncryptor.h"

using namespace std;

/*
 *  Dictiornary Attack
 *  Supponiamo di conoscere la chiave usata in DES e di voler cercare in un set di password se presente quella indicata
 *  Questo tipo di attacco è molto noto.
 *
 * Password: aaab.W/2
 * 1 threads --> 3635.43 s
 * 2 threads  --> 1870.2 s
 * 3 threads --> 1274.33 s
 * 4 threads --> 952.182 s
 * 5 threads --> 763.445 s
 * 6 threads --> 644.883 s
 * 7 threads --> 618.588 s
 * 8 threads --> 595.119 s
 *
 * Password: 1234
 *
 */
int main(int argc, char *argv[]) {
    //Setup environment --> Abilitiamo Cancel for
    char *hasCancel = getenv("OMP_CANCELLATION");
    if (hasCancel == nullptr) {
        printf("Bootstrapping...");
        setenv("OMP_CANCELLATION", "true", 1);
        //setenv("OMP_DISPLAY_ENV", "true", 1); //Controllo a display per vedere lo stato delle variabili in openMP

        // Restart the program here
        int output = execvp(argv[0], argv);
        // Execution should not continue past here
        printf("Bootstrapping failed with code %d\n",output);
        exit(1);
    } else {
        puts("Bootstrapping complete");
    }

    // Setup
    string text_key("A4rT9v.w");
    bitset<64> des_key(string_to_binary(text_key));
    cout << "Chiave selezionata: " << text_key << " ----> " << des_key << endl;
    string password = "2/W.caaa";
    vector<bitset<48>> sub_keys = create_sub_keys(des_key);
    bitset<64> chiper_password = des_encrypt_text(password, sub_keys);
    cout << "Password: " << password << " cifrata con DES ----> " << chiper_password << endl;

    int password_length = 8;

    long number_of_possible_passwords = (long)pow((double)allowed_char_size, (double)password_length);

    // SETUP openMP
    int number_threads = 4;

    cout << "Inizio attacco brute force con: " << number_threads << " threads" << endl;
    double start_time = omp_get_wtime();

    #pragma omp parallel default(none) shared(chiper_password, sub_keys, start_time, password_length, number_of_possible_passwords, number_threads) num_threads(number_threads)
    {
        char password_generate[password_length + 1];
        /*
         *In questo modo facciamo dei chunk statici degli indici.
         * Ad esempio se number_of_possible_passwords fosse = 100 e stessi utilizzando 5 threads, ogni threads avrebbe 100/5 iterazioni da gestire
         */
        #pragma omp for schedule(static, number_threads)
        for (long i = 0; i < number_of_possible_passwords; i++) {
            generate_all_possible_password(password_generate, password_length, i);
            password_generate[password_length] = '\0';
            bitset<64> chiper_password_generate = des_encrypt_text(password_generate, sub_keys);

            if (chiper_password == chiper_password_generate) {
                double end_time = omp_get_wtime();
                double total_time = end_time - start_time;
                cout << "Password trovata!" << endl;
                cout << "Tempo richiesto: " << total_time << endl;
                cout << "Attacco brute force terminato" << " s" << endl;
                #pragma omp cancel for
            }
        #pragma omp cancellation point for
        }
    }


    return 0;
}

