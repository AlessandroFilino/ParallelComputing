import matplotlib.pyplot as plt
import nltk
import shutil, os
import requests
import json
import re

def download_gutenberg_book(working_directory, book_id):
    base_url = 'https://www.gutenberg.org/ebooks/'
    book_format = ".txt.utf-8"

    book_url = f'{base_url}{book_id}{book_format}'

    print(f"Download from {book_url} ...")

    try:
        response = requests.get(book_url)
        response.raise_for_status()
        
        new_ebook = response.text

        with open(f"{working_directory}/resources/book_{book_id}.txt", 'w', encoding='utf-8') as file:
            file.write(new_ebook)

        print(f'Download complete in "{base_url}{book_id}".')

    except requests.exceptions.RequestException as e:
        print(f'HTTP error: {e}')

    except Exception as e:
        print(f'ERROR: {e}')

def log_to_file(working_directory, book_number, bigrams, trigrams):
    data = {
        "book_number": book_number,
        "bigrams": sorted([(bigram, count) for bigram, count in nltk.FreqDist(bigrams).items()], key=lambda x: x[1], reverse=True),
        "trigrams": sorted([(trigram, count) for trigram, count in nltk.FreqDist(trigrams).items()], key=lambda x: x[1], reverse=True),
    }

    with open(f"{working_directory}/resources/analysis/analysis_book_{book_number}.json", 'w', encoding='utf-8') as json_file:
        json.dump(data, json_file, indent=4)

    print(f"COMPLETE! Results available in: resources/analysis/analysis_book_{book_number}.json")


def make_histogram(working_directory, book_number, plot_all=False):
    with open(f"{working_directory}/resources/analysis/analysis_book_{book_number}.json", 'r', encoding='utf-8') as json_file:
        json_data = json.load(json_file)
        bigrams_data = json_data['bigrams']
        trigrams_data = json_data['trigrams']

    bigram_frequencies = [(tuple(bigram), count) for bigram, count in bigrams_data]
    trigram_frequencies = [(tuple(trigram), count) for trigram, count in trigrams_data]

    if plot_all is False:
        number_of_plots = 50
        plot_histogram(bigram_frequencies[:number_of_plots], 'Bigram Frequency Histogram', 'Bigram', 'Frequency')
        plot_histogram(trigram_frequencies[:number_of_plots], 'Trigram Frequency Histogram', 'Trigram', 'Frequency')
    else:
        plot_histogram(bigram_frequencies, 'Bigram Frequency Histogram', 'Bigram', 'Frequency')
        plot_histogram(trigram_frequencies, 'Trigram Frequency Histogram', 'Trigram', 'Frequency')
    
def plot_histogram(data, title, xlabel, ylabel):
    labels, values = zip(*data)
    indexes = range(len(labels))

    plt.bar(indexes, values, align='center')
    plt.xticks(indexes, labels, rotation='vertical', fontsize=5)
    plt.title(title)
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.show()

def preprocess_text(text):
    text = text.lower()
    text = re.sub(r'[^\w\s]', '', text)
    return text

def setup_system(directory):
    if os.path.exists(directory):
        try:
            shutil.rmtree(directory)
            os.mkdir(directory)
            os.mkdir(os.path.join(directory, "analysis"))
            print(f"Setup complete")
        except Exception as e:
            print(f"Setup Error: {e}")
    else:
        os.mkdir(directory)
        os.mkdir(os.path.join(directory, "analysis"))


def clean_directory(directory):
    try:
        shutil.rmtree(directory)
        print(f"Cleaning complete for directory: '{directory}'.")
    except Exception as e:
        print(f"ERROR: {e}")

