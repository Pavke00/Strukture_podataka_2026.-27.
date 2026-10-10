#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_IME 50
#define MAX_LINIJA 250

typedef struct {
	char ime[MAX_IME];
	char prezime[MAX_IME];
	int bodovi;
}Student;

int brojStudenata(FILE* fp) { //nacin na koji funkcija prima datoteku

	int brojac = 0;
	char buffer[MAX_LINIJA];

	while (fgets(buffer, sizeof(buffer), fp)!=NULL) { //do kraja uzimaj svaki red
		if (buffer[0] != '\n') { //preskacem prazan red tako da provjerim prvi znak u redu
			brojac++;
		}
	}
	return brojac;
}

int ucitajStudente(FILE* fp, Student* studenti, int n) {
	int i = 0; 

	while (i < n && fscanf(fp, "%49s %49s %d", //idi dok se uspjesno uzimaju svi podaci
		studenti[i].ime, studenti[i].prezime, &studenti[i].bodovi) == 3) {
		i++;
	}
	return i;
}

int maxBodovi(Student* studenti, int n) { 
	int i = 0;
	int max = studenti[i].bodovi;

	for (i = 1;i < n;i++) {
		if (max < studenti[i].bodovi) {
			max = studenti[i].bodovi;
		}
	}

	return max;
}

int ispisiStudente(Student *studenti, int n, int max) {
	int i;
	float relativni;

	for(i=0;i<n;i++) {
		relativni = (float)studenti[i].bodovi / max * 100;
		printf("%s %s %d %f\n",
			studenti[i].ime, studenti[i].prezime, studenti[i].bodovi, relativni);
	}


	return n;
}

int main() {

	FILE* fp = NULL; //postavim pokazivace na 0
	Student* studenti = NULL;
	int n = 0;

	fp = fopen("studenti.txt", "r");
	if (!fp) {
		printf("greska pri otvaranju datoteke");
		return 1;
	}
	
	n = brojStudenata(fp);//sad u n spremim broj studenata (redova)
	if (n == 0) {
		printf("datoteka je prazna\n");
		fclose(fp);
		return 0;
	}
	rewind(fp);//fgets dosa na kraj mape, vrati malog nazad odakle je i dosa

	studenti = (Student*)malloc(n * sizeof(Student));//alociram memoriju za studente koje cu ucitat
	if (!studenti) {
		printf("greska pri alokaciji memorije");
		fclose(fp);
		return 1;
	}

	n=ucitajStudente(fp, studenti, n);//sad je n onaj iz ucitaj studente, ako je svaki zapis bio ispravan
	//onda ce n iz brojStudenata i ucitajStudente bit isti

	int max = maxBodovi(studenti, n);

	ispisiStudente(studenti, n, max);

	fclose(fp);//ovo ne smis zaboravit
	free(studenti);//ovo isto



	return 0;
}
