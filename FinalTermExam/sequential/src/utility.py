import matplotlib.pyplot as plt
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

def log_to_file(working_directory, results, ngrams_type): 
    results_str_keys = {str(key): value for key, value in results.items()}
    with open(f"{working_directory}/resources/analysis/analysis_book_{ngrams_type}.json", 'w', encoding='utf-8') as json_file:
        json.dump(results_str_keys, json_file, indent=4)

    print(f"COMPLETE! Results available in: resources/analysis/analysis_book_{ngrams_type}.json")

def make_histogram(working_directory, ngrams_to_plot, plot_all=False):
    with open(f"{working_directory}/resources/analysis/analysis_book_{ngrams_to_plot}.json", 'r', encoding='utf-8') as json_file:
        ngrams_data = json.load(json_file)

    ngrams_frequencies = [(eval(ngram), count) for ngram, count in ngrams_data.items()]

    if not plot_all:
        number_of_plots = 50
        plot_histogram(ngrams_frequencies[:number_of_plots], f'{ngrams_to_plot} Frequency Histogram', ngrams_to_plot, 'Frequency')
    else:
        plot_histogram(ngrams_frequencies, f'{ngrams_to_plot} Frequency Histogram', ngrams_to_plot, 'Frequency')
    
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