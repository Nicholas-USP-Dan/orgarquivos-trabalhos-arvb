all: *.c *.h
	gcc -o programaTrab *.c -lmd
run: all
	./programaTrab