#include "Header.h"
#define dot 46

#define helpMessage " Huffman Coding\n kuna7@github.com/kuna728/Kodowanie-Huffmana\n" \
					" użycie: huffman <flags> <files>\n\n" \
					" Flagi:\n" \
					"  -c (default) --> kompresja\n" \
					"  -d --> dekompresja\n" \
					"  -l --> logi kompresji\n" \
					"  -t --> tryb testowy = kompresja -> dekompresja\n\n" \
					" W przypadku dekompresji można wskazać drugi plik - wcześniej wygenerowany plik .tabelaKodu." \
					"Domyślnie plik szukany jest w katalogu programu.\n\n" \
					" Przykłady użycia:\n" \
					"  huffman example.jpeg - kompresja pliku example.jpeg\n" \
					"  huffman -t -l example.jpeg - kompresja z logami pliku example.jpeg, następnie dekompresja\n" \
					"  huffman -d -l example.jpeg path/example.tabelaKodu - dekompresja z logami pliku example.jpeg " \
					"ze wskazaniem pliku z tabelą kodu\n" 

FILE * ChangeExt(char * newExt, int mode) 
{
	/*Funkcja zmienia rozszerzenie nazwy pliku (ciągu znaków przekazanego w pierwszym argumencie)
	i zwraca wskaźnik na otwarty plik w odpowiednim trybie, danym w drugim argumencie.
	Możliwe tryby:
	mode = 0 --> inicjalizacja zmiennej globalnej fileNameWithoutExt
	mode = 1 --> jak wyżej + inicjaizacja zmiennej globalnej ext
	mode = 2 --> tryb 'w'
	mode = 3 --> tryb 'w+b'
	mode = 4 --> tryb 'r'
	mode = 5 --> tryb 'r+b'*/
	int i=0, j=0;
	if(mode==1 || mode==0)
	{
		while(fileName[i]!=dot)  
		{						   
			fileNameWithoutExt[i]=fileName[i];
			i++;
		}
		if(mode==0) return;
	}
	if(mode==1)
	{
		i++;
		while(fileName[i]!=0)
		{
			ext[j] = fileName[i];
			i++; j++;
		}
		return;
	}
	char fileNameWithNewExt[23];
	strcpy(fileNameWithNewExt, fileNameWithoutExt);
	if(mode==2)return fopen(strcat(fileNameWithNewExt,newExt),"w"); 
	else if(mode==3)return fopen(strcat(fileNameWithNewExt,newExt),"w+b");
	else if(mode==4)return fopen(strcat(fileNameWithNewExt,newExt),"r");
	else if(mode==5) fopen(strcat(fileNameWithNewExt,newExt),"r+b");
}

int ProcessArgs(int argc, char ** argv)
{
	/*Funkcja przetwarza argumenty programu i podejmuje decyzje o dalszych działaniach.
	Plik nie może rozpoczynać się od znaku '-'.*/
	pathToTableFile[0] = 0;
	if(argc==2)
	{
		if(!strcmp(argv[1], "--help")) //huffman --help
		{
			printf(helpMessage);
			return -1;
		}
		else if(argv[1][0]=='-')
		{
			printf("Błędne argumenty. Spróbuj użyć huffman --help.\n");
			return -1;
		}
		else //huffman <file>
		{
			strcpy(fileName, argv[1]);
			return 2;
		}
	}
	else if(argc==3) //huffman <flag> <file>
	{
		strcpy(fileName, argv[2]);
		if(!strcmp(argv[1], "-l") && argv[2][0]!='-')
		{
			logFlag = 1;
			return 2;
		}
		else if(!strcmp(argv[1], "-c") && argv[2][0]!='-')
			return 2;
		else if(!strcmp(argv[1], "-d") && argv[2][0]!='-')
			return 1;
		else if(!strcmp(argv[1], "-t") && argv[2][0]!='-')
		{
			testFlag = 1;
			return 0;
		}
		else
		{
			printf("Błędne argumenty. Spróbuj użyć huffman --help.\n");
			return -1;
		}
	}
	else if(argc==4) // huffman <flag> <flag> <file>	||	huffman <flag> <file> <file>
	{
		if(!strcmp(argv[1], "-c") && !strcmp(argv[2], "-l") && argv[3][0]!='-' || !strcmp(argv[1], "-l") && !strcmp(argv[2], "-c") && argv[3][0]!='-')
		{
			strcpy(fileName, argv[3]);
			logFlag = 1;
			return 2;
		}
		else if(!strcmp(argv[1], "-d") && !strcmp(argv[2], "-l") && argv[3][0]!='-' || !strcmp(argv[1], "-l") && !strcmp(argv[2], "-d") && argv[3][0]!='-')
		{
			strcpy(fileName, argv[3]);
			logFlag = 1;
			return 1;	
		}
		else if(!strcmp(argv[1], "-t") && !strcmp(argv[2], "-l") && argv[3][0]!='-' || !strcmp(argv[1], "-l") && !strcmp(argv[2], "-t") && argv[3][0]!='-')
		{
			strcpy(fileName, argv[3]);
			logFlag = 1;
			return 0;	
		}
		else if(!strcmp(argv[1], "-d") && argv[2][0]!='-' && argv[3][0]!='-')
		{
			strcpy(fileName, argv[2]);
			strcpy(pathToTableFile, argv[3]);
			return 1;
		}
		else
		{
			printf("Błędne argumenty. Spróbuj użyć huffman --help.\n");
			return -1;
		}
	}
	else if(argc==5) // huffman <flag> <flag> <file> <file>
	{
		if(!strcmp(argv[1], "-d") && !strcmp(argv[2], "-l") && argv[3][0]!='-' && argv[4][0]!='-' 
			|| !strcmp(argv[1], "-l") && !strcmp(argv[2], "-d") && argv[3][0]!='-' && argv[4][0]!='-');
		{
			strcpy(fileName, argv[3]);
			strcpy(pathToTableFile, argv[4]);
			logFlag = 1;
			return 1;	
		}	
	}
	else
		printf("Użycie: huffman <flags> <files>. Pomoc: huffman --help\n");	
}

