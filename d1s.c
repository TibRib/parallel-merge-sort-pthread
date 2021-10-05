#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>

/** Devoir 1 
- Maxence Decourriere
- Thibaud Simon       

Version séquentielle du tri **/


/* Fonctions utilitaires */
void afficheTableau10(int* tab, int tabSize);
int* randTab(int nb, int maxValue);
void writeArrayToFile(char* filename, int *tab, int tabSize);
int* loadArrayFromFile(char* filename, int *nbElements);
int *alloueTableau(int nbElements);
int* copySection(int* tableau, int startIndex, int nb);
int* dupliqueSection(int* tableau, int startIndex, int nb);
long benchmark(void (*function)(int*, int), int* tab, int nb, int printcsl);

/* Structures et définitions */
#define SEUIL 100
#define NS_PER_SECOND 1000000000

/* Prototypes de fonctions */
void triFusion(int* T, int n);
void triInsertion(int* tab, int nbElements);
int thresholdFusionToInsert();

int main(int argc, char* argv[]){ 
    int tailleTableau;
    int nbElements;
    char* filename;
    int* fArray;
    
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
        fArray = randTab(nbElements, 10000);
        writeArrayToFile(filename, fArray, nbElements);
        printf("Tableau enregistré dans le fichier '%s'\n",filename);
        free(fArray);
    }
    else{
        char* tri = argv[1];
        char* filename = argv[2];
        int printArg = (argc>3)? !strcmp(argv[3], "-p") : 0;

        int* fArray = loadArrayFromFile(filename,&tailleTableau);

        if(strcmp(tri,"insert") == 0){
            benchmark(triInsertion, fArray, tailleTableau, printArg);
        } else if(strcmp(tri,"fusion") == 0){
            benchmark(triFusion, fArray, tailleTableau, printArg);
        } else{
            puts("Entrer un type de tri valide : insert   ou   fusion");
        }
        free(fArray);
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

void triFusion(int* T, int n){
    if(n < 2){
        return;
    }
    else if(n < SEUIL){
        return triInsertion(T,n);
    }
    
    int usize = n/2;
    int* U = copySection(T, 0, usize);
    triFusion(U, usize);

    int vsize = n - usize;
    int* V = copySection(T, usize, vsize); 
    triFusion(V, vsize);
    
    fusion(U,usize, V,vsize, T);
}

void triInsertion(int* tab, int nbElements){
    int i, j, clef;
    for(j=1; j<nbElements; j++){
        clef = tab[j];
        i=j;
        while( i>0 && tab[i-1]>clef ){
            tab[i]=tab[i-1];
            i--;
        }
        tab[i]=clef;
    }
    return;
}


int thresholdFusionToInsert(){
    int nbIteration = 10;
    long benchInsert;
    long benchFusion;
    int *tab1, *tab2;

    do{
        nbIteration++;
        tab1 = randTab(nbIteration, 10000);
        tab2 = dupliqueSection(tab1,0,nbIteration);
        benchInsert = benchmark(triInsertion, tab1, nbIteration, 0);
        benchFusion = benchmark(triFusion, tab2, nbIteration, 0);
        free(tab1);
        free(tab2);
    
    }while( benchFusion > benchInsert );
    printf("seuil est %d \n",nbIteration);
    return nbIteration;
}




/*** Fonctions utilitaires ***/

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

//Alloue et duplique la mémoire interne d'un tableau
int* dupliqueSection(int* tableau, int startIndex, int nb){
    int* tab = alloueTableau(nb);
    int i;
    for(i=0; i<nb; i++){
        tab[i] = tableau[startIndex + i];
    }
    return tab;
}

//Arithmétique de pointeurs sans allocations
int* copySection(int* tableau, int startIndex, int nb){
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
