#include "P1G145_877546_877485_istruction_set_stampa.h"

/* Istruzioni con nessun parametro
 Codice macchina:
 0  -> HALT
 21 -> RET
 */
void halt_ret(int codice_macchina) {
    /* Se il codice macchina è 0 */
    if (codice_macchina == 0)
        printf("HALT\n");
    /* Se non è 0, allora dev'essere 21 */
    else
        printf("RET\n");
}

/* PRINT_STACK
 Codice macchina: 2
 1 parametro numerico
 */
void print(int programma[], unsigned int *ip) {
    /* Stampiamo su console il codice assembly relativo al codice macchina analizzato */
    (*ip)++;
    printf("PRINT_STACK %d\n", programma[*ip]);
}

/* MOV
 Codice macchina: 12
 2 parametri: 1 registri R0-R31, 1 numerico
 */
void mov_stampa(int programma[], unsigned int *ip) {
    /* Stampiamo su console il codice assembly relativo al codice macchina analizzato */
    (*ip)++;
    printf("MOV R%d ", programma[*ip]);

    (*ip)++;
    printf("%d\n", programma[*ip]);
}

/* Istruzioni con 1 parametro (registro R0-R31)
 Codice macchina:
 1  -> DISPLAY
 10 -> PUSH
 11 -> POP
 */
void display_push_pop(int programma[], unsigned int *ip)
{
    /* Stampiamo su console il codice assembly relativo al codice macchina analizzato */
    if (programma[*ip] == 1) {
        printf("DISPLAY ");
    } else if (programma[*ip] == 10) {
        printf("PUSH ");
    } else {
        printf("POP ");
    }

    (*ip)++;
    printf("R%d\n", programma[*ip]);
}

/* Istruzioni con 1 parametro (posizione)
 Codice macchina:
 20 -> CALL
 22 -> JMP
 23 -> JZ
 24 -> JPOS
 25 -> JNEG
 */
void jump(int programma[], unsigned int *ip)
{
    /* Stampiamo su console il codice assembly relativo al codice macchina analizzato */
    if (programma[*ip] == 20) {
        printf("CALL ");
    } else if (programma[*ip] == 22) {
        printf("JMP ");
    } else if (programma[*ip] == 23) {
        printf("JZ ");
    } else if (programma[*ip] == 24) {
        printf("JPOS ");
    } else {
        printf("JNEG ");
    }

    (*ip)++;
    printf("%d\n", programma[*ip]);
}

/* Istruzioni con 2 parametri (registri R0-R31)
 Codice macchina:
 30 -> ADD
 31 -> SUB
 32 -> MUL
 33 -> DIV
 */
void operation(int programma[], unsigned int *ip)
{
    /* Stampiamo su console il codice assembly relativo al codice macchina analizzato */
    if (programma[*ip] == 30) {
        printf("ADD ");
    } else if (programma[*ip] == 31) {
        printf("SUB ");
    } else if (programma[*ip] == 32) {
        printf("MUL ");
    } else {
        printf("DIV ");
    }

    (*ip)++;
    printf("R%d ", programma[*ip]);

    (*ip)++;
    printf("R%d\n", programma[*ip]);
}
