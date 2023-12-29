from nltk import bigrams as nltk_bigrams, trigrams as nltk_trigrams
import time
import os
from utility import download_gutenberg_book, make_histogram, log_to_file, setup_system, preprocess_text, requests

working_directory = os.path.join(os.getcwd(), "sequential")

setup_system(f"{working_directory}/resources/")

books = 50

start_time = time.time()
for book in range(0, books):
    print("EXECUT BOOK:", book)
    try:
        download_gutenberg_book(working_directory, book)

        with open(f"{working_directory}/resources/book_{book}.txt", 'r', encoding='utf-8') as file:
            text = file.read()

        text = preprocess_text(text)

        bigrams = list(nltk_bigrams(text.split()))
        trigrams = list(nltk_trigrams(text.split()))

        log_to_file(working_directory, book, bigrams, trigrams)

        #make_histogram(book)
    except Exception as e:
        print(f'ERROR: {e}')

    print("\n")

end_time = time.time()

execution_time = end_time - start_time

print(f"Complete in: {round(execution_time, 3)} s")

#10 libri: 10.29 s
#50 libri: 88.963 s
#100 libri: 181.758 s
#500 libri: 891,633
#1000 libri: 1795.717 s
