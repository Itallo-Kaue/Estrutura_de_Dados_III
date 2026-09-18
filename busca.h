/*
    busca.h
    Leitura dos critérios de busca informados na entrada padrão e varredura do
    arquivo de dados em busca dos registros que satisfazem todos eles.
    Usado pelas funcionalidades de consulta e de remoção lógica.
*/

#ifndef BUSCA_H
#define BUSCA_H

#include <stdio.h>

#include "registro.h"

/* Tamanhos máximos do nome do campo e do valor lidos da entrada. */
#define TAM_NOME_CAMPO 32
#define TAM_VALOR      64

/* Um par (nomeCampo, valor) informado na entrada. O valor é guardado como
   texto porque os campos têm tipos diferentes (inteiro ou caractere); a
   conversão é feita na hora da comparação, de acordo com o nome do campo.
   Um valor nulo é representado por uma string vazia. */
typedef struct {
    char nomeCampo[TAM_NOME_CAMPO];
    char valor[TAM_VALOR];
} Criterio;

/* Lê n pares "nomeCampo valor" da entrada padrão e devolve um vetor alocado
   dinamicamente com eles. O vetor deve ser liberado com free por quem chamou.
   Retorna NULL em caso de falha de alocação. */
Criterio *lerCriterios(int n);

/* Retorna 1 se o registro satisfaz TODOS os n critérios e 0 caso contrário.
   Registros logicamente removidos nunca satisfazem a busca. */
int registroSatisfazCriterios(Registro *reg, Criterio *crit, int n);

/* Grava no registro, em memória, o valor de um critério, usando o nome do
   campo para saber onde escrever. Serve para a atualização de registros, que
   reaproveita o mesmo formato "nomeCampo valor" da busca.
   Campos obrigatórios não são alterados para nulo. */
void AtualizarCampo(Registro *reg, Criterio *c);

/* Varre o arquivo de dados a partir do primeiro registro e devolve um vetor
   alocado dinamicamente com os RRNs que satisfazem todos os critérios, que
   deve ser liberado com free por quem chamou.
   A quantidade de RRNs encontrados é devolvida em *qtd.
   Retorna NULL quando nenhum registro satisfaz a busca (com *qtd em 0). */
int *buscarRRNs(FILE *bin, Criterio *crit, int n, int *qtd);

#endif
