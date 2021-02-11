#ifndef Header_h
#define Header_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <math.h>

typedef struct symbol
{
	unsigned int number;
	long long int value;
	char code[255]; //maksymalna wysokosc drzewa w skrajnym przypadku
	short codeSize;
}symbol;

//Zmienne globalne
symbol symbols[256];
long long int inFileBytes = 0 ;
long long int outFileBytes=0;
char fileName[30];
char fileNameWithoutExt[23]; //w calym programie: Ext - Extension - rozszerzenie
char ext[6];
char logFlag = 0, testFlag = 0; //bool
char pathToTableFile[34];

#endif
