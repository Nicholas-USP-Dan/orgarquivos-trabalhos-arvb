include def.mk

UTEST_DIR=unit-tests
UTEST_BIN_DIR=$(UTESTDIR)/bin
CASOSDIR=test-cases

.PHONY: test-all $(CASOSDIR)/*

# Rodar todos os testes individuais
test-all:
	@echo hi

# Compilar um teste unitário
$(UTEST_BIN_DIR)/%: $(OBJS) $(HEADERS)
	@echo compiling $(UTEST_DIR)/$*.c $(OBJS)

# Executar um teste unitário
test-%: $(UTEST_BIN_DIR)/%
	# ./$(UTEST_DIR)/$*

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