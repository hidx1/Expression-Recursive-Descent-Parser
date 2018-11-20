// Expression := [ "-" ] Term { ("+" | "-") Term }
// Term       := Factor { ( "*" | "/" ) Factor }
// Factor     := RealNumber | "(" Expression ")"
// RealNumber := Digit{Digit} | [Digit] "." {Digit}
// Pangkat		:= Factor { ( "^" ) Factor}
// Digit      := "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include <unistd.h>
#include "boolean.h"
#include <complex.h>

// #define round8(var) ((round(var * 100000000))/100000000)
#define round8(var) (var)
// #define printf __mingw_printf

int digit (char karakter);
boolean isDigit(char karakter);
boolean isPlusMin(char karakter);
boolean isTimesDiv(char karakter);
long double complex Expression(char charDepan);
long double complex Term();
long double complex Factor();
long double complex Bilangan();
long double complex Pangkat();
char peek();


int main()
{
	// system("@cls||clear");
	// printf("  _____ ______ _____    _____      _            _       _             \n");
	// printf(" / ____|  ____/ ____|  / ____|    | |          | |     | |            \n");
	// sleep(1);
	// printf("| |    | |__ | |  __  | |     __ _| | ___ _   _| | __ _| |_ ___  _ __ \n");
	// printf("| |    |  __|| | |_ | | |    / _` | |/ __| | | | |/ _` | __/ _ \\| '__|\n");
	// sleep(1);
	// printf("| |____| |   | |__| | | |___| (_| | | (__| |_| | | (_| | || (_) | |   \n");
	// printf(" \\_____|_|    \\_____|  \\_____\\__,_|_|\\___|\\__,_|_|\\__,_|\\__\\___/|_|   \n");
	// sleep(1.5);
	// printf("\n");

	printf("Masukkan kalkulasi yang ingin dilakukan (tanpa spasi): \n");
	printf(">>> ");
	long double complex hasil = Expression('f');
	// if(hasil < -FLT_MAX || hasil > FLT_MAX)
	// { //Jika hasil infinite
	// 	printf("MATH ERROR\n");
	// } else if (hasil != hasil){ //Jika hasil NaN
	// 	printf("MATH ERROR\n");
	// }
	// else
	{
		printf("Hasil = %.20Lf %+.20Lfi\n",creal(hasil),cimag(hasil));
	}
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

boolean isOperator(char karakter)
{
	return (isPlusMin(karakter) || isTimesDiv(karakter) || isPow(karakter));
}

// Expression := [ "-" ] Term { ("+" | "-") Term }
long double complex Expression(char charDepan)
{
	long double complex exprValue;
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
    	long double complex nextTermValue = Term();
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
	if (charDepan=='f' && peek()!='\n')
	{
		printf("SYNTAX ERROR! Unexpected char!\n");
		exit(-1);
	}
    return round8(exprValue);
}

// Term := Pangkat { ( "*" | "/" ) Pangkat }
long double complex Term()
{
    long double complex termVal = Pangkat();
    while (isTimesDiv(peek()))
    {
        char operator = getchar();
        long double complex nextFactor = Pangkat();
        if (operator == '*')
		{
			termVal *= nextFactor;
		}
        else
		{
			if (nextFactor==0)
			{
				printf("Math error, division by zero!\n");
				exit(-1);
			}
			else
			{
				termVal /= nextFactor;
			}
		}
    }
    return round8(termVal);
}

//Pangka		:= Factor { ( "^" ) Factor}
long double complex Pangkat()
{
	long double complex base = Factor();
	while(isPow(peek()))
	{
		char operator = getchar();
		long double complex hasilNext = Pangkat();
		if(operator == '^')
		{
			base = cpow(base, hasilNext);
		}
	}
	return round8(base);
}

// Factor     := RealNumber | "(" Expression ")"
long double complex Factor()
{
	long double complex factorVal;
	if (isDigit(peek()) || peek()=='i')
	{
		factorVal = Bilangan();
	}
	else
	{
		if (peek()!='(')
		{
			printf("SYNTAX ERROR! Expected number or (, instead got: %c\n",peek());
			exit(-1);
		}
		getchar();
		factorVal=Expression('(');
		if (peek()!=')')
		{
			printf("SYNTAX ERROR! Expected ), instead got: %c\n",peek());
			exit(-1);
		}
		else
		{
			getchar();
		}
	}
	// printf("%.20Lfaa\n",cimag(factorVal));
	return round8(factorVal);
}

long double complex Bilangan()
{
	long double complex totalVal=0 + 0 * I;
	long double complex realVal=0 + 0 * I;
	if (peek()!='i')
	{
		while (isDigit(peek()))
		{
			realVal = realVal * 10 + digit(getchar());
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
		// printf("%c\n",peek());
		if (peek()=='i')
		{
			// printf("masuk\n");
			getchar();
			
			totalVal = realVal * I;
			// printf("%.20Lf\n",cimag(totalVal));
		}
		else
		{
			totalVal = realVal;
		}
	}
	else
	{
		getchar();
		totalVal = 1 * I;
	}
	return round8(totalVal);
}

char peek()
{
	char karakter;
	karakter = getchar();
	ungetc(karakter,stdin);
	return karakter;
}
