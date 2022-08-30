// create a hashtable (dictionary) in c

// using a user entered key: int
// and using a user enetered string: char[]

// maybe use linear probing?

/*

cd C:\Users\kvsha\Documents\Code\C\HashTable
gcc -Wall -Wextra -g HashTable.c -o hashtable
hashtable

*/


#include <stdio.h>
#include <stdbool.h> // for using a boolean array isActiveArray()
#include <stdlib.h>


#include "functions.h"


int main(void) {
	printf("Welcome to HashTable!\n");


	int size = 5;
	table myTable;
	table* p = &myTable; 

	initTable(p, size, LOAD_FACTOR, true);

	int input;
	char term;
	while (true) {
		printMenu();

		if (scanf("%i%c", &input, &term) != 2 || term != '\n') {
			errorMessage();
			continue;
		}

		if (input == -1) break; // exit program

		else if (input == 0) { // get a key/value pair
			// ask user to enter the key to find
			int key;

			printf("Enter a key: ");
			scanf("%i", &key);

			char* value = findValue(myTable, key);
			printf("value = %s\n", value);
		}

		else if (input == 1) { // add a key/value pair
			int key;
			char* value = malloc(101*sizeof(char)); // max length = 100
			
			printf("Enter key to get: \n");
			scanf("%d", &key);

			printf("Enter value (max 100 chars)\n");
			scanf("%s", value);

			put(p, myTable, key, value);

			free(value);

		}

		else if (input == 2) { // remove a key/value pair
			// when removing a pair, just set the isActiveArray
			// position to two

			int key;
			printf("Enter a key to delele: ");
			scanf("%i", &key);

			char* value = removeKey(p, key);

			printf("key/value pair removed: %i, %s\n", key, value);
		}

		else {
			printf("Number not in range of instructions! (-1: 2)\n");
		}

	}

	// free all the pointer arrays designated in table* p
	free(p->keyArray);
	free(p->valueArray);
	free(p->isActiveArray);

	return 0;

}

