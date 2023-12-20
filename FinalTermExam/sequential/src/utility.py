import matplotlib.pyplot as plt
import nltk
from nltk.probability import FreqDist
import shutil, os
import requests


def download_gutenberg_book(book_id):
    base_url = 'https://www.gutenberg.org/ebooks/'
    book_format = ".txt.utf-8"

    book_url = f'{base_url}{book_id}{book_format}'
    print(book_url)

    try:
        response = requests.get(book_url)
        new_ebook = response.text

        with open(f"./resources/book_{book_id}.txt", 'w', encoding='utf-8') as file:
            file.write(new_ebook)

        print(f'Il libro è stato scaricato con successo e salvato in "{file}".')

    except requests.exceptions.RequestException as e:
        print(f'Errore durante la richiesta HTTP: {e}')

    except Exception as e:
        print(f'Errore sconosciuto: {e}')

def log_to_file(book_number, bigrammi, trigrammi):
    with open(f"./resources/analysis/analysis_book_{book_number}.txt", 'w', encoding='utf-8') as file:
        file.write(f"Risultati analisi libro {book_number}:\n\n")

        file.write("Bigrammi:\n")
        for bigramma, count in sorted(nltk.FreqDist(bigrammi).items(), key=lambda x: x[1], reverse=True):
            file.write(f"{bigramma} - #{count}\n")

        file.write("\nTrigrammi:\n")
        for trigramma, count in sorted(nltk.FreqDist(trigrammi).items(), key=lambda x: x[1], reverse=True):
            file.write(f"{trigramma} - #{count}\n")

    print(f"Risultati salvati in: resources/analysis/analysis_book_{book_number}.txt")

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

