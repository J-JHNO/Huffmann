CC = gcc
SOURCE = Huffman.c
PROG = Huffman

all : $(PROG)

Huffman.o :  Huffman.h Huffman.c
	$(CC) -c Huffman.c

$(PROG) : $(SOURCE: .cpp=.o)
	$(CC) $(SOURCE: .cpp=.o) -o $(PROG)
