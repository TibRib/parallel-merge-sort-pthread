#include <stdio.h>
#include <stdlib.h>

#include "helper.h"
#include <string.h>

/** Devoir 1 
- Maxence Decourriere
- Thibaud Simon       

Version séquentielle du tri
 **/

/* Prototypes de fonctions */
void triFusion(int* T, int n);
void triFusion2(int* Tab, int start, int end);
void triInsertion(int* tab, int nbElements);

int main(int argc, char* argv[]){ 
    if(argc < 3 ){
        puts("Utilisation : fusion/insert <nom de fichier> (-p (optionnel : print to console))");
        puts("Autre possibilite : utiliser l'argument gen <nb_elements> <nom_fichier>\n");
        exit(1);
    }
    
    int tailleTableau;

    if(strcmp(argv[1],"gen") == 0){
        if(argc != 4){
              puts("Entrez un nom de fichier valide.\n ex: ./seq gen 376 nom_fichier.bin\n");
            return(1);
        }
        int nbElements = atoi(argv[2]);
        if(nbElements < 1){
            puts("Entrez un nombre d'elements valide.\n ex: ./seq gen 376 tab_376.bin\n");
            return(1);
        }
        
        char* filename = argv[3];
        int* fArray = randTab(nbElements, 10000);
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
            //benchmarkd(triFusion2, fArray, tailleTableau, printArg);
        } else{
            puts("Entrer un type de tri valide : insert   ou   fusion");
        }
        free(fArray);
    }

    return 0;
}

void afficheSection(int* arr, int start, int end){
    printf("[");
    for(int i=start; i<end-1; i++){
        printf(" %d,",arr[i]);
    }
    printf(" %d",arr[end-1]);
    printf("]\n (%d elements)\n\n", end-start);
}


void fusion(int* U, int n, int* V, int m, int* T){
    int i,j;
    i = 0;
    j = 0;

    int nbElements = m+n;
    for(int k=0; k <= nbElements; k++){
        if(U[i] < V[j]){
            T[k] = U[i++];
        }
        else{
            T[k] = V[j++];
        }
    }
}

void triFusion(int* T, int n){
    if(n < 2){
        return;
    }
    
    printf("Travail sur (n=%d) : ",n);
    afficheTableau(T, n);

    int mid = n/2;

    int* U = copySection(T, 0, mid);
    int usize = mid;
    triFusion(U, usize);
    

    int* V = copySection(T, mid, (n-mid));
    int vsize = n-mid;
    triFusion(V, vsize);
    
    //fusion(U,usize,V,vsize, T);
}


//Ok - validé
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