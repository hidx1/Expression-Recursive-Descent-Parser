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
double Expression(char charDepan);
double Term();
double Factor();
double Real();
double Pangkat();
char peek();


int main()
{
	// ungetc('',stdin);
	// printf("%c",getchar());
	printf("Masukkan kalkulasi yang ingin dilakukan (tanpa spasi): ");
	double hasil = Expression('f');
	if(hasil < -FLT_MAX || hasil > FLT_MAX)
	{
		printf("MATH ERROR\n");
	}
	else if (hasil != hasil)
	{
		printf("MATH ERROR\n");
	}
	else
	{
		printf("Hasil = %.20f\n",hasil);
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

int isInteger(float value)
{
	return(abs(round(value)-value) < 0.000001);
}

boolean isOperator(char karakter)
{
	return (isPlusMin(karakter) || isTimesDiv(karakter) || karakter=='^');
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
				printf("Math error, division by zero!");
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

//Pangka		:= Factor { ( "^" ) Factor}
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
	// printf("%c\n",peek());
	// if ((peek())=='-')
	// {
	// 	isNegative=true;
	// 	getchar();
	// }
	if (isDigit(peek()))
	{
		factorVal = Real();
	}
	else
	{
		if (peek()!='(')
		{
			printf("SYNTAX ERROR! Expected number or (, instead got: %c",peek());
			exit(-1);
		}
		getchar();
		factorVal=Expression('(');
		if (peek()!=')')
		{
			printf("SYNTAX ERROR! Expected ), instead got: %c",peek());
			exit(-1);
		}
		else
		{
			getchar();
		}
	}
	// if (isNegative)
	// {
	// 	factorVal*=-1;
	// }
	return factorVal;
}

double Real()
{
	double realVal=0.0;
	while (isDigit(peek()))
	{
		int a = digit(getchar());
		realVal = realVal * 10 + (double) a;
		// printf("%.20Lf aa\n",(double) a);
		// printf("%.20Lf\n",realVal);
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
