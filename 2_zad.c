#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE (50)
#define MALLOC_ERROR (-1)
#define PERSON_NOT_FIND (1)
#define SUCCESS (0)

struct person;
typedef struct person* Position;
typedef struct person {
	char ime[MAX_LINE];
	char prezime[MAX_LINE];
	int godina;
	Position next;
}Person;

Position createPerson(char* name, char* surname, int year);
int addToBeginning(Position head, char* name, char* surname, int year);
int printList(Position first);
int addToEnd(Position head, char* name, char* surname, int year);
Position findSurname(Position head, char* surname);
Position findPrevious(Position head, char* surname);
int deleteSurname(Position head, char* surname);
int deleteAll(Position head);


int main() {

	Person head = { "","",0,NULL };
	int result = 0;
	Position foundPerson = NULL;

	result = addToBeginning(&head, "Ana", "Anic", 2006);
	if (result != SUCCESS) return result;
	result = addToBeginning(&head, "Marko", "Markic", 1999);
	if (result != SUCCESS) return result;


	result = addToEnd(&head, "Ivo", "Ivic", 1999);
	if (result != SUCCESS) return result;



	printf("\nlista nakon dodavanja\n");
	printList(head.next);//adresa



	foundPerson = findSurname(&head, "Ivic");
	if (foundPerson != NULL)
		printf("\nPronadeno: %s %s, %d\n",
			foundPerson->ime, foundPerson->prezime, foundPerson->godina);
	else
		printf("\nOsoba nije pronadena.\n");


	result = deleteSurname(&head, "Anic");
	if (result == PERSON_NOT_FIND) {
		printf("Nema osobe za brisanje.\n");
	}

	printf("\nlista nakon brisanja\n");
	printList(head.next);//adresa

	deleteAll(&head);//nemoj zaboravit



	return 0;
}

















Position createPerson(char* name, char* surname, int year) {

	Position newPerson=NULL;
	
	newPerson = (Position)malloc(sizeof(Person));

	if (newPerson == NULL) {
		printf("greska pri alokaciji memorije\n");
		return MALLOC_ERROR;
	}

	strcpy(newPerson->ime, name); //mora strcpy za string, ne more pridruzivanje s = ko za int
	strcpy(newPerson->prezime, surname);
	newPerson->godina = year;
	newPerson->next = NULL;

	return newPerson;

}


int addToBeginning(Position head, char* name, char* surname, int year) {

	Position newPerson = NULL;

	newPerson = createPerson(name, surname, year);
	if (newPerson == NULL) {
		printf("greska pri alokaciji memorije\n");
		return MALLOC_ERROR;
	}

	newPerson->next = head->next;
	head->next = newPerson;


	return 0;

}


int printList(Position first) {
	Position current = first;

	if (current == NULL) {
		printf("lista je prazna\n");
		return 0;
	}

	while (current != NULL) {
		printf("%s %s %d\n", current->ime, current->prezime, current->godina);
		current = current->next;
	}

	return 0;

}


int addToEnd(Position head, char* name, char* surname, int year) {

	Position newPerson = NULL;

	newPerson = createPerson(name, surname, year);
	if (newPerson == NULL) {
		printf("greska pri alokaciji memorije\n");
		return MALLOC_ERROR;
	}

	Position current = NULL;
	current = head;

	while (current->next != NULL) {
		current = current->next;
	}

	newPerson->next = current->next;
	current->next = newPerson;


	return 0;

}

Position findSurname(Position head, char* surname) {
	Position current = NULL;
	current = head->next;

	while (current != NULL && strcmp(current->prezime, surname)!=0) {
		current = current->next;
	}

	return current;

}

Position findPrevious(Position head, char* surname) {

	Position previous = NULL;
	previous = head;

	while (previous->next != NULL && strcmp(previous->next->prezime, surname) != 0) {
		previous = previous->next;
	}

	if (previous->next == NULL) { //pazi na ovaj dio, da ga nema funkcija ce vratit zadnjeg clana a ne NULL
		return NULL;
	}

	return previous;

	
}

int deleteSurname(Position head, char* surname) {
	Position previous = NULL;
	Position toDelete = NULL;

	previous = findPrevious(head, surname);
	if (previous == NULL) {
		return PERSON_NOT_FIND;
	}

	toDelete = previous->next;//lagano
	previous->next = toDelete->next;
	free(toDelete);

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


