# Makefile
# Compilação do trabalho de Estrutura de Dados III.

CC      = gcc
CFLAGS  = -std=c99 -Wall -Wextra -g
EXEC    = programaTrabalho

SRC     = main.c funcionalidades.c csv.c registro.c busca.c pilha.c util.c fornecidas.c
OBJ     = $(SRC:.c=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

main.o:           funcionalidades.h
funcionalidades.o: funcionalidades.h busca.h csv.h fornecidas.h pilha.h registro.h util.h
csv.o:            csv.h registro.h
registro.o:       registro.h
busca.o:          busca.h fornecidas.h registro.h
pilha.o:          pilha.h registro.h
util.o:           util.h registro.h
fornecidas.o:


run: $(EXEC)
	./$(EXEC)

valgrind: $(EXEC)
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXEC)

# Remove os arquivos gerados pela compilação.
clean:
	rm -f $(OBJ) $(EXEC)

.PHONY: all run valgrind clean
