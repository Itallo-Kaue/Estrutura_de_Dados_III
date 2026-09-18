# Makefile
# Compilação do trabalho de Estrutura de Dados III.

CC      = gcc
CFLAGS  = -std=c99 -Wall -Wextra -g
EXEC    = programaTrabalho

# Um objeto por módulo do programa.
SRC     = main.c funcionalidades.c csv.c registro.c busca.c pilha.c util.c
OBJ     = $(SRC:.c=.o)

# Alvo padrão: gera o executável.
all: $(EXEC)

# Ligação final de todos os objetos.
$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ)

# Regra genérica de compilação de cada .c em .o.
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Dependências de cabeçalho: um .o é refeito quando um .h que ele usa muda.
main.o:           funcionalidades.h
funcionalidades.o: funcionalidades.h busca.h csv.h pilha.h registro.h util.h fornecidas.c
csv.o:            csv.h registro.h
registro.o:       registro.h
busca.o:          busca.h registro.h util.h
pilha.o:          pilha.h registro.h
util.o:           util.h registro.h

# Executa o programa.
run: $(EXEC)
	./$(EXEC)

# Executa sob o valgrind, para conferir vazamentos e acessos inválidos.
valgrind: $(EXEC)
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXEC)

# Remove os arquivos gerados pela compilação.
clean:
	rm -f $(OBJ) $(EXEC)

.PHONY: all run valgrind clean
