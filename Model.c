#include "Header.h"

int Compare(const void * a, const void *b) //qsort
{
	symbol _a = *(symbol*)a;
	symbol _b = *(symbol*)b;
    if(_a.value < _b.value) return -1;
    else if(_a.value == _b.value) return 0;
    else return 1;
}

int GenerateModel()
{
	/*Funkcja tworzy model źródła informacji i go sortuję.
	W przypadku flagi log drukuję odpowiednie informację*/
	FILE *fileIn, *fileOut;
	unsigned char buffer; int i, j;
	for(i=0;i<=255;i++)//inicjalizacja struktury
	{
		symbols[i].number=i;
		symbols[i].value=0;
		symbols[i].codeSize=0;
	}
	fileIn=fopen(fileName,"r");
	if(!fileIn) 
	{
		printf("Błąd przy otwieraniu pliku wejściowego. Wyjście z programu...\n");
		return 1;
	}
	while(fread(&buffer, 1, 1, fileIn)) //policz ilości wystąpień znaków
	{
		inFileBytes++;
		symbols[buffer].value++;
	}
	fclose(fileIn);

	if(logFlag)
	{
		fileOut=ChangeExt(".model", 2);
		if(!fileOut) return 1;
		fprintf(stderr, "Tworzenie modelu zrodla informacji:\n\n");
		for(i = 0; i<=255; i++) 
			if(symbols[i].value)
			{
				fprintf(fileOut, "%d %lld\n",symbols[i].number,symbols[i].value);
				fprintf(stderr, "%d %lld\n",symbols[i].number,symbols[i].value);
			}
		fprintf(stderr, "\nModel utworzony. Zapisano w pliku %s.model\n\n\n\n",fileNameWithoutExt);
		fclose(fileOut);
	}

	qsort(symbols, 256, sizeof(symbol), Compare);

	if(logFlag)
	{
		fileOut=ChangeExt(".modelSort", 2);
		if(!fileOut) return 1;
		fprintf(stderr, "Tworzenie posortowanego modelu zrodla informacji:\n\n");
		for(i = 0; i<=255; i++) 
			if(symbols[i].value)
			{
				fprintf(fileOut, "%d %lld\n",symbols[i].number,symbols[i].value);
				fprintf(stderr, "%d %lld\n",symbols[i].number,symbols[i].value);
			}
		fprintf(stderr, "\nPosortowany model utworzony. Zapisano w pliku %s.modelSort\n\n",fileNameWithoutExt);
		fclose(fileOut);
	}
	return 0;
}