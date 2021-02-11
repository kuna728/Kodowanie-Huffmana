/*Program korzysta z listy jednokierunkowej oraz dzewa binarnego przy
tworzeniu drzewa kodowania oraz tabeli kodowej. W pliku tym zaimplementowane
zostały struktury obsługujące operację na tych strukturach.*/

#include <math.h>

struct symbol;

typedef struct tree
{
	struct symbol sym;
	struct tree * rlink;
	struct tree * llink;
}tree;

typedef struct list
{
	tree * node;
	struct list* next;
}list;


/*Schemat działania listy:
	dodanie elementów do listy
	sortowanie
	usun 2 elementy z brzega, bedace najmniejszymi wartosciami w liscie
	dodaj nowy element
	sortowanie
	...*/

void Push(list ** head, tree * node) 
{
	//Dodanie na koniec listy
    list * newEl = (list*)malloc(sizeof(list));
    newEl->node = node;
    newEl->next = NULL;
    list * temp = *head;
    if(temp)
    {
    	while (temp->next)
        	temp = temp->next;
        temp->next=newEl;
    }
    else
    	*head = newEl;

}

tree * Pop(list ** head) 
{
	//Usunięcie z początku listy
	if(*head)
	{
	    tree * tempTree = (*head)->node;
	    list * tempList = (*head)->next;
		free(*head);
		*head = tempList;
		return tempTree;
	}
}

int ListSize(list *head) 
{
	//Funkcja zwraca ilość elementów na liście
	if(!head)
		return 0;
	int i=1;
	while(head->next)
	{
		head=head->next;
		i++;
	}
	return i;

}

void BubbleSort(list * head) 
{
	//Sortowanie listy metodą bąbęlkową 
	int i, n=ListSize(head);
	list * temp = head;
	tree * tr;
	do
	{
		for(i=0; i<n-1; i++)
		{
			if(temp->node->sym.value > temp->next->node->sym.value )
			{
				tr = temp->node;
				temp->node = temp->next->node;
				temp->next->node = tr;
			}
			temp = temp->next;
		}
		temp = head;
		n--;
	}while(n>1);
}


/*Do obsługi drzewa wystarczy procedura tworząca korzeń i 
procedura przeglądająca drzewo (preorder).
Procedura przeglądająca formatuje odpowiednio tekst.*/

void Insert(struct symbol value, tree** root) 
{
	//Utworzenie korzenia
    *root = (tree*)malloc(sizeof(tree));
    (*root)->llink = NULL;
    (*root)->rlink = NULL;
    (*root)->sym = value;
}
void PrintTree(tree * root, FILE * file) 
{
	//Drukowanie elementów drzewa, metodą preorder, z odpowiednim formatowaniem
	if(root)
	{
		fprintf(stderr, "%.3d   | %.10lld | ", root->sym.number, root->sym.value);

		if(root->llink)
			fprintf(stderr, "%.3d   ",root->llink->sym.number);
		else
			fprintf(stderr, "NULL ");
		if(root->rlink)
			fprintf(stderr, "%.3d |",root->rlink->sym.number);
		else
			fprintf(stderr, "NULL |");

		if(root->sym.number<256)
			fprintf(stderr, "   %s   \n",root->sym.code);
		else
			fprintf(stderr, "   -   \n");

		if(root->sym.number >= 256) //drukowanie do pliku .drzewo
			fprintf(file, "%d %d %d\n",root->sym.number, root->llink->sym.number, root->rlink->sym.number );

		PrintTree(root->llink, file);
		PrintTree(root->rlink, file);
	}
}



