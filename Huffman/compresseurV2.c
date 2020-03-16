#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef EOF
#define EOF (-1)
#endif

int nbOcc[256]; //  tableau nbr occurences
int taille = 0;	// taille du fichier
int tailleBIN = 0;	// taille du tableau des codes binaires
int nbrNoeud = 0;	// nbr de noeud
int tailleFin = 2; // taille du fichie compréssé
int T=0;
long unsigned int taillemaxCB = 0; // taille maximal des codes binaires
int conv[256]; // tableau pour retrouvé les caractères 
int *fg; // tableau des fils gauche
int *fd; // tableau des fils droits
int *pere; // tableau des pères
int *occ; // tableau des occurences
char *nom;	// tableau des caractères
char* bin[511]; // tableau des codes binaires

// Change le caractère '0' ou '1' en nombre 0 ou 1
float myatoi(char c) {
	if (c == '0') return 0.0;
	else /*(c == '1')*/ return 1.0;
}

//Calcul le nombre d'occurrences de chaque caractère présent dans le fichier
int calculNbOcc(char* s){
	FILE* fichier = NULL;
	fichier = fopen(s, "r"); //ouverture du fichier en lecture

	if (fichier != NULL)
    {
    	int c;
    	while(EOF != (c = fgetc(fichier))){ // parcours du fichier et récupération de caractère un par un
            //if (nbOcc[c])
			taille++;
			nbOcc[c]++;
    	}
    	fclose(fichier); // fermeture du fichier
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
			nom[j] = i;
			fg[j] = -1; // valeur par défaut
			fd[j] = -1; // valeur par défaut
			pere[j] = -1;  // valeur par défaut
			occ[j] = nbOcc[i];
			bin[j] = "/ /"; // valeur par défaut
			j++;
			tailleBIN++;
		}
	}
}

// affiche le recapitulatif avec pour chaque "noeud" son fils gauche, fils droit ect..
void afficheFIN(int* T1, int* T2, int* T3, int* T4, char* T5, int nbrF) {
	printf("    fg     fd     pere     occ         nom\n");

	for (int i=0; i<nbrF; i++) {
		printf("%d: %d       %d       %d       %d       '%c'\n",i, T1[i], T2[i], T3[i], T4[i], T5[i]);
	}

}

// Crée l'arbre en manipulant fg,fd,pere et occ
void creationArbre(int nbF, int nbrNoeud) {
	for (int nn=nbF; nn<nbrNoeud; nn++) {
		int min1=taille+1, min2=taille+1; // initialisation des valeurs min
		int imin1=taille,imin2=taille;

		// Recherche des 2 plus petites occurences
		for (int i=0; (i<nn); i++) {
			if (pere[i] == -1) { // pour éviter de réutiliser un noeud
				if (occ[i] < min1) { // occurence du caractère à la ième position plus petit que le premier min actuel?
					imin2 = imin1;
					min2 = min1;
					imin1 = i;
					min1 = occ[i];
				}
				else {
					if ((occ[i] < min2) && (pere[i] == -1)) { // occurence du caractère à la ième position plus petit que le second min actuel?
						imin2 = i;
						min2 = occ[i];
					}
				}
			}
		}
		pere[imin1] = nn; // affectation du père du premier fils
		fg[nn] = imin1; // affectation du fils gauche
		pere[imin2] = nn; // affectation du père du second fils
		fd[nn] = imin2; // affectation du fils droit
		occ[nn] = occ[imin1] + occ[imin2]; // affectation de l'occurence du nouveau noeud
		pere[nn] = -1; // le nouveau noeud n'a pas encore de père
	}
}
	
// Créer le code binaire (en chaine de caractère) de chaque caractère
void parcours(int n, char *code) {
	if (fg[n] == -1 /* Feuille? */) {
		char* c = NULL;
		c = malloc(256*sizeof(char)); // création chaine de caractère local
		if (tailleBIN == 1) { // Cas d'exception si le fichier à compresser n'a qu'un seul caractère distinct
			strcpy(c,"0"); 
			bin[n] = c; // affectation du code binaire O 
		}
		else {
			strcpy(c,code); 
			bin[n] = c; // affectation du code binaire créer en récursif
		}
		//printf("%c : %s \n",nom[n], bin[n]); // Ex: a:0111
	}
	else {
		char *s= malloc(strlen(code)+2); // place pour le code actuel + le nouveau caractère + le \0
		strcpy(s,code);
		strcat(s, "0"); // 0 pour le fils gauche
		parcours(fg[n],s); // Recursif fils gauche
		strcat((strcpy(s,code)), "1"); // 1 pour le fils droit
		parcours(fd[n],s); // Recursif fils droit
		
	}
	
}

