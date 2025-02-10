#ifndef StringFunc
#define StringFunc

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

class String {
public:
String ();
String(const char* str);
String(char* str);
/*
* converts a Double into a String
* @param Num - The value being converted
* @param str3 - The buffer were the Value is stored 
*/
void dtoa(double Num,char*str3);
void dtoa(double Num,const char*str3);
/*
* copies a string onto another string
* @param strData - String to be copied
* @param Str - String to be rewritten
*/
void UpdateString(char *strData,unsigned char*Str);
/*
* Reverses the elements in a Array
* @param Str - String to be reversed
*/
void reverse(char* str);	
void reverse(const char* str);
private:
char* string; 
/*
* Returns the integer part of a double
* @param op - double to be split
* @return - Integer 
*/
int upperNum(double op);
/*
* Returns the lower part of a double
* @param IN - double to be split
* @param back - determines the return value
* @return - decimal part as integer
*/
long lowerNum( long double IN,int BACK);
/*
* Clears the zeros in a string
* @param Num- number of zeros
* @param Str- Str to be cleared
*/
void CLZer(long Num,char*str);
/*
*combines the strings with the right number of zeros and the decimals for dota
* @param str1 - string for upper part of double
* @param str2 - string for lower part of double
* @param ZeroNum - Number of Zeros to be cleared in CLZer
*/
void AddDec(char*str1, char*str2, int ZeroNum);
};
 
#endif 