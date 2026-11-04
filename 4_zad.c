#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE (1024)
#define MALLOC_ERROR (-1)
#define FILE_NOT_OPEN (-2)
#define BAD_FORMAT (1)
#define SUCCESS (0)

struct term;
typedef struct term* Position;
typedef struct term {
	int koeficijent;
	int eksponent;
	Position next;
}Term;

Position createTerm(int coef, int exp);
int insertSorted(Position head, Position newTerm);
int deleteAfter(Position previous);
int readFromFile(Position head1, Position head2, char* fileName);
int readLine(Position head, char* buffer);
int printPoly(Position first);
int addPoly(Position headResult, Position first1, Position first2);
int multiplyPoly(Position headResult, Position first1, Position first2);
int deleteAll(Position head);


int main() {

	Term head1 = { 0,0,NULL };
	Term head2 = { 0,0,NULL };
	Term headSum = { 0,0,NULL };
	Term headProduct = { 0,0,NULL };
	int result = 0;

	result = readFromFile(&head1, &head2, "polinomi.txt");
	if (result != SUCCESS) {
		printf("greska kod citanja datoteke\n");
		deleteAll(&head1);
		deleteAll(&head2);
		return result;
	}

	printf("P1 = ");
	printPoly(head1.next);
	printf("P2 = ");
	printPoly(head2.next);



	result = addPoly(&headSum, head1.next, head2.next);
	if (result != SUCCESS) {
		printf("greska kod zbrajanja\n");
	}
	else {
		printf("\nP1 + P2 = ");
		printPoly(headSum.next);
	}



	result = multiplyPoly(&headProduct, head1.next, head2.next);
	if (result != SUCCESS) {
		printf("greska kod mnozenja\n");
	}
	else {
		printf("P1 * P2 = ");
		printPoly(headProduct.next);
	}



	deleteAll(&head1);//sve 4 liste oslobodit
	deleteAll(&head2);
	deleteAll(&headSum);
	deleteAll(&headProduct);



	return 0;
}



Position createTerm(int coef, int exp) {

	Position newTerm = NULL;

	newTerm = (Position)malloc(sizeof(Term));

	if (newTerm == NULL) {
		printf("greska pri alokaciji memorije\n");
		return NULL;
	}

	newTerm->koeficijent = coef;
	newTerm->eksponent = exp;
	newTerm->next = NULL;

	return newTerm;

}


int insertSorted(Position head, Position newTerm) {

	Position current = NULL;
	current = head;

	if (newTerm->koeficijent == 0) {//0*x^n je samo 0, nema smisla dodavat
		free(newTerm);
		return 0;
	}

	while (current->next != NULL && current->next->eksponent > newTerm->eksponent) {//sortiram od najveceg eksponenta prema manjem
		current = current->next;
	}

	if (current->next != NULL && current->next->eksponent == newTerm->eksponent) {//isti eksponent vec postoji, samo zbrojim koeficijente
		current->next->koeficijent += newTerm->koeficijent;
		free(newTerm);//ne treba mi vise

		if (current->next->koeficijent == 0) {//npr 3x^2 i -3x^2 se ponistu
			deleteAfter(current);
		}
	}
	else {
		newTerm->next = current->next;//ko addAfter
		current->next = newTerm;
	}


	return 0;

}


int deleteAfter(Position previous) {

	Position toDelete = NULL;

	if (previous == NULL || previous->next == NULL) {
		return 1;
	}

	toDelete = previous->next;
	previous->next = toDelete->next;
	free(toDelete);

	return 0;

}


int readFromFile(Position head1, Position head2, char* fileName) {

	FILE* fp = NULL;
	char buffer[MAX_LINE] = "";
	int result = 0;

	fp = fopen(fileName, "r");
	if (fp == NULL) {
		return FILE_NOT_OPEN;
	}

	if (fgets(buffer, MAX_LINE, fp) == NULL) {//prvi red je prvi polinom
		fclose(fp);
		return BAD_FORMAT;
	}
	result = readLine(head1, buffer);
	if (result != SUCCESS) {
		fclose(fp);//zatvorit i kad je greska
		return result;
	}

	if (fgets(buffer, MAX_LINE, fp) == NULL) {//drugi red je drugi polinom
		fclose(fp);
		return BAD_FORMAT;
	}
	result = readLine(head2, buffer);

	fclose(fp);


	return result;

}


int readLine(Position head, char* buffer) {

	char* current = buffer;
	int coef = 0;
	int exp = 0;
	int n = 0;
	int result = 0;
	Position newTerm = NULL;

	while (strlen(current) > 0) {

		result = sscanf(current, " %d %d %n", &coef, &exp, &n);//%n mi kaze koliko je znakova procita

		if (result == EOF) {//ostali samo razmaci i \n, gotov red
			break;
		}
		if (result != 2) {
			printf("krivi format u datoteci\n");
			return BAD_FORMAT;
		}

		newTerm = createTerm(coef, exp);
		if (newTerm == NULL) {
			return MALLOC_ERROR;
		}
		insertSorted(head, newTerm);//odma se sortira pa nije bitno kojim su redom u datoteci

		current += n;//pomaknem se za tolko znakova na iduci par
	}


	return 0;

}


int printPoly(Position first) {

	Position current = first;

	if (current == NULL) {//sve se ponistilo
		printf("0\n");
		return 0;
	}

	while (current != NULL) {

		if (current == first) {//prvi clan bez + ispred
			if (current->koeficijent < 0) {
				printf("-");
			}
		}
		else if (current->koeficijent < 0) {
			printf(" - ");
		}
		else {
			printf(" + ");
		}

		if (current->eksponent < 0) {//negativni eksponent u zagradu da se lipse vidi
			printf("%dx^(%d)", abs(current->koeficijent), current->eksponent);
		}
		else {
			printf("%dx^%d", abs(current->koeficijent), current->eksponent);
		}

		current = current->next;
	}
	printf("\n");


	return 0;

}


int addPoly(Position headResult, Position first1, Position first2) {

	Position current = NULL;
	Position newTerm = NULL;

	for (current = first1; current != NULL; current = current->next) {//prepisem sve iz prvog
		newTerm = createTerm(current->koeficijent, current->eksponent);
		if (newTerm == NULL) {
			return MALLOC_ERROR;
		}
		insertSorted(headResult, newTerm);
	}

	for (current = first2; current != NULL; current = current->next) {//i sve iz drugog, insertSorted sam zbroji iste eksponente
		newTerm = createTerm(current->koeficijent, current->eksponent);
		if (newTerm == NULL) {
			return MALLOC_ERROR;
		}
		insertSorted(headResult, newTerm);
	}


	return 0;

}


int multiplyPoly(Position headResult, Position first1, Position first2) {

	Position current1 = NULL;
	Position current2 = NULL;
	Position newTerm = NULL;

	for (current1 = first1; current1 != NULL; current1 = current1->next) {
		for (current2 = first2; current2 != NULL; current2 = current2->next) {//svaki sa svakim

			newTerm = createTerm(current1->koeficijent * current2->koeficijent,
				current1->eksponent + current2->eksponent);//koeficijenti se mnoze, eksponenti zbrajaju
			if (newTerm == NULL) {
				return MALLOC_ERROR;
			}
			insertSorted(headResult, newTerm);
		}
	}


	return 0;

}


int deleteAll(Position head) {

	Position toDelete = NULL;

	while (head->next != NULL) {//stalno brisemo prvog s liste
		toDelete = head->next;
		head->next = toDelete->next;
		free(toDelete);

	}



	return 0;
}
