# ParallelComputing

- **Anno Accademico**: 2023-2024
- **Mid Term Project**: DES Password Decryption
- **Final Term Project**: //
- **Student**: Alessandro Filino - 7125518
- **CFU**: 9

# Mid Term Progect
**DESCRIZIONE**
Fare un craccatore di password DES.
Assumere:
- Utilizzare DES (standard ormai deprecato ma che offre funzioni di libreria note)
- Considerare password a 8 caratteri (a-z, A-Z, 0-9, ., /)
- Possiamo scegliere di:
  - Decriptare una password all'inetrno di un dizionairo di password
  - Decriptare un certo numero di password di un dizionario (es. vogliamo craccare tutte le password scritte come date di nascita)
- Tipicamente una password utilizza un SALT (caratteri aggiuntivi inseriti per evitare attacchi brute force).
  - È memorizzato in chiaro e aggiunto quando alla password. Password + Salt vengono quindi cifrate con un operazione di hasking
- Supponiamo che il SALT sia noto (evitiamo attacchi basati su dizionario)