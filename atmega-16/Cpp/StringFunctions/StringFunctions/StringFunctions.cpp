#include "StringFunctions.h"


String::String (){};
String::String(const char* str){string = (char*)str;};
String::String(char* str){string = str;};


// Dtoa Turns Double into a string
void String::reverse(char* str)
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
void String::reverse(const char* str){
	reverse((char*)str);
}


int String::upperNum(double op){int Out=op;return Out;}

// finds the number below the decimal point. Counts how many zeros are between decimal point and number. Returns Number as integer.
long String::lowerNum( long double IN,int BACK){
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
void String::CLZer(long Num,char*str){ltoa(Num,str,10);reverse(str);long Nh=atol(str);ltoa(Nh,str,10);reverse(str);}

/*finds the number above the decimal  point on  turns into a string*/


/*Take in the double and move it around to the proper functions.*/
// combines the strings with the right number of zeros and the decimals

void String::AddDec(char*str1, char*str2, int ZeroNum){
	char DEC[]={"."};
	strcat(str1,DEC);
	char Zero[ZeroNum-1];
	int i=0;
	while(ZeroNum>i){Zero[i]='0';i++;}
	strcat(str1,Zero);
	strcat(str1,str2);
}


void String::dtoa(double Num,char*str3){
	char Holder1[10];
	char Holder2[6];
	long Up=lowerNum(Num,0);
	CLZer(Up,Holder2);
	itoa(upperNum(Num),Holder1,10);
	AddDec(Holder1,Holder2,lowerNum(Num,1));
	strcpy(str3,Holder1);
}
void String::dtoa(double Num,const char*str3){dtoa(Num,(char*)str3);}


void String::UpdateString(char *strData,unsigned char*Str){
	for(uint8_t i=0; i<= strlen(const_cast<char*>(strData)); i++){
		Str[i]=strData[i];
	}
}


