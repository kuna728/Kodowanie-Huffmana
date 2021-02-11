#./bin/bash
gcc Huffman.c -o huffman -lm -w
chmod +x huffman
./huffman -t example.png > /dev/null

diff example.png exampleT.png > /dev/null
if [ $? -eq 0 ]; then
	echo test wykonany prawidlowo, program dziala POPRAWNIE
else
	echo blad, program dziala NIEPRAWIDLOWO
fi;
rm exampleT.png
rm example.tabelaKodu
rm example.huffman