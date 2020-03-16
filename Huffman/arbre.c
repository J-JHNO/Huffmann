#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int taille = 0;
int taille2 = 0;;
int occTotal = 0;
int nbOcc[256];
char** C;
int* occ;
char** fg;
char** fd;
char** bin;



char* intTOcharV2(int i, char* s){
	if (i<0) {
		return NULL;
	}
	if (i<10){
		s[0] = i + '0';
		s[1] = '\0';
	}
	else {
		int l =strlen(intTOcharV2((i/10),s));
		char unite = i%10 + '0';
		s[l] = unite;
		s[l+1] = '\0';
	}
	return s;
}

int calculNbOcc(char* f) {
	//FILE* fichier = NULL;
	FILE* fichier = fopen(f, "r");
	if (fichier != NULL) {
		int c;
		while (EOF != (c = fgetc(fichier))) {
			taille++;
			nbOcc[c]++;
		}
		fclose(fichier);
		return 1;
	}
	return 0;
}

void parcours() {
	int j = 0;
	for (int i=0; i<256; i++) {
		char c;
		if (nbOcc[i] > 0) {
			occ[j] = nbOcc[i];
			j++;
			c= i;
			C[i] = i+'\0';
			printf("%s\n", C[i]);
			taille2++;
			occTotal += nbOcc[i];
		}
	}
}

void arbre() {
	int x = 0;
	int p = taille2;
	int* tmp = malloc(511*sizeof(int));
	for (int i = 0; i<p; i++) {
		tmp[i] = occ[i];
		fg[i] = "-1";
		fd[i] = "-1";
	}

	while (x != occTotal) {
		int pp = 0, pp2 = 0;
		int check=tmp[0], check2 = tmp[0];

		for (int j=0; j<511; j++) {
			if ((tmp[j] != -1) && (tmp[j] < check)) {
				check = tmp[j];
				pp = j;
				tmp[j] = -1;
			}
		}
		for (int k=0; k<511; k++) {
			if ((tmp[k] != -1) && (tmp[k] < check2) && (k != pp)) {
				check2 = tmp[k];
				pp2 = k;
				tmp[k] = -1;
			}
		}

		//tmp[p + 1] = strcat(C[pp],C[pp2]);
		tmp[p + 1] = check + check2;
		C[p + 1] = strcat(C[pp],C[pp2]);
		occ[p + 1] = check + check2;
		fg[p + 1] = C[pp];
		fd[p + 1] = C[pp2];
		taille2++;
		p++;
		x += check + check2;
	}

	free(tmp);
}

// void binaire(int n, char *code) {
// 	char* fin = C[taille2];

// 	// for (int i=0; i<511; i++) {
// 	// 	char* tmp, tmp2;
// 	// 	if (C[i] == fin) {
// 	// 		tmp = fg[i];
// 	// 		tmp2 = fd[i];
// 	// 	}
// 	// 	for (int j=0; j<511; j++) {
// 	// 		if (C[j] == tmp) {
				
// 	// 		}
// 	// 	}
// 	// }

	
// }

int main(int argc, char* argv[]) {

	C = malloc(511*sizeof(char));
	occ = malloc(511*sizeof(int));
	fg = malloc(511*sizeof(char));
	fd = malloc(511*sizeof(char));
	bin = malloc(511*sizeof(int));

	printf("%d\n", occ[0]);

	int res = calculNbOcc(argv[1]);
	for (int i=0; i<256; i++) {
		if (nbOcc[i] > 0) {
			printf("Le caractère %c a %d occurrences \n", i, nbOcc[i]);
		}
	}

	parcours(taille, occTotal, taille2, nbOcc, occ, C);
	printf("Deubg\n");

	printf("%d\n", occ[12]);
	printf("%s\n", C[0]);

	//arbre(fg, fd, C, occ, taille2, occTotal);

	// printf("ARBRE :\n");
	// for (int i=0; i<511; i++) {
	// 	printf("%s --> %d / fg: %s / fd: %s \n", C[i], occ[i], fg[i], fd[i]);
	// }

	return 0;
}