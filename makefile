all: *.c *.h
	gcc -o programaTrab *.c -lm
run: all
	./programaTrab