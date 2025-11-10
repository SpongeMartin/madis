#include "hashtable.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){
    char key_buffer[4096];
    char value_buffer[4096];
    HashTable *hash_table = constructHashTable(2);
    while (1){
        fgets(key_buffer, 4096, stdin);
        fgets(value_buffer, 4096, stdin);
        int *val = malloc(sizeof(int));
        *val = atoi(value_buffer);
        addElement(hash_table, key_buffer, val);
        int *msg = (int*)findElement(hash_table, key_buffer);
        if (msg){
            printf("I found %i\n", *msg);
        } else printf("Nigga nooooooo");
    }
    removeElement(hash_table, key_buffer);
    return 0;
}