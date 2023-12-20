from nltk import bigrams, trigrams
import time

from utility import download_gutenberg_book, make_histogram, log_to_file, setup_system

setup_system("./resources/")

book_number = 50

start_time = time.time()
for i in range(0, book_number):
    print("EXECUT BOOK:", i)
    try:
        download_gutenberg_book(i)

        with open(f"./resources/book_{i}.txt", 'r', encoding='utf-8') as file:
            testo = file.read()

        testo = testo.lower()
        testo = testo.replace('.', '')  # Rimuovi il punto alla fine della frase

        # Generazione di bigrammi e trigrammi
        bigrammi = list(bigrams(testo.split()))
        trigrammi = list(trigrams(testo.split()))

        #make_histogram(bigrammi, trigrammi)
        log_to_file(i, bigrammi, trigrammi)
    except Exception as e:
        print(f'ERROR: Download book_{i}: {e}')
    
    print("\n")

end_time = time.time()

execution_time = end_time - start_time

print(f"Complete in: {round(execution_time, 3)} s")




    
