#include "Header.h"

void Init(list**);
void CodeChar(tree*, char *, int);
int WriteCodeTable();

int TreeAndCode()
{
	/*Funkcja tworzy drzewo kodowania a następnie na jego podstawie
	wyznacza rekurencyjnie symbole kodowe.
	Wydrukowanie tabeli kodowej opoznione do momentu obliczenia
	bajtow pliku wynikowego.*/
	FILE * Ftree, * Ftable; 
	list* head = NULL;
	Init(&head);
	int iterator = 0;
	if(logFlag) fprintf(stderr, "\n\nTworzenie drzewa kodowania: \n\n");
	while(ListSize(head)>1)
	{
		/*
		Sortuj -> zdejmij 2 wartosci ze "stosu"
		Utworz nowy symbol i odpowiedno go zainicjuj
		Utworz drzewo-korzen z nowym symbolem, jego dzieci to zdjete wartosci
		Dodaj drzewo-korzen do listy, powtarzaj dopoki List.size() > 1
		*/
		tree * root = NULL;
		BubbleSort(head);
		tree * llink = Pop(&head);
		tree * rlink = Pop(&head);
		symbol sm;
		sm.value = llink->sym.value + rlink->sym.value;
		sm.number = 256 + iterator;
		Insert(sm, &root);
		root->llink = llink;
		root->rlink = rlink;
		Push(&head, root);
		iterator++;
	}
	CodeChar(head->node,"", -1); //Drzewo huffmana jest w 1 (jedynym) elemencie listy

	if(logFlag)
	{
		Ftree=ChangeExt(".drzewo", 2);
		if(!Ftree) return 1;
		fprintf(stderr, "Num   |     Val    |   Kids    |   Code\n--------------------------------------------\n");
		PrintTree(head->node, Ftree);//PrintTree() dodatkowo wypelnia plik .drzewo - przekaz uchwyt
		fprintf(stderr, "\nDrzewo kodowania utworzone. Zapisano w pliku %s.drzewo\n", fileNameWithoutExt);
		fclose(Ftree);
	}

	return 0;

}

void CodeChar(tree* node, char * code, int size) 
{
	//Rekurencyjne wyznaczanie znaków kodowych
	char Lstr[255], Rstr[255];
	memset(Lstr, 0, 255); memset(Rstr, 0, 255);
	strcpy(Lstr, code);
	strcpy(Rstr, code);
	strcpy(node->sym.code, code); // node->sym.code = code;
	node->sym.codeSize = size+1;

	if(node->sym.number <256)
	{
		int i=0;
		while(symbols[i].number!=node->sym.number) //znajdz odpowiedni symbol
			i++;
		strncpy(symbols[i].code, node->sym.code, node->sym.codeSize); //symbols[i].code = node->sym.code;
		symbols[i].codeSize = node->sym.codeSize;
	}

	if(node->llink)
		CodeChar(node->llink, strcat(Lstr, "0"), size+1);
	if(node->rlink)
		CodeChar(node->rlink, strcat(Rstr, "1"), size+1);
}

void Init(list ** head) 
{
	//Inicjalizacja listy drzewami-korzeniami
	int i =0;
	for(i=0; i<256; i++)
	{
		if(symbols[i].value)
		{
			tree * root = NULL;
			Insert(symbols[i],&root);
			Push(head, root);
		}
	}
}


int WriteCodeTable()
{
	//Wydruk tabeli kodowania i innych kluczowych dla dekompresji informacji do pliku 
	int iterator = 0;
	FILE * Ftable=ChangeExt(".tabelaKodu", 2);
	if(!Ftable) 
	{
		printf("Błąd przy otwieraniu pliku do zapisu tabeli kodu. Wyjście z programu...\n");
		return 1;
	}
	fprintf(Ftable, "%lld  %lld %s\n", inFileBytes, outFileBytes, ext);
	for(iterator = 0; iterator <256; iterator++)
		if(symbols[iterator].value)
			fprintf(Ftable, "%d %s\n", symbols[iterator].number, symbols[iterator].code);

	fclose(Ftable);
	if(logFlag) fprintf(stderr, "\nTabela kodowania utworzona. Zapisano w pliku %s.tabelaKodu\n", fileNameWithoutExt);
	return 0;
}