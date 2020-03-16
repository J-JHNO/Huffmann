#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef EOF
#define EOF (-1)
#endif

char* nom; // tableau des caractères
char* bin[256]; // tableau des codes binaires
char codeBin[9]; // tableau d'un code binaire

int charDif = 0; // nbr de caractères différents du fichier
int nbrNoeud = 0; // nbr de noeud
int lgChar = 0; // longueur d'un caractère

// transforme un décimal en binaire
void decTObin(int A) { 
	printf("Nombre décimal : %d\n", A);
	int q1 = A;
	double r=A%2;
 	double q2;
 	int i=0;

 	if (A==0) { // décimal égal à 0?
 		while (i < 8) {
 		 	codeBin[7-i] = '0';
 		 	i++;
 		}
 	}
 	else {
		while (q1>0) {
			if(r==0) { // reste égal à 0?
				codeBin[7-i]='0';
			}
			else {
				codeBin[7-i]='1';
			}
		 
			q2=q1;
			q1=q1/2;
			r=q1%2;
			i=i+1;
		}
	}
	
}

// décompresse le fichier
void decompression(char* s, char* s2) {
	
	FILE* fichier = NULL;
	fichier = fopen(s,"r"); // ouverture en lecture

	if (fichier != NULL) {
		FILE* fichierDEC = NULL;
		fichierDEC = fopen(s2,"wba"); // ouverture en écriture

		// Decompression en-tête
		printf("Décompression de l'en-tête\n\n");
		int taille = (fgetc(fichier)); // récupération de la taille du fichier
		charDif = (fgetc(fichier)); // récupération du nombre de dcaractère différents présent dans le fichier
		int code; // code décimal 
		printf("Taille du fichier : %i \n", taille);
		printf("Nombre de caractères différents : %i \n", charDif);
		printf("\n");

		for (int i=0; i<charDif; i++) { //parcours de l'en-tête
			nom[i] = fgetc(fichier);
			//printf("nom[i]:%c\n", nom[i]);
			lgChar = fgetc(fichier);
			//printf("lgChar:%d\n", lgChar);
			code = fgetc(fichier);
			//printf("code:%d\n", code);
			/* Dans notre compression, pour chaque caractère, on a 3 informations : le caractère, sa longueur et son code binaire en binaire */
			decTObin(code); // appel du décodage binaire
			//bin[i] = codeBin;
			int cpt=0;
			char* trns = NULL;
			trns = malloc(lgChar*sizeof(char));
			printf("codeBindeb: %s\n", codeBin);
			
			for (int k=0; k<lgChar; k++) {
				trns[k] = codeBin[cpt]; // Technical Reverse of the Natural Sense
				// printf("cpt: %d\n", cpt);
				// printf("codeBin: %c\n", codeBin[cpt]);
				cpt++;
			}
			
			//printf("trns:%s\n", trns);
			bin[i]=trns;
			printf("nom:%c lgChar:%d code décimal:%d code binaire:%s\n\n", nom[i],lgChar,code,bin[i]);
			
		}
		
		// Decompression reste du fichier
		printf("\nDécompression du reste du fichier :\n\n");
		int c,A=0;
		char* S = malloc(8*sizeof(char));
		while (EOF != (c = fgetc(fichier))) {
			decTObin(c);
			int check; // é check et mat
			int t=0;
			int indice=0;
			for (int j=0; j<8; j++) {
				check=7;
				char caz = codeBin[j]; // le mot gaz mais avec un c 
				S[t] = caz;
				t++;
				for (int i=0; i<charDif; i++) {
					for (int k=0; k<strlen(S); k++) {
						if ((S[k] == bin[i][k]) && (strlen(S)==strlen(bin[i]))){
							fputc(nom[i],fichierDEC);
							free(S);
							t=0;
							check = -1;
						}
					}

				}
			}
		}
	}
}

int main(int argc, char** argv) {
	
	nom = malloc(256*sizeof(char));

	decompression(argv[1],argv[2]);
	printf("--------------------------\n");
	nbrNoeud = (2*charDif)-1;
	printf("nbrNoeud : %d\n", nbrNoeud);

	printf("--------------------------\n");
	printf("\nRécupération des codes binaires : \n\n");
	for (int i=0; i<charDif; i++) {
		printf("%c:%s\n", nom[i],bin[i]);
	}
	
}