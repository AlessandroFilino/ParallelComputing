import matplotlib.pyplot as plt
import nltk
from nltk.probability import FreqDist
import shutil, os
import requests


def download_gutenberg_book(book_id):
    base_url = 'https://www.gutenberg.org/ebooks/'
    book_format = ".txt.utf-8"

    book_url = f'{base_url}{book_id}{book_format}'

    print(f"Download from {book_url} ...")

    try:
        response = requests.get(book_url)
        if response.status_code == 404:
            print(f'book_{book_id} not found')
            return
        new_ebook = response.text

        with open(f"./resources/book_{book_id}.txt", 'w', encoding='utf-8') as file:
            file.write(new_ebook)

        print(f'Download complete in "{base_url}{book_id}".')

    except requests.exceptions.RequestException as e:
        print(f'HTTP error: {e}')

    except Exception as e:
        print(f'ERROR: {e}')

def log_to_file(book_number, bigrammi, trigrammi):
    with open(f"./resources/analysis/analysis_book_{book_number}.txt", 'w', encoding='utf-8') as file:
        file.write(f"ANALYSIS FOR  book_{book_number}:\n\n")

        file.write("Bigrammi:\n")
        for bigramma, count in sorted(nltk.FreqDist(bigrammi).items(), key=lambda x: x[1], reverse=True):
            file.write(f"{bigramma} - #{count}\n")

        file.write("\nTrigrammi:\n")
        for trigramma, count in sorted(nltk.FreqDist(trigrammi).items(), key=lambda x: x[1], reverse=True):
            file.write(f"{trigramma} - #{count}\n")

    print(f"COMPLETE! Results available in: resources/analysis/analysis_book_{book_number}.txt")

def make_histogram(bigrammi, trigrammi):
    fdist_bigrammi = FreqDist(bigrammi)
    fdist_trigrammi = FreqDist(trigrammi)

    fdist_bigrammi.plot(30, cumulative=False)
    plt.show()

    fdist_trigrammi.plot(30, cumulative=False)
    plt.show()

def setup_system(directory):
    if os.path.exists(directory):
        try:
            shutil.rmtree(directory)
            os.mkdir(directory)
            os.mkdir(os.path.join(directory, "analysis"))
            print(f"Setup completato")
        except Exception as e:
            print(f"Setup Error: {e}")
    else:
        os.mkdir(directory)
        os.mkdir(os.path.join(directory, "analysis"))


def clean_directory(directory):
    try:
        shutil.rmtree(directory)
        print(f"Contenuto della directory '{directory}' rimosso con successo.")
    except Exception as e:
        print(f"Errore durante la rimozione del contenuto della directory '{directory}': {e}")

