#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;


#define LENGTH 16

void WriteBit (int bit);
void Flush_Bits (void);

int current_bit = 0;
unsigned char bit_buffer;

FILE * pFile;
FILE * tFile;

int main(){

pFile = fopen ("myfile2.txt" , "r");
tFile = fopen ("myfileby.txt",  "w");
char cur_bit;
 if (pFile == NULL || tFile == NULL) {
  perror ("Error opening file");
 }
 else{
   while ( 1 )
     {
       cur_bit = fgetc(pFile);
        if (cur_bit == EOF)
           break ;
	printf("%c \n",cur_bit);
	switch (cur_bit){
	  case '1' : {
	    WriteBit(1);
	    break;
	  }
	  case '0' : {
	    WriteBit(0);
	    break;
	  }
	  default: {
	    printf("Invalid Charcter in the textfile\n");
	    perror("Invalid Character !!! Stop !!");
	  }
	  }
     }
 Flush_Bits();
 fclose(pFile);
 fclose(tFile);
 }
  return 0;
}


//source : http://stackoverflow.com/questions/1856514/writing-files-in-bit-form-to-a-file-in-c

void WriteBit (int bit)
{
  printf("Calling writeBit %d \n",bit);
  if (bit)
    bit_buffer |= (1<<current_bit);
  printf("%d \n",bit_buffer);

  current_bit++;
  if (current_bit == 8)
  {
    printf("In Write Bit %d\n",bit_buffer);
    //fprintf(tFile,"%d",bit_buffer);
    fwrite(&bit_buffer, 1, 1, tFile);
    current_bit = 0;
    bit_buffer = 0;
  }
}

void Flush_Bits (void)
{
  while (current_bit) 
    WriteBit (0);
}
