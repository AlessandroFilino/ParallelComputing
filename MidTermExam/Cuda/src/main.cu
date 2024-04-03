#include <iostream>
#include <cuda_runtime.h>
#include <sys/time.h>

#include "desKeyGenerator.h"
#include "desTextEncryptor.h"
#include "utility.h"

using namespace std;

__device__ const char d_allowed_char [] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F',
'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
'W', 'X', 'Y', 'Z', '1', '2', '3', '4',
'5', '6', '7', '8', '9', '0', '.', '/'};
__device__ long d_allowed_char_size = sizeof(d_allowed_char) / sizeof(d_allowed_char[0]);

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

__device__ int* cuda_des_encrypt_text(int* bin_plain_text, int* sub_keys, int* cipher_text, int blockSize, int threads_number,
    int* result_initial_permutation, int* left_block, int* right_block, int* right_expanded, int* xor_result, 
    int* block, int* s_box_result, int* s_box_permuted_result, int* new_left_block, int* combined_key) {
    
    unsigned int index = blockIdx.x * blockSize + threadIdx.x;
    
    for (int i = 0; i < 64; i++) {
        result_initial_permutation[index * 64 + i] = bin_plain_text[index * 64 + d_initial_permutation_table[i] - 1]; 
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
            xor_result[index * 48 + i] = right_expanded[index * 48 + i] ^ sub_keys[round * 48 + i];
        }

        for (int i = 0; i < 8; i++) {
            int row = (xor_result[index * 48 + i * 6] << 1) + xor_result[index * 48 + i * 6 + 5];
            int col = (xor_result[index * 48 + i * 6 + 1] << 3) + (xor_result[index * 48 + i * 6 + 2] << 2) + (xor_result[index * 48 + i * 6 + 3] << 1) + xor_result[index * 48 + i * 6 + 4];
            int val = d_s_box_table[i][row][col];
            for (int j = 0; j < 4; j++) {
                s_box_result[index * 32 + i * 4 + j] = (val >> (3 - j)) & 1;
            }
        }

        for (int i = 0; i < 32; i++) {
            s_box_permuted_result[index * 32 + i] = s_box_result[index * 32 + d_straight_permutation_table[i] - 1];
        }

        for (int i = 0; i < 32; i++) {
            new_left_block[index * 32 + i] = left_block[index * 32 + i] ^ s_box_permuted_result[index * 32 + i];
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
    
    for (int j = 0; j < password_length; j++) {
        password[index * password_length + j] = d_allowed_char[iteration % d_allowed_char_size];
        iteration /= d_allowed_char_size;
    } 

    return password;
}

__device__ int* d_string_to_binary(char* string_text, int password_length, int* bin_text, int blockSize) {
    unsigned int index = blockIdx.x * blockSize + threadIdx.x;
    
    int current_index = 0;
    for (int i = 0; i < password_length; i++) {
        char currentChar = string_text[index * password_length + i];
        for (int j = 7; j >= 0; --j) {
            bin_text[index * 64 + current_index++] = (currentChar >> j) & 1;
        }
    }

    return bin_text;
}

__device__ bool isBinaryEqual(int* string1, int* string2, int blockSize){
    unsigned int index = blockIdx.x * blockSize + threadIdx.x;

    bool isEqual = true;
    for (int i = 0; i < 64; i++) {
        if(string1[(index * 64) + i] != string2[i]){
            isEqual = false;
        }
    }

    return isEqual;
}

__global__ void brute_force_attack(int* cipher_password_target, int* d_sub_keys, int blockSize, int threads_number, 
    int password_length, char* current_password, int* bin_current_password, int* cipher_current_password,
    int* result_initial_permutation, int* left_block, int* right_block, int* right_expanded, int* xor_result, 
    int* block, int* s_box_result, int* s_box_permuted_result, int* new_left_block, int* combined_key) {
    
    unsigned int index = blockIdx.x * blockSize + threadIdx.x;

    long number_of_possible_passwords = (long)pow((double)d_allowed_char_size,(double)(password_length));
    for(long i = index; i < number_of_possible_passwords; i += threads_number){
        //printf("iteration %d \n", i);
        generate_all_possible_password(current_password, password_length, blockSize, i);
        d_string_to_binary(current_password, password_length, bin_current_password, blockSize);
        cuda_des_encrypt_text(bin_current_password, d_sub_keys, cipher_current_password, blockSize, threads_number,
         result_initial_permutation, left_block, right_block, right_expanded, xor_result, 
         block, s_box_result, s_box_permuted_result, new_left_block, combined_key);
    
        if(isBinaryEqual(cipher_current_password, cipher_password_target, blockSize)){
            printf("Find by thread n. %d! \n", index);
            printf("password is: '");
            for(int j = 0; j < 8; j++){
                printf("%c", current_password[index * password_length + j]);
            }
            printf("' in DES: '");
            for(int j = 0; j < 64; j++){
                printf("%d", cipher_current_password[index * password_length * 8 + j]);
            }
            printf("'\n");
            __trap();
            return;
        }
    }     
}

void setupGrid(int threads_number, int blockSize, int *num_block, int *threads_per_block) {
    if (threads_number <= blockSize) {
        *threads_per_block = threads_number;
        *num_block = 1;
    } else if ((threads_number % 256) == 0) {
        *threads_per_block = 256;
        *num_block = threads_number / 256;
        blockSize = 256;
    } else if ((threads_number % 128) == 0) {
        *threads_per_block = 128;
        *num_block = threads_number / 128;
        blockSize = 128;
    } else if ((threads_number % 32) == 0) {
        *threads_per_block = 32;
        *num_block = threads_number / 32;
        blockSize = 32;
    } else {
        int mod256 = threads_number % 256;
        int mod128 = threads_number % 128;
        int mod32 = threads_number % 32;

        if (mod256 <= 128 && mod256 <= mod128 && mod256 <= mod32) {
            *threads_per_block = threads_number + (32 - mod256);
            blockSize = 256;
        } else if (mod128 <= mod32) {
            *threads_per_block = threads_number + (128 - mod128);
            blockSize = 128;
        } else {
            *threads_per_block = threads_number + (32 - mod32);
            blockSize = 32;
        }
        
        *num_block = threads_number / *threads_per_block;
    }
}

void getGPUProperties(int gpuID) {
    cudaDeviceProp prop;
    int deviceId = gpuID;

    cudaGetDeviceProperties(&prop, deviceId);

    std::cout << "GPU info:" << std::endl;
    std::cout << "Name: " << prop.name << std::endl;
    std::cout << "Max number of threads for block: " << prop.maxThreadsPerBlock << std::endl;
    std::cout << "Max blocks number : " << prop.maxGridSize[0] << std::endl;
}

int main() {
    //SETUP DES
    const char* key = "A4rT9v.w";
    int* des_key = (int*) malloc(64 * sizeof(int));
    string_to_binary(key, 8, des_key);

    cout << "Binary representation of the key '" << key << "': ";
    for(int i = 0; i < 64; i++){
        cout << des_key[i];
    }
    cout << endl;
    
    int** sub_keys = create_sub_keys(des_key);
    for(int i = 0; i < 16; i++){
        for(int j = 0; j < 48; j++){
        }
    }

    int d_sub_keys_1d[16 * 48];
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 48; j++) {
            d_sub_keys_1d[(i * 48) + j] = sub_keys[i][j];  
        }
    }
    
    cout << endl;

    //SETUP TARGET PASSWORD
    const char* password = "2/W.caaa";
    int* cipher_password_target = des_encrypt_text(password, d_sub_keys_1d);
    cout << "Password to find: '" << password << "' encrypted with DES: ";
    for(int i = 0; i < 64; i++){
        cout << cipher_password_target[i];
    }

    cout << endl;

    //SETUP CUDA
    //getGPUProperties(0); //Get GPU info
    unsigned int threads_number =384;
    int blockSize = 32;

    int threads_per_block;
    int num_block;
    setupGrid(threads_number, blockSize, &num_block, &threads_per_block);
    printf("Setup: <<<Grid Size: %d, Threads per Block: %d>>>\n", num_block, threads_per_block);

    int password_length = 8;
    char* current_password;
    int* bin_current_password;
    int* cipher_current_password;
    int* result_initial_permutation;
    int* left_block;
    int* right_block;
    int* right_expanded;
    int* xor_result;
    int* block; 
    int* s_box_result;
    int* s_box_permuted_result;
    int* new_left_block;
    int* combined_key;

    cudaMalloc((void**)&current_password, (threads_number * password_length * sizeof(char)));
    cudaMalloc((void**)&bin_current_password, (threads_number * password_length * 8 * sizeof(int)));
    cudaMalloc((void**)&cipher_current_password, (threads_number * 64 * sizeof(int)));
    cudaMalloc((void**)&result_initial_permutation, (threads_number * 64 * sizeof(int)));
    cudaMalloc((void**)&left_block, (threads_number * 32 * sizeof(int)));
    cudaMalloc((void**)&right_block, (threads_number * 32 * sizeof(int)));
    cudaMalloc((void**)&right_expanded, (threads_number * 48 * sizeof(int)));
    cudaMalloc((void**)&xor_result, (threads_number * 48 * sizeof(int)));
    cudaMalloc((void**)&block, (threads_number * 6 * sizeof(int)));
    cudaMalloc((void**)&s_box_result, (threads_number * 32 * sizeof(int)));
    cudaMalloc((void**)&s_box_permuted_result, (threads_number * 32 * sizeof(int)));
    cudaMalloc((void**)&new_left_block, (threads_number * 32 * sizeof(int)));
    cudaMalloc((void**)&combined_key, (threads_number * 64 * sizeof(int)));

    int* d_cipher_password_target;
    cudaMalloc((void**)&d_cipher_password_target, (threads_number * password_length * 8 * sizeof(int)));
    cudaMemcpy(d_cipher_password_target, cipher_password_target, password_length * 8 * sizeof(int), cudaMemcpyHostToDevice);

    int* d_sub_keys;
    cudaMalloc((void**)&d_sub_keys, (16 * 48 * sizeof(int)));
    cudaMemcpy(d_sub_keys, d_sub_keys_1d, 16 * 48 * sizeof(int), cudaMemcpyHostToDevice);

    //DA SISTEMARE --> è possibile avere qualcosa utilizzabile sia in gpu che in cpu ? tanto son odat statici
    long number_of_possible_passwords = (long)pow((double)allowed_char_size,(double)(password_length));
    cout << "Total of possible password: " << number_of_possible_passwords << " with: " << password_length << " characters" << endl;
    cout << endl;
    cout << "Brute force attack started ..." << endl;
    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    cout << endl;
    
    brute_force_attack<<<num_block, threads_per_block>>>(d_cipher_password_target, d_sub_keys, blockSize, threads_number, 
        password_length, current_password, bin_current_password, 
        cipher_current_password, result_initial_permutation, left_block, right_block, right_expanded, xor_result, 
        block, s_box_result, s_box_permuted_result, new_left_block, combined_key);
    cudaDeviceSynchronize();

    cudaError_t error = cudaGetLastError();
    if (error != cudaSuccess) {
        cout << "CUDA Error: " << cudaGetErrorString(error) << endl;
        cout << "Brute force attack aborted" << endl;
    }
    
    cout << endl;

    gettimeofday(&end_time, NULL);
    double total_time = ((end_time.tv_sec  - start_time.tv_sec) * 1000000u + end_time.tv_usec - start_time.tv_usec) / 1.e6;
    cout << "Execution Time: " << total_time << " s" << endl;
    cout << "Brute force attack terminated" << endl;

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