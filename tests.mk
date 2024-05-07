include def.mk

UTESTDIR = unit-tests

.PHONY: test-all

# Rodar todos os testes individuais
test-all:
	@echo hi

# Compilar um teste unitário
testc-%: $(OBJS) $(HEADERS)
	@echo compiling $(UTESTDIR)/$*.c $(OBJS)

# Executar um teste unitário
test-%: testc-%
	# sh $(UTESTDIR)/$*

# Teste com os casos de teste do trabalho
test-full-%:

# Executar o valgrind para verificar memory leakage
test-valgnd-%: 