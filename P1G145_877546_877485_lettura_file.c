/*
 Il file di input:
 - Dovrebbe contenere solo numeri e commenti
 - I commenti cominciano sempre con ';'
 - Ogni numero dovrebbe essere scritto su una riga diversa (eventualmente possono esserci un numero e un commento)
 - I numeri dovrebbero trovarsi in testa alla riga (niente spazi prima dei numeri)
 - Possono esserci righe vuote o con solo commenti
 - Il primo numero corrisponde al numero di istruzioni in linguaggio macchina che compongono il programma contenuto nel file di input
 - Il file di input può contenere sia LF ('\n') che CRLF ("\r\n")

 Questo programma:
 - Accetta più numeri per riga... il programma in linguaggio macchina potrebbe essere scritto anche tutto su una riga, e verrebbe acquisito correttamente
 - il primo numero (corrispondente al numero delle istruzioni del programma in linguaggio macchina) e tutti gli altri devono trovarsi su righe diverse

 Caratteri ammessi nel file:
 - numeri da 0 a 9
 - '-' per numeri negativi
 - '\n' e '\r'
 - ' ' spazi
 - ';' seguito da qualsiasi carattere
 Utilizzando getline per acuiqisire le righe del file, avremo anche il carattere '\0'
 */

#include "P1G145_877546_877485_lettura_file.h"

