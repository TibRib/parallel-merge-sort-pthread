#include <stdio.h>
#include <stdlib.h>

#include "helper.h"
#include <string.h>
#include <limits.h>

/** Devoir 1 
- Maxence Decourriere
- Thibaud Simon       

Version séquentielle du tri
 **/

#define SEUIL 115

/* Prototypes de fonctions */
void triFusion(int* T, int n);
void triInsertion(int* tab, int nbElements);

int thresholdFusionToInsert();

int main(int argc, char* argv[]){ 
    int tailleTableau;
    int nbElements;
    char* filename;
    int* fArray;
    /*
    float sum;
    for(int i=0; i<1000; i++){
        sum += thresholdFusionToInsert();
    }
    printf("seuil moyenne %f\n", sum/1000.0f);
    */
    

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

    printf("nb elements fusion = %d\n",nbElements);
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
    
    int usize = n/2; //10/2 = 5 
    int* U = copySection(T, 0, usize); /* Copie sur n+1 éléments pour permettre affectation de fonction Fusion */
    afficheTableau10(U, usize);
    triFusion(U, usize);

    int vsize = n - usize;
    int* V = copySection(T, usize, vsize); /* Copie sur m+1 éléments pour permettre affectation de fonction Fusion */
    afficheTableau10(V, vsize);
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
        tab2 = copySection(tab1,0,nbIteration);
        benchInsert = benchmark(triInsertion, tab1, nbIteration, 0);
        benchFusion = benchmark(triFusion, tab2, nbIteration, 0);
        free(tab1);
        free(tab2);
    
    }while( benchFusion > benchInsert );
    //printf("seuil est %d \n",nbIteration);
    return nbIteration;
}
