/* numero_istruzioni unsigned int */

#include "P1G145_877546_877485_esegui.h"

int esegui(const char *input) {
    /* Registri R0-R31 */
    int r[32];

    /* La macchina virtuale utilizza un'area di memoria di 64KB con accesso LIFO. In C, le dimensioni delle variabili int possono variare a seconda della piattaforma e del compilatore. Utilizzando sizeof, ci assicuriamo di avere il numero esatto di bytes occupato dal tipo int. */
    const int dimensione_stack = 64000 / sizeof(int);

    /* Creiamo un vettore in interi tale che occupi 64KB di memoria */
    int *stack = NULL;

    /* Puntatore per scorrere stack[] */
    unsigned int sp = 0;

    /* Questa variabile indica la prossima istruzione da eseguire nel nostro programma (file_input.txt) */
    long ip = -1;

    int i = 0;

    /* Queste due variabili verranno usate per richiamare le funzioni che interpretano il linguaggio macchina. Sono state aggiunte solo per migliorare la leggibilità del codice */
    int parametro1 = 0;
    int parametro2 = 0;

    /* Creiamo il vettore dove trascrivere il programma in linguaggio macchina contenuto nel file di input */
    int *programma = NULL;
    unsigned int numero_istruzioni = 0;
    /* Chiamiamo la funzione che si occupa di leggere e trascrivere il contenuto del file di input */
    int errori = lettura_file(input, &programma, &numero_istruzioni, 1);

    /* Se ci sono stati errori durante la lettura del file di input, interrompo il programma. L'errore è già stato segnalato all'interno della funzione lettura_file() */
    if (errori != 0) {
        free(stack);
        return errori;
    }

    stack = (int *)malloc(sizeof(int) * dimensione_stack);

    /* Se la malloc non è andata a buon fine */
    if (stack == NULL) {
        printf("\nErrore durante l'allocazione di memoria dinamica.\n");
        free(programma);
        return malloc_fallita;
    }

    /* Inizializzamo i registri R0-R31 */
    for (i = 0; i < 32; i++)
        r[i] = 0;

    /* Inizializzamo lo stack */
    for (i = 0; i < dimensione_stack; i++)
        stack[i] = 0;

    /* Il ciclo termina in due casi: (1) trova il codice macchina 0 (HALT); (2) riscontra un errore */
    while ((errori == 0) && ((++ip) < numero_istruzioni) && (programma[ip] != 0)) {
        /* Istruzione DISPLAY */
        if (programma[ip] == 1) {
            /* Il contenuto di un registro R0-R31 */
            parametro1 = r[programma[++ip]];

            display(parametro1);
        /* Istruzione PRINT_STACK */
        } else if (programma[ip] == 2) {            /* n numeri dello stack da stampare */
            parametro1 = programma[++ip];

            print_stack(parametro1, stack, sp);
        /* Istruzione PUSH */
        } else if (programma[ip] == 10) {
            /* Il contenuto di un registro R0-R31 */
            parametro1 = r[programma[++ip]];

            errori = errori + push(parametro1, stack, &sp, dimensione_stack);
        /* Istruzione POP */
        } else if (programma[ip] == 11) {
            /* Salviamo in parametro1 l'indice del registro dove eseguire l'operazione di POP */
            parametro1 = programma[++ip];

            errori = errori + pop(&(r[parametro1]), stack, &sp);
        /* Istruzione MOV */
        } else if (programma[ip] == 12) {
            /* Indice di un registro R0-R31 */
            parametro1 = programma[++ip];

            /* Numero */
            parametro2 = programma[++ip];
                
            mov(&r[parametro1], parametro2);
        /* Istruzione CALL */
        } else if (programma[ip] == 20) {
            /* Il parametro è una posizione */
            parametro1 = programma[++ip];

            errori = errori + call(&ip, parametro1, stack, &sp, dimensione_stack);
        /* Istruzione RET */
        } else if (programma[ip] == 21) {
            errori = ret(&ip, stack, &sp, numero_istruzioni);
        /* Istruzione JMP */
        } else if (programma[ip] == 22) {
            /* Il parametro è una posizione */
            parametro1 = programma[++ip];

            jmp(&ip, parametro1);
        /* Istruzione JZ */
        } else if (programma[ip] == 23) {
            /* Il parametro è una posizione */
            parametro1 = programma[++ip];

            errori = errori + jz(&ip, parametro1, stack, &sp);
        /* Istruzione JPOS */
        } else if (programma[ip] == 24) {
            /* Il parametro è una posizione */
            parametro1 = programma[++ip];

            errori = errori + jpos(&ip, parametro1, stack, &sp);
        /* Istruzione JNEG */
        } else if (programma[ip] == 25) {
            /* Il parametro è una posizione */
            parametro1 = programma[++ip];

            errori = errori + jneg(&ip, parametro1, stack, &sp);
        /* Istruzione ADD */
        } else if (programma[ip] == 30) {
            /* Il primo parametro è il contenuto di un registro R0-R31 */
            parametro1 = r[programma[++ip]];

            /* Il secondo parametro è il contenuto di un registro R0-R31 */
            parametro2 = r[programma[++ip]];

            errori = errori + add(parametro1, parametro2, stack, &sp, dimensione_stack);
        /* Istruzione SUB */
        } else if (programma[ip] == 31) {
            /* Il primo parametro è il contenuto di un registro R0-R31 */
            parametro1 = r[programma[++ip]];

            /* Il secondo parametro è il contenuto di un registro R0-R31 */
            parametro2 = r[programma[++ip]];

            errori = errori + sub(parametro1, parametro2, stack, &sp, dimensione_stack);
        /* Istruzione MUL */
        } else if (programma[ip] == 32) {
            /* Il primo parametro è il contenuto di un registro R0-R31 */
            parametro1 = r[programma[++ip]];

            /* Il secondo parametro è il contenuto di un registro R0-R31 */
            parametro2 = r[programma[++ip]];

            errori = errori + mul(parametro1, parametro2, stack, &sp, dimensione_stack);
        /* Istruzione DIV */
        } else if (programma[ip] == 33) {
            /* Il primo parametro è il contenuto di un registro R0-R31 */
            parametro1 = r[programma[++ip]];

            /* Il secondo parametro è il contenuto di un registro R0-R31 */
            parametro2 = r[programma[++ip]];

            errori = errori + divisione(parametro1, parametro2, stack, &sp, dimensione_stack);
        }
    }

    /* Se il programma in codice macchina è incompleto e manca l'istruzione HALT (0) */
    if (ip >= numero_istruzioni) {
        printf("\nCi dev'essere un errore nel file di input: sono arrivato al termine del programma senza incontrare l'istruzione HALT.");
        errori =  programma_incompleto;
    }

    printf("\n\n");

    /* Libero la memoria occupata dal vettore programma[] */
    free(programma);
    /* Libero la memoria occupata dallo stack */
    free(stack);

    return errori;
}
