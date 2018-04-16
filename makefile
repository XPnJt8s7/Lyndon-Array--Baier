CC = gcc
CFLAGS= -g -Wall -pedantic -ansi -Werror -Wconversion -std=c11

.PHONY: all, clean #this names are not files

TARGET = genere-texte LA LAB LA_gprof newLA LA_test newLA_test

all: $(TARGET)

genere-texte : genere-texte.c
	$(CC) $(CFLAGS) $< -o $@

LA : LA_main.o LA.o
	$(CC) $(CFLAGS) $^ -o $@

LA_test : LA_main.o LA_test.o
	$(CC) $(CFLAGS) $^ -o $@

LAB : LA_main.o LAB.o
	$(CC) $(CFLAGS) $^ -o $@

LA_gprof : LA_main.o LA_gprof.o
	gcc -Wall -pg $^ -o $@

newLA : LA_main.o LA.bak.5.o
	$(CC) $(CFLAGS) $^ -o $@

newLA_test : LA_main.o LA.bak.5_test.o
	$(CC) $(CFLAGS) $^ -o $@

LA.bak.5.o : LA.bak.5.c
	$(CC) $(CFLAGS) -D Prints -c $^ -o $@

LA.bak.5_test.o : LA.bak.5.c
	$(CC) $(CFLAGS) -c $^ -o $@

LA_main.o : LA_main.c
	$(CC) $(CFLAGS) -D Prints -c $^ -o $@

LA.o : LA.c
	$(CC) $(CFLAGS) -D Prints -c $^ -o $@

LA_test.o : LA.c
	$(CC) $(CFLAGS) -c $^ -o $@

LAB.o : LA.bak.c
	$(CC) $(CFLAGS) -D Prints -c $^ -o $@

LA_gprof.o : LA.c
	gcc -Wall -pg -c $< -o $@


clean:
	rm -f *.o *.s $(TARGET)
