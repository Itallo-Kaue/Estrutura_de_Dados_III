/*
    csv.c
    Implementação da carga inicial do arquivo de dados a partir do arquivo
    conectaPoPs.csv. Cada linha do CSV vira um registro de tamanho fixo escrito
    campo a campo no arquivo binário, na ordem em que aparece na entrada.
*/

#include "csv.h"
#include "registro.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Delimitadores dos campos do CSV. A vírgula e o ponto e vírgula são aceitos
   porque o arquivo de entrada aparece nas duas formas. O '\r' e o '\n' entram
   na lista porque a quebra de linha pode estar no padrão Windows (CRLF) e, sem
   eles, o último campo da linha carregaria o '\r' junto. */
#define DELIM ",;\r\n"

/* Tamanho máximo de uma linha lida do CSV. */
#define TAM_LINHA 1024

/* ------------------------------------------------------------------ */
/* Carga do arquivo de dados a partir do CSV                          */
/* ------------------------------------------------------------------ */

/* Percorre o CSV de entrada linha a linha e grava um registro de dados para
   cada uma delas. O arquivo binário é criado com o cabeçalho inicial (status
   '0') e só é marcado como consistente no fechamento. Os contadores proxRRN e
   nroPares do cabeçalho são atualizados a cada registro inserido.
   Retorna 1 em caso de sucesso e 0 em caso de falha de abertura. */
int criarTabela(const char *nomeCSV, const char *nomeBin) {
    /* O CSV é apenas lido, então basta abri-lo em modo texto para leitura. */
    FILE *csv = fopen(nomeCSV, "r");
    if (csv == NULL) return 0;

    /* Cria o arquivo de dados já com o cabeçalho inicial gravado. */
    Cabecalho cab;
    FILE *bin = criarArquivoBinario(nomeBin, &cab);
    if (bin == NULL) {
        fclose(csv);
        return 0;
    }

    char linha[TAM_LINHA];

    /* A primeira linha do CSV é o cabeçalho com os nomes dos campos e não
       corresponde a nenhum registro, portanto é apenas descartada. */
    fgets(linha, sizeof(linha), csv);

    while (fgets(linha, sizeof(linha), csv) != NULL) {
        /* Quebra a linha nos quatro campos. A primeira chamada de strtok
           recebe a linha e as demais recebem NULL para continuar de onde a
           anterior parou. */
        char *pops = strtok(linha, DELIM);
        char *cone = strtok(NULL, DELIM);
        char *velo = strtok(NULL, DELIM);
        char *unid = strtok(NULL, DELIM);

        /* idPoPs e idPoPsConectado não podem ser nulos: uma linha sem esses
           campos é ignorada (linha em branco no fim do arquivo, por exemplo). */
        if (pops == NULL || cone == NULL) continue;

        /* O registro nasce com os campos de controle padrão e os campos de
           dados nulos (-1 para inteiros e '$' para a unidade de medida). */
        Registro reg;
        inicializarRegistro(&reg);

        reg.idPoPs = atoi(pops);
        reg.idPoPsConectado = atoi(cone);

        /* Campos opcionais: quando ausentes ou vazios no CSV, permanecem com
           os valores nulos definidos em inicializarRegistro. */
        if (velo != NULL && velo[0] != '\0') reg.velocidade = atoi(velo);
        if (unid != NULL && unid[0] != '\0') reg.unidadeMedida = unid[0];

        /* Grava o registro no próximo RRN disponível e atualiza os contadores
           do cabeçalho. Nesta funcionalidade não há reaproveitamento de
           espaço, então os registros são sempre inseridos no fim do arquivo. */
        escreverRegistroPorRRN(bin, &reg, cab.proxRRN);
        cab.proxRRN++;
        cab.nroPares++;
    }

    fclose(csv);

    /* Regrava o cabeçalho com os contadores finais, marca o arquivo como
       consistente ('1') e o fecha. */
    fecharArquivoBinario(bin, &cab);

    return 1;
}
