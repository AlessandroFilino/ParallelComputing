from nltk import bigrams as nltk_bigrams, trigrams as nltk_trigrams
from nltk.probability import FreqDist
import queue
import time
import os
from utility import download_gutenberg_book, make_histogram, log_to_file, setup_system, preprocess_text, requests

working_directory = os.path.join(os.getcwd(), "sequential")

def combine_result(shared_queue):
    result_dict = {}
    while not shared_queue.empty():
        item = shared_queue.get()
        for key, value in item.items():
            result_dict[key] = result_dict.get(key, 0) + value
    return result_dict


def process_book(book, bigrams_shared_queue, trigrams_shared_queue):
    print("EXECUTE BOOK:", book)
    try:
        download_gutenberg_book(working_directory, book)

        with open(f"{working_directory}/resources/book_{book}.txt", 'r', encoding='utf-8') as file:
            text = file.read()

        text = preprocess_text(text)

        book_bigrams = list(nltk_bigrams(text.split()))
        bigrams_freq_dist = FreqDist(book_bigrams)
        bigrams_shared_queue.put(dict(bigrams_freq_dist))

        book_trigrams = list(nltk_trigrams(text.split()))
        trigrams_freq_dist = FreqDist(book_trigrams)
        trigrams_shared_queue.put(dict(trigrams_freq_dist))

    except Exception as e:
        print(f'ERROR: {e}')

def main(): 
    setup_system(f"{working_directory}/resources/")
       
    books = 100

    bigrams_shared_queue = queue.Queue()
    trigrams_shared_queue = queue.Queue()

    start_time = time.time()

    for book in range(books):
        process_book(book, bigrams_shared_queue, trigrams_shared_queue)

    total_bigrams = combine_result(bigrams_shared_queue)
    total_trigrams = combine_result(trigrams_shared_queue)

    log_to_file(working_directory, total_bigrams, "bigrams")
    log_to_file(working_directory, total_trigrams, "trigrams")

    print("ANALYSIS COMPLETED")

    end_time = time.time()

    execution_time = end_time - start_time

    print(f"Complete in: {round(execution_time, 3)} s")

    make_histogram(working_directory, "bigrams")
    make_histogram(working_directory, "trigrams")

main()

#10 libri: 10.219 s   
#50 libri: 79.68 s   
#100 libri: 162.13 s
#500 libri: 787.209 s
#1000 libri: 1851.585 s