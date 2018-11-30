assembler: main.o list_char.o utility.o
	gcc -o assembler main.o list_char.o utility.o

main.o: main.c list_char.h utility.h
	gcc -c main.c

list_char.o: list_char.c list_char.h
	gcc -c list_char.c

utility.o: utility.c utility.h
	gcc -c utility.c

clean:
	rm *.o assembler