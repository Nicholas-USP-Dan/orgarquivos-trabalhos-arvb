include def.mk

.PHONY: all run compile submit-* *.c *.h

# Compilar todos os executáveis
all: $(OBJDIR) $(OBJDIR)/utils $(BINDIR) $(BINS)

# Gerar os object files (.o) de todos os arquivos .c em src/
compile: $(OBJDIR) $(OBJDIR)/utils $(OBJS)

# Gerar a pasta compactada para envio no run-codes
submit-%: $(SUBMITDIR)/%.zip
	@echo $<

# Limpar todos os arquivos binários e .o
clean:
	rm -r $(OBJDIR)/ $(BINDIR)/

setup: $(OBJDIR) $(BINDIR)

$(OBJDIR):
	mkdir $@/

$(OBJDIR)/utils:
	mkdir $@

$(BINDIR):
	mkdir $@/

# Gerar um object file
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	gcc $(FLAGS) -c -o $@ $<

# Gerar um executável
$(BINDIR)/%: %.c $(OBJS) $(HEADERS)
	gcc $(FLAGS) -o $@ $< $(OBJS)

# Gerar uma pasta compactada para envio
$(SUBMITDIR)/%.zip: %.c $(SRCS)
	zip -r $@ src/ $(SUBMITDIR)/makefile-entrega $< README.md 
#	Renomear o makefile
	printf "@ $(SUBMITDIR)/makefile-entrega\n@=makefile\n" | zipnote -w $@

include tests.mk