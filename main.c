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

int digit (char karakter);
boolean isDigit(char karakter);
boolean isPlusMin(char karakter);
boolean isTimesDiv(char karakter);
double Expression(char charDepan);
double Term();
double Factor();
double Real();
double Pangkat();
char peek();


int main()
{
	system("@cls||clear");
	printf("  _____ ______ _____    _____      _            _       _             \n");
	printf(" / ____|  ____/ ____|  / ____|    | |          | |     | |            \n");
	sleep(1);
	printf("| |    | |__ | |  __  | |     __ _| | ___ _   _| | __ _| |_ ___  _ __ \n");
	printf("| |    |  __|| | |_ | | |    / _` | |/ __| | | | |/ _` | __/ _ \\| '__|\n");
	sleep(1);
	printf("| |____| |   | |__| | | |___| (_| | | (__| |_| | | (_| | || (_) | |   \n");
	printf(" \\_____|_|    \\_____|  \\_____\\__,_|_|\\___|\\__,_|_|\\__,_|\\__\\___/|_|   \n");
	sleep(1.5);
	printf("\n");

	printf("Masukkan kalkulasi yang ingin dilakukan (tanpa spasi): \n");
	printf(">>> ");
	double hasil = Expression('f');
	if(hasil < -FLT_MAX || hasil > FLT_MAX){ //Jika hasil infinite
		printf("MATH ERROR\n");
	} else if (hasil != hasil){ //Jika hasil NaN
		printf("MATH ERROR\n");
	} else{
		printf("Hasil = %.20f\n", hasil);
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
double Expression(char charDepan)
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
    	double nextTermValue = Term();
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
		printf("SYNTAX ERROR!\n");
		exit(-1);
	}
    return exprValue;
}

// Term := Pangkat { ( "*" | "/" ) Pangkat }
double Term()
{
    double termVal = Pangkat();
    while (isTimesDiv(peek()))
    {
        char operator = getchar();
        double nextFactor = Pangkat();
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
    return termVal;
}

//Pangkat		:= Factor { ( "^" ) Factor}
double Pangkat()
{
	double base = Factor();
	while(isPow(peek()))
	{
		char operator = getchar();
		double hasilNext = Pangkat();
		if(operator == '^')
		{
			base = pow(base, hasilNext);
		}
	}
	return base;
}

// Factor     := {-} RealNumber | "(" Expression ")"
double Factor()
{
	double factorVal;
	boolean isNegative=false;
	if (isDigit(peek()))
	{
		factorVal = Real();
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
	return factorVal;
}

double Real()
{
	double realVal=0.0;
	while (isDigit(peek()))
	{
		int a = digit(getchar());
		realVal = realVal * 10 + (double) a;
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
