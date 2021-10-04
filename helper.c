#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "helper.h"

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

void benchmark(void (*function)(int*, int), int* tab, int nb, int printcsl){
    if(printcsl)
        afficheTableau10(tab, nb);
    float startTime = (float) clock()/CLOCKS_PER_SEC;
    function(tab, nb);
    float endTime = (float) clock()/CLOCKS_PER_SEC;
    if(printcsl)
        afficheTableau10(tab, nb);
    printf("function took %f seconds\n", (endTime - startTime));
}

int* randTab(int nb, int maxValue){
    srand(time(NULL));

    int *tab = (int*) malloc(sizeof(int) * nb);
    int k;
    for(k = 0; k<nb; k++)
        tab[k] = rand() % maxValue;

    return tab;
}

int *alloueTableau(int nbElements){
    return (int*) malloc(sizeof(int)*nbElements);
}

int* copySection(int* tableau, int startIndex, int nb){
    int* tab = alloueTableau(nb);
    int i;
    for(i=0; i<nb; i++){
        tab[i] = tableau[startIndex + i];
    }
    return tab;
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