// Creer le tableau des codes binaires 
void creeBIN(int nbrNoeud) {
	parcours(nbrNoeud-1, ""); // appel récursif avec la racine
}

// Affiche les caractères avec leurs code binaire
void afficheBIN() {
	for (int i=0; i<tailleBIN; i++) {
		printf("'%c' : %s \n", nom[i], bin[i]);
	}
}

// Calcule la longueur du code binaire le plus long
void maxCB() {
	for (int i=0; i<tailleBIN; i++) {
		if (strlen(bin[i]) > taillemaxCB) {
			taillemaxCB = strlen(bin[i]);
		}
	}
}

// Effectue la compression
void compression(char* s, char* s2) {
	FILE* fichier = NULL;
	fichier = fopen(s, "r"); // ouverture du fichier en lecture



	if (fichier != NULL)
    {
    	if (taille == 514) { // Cas d'exception pour un fichier qui contient un caractère de chaque 
    		FILE* fichierBIN = NULL;
			fichierBIN = fopen(s2, "wba"); // ouverture du fichier en écriture binaire ajouter
			unsigned char bufferET[8];
			// fputc(taille,fichierBIN);
			// fputc(tailleBIN,fichierBIN);
			unsigned int t = 0; // indice du dernier caractère insérer dans le buffer
			printf("EN-TÊTE A ECRIRE\n\n");
			printf("taille:%d --------> '%c'\n", taille, taille);
			printf("tailleBIN:%d --------> '%c'\n\n", tailleBIN, tailleBIN);
    		
    		int c; // dernier caractère pris dans le fichier
	    	unsigned int t2 = 0; // compteur de caractère du fichier
    		while(EOF != (c = fgetc(fichier))){
    			char* cb; // chaine de caractère pour le code binaire
	            for (int j=0; j<tailleBIN; j++) { // parcours du tableau de code binaire
	            	if (nom[j] == c) { // recherche du code binaire du caractère c
	            		cb = bin[j];
	            		t2++; // incrémentation du nbr de caractère présent dans le fichier
	            	}
	            }
	            for (int k=0; k<strlen(cb) || t2==taille; k++) { // récupération caractère par caractère du code binaire

	            	if (t < 8) { // buffer plein ?
	            		if (t2 == taille) { // dernier caractère atteint ?
	            			if ((k+1) != strlen(cb)) {
	            				bufferET[t] = '0'; // remplissage du buffer avec des 0
	            			}
	            		}
	            		else {

		            		bufferET[t] = cb[k]; // affectation à la case t du buffer de la case k du code binaire du caractère c
	            		}
	            		t++;
	            	}
	            	
	          		
	            	else { // SI BUFFER PLEIN

	            		if (t>=8 && t2==taille) {
	            			t2=0; // pour éviter d'avoir une boucle infinie 
	            		}
	            		unsigned int cpt2 = 0; // transformation du code binaire en décimal
						for (int l=0; l<8; l++) {
							int tmp2 = myatoi(bufferET[l]);
							int power2 = pow(2,(7-l));
							cpt2 += tmp2 * power2;
						}
						// printf("%s --------> %d --------> %c\n", bufferET, cpt2, cpt2);
						fputc(cpt2,fichierBIN); // insère dans le fichier le caractère correspondant à cpt2
						tailleFin++; // incrémentation taille final du fichier compréssé
						t=0;
						bufferET[t++] = cb[k]; // si le buffer est plein mais le code binaire n'est pas inséré totalement
	            	}
	            }
    		}
    	}
    	else { // CAS GENERAL
	    	FILE* fichierBIN = NULL;
			fichierBIN = fopen(s2, "wba");
			unsigned char bufferET[8];
			fputc(taille,fichierBIN);
			fputc(tailleBIN,fichierBIN);
			unsigned int t = 0;
			printf("EN-TÊTE A ECRIRE\n\n");
			printf("taille:%d --------> '%c'\n", taille, taille);
			printf("tailleBIN:%d --------> '%c'\n\n", tailleBIN, tailleBIN);
			// COMPRESSION EN-TÊTE
			for (int i=0; i<tailleBIN; i++) {
				for (int j=0; j<strlen(bin[i]); j++) {
					bufferET[j] = bin[i][j];
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
				// printf("nom:%c | longueur:%ld | buffer:%s --------> %d --------> %c\n", nom[i],strlen(bin[i]),bufferET, cpt, cpt);
				fputc(nom[i],fichierBIN);
				fputc(strlen(bin[i]),fichierBIN);
				fputc(cpt,fichierBIN);
				tailleFin += 3;
				for (int I=0; I<8; I++) {
					bufferET[I] = ' ';
				}

			}

			// COMPRESSION RESTE DU FICHIER
			//printf("\nRESTE DU FICHIER A ECRIRE\n\n");
	    	int c;
	    	unsigned int t2 = 0;
	    	t=0;
	    	while(EOF != (c = fgetc(fichier))){
	    		char* cb;
	            for (int j=0; j<tailleBIN; j++) {
	            	if (nom[j] == c) {
	            		cb = bin[j];
	            		t2++;
	            	}
	            }
	            for (int k=0; k<strlen(cb) || t2==taille; k++) { // récupération caractère par caractère du code binaire

	            	if (t < 8) {
	            		if (t2 == taille) {
	            			if ((k+1) != strlen(cb)) {
	            				bufferET[t] = '0';
	            			}
	            		}
	            		else {

		            		bufferET[t] = cb[k];
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
						// printf("%s --------> %d --------> %c\n", bufferET, cpt2, cpt2);
						fputc(cpt2,fichierBIN);
						tailleFin++;
						t=0;
						bufferET[t++] = cb[k];
	            	}
	            }
    		}
    	}
    	int gain = 100-((tailleFin*100)/taille);
    	printf("---------------------------\n");
    	printf("\n");
    	printf("Taille début: %d | Taille finale: %d\n", taille, tailleFin);
    	printf("Gain de %d%c après compression\n", gain,'%');
    	fclose(fichier);
    }
    else
    {
        exit(EXIT_FAILURE); // si le fichier ne s'est pas bien ouvert
    }
}

int main(int argc, char **argv) {
	if (argc != 3) { // vérification présence executable source destination
		fprintf(stderr, "ERREUR: 2 arguments attendus \n");
		return 1;
	}
	else { 
		int a = calculNbOcc(argv[1]); // juste pour appeler la fonction
		a++;
		int nbrF = nbrFeuille(); 
		nbrNoeud = ((2*nbrF)-1); // calcul du nombre de noeud
		fg = malloc(nbrNoeud*sizeof(int)); 
		fd = malloc(nbrNoeud*sizeof(int)); 
		pere = malloc(nbrNoeud*sizeof(int));
		occ = malloc(nbrNoeud*sizeof(int));
		nom = malloc(nbrNoeud*sizeof(char));

		initNom(); // initialisation du tableau de nom
		initArbre(); // initialisation de l'arbre
		//afficheFIN(fg,fd,pere,occ,nom,nbrNoeud);
		printf("-----------------------------------------------\n\n");
		printf("nbrNoeud = %d\n", nbrNoeud);
		printf("nbrF = %d\n", nbrF);
		printf("taille = %d\n\n", taille);
		printf("-----------------------------------------------\n\n");
		creationArbre(nbrF, nbrNoeud); // création de l'arbre
		printf("Arbre d'Huffman :\n\n");
		afficheFIN(fg,fd,pere,occ,nom,nbrNoeud); // affichage de l'arbre
		printf("\n\n\n");




		//printf("----------------- \n\n");
		creeBIN(nbrNoeud); // création des codes binaires
		//printf("----------------- \\n");
		printf("Les lettres et leur code dans l'ordre alphabétique :\n\n");
		afficheBIN(); // affichage des codes binaires
		printf("----------------- \n\n");
		printf("Nbr caractères différents: %d\n\n", tailleBIN);


		// CALCUL DE LA LONGUEUR MOYENNE DES CODES BINAIRES
		double moy = 0;
		for (int i=0; i<tailleBIN; i++) {
			moy = moy + strlen(bin[i]);
		}
		moy = moy / tailleBIN;
		printf("La longueur moyenne des codes binaires est : %f\n\n", moy);

		// On calcul la probabilité d'apparation de chaque caractère
		float proba = 0.0;
		for (int K=0; K<tailleBIN; K++) {
			float f = occ[K];
			proba = f/taille;
			printf("La probabilité d'apparition de '%c' est %f\n", nom[K], proba);
		}
		printf("\n\n");
		maxCB(); // longueur maximal des codes binaires
		printf("Taille max d'un code binaire: %ld\n\n", taillemaxCB);
		printf("----------------- \n\n");
		compression(argv[1],argv[2]); // appel 
		printf("----------------- \n\n");

		// libération de l'espace mémoire
		free(fg);
		free(fd);
		free(pere);
		free(occ);
		free(nom);

		return 0; // FIIIIIIIINNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
	}
}