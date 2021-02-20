/**
 * Implements "Mario more comfortable" assignment.
 * 
 * Creates "super Mario's style" pyramids using a user definged number of '#' as bricks.
 * 
 * */

#include <stdio.h>
#include <cs50.h>

int main(void)
{
  //ask user for pyramid heigth. must be an integer between 1 and 8 inclusive.
  int height = 0;
  do
  {
    height = get_int("Height: ");
  } 
  while (height < 1 || height > 8);

  //print each line of the pyramid
  int line = 1;
  while (line <= height)
  {
    //print spaces
    for (int i = 0; i < height - line; i++)
    {
      printf(" ");
    }
    //print right side hashes
    for (int j = 0; j < line; j++)
    {
      printf("#");
    }
    //print double spaces inbetween pyramid sides
    printf("  ");
    //print left side hashes
    for (int j = 0; j < line; j++)
    {
      printf("#");
    }
    //move to next line
    printf("\n");
    line++;
  }
}
    
