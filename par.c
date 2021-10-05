#include <stdio.h>
#include <stdlib.h>

#include "helper.h"
#include <string.h>
#include <limits.h>

#include <pthread.h>

/** Devoir 1 
- Maxence Decourriere
- Thibaud Simon       

Version parallèle du tri
 **/

typedef struct array{
    int* el;
    int size;
}array;

#define SEUIL 115

#define NUM_THREADS 1

/* Prototypes de fonctions */
//void triFusion(int* T, int n);
void* triInsertion(void * arg);

void triSegmente(int* tableau, int nbElements);

int main(int argc, char *argv[])
{
    int tabSize;
    int* tab = loadArrayFromFile("tab_200k.bin", &tabSize);
    printf("tableau de %d elements chargé.\n",tabSize);
    afficheTableau10(tab,tabSize);

    printf("benchmark sur %d threads.\n",NUM_THREADS);
    benchmark(triSegmente, tab, tabSize, 0);
    afficheTableau10(tab,tabSize);

    free(tab);
}


void* triInsertion(void* args){
    array tab = *(array*)args;

    int i, j, clef;
    for(j=1; j<tab.size; j++){
        clef = tab.el[j];
        i=j;
        while( i>0 && tab.el[i-1]>clef ){
            tab.el[i]=tab.el[i-1];
            i--;
        }
        tab.el[i]=clef;
    }
    return NULL;
}

void triSegmente(int* tableau, int nbElements){
    array tab = {tableau, nbElements};

    int nbPerTable = (tab.size/NUM_THREADS);
    int nbLastTab = tab.size - (NUM_THREADS-1)*nbPerTable;

    array tabs[NUM_THREADS];
    pthread_t tabThreads[NUM_THREADS];

    for(int i=0; i< NUM_THREADS - 1; i++){
        int start = i*nbPerTable;
        tabs[i].el = tab.el + start;
        tabs[i].size = nbPerTable;
    }
    tabs[NUM_THREADS-1].el = tab.el+(NUM_THREADS-1)*nbPerTable;
    tabs[NUM_THREADS-1].size= nbLastTab;

    /* Trier portions de tableaux */
    for(int i=0; i< NUM_THREADS; i++){
        /* Lancer parallélisation */
        pthread_create(&tabThreads[i], NULL, triInsertion, (void*)&tabs[i]);
    }
    for(int i=0; i< NUM_THREADS; i++){
        /* Fin parallélisation */
        pthread_join(tabThreads[i], NULL);
    }
    pthread_t final;
    pthread_create(&final, NULL, triInsertion, (void*)&tab);
    pthread_join(final, NULL);
}
