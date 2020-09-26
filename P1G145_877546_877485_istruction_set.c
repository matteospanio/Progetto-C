#include "P1G145_877546_877485_istruction_set.h"

/* DISPLAY    R0-R31 */
void display(int registro)
{
    /* Stampo su console il */
    printf("\n%d", registro);
}

/* PRINT_STACK    numero n */
void print_stack(int n, int *stack, unsigned int sp)
{
    int i = 0;  /* Contatore */

    /* Si ripere n volte */
    for (i = 1; i <= n; i++)
    {
        /* Stampo l'elemento (sp - i)-esimo dello stack */
        printf("\nstack[%d]: %d", sp - i, stack[sp - i]);

        /* Se provo a stampare più elementi di quelli disponibili, segnalo la cosa e interrompo la funzione.
         Ho comunque stampato tutti gli elementi disponibili e il programma non si interrompe. */
        if (((sp - i) == 0) && (i < n))
        {
            printf("\nERRORE PRINT_STACK: dovrei stampare gli ultimi %d elementi dello stack, ma ce ne sono solo %d.", n, sp);
            return;
        }
    }
}

/* PUSH   R0-R31 */
int push(long registro, int *stack, unsigned int *sp, int dimensione_stack)
{
    /* Se lo stack è pieno */
    if (((int) *sp) >= dimensione_stack)
    {
        printf("\nErrore durante un'operazione PUSH: lo stack è pieno.");
        return errore_stack;
    }

    /* Salvo nello stack il valore contenuto nel registro */
    stack[*sp] = (int) registro;
    (*sp)++;
    
    return 0;
}

/* POP    R0-R31 */
int pop(int *registro, int *stack, unsigned int *sp)
{
    /* Se lo stack è vuoto */
    if (*sp == 0)
    {
        printf("\nErrore durante un'operazione POP: lo stack è vuoto.");
        return errore_stack;
    }

    /* Estraggo un valore dallo stack */
    (*sp)--;
    *registro = stack[*sp];

    return 0;
}

/* MOV    R0-R31  numero */
void mov(int *registro, int numero)
{
    /* Salviamo un numero in un registro */
    *registro = numero;
}

/* CALL   posizione */
int call(long *ip, int posizione, int *stack, unsigned int *sp, int dimensione_stack)
{
    int errori = 0;

    (*ip)++;

    /* Salviamo il valore di ip corrente nello stack */
    errori = push(*ip, stack, sp, dimensione_stack);

    /* Eseguiamo il salto per la chiamata di funzione */
    jmp(ip, posizione);

    return errori;
}

/* RET */
int ret(long *ip, int *stack, unsigned int *sp, unsigned int numero_istruzioni)
{
    int posizione = 0;

    /* Preleviamo dallo stack la posizione a cui tornare */
    int errori = pop(&posizione, stack, sp);

    /* Se la posizione indicata non rientra nei limiti consentiti (posizione al di fuori del programma o maggiore di INT_MAX) */
    if ((posizione < 0) || ((unsigned int) posizione >= numero_istruzioni))
    {
        printf("\nC'è stato un errore durante l'esecuzione di un'istruzione RET. La posizione estratta (%d) dallo stack non corrisponde ad una di quelle consentite in questo programma (0 <= posizione < %d.\n", posizione, numero_istruzioni);
        errori = 13;
        return errori;
    }

    /* Eseguiamo il salto */
    (*ip) = posizione - 1;

    return errori;
}

/* JMP    posizione */
void jmp(long *ip, int posizione)
{
    /* Eseguiamo il salto */
    *ip = posizione - 1;
}

/* JZ     posizione */
int jz(long *ip, int posizione, int *stack, unsigned int *sp)
{
    int errori = 0;
    int temp = 0;

    /* Prendiamo dallo stack il numero da valutare */
    errori = pop(&temp, stack, sp);

    /* Se il numero è pari a 0 eseguiamo il salto */
    if (temp == 0)
    {
        *ip = posizione - 1;
    }

    return errori;
}

/* JPOS   posizione */
int jpos(long *ip, int posizione, int *stack, unsigned int *sp)
{
    int errori = 0;
    int temp = 0;

    /* Prendiamo dallo stack il numero da valutare */
    errori = pop(&temp, stack, sp);

    /* Se il numero è positivo eseguiamo il salto */
    if (temp > 0)
    {
        *ip = posizione - 1;
    }

    return errori;
}

/* JNEG   posizione */
int jneg(long *ip, int posizione, int *stack, unsigned int *sp)
{
    int errori = 0;
    int temp = 0;

    /* Prendiamo dallo stack il numero da valutare */
    errori = pop(&temp, stack, sp);

    /* Se il numero è negativo eseguiamo il salto */
    if (temp < 0)
    {
        *ip = posizione - 1;
    }

    return errori;
}

