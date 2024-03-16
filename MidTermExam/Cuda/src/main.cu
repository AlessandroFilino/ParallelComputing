#include <iostream>
#include <cuda_runtime.h>
#include <sys/time.h>
#include "desKeyGenerator.h"
#include "desTextEncryptor.h"
#include "utility.h"

using namespace std;

__device__ int d_initial_permutation_table[64] = {
        58, 50, 42, 34, 26, 18, 10, 2,
        60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6,
        64, 56, 48, 40, 32, 24, 16, 8,
        57, 49, 41, 33, 25, 17,  9, 1,
        59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5,
        63, 55, 47, 39, 31, 23, 15, 7
};

__device__ int d_expansion_permutation_table[48] = {
        32, 1, 2, 3, 4, 5, 4, 5,
        6, 7, 8, 9, 8, 9, 10, 11,
        12, 13, 12, 13, 14, 15, 16, 17,
        16, 17, 18, 19, 20, 21, 20, 21,
        22, 23, 24, 25, 24, 25, 26, 27,
        28, 29, 28, 29, 30, 31, 32, 1
};

__device__ int d_s_box_table[8][4][16] = {
        {
                14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
                0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
                4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
                15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13
        },
        {
                15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
                3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
                0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
                13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9
        },
        {
                10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
                13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
                13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
                1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12
        },
        {
                7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
                13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
                10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
                3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14
        },
        {
                2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
                14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
                4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
                11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3
        },
        {
                12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
                10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
                9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
                4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13
        },
        {
                4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
                13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
                1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
                6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12
        },
        {
                13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
                1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
                7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
                2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
        }
};

__device__ int d_straight_permutation_table[32] = {
        16,  7, 20, 21,
        29, 12, 28, 17,
        1, 15, 23, 26,
        5, 18, 31, 10,
        2,  8, 24, 14,
        32, 27,  3,  9,
        19, 13, 30,  6,
        22, 11,  4, 25
};

__device__ int d_final_permutation_table[64] = {
        40, 8, 48, 16, 56, 24, 64, 32,
        39, 7, 47, 15, 55, 23, 63, 31,
        38, 6, 46, 14, 54, 22, 62, 30,
        37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58, 26,
        33, 1, 41, 9, 49, 17, 57, 25
};

__device__ char* cuda_des_encrypt_text(char* bin_plain_text, char* sub_keys, char* cipher_text, int blockSize, int threads_number,
    char* result_initial_permutation, char* left_block, char* right_block, char* right_expanded, char* xor_result, 
    char* block, char* s_box_result, char* s_box_permuted_result, char* new_left_block, char* combined_key) {

    int index = blockIdx.x * blockSize + threadIdx.x;

    for (int i = 0; i < 64; i++) {
        result_initial_permutation[index * 64 + i] = bin_plain_text[d_initial_permutation_table[i] - 1];
    }

    for (int i = 0; i < 32; i++) {
        left_block[index * 32 + i] = result_initial_permutation[index * 64 + i];
        right_block[index * 32 + i] = result_initial_permutation[index * 64 + 32 + i];
    }

    for (int round = 0; round < 16; round ++) {
        for (int i = 0; i < 48; i++) {
            right_expanded[index * 48 + i] = right_block[index * 32 + d_expansion_permutation_table[i] - 1];
        }

        for (int i = 0; i < 48; i++) {
            xor_result[index * 48 + i] = sub_keys[round * 48 + i] ^ (right_expanded[index * 48 + i] - '0');
        }

        for (int i = 0; i < 8; i++){
            for (int j = 0; j < 6; j++){
                block[j] = xor_result[index * 48 + i * 6 + j];
            }

            int row = (block[0] - '0') * 2 + (block[5] - '0');
            int col = (block[1] - '0') * 8 + (block[2] - '0') * 4 + (block[3] - '0') * 2 + (block[4] - '0');

            int s_box_value = d_s_box_table[i][row][col];
            
            for (int k = 0; k < 4; k++) {
                s_box_result[index * 32 + i * 4 + k] = ((s_box_value >> (3 - k)) & 1) + '0';
            }
        }

        for (int i = 0; i < 32; i++) {
            s_box_permuted_result[index * 32 + i] = s_box_result[index * 32 + d_straight_permutation_table[i] - 1];
        }

        for (int i = 0; i < 32; i++) {
            new_left_block[index * 32 + i] = (left_block[index * 32 + i] - '0') ^ (s_box_permuted_result[index * 32 + i] - '0') + '0';
        }

        if (round != 15) {
            for (int i = 0; i < 32; i++) {
                left_block[index * 32 + i] = right_block[index * 32 + i];
                right_block[index * 32 + i] = new_left_block[index * 32 + i];
            }
        } else {
            for (int i = 0; i < 32; i++) {
                left_block[index * 32 + i] = new_left_block[index * 32 + i];
            }
        }
    }
    
    for (int i = 0; i< 32; i++) {
        combined_key[index * 64 + i] = left_block[index * 32 + i];
        combined_key[index * 64 + 32 + i] = right_block[index * 32 + i];
    }

    for (int i = 0; i < 64; i++) {
        cipher_text[index * 64 + i] = combined_key[index * 64 + d_final_permutation_table[i] - 1];
    }

    return cipher_text;
}

