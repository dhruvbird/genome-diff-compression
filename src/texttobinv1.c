#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define MAXLENGTH 100

FILE * pFile;//For reading
FILE * wFile;//For writing

void binary(unsigned int);
int main()
{
   
   char mystring [MAXLENGTH];
// decide how many max characters present in each line of the text file
   int len;

   pFile = fopen ("myfile.txt" , "r");
   wFile = fopen ("myfile1.txt" , "w");
   if (pFile == NULL || wFile == NULL) perror ("Error opening file");
   else {
     while ( fgets (mystring , MAXLENGTH , pFile) != NULL ){
       //Process each line here
       len = strlen(mystring);
       mystring[len-1]='\0';
       unsigned int number = atoi(mystring);
       binary(number);
     }
     fclose (pFile);
   }

   return 0;
}

//Writes the binary value of the number passed to the file
void binary(unsigned int number) {
        unsigned int remainder;
        if(number <= 1) {
	  fprintf(wFile,"%d",number);
                return;
        }

        remainder = number%2;
        binary(number >> 1);
	fprintf(wFile,"%d",remainder);
}

