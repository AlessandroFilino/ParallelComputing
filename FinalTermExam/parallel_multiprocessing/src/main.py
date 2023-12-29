from nltk import bigrams as nltk_bigrams, trigrams as nltk_trigrams
import time
import os
from multiprocessing import Pool
from utility import download_gutenberg_book, make_histogram, log_to_file, setup_system, preprocess_text

working_directory = os.path.join(os.getcwd(), "parallel_multiprocessing")

def process_book(book):
    try:
        download_gutenberg_book(working_directory, book)
        with open(f"{working_directory}/resources/book_{book}.txt", 'r', encoding='utf-8') as file:
            text = file.read()

        text = preprocess_text(text)

        bigrams = list(nltk_bigrams(text.split()))
        trigrams = list(nltk_trigrams(text.split()))

        log_to_file(working_directory, book, bigrams, trigrams)

    except Exception as e:
        print(f'ERROR: {e}')


def main():
    setup_system(f"{working_directory}/resources/")

    books = 500
    
    start_time = time.time()

    with Pool(processes=8) as pool:
        pool.map(process_book, range(books))

    end_time = time.time()
    execution_time = end_time - start_time

    print(f"Complete in: {round(execution_time, 3)} s")

if __name__ == "__main__":
    main()


#10 libri: 3.019 s

#50 libri: 17.546 s  

#100 libri: 
    #32 processes: 18.547 s
    #16 processes: 16.372 s
    # --> #8 processes: 25.581 s
    #7 processes: 29.677 s s
    #6 processes: 35.622 s
    #5 processes: 38.908 s
    #4 processes: 47.598 s
    #3 processes: 66.851 s
    #2 processes: 92.992 s
    #1 processes: 187.664 s

#500 libri: 115.202 s

#1000 libri: 272.626 s

