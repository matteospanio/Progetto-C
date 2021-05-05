#include "P1G145_877546_877485_controllo_programma.h"

int controllo_programma(int **programma, long *programma_file, unsigned int numero_istruzioni, int comando) {
    int errori = 0;
    int halt = 0;       /* flag per indicare se nel programma è presente o meno l'istruzione HALT */
    long ip = -1;

    /* Gli elementi del vettore istruzioni[] conterranno 1 se l'elemento corrispondente di programma[] è un'istruzione, 0 se è un parametro */
    int *istruzioni = (int *)malloc(sizeof(int) * numero_istruzioni);

    /* Se la malloc non è andata a buon fine */
    if (istruzioni == NULL) {
        printf("\nErrore durante l'allocazione di memoria dinamica.\n");
        return malloc_fallita;
    }

    (*programma) = (int *)malloc(sizeof(int) * numero_istruzioni);

    /* Se la malloc non è andata a buon fine */
    if ((*programma) == NULL) {
        printf("\nErrore durante l'allocazione di memoria dinamica.\n");
        return malloc_fallita;
    }

    /* Scorriamo gli elementi di programma_file[]. Per ogni elemento verifichiamo se (a) i codici macchina sono corretti, (b) i parametri non presentano problemi; se non troviamo errori, (c) copiamo gli elementi di programma_file in programma[] e (d) riempiamo il vettore istruzioni[]. */
    while ((errori == 0) && ((++ip) < (long) numero_istruzioni)) {
        /* L'istruzione non corrisponde ad uno dei codici macchina disponibili */
        if ((programma_file[ip] < 0) || ((programma_file[ip] > 2) && (programma_file[ip] < 10)) || ((programma_file[ip] > 12) && (programma_file[ip] < 20)) || ((programma_file[ip] > 25) && (programma_file[ip] < 30)) || (programma_file[ip] > 33)) {
            printf("\nL'istruzione alla riga [%ld] non corrisponde ad uno dei codici macchina disponibili.\n", ip);
            errori = errori + codice_macchina_non_riconosciuto;
        }
        /* HALT */
        else if (programma_file[ip] == 0) {
            halt = 1;
            (*programma)[ip] = (int) programma_file[ip];
            istruzioni[ip] = 1;
        /* PRINT_STACK */
        }
        else if (programma_file[ip] == 2) {
            /* Controlliamo che programma_file[] contenga elementi a sufficienza affinché l'istruzione analizzata abbia accesso a tutti i suoi parametri */
            errori = errori + test_parametri(ip, 1, numero_istruzioni);

            if (errori == 0) {
                (*programma)[ip] = (int) programma_file[ip];
                istruzioni[ip] = 1;
                ip++;

                /* Verichiamo che il parametro di PRINT_STACK sia un numero positivo */
                if ((programma_file[ip] >= 0) && (programma_file[ip] <= INT_MAX)) {
                    (*programma)[ip] = (int) programma_file[ip];
                    istruzioni[ip] = 0;
                }
                else {
                    printf("\nIl parametro alla riga [%ld] non rientra nei limiti consentiti. Dev'essere compreso fra 0 e %d\n", ip, INT_MAX);
                    errori = errori + numero_non_valido;
                }
            }
        /* RET */
        } else if (programma_file[ip] == 21) {
            (*programma)[ip] = (int) programma_file[ip];
            istruzioni[ip] = 1;
        /* MOV */
        } else if (programma_file[ip] == 12) {
            /* Controlliamo che programma_file[] contenga elementi a sufficienza affinché l'istruzione analizzata abbia accesso a tutti i suoi parametri */
            errori = errori + test_parametri(ip, 2, numero_istruzioni);

            if (errori == 0) {
                (*programma)[ip] = (int) programma_file[ip];
                istruzioni[ip] = 1;
                ip++;

                /* Verifichiamo che il registro esista */
                if (test_registro(ip, programma_file[ip])) {
                    (*programma)[ip] = (int) programma_file[ip];
                    istruzioni[ip] = 0;
                    ip++;

                    /* Verifichiamo che il parametro rientri nei range consentiti */
                    if ((programma_file[ip] >= INT_MIN) && (programma_file[ip] <= INT_MAX)) {
                        (*programma)[ip] = (int) programma_file[ip];
                        istruzioni[ip] = 0;
                    }
                    else {
                        printf("\nIl parametro alla riga [%ld] non rientra nei limiti consentiti. Dev'essere compreso fra %d e %d\n", ip, INT_MIN, INT_MAX);
                        errori = errori + numero_non_valido;
                    }
                }
                else {
                    errori = errori + errore_registro;
                }
            }
        /*
         DISPLAY, PUSH o POP
         Considerando i codici macchina analizzati negli if precedenti:
         "(programma[ip] < 12)" corrisponde a "(programma[ip] == 10 || programma[ip] == 11)"
         È questo il motivo per cui in istruction_set_stampa.h le funzioni print() e jump() sono state dichiarate separatamente.
         */
        }
        else if (programma_file[ip] < 12) {
            /* Controlliamo che programma_file[] contenga elementi a sufficienza affinché l'istruzione analizzata abbia accesso a tutti i suoi parametri */
            errori = errori + test_parametri(ip, 1, numero_istruzioni);

            if (errori == 0) {
                (*programma)[ip] = (int) programma_file[ip];
                istruzioni[ip] = 1;
                ip++;

                /* Verifichiamo che il registro esista */
                if (test_registro(ip, programma_file[ip])) {
                    (*programma)[ip] = (int) programma_file[ip];
                    istruzioni[ip] = 0;
                }
                else {
                    errori = errori + errore_registro;
                }
            }
        /*
         CALL, JMP, JZ, JPOS o JNEG
         "(programma[ip] < 30)" corrisponde a "(programma[ip] == 20 || programma[ip] == 22 || programma[ip] == 23 || programma[ip] == 24 || programma[ip] == 25)"
         */
        } else if (programma_file[ip] < 30) {
            /* Controllo aggiuntivo su CALL. È necessario verificare che il valore di ip da salvare nello stack sia valido. */
            if (programma_file[ip] == 20)
                if (ip > INT_MAX) {
                    printf("\nIstruzione CALL non valida alla riga [%ld]: tutte le istruzioni CALL devono trovarsi entro le prime %d istruzioni in codice macchina.\n", ip, INT_MAX);
                    errori = errori + errore_salto;
                }

            /* Controlliamo che programma_file[] contenga elementi a sufficienza affinché l'istruzione analizzata abbia accesso a tutti i suoi parametri */
            errori = errori + test_parametri(ip, 1, numero_istruzioni);

            if (errori == 0) {
                (*programma)[ip] = (int) programma_file[ip];
                istruzioni[ip] = 1;
                ip++;

                /* Verifichiamo che il parametro rientri nei range consentiti */
                if (test_salto(ip, programma_file[ip], numero_istruzioni)) {
                    (*programma)[ip] = (int) programma_file[ip];
                    istruzioni[ip] = 0;
                }
                else {
                    errori = errori + errore_salto;
                }
            }
        /*
         ADD, SUB, MUL o DIV
         "else" corrisponde a "if (programma[ip] == 30 || programma[ip] == 31 || programma[ip] == 32 || programma[ip] == 33)"
         */
        } else {
            /* Controlliamo che programma_file[] contenga elementi a sufficienza affinché l'istruzione analizzata abbia accesso a tutti i suoi parametri */
            errori = errori + test_parametri(ip, 2, numero_istruzioni);

            if (errori == 0) {
                (*programma)[ip] = (int) programma_file[ip];
                istruzioni[ip] = 1;
                ip++;

                /* Verifichiamo che il registro esista */
                if (test_registro(ip, programma_file[ip])) {
                    (*programma)[ip] = (int) programma_file[ip];
                    istruzioni[ip] = 0;
                    ip++;

                    /* Verifichiamo che il registro esista */
                    if (test_registro(ip, programma_file[ip])) {
                        (*programma)[ip] = (int) programma_file[ip];
                        istruzioni[ip] = 0;
                    }
                    else {
                        errori = errori + errore_registro;
                    }
                }
                else {
                    errori = errori + errore_registro;
                }
            }
        }
    }

    ip = -1;

    /*
     Controllo sulle istruzioni che eseguono un salto (CALL, JMP, JZ, JPOS e JNEG). La posizione a cui cercano di spostarsi dev'essere un'istruzione, non il parametro di una qualche istruzione precedente.
     Es (caso di errore):
     [27] ADD R1 R4
     [30] JMP 28
     Nell'esempio JMP tenta di eseguire un salto nell'istruzione che contiene il parametro R1 relativo all'ADD precedente.
     */
    if (errori == 0)
        while (((++ip) < (long) numero_istruzioni)) {
            /* Se programma[i] contiene un'istruzione */
            if (istruzioni[ip] == 1)
                /* Se programma[i] corrisponde ad una delle istruzioni che eseguono un salto */
                if (((*programma)[ip] == 20) || (((*programma)[ip] > 21) && ((*programma)[ip] < 30))) {
                    /* Se il parametro dell'istruzione che esegue un salto è errato */
                    if (istruzioni[(*programma)[ip + 1]] == 0) {
                        printf("\nL'istruzione di salto (CALL, JMP, JZ, JPOS o JNEG) alla riga [%ld] non punta ad un'istruzione.\n", ip);
                        errori = errori + errore_salto;
                    }
                }
        }
/*
    for (int i = 0; i < numero_istruzioni; i++)
        printf("\n%d", istruzioni[i]);*/

    /* Se non abbiamo trovato l'istruzione HALT */
    if ((errori == 0) && (halt == 0)) {
        printf("\nQuesto programma è privo dell'istruzione HALT. ");
        if (comando == 0)
            printf("Lo stamperò comunque.\n");
        else
            printf("Proverò ad eseguirlo comunque, nel caso il programma sia pensato appositamente per finire in un loop infinito.\n");
    }

    free(istruzioni);

    return errori;
}

