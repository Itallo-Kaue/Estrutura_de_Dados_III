/* 
    registro.c
    Implementação da manipulação do registro de cabeçalho e dos registros de
    dados. Toda a entrada/saída é feita campo a campo.
*/

#include "registro.h"

#include <stdio.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ 
    Cabeçalho                                                         
   ------------------------------------------------------------------ */

/* Valores iniciais de um arquivo de dados recém-criado: arquivo ainda aberto
   para escrita (status '0'), pilha de removidos vazia e contadores zerados. */

void inicializarCabecalho(Cabecalho *cab) {
    cab->status = STATUS_INCONSISTENTE;
    cab->topoPilha = -1;
    cab->proxRRN = 0;
    cab->nroRegRem = 0;
    cab->nroPares = 0;
}

/* Grava os 17 bytes do cabeçalho, campo a campo, sempre a partir do byte 0. */

void escreverCabecalho(FILE *bin, Cabecalho *cab) {
    fseek(bin, 0, SEEK_SET);

    fwrite(&cab->status, sizeof(char), 1, bin);
    fwrite(&cab->topoPilha, sizeof(int), 1, bin);
    fwrite(&cab->proxRRN, sizeof(int), 1, bin);
    fwrite(&cab->nroRegRem, sizeof(int), 1, bin);
    fwrite(&cab->nroPares, sizeof(int), 1, bin);
}

/* Lê os 17 bytes do cabeçalho, campo a campo, a partir do byte 0.
   Retorna 1 se todos os campos foram lidos e 0 caso contrário. */

int lerCabecalho(FILE *bin, Cabecalho *cab) {
    fseek(bin, 0, SEEK_SET);

    if (fread(&cab->status, sizeof(char), 1, bin) != 1) return 0;
    if (fread(&cab->topoPilha, sizeof(int), 1, bin) != 1) return 0;
    if (fread(&cab->proxRRN, sizeof(int), 1, bin) != 1) return 0;
    if (fread(&cab->nroRegRem, sizeof(int), 1, bin) != 1) return 0;
    if (fread(&cab->nroPares, sizeof(int), 1, bin) != 1) return 0;

    return 1;
}

/* ------------------------------------------------------------------ 
    Registros de dados                                                
   ------------------------------------------------------------------ */

/* Byte offset do registro de RRN informado: o cabeçalho ocupa os primeiros
   17 bytes e cada registro tem 18 bytes de tamanho fixo. */

long byteOffset(int rrn) {
    return (long)TAM_CABECALHO + (long)rrn * TAM_REGISTRO;
}

/* Registro em branco: não removido, fora da pilha e com os campos de dados
   nulos, ou seja, -1 para os inteiros e '$' para a string. */

void inicializarRegistro(Registro *reg) {
    reg->removido = NAO_REMOVIDO;
    reg->encadeamentoPilha = -1;
    reg->idPoPs = INT_NULO;
    reg->idPoPsConectado = INT_NULO;
    reg->velocidade = INT_NULO;
    reg->unidadeMedida = LIXO;
}

/* Grava os 18 bytes do registro, campo a campo, na posição corrente.
   Os campos não são alterados aqui: quem chama decide se o
   registro está removido e qual é o encadeamento da pilha. */

void escreverRegistro(FILE *bin, Registro *reg) {
    fwrite(&reg->removido, sizeof(char), 1, bin);
    fwrite(&reg->encadeamentoPilha, sizeof(int), 1, bin);
    fwrite(&reg->idPoPs, sizeof(int), 1, bin);
    fwrite(&reg->idPoPsConectado, sizeof(int), 1, bin);
    fwrite(&reg->velocidade, sizeof(int), 1, bin);
    fwrite(&reg->unidadeMedida, sizeof(char), 1, bin);
}

/* Acesso direto para escrita: posiciona no byte offset do RRN e grava. */

void escreverRegistroPorRRN(FILE *bin, Registro *reg, int rrn) {
    fseek(bin, byteOffset(rrn), SEEK_SET);
    escreverRegistro(bin, reg);
}

/* Lê os 18 bytes do registro, campo a campo, na posição corrente.
   Retorna 0 quando o arquivo acabou. */

