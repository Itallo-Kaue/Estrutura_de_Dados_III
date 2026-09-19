/*
    busca.c
    Implementação da leitura dos critérios de busca e da varredura sequencial
    do arquivo de dados. A comparação é feita campo a campo sobre o registro já
    lido em memória, e um registro só é considerado resposta quando satisfaz
    todos os critérios informados na entrada.
*/

#include "busca.h"
#include "fornecidas.h"
#include "registro.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAP_INICIAL 16

/* ------------------------------------------------------------------
    Leitura dos critérios                                              
   ------------------------------------------------------------------ */

/* Lê da entrada o valor de um campo numérico. A função fornecida
   ScanQuoteString não funciona, pois para um valor sem aspas ela lê o primeiro
   caractere e chama scanf("%s") para o resto, o que faz um valor de um único
   dígito ser concatenado com a palavra seguinte da entrada. Os campos
   numéricos, portanto, são lidos com %s e a palavra NULO vira string vazia. */

void LerValorNumerico(char *destino) {
    if (scanf("%63s", destino) != 1) {
        destino[0] = '\0';
        return;
    }

    if (strcmp(destino, "NULO") == 0) destino[0] = '\0';
}

/* Lê da entrada o valor correspondente ao campo informado: a unidade de
   medida vem entre aspas e é lida com a função fornecida; os demais campos
   são numéricos. */

void LerValorDoCampo(const char *nomeCampo, char *destino) {
    if (strcmp(nomeCampo, "unidadeMedida") == 0) ScanQuoteString(destino);
    else                                         LerValorNumerico(destino);
}

/* Lê os n pares "nomeCampo valor" da entrada padrão, na ordem em que
   aparecem, e devolve o vetor de critérios correspondente. */

Criterio *lerCriterios(int n) {
    if (n <= 0) return NULL;

    Criterio *crit = (Criterio *)malloc((size_t)n * sizeof(Criterio));
    if (crit == NULL) return NULL;

    for (int i = 0; i < n; i++) {
        if (scanf("%31s", crit[i].nomeCampo) != 1) {
            free(crit);
            return NULL;
        }
        LerValorDoCampo(crit[i].nomeCampo, crit[i].valor);
    }

    return crit;
}

/* ------------------------------------------------------------------ 
    Comparação de um registro com os critérios                         
   ------------------------------------------------------------------ */

/* Compara um único critério com o registro. Como não existe switch sobre
   strings em C, o nome do campo é testado com strcmp. O valor, guardado como
   texto, é convertido para inteiro ou caractere de acordo com o campo.
   Retorna 1 se o campo do registro casa com o valor procurado. */

static int campoCasa(Registro *reg, Criterio *c) {
    int nulo = (c->valor[0] == '\0');

    if (strcmp(c->nomeCampo, "idPoPs") == 0) {
        /* Campo obrigatório: nunca é nulo no arquivo. */
        if (nulo) return 0;
        return reg->idPoPs == atoi(c->valor);
    }

    if (strcmp(c->nomeCampo, "idPoPsConectado") == 0) {
        if (nulo) return 0;
        return reg->idPoPsConectado == atoi(c->valor);
    }

    if (strcmp(c->nomeCampo, "velocidade") == 0) {
        if (nulo) return reg->velocidade == INT_NULO;
        return reg->velocidade == atoi(c->valor);
    }

    if (strcmp(c->nomeCampo, "unidadeMedida") == 0) {
        if (nulo) return reg->unidadeMedida == LIXO;
        return reg->unidadeMedida == c->valor[0];
    }

    return 0;
}

/* Um registro é resposta da busca apenas quando satisfaz todos os critérios
   ao mesmo tempo. */

int registroSatisfazCriterios(Registro *reg, Criterio *crit, int n) {
    if (reg->removido == REMOVIDO) return 0;

    for (int i = 0; i < n; i++) {
        if (!campoCasa(reg, &crit[i])) return 0;
    }

    return 1;
}

/* ------------------------------------------------------------------ 
    Atualização de um campo                                            
   ------------------------------------------------------------------ */

/* Escreve o valor do critério no campo correspondente do registro. Como os
   registros têm tamanho fixo, a atualização é feita sobre o registro em
   memória e regravada no mesmo RRN por quem chamou.
   idPoPs e idPoPsConectado não podem ficar nulos, então um valor nulo para
   eles é ignorado. */

void AtualizarCampo(Registro *reg, Criterio *c) {
    int nulo = (c->valor[0] == '\0');

    if (strcmp(c->nomeCampo, "idPoPs") == 0) {
        if (!nulo) reg->idPoPs = atoi(c->valor);
        return;
    }

    if (strcmp(c->nomeCampo, "idPoPsConectado") == 0) {
        if (!nulo) reg->idPoPsConectado = atoi(c->valor);
        return;
    }

    if (strcmp(c->nomeCampo, "velocidade") == 0) {
        reg->velocidade = nulo ? INT_NULO : atoi(c->valor);
        return;
    }

    if (strcmp(c->nomeCampo, "unidadeMedida") == 0) {
        reg->unidadeMedida = nulo ? LIXO : c->valor[0];
        return;
    }

}

/* ------------------------------------------------------------------
    Varredura do arquivo de dados                                     
   ------------------------------------------------------------------ */

/* Percorre o arquivo sequencialmente, do primeiro registro até o fim, e
   guarda o RRN de cada registro que satisfaz a busca. O RRN é controlado por
   um contador porque a leitura é sequencial. */

int *buscarRRNs(FILE *bin, Criterio *crit, int n, int *qtd) {
    *qtd = 0;
    if (bin == NULL) return NULL;

    int capacidade = CAP_INICIAL;
    int *rrns = (int *)malloc((size_t)capacidade * sizeof(int));
    if (rrns == NULL) return NULL;

    /* Posiciona logo após o cabeçalho, no início do primeiro registro. */
    fseek(bin, (long)TAM_CABECALHO, SEEK_SET);

    Registro reg;
    int rrn = 0;

    while (lerRegistro(bin, &reg)) {
        if (registroSatisfazCriterios(&reg, crit, n)) {
            if (*qtd == capacidade) {
                capacidade *= 2;
                int *novo = (int *)realloc(rrns, (size_t)capacidade * sizeof(int));
                if (novo == NULL) {
                    free(rrns);
                    *qtd = 0;
                    return NULL;
                }
                rrns = novo;
            }
            rrns[*qtd] = rrn;
            (*qtd)++;
        }
        rrn++;
    }

    if (*qtd == 0) {
        free(rrns);
        return NULL;
    }

    return rrns;
}
