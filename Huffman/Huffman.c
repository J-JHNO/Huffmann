#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef EOF
#define EOF (-1)
#endif

int nbOcc[256];
int taille = 0;
int tailleBIN = 0;
int nbrNoeud = 0;
long unsigned int taillemaxCB = 0;
int conv[256];
int *rconv;
int *fg;
int *fd;
int *pere;
int *occ;
char *nom;
char *bin[511];

float myatoi(char c) {
	if (c == '0') return 0.0;
	else /*(c == '1')*/ return 1.0;
}

//Calcul le nombre d'occurrences de chaque caractère présent dans le fichier
int calculNbOcc(char* s){
	FILE* fichier = NULL;
	fichier = fopen(s, "r");

	if (fichier != NULL)
    {
    	int c;
    	while(EOF != (c = fgetc(fichier))){
            //if (nbOcc[c])
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

// Calcul du nombre de feuille
int nbrFeuille() {
	int nbF = 0;
	for (int i=0; i<256; i++) {
		if (nbOcc[i] != 0) {
			nbF++;
		}
	}
	return nbF;
}

// Affiche pour chaque caractère son nombre d'occurrence
void afficheOcc() {
	for (int i=0; i<256; i++) {
    		if (nbOcc[i] > 0) {
    			printf("Le nombre d'occurrences de %c est %d !\n", i, nbOcc[i]);
    		}
    	}
}

// initialise le tableau nom avec les caractères présents dans le fichier
void initNom() {
	int j=0;
	for (int i=0; i<256; i++) {
		if (nbOcc[i] > 0) {
			nom[j] = (char)i;
			j++;
		}
		
	}
}

// initialise l'arbre
void initArbre() {
	int j=0;
	for (int i=0; i<256; i++) {
		if (nbOcc[i]>0) {
			conv[i] = j;
			rconv[j] = i;
			fg[j] = -1;
			fd[j] = -1;
			pere[j] = -1;
			occ[j] = nbOcc[i];
			bin[j] = "/ /";
			j++;
			tailleBIN++;
		}
	}
}

// affiche le recapitulatif avec pour chaque "noeud" son fils gauche, fils droit ect..
void afficheFIN(int* T1, int* T2, int* T3, int* T4, int* T5, char* T6, int nbrF) {
	printf("    fg     fd     pere     occ     rconv     nom\n");

	for (int i=0; i<nbrF; i++) {
		printf("%d: %d       %d       %d       %d       %c       %c\n",i, T1[i], T2[i], T3[i], T4[i], T5[i], T6[i]);
	}

}

// Crée l'arbre en manipulant fg,fd,pere et occ
void creationArbre(int nbF, int nbrNoeud) {
	for (int nn=nbF; nn<nbrNoeud; nn++) {
		int min1=taille+1, min2=taille+1;
		int imin1=taille,imin2=taille;

		// Recherche des 2 plus petits
		for (int i=0; (i<nn); i++) {
			if (pere[i] == -1) {
				if (occ[i] < min1) {
					imin2 = imin1;
					min2 = min1;
					imin1 = i;
					min1 = occ[i];
				}
				else {
					if ((occ[i] < min2) && (pere[i] == -1)) {
						imin2 = i;
						min2 = occ[i];
					}
				}
			}
		}
		pere[imin1] = nn;
		fg[nn] = imin1;
		pere[imin2] = nn;
		fd[nn] = imin2;
		occ[nn] = occ[imin1] + occ[imin2];
		pere[nn] = -1;
		// char* c1 = (char)imin1;
		// char* c2 = (char)imin2;
		// char *s1= malloc(2*sizeof(char));
		// char *s2= malloc(2*sizeof(char));
		// strcpy(s1,c1);
		// strcpy(s2,c2);
		// rconv[nn] = strcat(s1, s2);
		// free(s1);
		// free(s2);
	}
}
	
// Affiche le code binaire (en chaine de caractère) de chaque caractère
void parcours(int n, char *code, int nbF) {
	if (fg[n] == -1 /* Feuille? */) {
		char* c = code;
		bin[n] = c;
		//printf("bin[%d]: %s\n",n,bin[n] );
		// 	printf("bin[n]: %s\n", bin[n]);
		// 	printf("n: %d\n", n);
		//printf("etape %d\n", i);
		printf("%c : %s \n",nom[n], code); // Ex: a:0111
	}
	else {
		char *s= malloc(strlen(code)+2); // 1\0
		strcpy(s,code);
		strcat(s, "0");
		parcours(fg[n],s,nbF); // Recursif
		strcat((strcpy(s,code)), "1");
		parcours(fd[n],s,nbF); // Recursif
		
	}
	
}

// Creer le tableau des codes binaires 
void creeBIN(int nbrNoeud, int nbF) {
	parcours(nbrNoeud-1, "", nbF);
}

// void binReduit() {
// 	for (int i=0; i<tailleBIN; i++) {
// 		if (bin[i] != "/ /") {
// 			for (int j=1; j<tailleBIN; j++) {
// 				bin[j-1] = bin[j];
// 			}
// 			tailleBIN--;
// 		}
// 	}
// }



// Affiche les caractères avec leurs code binaire
void afficheBIN() {
	for (int i=0; i<tailleBIN; i++) {
		printf("%c : %s \n", nom[i], bin[i]);
	}
}

void maxCB() {
	for (int i=0; i<tailleBIN; i++) {
		if (strlen(bin[i]) > taillemaxCB) {
			taillemaxCB = strlen(bin[i]);
		}
	}
}

void compression(char* s, char* s2) {
	FILE* fichier = NULL;
	fichier = fopen(s, "r");



	if (fichier != NULL)
    {
    	FILE* fichierBIN = NULL;
		fichierBIN = fopen(s2, "wba");

		// EN-TÊTE DU FICHIER
		//char tailleINchar = (char)taille;
		//fwrite(tailleCHAR, sizeof(tailleCHAR), taille/10, fichierBIN);
		unsigned char bufferET[8];
		//fprintf(fichierBIN, "%d%ld", taille, taillemaxCB);
		fputc(taille,fichierBIN);
		fputc(tailleBIN,fichierBIN);
		unsigned int t = 0;
		printf("EN-TÊTE A ECRIRE\n");
		printf("taille:%d --------> %c\n", taille, taille);
		printf("tailleBIN:%d --------> %c\n", tailleBIN, tailleBIN);
		for (int i=0; i<tailleBIN; i++) {
			//fprintf(fichierBIN, "%c:%s", nom[i],bin[i]);
			for (int j=0; j<strlen(bin[i]); j++) {
				//printf("%ld\n", strlen(bin[i]));
				bufferET[j] = bin[i][j];
				//printf("%c\n", buffer[j]);
				t=j;
			}
			t++;
			while (t < 8) {
				bufferET[t] = '0';
				t++;
			}
			t = 0;

			unsigned int cpt = 0;
			for (int k=0; k<8; k++) {
				int tmp = myatoi(bufferET[k]);
				int power = pow(2,(7-k));
				cpt += tmp * power;
			}
			printf("nom:%c | longueur:%ld | buffer:%s --------> %d --------> %c\n", nom[i],strlen(bin[i]),bufferET, cpt, cpt);
			//fprintf(fichierBIN, "%d", cpt);
			fputc(nom[i],fichierBIN);
			fputc(strlen(bin[i]),fichierBIN);
			fputc(cpt,fichierBIN);
			for (int I=0; I<8; I++) {
				bufferET[I] = ' ';
			}

		}

		// RESTE DU FICHIER
		printf("\nRESTE DU FICHIER A ECRIRE\n");
		//unsigned char bufferRF[nbrNoeud];
    	int c;
    	unsigned int t2 = 0;
    	t=0;
    	while(EOF != (c = fgetc(fichier))){
    		char* cb;
            for (int j=0; j<nbrNoeud; j++) {
            	if (nom[j] == c) {
            		cb = bin[j];
            		t2++;
            		//printf("bin:%s\n", bin[j]);
            	}
            }
            for (int k=0; k<strlen(cb) || t2==taille; k++) { // récupération caractère par caractère du code binaire
            	//printf("t:%d\n", t);

            	if (t < 8) {
            		
            		//printf("taille%d / t2:%d / t:%d\n", taille, t2,t);
            		if (t2 == taille) {
            			bufferET[t] = '0';
            		}
            		else {

	            		bufferET[t] = cb[k];
	            		//printf("bf:%c\n", bufferRF[t]);
            		}
            		t++;
            	}
            	
          		
            	else { // SI BUFFER PLEIN

            		if (t>=8 && t2==taille) {
            			t2=0;
            		}
            		unsigned int cpt2 = 0;
					for (int l=0; l<8; l++) {
						int tmp2 = myatoi(bufferET[l]);
						int power2 = pow(2,(7-l));
						cpt2 += tmp2 * power2;
					}
					printf("%s --------> %d --------> %c\n", bufferET, cpt2, cpt2);
					fputc(cpt2,fichierBIN);
					t=0;
					//bufferET[++t] = cb[k];
            	}


            }
            

            //fwrite(cb, sizeof(cb), taillemaxCB, fichierBIN);
    	}
    	fclose(fichier);
    }
    else
    {
        exit(EXIT_FAILURE);    
    }
}

int main(int argc, char **argv) {
	if (argc != 3) {
		fprintf(stderr, "ERREUR: 1 argument attendu \n");
		return 1;
	}
	else {
		int a = calculNbOcc(argv[1]);
		int nbrF = nbrFeuille();
		nbrNoeud = ((2*nbrF)-1);
		printf("%d \n", nbrNoeud);
		fg = malloc(nbrNoeud*sizeof(int));
		fd = malloc(nbrNoeud*sizeof(int));
		pere = malloc(nbrNoeud*sizeof(int));
		occ = malloc(nbrNoeud*sizeof(int));
		rconv = malloc(nbrNoeud*sizeof(int));
		nom = malloc(nbrNoeud*sizeof(char));
		//bin = malloc(nbrNoeud*sizeof(char[nbrNoeud]));

		initNom();
		initArbre();
		afficheFIN(fg,fd,pere,occ,rconv,nom,nbrNoeud);
		printf("-----------------------------------------------\n");
		printf("nbrNoeud = %d\n", nbrNoeud);
		printf("nbrF = %d\n", nbrF);
		printf("taille = %d\n", taille);
		creationArbre(nbrF, nbrNoeud);
		afficheFIN(fg,fd,pere,occ,rconv,nom,nbrNoeud);
		printf("\n\n\n");

		for (int i=0; i<tailleBIN; i++) {
			printf("bin[%d]: %s\n",i,bin[i] );
		}
		printf("----------------- \n\n");
		creeBIN(nbrNoeud, nbrF);
		//binReduit();
		printf("----------------- \n\n");
		afficheBIN();
		printf("----------------- %s\n", bin[0]);
		printf("%d\n", tailleBIN);
		maxCB();
		printf("%ld\n", taillemaxCB);
		printf("%d\n", taille);
		printf("----------------- \n\n");
		for (int i=0; i<tailleBIN; i++) {
			printf("bin[%d]: %s\n",i,bin[i] );
		}
		printf("----------------- \n\n");
		compression(argv[1],argv[2]);

		return 0;
	}
}





