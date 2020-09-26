/*
 Le istruzioni sono state divise in 6 diverse funzioni in base al numero e tipo dei loro parametri.
 Si potrebbe unire in un'unica funzione print() e jump(), ma la funzione stampa() andrebbe poi modificata di conseguenza.

 A tutte le funzioni vengono passati come parametri:
 - programma[]          vettore contenente le istruzioni in codice macchina
 - ip                   indice per scorrere programma[]
 - numero_istruzioni    è il numero di elementi di programma[]
 La funzione halt_ret() è l'unica eccezione in quanto le istruzioni HALT e RET non hanno bisogno di parametri.

 Tutte le funzioni (eccetto halt_ret()) terminano con un errore nel caso in cui, partendo dall'elemento programma[i], programma[] non contiene un numero sufficiente a stampare tutti i parametri dell'istruzione analizzata.
 */

#include <stdio.h>

/*
 Istruzioni HALT e RET.
 0 parametri.
 */
void halt_ret(int codice_macchina);

/*
 Istruzione PRINT_STACK.
 1 parametro: numero.
 */
void print(int programma[], unsigned int *ip);

/*
 Istruzione MOV.
 2 parametri: registro R0-R31, numero.
 L'abbiamo chiamata mov_stampa() e non mov() perché è stata dichiarata una mov() in esegui.h
 */
void mov_stampa(int programma[], unsigned int *ip);

/*
 Istruzioni DISPLAY, PUSH e POP.
 1 parametro: registro R0-R31.
 */
void display_push_pop(int programma[], unsigned int *ip);

/*
 Istruzioni di salto JMP, JZ, JPOS e JNEG.
 1 parametro: posizione dove eseguire il salto.
 */
void jump(int programma[], unsigned int *ip);

/*
 4 operazioni ADD, SUB, MUL e DIV.
 2 parametri: registro R0-R31, registro R0-R31.
 */
void operation(int programma[], unsigned int *ip);
