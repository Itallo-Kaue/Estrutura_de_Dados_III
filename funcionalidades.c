/*
    funcionalidades.c
    Implementação das funcionalidades do programa. Cada função lê da entrada
    padrão os parâmetros da sua operação, manipula o arquivo de dados por meio
    das rotinas de registro.c / busca.c / pilha.c e imprime a saída
    correspondente.
*/

#include <stdio.h>
#include <stdlib.h>

#include "busca.h"
#include "csv.h"
#include "pilha.h"
#include "registro.h"
#include "util.h"

#include "fornecidas.c"

#define TAM_NOME_ARQUIVO 64

#define MSG_FALHA "Falha no processamento do arquivo.\n"

#define MSG_INEXISTENTE "Registro inexistente.\n"

/* ------------------------------------------------------------------ 
    1º Funcionalidade: Criação                 
   ------------------------------------------------------------------ */

/* Entrada: o nome do arquivo CSV de origem e o nome do arquivo binário a ser
   criado. Toda o trabalho é feito a partir da função criarTabela, imprimindo uma mensagem
   de erro se não for possível criar a tabela. */

   void Funcionalidade_1() {
    char nomeCSV[TAM_NOME_ARQUIVO];
    char nomeBin[TAM_NOME_ARQUIVO];

    if (scanf("%63s %63s", nomeCSV, nomeBin) != 2) return;

    if (!criarTabela(nomeCSV, nomeBin)) {
        printf(MSG_FALHA);
        return;
    }

    BinarioNaTela(nomeBin);
}

/* ------------------------------------------------------------------ 
    2º Funcionalidade: Listagem                                
   ------------------------------------------------------------------ */

/* Entrada: o nome do arquivo de dados. Percorre o arquivo sequencialmente, do
   primeiro registro até o fim, e imprime os que não estão removidos. O
   arquivo é aberto apenas para leitura. */

   void Funcionalidade_2() {
    char nomeArquivo[TAM_NOME_ARQUIVO];

    if (scanf("%63s", nomeArquivo) != 1) return;

    Cabecalho cab;
    FILE *bin = abrirArquivoBinarioLeitura(nomeArquivo, &cab);
    if (bin == NULL) {
        printf(MSG_FALHA);
        return;
    }

    /* Posiciona no início do primeiro registro, logo após o cabeçalho. */
    fseek(bin, (long)TAM_CABECALHO, SEEK_SET);

    Registro reg;
    int impressos = 0;

    while (lerRegistro(bin, &reg)) {
        impressos += ImprimirRegistros(&reg);
    }

    /* Se nenhum registro for encontrado, mostra a mensagem de inexistente. */
    if (impressos == 0) printf(MSG_INEXISTENTE);

    fclose(bin);
}

/* ------------------------------------------------------------------ 
    3º Funcionalidade: Busca                                           
   ------------------------------------------------------------------ */

/* Entrada: o nome do arquivo de dados e a quantidade n de buscas. Logo após pegar
   essas entradas, é lido, para cada busca, a quantidade m de campos seguida dos
   m pares do "nomeCampo" e do "valor". Cada busca imprime os registros que satisfazem 
   todos os seus campos e é encerrada por uma linha em branco. */

void Funcionalidade_3() {
    char nomeArquivo[TAM_NOME_ARQUIVO];
    int n;

    if (scanf("%63s %d", nomeArquivo, &n) != 2) return;

    Cabecalho cab;
    FILE *bin = abrirArquivoBinarioLeitura(nomeArquivo, &cab);
    if (bin == NULL) {
        printf(MSG_FALHA);
        return;
    }

    for (int i = 0; i < n; i++) {
        int m;
        if (scanf("%d", &m) != 1) break;

        Criterio *crit = lerCriterios(m);
        if (crit == NULL) continue;

        int qtd = 0;
        int *rrns = buscarRRNs(bin, crit, m, &qtd);

        if (qtd == 0) {
            printf(MSG_INEXISTENTE);
        } else {
            for (int j = 0; j < qtd; j++) {
                Registro reg;
                if (lerRegistroPorRRN(bin, &reg, rrns[j])) ImprimirRegistros(&reg);
            }
        }

        /* Linha em branco separando o resultado de cada busca. */
        printf("\n");

        free(rrns);
        free(crit);
    }

    fclose(bin);
}