int lettura_file(const char *file_input, int **programma, unsigned int *numero_istruzioni, int comando)
{
    /* Variabili necessarie al corretto funzionamento di getline */
    char *line_t = NULL;        /* Conterrà la stringa ricevuta */
    size_t line_t_size = 0;     /* Lunghezza stringa ricevuta */
    ssize_t line_lenght = 0;    /* Lunghezza stringa ricevuta. Varrà -1 se abbiamo finito le righe */

    int dim = 0;                        /* flag dim-ensioni_programma */
    int i = 0;                          /* Contatore */
    int j = 0;                          /* Contatore */
    long z = 0;                         /* Contatore */
    int inizio_numero = 0;              /* Indicherà la prima cifra di ogni numero da salvare */
    int cifre_numero = 0;               /* Numero di cifre del numero da salvare */
    char *numero_da_convertire = NULL;  /* Numero da salvare */
    long linee = 0;                     /* Numero di linee del file lette. Serve solo per segnalare un errore */

    long dimensioni_programma = 0;      /* Primo numero estratto dal file */
    long *programma_input = NULL;       /* Vettore dove verranno salvati i numeri acquisiti dal file di input */

    int errori = 0;

    /* File di lettura */
    FILE* input = fopen(file_input, "r");

    printf("\nMi avete chiesto di elaborare il file: %s\n", file_input);

    /* File non trovato */
    if (input == NULL)
    {
        printf("\nNon trovo il file.\n");
        return file_inesistente;
    }

    printf("\nFile individuato.\n");

    /*
     Acquisisce una alla volta tutte le righe del file di input. Nel momento in cui trova un numero, il valore di dim viene modificato e il ciclo termina.
     Questo ciclo serve ad acquisire il primo numero contenuto nel file di input.
     */
    while ((dim == 0) && (line_lenght = getline(&line_t, &line_t_size, input)) && (line_lenght >= 0))
    {
        /* Il file di input dovrebbe avere ogni riga delimitata esclusivamente da '\n'. Quindi, ogni volta che getline va a buon fine abbiamo una nuova riga del file e incrementiamo linee di 1 */
        linee++;

        /* Inizializziamo i in modo che punti (line_t[i]) all'inizio della riga. i = -1 e non 0 perché viene incrementato di 1 a breve. */
        i = -1;

        inizio_numero = 0;

        /* Scorriamo tutta la stringa */
        /* Il ciclo termina se siamo arrivati alla fine della stringa o se abbiamo trovato dei commenti (e quindi non ci interessa il resto). */
        while ((line_t[++i] != ';') && (line_t[i] != '\0'))
        {
            /*
             Se il file di input contiene errori il programma si interrompe segnalando un errore.
             La condizione è che il carattere con indice i NON sia un numero, ' ', '\0' o ';', '\n', '\r'.
             Il simbolo '-' è fra quelli non accettati perché stiamo acquisendo il primo numero che indica quante istruzioni ci sono nel file di input, quindi può essere solo positivo.
             */
            if ((line_t[i] != ' ') && (line_t[i] != '\0') && (line_t[i] != ';') && (line_t[i] != '\n') && (line_t[i] != '\r') && ((line_t[i] < '0') || (line_t[i] > '9')))
            {
                printf("\nHo trovato un carattere (posizione %d) non valido alla riga %ld del file di input. Assicurati che ';' sia presente su ogni commento o che non ci siano altri errori. Il primo numero (che indica la lunghezza del programma) dev'essere strettamente positivo.\n", i, linee);
                fclose(input);
                free(line_t);
                return carattere_non_valido;
            }

            /* Individuiamo la prima cifra di un numero.
             Se il file di input è scritto correttamente e ogni riga contiene un solo numero a inizio riga, questo if è superfluo.
             Se il numero contenuto in questa riga è preceduto da degli spazi, questo if permette di trovare l'elemento della stringa dove gli spazi finiscono.
             La condizione è che il carattere con indice i sia pari a " " e che quello successivo (i + 1) sia un numero (stiamo acquisendo il primo numero che indica quante istruzioni ci sono, non può essere negativo).
             Nel caso non ci siano spazi, inizio_numero è già stato settato a 0 prima che cominciasse questo ciclo while.
             */
            if ((line_t[i] == ' ') && ((line_t[i + 1] >= '0') && (line_t[i + 1] <= '9')))
                inizio_numero = i + 1;

            /* Individuiamo l'ultima cifra di un numero
             La condizione è che il carattere con indice i sia un numero e quello successivo (i + 1) no.
             */
            if (((line_t[i + 1] == ' ') || (line_t[i + 1] == ';') || (line_t[i + 1] == '\n') || (line_t[i + 1] == '\r') || (line_t[i + 1] == '\0')) && ((line_t[i] >= '0') && (line_t[i] <= '9')))
            {
                /* Abbiamo trovato il numero che stavo cercando: segnalo che una volta finita l'acquisizione, devo smettere di leggere il file */
                dim = 1;

                /* Calcoliamo il numero di cifre che compongono il numero
                 inizio_numero è l'indice della prima cifra del numero da salvare; i è quello dell'ultima. */
                cifre_numero = i - inizio_numero + 1;

                /* Allochiamo la memoria necessaria a salvarci il numero (sotto forma di stringa) */
                /* lunghezza_numero + 1 per il carattere di fine stringa */
                numero_da_convertire = (char *)malloc(sizeof(char) * (cifre_numero + 1));

                /* Se la malloc non è andata a buon fine */
                if (numero_da_convertire == NULL)
                {
                    printf("\nErrore durante l'allocazione di memoria dinamica.\n");
                    fclose(input);
                    free(line_t);
                    return malloc_fallita;
                }

                /*
                 Copiamo una ad una le cifre del numero in numero_da_convertire
                 inizio_numero è l'indice della prima cifra del numero da salvare; i è quello dell'ultima.
                 */
                for (j = inizio_numero; j <= i; j++)
                {
                    numero_da_convertire[j - inizio_numero] = line_t[j];
                }
                /* Aggiungiamo il carattere di fine stringa */
                numero_da_convertire[cifre_numero] = '\0';
            }
        }
    }

    /* Se siamo arrivati alla fine del file senza trovare alcun numero */
    if (line_lenght < 0)
    {
        printf("\nIl file è vuoto.\n\n");
        fclose(input);
        free(line_t);
        return file_vuoto;
    }

    /* Convertiamo il numero raccolto nel ciclo precedente in un dato di tipo integer */
    dimensioni_programma = atol(numero_da_convertire);
    /* Libero la memoria occupata da numero_da_convertire */
    free(numero_da_convertire);

    /* Verifica sul primo numero contenuto nel file. Se troppo grande il programma si arresta, altrimenti lo salva in numero_istruzioni e prosegue. */
    if (dimensioni_programma > UINT_MAX)
    {
        printf("\nIl primo numero del file (indicante il numero di istruzioni che compongono il programma in linguaggio macchina) è troppo grande: dev'essere compreso fra 0 e %d\n\n", UINT_MAX);
        fclose(input);
        free(line_t);
        return primo_valore_errato;
    } else
    {
        *numero_istruzioni = (unsigned int) dimensioni_programma;
    }

    /* Allochiamo la memoria necessaria a salvarmi tutte le informazioni contenute nel file di input */
    programma_input = (long *)malloc(sizeof(long) * (*numero_istruzioni));

    /* Se la malloc non è andata a buon fine */
    if (numero_da_convertire == NULL)
    {
        printf("\nErrore durante l'allocazione di memoria dinamica.\n");
        fclose(input);
        free(line_t);
        return malloc_fallita;
    }

    /* Acquisisce una alla volta i codici macchina del file di input. Nel momento in cui trova un numero, lo salva nel vettore programma_input[]. Il numero acquisito nel ciclo while precedente indica quanti numeri dobbiamo acquisire in questo. Se il file di input contiene meno numeri di quelli attesi, il programma termina con un errore; se ce ne sono di più, gli ultimi vengono ignorati.
     */
    while ((z < *numero_istruzioni) && (line_lenght = getline(&line_t, &line_t_size, input)) && (line_lenght >= 0))
    {
        /* Il file di input dovrebbe avere ogni riga delimitata esclusivamente da '\n'. Quindi, ogni volta che getline va a buon fine abbiamo una nuova riga del file e incrementiamo linee di 1 */
        linee++;

        /* Inizializziamo i in modo che punti (line_t[i]) all'inizio della riga. i = -1 e non 0 perché viene incrementato di 1 a breve. */
        i = -1;

        inizio_numero = 0;

        /* Scorriamo tutta la stringa */
        /* Il ciclo termina se siamo arrivati alla fine della stringa o se abbiamo trovato dei commenti (e quindi non ci interessa il resto). */
        while ((line_t[++i] != ';') && (line_t[i] != '\0'))
        {
            /*
             Se il file di input contiene errori il programma si interrompe segnalando un errore.
             La condizione è che il carattere con indice i NON sia un numero, ' ', '\0' o ';', '\n', '\r', '-'.
             */
            if ((line_t[i] != ' ') && (line_t[i] != '\0') && (line_t[i] != ';') && (line_t[i] != '\n') && (line_t[i] != '\r') && (line_t[i] != '-') && ((line_t[i] < '0') || (line_t[i] > '9')))
            {
                printf("\nHo trovato un carattere (posizione %d) non valido alla riga %ld del file di input. Assicurati che ';' sia presente su ogni commento o che non ci siano altri errori.\n", i, linee);
                fclose(input);
                free(programma_input);
                free(line_t);
                return carattere_non_valido;
            }

            /* Se troviamo un simbolo '-' seguito da qualcosa che non è un numero */
            if ((line_t[i] == '-') && ((line_t[i + 1] < '0') || (line_t[i + 1] > '9')))
            {
                printf("\nHo trovato un carattere (posizione %d) non valido alla riga %ld del file di input. Assicurati che ';' sia presente su ogni commento o che non ci siano altri errori. Fra il simbolo negativo '-' e il relativo numero non devono essere presenti spazi.\n", i, linee);
                fclose(input);
                free(programma_input);
                free(line_t);
                return carattere_non_valido;
            }

            /* Individuiamo la prima cifra di un numero.
             Se il file di input è scritto correttamente e ogni riga contiene un solo numero a inizio riga, questo if è superfluo.
             Se il numero contenuto in questa riga è preceduto da degli spazi, questo if permette di trovare l'elemento della stringa dove gli spazi finiscono.
             La condizione è che il carattere con indice i sia pari a " " e che quello successivo (i + 1) sia un numero o il simbolo '-'.
             Nel caso non ci siano spazi, inizio_numero è già stato settato a 0 prima che cominciasse questo ciclo while.
             */
            if ((line_t[i] == ' ') && ((line_t[i + 1] == '-') || ((line_t[i + 1] >= '0') && (line_t[i + 1] <= '9'))))
                inizio_numero = i + 1;

            /* Individuiamo l'ultima cifra di un numero
             La condizione è che il carattere con indice i sia un numero e quello successivo (i + 1) no.
             */
            if (((line_t[i + 1] == ' ') || (line_t[i + 1] == ';') || (line_t[i + 1] == '\n') || (line_t[i + 1] == '\r') || (line_t[i + 1] == '\0')) && ((line_t[i] >= '0') && (line_t[i] <= '9')))
            {
                /* Calcoliamo il numero di cifre che compongono il numero
                 inizio_numero è l'indice della prima cifra del numero da salvare; i è quello dell'ultima. */
                cifre_numero = i - inizio_numero + 1;

                /* Allochiamo la memoria necessaria a salvarmi il numero (sotto forma di stringa) */
                /* lunghezza_numero + 1 per il carattere di fine stringa */
                numero_da_convertire = (char *)malloc(sizeof(char) * (cifre_numero + 1));

                /* Se la malloc non è andata a buon fine */
                if (numero_da_convertire == NULL)
                {
                    printf("\nErrore durante l'allocazione di memoria dinamica.\n");
                    fclose(input);
                    free(line_t);
                    return malloc_fallita;
                }

                /*
                 Copiamo una ad una le cifre del numero in numero_da_convertire
                 inizio_numero è l'indice della prima cifra del numero da salvare; i è quello dell'ultima.
                 */
                for (j = inizio_numero; j <= i; j++)
                {
                    numero_da_convertire[j - inizio_numero] = line_t[j];
                }
                /* Aggiungiamo il carattere di fine stringa */
                numero_da_convertire[cifre_numero] = '\0';

                /* Convertiamo il numero trovato in un dato di tipo integer e lo salvo in programma_input[z] */
                programma_input[z] = atol(numero_da_convertire);
                /* Liberiamo la memoria occupata da numero_da_convertire */
                free(numero_da_convertire);

                /* Incrementiamo z in modo tale che programma_input[z] indichi la prossima cella del vettore da scrivere */
                z++;
            }
        }
    }

    /* Liberiamo la memoria occupata da line_t */
    free(line_t);
    /* Chiudiamo il file di input */
    fclose(input);

    /* Se siamo arrivati alla fine del file senza trovare tutti i numeri attesi */
    if (line_lenght < 0)
    {
        printf("\nIl programma in codice macchina è incompleto: ho trovato %ld su %d\n\n", z, (*numero_istruzioni));
        return programma_incompleto;
    }

    printf("\nFile acquisito.\n");

    errori = controllo_programma(programma, programma_input, *numero_istruzioni, comando);
    free(programma_input);

    if (errori == 0)
        printf("\nProgramma verificato.\n");
    else
        printf("\nCorreggete il file di input e richiamatemi.\n");

    printf("\nIl programma in codice macchina è composto da %d istruzioni.\n", (*numero_istruzioni));

    return errori;
}
