#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int main(void)
{
  //get input text from user.
  string text = get_string("Text: ");

  int i = 0;
  int letters, words, sentences;
  letters = words = sentences = 0;

  //count letters, words and sentences in the user provided text.
  while(text[i] != '\0')
  {
    char c = text[i];
    if (isalpha(c))
    {
      letters++;
    }
    if (isspace(c)) 
    {
      words++;
    }
    if (c == '.' || c == '!' || c == '?')
    {
      sentences++;
    }
    //check if the string is about to end and count the last word if it is.
    if (text[i + 1] == '\0')
    {
      words++;
    }
    i++;
  }
  float l = (letters * 100) / words;
  float s = (sentences * 100) / words;
  float index = (0.0588 * l) - (0.296 * s) - 15.8;
  
  if (index < 0)
  {
    printf("Before Grade 1\n");
  }
  else if (index >=16)
  {
    printf("Grade 16+\n");
  }
  else
  {
    printf("Grade %.0f\n", round(index));
  }
}