#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main (int argc, char *argv[])
{
  //check if the amount or arguments the program was run with is right (only one argument).
  if (argc != 2)
  {
    printf("Usage: %s key\n", argv[0]);
    return 1;
  }
  //check if the length of the cipher provided is correct
  int cipher_len = strlen(argv[1]);
  printf("cipherlen = %d\n", cipher_len);
  if (cipher_len != 26)
  {
    printf("Key must contain 26 characters.\n");
    return 1;
  }
  //iterate over each element of the cipher key
  //check that the cipher contains only letters and only one instance of each letter
  for (int i = 0; i < cipher_len - 1; i++)
  {
    //if the current element is not a letter print error message and exit
    if(!(isalpha(argv[1][i])))
    {
      printf("Key must contain only letters.\n");
      return 1;
    }
    //compare the current element of the array with the remaining elements 
    //to ensure no duplicates are peresent
    for (int j = i + 1; j <= cipher_len; j++)
    {
      if (argv[1][i] == argv[1][j])
      {
        printf("Key must contain only one occurence of each letter.\n");
        return 1;
      }
    }
  }
}