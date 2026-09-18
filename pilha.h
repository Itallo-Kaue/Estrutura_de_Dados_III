/*
    pilha.h
    Manipulação da pilha de registros logicamente removidos. A pilha não fica
    em memória: o topo é o campo topoPilha do cabeçalho e o encadeamento é o
    campo encadeamentoPilha de cada registro removido, ou seja, ela é uma lista
    encadeada gravada dentro do próprio arquivo de dados.
*/

#ifndef PILHA_H
#define PILHA_H

#include <stdio.h>

#include "registro.h"

/* Empilha o registro de RRN informado: marca o registro como removido, faz
   seu encadeamento apontar para o antigo topo e passa a ser o novo topo.
   Também incrementa o contador de registros removidos do cabeçalho.
   Os campos de dados devem ter sido preenchidos com lixo antes da chamada. */
void Empilhar(FILE *bin, Cabecalho *cab, int rrn);

/* Desempilha o registro do topo e devolve o seu RRN, para que o espaço seja
   reaproveitado em uma inserção. O novo topo passa a ser o encadeamento do
   registro retirado e o contador de removidos é decrementado.
   Retorna -1 quando a pilha está vazia. */
int Desempilhar(FILE *bin, Cabecalho *cab);

#endif
