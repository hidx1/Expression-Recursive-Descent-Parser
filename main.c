// Expression := [ "-" ] Term { ("+" | "-") Term }
// Term       := Factor { ( "*" | "/" ) Factor }
// Factor     := RealNumber | "(" Expression ")"
// RealNumber := Digit{Digit} | [Digit] "." {Digit}
// Digit      := "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "boolean.h"

int digit (char karakter);
boolean isDigit(char karakter);
boolean isPlusMin(char karakter);
boolean isTimesDiv(char karakter);
double Expression();
double Term();
double Factor();
double Real();
char peek();


int main()
{
	printf("Masukkan kalkulasi yang ingin dilakukan (tanpa spasi): ");
	double hasil = Expression();
	printf("Hasil = %f\n",hasil);
	return 0;
}

int digit (char karakter)
{
	return (int) (karakter - '0');
}

boolean isDigit(char karakter)
{
	int digit = (int) (karakter - '0');
	return (digit>=0) && (digit<=9);
}

boolean isPlusMin(char karakter)
{
	return (karakter == '+') || (karakter == '-');
}

boolean isTimesDiv(char karakter)
{
	return (karakter == '*') || (karakter == '/');
}

boolean isPow(char karakter)
{
	return (karakter == '^');
}

int isInteger(float value)
{
	return(abs(round(value)-value) < 0.000001);
}

// Expression := [ "-" ] Term { ("+" | "-") Term }
double Expression()
{
	double exprValue;
    char nextChar = peek();
    boolean isNegative = false;
    if (nextChar == '-')	//consume
    {
    	isNegative = true;
    	getchar();
    }
    exprValue = Term();
    if (isNegative)
    {
    	exprValue *=-1;
    }
    while (isPlusMin(peek()))
    {
    	char operator = getchar();
    	// printf("%c\n",getchar());
    	double nextTermValue = Term();
    	// printf("%f\n",nextTermValue);
    	if (operator == '-')
    	{
    		exprValue -= nextTermValue;
    	}
    	else
    	{
    		exprValue += nextTermValue;
    	}
    }
    return exprValue;
}

// Term := Factor { ( "*" | "/" ) Factor }
double Term()
{
    double termVal = Factor();
    while (isTimesDiv(peek()))
    {
        char operator = getchar();
        double nextFactor = Factor();
        if (operator == '*')
            termVal *= nextFactor;
        else
        	  termVal /= nextFactor;
    }
		while(isPow(peek()))
		{
			char operator = getchar();
			double nextFactor = Factor();
			if(operator == '^')
			    termVal = pow(termVal, nextFactor);
		}
    return termVal;
}

// Factor     := RealNumber | "(" Expression ")"
double Factor()
{
	double factorVal;
	if (isDigit(peek()))
	{
		factorVal = Real();
	}
	else
	{
		if (peek()!='(')
		{
			printf("Expected number or (, instead got: %c",peek());
			exit(-1);
		}
		getchar();
		factorVal=Expression();
		if (peek()!=')')
		{
			printf("Expected ), instead got: %c",peek());
			exit(-1);
		}
		else
		{
			getchar();
		}
	}
	return factorVal;
}

double Real()
{
	double realVal=0;
	while (isDigit(peek()))
	{
		realVal= realVal * 10 + digit(getchar());
	}
	int power = 1;
	if (peek()=='.')
	{
		getchar();
		while (isDigit(peek()))
		{
			realVal += digit(getchar())/pow(10,power);
			power++;
		}
	}
	return realVal;
}

char peek()
{
	char karakter;
	karakter = getchar();
	ungetc(karakter,stdin);
	return karakter;
}
