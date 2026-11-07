#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE (50)
#define MALLOC_ERROR (-1)
#define FILE_NOT_OPEN (-2)
#define BAD_FORMAT (1)
#define EMPTY_STACK (2)
#define SUCCESS (0)

struct stackElement;
typedef struct stackElement* Position;
typedef struct stackElement {
	double broj;
	Position next;
}StackElement;

Position createElement(double number);
int push(Position head, double number);
int pop(Position head, double* number);
int calculate(Position head, char operation);
int readFromFile(Position head, char* fileName);
int deleteAll(Position head);


int main() {

	StackElement head = { 0,NULL };
	double value = 0;
	int result = 0;

	result = readFromFile(&head, "postfix.txt");
	if (result != SUCCESS) {
		printf("greska kod racunanja izraza\n");
		deleteAll(&head);
		return result;
	}

	result = pop(&head, &value);
	if (result != SUCCESS) {
		printf("stog je prazan\n");
		deleteAll(&head);
		return result;
	}

	printf("rezultat = %g\n", value);

	deleteAll(&head);



	return 0;
}



Position createElement(double number) {

	Position newElement = NULL;

	newElement = (Position)malloc(sizeof(StackElement));

	if (newElement == NULL) {
		printf("greska pri alokaciji memorije\n");
		return NULL;
	}

	newElement->broj = number;
	newElement->next = NULL;

	return newElement;

}


int push(Position head, double number) {

	Position newElement = NULL;

	newElement = createElement(number);
	if (newElement == NULL) {
		return MALLOC_ERROR;
	}

	newElement->next = head->next;
	head->next = newElement;


	return 0;

}


int pop(Position head, double* number) {

	Position toDelete = NULL;

	if (head->next == NULL) {
		return EMPTY_STACK;
	}

	toDelete = head->next;
	*number = toDelete->broj;
	head->next = toDelete->next;
	free(toDelete);


	return 0;

}


int calculate(Position head, char operation) {

	double first = 0;
	double second = 0;

	if (pop(head, &second) != SUCCESS) {
		return EMPTY_STACK;
	}
	if (pop(head, &first) != SUCCESS) {
		return EMPTY_STACK;
	}

	switch (operation) {
	case '+':
		return push(head, first + second);
	case '-':
		return push(head, first - second);
	case '*':
		return push(head, first * second);
	case '/':
		return push(head, first / second);
	default:
		return BAD_FORMAT;
	}

}


int readFromFile(Position head, char* fileName) {

	FILE* fp = NULL;
	char word[MAX_LINE] = "";
	double number = 0;
	int result = 0;

	fp = fopen(fileName, "r");
	if (fp == NULL) {
		return FILE_NOT_OPEN;
	}

	while (fscanf(fp, "%s", word) == 1) {

		if (sscanf(word, "%lf", &number) == 1) {
			result = push(head, number);
		}
		else {
			result = calculate(head, word[0]);
		}

		if (result != SUCCESS) {
			fclose(fp);
			return result;
		}
	}

	fclose(fp);


	return 0;

}


int deleteAll(Position head) {

	Position toDelete = NULL;

	while (head->next != NULL) {
		toDelete = head->next;
		head->next = toDelete->next;
		free(toDelete);
	}



	return 0;
}