// Implements a dictionary's functionality

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#include "dictionary.h"

// define number of symbols in our alphabet 26 for the letters and one for the apostrophe
#define SYMBOLS 27

// trie node definition
typedef struct trieNode
{
    struct trieNode *children[SYMBOLS];
    // if the node represent the end of a word
    bool isEnd;
}
trieNode;

// number of words in dictionary
unsigned int word_count = 0;

// dictionary already loaded or not
bool loaded = false;

// creates a new trie node
trieNode* makeNode(void);
// free a trie structure given the a pointer to its root
void freeTrie(trieNode *head);

// pointer to the root of the trie
trieNode *root = NULL;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int len = strlen(word);
    trieNode *tmp = root;

    for (int i = 0; i < len; i++)
    {
        int index = (word[i] == '\'') ? 26 : tolower(word[i]) - 'a';
        if (tmp->children[index] == NULL)
        {
            return false;
        }
        else
        {
            tmp = tmp->children[index];
        }
    }
    return (tmp->isEnd) ? true : false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // not needed for a trie implementation
    return 0;
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

    // buffer to store all the words in the dictionary
    char *buffer = NULL;

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
    loaded = true;

    // create root node
    root = makeNode();

    // pointer we will use to traverse and build the trie structure
    trieNode *tmp = root;
    
    // iterate over each char in buffer
    for (int i= 0; i < fsize; i++)
    {
        // when we reach the end of a word
        // set isEnd to true, increase word count
        // and reset the pointer to the root of the trie
        if (buffer[i] == '\n')
        {
            tmp->isEnd = true;
            word_count++;
            tmp = root;
        }
        else
        {
            // get a index based on the current char
            int index = (buffer[i] == '\'') ? 26 : tolower(buffer[i]) - 'a';

            // if there is no node at the current location
            // create one and move the pointer to it
            if (tmp->children[index] == NULL)
            {
                tmp->children[index] = makeNode();
                tmp = tmp->children[index];
            }
            // if there is already a node move the pointer to it
            else
            {
                tmp = tmp->children[index];
            }
        }
    }

    // once we are done creating the trie structure
    // free the dictionary buffer
    free(buffer);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return (loaded) ? word_count : 0    ;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    if(loaded)
    {
        trieNode *tmp = root;
        freeTrie(tmp);
        return true;
    }
    return false;
}

// creates a trie node and returns a pointer to it
trieNode* makeNode(void)
{
    trieNode *node = (trieNode *)malloc(sizeof(trieNode));
    node->isEnd = false;
    for (int i = 0; i < SYMBOLS; i++)
    {
        node->children[i] = NULL;
    }
    return node;
}

// frees a tire given a pointer to its root node
void freeTrie(trieNode *head)
{
    for (int i = 0; i < SYMBOLS; i++)
    {
        if (head->children[i] != NULL)
        {
            freeTrie(head->children[i]);
        }
    }
    free(head);
}

