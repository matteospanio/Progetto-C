#include <stdio.h>
#include <stdlib.h>
#include "P1G145_877546_877485_controllo_programma.h"
#include <limits.h>

/* Codici errore */
#define carattere_non_valido 3
#define file_inesistente 4
#define file_vuoto 6
#define primo_valore_errato 7
#define programma_incompleto 8

/*
 Acquisisce il programma contenuto in input.cvm e lo salva in un vettore.

 Parametri:
 - file_input           nome del file di input da leggere
 - programma            vettore di interi dove verranno salvate le istruzioni macchina contenute nel file di input
 - numero_istruzioni    al termine della funzione, conterrà il numero di elementi di programma[]
 - comando              0 se è stata richiesta l'operazione "stampa", 1 se "esegui"
 */
int lettura_file(const char *file_input, int **programma, unsigned int *numero_istruzioni, int comando);
