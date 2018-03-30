CC = gcc
CFLAGS= -g -Wall -pedantic -ansi -Werror -Wconversion -std=c11

.PHONY: all, clean #this names are not files

TARGET = genere-texte LA LAB LA_gprof

all: $(TARGET)

genere-texte : genere-texte.c
	$(CC) $(CFLAGS) $< -o $@

LA : LA_main.o LA.o
	$(CC) $(CFLAGS) $^ -o $@

LAB : LA_main.o LAB.o
	$(CC) $(CFLAGS) $^ -o $@

LA_gprof : LA_main.o LA_gprof.o
	gcc -Wall -pg $^ -o $@

LA_main.o : LA_main.c
	$(CC) $(CFLAGS) -D Prints -c $^ -o $@

LA.o : LA.c
	$(CC) $(CFLAGS) -D Prints -c $^ -o $@

LAB.o : LA.bak.c
	$(CC) $(CFLAGS) -D Prints -c $^ -o $@

LA_gprof.o : LA.c
	gcc -Wall -pg -c $< -o $@

clean:
	rm -f *.o *.s $(TARGET)
