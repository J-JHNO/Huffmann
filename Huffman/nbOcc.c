#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "nbOcc.h"

#ifndef EOF
#define EOF (-1)
#endif

int nbOcc[256];
int taille = 0;
int prob[256];

int calculNbOcc(char* s){
	FILE* fichier = NULL;
	fichier = fopen(s, "r");

	if (fichier != NULL)
    {
    	int c;
    	while(EOF != (c = fgetc(fichier))){
    		taille++;
    		nbOcc[c]++;
    	}
    	fclose(fichier);
    	return 0;
    }
    else
    {
        return 1;
    }

}

int main(int argc, char* argv[]) {
    int a = calculNbOcc(argv[1]);
    for (int i=0; i<256; i++) {
        if (nbOcc[i] > 0) {
            printf("Le caractère %c apparait %d fois \n", i, nbOcc[i]);
        }
    }
}


