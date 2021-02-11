#include "Header.h"

unsigned int Match(unsigned char *, int);
void ToBin(unsigned char *, unsigned char *, int, int);
int ReadFile(unsigned char * , FILE *, int);
int LoadCodeTable();

int Decompression()
{
	/*Dekompresja pliku:
	wczytaj znaki do bufora
	sprawdzaj dopasowanie dla bufor[1...i]
	jeżeli dopasowanie - przesuń wskaźnik o i, powtarzaj dopoki i<=bufor.size()
	sprawdz czy caly bufor zostal przetworzony, jezeli nie to przepis koncowke na poczatek
	...*/
	ChangeExt("", 0);

	if(!testFlag)
		if(LoadCodeTable())
		{
			printf("Błąd przy ładowaniu tabeli kodu ");
			if(pathToTableFile[0]) printf("ze ścieżki %s",pathToTableFile);
			else printf("z pliku %s.tabelaKodu",fileNameWithoutExt);
			printf(". Wyjście z programu...\n");
			return 1;
		}

	FILE * fileIn, * fileOut;
	fileIn=ChangeExt(".huffman", 5);
	char temporary[8];
	if(testFlag)
		fileOut=ChangeExt(strcat(strcpy(temporary,"T."),ext), 2);
	else
		fileOut=ChangeExt(strcat(strcpy(temporary,"."),ext), 2);
	if(!fileIn)
	{
		printf("Błąd przy otwieraniu skompresowanego pliku wejściowego. Wyjście z programu...\n");
		return 1;
	}

	long long int wroteBytes = 0, readBytes = 0;
	int i=1, offset=0, readChars, loopCond;
	unsigned char data[32], bufor[512], temp[256];
	unsigned int charToWrite;
	memset(data, 0, 32); memset(bufor, 0, 512);
	while(wroteBytes != inFileBytes)
	{
		readChars = ReadFile(data, fileIn, readBytes);
		readBytes+=readChars;
		ToBin(data, bufor, readChars, i-1-offset);
		loopCond = i - 1 - offset + readChars*8;
		offset = 0;
		for(i=1; i<=loopCond; i++)
		{
			charToWrite = Match(bufor+offset, i-offset);
			if(charToWrite!=256)
			{
				fwrite(&charToWrite, 1, 1, fileOut);
				wroteBytes++;
				offset = i;
				if(wroteBytes==inFileBytes)
					break;
			}
		}

		if(offset!=(i-1))
		{
			memset(temp, 0, 256);
			strcpy(temp, bufor + offset);
			memset(bufor, 0, 512);
			strcpy(bufor, temp);
		}
		else
			memset(bufor, 0, 512);

		memset(data, 0, 32);
	}
	if(testFlag) printf("Dekompresja pliku %s.huffman zakończona. Zapisano w pliku %sT.%s\n",fileNameWithoutExt, fileNameWithoutExt, ext);
	else printf("Dekompresja pliku %s.huffman zakończona. Zapisano w pliku %s.%s\n",fileNameWithoutExt, fileNameWithoutExt, ext);
	fclose(fileIn); fclose(fileOut);
}

int LoadCodeTable()
{
	//Załaduj tablicę kodową i wymagane informację do pamięci
	FILE * file; int i=0;
	if(pathToTableFile[0])
		file = fopen(pathToTableFile, "r");
	else
		file = ChangeExt(".tabelaKodu", 4);
	if(!file) return 1;
	fscanf(file, "%lld %lld %s\n",&inFileBytes, &outFileBytes, &ext);
	while(!feof(file))
	{
		fscanf(file, "%d %s\n",&symbols[i].number, &symbols[i].code);
		symbols[i].codeSize = strlen(symbols[i].code);
		i++;
	}
	fclose(file);
	return 0;
}

int ReadFile(unsigned char * data, FILE* file, int ReadBytes)
{
	/*Możliwa sekwencja składająca się z samych zer, ustawiona w ciągu tak, że tworzy 
	bajt o wartości 0. Standardowa funkcja czytająca wtedy zawodzi, potrzebna alternatywa:
	Jeżeli nie przeczytano 32 znaków (rozmiaru bufora) to sprawdź czy wskaźnik znajduję się 
	w końcówce pliku (po warunku rozmiaru pliku skompresowanego).
	Jeżeli nie to podejmij dodatkowe działania - wymuś doczytanie odpowiedniej ilości bajtów.*/
	int k, j, i = fread(data, 1, 32, file);
	if(i==32 || (outFileBytes-i)/32==0)
		return i;
	else
	{
		if(outFileBytes-ReadBytes <= 32-i)
			j = outFileBytes - ReadBytes;
		else
			j = 32 - i;
		data[i] = 0;
		for(k=1; k<=j; k++ )
			data[k+i] = getc(file);
		return j+i;
	}
}


unsigned int Match(unsigned char * string, int size)
{
	/*Sprawdz dopasowanie zadanego ciagu do ciągów kodowych.
	Jeżeli pasuje - zwróć kod znaku, jeżeli nie - zwróć 0.*/
	int i;
	for(i=0; i<256; i++)
		if(symbols[i].codeSize==size && !strncmp(symbols[i].code, string, size))
			return symbols[i].number;
	return 256;	
}

void ToBin(unsigned char * data , unsigned char * bufor,int size, int indexOffset)
{
	/*Przekonwertuj zadany ciąg znaków na postać binarną.
	Zapisz w odpowiednich miejscach w buforze*/
	int i, j, temp;
	for(i = 0; i<size; i++)
    {
    	temp = data[i];
	    for(j = 0; j<8;j++)
	    {
	        if(temp%2==0)
	            bufor[indexOffset + 8*i + 7-j]='0';
	        else
	            bufor[indexOffset + 8*i + 7-j]='1';
	        temp /=2;
	    }
    }
}