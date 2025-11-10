#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stddef.h>

typedef struct Entry{
    char *key;
    void *value;
    struct Entry *next;
} Entry;

typedef struct {
    Entry** table;
    size_t size;
} HashTable;

HashTable *constructHashTable(size_t table_size);
void addElement(HashTable *ht, char *key, void *value);
void *findElement(HashTable *ht, char *key);
void removeElement(HashTable *ht, char *key);

#endif //HASHTABLE_H