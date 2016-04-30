CFLAGS=-Wall -g

clean:
	rm -f bin
	mkdir bin

all:
	make ex1
	make ex3
	make ex4
	make ex5
	make ex6
	make ex7
	make ex8
	make ex9
	make ex10
	make ex11
	make ex12
	make ex13
	make ex14
	find . -maxdepth 1 -not -name "*.c*" -not -name "*.dSYM" -name "ex*" | xargs -I file mv -f -v file bin/file
