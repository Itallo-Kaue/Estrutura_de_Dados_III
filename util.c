/*
    util.c
    Implementação das rotinas auxiliares. A leitura de valores concentra num
    único lugar o tratamento de aspas e da palavra NULO, e a impressão respeita
    a representação de valores nulos adotada no arquivo: -1 para os inteiros e
    '$' para a unidade de medida.
*/
 
#include "util.h"
#include "registro.h"
 
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
/* ------------------------------------------------------------------ */
/* Leitura de valores da entrada                                      */
/* ------------------------------------------------------------------ */
 
/* Um valor nulo, escrito como NULO na entrada, é representado internamente
   por uma string vazia; quem chama converte para -1 ou '$' conforme o campo. */
int LerValorEntrada(char *destino, int tamanho) {
    int c;
 
    /* Ignora espaços, tabulações e quebras de linha antes do valor. */
    while ((c = getchar()) != EOF && isspace(c))
        ;
 
    if (c == EOF) {
        destino[0] = '\0';
        return 0;
    }
 
    if (c == '"') {
        /* Valor entre aspas: copia tudo até a aspa de fechamento. */
        int i = 0;
        while ((c = getchar()) != EOF && c != '"') {
            if (i < tamanho - 1) destino[i++] = (char)c;
        }
        destino[i] = '\0';
        return 1;
    }
 
    /* Valor sem aspas: copia até o próximo espaço em branco. */
    {
        int i = 0;
        destino[i++] = (char)c;
        while ((c = getchar()) != EOF && !isspace(c)) {
            if (i < tamanho - 1) destino[i++] = (char)c;
        }
        destino[i] = '\0';
    }
 
    /* A palavra NULO representa ausência de valor e vira string vazia. */
    if (strcmp(destino, "NULO") == 0) destino[0] = '\0';
 
    return 1;
}
 
/* ------------------------------------------------------------------ */
/* Impressão de registros                                             */
/* ------------------------------------------------------------------ */
 
/* Imprime os quatro campos de dados do registro separados por espaço e
   encerrados por quebra de linha. Registros logicamente removidos não fazem
   parte da saída de nenhuma funcionalidade, por isso são descartados aqui.
   idPoPs e idPoPsConectado nunca são nulos; velocidade e unidadeMedida podem
   ser, e nesse caso saem como NULO. */
int ImprimirRegistros(Registro *registro) {
    if (registro == NULL || registro->removido == REMOVIDO) return 0;
 
    printf("%d %d ", registro->idPoPs, registro->idPoPsConectado);
 
    if (registro->velocidade == INT_NULO) printf("NULO ");
    else                                  printf("%d ", registro->velocidade);
 
    /* A unidade de medida é impressa entre aspas, como exige o formato de
       saída da especificação. */
    if (registro->unidadeMedida == LIXO) printf("NULO\n");
    else                                 printf("\"%c\"\n", registro->unidadeMedida);
 
    return 1;
}