/*
    util.c
    Implementação das rotinas auxiliares. A impressão respeita a representação
    de valores nulos adotada no arquivo: -1 para os inteiros e '$' para a
    unidade de medida.
*/

#include "util.h"
#include "registro.h"

#include <stdio.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ 
    Impressão de registros                                            
   ------------------------------------------------------------------ */

/* Imprime os quatro campos de dados do registro separados por espaço e
   encerrados por quebra de linha. Registros logicamente removidos são 
   descartados aqui. idPoPs e idPoPsConectado nunca são nulos, enquanto que 
   velocidade e unidadeMedida podem ser nulos, e nesses casos saem como NULO. */

int ImprimirRegistros(Registro *registro) {
    if (registro == NULL || registro->removido == REMOVIDO) return 0;

    printf("%d %d ", registro->idPoPs, registro->idPoPsConectado);

    if (registro->velocidade == INT_NULO) printf("NULO ");
    else printf("%d ", registro->velocidade);

    if (registro->unidadeMedida == LIXO) printf("NULO\n");
    else printf("\"%c\"\n", registro->unidadeMedida);

    return 1;
}