/* ADD    R0-R31  R0-R31 */
int add(int addendo1, int addendo2, int *stack, unsigned int *sp, int dimensione_stack)
{
    int errori = 0;
    int risultato = 0;

    /* Verifica overflow */
    /* numero_positivo + numero_positivo */
    if ((addendo1 > 0) && (addendo2 > 0))
    {
        if ((INT_MAX - addendo1) < addendo2)
        {
            printf("\nErrore: overflow durante un'addizione.");
            return errore_operazione;
        }
    }

    /* Verifica overflow */
    /* numero_negativo - numero_negativo */
    if ((addendo1 < 0) && (addendo2 < 0))
    {
        /* Se il risultato + positivo */
        if ((INT_MIN - addendo1) > addendo2)
        {
            printf("\nErrore: overflow durante un'addizione.");
            return errore_operazione;
        }
    }

    risultato = addendo1 + addendo2;

    /* Salvo il risultato nello stack */
    errori = push(risultato, stack, sp, dimensione_stack);

    return errori;
}

/* SUB    R0-R31  R0-R31 */
int sub(int minuendo, int sottraendo, int *stack, unsigned int *sp, int dimensione_stack)
{
    int errori = 0;
    int risultato = 0;

    /* Verifica overflow */
    /* numero_negativo - numero_positivo */
    if ((minuendo < 0) && (sottraendo > 0))
    {
        /* Se il risultato + positivo */
        if ((minuendo == INT_MIN) || ((INT_MIN - minuendo) > -sottraendo))
        {
            printf("\nErrore: overflow durante una sottrazione.");
            return errore_operazione;
        }
    }

    /* Verifica overflow */
    /* numero_positivo - numero_negativo */
    if ((minuendo >= 0) && (sottraendo < 0))
    {
        /* Se il risultato è negativo */
        if ((sottraendo == INT_MIN) || ((INT_MAX - minuendo) < (-1) * sottraendo))
        {
            printf("\nErrore: overflow durante una sottrazione.");
            return errore_operazione;
        }
    }

    risultato = minuendo - sottraendo;

    /* Salvo il risultato nello stack */
    errori = push(risultato, stack, sp, dimensione_stack);

    return errori;
}

/* MUL    R0-R31  R0-R31 */
int mul(int fattore1, int fattore2, int *stack, unsigned int *sp, int dimensione_stack)
{
    int errori = 0;
    int risultato = 0;

    /* Verifica overflow */
    if ((fattore1 != 0) && (fattore2 != 0))
    {
        if (((fattore1 == INT_MIN) && (fattore2 == -1)) || ((fattore2 == INT_MIN) && (fattore1 == -1)))
        {
            printf("\nErrore: overflow durante una moltiplicazione.");
            return errore_operazione;
        }
        else if (((fattore1 > 0) && (fattore2 > 0)) || ((fattore1 < 0) && (fattore2 < 0)))
        {
            if (fattore1 > 0)
            {
                if (fattore1 > (INT_MAX / fattore2))
                {
                    printf("\nErrore: overflow durante una moltiplicazione.");
                    return errore_operazione;
                }
            }
            else
            {
                if (fattore1 < (INT_MAX / fattore2))
                {
                    printf("\nErrore: overflow durante una moltiplicazione.");
                    return errore_operazione;
                }
            }
        }
        else if (((fattore1 > 0) && (fattore2 < 0)) || ((fattore1 < 0) && (fattore2 > 0)))
        {
            if (fattore1 > 0)
            {
                if (fattore1 > (INT_MIN / fattore2))
                {
                    printf("\nErrore: overflow durante una moltiplicazione.");
                    return errore_operazione;
                }
            }
            else
            {
                if (fattore1 < (INT_MAX / fattore2))
                {
                    printf("\nErrore: overflow durante una moltiplicazione.");
                    return errore_operazione;
                }
            }
        }
    }

    risultato = fattore1 * fattore2;

    /* Salvo il risultato nello stack */
    errori = push(risultato, stack, sp, dimensione_stack);

    return errori;
}

/* DIV    R0-R31  R0-R31 */
int divisione(int dividendo, int divisore, int *stack, unsigned int *sp, int dimensione_stack)
{
    int errori = 0;
    int risultato = 0;

    /* Divisione per 0 */
    if (divisore == 0)
    {
        printf("\nErrore: divisione per 0.");
        return errore_operazione;
    }
    else if ((dividendo == INT_MIN) && (divisore == -1))
    {
        printf("\nErrore: overflow durante una divisione.");
        return errore_operazione;
    }

    risultato = dividendo / divisore;

    /* Salvo il risultato nello stack */
    errori = push(risultato, stack, sp, dimensione_stack);

    return errori;
}
