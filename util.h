/*
    util.h
    Rotinas auxiliares compartilhadas pelas funcionalidades: leitura de um
    valor da entrada padrão e impressão de um registro de dados.
*/

#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

#include "registro.h"

/* Lê da entrada padrão o próximo valor e o guarda em destino, já sem as
   aspas. Trata os três formatos aceitos na entrada:
     - um valor entre aspas ("G"), gravado sem as aspas;
     - a palavra NULO, gravada como string vazia;
     - um valor simples sem aspas (um inteiro, por exemplo).
   Retorna 1 em caso de sucesso e 0 se a entrada acabar antes. */
int LerValorEntrada(char *destino, int tamanho);

/* Imprime na saída padrão os campos de dados de um registro, em uma linha.
   Campos nulos são impressos como NULO.
   Retorna 1 se o registro foi impresso e 0 se ele estava logicamente
   removido, caso em que nada é escrito na saída. */
int ImprimirRegistros(Registro *registro);

#endif
