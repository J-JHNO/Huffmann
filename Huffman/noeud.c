#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct noeud noeud;

struct noeud {
	char* nom;
	int nbOcc;
	struct noeud *fg;
	struct noeud *fd;
	struct noeud *pere;
}

noeud* Arbre = malloc(511*sizeof(noeud));

void init() {

	for (int i=0; i<256; i++) {
		noeud n(i,nbOcc[i],NULL,NULL,NULL);
		Arbre[i] = n;
	}
}