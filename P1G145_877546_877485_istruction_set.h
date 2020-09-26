/*
 * ISTRUCTION SET e CODICE MACCHINA
 *
 * Codice macchina    Istruzione
 *           0        HALT
 *           1        DISPLAY
 *           2        PRINT_STACK
 *          10        PUSH
 *          11        POP
 *          12        MOV
 *          20        CALL
 *          21        RET
 *          22        JMP
 *          23        JZ
 *          24        JPOS
 *          25        JNE
 *          30        ADD
 *          31        SUB
 *          32        MUL
 *          33        DIV
 */

#include <stdio.h>
#include <limits.h> /* Definisce INT_MAX = +2147483647 e INT_MIN = -2147483648 */

/* Codici errori */
#define errore_operazione 14
#define errore_stack 15

/*
 DISPLAY    R0-R31
 Codice macchina: 1
 Stampa su console il contenuto di uno dei registri R0-R31.

 Parametri:
 - r        registri R0-R31
 - registro indice che punta ad uno dei 32 registri (R0-R31)
 */
void display(int registro);

/*
 PRINT_STACK    numero n
 Codice macchina: 2
 Stampa su console n posizioni dello stack partendo da stack[sp - 1] fino a stack[sp - n]
 Stampa sia l'indice i della posizione che il valore contenuto in stack[i].

 Parametri:
 - n          elementi dello stack da stampare
 - stack      memoria stack
 - sp         puntatore stack

 In caso di errore stampa a monitor un messaggio di avviso.
 */
void print_stack(int n, int *stack, unsigned int sp);

/*
 PUSH   R0-R31
 Codice macchina: 10
 Copia il contenuto del registro indicato (R0-R31) in stack[sp] e incrementa sp.

 Parametri:
 - registro           valore da inserire nello stack
 - stack              memoria stack
 - sp                 puntatore stack
 - dimensione_stack   dimensione massima dello stack

 Restituisce:
 -  0 se la funzione arriva a buon termine
 - 10 in caso di errore (stack pieno)
 */
int push(long registro, int *stack, unsigned int *sp, int dimensione_stack);

/*
 POP    R0-R31
 Codice macchina: 11
 Decrementa sp e copia il contenuto di stack[sp] nel registro indicato (R0-R31).

 Parametri:
 - registro   registro in cui salvare l'elemento rimosso dallo stack
 - stack      memoria stack
 - sp         puntatore stack

 Restituisce:
 -  0 se la funzione arriva a buon termine
 - 11 in caso di errore (stack vuoto)
 */
int pop(int *registro, int *stack, unsigned int *sp);

/*
 MOV    R0-R31  numero
 Codice macchina: 12
 Copia un determinato numero nel registro indicato (R0-R31).

 Parametri:
 - r          registri R0-R31
 - registro   indice 0-31
 - numero     numero da copiare

 Restituisce:
 -  0 se la funzione arriva a buon termine
 - 12 in caso di errore
 */
void mov(int *registro, int numero);

/*
 CALL   posizione
 Codice macchina: 20
 Chiamata a subroutine: prima esegue un'operazione di PUSH copiando il valore di IP (posizione successiva a CALL), poi esegue un JMP puntando alla posizione indicata.

 Parametri:
 - ip                 registro di cui salvare il contenuto
 - posizione          posizione per l'operazione JMP
 - stack              memoria stack
 - sp                 puntatore stack
 - dimensione_stack   dimensione massima dello stack

 Restituisce:
 -  0 se la funzione arriva a buon termine
 - 20 in caso di errore durante l'operazione di PUSH o JMP
 - 40 in caso di errore sia durante l'operazione di PUSH che di JMP
 */
int call(long *ip, int posizione, int *stack, unsigned int *sp, int dimensione_stack);

/*
 RET
 Codice macchina: 21
 Ritorno da chiamata a subroutine: operazione di POP in IP.

 Parametri:
 - ip     registro dove copiare l'ultimo elemento dello stack
 - stack  memoria stack
 - sp     puntatore stack

 Restituisce:
 -  0 se la funzione arriva a buon termine
 - 21 in caso di errore
 */
int ret(long *ip, int *stack, unsigned int *sp, unsigned int numero_istruzioni);

