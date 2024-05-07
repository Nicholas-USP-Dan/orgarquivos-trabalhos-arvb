include def.mk

UTESTDIR=unit-tests
CASOSDIR=test-cases

.PHONY: test-all $(CASOSDIR)/*

# Rodar todos os testes individuais
test-all:
	@echo hi

# Compilar um teste unitário
testc-%: $(OBJS) $(HEADERS)
	@echo compiling $(UTESTDIR)/$*.c $(OBJS)

# Executar um teste unitário
test-%: testc-%
	# sh $(UTESTDIR)/$*

# Teste com os casos de teste
test-cases-trab-int: $(BINDIR)/trab-int
	@for input in $(wildcard $(CASOSDIR)/trab-int/in/*) ; do \
		./$< < $$input > temp.out ; \
		cmp $(patsubst $(CASOSDIR)/trab-int/in/%.in, $(CASOSDIR)/trab-int/out/%.out, $$input) temp.out ; \
	done

	# Comparação de binários

	rm temp.out *.bin

# Executar o valgrind para verificar memory leakage
test-valgnd-%: $(CASOSDIR)/%/*