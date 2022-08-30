#ifndef HASHTABLE_H
#define HASHTABLE_H

// define the load_factor for resizing
#define LOAD_FACTOR 0.7

typedef struct hashtable
{
	int *keyArray;

	// 1d array of strings with maximum length (100 + 1) * capcacity ('\0' character)
	char *valueArray; 
	bool *isActiveArray; // for deleting elements

	int count;
	int capacity;
	double loadFactor;

	// true: linear probing, false: quadratic probing
	bool collisionHandler;

} table;

#endif