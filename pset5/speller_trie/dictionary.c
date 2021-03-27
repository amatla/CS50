// Implements a dictionary's functionality

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#include "dictionary.h"

// define number of symbols in our alphabet
#define SYMBOLS 27

typedef struct trieNode
{
    struct trieNode *children[SYMBOLS];
    bool isEnd;
}
trieNode;

// number of words in dictionary
unsigned int word_count = 0;

// dictionary already loaded or not
bool loaded = false;

trieNode* makeNode(void);
void addWord(trieNode *head, char *word);
void freeTrie(trieNode *head);
void printTrie(trieNode *head, char *word, int index);

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
    // TODO
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

    root = makeNode();
    trieNode *tmp = root;

    for (int i= 0; i < fsize; i++)
    {
        if (buffer[i] == '\n')
        {
            tmp->isEnd = true;
            word_count++;
            tmp = root;
        }
        else
        {
            int index = (buffer[i] == '\'') ? 26 : tolower(buffer[i]) - 'a';
            if (tmp->children[index] == NULL)
            {
                tmp->children[index] = makeNode();
                tmp = tmp->children[index];
            }
            else
            {
                tmp = tmp->children[index];
            }
        }
    }
    //char word[LENGTH + 1];
    //printTrie(root, word, 0);
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

void addWord(trieNode *head, char *word)
{
    int len = strlen(word);
    for(int i = 0; i < len; i++)
    {
        int index = (tolower(word[i]) - 'a');
        if (head->children[index] == NULL)
        {
            head->children[index] = makeNode();
        }
        head = head->children[index];
    }
    head->isEnd = true;
}

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

void printTrie(trieNode *head, char *word, int index)
{
    if (head->isEnd)
    {
        word[index] = '\0';
        printf("%s\n", word);
    }
    for (int i = 0; i < SYMBOLS; i++)
    {
        if (head->children[i] != NULL)
        {
            word[index] = i + 'a';
            printTrie(head->children[i], word, index + 1);
        }
    }
}

