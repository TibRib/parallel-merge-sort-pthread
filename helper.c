#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <unistd.h>

#include "helper.h"

#define NS_PER_SECOND 1000000000

void afficheTableau(int* tab, int tabSize){
    printf("[");
    int i;
    for(i=0; i<tabSize-1; i++){
        printf(" %d,",tab[i]);
    }
    printf(" %d",tab[tabSize-1]);
    printf("]\n (%d elements)\n\n", tabSize);
}

void afficheTableau10(int* tab, int tabSize){
    if(tabSize < 21){
        return afficheTableau(tab, tabSize);
    }

    printf("[");
    int i;
    for(i=0; i<10; i++){
        printf(" %d,",tab[i]);
    }
    printf(" ...,");
    for(i=tabSize-10; i<tabSize-1; i++){
        printf(" %d,",tab[i]);
    }
    printf(" %d",tab[tabSize-1]);
    printf("]\n (%d elements)\n\n", tabSize);
}

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


long benchmark(void (*function)(int*, int), int* tab, int nb, int printcsl){
    if(printcsl)
        afficheTableau10(tab, nb);
        
    struct timespec start, finish, delta;
   
    clock_gettime(CLOCK_REALTIME, &start);

    function(tab, nb);
    
    clock_gettime(CLOCK_REALTIME, &finish);
    sub_timespec(start, finish, &delta);
   
    if(printcsl)
        afficheTableau10(tab, nb);
        
    printf("function took %d.%.9ld seconds\n", (int)delta.tv_sec, delta.tv_nsec);

    return (long)delta.tv_sec*NS_PER_SECOND + (long)delta.tv_nsec;
}

int* randTab(int nb, int maxValue){
    int *tab;
    int k;
    srand(time(NULL));
    tab = (int*) malloc(sizeof(int) * nb);
    for(k = 0; k<nb; k++)
        tab[k] = rand() % maxValue;

    return tab;
}

int *alloueTableau(int nbElements){
    return (int*) malloc(sizeof(int)*nbElements);
}

int* copySection(int* tableau, int startIndex, int nb){
    /*
    int* tab = alloueTableau(nb);
    int i;
    for(i=0; i<nb; i++){
        tab[i] = tableau[startIndex + i];
    }*/
    return tableau + startIndex;
}

void writeArrayToFile(char* filename, int *tab, int tabSize){
    FILE *f = fopen(filename, "wb");
    if(f == NULL){
        printf("Could not write the file '%s'\n",filename);
        return;
    }
    fwrite(tab, tabSize * sizeof(int), 1, f);
    fclose(f);
}

int* loadArrayFromFile(char* filename, int* nbElements){
    FILE *f = fopen(filename, "rb");
    if(f == NULL){
        printf("Could not read the file '%s'\n",filename);
        return NULL;
    }
    /*Determine the number of bytes taken by the bin file */

    fseek(f, 0L, SEEK_END);
    int tabSize = ftell(f);
    rewind(f);

    /* Deduct the number of ints registered */
    int nbEntiers = tabSize / sizeof(int);
    *nbElements = nbEntiers;
    /* allocate a new array */
    int *array = (int*) malloc(tabSize);

    /* read the values and fill the array */
    fread(array, sizeof(int), nbEntiers, f);

    fclose(f);
    
    return array;
}