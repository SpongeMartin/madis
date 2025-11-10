#include "hashtable.c"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "auxiliary.c"

//gcc -Wall -Wextra -o hashtable test.c hashtable.c

int main(){
    /* char key_buffer[4096];
    char value_buffer[4096];
    HashTable *hash_table = constructHashTable(2);
    
    fgets(key_buffer, 4096, stdin);
    fgets(value_buffer, 4096, stdin);
    int *val = malloc(sizeof(int));
    *val = atoi(value_buffer);
    addElement(hash_table, key_buffer, val);
    int *msg = (int*)findElement(hash_table, key_buffer);
    if (msg){
        printf("I found %i\n", *msg);
    } else printf("Nigga nooooooo");
    
    removeElement(hash_table, key_buffer);
    return 0; */

    int key = 10;
    char stringified_key[16];
    itoa(key, stringified_key);

    HashTable *hash_table = constructHashTable(100);
    char value_buffer[] = "Hello Nickher";
    printf("Hey boy %s\n", stringified_key);


    addElement(hash_table, stringified_key, strdup(value_buffer));
    char *msg = findElement(hash_table, stringified_key);
    if (msg) {
        printf("I found: %s\n", msg);
    } else {
        printf("Not found.\n");
    }
    removeElement(hash_table, stringified_key);
    return 0;
}