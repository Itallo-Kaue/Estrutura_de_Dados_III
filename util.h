/*
    util.h
    Rotinas auxiliares compartilhadas pelas funcionalidades: impressão de um
    registro de dados na saída padrão.
*/

#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

#include "registro.h"

/* Imprime na saída padrão os campos de dados de um registro, em uma linha.
   Campos nulos são impressos como NULO.
   Retorna 1 se o registro foi impresso e 0 se ele estava logicamente
   removido, caso em que nada é escrito na saída. */
   
int ImprimirRegistros(Registro *registro);

#endif
