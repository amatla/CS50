// Implements a dictionary's functionality

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
// changed word to a char * so we can make it point 
// directly to a word in the dictionary buffer we load into     memory
typedef struct node
{
    char *word;
    struct node *next;
}
node;

/** instead of using a fixed number for the hash table size 
    we are goin go scale it depending on the number of words in the dictionary **/

// Number of buckets in hash table
// const unsigned int N = 1;

/** value to scale the size of the hash table 
    based on the number of words in dictionary. 
    a value of 1 will produce a table the same size of the word number
    a value of 0.5 a table half the size and a value of 2 a table double the size **/
#define TABLELOAD 1

/** changed the hash table to be a pointer to an array of pointers that we can allocate later
    with a size based on the number of words in the dictionary **/
// Hash table
node **table;

//size of the hash table 
//it' s going to be the product of TABLELOAD and the numnber of words in the dictionary
int tablesize = 0;

// buffer to store all the words in the dictionary
char *buffer = NULL;

// number of words in dictionary
unsigned int word_count = 0;

// array of nodes that is going to contain all the nodes we are going to need
node *nodeArray = NULL;

// dictionary already loaded or not
bool loaded = false;

//Returns true if word is in dictionary, else false
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
// using Daniel J. Bernstein djb2 hash function as posted by him years ago on comp.lang.c
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
    // if it exist open dictionary file for reading
    // return false if the file doesnt exist.
    FILE *ptr = fopen(dictionary, "r");
    if (ptr == NULL)
    {
        return false;
    }
    // move at the end of the dictionary file
    fseek(ptr, 0, SEEK_END);

    // get the size of the dictionary in bytes
    unsigned int fsize = ftell(ptr);

    // move back at the beginning of the file
    fseek(ptr, 0, SEEK_SET);

    // allocate memory for the dictionary
    buffer = (char *)malloc(fsize);

    // read the whole dictionary into buffer
    fread(buffer, sizeof(char), fsize, ptr);

    // close the file
    fclose(ptr);

    // make sure buffer could be created
    if(buffer == NULL)
    {
        printf("Can't crate buffer");
        return false;
    }

    // replace every end of line char with a '\0'
    // so now we have a buffer with every word in dictionary
    // as sequence of valid strings
    for (int i = 0; i < fsize; i++)
    {
        if (buffer[i] == '\n')
        {
            word_count++;
            buffer[i] = '\0';
        }
    }

    // create a hash table based on the number of words in       dictionary
    tablesize = floor(word_count * TABLELOAD);
    table = (node **)malloc(tablesize * sizeof(node *));

    // initialize each element of the hash table to NULL.
    for (int i = 0; i < tablesize; i++)
    {
        table[i] = NULL;
    }

    // create an array of nodes as big as the number of words in dictionary and allocate memory for it
    nodeArray = (node *)malloc(word_count * sizeof(node));

    // index to keep track of the position in the buffer 
    unsigned int word_length = 0;

    // hashed index 
    unsigned int index = 0;

    // for each word in the dictionary
    for (int i = 0; i < word_count; i++)
    {   
        // set the corresponding node word to point to 
        // the correct postion in the buffer array
        nodeArray[i].word = &buffer[word_length];
        nodeArray[i].next = NULL;

        // get a hashed index from the current word
        index = hash(nodeArray[i].word) % tablesize;

        // insert our node at the beginning of the linked list 
        // at table[index]
        if (table[index] == NULL)
        {
            table[index] = &nodeArray[i];
        }
        else
        {
            nodeArray[i].next = table[index];
            table[index] = &nodeArray[i];
        }
        // move to the position of next word in buffer
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