/* Verifica che i parametri necessari a stampare una funzione non superino il numero massimo di elementi contenuti in programma[] */
int test_parametri(long ip, int parametri, unsigned int numero_istruzioni)
{
    /* Se le istruzioni in codice macchina contenute in programma[] non bastano */
    if ((ip + parametri) >= (long) numero_istruzioni) {
        printf("\nErrore nel file di input, non trovo i parametri necessari per l'istruzione alla riga [%ld]. Controllate il file di input e richiamatemi.\n", ip);
        return errore_parametri;
    }

    /* Se non ci sono problemi */
    return 0;
}

/* Verifica se il parametro registro è compreso fra 0 e 31. Se non lo è, restituisce un errore. */
int test_registro(long ip, long registro) {
    /* Se il registro non esiste */
    if ((registro < 0) || (registro > 31)) {
        printf("\nIl registro R%ld indicato alla riga [%ld] non è fra quelli esistenti. Ci sono solo i registri R0-R31. Controllate il file di input e richiamatemi.\n", registro, ip);
        return 0;
    }

    /* Se non ci sono problemi */
    return 1;
}

/* Verifica se posizione è compreso fra 0 e numero_istruzioni (escluso); deve anche essere minore di INT_MAX. Se non lo è, restituisce un errore. */
int test_salto(long ip, long posizione, unsigned int numero_istruzioni) {
    /* Se la posizione indicata non rientra nei limiti consentiti (posizione al di fuori del programma o maggiore di INT_MAX) */
    if ((posizione < 0) || (posizione >= numero_istruzioni) || (posizione > INT_MAX)) {
        printf("\nC'è un errore nell'istruzione di salto (CALL, JMP, JZ, JPOS o JNEG) alla riga %ld: il relativo parametro %ld punta al di fuori dei limiti consentiti. Le istruzioni di salto possono puntare a posizioni comprese fra 0 e %d; in caso di programmi in codice macchina molto lunghi, le istruzioni di salto possono comunque puntare solo fino alla %d-esima istruzione.\n", (ip - 1), posizione, (numero_istruzioni - 1), INT_MAX);
        return 0;
    }
    /* Se non ci sono problemi */
    return 1;
}