/* ------------------------------------------------------------------ 
    4º Funcionalidade: Busca                                                 
   ------------------------------------------------------------------ */

/* Entrada: o nome do arquivo de dados e o RRN do registro desejado.
   Sem varredura, o offset é calculado diretamente a partir do RRR,
   devido ao registro ter tamanho fixo. Um RRN fora do arquivo
   ou um registro logicamente removido não serão considerados. */

void Funcionalidade_4() {
    char nomeArquivo[TAM_NOME_ARQUIVO];
    int rrn;

    if (scanf("%63s %d", nomeArquivo, &rrn) != 2) return;

    Cabecalho cab;
    FILE *bin = abrirArquivoBinarioLeitura(nomeArquivo, &cab);
    if (bin == NULL) {
        printf(MSG_FALHA);
        return;
    }

    Registro reg;

    /* O RRN precisa estar dentro da faixa já usada do arquivo. */
    if (rrn < 0 || rrn >= cab.proxRRN || !lerRegistroPorRRN(bin, &reg, rrn)) {
        printf(MSG_INEXISTENTE);
    } else if (!ImprimirRegistros(&reg)) {
        /* ImprimirRegistros devolve 0 quando o registro está removido. */
        printf(MSG_INEXISTENTE);
    }

    fclose(bin);
}

/* ------------------------------------------------------------------ 
    5º Funcionalidade: Remoção                                                
   ------------------------------------------------------------------ */

/* Entrada: o nome do arquivo de dados e a quantidade n de operações de
   remoção.Logo após pegar essas entradas, é lido, para cada busca, a 
   quantidade m de campos seguida dos m pares do "nomeCampo" e do "valor".
   Todo registro que satisfizer todos os campos de uma operação é marcado como
   removido, tem seus campos de dados preenchidos com lixo e passa a ser o topo
   da pilha de registros removidos. */

void Funcionalidade_5() {
    char nomeArquivo[TAM_NOME_ARQUIVO];
    int n;

    if (scanf("%63s %d", nomeArquivo, &n) != 2) return;

    /* Abre o arquivo para leitura e escrita; o status vai para '0' enquanto
       ele estiver em uso e volta para '1' no fechamento. */
    Cabecalho cab;
    FILE *bin = abrirArquivoBinario(nomeArquivo, &cab);
    if (bin == NULL) {
        printf(MSG_FALHA);
        return;
    }

    for (int i = 0; i < n; i++) {
        int m;
        if (scanf("%d", &m) != 1) break;

        /* Lê os m pares (nomeCampo, valor) desta operação de remoção. */
        Criterio *crit = lerCriterios(m);
        if (crit == NULL) continue;

        /* Localiza todos os RRNs que satisfazem os critérios ao mesmo tempo. */
        int qtd = 0;
        int *rrns = buscarRRNs(bin, crit, m, &qtd);

        for (int j = 0; j < qtd; j++) {
            Registro reg;
            if (!lerRegistroPorRRN(bin, &reg, rrns[j])) continue;

            /* Os campos de dados são sobrescritos com lixo e regravados; em
               seguida a pilha cuida dos campos de controle, colocando o
               registro no topo e atualizando o cabeçalho. */
            preencherComLixo(&reg);
            escreverRegistroPorRRN(bin, &reg, rrns[j]);

            Empilhar(bin, &cab, rrns[j]);
        }

        /* Libera as estruturas alocadas por buscarRRNs e lerCriterios antes
           de passar para a próxima operação de remoção. */
        free(rrns);
        free(crit);
    }

    /* Regrava o cabeçalho atualizado, marca o arquivo como consistente ('1')
       e o fecha antes de imprimir o resultado. */
    fecharArquivoBinario(bin, &cab);

    BinarioNaTela(nomeArquivo);
}

/* ------------------------------------------------------------------ 
    6º Funcionalidade: Inserção                                    
   ------------------------------------------------------------------ */

