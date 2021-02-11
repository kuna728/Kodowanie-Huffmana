#include "Header.h"
#include "Functions.c"
#include "DataStruct.c"
#include "Model.c"
#include "Code.c"
#include "Compress.c"
#include "Decompress.c"

int main(int argc, char** argv)
{
	int toDo = ProcessArgs(argc, argv);
	if(toDo==2)
		Compression();
	else if(toDo==1)
		Decompression();
	else if(toDo==0)
	{
		Compression();
		Decompression();
	}
	return 0;
}