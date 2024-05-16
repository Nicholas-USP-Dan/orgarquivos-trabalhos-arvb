include def.mk

UTEST_DIR=unit-tests
UTEST_BIN_DIR=$(UTEST_DIR)/bin

UTESTS=$(wildcard $(UTEST_DIR)/*.c)
UTESTS_BINS=$(patsubst $(UTEST_DIR)/T-%.c, $(UTEST_BIN_DIR)/%, $(UTESTS))
# UTEST_EXS=$(addprefix test-,$(patsubst $(UTEST_DIR)/%.c, %, $(UTESTS)))

CASOSDIR=test-cases

.PHONY: test-all test-* test-cases-trab-int $(CASOSDIR)/*

# Rodar todos os testes individuais
test-all: $(UTESTS_BINS)

test-%: $(UTEST_BIN_DIR)/%
	./$(UTEST_BIN_DIR)/$*

t-valgrind-%: $(UTEST_BIN_DIR)/%
	valgrind -s ./$(UTEST_BIN_DIR)/$*

t-clean:
	rm -r $(UTEST_BIN_DIR)/* $(UTEST_DIR)/out/*

# Compilar um teste unitário
$(UTEST_BIN_DIR)/%: $(UTEST_DIR)/T-%.c $(OBJS) $(HEADERS)
	gcc $(FLAGS) -o $@ $< $(OBJS)