/* Entrada: o nome do arquivo de dados e a quantidade n de inserções. Logo em
   seguida, é feito uma linha por registro com os quatro campos na seguinte ordem: 
   idPoPs,idPoPsConectado, velocidade e unidadeMedida (usando NULO para os campos
   nulos). A inserção reaproveita o espaço dos registros removidos: só quando a pilha
   está vazia é que o registro vai para o fim do arquivo, no proxRRN. */

void Funcionalidade_6() {
    char nomeArquivo[TAM_NOME_ARQUIVO];
    int n;

    if (scanf("%63s %d", nomeArquivo, &n) != 2) return;

    Cabecalho cab;
    FILE *bin = abrirArquivoBinario(nomeArquivo, &cab);
    if (bin == NULL) {
        printf(MSG_FALHA);
        return;
    }

    for (int i = 0; i < n; i++) {
        char valor[TAM_VALOR];

        Registro reg;
        inicializarRegistro(&reg);

        /* Os campos vêm em ordem fixa, sem o nome do campo, mas com o mesmo
           tratamento de aspas e de NULO usado na busca. */
        if (!LerValorEntrada(valor, TAM_VALOR)) break;
        reg.idPoPs = atoi(valor);

        if (!LerValorEntrada(valor, TAM_VALOR)) break;
        reg.idPoPsConectado = atoi(valor);

        if (!LerValorEntrada(valor, TAM_VALOR)) break;
        if (valor[0] != '\0') reg.velocidade = atoi(valor);

        if (!LerValorEntrada(valor, TAM_VALOR)) break;
        if (valor[0] != '\0') reg.unidadeMedida = valor[0];

        /* Tenta reusar o topo da pilha de removidos; se não houver nenhum,
           o registro é acrescentado no fim do arquivo. */
        int rrn = Desempilhar(bin, &cab);
        if (rrn == -1) {
            rrn = cab.proxRRN;
            cab.proxRRN++;
        }

        escreverRegistroPorRRN(bin, &reg, rrn);
    }

    fecharArquivoBinario(bin, &cab);

    BinarioNaTela(nomeArquivo);
}

/* ------------------------------------------------------------------ 
    7º Funcionalidade: Atualização                                        
   ------------------------------------------------------------------ */

/* Entrada: o nome do arquivo de dados e a quantidade n de atualizações; Seguinte a isso,
   para cada atualização, duas linhas no formato "nomeCampo" e "valor": a primeira com os 
   campos de busca e a segunda com os campos a serem alterados. Como os registros têm 
   tamanho fixo, a alteração é feita no próprio RRN, sem remover e reinserir. */

void Funcionalidade_7() {
    char nomeArquivo[TAM_NOME_ARQUIVO];
    int n;

    if (scanf("%63s %d", nomeArquivo, &n) != 2) return;

    Cabecalho cab;
    FILE *bin = abrirArquivoBinario(nomeArquivo, &cab);
    if (bin == NULL) {
        printf(MSG_FALHA);
        return;
    }

    for (int i = 0; i < n; i++) {
        int m;
        if (scanf("%d", &m) != 1) break;
        Criterio *busca = lerCriterios(m);

        int p;
        if (scanf("%d", &p) != 1) {
            free(busca);
            break;
        }
        Criterio *novos = lerCriterios(p);

        if (busca == NULL || novos == NULL) {
            free(busca);
            free(novos);
            continue;
        }

        int qtd = 0;
        int *rrns = buscarRRNs(bin, busca, m, &qtd);

        for (int j = 0; j < qtd; j++) {
            Registro reg;
            if (!lerRegistroPorRRN(bin, &reg, rrns[j])) continue;

            /* Aplica, na ordem em que foram lidos, todos os campos novos. */
            for (int k = 0; k < p; k++) AtualizarCampo(&reg, &novos[k]);

            escreverRegistroPorRRN(bin, &reg, rrns[j]);
        }

        free(rrns);
        free(novos);
        free(busca);
    }

    fecharArquivoBinario(bin, &cab);

    BinarioNaTela(nomeArquivo);
}
