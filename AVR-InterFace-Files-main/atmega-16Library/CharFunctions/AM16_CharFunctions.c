
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <math.h>
#include <string.h>



// Dtoa Turns Double into a string
void reverse(char* str)
{
int len=strlen(str);
int i = 0, j = len - 1, temp;
while (i < j) {
temp = str[i];
str[i] = str[j];
str[j] = temp;
i++;
j--;
}
}

int upperNum(double op){int Out=op;return Out;}	

// finds the number below the decimal point. Counts how many zeros are between decimal point and number. Returns Number as integer. 
long lowerNum( long double IN,int BACK){	
int COUNT2=0;
COUNT2-=1;
int Zero=0;
IN-=upperNum(IN);
long long R=IN*pow(10,5);
for(int POW=0; POW<5;IN*=10,POW++){Zero=IN; if(Zero==0){COUNT2++;}}
IN++;
if(R!=IN){IN+=1;}
if(BACK==1){return COUNT2;}
if(BACK==0){return R;}
	return 0;
}

// Clear Zero's  Ex--(900->9)
void CLZer(long Num,char*str){ltoa(Num,str,10);reverse(str);long Nh=atol(str);ltoa(Nh,str,10);reverse(str);}
	
/*finds the number above the decimal  point on  turns into a string*/

	
/*Take in the double and move it around to the proper functions.*/
// combines the strings with the right number of zeros and the decimals

void AddDec(char*str1, char*str2, int ZeroNum){
char DEC[]={"."};
strcat(str1,DEC); 
char Zero[ZeroNum-1];
int i=0;
while(ZeroNum>i){Zero[i]='0';i++;}
strcat(str1,Zero);
strcat(str1,str2);
}


#if defined (LCD)
void LCD_Dtoa(double Num){
	char str3[10];
	char Holder1[10];
	char Holder2[6];
	long Up=lowerNum(Num,0);
	CLZer(Up,Holder2);
	itoa(upperNum(Num),Holder1,10);
	AddDec(Holder1,Holder2,lowerNum(Num,1));
	strcpy(str3,Holder1);
	LCD_String(str3);
}
#endif

void dtoa(double Num,char*str3){
	char Holder1[10];
	char Holder2[6];
	long Up=lowerNum(Num,0);
	CLZer(Up,Holder2);
	itoa(upperNum(Num),Holder1,10);
	AddDec(Holder1,Holder2,lowerNum(Num,1));
	strcpy(str3,Holder1);
}

void UpdateString( unsigned char*strData,unsigned char*Str){
const char* temp = strData;
	for(int i=0; i<= strlen(temp); i++){
		Str[i]=strData[i];
	}
}