int lerRegistro(FILE *bin, Registro *reg) {
    if (fread(&reg->removido, sizeof(char), 1, bin) != 1) return 0;
    if (fread(&reg->encadeamentoPilha, sizeof(int), 1, bin) != 1) return 0;
    if (fread(&reg->idPoPs, sizeof(int), 1, bin) != 1) return 0;
    if (fread(&reg->idPoPsConectado, sizeof(int), 1, bin) != 1) return 0;
    if (fread(&reg->velocidade, sizeof(int), 1, bin) != 1) return 0;
    if (fread(&reg->unidadeMedida, sizeof(char), 1, bin) != 1) return 0;

    return 1;
}

/* Ele posiciona no byte offset do RRN e lê.
   Retorna 0 se o RRN for inválido ou estiver além do fim do arquivo. */

int lerRegistroPorRRN(FILE *bin, Registro *reg, int rrn) {
    if (rrn < 0) return 0;
    if (fseek(bin, byteOffset(rrn), SEEK_SET) != 0) return 0;

    return lerRegistro(bin, reg);
}

/* Preenche com lixo '$' todos os bytes dos campos de dados, preservando os
   campos de controle (removido e encadeamentoPilha). Como os inteiros são 
   de 4 bytes, o preenchimento é feito byte a byte sobre a representação do inteiro. */

void preencherComLixo(Registro *reg) {
    unsigned char *p;
    int i;

    p = (unsigned char *)&reg->idPoPs;
    for (i = 0; i < (int)sizeof(int); i++) p[i] = (unsigned char)LIXO;

    p = (unsigned char *)&reg->idPoPsConectado;
    for (i = 0; i < (int)sizeof(int); i++) p[i] = (unsigned char)LIXO;

    p = (unsigned char *)&reg->velocidade;
    for (i = 0; i < (int)sizeof(int); i++) p[i] = (unsigned char)LIXO;

    reg->unidadeMedida = LIXO;
}

/* ------------------------------------------------------------------ 
    Abertura e fechamento do arquivo de dados                         
   ------------------------------------------------------------------ */

/* Cria o arquivo de dados e já grava o cabeçalho inicial com status '0',
   indicando que o arquivo está aberto. */

FILE *criarArquivoBinario(const char *nomeArquivo, Cabecalho *cab) {
    FILE *bin = fopen(nomeArquivo, "wb+");
    if (bin == NULL) return NULL;

    inicializarCabecalho(cab);
    escreverCabecalho(bin, cab);

    return bin;
}

/* Abre um arquivo já existente para leitura e escrita. O cabeçalho é lido e,
   em seguida, o status é regravado como '0' enquanto o arquivo estiver em uso.
   Retorna NULL se o arquivo não existir ou estiver inconsistente. */

FILE *abrirArquivoBinario(const char *nomeArquivo, Cabecalho *cab) {
    FILE *bin = fopen(nomeArquivo, "rb+");
    if (bin == NULL) return NULL;

    if (!lerCabecalho(bin, cab) || cab->status != STATUS_CONSISTENTE) {
        fclose(bin);
        return NULL;
    }

    cab->status = STATUS_INCONSISTENTE;
    escreverCabecalho(bin, cab);

    return bin;
}

/* Abre um arquivo já existente apenas para leitura, que não alteram o arquivo 
   e, portanto, não mexem no status. */

FILE *abrirArquivoBinarioLeitura(const char *nomeArquivo, Cabecalho *cab) {
    FILE *bin = fopen(nomeArquivo, "rb");
    if (bin == NULL) return NULL;

    if (!lerCabecalho(bin, cab) || cab->status != STATUS_CONSISTENTE) {
        fclose(bin);
        return NULL;
    }

    return bin;
}

/* Regrava o cabeçalho com os contadores atualizados, marca o arquivo como
   consistente '1' e o fecha. */

void fecharArquivoBinario(FILE *bin, Cabecalho *cab) {
    if (bin == NULL) return;

    cab->status = STATUS_CONSISTENTE;
    escreverCabecalho(bin, cab);

    fclose(bin);
}
