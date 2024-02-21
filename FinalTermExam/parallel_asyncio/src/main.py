from nltk import bigrams as nltk_bigrams, trigrams as nltk_trigrams, FreqDist
import time
import os
import asyncio
import aiohttp
from multiprocessing import Pool, Manager, Process
from utility import download_gutenberg_book, make_histogram, log_to_file, setup_system, preprocess_text

working_directory = os.path.join(os.getcwd(), "parallel_asyncio")

def process_book(args):
    book, working_directory, bigrams_shared_queue, trigrams_shared_queue = args
    print("EXECUTE BOOK:", book)
    try:
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

def combine_and_log_results(shared_queue, file_name):
    result_dict = {}
    while not shared_queue.empty():
        item = shared_queue.get()
        for key, value in item.items():
            result_dict[key] = result_dict.get(key, 0) + value
    log_to_file(working_directory, result_dict, file_name)

async def main():
    setup_system(f"{working_directory}/resources/")

    books = 100

    manager = Manager()
    bigrams_shared_queue = manager.Queue()
    trigrams_shared_queue = manager.Queue()

    start_time = time.time()

    async with aiohttp.ClientSession() as session:
        download_and_process_tasks = [
            download_gutenberg_book(working_directory, session, book) for book in range(books)
        ]
        await asyncio.gather(*download_and_process_tasks)

    args = [(i, working_directory, bigrams_shared_queue, trigrams_shared_queue) for i in range(books)]
    with Pool(processes=32) as process_pool: 
        process_pool.map(process_book, args)
    
    bigrams_process = Process(target=combine_and_log_results, args=(bigrams_shared_queue, "bigrams"))
    trigrams_process = Process(target=combine_and_log_results, args=(trigrams_shared_queue, "trigrams"))

    bigrams_process.start()
    trigrams_process.start()

    bigrams_process.join()
    trigrams_process.join()
        
    end_time = time.time()
    execution_time = end_time - start_time

    print(f"Complete in: {round(execution_time, 3)} s")

    make_histogram(working_directory, "bigrams")
    make_histogram(working_directory, "trigrams")

if __name__ == "__main__":
    asyncio.run(main())


#10 libri: 2.881 s
#50 libri: 23.812 s
# 100 libri: 
    #32 processes: 29.321 s
    #16 processes: 28.25 s
    # --> 8 processes: 27.412 s
    #7 processes: 27.134 s
    #6 processes: 28.364 s
    #5 processes: 28.316 s
    #4 processes: 30.09 s
    #3 processes: 33.618 s
    #2 processes: 31.112 s
    #1 processes: 38.095 s
#500 libri: 94.049 s
#1000 libri: 259.27 s