#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

unsigned long hash(unsigned char *str){
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

void *findElement(HashTable *ht, char *key){
    unsigned long index = hash((unsigned char *) key) % ht->size;

    Entry *curr = ht->table[index];
    while (curr) {
        if (strcmp(curr->key,key) == 0) {
            return curr->value;
        }
        curr = curr->next;
    }
    return NULL;
}

void addElement(HashTable *ht, char *key, void *value) {
    unsigned long index = hash((unsigned char *)key) % ht->size;
    Entry *curr = ht->table[index];
    Entry *prev = NULL;

    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            curr->value = value;
            return;
        }
        prev = curr;
        curr = curr->next;
    }

    Entry *new_entry = malloc(sizeof(Entry));
    if (!new_entry) {
        perror("Failed creating new entry");
        exit(EXIT_FAILURE);
    }
    new_entry->key = strdup(key);
    new_entry->value = value;
    new_entry->next = NULL;
    if (prev)
        prev->next = new_entry;
    else
        ht->table[index] = new_entry;
}

void removeElement(HashTable *ht, char *key) {
    unsigned long index = hash((unsigned char *)key) % ht->size;
    Entry *curr = ht->table[index];
    Entry *prev = NULL;
    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            if(prev) {
                prev->next = curr->next;
            } else {
                ht->table[index] = curr->next;
            }
            free(curr->key);
            free(curr->value);
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}


HashTable *constructHashTable(size_t table_size){
    HashTable *ht = malloc(sizeof(HashTable));
    if (!ht) perror("Reserving space for Hash Table failed");
    ht->size = table_size;
    ht->table = calloc(table_size, sizeof(Entry *));
    if (!ht->table) perror("Reserving space for Hash Table table failed");
    return ht;
}