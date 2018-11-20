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
long double complex round8(long double complex input);
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
	// double asal = 92233720368547758089 ;
	// long double y ;
	// scanf ("%Lf", &y ) ;
	// printf("%Lf",y);
	// printf("LDBL_MAX      = %Le\n", LDBL_MAX);
	printf("Hasil = %.20Lf %+.20Lfi\n",creall(cpowl(-1,0.5)),cimagl(cpowl(-1,0.5)));
	printf("Masukkan kalkulasi yang ingin dilakukan (tanpa spasi): \n");
	printf(">>> ");
	long double complex hasil = Expression('f');
	
	if(creall(hasil) < -LDBL_MAX || creall(hasil) > LDBL_MAX || cimagl(hasil) < -LDBL_MAX || cimagl(hasil) > LDBL_MAX)
	{ //Jika hasil infinite
		printf("MATH ERROR\n");
	}
	else if (hasil != hasil){ //Jika hasil NaN
		printf("MATH ERROR\n");
	}
	else
	{
		if (fabsl(cimagl(hasil)) < 0.0000000001)
		{
			printf("Hasil = %.20Lf\n",creall(hasil));
		}	
		else if (fabsl(creall(hasil)) < 0.0000000001 && fabsl(cimagl(hasil)) >= 0.0000000001)
		{
			printf("Hasil = %.20Lfi\n",cimagl(hasil));
		}
		else
		{
			printf("Hasil = %.20Lf %+.20Lfi\n",creall(hasil),cimagl(hasil));
		}	
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
		if (cimagl(exprValue)==0)
		{
			exprValue = creall(exprValue)*-1;
		}
		else
		{
			exprValue *= -1;
		}
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
			// printf("Hasil = %.20Lf %+.20Lfi\n",creall(base),cimagl(base));
			// printf("Hasil = %.20Lf %+.20Lfi\n",creall(hasilNext),cimagl(hasilNext));
			base = -1 + 0*I;
			// hasilNext = 0.5 + 0*I;
			base = cpowl(base, hasilNext);
			// base = cpowl(-1 , 0.8);
			// printf("Hasil = %.20Lf %+.20Lfi\n",creall(base),cimagl(base));
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
	// printf("%.20Lfaa\n",cimagl(factorVal));
	return round8(factorVal);
}
// 9999999999999999999

long double complex Bilangan()
{
	long double complex totalVal=0 + 0 * I;
	long double complex realVal= (long double complex)(0 + 0 * I);
	if (peek()!='i')
	{
		while (isDigit(peek()))
		{
			realVal = realVal * 10 + (long double complex) digit(getchar());
			// printf("%.20Lfcc\n",creall(realVal));
		}
		int power = 1;
		if (peek()=='.')
		{
			getchar();
			while (isDigit(peek()))
			{
				realVal += (long double complex) digit(getchar())/pow(10,power);
				power++;
			}
		}
		// printf("%c\n",peek());
		if (peek()=='i')
		{
			// printf("masuk\n");
			getchar();
			// printf("%.20Lfaa\n",creall(realVal));
			totalVal = realVal * I;
			// printf("%.20Lfbb\n",cimagl(totalVal));
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
	// printf("%.20Lfbb\n",creall(totalVal));
	return round8(totalVal);
}
long double complex round8(long double complex input)
{
	// return ((round(creall(input) * 100000000))/100000000) + ((round(cimagl(input) * 100000000))/100000000)*I;
	return (input);
}

char peek()
{
	char karakter;
	karakter = getchar();
	ungetc(karakter,stdin);
	return karakter;
}
