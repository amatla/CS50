// Implements a dictionary's functionality
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <cs50.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// number of words in dictionary
unsigned int word_count = 0;
// Number of buckets in hash table
const unsigned int N = 150000;

// Hash table
node *table[N];

// dictionary successfully loaded or not
bool loaded = false;

// creates a new linked list node.
node *create_node(char *word);

// pushes a node at the beginning of a list and returns a pointer to the head of the list.
node *push_node(node *head, node *new_node);

// prints a list given the list head.
void print_list(node *head);

// inserts a node into the hash table at a given index.
void insert_table(node *element, unsigned int index);

// prints the hash table
void print_table(node *hash_table[]);

// frees all the nodes in a list given a pointer to the list head.
void free_list(node *head);

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // get a index for the word being checked.
    unsigned int index = hash(word);
    index = index % N;

    // pointer to the head of the list in table[index].
    node *head = table[index];

    // check every node in the list to see if cointains the word we are checking for.
    while (head != NULL)
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
    unsigned int hash = 0;
    while (*word != '\0')
    {
        hash = (hash * 33) ^ tolower(*word);
        word++;
    }
    return hash;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // if it exist open dictionary file for reading
    // return false if the file doesnt exist.
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }
    // move at the end of the dictionary file
    fseek(dict, 0, SEEK_END);

    // calculate size of the dictionay in bytes
    long fsize = ftell(dict);

    // move back at the beginning of the dictionary file
    fseek(dict, 0, SEEK_SET);

    // allocate memory for the dictionary
    char *buffer = (char *)malloc(fsize);

    // copy the dictionary from file to buffer
    fread(buffer, 1, fsize, dict);

    // close dictionary file
    fclose(dict);

    // make sure buffer could be created
    if (buffer == NULL)
    {
        return false;
    }

    // initialize each element of the hash table to NULL.
    for (int k = 0; k < N; k++)
    {
        table[k] = NULL;
    }

    // current word being read form buffer.
    char word[LENGTH + 1];

    // hashed index of the current word being read from buffer.
    unsigned int index = 0;

    // pointer to the node being created
    node *tmp_node;

    // index for the current letter of the word being read from buffer.
    int j = 0;

    // for every char in buffer
    for (int i = 0; i < fsize; i++)
    {
        // if we reach the end of a word
        if (buffer[i] == '\n')
        {
            // increase word count
            word_count++;
            // add '\0' to the end of the string
            word[j] = '\0';
            // create a node from the current word
            // and get a hashed index from it
            tmp_node = create_node(word);
            index = hash(word);
            index = index % N;
            // insert node in hash table
            insert_table(tmp_node, index);
            // reset next word char index to 0
            j = 0;
        }
        // add char from buffer to the current word
        else
        {
            word[j++] = buffer[i];
        }
    }
    // free memory allocated for buffer
    free(buffer);
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
    for (int i = 0; i < N; i++)
    {
        if (table[i] != NULL)
        {
            free_list(table[i]);
        }
    }
    return true;
}

// frees all the nodes in a list given a pointer to the list head.
void free_list(node *head)
{
    node *tmp = head;
    while (head->next != NULL)
    {
        head = head->next;
        free(tmp);
        tmp = head;
    }
    free(head);
}

// creates a new linked list node.
node *create_node(char *word)
{
    node *new_node = (node *)malloc(sizeof(node));
    strcpy(new_node->word, word);
    new_node->next = NULL;
    return new_node;
}

// pushes a node at the beginning of a list and returns a pointer to the head of the list.
node *push_node(node *head, node *new_node)
{
    new_node->next = head;
    head = new_node;
    return head;
}

// inserts a node into the hash table at a given index.
void insert_table(node *element, unsigned int index)
{
    if (table[index] != NULL)
    {
        table[index] = push_node(table[index], element);
    }
    else
    {
        table[index] = element;
    }
}

// prints a list given the list head.
void print_list(node *head)
{
    node *tmp = head;
    while (tmp != NULL)
    {
        printf("%s->", tmp->word);
        tmp = tmp->next;
    }
    printf("NULL\n");
}

// prints the hash table.
void print_table(node *hash_table[])
{
    for (int i = 0; i < N; i++)
    {
        if (table[i] !=  NULL)
        {
            printf("table[%d] = ", i);
            print_list(table[i]);
        }
        else
        {
            printf("table[%d] = NULL\n", i);
        }
    }
}