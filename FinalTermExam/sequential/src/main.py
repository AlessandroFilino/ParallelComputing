from nltk import bigrams as nltk_bigrams, trigrams as nltk_trigrams
import time

from utility import download_gutenberg_book, make_histogram, log_to_file, setup_system, preprocess_text

setup_system("./resources/")

books = 2

start_time = time.time()
for book in range(0, books):
    print("EXECUT BOOK:", book)
    try:
        download_gutenberg_book(book)

        with open(f"./resources/book_{book}.txt", 'r', encoding='utf-8') as file:
            text = file.read()

        text = preprocess_text(text)

        bigrams = list(nltk_bigrams(text.split()))
        trigrams = list(nltk_trigrams(text.split()))

        log_to_file(book, bigrams, trigrams)

        make_histogram(book)
    except Exception as e:
        print(f'ERROR: Download book_{book}: {e}')

    print("\n")

end_time = time.time()

execution_time = end_time - start_time

print(f"Complete in: {round(execution_time, 3)} s")