__device__ char* generate_all_possible_password(char* password, int password_length, int blockSize, long iteration) {
    unsigned int index = blockIdx.x * blockSize + threadIdx.x;
    for (int j = 0; j < password_length - 1; j++) {
        password[index * password_length + j] = d_allowed_char[iteration % allowed_char_size];
        iteration /= allowed_char_size;
    } 

    //JUST FOR TESTING
    /*
    char prev[8];
    for(int i=0; i< 7; i++){
        prev[i] = password[index*password_length + i];
    } 
    printf("thread n. %d pwd: %s\n",index, prev);
    */

    return password;
}

__device__ char* d_string_to_binary(char* string_text, int password_length, char* bin_text, int blockSize) {
    unsigned int index = blockIdx.x * blockSize + threadIdx.x;

    int start_index = 0;
    for (int i = 0; i < password_length - 1; i++) {
        for (int j = 7; j >= 0; --j) {
            bin_text[(index * password_length * 8) + (i * 8) + (7 - j)] = ((string_text[i] >> j) & 1) ? '1' : '0';
        }
    }

    return bin_text;
}

__device__ bool isBinaryStringEqual(char* string1, char* string2, int blockSize){
    unsigned int index = blockIdx.x * blockSize + threadIdx.x;
    bool isEqual = true;
    for (int i= 0; i < 63; i++) {
        if(string1[(index * 64) + i] != string2[i]){
            isEqual = false;
        }
    }

    return isEqual;
}

__global__ void brute_force_attack(char* cipher_password_target, char* sub_keys_1d, int blockSize, int threads_number, 
    int password_length, char* current_password, char* bin_current_password, char* cipher_current_password,
    char* result_initial_permutation, char* left_block, char* right_block, char* right_expanded, char* xor_result, 
    char* block, char* s_box_result, char* s_box_permuted_result, char* new_left_block, char* combined_key) {
    
    unsigned int index = blockIdx.x * blockSize + threadIdx.x;
    long number_of_possible_passwords = (long)pow((double)allowed_char_size,(double)password_length);
    bool password_found = false;
    
    long password_per_thread = number_of_possible_passwords / threads_number;
    long start_index = index * password_per_thread;
    long end_index = password_per_thread + start_index;
    
    for (long i = start_index; i < end_index; i++){
        generate_all_possible_password(current_password, password_length, blockSize, i);
        d_string_to_binary(current_password, password_length, bin_current_password, blockSize);
        cuda_des_encrypt_text(bin_current_password, sub_keys_1d, cipher_current_password, blockSize, threads_number,
            result_initial_permutation, left_block, right_block, right_expanded, xor_result, 
            block, s_box_result, s_box_permuted_result, new_left_block, combined_key);
        
        if (isBinaryStringEqual(cipher_current_password, cipher_password_target, blockSize)){
            password_found = true;
            printf("Password is found! \n");
            return;
        }
        

    } 
}

void getGPUProperties(int gpuID) {
    cudaDeviceProp prop;
    int deviceId = gpuID; // ID della tua GPU, potrebbe essere diverso se hai più di una GPU installata

    cudaGetDeviceProperties(&prop, deviceId);

    std::cout << "Proprietà della GPU:" << std::endl;
    std::cout << "Nome: " << prop.name << std::endl;
    std::cout << "Massimo numero di thread per blocco: " << prop.maxThreadsPerBlock << std::endl;
}

