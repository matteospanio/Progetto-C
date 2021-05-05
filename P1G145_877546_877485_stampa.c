#include "P1G145_877546_877485_stampa.h"

int stampa(const char *input) {
    /* Indice per scorrere programma[] */
    unsigned int ip = 0;

    /* Creiamo il vettore dove trascrivere il programma in linguaggio macchina contenuto nel file di input */
    int *programma = NULL;
    unsigned int numero_istruzioni = 0;
    /* Chiamiamo la funzione che si occupa di leggere e trascrivere il contenuto del file di input */
    int errori = lettura_file(input, &programma, &numero_istruzioni, 0);

    /* Se ci sono stati errori durante la lettura del file di input, interrompiamo il programma. L'errore è già stato segnalato all'interno della funzione lettura_file() */
    if (errori != 0) {
        return errori;
    }

    printf("\n");

    /*
     All'interno del ciclo scorreremo il vettore programma usando l'indice ip. Il ciclo termina una volta raggiunto l'ultimo elemento del vettore.
     Ci sono due casi di errore:
     (1) Viene rilevata un codice macchina sconosciuto.
     (2) L'ultima istruzione non trova il corretto numero di parametri per essere stampata correttamente.
     */
    while (ip < numero_istruzioni) {
        /* Dobbiamo visualizzare a monitor due cose: (1) il numero della riga e (2) l'istruzione corrispondente con relativi parametri */

        /* Scriviamo il numero della riga */
        printf(" [ ");
        /* Il prossimo if serve ad aggiungere uno spazio e incolonnare  i numeri. Scritto così, verranno incolonnati correttamente tutti i numeri inferiori a 100. Dovrebbe essere ampiamente sufficiente per i programmi contenuti nei nostri file di input... e comunque si tratta solo di un dettaglio a livello grafico. */
        if (ip < 10)
            printf(" ");
        printf("%d]  ", ip);

        /* Scriviamo a monitor l'istruzione: */
        /* HALT o RET */
        if ((programma[ip] == 0) || (programma[ip] == 21)) {
            halt_ret(programma[ip]);
        /* PRINT_STACK */
        } else if (programma[ip] == 2) {
            print(programma, &ip);
        /* MOV */
        } else if (programma[ip] == 12) {
            mov_stampa(programma, &ip);
        /*
         DISPLAY, PUSH o POP
         Considerando i codici macchina analizzati negli if precedenti:
         "(programma[ip] < 12)" corrisponde a "(programma[ip] == 10 || programma[ip] == 11)"
         È questo il motivo per cui in istruction_set_stampa.h le funzioni print() e jump() sono state dichiarate separatamente.
         */
        } else if (programma[ip] < 12) {
            display_push_pop(programma, &ip);
        /*
         CALL, JMP, JZ, JPOS o JNEG
         "(programma[ip] < 30)" corrisponde a "(programma[ip] == 20 || programma[ip] == 22 || programma[ip] == 23 || programma[ip] == 24 || programma[ip] == 25)"
         */
        } else if (programma[ip] < 30) {
            jump(programma, &ip);
        /*
         ADD, SUB, MUL o DIV
         "else" corrisponde a "if (programma[ip] == 30 || programma[ip] == 31 || programma[ip] == 32 || programma[ip] == 33)"
         */
        } else {
            operation(programma, &ip);
        }
        
        ip++;
    }

    printf("\n");

    /* Liberiamo la memoria occupata da programma[] */
    free(programma);

    return errori;
}
