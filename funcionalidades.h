/*
    funcionalidades.h
    Declaração das sete funcionalidades do programa. Cada uma corresponde a uma
    opção lida no início da entrada pelo main e é responsável por ler os
    próprios parâmetros da entrada padrão e por imprimir a sua saída.
*/

#ifndef funcionalidades_h
#define funcionalidades_h

#include <stdio.h>
#include <stdlib.h>

/* 1º Cria o arquivo de dados a partir do CSV de entrada.
   Entrada: nome do arquivo CSV e nome do arquivo binário a ser gerado. */

void Funcionalidade_1();

/* 2º Lista todos os registros não removidos do arquivo de dados.
   Entrada: nome do arquivo de dados. */

void Funcionalidade_2();

/* 3º Recupera os registros que satisfazem um conjunto de critérios.
   Entrada: nome do arquivo, quantidade n de buscas e, para cada busca, a
   quantidade de campos seguida dos pares "nomeCampo" e "valor". */

void Funcionalidade_3();

/* 4º Recupera um único registro a partir do seu RRN, por acesso direto.
   Entrada: nome do arquivo de dados e o RRN desejado. */

void Funcionalidade_4();

/* 5º Remove logicamente os registros que satisfazem os critérios,
   inserindo-os na pilha de registros removidos.
   Entrada: nome do arquivo, quantidade n de remoções e, para cada remoção, a
   quantidade de campos seguida dos pares "nomeCampo" e "valor". */

void Funcionalidade_5();

/* 6º Insere novos registros, reaproveitando o espaço dos removidos.
   Entrada: nome do arquivo, quantidade n de inserções e, para cada uma, os
   quatro campos do registro na ordem da especificação. */

void Funcionalidade_6();

/* 7º Atualiza os registros que satisfazem os critérios de busca.
   Entrada: nome do arquivo, quantidade n de atualizações e, para cada uma,
   uma linha com os campos de busca e outra com os campos a alterar. */
   
void Funcionalidade_7();

#endif
