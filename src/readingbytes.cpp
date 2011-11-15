#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <math.h>
using namespace std;


#define LENGTH 16

void WriteBit (int bit);
void Flush_Bits (void);
void readingbytes();

int current_bit = 0;
unsigned char bit_buffer;

FILE * pFile;
FILE * tFile;

int main(){

  readingbytes();

  return 0;
}


void readingbytes(){
  
  FILE *read;
  read =fopen ("myfileby.txt",  "r");
  long lSize;
  char buffer;
  size_t result;
  printf("In reading bytes \n");
  result = fread (&buffer,1,1,read);
  //printf("%d",result);
  int i;
    printf("Values : %d %c\n",buffer,buffer);
  fclose(read);
}

