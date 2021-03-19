// Implements a dictionary's functionality
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 1;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
    return 0;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    //if it exist open dictionary file for reading
    //return false if the file doesnt exist.
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
      return false;
    
    fseek(dict, 0, SEEK_END);
    long fsize = ftell(dict);
    fseek(dict, 0, SEEK_SET);
    char *buffer = malloc(fsize);
    if (buffer == NULL)
      return false;
    fread(buffer, 1, fsize, dict);
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    return false;
}
