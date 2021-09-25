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
void fusion(int* U, int n, int* V, int m, int* T);
void triFusion(int* T, int n);
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
        } else{
            puts("Entrer un type de tri valide : insert   ou   fusion");
        }
        free(fArray);
    }

    return 0;
}

void triFusion(int* T, int n){

    printf("Travail sur : ");
    afficheTableau(T,n);
    
    //Si n est petit, tri insertion
    if(n <= 1){
        return;
    }
    /*
    if(n < 3){
        triInsertion(T, n);
    } */
    

    //Allocations <-
    int nb2 = n/2;

//[1 2 3 4 5]
//U = [ 1 2 3 ] (nb = 3)   index = 0,   nb = 3 = nb2 + ((n%2!=0)?1:0))
//V = [ 4 5]  (nb = 2) index = nb2 + ((n%2!=0)?1:0) nb = nb2

    int *U = copySection(T,0,nb2 + n%2 );
    int *V = copySection(T, nb2 + 1 + n%2, nb2);

    triFusion(U, nb2 + n%2);
    triFusion(V, nb2); //ICI
    
    fusion(U, nb2, V, nb2, T);

    //TODO : free
}


void fusion(int* U, int n, int* V, int m, int* T){
    int i,j;
    i = 0;
    j = 0;

    #define MAXINT 2147483647

    // ???
    //U[n] = MAXINT; //ça va péter
    //V[m] = MAXINT; //là aussi

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