import matplotlib.pyplot as plt 

def sequenziale_parallelo():
    #MAC
    num_threads = [1, 2, 3, 4, 5, 6, 7, 8, 16, 32]
    tempo_esecuzione_sequenziale = [181.758]
    tempo_esecuzione_parallelo_asyncio = [51.331, 33.196, 27.009, 23.531, 20.634, 21.166, 20.351, 19.956, 21.249, 22.875]
    tempo_esecuzione_parallelo_multiprocessing=[187.664, 92.992, 66.851, 47.598, 38.908, 35.622, 29.677, 25.581, 16.372, 18.547]

    plt.scatter(num_threads[0], tempo_esecuzione_sequenziale, color='red', marker='o', label='Sequenziale')
    plt.plot(num_threads[0], tempo_esecuzione_sequenziale, color='red', linestyle='-', linewidth=2)


    plt.scatter(num_threads, tempo_esecuzione_parallelo_asyncio, color='blue', marker='o', label='Asyncio + Multiprocessing')
    plt.plot(num_threads, tempo_esecuzione_parallelo_asyncio, color='blue', linestyle='-', linewidth=2)

    plt.scatter(num_threads, tempo_esecuzione_parallelo_multiprocessing, color='green', marker='o', label='Multiprocessing')
    plt.plot(num_threads, tempo_esecuzione_parallelo_multiprocessing, color='green', linestyle='-', linewidth=2)

    plt.grid(True, linestyle='--', linewidth=0.5, color='gray', alpha=0.7)

    plt.suptitle('Bigrams And Trigrams In Python', fontsize = 20)
    plt.title('N. Libri: 100')
    plt.xlabel('Numero di Processes')
    plt.ylabel('Tempo di Esecuzione (s)')
    plt.legend()
    plt.show()

def sequenziale_parallelo_completo():
    #MAC
    num_book = [10, 50, 100, 500, 1000]
    tempo_esecuzione_sequenziale = [10.29, 88.963, 181.758, 891.633, 1795.717]
    tempo_esecuzione_parallelo_asyncio = [2.492, 14.049, 19.956, 64.102, 136.378]
    tempo_esecuzione_parallelo_multiprocessing=[3.019, 17.546, 25.581, 115.202, 272.626]
    print("num_book:", num_book)
    print("tempo_esecuzione_sequenziale:", tempo_esecuzione_sequenziale)


    plt.scatter(num_book, tempo_esecuzione_sequenziale, color='red', marker='o', label='Sequenziale')
    plt.plot(num_book, tempo_esecuzione_sequenziale, color='red', linestyle='-', linewidth=2)

    plt.scatter(num_book, tempo_esecuzione_parallelo_asyncio, color='blue', marker='o', label='Asyncio + Multiprocessing')
    plt.plot(num_book, tempo_esecuzione_parallelo_asyncio, color='blue', linestyle='-', linewidth=2)

    plt.scatter(num_book, tempo_esecuzione_parallelo_multiprocessing, color='green', marker='o', label='Multiprocessing')
    plt.plot(num_book, tempo_esecuzione_parallelo_multiprocessing, color='green', linestyle='-', linewidth=2)

    plt.grid(True, linestyle='--', linewidth=0.5, color='gray', alpha=0.7)

    plt.suptitle('Bigrams And Trigrams In Python', fontsize=20)
    plt.title('N. Processes = 8')
    plt.xlabel('Numero di Libri')
    plt.ylabel('Tempo di Esecuzione (s)')
    plt.legend()
    plt.show()



def speed_up_sequenziale_asyncio():
    #MAC
    num_threads = [1, 2, 3, 4, 5, 6, 7, 8, 16, 32]
    tempo_esecuzione_sequenziale = [181.758]
    tempo_esecuzione_parallelo_asyncio = [51.331, 33.196, 27.009, 23.531, 20.634, 21.166, 20.351, 19.956, 21.249, 22.875]


    speed_up = []
    for parallelo in tempo_esecuzione_parallelo_asyncio:
        risultato = tempo_esecuzione_sequenziale[0] / parallelo
        speed_up.append(risultato)

    plt.scatter(num_threads, speed_up, color='red', marker='o', label='Speed-up')
    plt.plot(num_threads, speed_up, color='red', linestyle='-', linewidth=2)

    plt.grid(True, linestyle='--', linewidth=0.5, color='gray', alpha=0.7)
    
    plt.suptitle('Bigrams And Trigrams In Python', fontsize = 20)
    plt.title('N. Libri: 100')
    plt.xlabel('Numero di Processes')
    plt.ylabel('Speedup')
    plt.legend()
    plt.show()

def speed_up_sequenziale_multiprocessing():
    #MAC
    num_threads = [1, 2, 3, 4, 5, 6, 7, 8, 16, 32]
    tempo_esecuzione_sequenziale = [181.758]
    tempo_esecuzione_parallelo_multiprocessing=[187.664, 92.992, 66.851, 47.598, 38.908, 35.622, 29.677, 25.581, 16.372, 18.547]

    speed_up = []
    for parallelo in tempo_esecuzione_parallelo_multiprocessing:
        risultato = tempo_esecuzione_sequenziale[0] / parallelo
        speed_up.append(risultato)

    plt.scatter(num_threads, speed_up, color='red', marker='o', label='Speed-up')
    plt.plot(num_threads, speed_up, color='red', linestyle='-', linewidth=2)

    plt.grid(True, linestyle='--', linewidth=0.5, color='gray', alpha=0.7)
    
    plt.suptitle('Bigrams And Trigrams In Python', fontsize = 20)
    plt.title('N. Libri: 100')
    plt.xlabel('Numero di processes')
    plt.ylabel('Speedup')
    plt.legend()
    plt.show()

def speed_up_parallelo():
    num_threads = [1, 2, 3, 4, 5, 6, 7, 8, 16, 32]
    tempo_esecuzione_parallelo_asyncio = [51.331, 33.196, 27.009, 23.531, 20.634, 21.166, 20.351, 19.956, 21.249, 22.875]
    tempo_esecuzione_parallelo_multiprocessing=[187.664, 92.992, 66.851, 47.598, 38.908, 35.622, 29.677, 25.581, 16.372, 18.547]

    speed_up = []
    for i in range(len(num_threads)):
        risultato = tempo_esecuzione_parallelo_asyncio[i] / tempo_esecuzione_parallelo_multiprocessing[i]
        speed_up.append(risultato)

    plt.scatter(num_threads, speed_up, color='red', marker='o', label='Speed-up')
    plt.plot(num_threads, speed_up, color='red', linestyle='-', linewidth=2)

    plt.grid(True, linestyle='--', linewidth=0.5, color='gray', alpha=0.7)
    
    plt.suptitle('Bigrams And Trigrams In Python', fontsize = 20)
    plt.title('N. Libri: 100')
    plt.xlabel('Numero di processes')
    plt.ylabel('Speedup')
    plt.legend()
    plt.show()


#sequenziale_parallelo()
#sequenziale_parallelo_completo()
#speed_up_sequenziale_asyncio()
#speed_up_sequenziale_multiprocessing()
speed_up_parallelo()