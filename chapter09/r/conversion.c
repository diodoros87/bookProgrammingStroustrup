#include <math.h>
#include <stdio.h>

int main(void) {
   char conversion[1076], intPart_reversed[311];
   int i, charCount = 0;
   double fp_int, fp_frac, fp = 3.14159;

   fp_frac = modf(fp,&fp_int); //Separate integer/fractional parts

   while (fp_int > 0) //Convert integer part, if any
      {
      intPart_reversed[charCount++] = '0' + (int)fmod(fp_int,10);
      fp_int = floor(fp_int/10);
      }

   for (i=0; i<charCount; i++) //Reverse the integer part, if any
      conversion[i] = intPart_reversed[charCount-i-1];

   conversion[charCount++] = '.'; //Decimal point

   while (fp_frac > 0) //Convert fractional part, if any
      {
      fp_frac*=10;
      fp_frac = modf(fp_frac,&fp_int);
      conversion[charCount++] = '0' + (int)fp_int;
      }

   conversion[charCount] = 0; //String terminator
   printf("conversion = %s\n", conversion);
}

/* gcc  -Wfatal-errors -Wall -Wextra -Wconversion -std=c11 -o conversion conversion.c  */
