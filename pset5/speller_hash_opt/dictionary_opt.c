// Implements a dictionary's functionality
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

//#define TABLESIZE 200003

//value to scale the size of the hash table 
//based on the number of words in dictionary. 
//a value of 0.5 will produce a table twice the size of the word number
#define TABLELOAD 0.75 

typedef struct listNode
{   
    char *data;
    struct listNode *next;
}
listNode;

unsigned int dbj2(const char* word);

//global variable to store the number of words in dictionary
unsigned int wordCount = 0;

//global variable for dictionary loaded
bool loaded = false;

//pointer for string buffer
char *dictBuffer = NULL;

//hash table pointer
listNode **hashTable = NULL;

//global variable for the size of the hash table
int tableSize = 0;

//node array pointer
listNode *nodeArray = NULL;

//Returns true if word is in dictionary else false
bool check(const char *word)
{
    //convert word to lowercase
    int i = 0;
    char wordLower[LENGTH + 1] = {'\0'};
    while(word[i] != '\0')
    {
        wordLower[i] = tolower(word[i]);
        i++;
    }

    //hash the lowercase word
    int index = (dbj2(wordLower) % tableSize);

    listNode *wordPtr = hashTable[index];

    //check if word is at hashTable[index]
    while(wordPtr != NULL)
    {
        if (strcmp((wordPtr->data), wordLower) == 0)
        {
            return true;
        }
        else
        {
            wordPtr = wordPtr->next;
        }
    }
    return false;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //open the dictionary file for reading
    FILE *dictPtr = fopen(dictionary, "r");
    if ( dictPtr == NULL)
    {
        fprintf(stderr, "Could not open dictionary %s\n", dictionary);
        return false;
    }

    //move file ptr to the end of the file so we can determine its size
    fseek(dictPtr, 0, SEEK_END);
    unsigned int bufSize = ftell(dictPtr);

    //allocate memory for a char array of bufSize
    dictBuffer = (char *) malloc((bufSize) *sizeof(char));

    //move file pointer back at the beginning of the file
    fseek(dictPtr, 0, SEEK_SET);

    //read the whole file into our buffer
    fread(dictBuffer, sizeof(char), bufSize, dictPtr);

    //close dictionary file
    fclose(dictPtr);

    //iterate over the array and change '/n' to '\0' and count number of words
    for(unsigned int i = 0; i < bufSize ; i++)
    {
        if(dictBuffer[i] == '\n')
        {
            dictBuffer[i] = '\0';
            wordCount++;
        }
    }

    //initialize table size
    tableSize = floor(wordCount/TABLELOAD);

    //create hash table as an array pf pointers to listNode
    hashTable = (listNode **) malloc(tableSize * sizeof(listNode *));

    //create an array of listNode to hold the words in the dictionary
    nodeArray = (listNode *) malloc(wordCount * sizeof(listNode));

    //init hash table
    for (int i = 0; i < tableSize; i++)
    {
        hashTable[i] = NULL;
    }

    //for each word in the dictionary
    int currentWord = 0;
    int wordLen = 0;
    for (unsigned int i = 0; i < wordCount; i++)
    {
        //set the corresponding node data to the word
        nodeArray[currentWord].data = &dictBuffer[wordLen];

        //hash the word to get an index
        int index = (dbj2(nodeArray[currentWord].data)) % tableSize;

        //insert the node at the beginning of the linked list
        //at index position in the hash table
        nodeArray[currentWord].next = hashTable[index];
        hashTable[index] = &nodeArray[currentWord];

        //increase word counter and
        //find the position of the next word in the dictionary buffer (dictBuf)
        //by adding the lenght of the previous word (nodeArray[currentWord-1]) to wordLen (add +1 for '\0')
        currentWord++;
        wordLen += strlen(nodeArray[currentWord-1].data) + 1;
    }
    loaded = true;
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return (loaded) ? wordCount : 0;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    free(nodeArray);
    free(hashTable);
    free(dictBuffer);
    return true;
}

//djb2 implementation adapted from Neel Mehta @ https://github.com/hathix
unsigned int dbj2(const char* word)
{
    unsigned long hash = 5381;

    for (const char* ptr = word; *ptr != '\0'; ptr++)
    {
        hash = ((hash << 5) + hash) + *ptr;
    }
    return hash;
}
