#include <stdio.h>
#include <stdlib.h>

#include "helper.h"
#include <string.h>
#include <limits.h>

#include <pthread.h>
#include <time.h>

/** Devoir 1 
- Maxence Decourriere
- Thibaud Simon       

Version parallèle du tri
 **/

typedef struct array{
    int* el;
    int size;
}array;

int nbThreads = 0;

#define SEUIL 100

#define NUM_THREADS 4

/* Prototypes de fonctions */
void* triFusion(void * args);
void* triInsertion(void * arg);
void* launchFusion(void * arg);

void triSegmente(int* tableau, int nbElements);

#define NS_PER_SECOND 1000000000
void sub_timespec(struct timespec t1, struct timespec t2, struct timespec *td)
{
    td->tv_nsec = t2.tv_nsec - t1.tv_nsec;
    td->tv_sec  = t2.tv_sec - t1.tv_sec;
    if (td->tv_sec > 0 && td->tv_nsec < 0)
    {
        td->tv_nsec += NS_PER_SECOND;
        td->tv_sec--;
    }
    else if (td->tv_sec < 0 && td->tv_nsec > 0)
    {
        td->tv_nsec -= NS_PER_SECOND;
        td->tv_sec++;
    }
}
long benchmark(void* (*function)(void*), array* tab, int printcsl){
    if(printcsl)
        afficheTableau10(tab->el, tab->size);
        
    struct timespec start, finish, delta;
   
    clock_gettime(CLOCK_REALTIME, &start);

    function((void*)tab);
    
    clock_gettime(CLOCK_REALTIME, &finish);
    sub_timespec(start, finish, &delta);
   
    if(printcsl)
        afficheTableau10(tab->el, tab->size);
        
    printf("function took %d.%.9ld seconds\n", (int)delta.tv_sec, delta.tv_nsec);

    return (long)delta.tv_sec*NS_PER_SECOND + (long)delta.tv_nsec;
}



int main(int argc, char *argv[])
{
    int nbElements;
    char* filename;
    array fArray;

    printf("%d Threads utilisés.\n",NUM_THREADS);
    
    if(argc < 3 ){
        puts("Utilisation : fusion/insert <nom de fichier> (-p (optionnel : print to console))");
        puts("Autre possibilite : utiliser l'argument gen <nb_elements> <nom_fichier>\n");
        exit(1);
    }

    if(strcmp(argv[1],"gen") == 0){
        if(argc != 4){
              puts("Entrez un nom de fichier valide.\n ex: ./seq gen 376 nom_fichier.bin\n");
            return(1);
        }
        nbElements = atoi(argv[2]);
        if(nbElements < 1){
            puts("Entrez un nombre d'elements valide.\n ex: ./seq gen 376 tab_376.bin\n");
            return(1);
        }
        
        filename = argv[3];
        fArray.el = randTab(nbElements, 10000);
        fArray.size = nbElements;
        writeArrayToFile(filename, fArray.el, nbElements);
        printf("Tableau enregistré dans le fichier '%s'\n",filename);
        free(fArray.el);
    }
    else{
        char* tri = argv[1];
        char* filename = argv[2];
        int printArg = (argc>3)? !strcmp(argv[3], "-p") : 0;

        fArray.el = loadArrayFromFile(filename,&(fArray.size));

        if(strcmp(tri,"insert") == 0){
            benchmark(triInsertion, &fArray, printArg);
        } else if(strcmp(tri,"fusion") == 0){
            benchmark(launchFusion, &fArray, printArg);
        } else{
            puts("Entrer un type de tri valide : insert   ou   fusion");
        }
        free(fArray.el);
    }
    
    return 0;
}


void fusion(int* U, int n, int* V, int m, int* T){
    int i,j,k;

    int nbElements = m+n;

    int* UTemp = alloueTableau(n+1);
    for(i=0; i<n; i++) UTemp[i] = U[i];
    UTemp[n] = INT_MAX;

    int* VTemp = alloueTableau(m+1);
    for(i=0; i<m; i++) VTemp[i] = V[i];
    VTemp[m] = INT_MAX;

    i = 0;
    j = 0;
    k = 0;
    
    for(k=0; k < nbElements; k++){
        if(UTemp[i] < VTemp[j]){
            T[k] = UTemp[i++]; 
        }
        else{
            T[k] = VTemp[j++];
        }
    }

    free(UTemp);
    free(VTemp);
}

void* launchFusion(void *args){
    array tab = *(array*)args;
    int i;
    
    int nbPerTable = (tab.size/NUM_THREADS);
    int nbLastTab = tab.size - (NUM_THREADS-1)*nbPerTable;

    array tabs[NUM_THREADS];
    pthread_t tabThreads[NUM_THREADS];

    for(i=0; i< NUM_THREADS - 1; i++){
        int start = i*nbPerTable;
        tabs[i].el = tab.el + start;
        tabs[i].size = nbPerTable;
    }
    tabs[NUM_THREADS-1].el = tab.el+(NUM_THREADS-1)*nbPerTable;
    tabs[NUM_THREADS-1].size= nbLastTab;

    /* Trier portions de tableaux */
    for(i=0; i< NUM_THREADS; i++){
        /* Lancer parallélisation */
        pthread_create(&tabThreads[i], NULL, triFusion, (void*)&tabs[i]);
    }
    for(i=0; i< NUM_THREADS; i++){
        /* Fin parallélisation */
        pthread_join(tabThreads[i], NULL);
    }

    array aFusionner[NUM_THREADS];
    for (i = 0; i < NUM_THREADS; i++)
    {
        aFusionner[i] = tabs[i];
    }

    int nbAFusionner = NUM_THREADS;
    int b = 0;

    while (nbAFusionner > 1)
    {
        int cpt=0;
        int lastIndex=0;
        for(b=0; b<nbAFusionner-1; b+=2){
            fusion(aFusionner[b].el, aFusionner[b].size, aFusionner[b+1].el, aFusionner[b+1].size, aFusionner[b].el);
            int size = aFusionner[b].size + aFusionner[b+1].size;
            
            array bufferFusion = { copySection(tab.el, lastIndex, size), size};
            lastIndex += size;
            aFusionner[cpt] = bufferFusion;
            cpt++;
        }
        nbAFusionner = nbAFusionner >> 1;
    }

    return NULL;
}


void* triFusion(void* args){
    array arr = *(array*)args;
    int *T = arr.el;
    int n = arr.size;
    if(n < 2){
        return NULL;
    }
    else if(n < SEUIL){
        return triInsertion(args);
    }
    
    /* Section parallele 1 */
    int usize = n/2;
    int* U = copySection(T, 0, usize);
    array uArray = {U, usize};
    triFusion( (void*)&uArray);
    //pthread_t uThread;
    //pthread_create(&uThread, NULL, triFusion, (void*)&uArray);

    /* Section parallele 2 */
    int vsize = n - usize;
    int* V = copySection(T, usize, vsize);
    array vArray = {V, vsize};
    triFusion( (void*)&vArray);
    //pthread_t vThread;
    //pthread_create(&vThread, NULL, triFusion, (void*)&vArray);

    /* Jointure */
    //pthread_join(uThread, NULL);
    //pthread_join(vThread, NULL);
    fusion(U,usize, V,vsize, T);

    return NULL;
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
    triInsertion((void*)&tab);
}
