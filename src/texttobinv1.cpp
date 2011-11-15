#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<math.h>

using namespace std;


#define MAXLENGTH 100

/*Files pointer for reading writing and testing*/
FILE * pFile;//For reading
FILE * wFile;//For writing
/*Files pointer for reading writing and testing*/

/******For Binary Conversion***********/
void binary(unsigned int number);
void init();
void writebin2file(int nob);
void binary_no(unsigned int no,int no_of_bits);
/******For Binary Conversion***********/


/******Global Static & Constant Variables For Binary Conversion***********/
int binary1[32];
static int i;
const int zero=0;
/******Global Static & Constant Variables For Binary Conversion***********/

int current_bit = 0;
unsigned char bit_buffer;

void case_runofchar(string mystrings);
void case_range(string mystrings);
void case_singlechar(char base);

int main()
{
   
   char mystring [MAXLENGTH];
   int len;

   pFile = fopen ("myfile.txt" , "r");
   wFile = fopen ("myfile2.txt" , "w");

   if (pFile == NULL || wFile == NULL) 
     perror ("Error opening file");
   else {
     while ( fgets (mystring , MAXLENGTH , pFile) != NULL ){
       //Process each line here
       len = strlen(mystring);
       mystring[len-1]='\0';
       string mystrings = mystring;       
       //printf("%s\n%s\n",mystring,mystrings.c_str());
       string typec;
       int pos_type = mystrings.find(' ');
       //printf("pos_type:%d \n",pos_type);
       typec=mystrings.substr(0,pos_type);
       //cout << typec<<endl;
       int type = atoi(typec.c_str());
       //       fputc('x',wFile);
       //CASE 2: when we have a run of characters :01,32,C
       switch(type){
       case 1: {
	 // Single character
	 //printf("Case 1\n");
	 char base = mystrings.substr(1,mystrings.find('\n'))[1];
	 //cout << mystrings<<endl;
	 //printf("base: %c\n",base);
	 // printf("%d",zero);
	 binary_no(0,1);
	 case_singlechar(base);
	// printf("\n");
	 break;
       }
       case 2:{
	 //printf("case 2\n");
	 binary_no(2,2);
	 case_range(mystrings);	 
	// printf("\n");
	 break;
       }
     //CASE 3 : 11,236-256 
       case 3:{
	 //printf("case 3\n");
	 binary_no(3,2);
	 case_runofchar(mystrings);
	// printf("\n");
	 break;
       }
       }
     }
     fclose (pFile);
     fclose (wFile);
   }
   return 0;
}

//Case 1: Single Character 1 A
void case_singlechar(char base){

  //A-001 -1,C-010:2,T:011:3,G:010(4),N:011
switch(toupper(base)){
       case 'A':
	 binary_no(1,3);
	 break;
       case 'C':
	 binary_no(2,3);
	 break;
       case 'T':
	 binary_no(3,3);
	 break;
       case 'G':
	 binary_no(4,3);
	 break;
       case 'N' :
	 binary_no(5,3);
	 break;
       }

}
//Case 2 :For range : format assumed as 2 256 30
void case_range(string mystrings){
  //printf("case_range \n");
     int pos_n = mystrings.find(' ',1)+1;
     int pos_e = mystrings.find(' ',pos_n+1);
     //printf("pos_n:%d\n",pos_n);
     //printf("pos_e:%d\n",pos_e);
     unsigned int myfromrange = atoi(mystrings.substr(pos_n,pos_e-pos_n).c_str());
     int pos_range_d = mystrings.find(' ',pos_e);
     unsigned int diff = atoi(mystrings.substr(pos_range_d).c_str());
     //printf("from:%d to:%d\n", myfromrange,diff);
    
     binary_no(myfromrange,30);
     //     fputc('x',wFile);//Only added for testing
     binary_no(diff,16);
     //     fputc('x',wFile);
}
//Case 3:For run of characters : format assumed as 3 32 C
void case_runofchar(string mystrings){
  //printf("case_runofchar\n");
       string mynumber;
       int pos_n = mystrings.find(' ',1)+1;
       int pos_e = mystrings.find(' ',pos_n+1);
       //printf("pos_n:%d\n",pos_n);
       //printf("pos_e:%d\n",pos_e);
       mynumber = mystrings.substr(pos_n,pos_e-pos_n);
       //cout<<mynumber<<endl;
       unsigned int number = atoi(mynumber.c_str());
       //fputc('x',wFile);
       binary_no(number,11);
       char base = mystrings.c_str()[pos_e+1];
       //printf("base : %c\n", base);
       case_singlechar(base);
       //fputc(base,wFile);//For testing
       //printf("%c",base);
       
   }


/******Functions For Binary Conversion***********/
void binary_no(unsigned int no,int no_of_bits){
  init();
  binary(no);
  binary1[i]=-1;
  writebin2file(no_of_bits);
}

void init(){
  int j;
  for(j=0;j<31;j++)
    binary1[i]=0;
  i=0;
}

void writebin2file(int nob){

  int j,k;
  int no=0;

  if(nob < i){
    printf("More numbers required to represent the number \n");
    return;
  }

  for(k=0;k<(nob-i);i++){
    //For testing:
    printf("%d",zero);

    //Writing to a file
    fprintf(wFile,"%d",zero);
    no++;
  }

  for(j=0;binary1[j]!=-1;j++){
    printf("%d",binary1[j]);
    fprintf(wFile,"%d",binary1[j]);
    no++;
  }

  //printf("\t %d\n",no);

}

//Writes the binary value of the number passed to the file
void binary(unsigned int number) {
        unsigned int remainder;
        if(number <= 1) {
	  binary1[i++]=number;
                return;
        }

        remainder = number%2;
        binary(number >> 1);
	binary1[i++]=remainder;
}
/******For Binary Conversion***********/