int main() {

    //SETUP DES
    const char* key = "A4rT9v.w";
    char* des_key = (char*) malloc(64 * sizeof(char));
    string_to_binary(key, des_key);

    cout << "Binary representation of the key'" << key << "': " << des_key << endl;
    
    char** sub_keys = create_sub_keys(des_key);
    char sub_keys_1d[16 * 48]; //creiamo un array 1D in cui inseriamo le chiavi da trasferire in GPU
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 48; j++) {
            sub_keys_1d[(i * 48) + j] = sub_keys[i][j];  
        }
    }

    cout << endl;

    //SETUP TARGET PASSWORD
    const char* password = "Zaaaaaaa";
    char* cipher_password_target = des_encrypt_text(password, sub_keys_1d);
    cout << "Password to find: '" << password << "' encrypted with DES: " << cipher_password_target << endl;

    //SETUP CUDA
    //getGPUProperties(0); //Get info
    unsigned int threads_number = 1;
    int blockSize = 32;

    //Setup block
    int numBlocks;
    int threads_per_block;
    if (threads_number <= 32) {
        numBlocks = 1;
        threads_per_block = threads_number;
    } else if (threads_number % 128 == 0) {
        numBlocks = threads_number / 128;
        threads_per_block = 128;
    } else {
        numBlocks = (threads_number + 31) / 32;  // Arrotonda al prossimo multiplo di 32
        threads_per_block = 32;
    }
    printf("Setup: <<<%d,%d>>>\n", numBlocks, threads_per_block);
    
    int password_length = 8;  //length + 1 for \0
    char* current_password;
    char* bin_current_password;
    char* cipher_current_password;
    char* result_initial_permutation;
    char* left_block;
    char* right_block;
    char* right_expanded;
    char* xor_result;
    char* block; 
    char* s_box_result;
    char* s_box_permuted_result;
    char* new_left_block;
    char* combined_key;

    cudaMalloc((void**)&current_password, (threads_number * (password_length +1) * sizeof(char)));
    cudaMalloc((void**)&bin_current_password, (threads_number * 64 * sizeof(char)));
    cudaMalloc((void**)&cipher_current_password, (threads_number * 64 * sizeof(char)));
    cudaMalloc((void**)&result_initial_permutation, (threads_number * 64 * sizeof(char)));
    cudaMalloc((void**)&left_block, (threads_number * 32 * sizeof(char)));
    cudaMalloc((void**)&right_block, (threads_number * 32 * sizeof(char)));
    cudaMalloc((void**)&right_expanded, (threads_number * 48 * sizeof(char)));
    cudaMalloc((void**)&xor_result, (threads_number * 48 * sizeof(char)));
    cudaMalloc((void**)&block, (threads_number * 6 * sizeof(char)));
    cudaMalloc((void**)&s_box_result, (threads_number * 32 * sizeof(char)));
    cudaMalloc((void**)&s_box_permuted_result, (threads_number * 32 * sizeof(char)));
    cudaMalloc((void**)&new_left_block, (threads_number * 32 * sizeof(char)));
    cudaMalloc((void**)&combined_key, (threads_number * 64 * sizeof(char)));

    char* d_cipher_password_target;
    cudaMalloc((void**)&d_cipher_password_target, (threads_number * 64 * sizeof(char)));
    cudaMemcpy(d_cipher_password_target, cipher_password_target, 64 * sizeof(char), cudaMemcpyHostToDevice);

    char* d_sub_keys;
    cudaMalloc((void**)&d_sub_keys, (16 * 48 * sizeof(char)));
    cudaMemcpy(d_sub_keys, sub_keys_1d, 16 * 48 * sizeof(char), cudaMemcpyHostToDevice);

    long number_of_possible_passwords = (long)pow((double)allowed_char_size,(double)(password_length));
    cout << "Total of possible password: " << number_of_possible_passwords << " with: " << password_length << " characters" << endl;
    cout << endl;
    cout << "Brute force attack started ..." << endl;
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    cout << endl;

    brute_force_attack<<<numBlocks, threads_per_block>>>(d_cipher_password_target, d_sub_keys, blockSize, threads_number, 
        (password_length +1), current_password, bin_current_password, 
        cipher_current_password, result_initial_permutation, left_block, right_block, right_expanded, xor_result, 
        block, s_box_result, s_box_permuted_result, new_left_block, combined_key);
    cudaDeviceSynchronize();
    cudaError_t error = cudaGetLastError();
    if (error != cudaSuccess) {
        cout << "Errore CUDA: " << cudaGetErrorString(error) << endl;
    }

    cout << endl;

    gettimeofday(&end_time, NULL);
    double total_time = ((end_time.tv_sec  - start_time.tv_sec) * 1000000u + end_time.tv_usec - start_time.tv_usec) / 1.e6;
    cout << "Tempo richiesto: " << total_time << " s" << endl;
    cout << "Attacco brute force terminato" << endl;


    cudaFree(current_password);
    cudaFree(bin_current_password);
    cudaFree(cipher_current_password);
    cudaFree(result_initial_permutation);
    cudaFree(left_block);
    cudaFree(right_block);
    cudaFree(right_expanded);
    cudaFree(xor_result);
    cudaFree(block);
    cudaFree(s_box_result);
    cudaFree(s_box_permuted_result);
    cudaFree(new_left_block);
    cudaFree(combined_key);
    cudaFree(d_sub_keys);
    cudaFree(d_cipher_password_target);

    return 0;
}
