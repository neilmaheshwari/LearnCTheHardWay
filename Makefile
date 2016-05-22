CFLAGS=-Wall -g

clean:
	rm -rf bin
	mkdir bin
	cd ex19; make clean; cd ..

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
	make ex15
	make ex16
	make ex17
	make ex18
	make stack
	find . -maxdepth 1 -not -name "*.sh" -not -name "*.c*" -not -name "*.dSYM" -name "ex*" -not -name "ex19" | xargs -I file mv -f -v file bin/file
	find . -maxdepth 1 -name "*.dSYM" | xargs -I file rm -rfv bin/file
	find . -maxdepth 1 -name "*.dSYM" | xargs -I file mv -f -v file bin
	mv -f -v stack bin/stack
	cd ex19; make all; cd ..
