#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string.h> // for memset, memcpy
//#include <assert.h> // for memset
#include "hashtable.h"
#include <stdbool.h>


// for printing
void printMenu();
void errorMessage();

// use myTable for getters
int getSize(table myTable);
int getCapacity(table myTable);
double getLoadFactor(table myTable);
double calcLoadFactor(table myTable);
char* findValue(table myTable, int key);

// to use in put()
int getIndex(table* p, int key);

// use *p for setters, to directly modify memory address
void initTable(table* p, int size, double loadFactor, bool collisionHandler);
void put(table* p, table myTable, int key, char* value);
char* removeKey(table* p, int key);
void resizeArray(table* p, int old_size);

// for collisions
int resolveCollision(table* p, int index);
int linearProbe(table* p, int index);
int quadraticProbe(table* p, int index);

//-----------------------------------------------------------
//Define all the functions' bodies



void errorMessage() {
	// print error message for instruction and clear input buffer
	printf("Invalid input...\n");
	fflush(stdin);
}

void printMenu() {
	printf("-1: Exit\n");
	printf("0: Get a Value\n");
	printf("1: Add Key/Value Pair\n");
	printf("2: Remove Key/Value Pair\n");
}

void initTable(table* p, int size, double loadFactor, bool collisionHandler) {
	// constructor
	// https://stackoverflow.com/questions/73396386/how-to-make-a-2d-char-array-with-nulls-in-c-with-a-struct-error-expected/73396666#73396666
	// (Andreas Wenzel)
	p->count = 0;
	p->capacity = size;
	p->loadFactor = loadFactor; 
	p->collisionHandler = collisionHandler;

	p->keyArray = malloc(p->capacity * sizeof(int));
	if (p->keyArray == NULL) { // protect malloc
		exit(1);
	}
	memset(p->keyArray, -1, p->capacity * sizeof(int));

	p->valueArray = malloc(p->capacity * 101 * sizeof(char));
	if (p->valueArray == NULL) { // protect malloc
		exit(2);
	}
	memset(p->valueArray, 0, p->capacity * 101);
    
    p->isActiveArray = malloc(p->capacity * sizeof(int));
    if (p->isActiveArray == NULL) { //protect malloc
    	exit(3);
    }
	memset(p->isActiveArray, 0, p->capacity);   
	
} 

void resizeArray(table* p, int old_size) {
	// create a new table, and for each key, add it to new array
	// https://stackoverflow.com/questions/73452047/how-to-make-a-custom-size-array-in-a-struct-in-c/73452977#73452977
	int** keys = &p->keyArray;
	if (!keys) exit(1);

	char** values = &p->valueArray;
	if (!values) exit(2);

	bool** actives = &p->isActiveArray;
	if (!actives) exit(3);

	int* new_keys = malloc(2 * old_size * sizeof(int));
	if (!new_keys) exit(4);
	memset(new_keys, -1, 2*old_size*sizeof(int)); // set all elems to -1

	char* new_values = malloc(101 * 2 * old_size * sizeof(char));
	if (!new_values) exit(5); 
	memset(new_values, 0, 2*101*old_size); // set all chars to '\0'

	bool* new_actives = malloc(2 * old_size * sizeof(int));
	if(!new_actives) exit(6);
	memset(new_actives, 0, 2 * old_size); // set all elems to false

	memcpy(new_keys, *keys, old_size * sizeof(int)); // <- changed here
	memcpy(new_values, *values, 101*old_size);
	memcpy(new_actives, *actives, old_size);

	free(*keys);
	free(*values);
	free(*actives);

	*keys = new_keys;
	*values = new_values;
	*actives = new_actives;

	p->capacity *= 2;

}

void put(table* p, table myTable, int key, char* value) {
	// Implement putting a key/value pair into the table

	double currentLoad = calcLoadFactor(myTable);
	if (currentLoad > myTable.loadFactor) {
		printf("resize called");
		resizeArray(p, p->capacity);
	}

	int collisionIndex = getIndex(p, key);
	if (collisionIndex == -1) {
		collisionIndex = resolveCollision(p, key % p->capacity);
	}
	p->keyArray[collisionIndex] = key;

	// copy every character over to valueArray one by one
	for (unsigned int i = 0; i < strlen(value); ++i) {
		p->valueArray[101*collisionIndex + i] = value[i];
	}
	p->isActiveArray[collisionIndex] = true;
	p->count++;
}

char* removeKey(table* p, int key) {
	if (key < 0) return 0;
	int index = key % p->capacity;
	while (p->keyArray[index] != -1) {
		if (p->keyArray[index] == key && p->isActiveArray[index]) {
			// create a seperate reference to return value
			// so that nothing gets accidentally changed
			char* value = &p->valueArray[101*index];
			p->isActiveArray[index] = false;
			p->count--;
			return value;
		}
		index = (index + 1) % p->capacity;
		if (index == key) {
			return 0;
		}
	}
	return 0;
}

char* findValue(table myTable, int key) {
	// in case of no find, returning zero is the 
	// same as returning null, and zero is easier
	// to check of char* == NULL
	if (key < 0) return 0;
	int i = key % myTable.capacity;
	while (myTable.keyArray[i] != 0) {
		if (myTable.keyArray[i] == key && myTable.isActiveArray[i] == true) {
			// to avoid 'function returns address of local variable'
			// in which the return value memory location is not guaranteed
			char* value = &myTable.valueArray[101*i];
			return value;
		}
		i = (i + 1) % myTable.capacity;
		if (i == key % myTable.capacity) {
			return 0;
		}
	}
	return 0;
}

int getIndex(table* p, int key) {
	int index = key % p->capacity;
	if (index < 0) return -1;
	int i = index;
	while (p->keyArray[i] == -1) {
		if (p->keyArray[i] == index) {
			return i;
		}
		i = (i + 1) % p->capacity;
		if (i == index) {
			return -1;
		}
	}
	return -1;
}

int resolveCollision(table* p, int index) {
	// using linear or quadratic probing
	if (p->collisionHandler) { // linear probing
		return linearProbe(p, index);
	}
	else { // quadratic probing
		return quadraticProbe(p, index);
	}
}

int linearProbe(table* p, int index) {
	while (p->isActiveArray[index]) {
		index = (index + 1) % p->capacity;
	}
	return index;
}

int quadraticProbe(table* p, int index) {
	int curIndex = index % p->capacity;
	int i = 1;
	while (p->isActiveArray[curIndex]) {
		curIndex = (curIndex + i + i*i) % p->capacity;
		i+=1;
	}
	return curIndex;
}

double calcLoadFactor(table myTable) {
	// calculate the load factor for resizing
	return (double)getSize(myTable) / (double)myTable.capacity;
}

/*
--------------------------
Getter methods for myTable
--------------------------
*/

int getSize(table myTable) {
	return myTable.count;
}

double getLoadFactor(table myTable) {
	return myTable.loadFactor;
}

int getCapacity(table myTable) {
	return myTable.capacity;
}

#endif