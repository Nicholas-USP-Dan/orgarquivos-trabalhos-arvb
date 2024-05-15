include def.mk

FLAGS=-ggdb3 -Wall -Werror

UTEST_DIR=unit-tests
UTEST_BIN_DIR=$(UTEST_DIR)/bin

UTESTS=$(wildcard $(UTEST_DIR)/*.c)
UTESTS_BINS=$(patsubst $(UTEST_DIR)/%.c, $(UTEST_BIN_DIR)/%, $(UTESTS))
# UTEST_EXS=$(addprefix test-,$(patsubst $(UTEST_DIR)/%.c, %, $(UTESTS)))

CASOSDIR=test-cases

.PHONY: test-all test-* test-cases-trab-int $(CASOSDIR)/*

# Rodar todos os testes individuais
test-all: $(UTESTS_BINS)

test-%: $(UTEST_BIN_DIR)/%
	./$(UTEST_BIN_DIR)/$*

t-valgrind-%: $(UTEST_BIN_DIR)/%
	valgrind -s ./$(UTEST_BIN_DIR)/$*

t-clear:
	rm -r $(UTEST_BIN_DIR)/*

# Teste com os casos de teste
test-cases-trab-int: $(BINDIR)/trab-int
	@for input in $(wildcard $(CASOSDIR)/trab-int/in/*) ; do \
		./$< < $$input > temp.out ; \
		cmp $(patsubst $(CASOSDIR)/trab-int/in/%.in, $(CASOSDIR)/trab-int/out/%.out, $$input) temp.out ; \
	done

	# Comparação de binários

	rm temp.out *.bin

# Compilar um teste unitário
$(UTEST_BIN_DIR)/%: $(UTEST_DIR)/%.c $(OBJS) $(HEADERS)
	gcc $(FLAGS) -o $@ $(UTEST_DIR)/$*.c $(OBJS)