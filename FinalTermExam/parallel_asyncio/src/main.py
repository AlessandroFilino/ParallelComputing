from nltk import bigrams as nltk_bigrams, trigrams as nltk_trigrams
import time
import os
import asyncio
import aiohttp
from concurrent.futures import ProcessPoolExecutor
from utility import download_gutenberg_book, log_to_file, setup_system, preprocess_text

working_directory = os.path.join(os.getcwd(), "parallel_asyncio")

def process_book(book):
    try:
        with open(f"{working_directory}/resources/book_{book}.txt", 'r', encoding='utf-8') as file:
            text = file.read()

        text = preprocess_text(text)

        bigrams = list(nltk_bigrams(text.split()))
        trigrams = list(nltk_trigrams(text.split()))

        log_to_file(working_directory, book, bigrams, trigrams)
    except Exception as e:
        print(f'ERROR: {e}')

async def main():
    
    setup_system(f"{working_directory}/resources/")

    books = 100
    start_time = time.time()

    async with aiohttp.ClientSession() as session:
        download_and_process_tasks = [
            download_gutenberg_book(working_directory, session, book)
            for book in range(books)
        ]
        await asyncio.gather(*download_and_process_tasks)

    with ProcessPoolExecutor(max_workers=8) as executor:
        executor.map(process_book, range(books))

    end_time = time.time()
    execution_time = end_time - start_time

    print(f"Complete in: {round(execution_time, 3)} s")

if __name__ == "__main__":
    asyncio.run(main())

#10 libri: 2.492 s  
#50 libri: 14.049 s  
# 100 libri: 
    #32 workers: 92.674 s
    #16 workers: 22.663 s
    #8 workers: 19.956 s
    #7 workers: 20.351
    #6 workers: 21.166 s
    #5 workers: 23.222 s
    #4 workers: 26.752 s
    #3 workers: 27.009 s
    #2 workers: 33.196 s
    #1 workers: 51.331 s
#500 libri: 64.102 s
#1000 libri: 136.378 s

    

