import matplotlib.pyplot as plt 

def sequenziale_parallelo_8():
    #MAC
    num_threads = [1, 2, 3, 4, 5, 6, 7, 8]
    tempo_esecuzione_sequenziale = [3964.3]
    tempo_esecuzione_parallelo = [3635.43, 1870.2, 1274.33, 952.182, 763.445, 644.883, 618.588, 595.119]
    
    #LINUX
    #num_threads = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32]
    #tempo_esecuzione_sequenziale = [3964.3]
    #tempo_esecuzione_parallelo = [4461.75, 2236.64, 1493.41, 1124.49, 901.942, 756.772, 649.167, 594.315, 521.07, 469.617, 434.817, 398.885, 373.993, 349.434, 327.47, 309.461, 286.749, 279.44, 268.801, 250.751, 236.494, 235.226, 219.68, 215.853, 209.417, 199.965, 194.025, 186.961, 184.563, 174.373, 171.084, 164.497]
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
    num_threads = [1, 2, 3, 4, 5, 6, 7, 8]
    tempo_esecuzione_sequenziale = [3964.3]
    tempo_esecuzione_parallelo = [3635.43, 1870.2, 1274.33, 952.182, 763.445, 644.883, 618.588, 595.119]
    
    #LINUX
    #num_threads = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32]
    #tempo_esecuzione_sequenziale = [3964.3]
    #tempo_esecuzione_parallelo = [4461.75, 2236.64, 1493.41, 1124.49, 901.942, 756.772, 649.167, 594.315, 521.07, 469.617, 434.817, 398.885, 373.993, 349.434, 327.47, 309.461, 286.749, 279.44, 268.801, 250.751, 236.494, 235.226, 219.68, 215.853, 209.417, 199.965, 194.025, 186.961, 184.563, 174.373, 171.084, 164.497]
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


sequenziale_parallelo_8 ()
#speed_up_sequenziale_parallelo_8()
#parallelo_vs_parallelo_modificato()