/*
 JMP    posizione
 Codice macchina: 22
 Sostituisce il valore di IP con quello indicato.

 Parametri:
 - ip         registro ip
 - posizione  valore da copiare in ip

 Restituisce:
 -  0 se la funzione arriva a buon termine
 - 22 in caso di errore
 */
void jmp(long *ip, int posizione);

/*
 JZ     posizione
 Codice macchina: 23
 Verifica se l'ultimo elemento inserito nello stack è pari a zero, rimuovendolo e decrementando sp; se lo è, sostituisce il valore di IP con quello indicato.

 Parametri:
 - ip         registro ip
 - posizione  valore da copiare in ip
 - stack      memoria stack
 - sp         puntatore stack

 Restituisce:
 -  0 se la funzione arriva a buon termine
 - 23 in caso di errore
 */
int jz(long *ip, int posizione, int *stack, unsigned int *sp);

/*
 JPOS   posizione
 Codice macchina: 24
 Verifica se l'ultimo elemento inserito nello stack è positivo, rimuovendolo e decrementando sp; se lo è, sostituisce il valore di IP con quello indicato.

 Parametri:
 - ip         registro ip
 - posizione  valore da copiare in ip
 - stack      memoria stack
 - sp         puntatore stack

 Restituisce:
 -  0 se la funzione arriva a buon termine
 - 24 in caso di errore
 */
int jpos(long *ip, int posizione, int *stack, unsigned int *sp);

/*
 JNEG   posizione
 Codice macchina: 25
 Verifica se l'ultimo elemento inserito nello stack è negativo, rimuovendolo e decrementando sp; se lo è, sostituisce il valore di IP con quello indicato.

 Parametri:
 - ip         registro ip
 - posizione  valore da copiare in ip
 - stack      memoria stack
 - sp         puntatore stack

 Restituisce:
 -  0 se la funzione arriva a buon termine
 - 25 in caso di errore
 */
int jneg(long *ip, int posizione, int *stack, unsigned int *sp);

/*
 ADD    R0-R31  R0-R31
 Codice macchina: 30
 Somma intera dei due parametri; il risultato viene inserito nello stack.
 Segnalazione errore in caso di overflow.

 Parametri:
 - addendo1           primo addendo
 - addendo2           secondo addendo
 - stack              memoria stack
 - sp                 puntatore stack
 - dimensione_stack   dimensione massima dello stack

 Restituisce:
 -  0 se la funzione arriva a buon termine
 - 30 in caso di errore
 */
int add(int addendo1, int addendo2, int *stack, unsigned int *sp, int dimensione_stack);

/*
 SUB    R0-R31  R0-R31
 Codice macchina: 31
 Somma intera dei due parametri; il risultato viene inserito nello stack.
 Segnalazione errore in caso di overflow.

 Parametri:
 - minuendo           minuendo
 - sottraendo         sottraendo
 - stack              memoria stack
 - sp                 puntatore stack
 - dimensione_stack   dimensione massima dello stack

 Restituisce:
 -  0 se la funzione arriva a buon termine
 - 31 in caso di errore
 */
int sub(int minuendo, int sottraendo, int *stack, unsigned int *sp, int dimensione_stack);

/*
 MUL    R0-R31  R0-R31
 Codice macchina: 32
 Moltiplicazione intera dei due parametri; il risultato viene inserito nello stack.
 Segnalazione errore in caso di overflow.

 Parametri:
 - fattore1           primo fattore
 - fattore2           secondo fattore
 - stack              memoria stack
 - sp                 puntatore stack
 - dimensione_stack   dimensione massima dello stack

 Restituisce:
 -  0 se la funzione arriva a buon termine
 - 32 in caso di errore
 */
int mul(int fattore1, int fattore2, int *stack, unsigned int *sp, int dimensione_stack);

/*
 DIV    R0-R31  R0-R31
 Codice macchina: 33
 Divisione intera dei due parametri; il risultato viene inserito nello stack.
 Segnalazione errore in caso di divisione per 0.

 Parametri:
 - dividendo          dividendo
 - divisore           divisore
 - stack              memoria stack
 - sp                 puntatore stack
 - dimensione_stack   dimensione massima dello stack

 Restituisce:
 -  0 se la funzione arriva a buon termine
 - 33 in caso di errore

 La funzione div() esiste già in una delle librerie utilizzate.
 */
int divisione(int dividendo, int divisore, int *stack, unsigned int *sp, int dimensione_stack);
