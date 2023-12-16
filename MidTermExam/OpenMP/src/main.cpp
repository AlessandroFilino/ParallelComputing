//
// Created by Alessandro Filino on 01/12/23.
//

#include <iostream>
#include <omp.h>
#include <unistd.h>
#include <cmath>

#include "utility.h"
#include "desKeyGenerator.h"
#include "desTextEncryptor.h"

using namespace std;

/*
 *  Dictiornary Attack
 *  Supponiamo di conoscere la chiave usata in DES e di voler cercare in un set di password se presente quella indicata
 *  Questo tipo di attacco è molto noto.
 *
 * Password: 2/W.caaa
 * MAC M1 Pro 8 core (6 performance + 2 efficiency)
 * 1 threads --> 3635.43 s
 * 2 threads  --> 1870.2 s
 * 3 threads --> 1274.33 s
 * 4 threads --> 952.182 s
 * 5 threads --> 763.445 s
 * 6 threads --> 644.883 s
 * 7 threads --> 618.588 s
 * 8 threads --> 595.119 s
 *
 * Supponiamo di conoscere il numero totale di iterazioni necessarie per trovare quella password (50008053).
 * Proviamo quindi ad accorciare il ciclo for e a lasciare ad openMP la gestione degli indici.
 * Il risultato ci mostra rispetto al caso sequenziale si riesca comunque a  migliorare la prestazione ma non sia ottimizzato come l'assegnazione statica fatta in precedenza
 *
 * 1 threads --> 3888.58 s
 * 2 threads --> 4030.15 s
 * 3 threads --> 4073.9 s
 * 4 threads --> 4092.4 s
 * 5 threads --> 4183.47 s
 * 6 threads --> 4673.27 s
 * 7 threads --> 5979.54 s
 * 8 threads --> 9127.22 s
 *
 *
 * LINUX Intel(R) Xeon (R) Silver 4314 CPU @ 2.40GHz (16 core 32 threads)
 * 1 threads --> 4461.75 s
 * 2 threads --> 2236.64 s
 * 3 threads --> 1493.41 s
 * 4 threads --> 1124.49 s
 * 5 threads --> 901.942 s
 * 6 threads --> 756.772 s
 * 7 threads --> 649.167 s
 * 8 threads --> 594.315 s
 * 9 threads --> 521.07 s
 * 10 threads --> 469.617 s
 * 11 threads --> 434.817 s
 * 12 threads --> 398.885 s
 * 13 threads --> 373.993 s
 * 14 threads --> 349.434 s
 * 15 threads --> 327.47 s
 * 16 threads --> 309.461 s
 * 17 threads --> 286.749 s
 * 18 threads --> 279.44 s
 * 19 threads --> 268.801 s
 * 20 threads --> 250.751 s
 * 21 threads --> 236.494 s
 * 22 threads --> 235.226 s
 * 23 threads --> 219.68 s
 * 24 threads --> 215.853 s
 * 25 threads --> 209.417 s
 * 26 threads --> 199.965 s
 * 27 threads --> 194.025 s
 * 28 threads --> 186.961 s
 * 29 threads --> 184.563 s
 * 30 threads --> 174.373 s
 * 31 threads --> 171.084 s
 * 32 threads --> 164.497 s
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
    bitset<64> cipher_password = des_encrypt_text(password, sub_keys);
    cout << "Password: " << password << " cifrata con DES ----> " << cipher_password << endl;

    int password_length = 8;

    long number_of_possible_passwords = (long)pow((double)allowed_char_size, (double)password_length);

    // SETUP openMP
    int number_threads = 16;

    cout << "Inizio attacco brute force con: " << number_threads << " threads" << endl;
    double start_time = omp_get_wtime();

    #pragma omp parallel default(none) shared(cipher_password, sub_keys, start_time, password_length, number_of_possible_passwords, number_threads, cout) num_threads(number_threads)
    {
        char password_generate[password_length + 1];
        /*
         * In questo modo facciamo dei chunk statici degli indici.
         * Ad esempio se number_of_possible_passwords fosse = 100 e stessi utilizzando 5 threads, ogni threads avrebbe 100/5 iterazioni da gestire
         */
        #pragma omp for schedule(static, number_threads)
        for (long i = 0; i < number_of_possible_passwords; i++) {
            generate_all_possible_password(password_generate, password_length, i);
            password_generate[password_length] = '\0';
            bitset<64> cipher_password_generate = des_encrypt_text(password_generate, sub_keys);

            if (cipher_password == cipher_password_generate) {
                double end_time = omp_get_wtime();
                double total_time = end_time - start_time;
                cout << "Password trovata!" << endl;
                cout << "Tempo richiesto: " << total_time << " s" << endl;
                cout << "Attacco brute force terminato" << endl;
                #pragma omp cancel for
            }
            #pragma omp cancellation point for
        }
    }

    return 0;
}

