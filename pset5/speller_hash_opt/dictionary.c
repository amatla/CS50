// Implements a dictionary's functionality

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char *word;
    struct node *next;
}
node;

// Number of buckets in hash table
//const unsigned int N = 1;

//value to scale the size of the hash table 
//based on the number of words in dictionary. 
//a value of 1 will produce a table the same size of the word number
#define TABLELOAD 1

// Hash table
node **table;

//size of the hash table 
//it is going to be the product of TABLELOAD and the numnber of words in the dictionary
int tablesize = 0;

char *buffer = NULL;
//number of words in dictionary
unsigned int word_count = 0;

//array of nodes
node *nodeArray = NULL;

//dictionary already loaded or not
bool loaded = false;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    unsigned int index = hash(word) % tablesize;
    node *head = table[index];
    while(head != NULL)
    {
        if (strcasecmp(head->word, word) == 0)
        {
            return true;
        }
        head = head->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned long hash = 5381;

    while(*word != '\0')
    {
        hash = ((hash << 5) + hash) + tolower(*word);
        word++;
    }
    return hash;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *ptr = fopen(dictionary, "r");
    if (ptr == NULL)
    {
        return false;
    }
    fseek(ptr, 0, SEEK_END);
    unsigned int fsize = ftell(ptr);
    fseek(ptr, 0, SEEK_SET);
    buffer = (char *)malloc(fsize);
    fread(buffer, sizeof(char), fsize, ptr);
    fclose(ptr);
    if(buffer == NULL)
    {
        printf("Can't crate buffer");
        return false;
    }

    for (int i = 0; i < fsize; i++)
    {
        if (buffer[i] == '\n')
        {
            word_count++;
            buffer[i] = '\0';
        }
    }

    tablesize = floor(word_count * TABLELOAD);
    table = (node **)malloc(tablesize * sizeof(node *));

    for (int i = 0; i < tablesize; i++)
    {
        table[i] = NULL;
    }

    nodeArray = (node *)malloc(word_count * sizeof(node));
    unsigned int word_length = 0;
    unsigned int index = 0;
    for (int i = 0; i < word_count; i++)
    {
        nodeArray[i].word = &buffer[word_length];
        nodeArray[i].next = NULL;
        index = hash(nodeArray[i].word) % tablesize;
        if (table[index] == NULL)
        {
            table[index] = &nodeArray[i];
        }
        else
        {
            nodeArray[i].next = table[index];
            table[index] = &nodeArray[i];
        }
        word_length += strlen(nodeArray[i].word) + 1;
    }
    loaded = true;
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return (loaded) ? word_count : 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    if(loaded)
    {   
        free(buffer);
        free(nodeArray);
        free(table);
        return true;
    }
    else
    {
        return false;
    }
}
