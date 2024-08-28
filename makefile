all: tp2virtual

tp2virtual: tp2virtual memory_manager.o hash.o base_converter.o file.o
	gcc tp2virtual.c memory_manager.o hash.o base_converter.o file.o -lm -o tp2virtual

memory_manager.o: memory_manager.c
	gcc -c memory_manager.c

hash.o: hash.c
	gcc -c hash.c

base_converter.o: base_converter.c
	gcc -c base_converter.c

file.o: file.c
	gcc -c file.c

clean:
	rm memory_manager.o
	rm base_converter.o
	rm hash.o
	rm file.o
	rm tp2virtual