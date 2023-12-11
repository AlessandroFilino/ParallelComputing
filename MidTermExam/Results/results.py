import matplotlib.pyplot as plt 

def sequenziale_parallelo_8():
    num_threads = [1, 2, 3, 4, 5, 6, 7, 8]
    tempo_esecuzione_sequenziale = [3964.3]
    tempo_esecuzione_parallelo = [3635.43, 1870.2, 1274.33, 952.182, 763.445, 644.883, 618.588, 595.119]

    plt.scatter(num_threads[0], tempo_esecuzione_sequenziale, color='red', marker='o', label='Sequenziale')
    plt.plot(num_threads[0], tempo_esecuzione_sequenziale, color='red', linestyle='-', linewidth=2)


    plt.scatter(num_threads, tempo_esecuzione_parallelo, color='blue', marker='o', label='Parallelo')
    plt.plot(num_threads, tempo_esecuzione_parallelo, color='blue', linestyle='-', linewidth=2)

    plt.suptitle('Brute Force - Sequenziale Vs Parallelo', fontsize = 20)
    plt.title('Chaive DES: A4rT9v.w Password: 2/W.caaa')
    plt.xlabel('Numero di Threads')
    plt.ylabel('Tempo di Esecuzione (s)')
    plt.legend()
    plt.show()

def speed_up_sequenziale_parallelo_8():
    num_threads = [1, 2, 3, 4, 5, 6, 7, 8]
    tempo_esecuzione_sequenziale = [3964.3]
    tempo_esecuzione_parallelo = [3635.43, 1870.2, 1274.33, 952.182, 763.445, 644.883, 618.588, 595.119]

    speed_up = []
    for parallelo in tempo_esecuzione_parallelo:
        risultato = tempo_esecuzione_sequenziale[0] / parallelo
        speed_up.append(risultato)

    plt.scatter(num_threads, speed_up, color='red', marker='o', label='Speed-up')
    plt.plot(num_threads, speed_up, color='red', linestyle='-', linewidth=2)
    
    plt.suptitle('Brute Force - Speed-Up', fontsize = 20)
    plt.title('Chaive DES: A4rT9v.w Password: 2/W.caaa')
    plt.xlabel('Numero di Threads')
    plt.ylabel('Speedup')
    plt.legend()
    plt.show()

#sequenziale_parallelo_8 ()
speed_up_sequenziale_parallelo_8()