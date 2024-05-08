import matplotlib.pyplot as plt 

def sequenziale_parallelo_8():
    #MAC
    num_threads = [1, 2, 3, 4, 5, 6, 7, 8, 16]
    tempo_esecuzione_sequenziale = [3964.3]
    tempo_esecuzione_parallelo = [3635.43, 1870.2, 1274.33, 952.182, 763.445, 644.883, 618.588, 595.119, 612.199]
    
    plt.scatter(num_threads[0], tempo_esecuzione_sequenziale, color='red', marker='o', label='Sequenziale')
    plt.plot(num_threads[0], tempo_esecuzione_sequenziale, color='red', linestyle='-', linewidth=2)


    plt.scatter(num_threads, tempo_esecuzione_parallelo, color='blue', marker='o', label='Parallelo')
    plt.plot(num_threads, tempo_esecuzione_parallelo, color='blue', linestyle='-', linewidth=2)

    plt.grid(True, linestyle='--', linewidth=0.5, color='gray', alpha=0.7)

    plt.suptitle('Brute Force - Sequenziale Vs Parallelo', fontsize = 20)
    plt.title('Chaive DES: A4rT9v.w Password: 2/W.caaa')
    plt.xlabel('Numero di Threads')
    plt.ylabel('Tempo di Esecuzione (s)')
    plt.legend()
    plt.show()

def speed_up_sequenziale_parallelo_8():
    #MAC
    num_threads = [1, 2, 3, 4, 5, 6, 7, 8, 16]
    tempo_esecuzione_sequenziale = [3964.3]
    tempo_esecuzione_parallelo = [3635.43, 1870.2, 1274.33, 952.182, 763.445, 644.883, 618.588, 595.119, 612.199]
    
    speed_up = []
    for parallelo in tempo_esecuzione_parallelo:
        risultato = tempo_esecuzione_sequenziale[0] / parallelo
        speed_up.append(risultato)

    plt.scatter(num_threads, speed_up, color='red', marker='o', label='Speed-up')
    plt.plot(num_threads, speed_up, color='red', linestyle='-', linewidth=2)

    plt.grid(True, linestyle='--', linewidth=0.5, color='gray', alpha=0.7)
    
    plt.suptitle('Brute Force - Speed-Up', fontsize = 20)
    plt.title('Chaive DES: A4rT9v.w Password: 2/W.caaa')
    plt.xlabel('Numero di Threads')
    plt.ylabel('Speedup')
    plt.legend()
    plt.show()

def parallelo_vs_parallelo_modificato():
    #MAC
    num_threads = [1, 2, 3, 4, 5, 6, 7, 8]
    tempo_esecuzione_sequenziale = [3964.3]
    tempo_esecuzione_parallelo = [3888.58, 4030.15, 4073.9, 4092.4, 4183.47, 4673.27, 5979.54, 9127.22]
    tempo_esecuzione_parallelo_modificato = [3635.43, 1870.2, 1274.33, 952.182, 763.445, 644.883, 618.588, 595.119]

    plt.scatter(num_threads[0], tempo_esecuzione_sequenziale, color='red', marker='o', label='Sequenziale')
    plt.plot(num_threads[0], tempo_esecuzione_sequenziale, color='red', linestyle='-', linewidth=2)


    plt.scatter(num_threads, tempo_esecuzione_parallelo, color='green', marker='o', label='Parallelo')
    plt.plot(num_threads, tempo_esecuzione_parallelo, color='green', linestyle='-', linewidth=2)

    plt.scatter(num_threads, tempo_esecuzione_parallelo_modificato, color='blue', marker='o', label='parallelo_modificato')
    plt.plot(num_threads, tempo_esecuzione_parallelo_modificato, color='blue', linestyle='-', linewidth=2)

    plt.grid(True, linestyle='--', linewidth=0.5, color='gray', alpha=0.7)

    plt.suptitle('Brute Force - Parallelo Vs Parallelo V2', fontsize = 20)
    plt.title('Chaive DES: A4rT9v.w Password: 2/W.caaa')
    plt.xlabel('Numero di Threads')
    plt.ylabel('Tempo di Esecuzione (s)')
    plt.legend()
    plt.show()

