/*
    pilha.c
    Implementação da pilha de registros removidos. Todas as operações mexem
    apenas nos campos de controle (removido e encadeamentoPilha) do registro e
    nos campos topoPilha e nroRegRem do cabeçalho; os campos de dados não são
    tocados aqui.
*/

#include "pilha.h"
#include "registro.h"

#include <stdio.h>
#include <stdlib.h>

/* Os campos de controle ficam no início do registro, nesta ordem:
   removido (1 byte) e encadeamentoPilha (4 bytes). O deslocamento abaixo
   posiciona diretamente no encadeamento, sem precisar ler o registro todo. */
#define DESLOC_ENCADEAMENTO ((long)sizeof(char))

/* ------------------------------------------------------------------ */
/* Inserção na pilha                                                  */
/* ------------------------------------------------------------------ */

/* O registro removido vira o novo topo: o encadeamento dele guarda o topo
   anterior, formando a lista encadeada dentro do arquivo. Os dois campos de
   controle são gravados juntos, a partir do byte offset do RRN. */
void Empilhar(FILE *bin, Cabecalho *cab, int rrn) {
    if (bin == NULL || rrn < 0) return;

    char removido = REMOVIDO;
    int encadeamento = cab->topoPilha;

    fseek(bin, byteOffset(rrn), SEEK_SET);
    fwrite(&removido, sizeof(char), 1, bin);
    fwrite(&encadeamento, sizeof(int), 1, bin);

    /* Atualiza o cabeçalho: novo topo e mais um registro removido. */
    cab->topoPilha = rrn;
    cab->nroRegRem++;
}

/* ------------------------------------------------------------------ */
/* Remoção da pilha                                                   */
/* ------------------------------------------------------------------ */

/* Retira o registro do topo para reaproveitar o seu espaço. Basta ler o
   encadeamento do registro apontado por topoPilha: ele é o próximo topo.
   Retorna o RRN retirado, ou -1 se não houver registro removido disponível. */
int Desempilhar(FILE *bin, Cabecalho *cab) {
    if (bin == NULL || cab->topoPilha == -1) return -1;

    int rrn = cab->topoPilha;
    int proximoTopo;

    /* Posiciona no campo encadeamentoPilha do registro do topo. */
    fseek(bin, byteOffset(rrn) + DESLOC_ENCADEAMENTO, SEEK_SET);
    if (fread(&proximoTopo, sizeof(int), 1, bin) != 1) return -1;

    /* Atualiza o cabeçalho: o encadeamento lido vira o novo topo. */
    cab->topoPilha = proximoTopo;
    cab->nroRegRem--;

    return rrn;
}
