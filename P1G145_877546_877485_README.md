**GRUPPO P1G145**

Marco  Spanio  matr. 877546
Matteo Spanio  matr. 877485

### Progetto finale:

## IMPLEMANTAZIONE DI UNA VIRTUAL MACHINE

Indice:
    - Macchina virtuale
    - Programmi in codice macchina (.cvm)
    - Gestione degli errori
    - Struttura del codice

**MACCHINA VIRTUALE**
Lo scopo del progetto è implementare una macchina virtuale con registri e stack che esegua dei programmi scritti in codice macchina in un file (.cvm).

**PROGRAMMI IN CODICE MACCHINA**
I programmi da stampare/eseguire sono contenuti in file di testo (*.cvm).
Dalla consegna "[...]Ciascuna linea del file di testo contiene un numero intero corrispondente ad un elemento del codice macchina del programma [...] ed eventuali commenti preceduti dal carattere ’;’. La prima linea contiene il numero di linee non vuote del file (esclusa la prima)[...]".
Nonostante dalla consegna sia previsto un numero per riga, nella versione da noi implementata della virtual machine è possibile che nel file contenente il codice macchina siano presenti più numeri nella stessa riga (ad eccezione della riga contenente esclusivamente il numero di istruzioni).
La virtual machine è stata programmata in modo tale da acquisire solo le n istruzioni indicate nel primo numero scritto nel file: una volta raggiunta la n-esima istruzione non viene tenuto conto di cosa possa essere stato scritto dopo. Quindi, nel caso in cui il file con il programma contenga del testo non preceduto da ';' nelle righe che seguono il programma, la macchina virtuale ignorerà la cosa.

**GESTIONE ERRORI**
Prima di stampare/eseguire il programma, vi è una fase di acquisizione/lettura del file di input a cui segue una verifica del programma in codice macchina:
(a) Durante l'esecuzione del codice macchina le istruzioni di jump (JMP, JPOS, JNEG) non puntino a dei valori che non corrispondano ad istruzioni.
Se quindi ci trovassimo davanti a questo codice:
12 ; [0] MOV R0 1
0  ; [1]
1  ; [2]
22 ; [3] JMP 1
1  ; [4]
0  ; [5] HALT
La macchina virtuale si arresta dopo aver segnalato che "JMP 1" non punta ad un'istruzione ma al primo parametro dell'istruzione "MOV R0 1".
(b) Viene controllato che il programma non sia troppo corto (che tutte le istruzioni presenti abbiano i relativi parametri)
(c) Viene controllato che il programma non sia incompleto (ci sono meno codici macchina di quanti dichiarati all'inizio)
(d) Che tutti i parametri delle varie istruzioni rientrino nei range massimi previsti

In assenza di errori le funzioni terminano sempre con un return 0. Nei casi in cui vengano riscontrati uno o più errori, invece, vengono restituiti vari numeri in base al tipo di errore:

- 1 se il comando richiesto non è nè "stampa" nè "esegui"
- 2 se ci sono meno di 2 parametri quando si lancia la macchina virtuale
- 3 carattere non valido nel file di input
- 4 file inesistente
- 5 allocazione dinamica della memoria fallita
- 6 file vuoto
- 7 il primo valore del file è troppo grande
- 8 programma incompleto (rispetto a quanto dichiarato con il primo numero)
- 9 codice macchina non riconosciuto
- 10 l'ultima istruzione del programma non ha parametri sufficienti per essere eseguita/stampata
- 11 un parametro numerico supera il range massimo
- 12 parametro errato per i registri R0-R31
- 13 parametri scorretti nelle operazioni di salto
- 14 errore durante un'operazione (overflow o divisione per 0)
- 15 errore durante un'operazione di PUSH o POP
  Nel caso si verifichino più errori, il codice di uscita della macchina virtuale è la somma dei codici di errore dei problemi verificatesi.