def cuda_performance():
    num_threads = [1, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536]
    tempo_esecuzione = [8712.53, 709.246, 366.658, 374.512, 342.683, 179.263, 90.5714, 41.6371, 32.3622, 17.3052, 67.4107, 40.2187, 20.3382]

    plt.scatter(num_threads, tempo_esecuzione, color='blue', marker='o', label='cuda')
    plt.plot(num_threads, tempo_esecuzione, color='blue', linestyle='-', linewidth=2)

    plt.grid(True, linestyle='--', linewidth=0.5, color='gray', alpha=0.7)

    plt.suptitle('Brute Force - Cuda Performance', fontsize=20)
    plt.title('Chaive DES: A4rT9v.w Password: 2/W.caaa')
    plt.xlabel('Numero di Threads')
    plt.ylabel('Tempo di Esecuzione (s)')
    plt.legend()
    plt.xscale('log')
    plt.xticks(num_threads, num_threads)
    plt.show()

def cuda_speed_up():
    num_threads = [1, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536]
    tempo_esecuzione_sequenziale = [3964.3]
    tempo_esecuzione_cuda = [8712.53, 709.246, 366.658, 374.512, 342.683, 179.263, 90.5714, 41.6371, 32.3622, 17.3052, 67.4107, 40.2187, 20.3382]

    speed_up = []
    for parallelo in tempo_esecuzione_cuda:
        risultato = tempo_esecuzione_sequenziale[0] / parallelo
        speed_up.append(risultato)

    best_result = speed_up.index(max(speed_up))

    plt.scatter(num_threads, speed_up, color='red', marker='o', label='Speed-up')
    plt.plot(num_threads, speed_up, color='red', linestyle='-', linewidth=2)

    plt.scatter(num_threads[best_result], speed_up[best_result], color='green', marker='o', label=f'Best Result: {speed_up[best_result]:.2f}')

    plt.grid(True, linestyle='--', linewidth=0.5, color='gray', alpha=0.7)
    
    plt.suptitle('Brute Force - Speed-Up CUDA', fontsize=20)
    plt.title('Chaive DES: A4rT9v.w Password: 2/W.caaa')
    plt.xlabel('Numero di Threads')
    plt.ylabel('Speedup')
    plt.legend()

    plt.xscale('log')
    plt.xticks(num_threads, num_threads)

    plt.show()

def cuda_grid_threadsBlock_comparison():

    grid_size = [13, 14, 15, 32, 64, 256]
    threads_per_block = [576, 576 , 576, 256, 128, 32]
    occupancy = [0.75, 0.75, 0.75, 0.66, 0.75, 0.33]
    results = [58.8041, 31.1302, 48.9386, 17.3052, 29.0586, 23.9934]
    total_threads_number = [7488, 8064, 8640, 8192, 8192, 8192]

    plt.scatter(total_threads_number, results, color='blue', marker='o', label='cuda')
    plt.plot(total_threads_number, results, color='blue', linewidth=2)

    plt.grid(True, linestyle='--', linewidth=0.5, color='gray', alpha=0.7)

    plt.suptitle('Brute Force - Cuda Performance', fontsize=20)
    plt.title('Chaive DES: A4rT9v.w Password: 2/W.caaa')
    plt.xlabel('Numero di Threads')
    plt.ylabel('Tempo di Esecuzione (s)')
    plt.legend()
    plt.xscale('log')
    plt.xticks(total_threads_number, results)
    plt.show()

def general_evaluation():
    tempo_esecuzione_sequenziale = 3964.3
    tempo_esecuzione_parallelo = 3888.58
    tempo_esecuzione_parallelo_modificato = 595.119
    tempo_esecuzione_cuda = 17.3052
    
    speedup_parallelo = tempo_esecuzione_sequenziale / tempo_esecuzione_parallelo
    speedup_parallelo_modificato = tempo_esecuzione_sequenziale / tempo_esecuzione_parallelo_modificato
    speedup_cuda = tempo_esecuzione_sequenziale / tempo_esecuzione_cuda
    
    labels = ['Parallelo', 'Parallelo Modificato', 'CUDA']
    values = [speedup_parallelo, speedup_parallelo_modificato, speedup_cuda]
    
    plt.bar(labels, values, color=['blue', 'green', 'red'])
    plt.suptitle('Brute Force - Confronto tra SpeedUP', fontsize=20)
    plt.title('Chaive DES: A4rT9v.w Password: 2/W.caaa')
    plt.ylabel('Speedup', fontsize=12)
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.show()





#sequenziale_parallelo_8 ()
#speed_up_sequenziale_parallelo_8()
#parallelo_vs_parallelo_modificato()

#cuda_performance()
#cuda_speed_up()
#cuda_grid_threadsBlock_comparison() --> DA FINIRE

general_evaluation()