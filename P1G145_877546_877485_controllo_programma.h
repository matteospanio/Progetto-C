#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define malloc_fallita 5
#define codice_macchina_non_riconosciuto 9
#define errore_parametri 10
#define numero_non_valido 11
#define errore_registro 12
#define errore_salto 13

/*
 Verifica i numeri acquisiti dal file di input
 Parametri:
 - programma            Vettore dove trascrivere il programma in codice macchina una volta controllato
 - programma_file       Vettore contenente i numeri acquisiti dal file di input
 - numero_istruzioni    Numero elementi di programma_file[]
 - comando              0 "stampa", 1 "esegui"
 */
int controllo_programma(int **programma, long *programma_file, unsigned int numero_istruzioni, int comando);

/* Verifica che il vettore contenente le istruzioni in codice macchina abbia elementi a sufficienza per eseguire l'istruzione corrente */
int test_parametri(long ip, int parametri, unsigned int numero_istruzioni);

/* Verifica che il registro selezionato sia uno di quelli esistenti (R0-R31) */
int test_registro(long ip, long registro);

/* Verifica se un'istruzione di salto (CALL, JMP, JZ, JPOS, JNEG, RET) punta ad un'istruzione inesistente */
int test_salto(long ip, long posizione, unsigned int numero_istruzioni);
