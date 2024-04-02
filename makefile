all: *.c
	gcc -o programaTrab *.c -lmd
run: all
	./programaTrab