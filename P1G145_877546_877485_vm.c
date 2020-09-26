/*
 Marco  Spanio  877546
 Matteo Spanio  877485

 Progetto Finale
 Implementazione di una macchina virtuale

 MACCHINA VIRTUALE

 Lo scopo del progetto è implementare una macchina virtuale con registri e stack che esegua dei programmi in codice macchina contenuti in un'array di interi nel quale ciascuna istruzione occupa un numero variabile di posizioni.

 Programmi in codice macchina
 I programmi da stampare/eseguire sono contenuti in file di testo (*.cvm).

 Lanciare il programma
 All'avvio della macchina virtuale è necessario aggiungere due parametri:
 (1) modalità da eseguire (la macchina virtuale ne ha 2):
    - "stampa" permette di visualizzare su console il programma contenuto nel file in input
    - "esegui" esegue le istruzioni scritte in codice macchina
 (2) nome del file di input
 Esempio: ./vm stampa esempio.cvm
 */

/*
 All'interno del programma abbiamo provato a gestire alcuni possibili errori come file di input con contenuti non validi. Stando a quanto ci è stato detto i file di input dovrebbero contenere sempre programmi corretti. Per questo motivo, parte delle nostre funzioni possono essere considerate "superflue" (visto che gestiscono errori che non dovrebbero verificarsi).
 */

#include <stdio.h>
#include <string.h>
#include "P1G145_877546_877485_stampa.h"
#include "P1G145_877546_877485_esegui.h"

/* Codici errore */
#define errore_pochi_parametri 1
#define operazione_non_riconosciuta 2

/* argv[] è un vettore contenente i parametri digitati a terminale quando si lancia questo programma.
 Il computer prende come primo parametro il programma stesso (./vm).
 I due parametri stampa/esegui e il file.txt, che noi consideriamo il primo e secondo parametro, vengono in realtà salvati nella seconda e terza posizione del vettore argv.
 argc invece dovrebbe corrispondere al numero di parametri passati ma, per il motivo appena citato, lanciando correttamente il programma verranno conteggiati 3 parametri e non 2.
 */

int main(int argc, char const *argv[])
{
    int errori = 0;

    /* Verifica numero di parametri digitati lanciando il programma */
    /* Ci sono più parametri del necessario: uso i primi due ignorando il resto */
    if ((argc - 1) > 2)
    {
        printf("\nSono stati digitati più parametri (%d) del previsto; valuterò solo i primi 2, ignorando il resto.\n", (argc - 1));
        /* I parametri non sono sufficienti: il programma si ferma */
    } else if ((argc - 1) < 2)
    {
        printf("\nSono stati digitati meno parametri (%d) del previsto. Per funzionare correttamente ho bisogno di almeno 2 parametri.\nIl primo deve essere \"esegui\" o \"stampa\".\nIl secondo è il nome del file contenente un programma in linguaggio macchina.\nRichiamatemi con tutti i parametri necessari.\n\n", (argc - 1));
        return errore_pochi_parametri;
    }

    printf("\nOperazione richiesta: %s\n", argv[1]);

    /* Controlliamo che tipo di operazione devo eseguire con il file in input e richiamiamo la relativa funzione: */
    /* Esegui */
    if (strcmp(argv[1], "esegui") == 0)
    {
        errori = esegui(argv[2]);
    /* Stampa */
    } else if (strcmp(argv[1], "stampa") == 0)
    {
        errori = stampa(argv[2]);
    /* Se il parametro ricevuto non è corretto: */
    } else
    {
        printf("\nOperazione non riconosciuta, rilanciatemi digitando \"stampa\" oppure \"esegui\".\n\n");
        errori = operazione_non_riconosciuta;
    }

    return errori;
}
