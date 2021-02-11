#include "Header.h"

int Compression()
{
    /*Kompresja pliku na podstawie tabeli kodowania: 
    wczytuj znaki z pliku IN
    konwertuj po bicie kody znakow na postac decymalna x
    jezeli pozycja bitu == 7, zapisz x do pliku
    ...
    Kody wczytanych znakow traktowane sa jako jeden ciag*/

    ChangeExt("", 1); 
    if(GenerateModel())
        return 1;
    if(TreeAndCode())
        return 1;

    FILE * fileIn, * fileOut;
    fileIn = fopen(fileName, "r");
    if(!fileIn) return 1;
    fileOut = ChangeExt(".huffman", 3);
    if(!fileOut) return 1;
    unsigned int charCode=0, i=0, j=0;
    int bitPosition = 0;
    unsigned char  charToWrite;
    while(fread(&charToWrite, 1, 1, fileIn))
    {
        i = 0, j=0;
		while(symbols[j].number!=charToWrite) //znajdz odpowiedni symbol
			j++;
        while(i<symbols[j].codeSize)
        {
            if(symbols[j].code[i]==49) charCode+=pow(2,7-bitPosition);
            if(bitPosition==7)
            {
                fwrite(&charCode,1,1,fileOut);
                charCode = 0;
                bitPosition= -1;
                outFileBytes++;
            }
            i++;
            bitPosition++;
        }
    }
    if(charCode!=0)
    {
        fwrite(&charCode,1,1,fileOut);
        outFileBytes++;
    }
    if(WriteCodeTable())
        return 1;
    float w;
    w = (float)outFileBytes/inFileBytes;
    printf("Kompresja pliku %s zakonczona pomyslnie. Zapisano w pliku %s.huffman.\nDo zdekompresowania pliku wymagana bedzie tabela kodu, zapisana w pliku %s.tabelaKodu\n", fileName,fileNameWithoutExt, fileNameWithoutExt);
    printf("---------------------------------------------------------------------------------------\n");
    printf("Statystyki kompresji ~~ outFile: %lldB, inFile: %lldB ~ wspolczynnik  %.2f % \n",outFileBytes,inFileBytes, w*100);
    fclose(fileIn);
    fclose(fileOut);
    return 0;
}