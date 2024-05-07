FLAGS=-g -Wall

SRCDIR=src
OBJDIR=obj
BINDIR=bin
SUBMITDIR=entregas

HEADERS=$(wildcard $(SRCDIR)/*.h) $(wildcard $(SRCDIR)/**/*.h)
SRCS=$(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/**/*.c)
OBJS=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))
PROGS=$(wildcard *.c)
BINS=$(patsubst %.c, $(BINDIR)/%, $(PROGS))

.PHONY: all run compile submit* *.c *.h

# Compilar todos os executáveis
all: $(BINS)

# Gerar os object files (.o) de todos os arquivos .c em src/
compile: $(OBJS)

# Gerar a pasta compactada para envio no run-codes
submit-%: $(SUBMITDIR)/%.zip

# Limpar todos os arquivos binários e .o
clean:
	rm $(wildcard $(OBJDIR)/*.o) $(wildcard $(BINDIR)/*) $(wildcard $(OBJDIR)/**/*.o)

# Gerar um object file
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	gcc $(FLAGS) -c -o $@ $^

# Gerar um executável
$(BINDIR)/%: %.c $(OBJS) $(HEADERS)
	gcc $(FLAGS) -o $@ $< $(OBJS)

# Gerar uma pasta compactada para envio
$(SUBMITDIR)/%.zip: %.c
	zip -r $@ src/ $(SUBMITDIR)/makefile-entrega $< README.md 
#	Renomear o makefile
	printf "@ $(SUBMITDIR)/makefile-entrega\n@=makefile\n" | zipnote -w $@