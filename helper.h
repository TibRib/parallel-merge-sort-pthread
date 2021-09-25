#ifndef HELPER_H
#define HELPER_H

void benchmark(void (*function)(int*, int), int* tab, int nb, int printcsl);
void afficheTableau(int* tab, int tabSize);
int* randTab(int nb, int maxValue);

void writeArrayToFile(char* filename, int *tab, int tabSize);
int* loadArrayFromFile(char* filename, int *nbElements);

int *alloueTableau(int nbElements);
int* copySection(int* tableau, int startIndex, int nb);

#endif