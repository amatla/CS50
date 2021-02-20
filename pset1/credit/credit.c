#include <stdio.h>
#include <cs50.h>

int main(void) 
{
  
    //get credit card number from user making sure it is a positive number
    long cc_number = 0;
    do
    {
        cc_number = get_long("Number:");
    }
    while (cc_number < 0);

    //Implement Luhn’s Algorithm to determine if it is a valid cc number
    bool isOther = false;
    int checksum = 0;
    int digits = 0;
    int nstart = 0;

    while (cc_number > 0)
    {
      //multiply every other digit by 2 and add it to the checksum. 
      //if the result of the multiplication is double digits (> 9) add them together before adding to checksum.
      //otherwise add the digit to the checksum.
      if (isOther)
      {
        int value = (cc_number % 10) * 2;

        checksum += (value < 9) ? value : value - 9;
      }
      else
      {
        checksum += cc_number % 10;
      }
      //save the first 2 digit of the card
      if (cc_number < 100 && cc_number > 9)
      {
        nstart = cc_number;
      }
      //move to the next number and keep count ot the number of digits
      cc_number /= 10;
      digits++;
      isOther = !isOther;
    }
    //check id checksum is valid
    bool isValid = (checksum % 10) == 0 ? true : false;

    //check if it is a valid card and print the result
    if (isValid && digits == 15 && (nstart == 34 || nstart == 37))
    {
      printf("AMEX\n");
    }
    else if (isValid && digits == 16 && (nstart >= 51 && nstart <=55))
    {
      printf("MASTERCARD\n");
    }
    else if (isValid && (digits == 13 || digits == 16) && ((nstart / 10) == 4))
    {
      printf("VISA\n");
    }
    else
    {
      printf("INVALID\n");
    }
 }