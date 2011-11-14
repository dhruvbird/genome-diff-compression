#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#define MAXLENGTH 100

FILE * pFile;//For reading
FILE * wFile;//For writing

void binary(unsigned int);
void case_runofchar(string mystrings);
void case_range(string mystrings);
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
       string mystrings = mystring;       
       printf("%s\n%s\n",mystring,mystrings.c_str());
       string typec;
       int pos_type = mystrings.find(',');
       printf("pos_type:%d \n",pos_type);
       typec=mystrings.substr(0,pos_type);
       cout << typec<<endl;
       int type = atoi(typec.c_str());
       binary(type);
       //       fputc('x',wFile);
       //CASE 2: when we have a run of characters :01,32,C
       switch(type){
       case 2:{
	 printf("case 2\n");
	 case_runofchar(mystrings);
	 break;
       }
     //CASE 3 : 11,236-256 
       case 3:{
	 printf("case 3\n");
	 case_range(mystrings);
	 break;
       }
       }
     }
     fclose (pFile);
   }
   return 0;
}
//Case 3 :For range : format assumed as 2,256-276,
void case_range(string mystrings){
     printf("case_range \n");
     int pos_n = mystrings.find(',',1)+1;
     int pos_e = mystrings.find(',',pos_n+1);
     printf("pos_n:%d\n",pos_n);
     printf("pos_e:%d\n",pos_e);
     string myrange = mystrings.substr(pos_n,pos_e-pos_n);
     cout << myrange << endl;
     int pos_range_d = myrange.find('-',0);
     printf("pos_range_d : %d\n",pos_range_d);
     unsigned int myfromrange = atoi(myrange.substr(0,pos_range_d).c_str());
     unsigned int mytorange = atoi(myrange.substr(pos_range_d+1).c_str());
     printf("from:%d to:%d\n", myfromrange,mytorange);
     unsigned int diff = mytorange-myfromrange;
     printf("total : %d\n",diff);
     binary(myfromrange);
     fputc('x',wFile);//Only added for testing
     binary(diff);
     fputc('x',wFile);
}
//Case 2:For run of characters : format assumed as 3,32,C
void case_runofchar(string mystrings){
       printf("case_runofchar\n");
       string mynumber;
       int pos_n = mystrings.find(',',1)+1;
       int pos_e = mystrings.find(',',pos_n+1);
       printf("pos_n:%d\n",pos_n);
       printf("pos_e:%d\n",pos_e);
       mynumber = mystrings.substr(pos_n,pos_e-pos_n);
       cout<<mynumber<<endl;
       unsigned int number = atoi(mynumber.c_str());
       fputc('x',wFile);
       binary(number);
       char base = mystrings.c_str()[pos_e+1];
       fputc(base,wFile);//For testing
       //printf("%c",base);
       switch(toupper(base)){
       case 'A':
	 fprintf(wFile,"%s","00");
	 break;
       case 'C':
	 fprintf(wFile,"%s","01");
	 break;
       case 'T':
	 fprintf(wFile,"%s","10");
	 break;
       case 'G':
	 fprintf(wFile,"%s","11");
	 break;
       }
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

